/*======================================================
    > File Name: hodis_workthread.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月23日 星期二 12时17分03秒
 =======================================================*/


#include "hodis_workthread.h"

namespace hodis{

workthread::
workthread(int read_fd, int _id, ItemQueue _item_aq, ItemQueueCondition _item_aq_condition){
    notify_receive_fd = read_fd;
    id = _id;
    item_aq = _item_aq;
    item_aq_condition = _item_aq_condition;

    worker_init();
    worker = std::make_unique<std::thread>(&workthread::run, std::ref(*this));
    std::cout << "worker create success!" << std::endl;
}

workthread::
~workthread(){

}

void
workthread::
run(){
    std::cout << "workthread start ..." << std::endl;
    while(1){
        int nfds = epoll_wait(epoll_fd, events, Max_conn, -1);
        if(nfds == -1){
            fprintf(stderr, "epoll_wait() error\n");
            continue;
        }

        /* event handle */
        for(int i = 0; i < nfds; ++i){
            if(events[i].data.fd == notify_receive_fd){
                std::cout << "主线程事件" << std::endl;
                std::cout << "id:" << id << std::endl;
                handle_register_event();
            }
            else if(events[i].events & EPOLLIN){
                /* 断开连接也是 events */
                std::cout << "read events" << std::endl;
            }else if(events[i].events & EPOLLHUP){
                std::cout << "hup events" << std::endl;
            }else if(events[i].events & EPOLLERR){
                std::cout << "error events" << std::endl; 
            }
        }
    }
}

bool
workthread::
worker_init(){
    bool ret = true;
    struct epoll_event ev;

    if((epoll_fd = epoll_create(Max_conn)) == -1){
        fprintf(stderr, "epoll_create() error");
        ret = false;
    }

    ev.data.fd = notify_receive_fd;
    ev.events = EPOLLIN | EPOLLET;

    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, notify_receive_fd, &ev) == -1){
        fprintf(stderr, "epoll_ctl() error\n");
        ret = false;
    }

    return ret;
}

bool
workthread::
handle_register_event(){
    uint64_t u;
    uint16_t accumulator = 0;
    struct epoll_event ev;
    read(notify_receive_fd, &u, sizeof(uint64_t));
    printf("event:%ld\n", u);
    
    /* To remove the connection from the connection queue
     * and register event 
     * The default try 5 times */
    while(!item_aq->empty()) {
        if(++accumulator == 5) {
            break;
        }
        auto &t = item_aq->getConnect();
        ev.data.fd = t.fd;
        ev.events = EPOLLIN | EPOLLET;
        if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, t.fd, &ev) == -1) {
            fprintf(stderr, "epoll_ctl() error\n");
            return false;
        }
    }

    return true;
}

};
