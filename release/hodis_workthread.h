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
#include <atomic>

#define Max_conn 10000

typedef struct Item{
    int fd;
}Item;


namespace hodis{

class workthread{
    public:
        using ItemQueue = std::shared_ptr<std::pair<std::list<Item>, std::list<Item>>>;
        using ItemQueueCondition = std::shared_ptr<std::atomic<bool>>;

        workthread(int read_fd, int _id, ItemQueue _item_aq, ItemQueueCondition _item_aq_condition);
        ~workthread();

        void run();
    
    private:
        bool worker_init();
        bool handle_register_event();

    private:
        /* 
         * worker thread
         * responsible for handling the event 
         * */
        std::unique_ptr<std::thread> worker;
        /* item accept queue */
        ItemQueue item_aq;
        ItemQueueCondition item_aq_condition;

        int id;
        int notify_receive_fd;

        /* epoll */
        int epoll_fd = 0;
        struct epoll_event events[Max_conn];
        struct epoll_event ev;
};

}; /* hodis */



#endif
