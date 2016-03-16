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

using namespace hodis;

int main(){
    /* slab_size, slab_num, slab_init(item), slab_incre */
    hodis::mem_pool pool(1024*1024, 100, 100, 1.1, 1024*1024*512);
    //std::shared_ptr<item> ptr;
    struct timeval start;
    struct timeval end;
    std::vector<std::shared_ptr<item>> ivec;
    gettimeofday(&start, NULL);
    for(int i =0; i < 1000000; ++i){
        ivec.push_back(std::move(pool.alloc_item(100)));
    }
    gettimeofday(&end, NULL);
    std::cout << "time:" << (end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec) << std::endl;
}
