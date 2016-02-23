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
workthread(int read_fd){
    notify_receive_fd = read_fd;
    std::cout << "worker create success!" << std::endl;
    worker = std::make_unique<std::thread>(&workthread::run, std::ref(*this));
}

workthread::
~workthread(){

}

void
workthread::
run(){
    std::cout << "workthread start ..." << std::endl;
    char buffer[1024];
    bzero(buffer, 1024);
    while(1){
        read(notify_receive_fd, buffer, 1024);
        printf("接受到了:%s\n", buffer);
    }
}


};
