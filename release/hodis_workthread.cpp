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
workthread(int read_fd, int _id, ItemQueue _item_aq){
    notify_receive_fd = read_fd;
    id = _id;
    item_aq = _item_aq;
    worker_init();
    worker_assist_init();
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
            if(events[i].events & EPOLLIN){
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

    if((epoll_fd = epoll_create(Max_conn)) == -1){
        fprintf(stderr, "epoll_create() error");
        ret = false;
    }

    return ret;
}

bool
workthread::
worker_assist_init(){
    worker_assist = std::make_unique<std::thread>(&workthread::assist_run, std::ref(*this));
    worker_assist->detach();
}

void
workthread::
assist_run(){
    int ret;
    int len = sizeof(struct Item);
    
    Item *item = (Item*)malloc(sizeof(Item));
    if(item == nullptr){
        fprintf(stderr, "malloc() error");
        exit(1);
    }

    
//    while(1){
//        ret = read(notify_receive_fd, item, len);
//        std::cout << "----------------------" << std::endl;
//        std::cout << item->fd << std::endl;
//        std::cout << "----------------------" << std::endl;
//
//        if(ret < 0)
//        {
//            fprintf(stderr, "recv() error\n");
//            printf("%d\n", ret);
//            continue;
//        }else if(ret == len){
//            ev.events = EPOLLIN | EPOLLET;
//            ev.data.fd = item->fd;
//            if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, item->fd, &ev) == -1){
//                fprintf(stderr, "epoll_ctl() error");
//                close(item->fd);
//                continue;
//            }
//        }
//    }
    while(1){
        int fd;
        ret = read(notify_receive_fd, (char*)&fd, sizeof(fd));
        std::cout << "----------------------" << std::endl;
        std::cout << fd << std::endl;
        std::cout << "----------------------" << std::endl;

        if(ret < 0)
        {
            fprintf(stderr, "recv() error\n");
            printf("%d\n", ret);
            continue;
        }else if(ret == len){
            ev.events = EPOLLIN | EPOLLET;
            ev.data.fd = fd;
            if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1){
                fprintf(stderr, "epoll_ctl() error");
                close(fd);
                continue;
            }
        }
    }
}

};
