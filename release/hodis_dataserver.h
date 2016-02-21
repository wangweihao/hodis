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
#include <iostream>
#include <vector>
#include <memory>
#include <string>

/* memory pool */
#include "hodis_mem_pool.h"

class dataserver{
    public:
        dataserver(std::ifstream &in);
        ~dataserver();

        void run();

    private:

    private:
        /* worker thread */
        std::vector<std::unique_ptr<std::thread>> thread_group;
        /* distrubute event to worker thread by counter */
        std::atomic<uint_fast64_t> counter;

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

#endif
