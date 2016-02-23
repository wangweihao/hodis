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

/* C++ header */
#include <thread>
#include <memory>
#include <iostream>

namespace hodis{

class workthread{
    public:
        workthread(int read_fd);
        ~workthread();

        void run();

    private:
        /* 
         * worker thread
         * responsible for handling the event 
         * */
        std::unique_ptr<std::thread> worker;
        
        int notify_receive_fd;
};

}; /* hodis */



#endif
