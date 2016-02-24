/*======================================================
    > File Name: test_eventfd.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月24日 星期三 15时01分37秒
 =======================================================*/


#include <sys/eventfd.h>
#include <thread>
#include <iostream>
#include <unistd.h>

void func(int fd){
    uint64_t ev;
    printf("wait read...\n");
    read(fd, &ev, sizeof(uint64_t));
    printf("event success...\n");   
}

int main(){
    uint64_t ev = 0;
    int fd = 0;
    if((fd = eventfd(0, 0)) == -1){
        perror("eventfd() error\n");
        exit(1);
    }

    std::thread tid(func, fd);
    sleep(4);
   // ev++;
    write(fd, &ev, sizeof(uint64_t));


    tid.join();
}
