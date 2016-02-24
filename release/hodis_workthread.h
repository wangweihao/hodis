/*======================================================
    > File Name: hodis_workthread.h
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月23日 星期二 12时17分04秒
 =======================================================*/


#ifndef _HODIS_WORKTHREAD_H_
#define _HODIS_WORKTHREAD_H_

/* C header */
#include <strings.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>

/* C++ header */
#include <thread>
#include <memory>
#include <iostream>
#include <list>

#define Max_conn 10000

typedef struct Item{
    int fd;
}Item;


namespace hodis{

class workthread{
    public:
        using ItemQueue = std::shared_ptr<std::list<Item>>;

        workthread(int read_fd, int _id, ItemQueue _item_aq);
        ~workthread();

        void run();
    
    private:
        bool worker_init();

    private:
        /* 
         * worker thread
         * responsible for handling the event 
         * */
        std::unique_ptr<std::thread> worker;
        /* item accept queue */
        ItemQueue item_aq;

        int id;
        int notify_receive_fd;

        /* epoll */
        int epoll_fd = 0;
        struct epoll_event events[Max_conn];
        struct epoll_event ev;
};

}; /* hodis */



#endif
