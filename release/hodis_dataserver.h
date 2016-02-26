/*======================================================
    > File Name: hodis_dataserver.h
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月21日 星期日 20时24分33秒
 =======================================================*/


#ifndef _HODIS_DATASERVER_H_
#define _HODIS_DATASERVER_H_

/* c header */
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <sys/eventfd.h>

/* c++ header */
#include <cstdio>
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <thread>
#include <fstream>
#include <sstream>
#include <map>
#include <list>

/* memory pool */
#include "hodis_mem_pool.h"
/* worker thread */
#include "hodis_workthread.h"

#define Max_conn 10000
#define BackLog  1024

namespace hodis{

class dataserver{
    public:
        using ParaMap = std::map<std::string, std::string>;
        using WorkerThreadGroup = std::vector<std::unique_ptr<hodis::workthread>>;
        using WorkerItemAQ = std::unique_ptr<std::vector<std::shared_ptr<
            std::pair<std::list<Item>, std::list<Item>>>>>;
        using WorkerAQCondition = std::unique_ptr<std::vector<std::shared_ptr<
            std::atomic<bool>>>>;
        using WorkerEvent = std::vector<uint64_t>;
        using WorkerEventFd = std::vector<int>;

        dataserver() = delete;

        dataserver(std::ifstream &in);
        ~dataserver();

        void run();

    private:
        int setnonblocking(int fd);
        bool event_init();
        bool worker_init();
        bool master_init(ParaMap &para);
        bool analyse_parameter(std::ifstream &in, ParaMap &para);
        bool register_worker(int fd);
        int accept_connect();


    private:
        /* worker thread num */
        int thread_num;
        /* worker thread */
        WorkerThreadGroup work_thread_group;
        /* worker thread write event fd */
        WorkerEvent event;
        WorkerEventFd eventfds;
        /* worker thread accept connection item queue */
        WorkerItemAQ worker_item_aq;
        /* false insert first queue, true insert second queue */
        WorkerAQCondition worker_item_aq_condition;
        /* distrubute event to worker thread by counter */
        std::atomic<uint_fast64_t> counter;

        /* log thread 
         * redo log
         * undo log*/
        std::unique_ptr<std::thread> log_thread;

        /* mem_pool */
        std::unique_ptr<hodis::mem_pool> pool;

        /* port and ip */
        std::string ip;
        int port;

        /* socket config */
        int listen_fd = 0;
        int epoll_fd = 0;

        struct sockaddr_in server_addr;
        struct epoll_event events[Max_conn];
        struct epoll_event ev;

};

};  /* hodis */

#endif
