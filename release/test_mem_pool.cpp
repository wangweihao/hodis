/*======================================================
    > File Name: test_mem_pool.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月11日 星期四 20时16分11秒
 =======================================================*/


#include "hodis_mem_pool.h"
#include <time.h>
#include <sys/time.h>

int main(){
    /* slab_size, slab_num, slab_init(item), slab_incre */
    hodis::mem_pool pool(1024*1024, 100, 100, 1.1, 1024*1024*512);
    //std::shared_ptr<item> ptr;
    struct timeval start;
    struct timeval end;
    std::cout << "--------------" << std::endl;
    gettimeofday(&start, NULL);
    for(int i =0; i < 10000; ++i){
        //pool.alloc_item(105);
        char *p = (char*)malloc(105);
        if(p == NULL){
            printf("..");
        }
    }
    gettimeofday(&end, NULL);
    std::cout << "time:" << (end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec) << std::endl;
}
