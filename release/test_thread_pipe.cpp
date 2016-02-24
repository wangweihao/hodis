/*======================================================
    > File Name: test_thread_pipe.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月24日 星期三 11时18分01秒
 =======================================================*/

#include <iostream>
#include <thread>
#include <unistd.h>

struct Item{
    int fd;
    int fdd;
};

void func(int fd){
    char buf[100];
    if(read(fd, buf, 100) == -1){
        fprintf(stderr, "read() error\n");
        exit(1);
    }
    struct Item *p = (struct Item*)buf;
    printf("%d\n", p->fd);
    printf("%d\n", p->fdd);

}

int main(){
    int pipe_fd[2];
    struct Item item;
    item.fd = 1000;
    item.fdd = 10000;

    if(pipe(pipe_fd) == -1){
        fprintf(stderr, "pipe() error\n");
        exit(1);
    }

    std::thread tid(func, pipe_fd[0]);
    
    write(pipe_fd[1], (void*)(&item), sizeof(item));  
    tid.join();
}
