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
    /* constructor memory pool */
    pool = std::make_unique<hodis::mem_pool>(
            strtoull(para.at("slab_size").c_str(), nullptr, 10)*1024, 
            strtoull(para.at("slab_num").c_str(), nullptr, 10),
            strtoull(para.at("slab_init").c_str(), nullptr, 10),
            strtof(para.at("slab_incre").c_str(), nullptr),
            strtoull(para.at("memory_size").c_str(), nullptr, 10)*1024*1024*1024
            );

}


dataserver::
~dataserver(){

}


void
dataserver::
run(){

}


bool 
dataserver::
event_init(){
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_port = htons(port);
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr);

    if((listen_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0){
        fprintf(stderr, "socket() error!");
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

};  /* hodis */
