/*======================================================
    > File Name: hodis_dataserver.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月21日 星期日 20时41分01秒
 =======================================================*/


#include "hodis_dataserver.h"

namespace hodis{

dataserver::
dataserver(std::ifstream &in){
    std::map<std::string, std::string> para;
    
    analyse_parameter(in, para);
    /* master thread init */
    master_init(para);
    /* event_init */
    event_init();
    /* worker thread init */
    worker_init();

}


dataserver::
~dataserver(){

}


bool
dataserver::
master_init(std::map<std::string, std::string> &para){
    /* constructor memory pool */
    pool = std::make_unique<hodis::mem_pool>(
            strtoull(para.at("slab_size").c_str(), nullptr, 10)*1024, 
            strtoull(para.at("slab_num").c_str(), nullptr, 10),
            strtoull(para.at("slab_init").c_str(), nullptr, 10),
            strtof(para.at("slab_incre").c_str(), nullptr),
            strtoull(para.at("memory_size").c_str(), nullptr, 10)*1024*1024*1024,
            thread_num
            );
    worker_item_aq = std::make_unique<std::vector<std::shared_ptr<hodis::lockList<Item>>>>(thread_num);
    worker_item_aq_condition = std::make_unique<std::vector<std::shared_ptr<std::atomic<bool>>>>(thread_num);
    /* worker thread accept connection item queue init */
    for(int i = 0; i < thread_num; ++i){
        (*worker_item_aq)[i] = std::make_shared<hodis::lockList<Item>>();
        (*worker_item_aq_condition)[i] = std::make_shared<std::atomic<bool>>(false);
    }
}

void
dataserver::
run(){
    uint32_t revents;

    while(1){
        int nfds = epoll_wait(epoll_fd, events, Max_conn, -1);
        if(nfds == -1){
            fprintf(stderr, "epoll_wait() error");
            continue;
        }

        for(int i = 0; i < nfds; ++i){
            revents = events[i].events;

            if(revents & (EPOLLERR | EPOLLHUP)){
                std::cout << "收到 RST 报文。可能还有数据未读取" << std::endl;
            }

            if(events[i].data.fd == listen_fd){
                std::cout << "加入连接" << std::endl;
                int con_fd = accept_connect();
                if(con_fd == -1){
                    fprintf(stderr, "accept_connect() error\n");
                    continue;
                }else{
                    /* accept a new connection, register worker thread */
                    register_worker(con_fd);
                }
            }else{
                std::cout << "特殊情况" << std::endl;
            }
        }
    }
}


bool 
dataserver::
event_init(){
    int reuse_addr = 1;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_port = htons(port);
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr);

    if((listen_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0){
        fprintf(stderr, "socket() error!");
        exit(1);
    }

    if(setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, (void*)&reuse_addr, sizeof(int)) != 0){
        fprintf(stderr, "setsockopt() error");
        exit(1);
    }

    if(bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1){
        fprintf(stderr, "bind() error!");
        exit(1);
    }

    if(listen(listen_fd, BackLog) == -1){
        fprintf(stderr, "listen() error!");
        exit(1);
    }

    /* create epoll event */
    if((epoll_fd = epoll_create(Max_conn)) == -1){
        fprintf(stderr, "epoll_create() error!");
        exit(1);
    }
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = listen_fd;

    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev) == -1){
        fprintf(stderr, "epoll_ctl() error");
        exit(1);
    }

    setnonblocking(listen_fd);
}

int 
dataserver::
setnonblocking(int fd){
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

bool
dataserver::
analyse_parameter(std::ifstream &in, std::map<std::string, std::string> &para){
    std::string line;
    std::string name;
    std::string value;

    while(getline(in, line)){
        std::istringstream is(line);
        is >> name;
        if(name.substr(0, 2) == "//"){
            continue;
        }
        is >> value;
        para.insert(std::make_pair(name, value));
    }
    for(auto &p : para){
        std::cout << p.first << " " << p.second << std::endl;
    }


    ip = para["dataserver_ip"];
    port = atoi(para["dataserver_port"].c_str());
    thread_num = atoi(para["work_thread_num"].c_str());
}

int
dataserver::
accept_connect(){
    struct sockaddr_in client_addr;
    socklen_t addrlen = sizeof(struct sockaddr_in);
    int connect_fd = 0;

    bzero(&client_addr, sizeof(client_addr));

    if((connect_fd = accept4(listen_fd, (struct sockaddr*)&client_addr, &addrlen, SOCK_NONBLOCK)) == -1){
        fprintf(stderr, "accept4() error");
        return -1;
    }else{
        return connect_fd;
    }

}

bool
dataserver::
register_worker(int fd){
    /* obtain current worker thread */
    Item item;
    auto index = counter.load() % thread_num;
    unsigned int size = sizeof(uint64_t);
    
    ++counter;
    item.fd = fd;
    /* false insert first queue
     * true insert second queue */
    std::cout << "join... fd:" << item.fd << std::endl;
    ((*worker_item_aq)[index])->putConnect(item);
    ++event[index];
    if(size != write(eventfds[index], &event[index], size)){
        fprintf(stderr, "write() error\n");
        return false;
    }
    
    return true;
}


/*
 * Create work thread
 * Establish the pipeline with the main thread
 * */
bool
dataserver::
worker_init(){
    for(int i = 0; i < thread_num; ++i){
        int fd;
        uint64_t ev = 0;
        if((fd = eventfd(0, 0)) == -1){
            fprintf(stderr, "eventfd() error\n");
            exit(1);
        }
        eventfds.push_back(fd);
        event.push_back(ev);
        auto one_worker = std::make_unique<hodis::workthread>(
                fd, i, (*worker_item_aq)[i], (*worker_item_aq_condition)[i]);
        work_thread_group.push_back(std::move(one_worker));
    }
}

};  /* hodis */
