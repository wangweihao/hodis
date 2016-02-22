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

/* memory pool */
#include "hodis_mem_pool.h"

#define Max_conn 10000
#define BackLog  1024

namespace hodis{

class dataserver{
    public:
        dataserver() = delete;
        dataserver(std::ifstream &in);
        ~dataserver();


        void run();

    private:
        int setnonblocking(int fd);
        bool event_init();
        bool worker_init();
        bool analyse_parameter(std::ifstream &in, std::map<std::string, std::string> &para);
        bool register_worker(int fd);
        int accept_connect();

    private:
        /* worker thread num */
        int thread_num;
        /* worker thread */
        std::vector<std::unique_ptr<std::thread>> work_thread_group;
        /* distrubute event to worker thread by counter */
        std::atomic<uint_fast64_t> counter;

        /* log thread 
         * redo log
         * undo log*/
        std::unique_ptr<std::thread> log_thread;

        /* mem_pool */
        std::unique_ptr<hodis::mem_pool> pool;

        /* port and is */
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
