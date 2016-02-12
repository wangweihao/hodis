/*======================================================
    > File Name: test_mem_pool.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月11日 星期四 20时16分11秒
 =======================================================*/


#include "hodis_mem_pool.h"
#include <time.h>

int main(){
    /* slab_size, slab_num, slab_init(item), slab_incre */
    hodis::mem_pool pool(1024*1024, 100, 100, 1.1, 1024*1024*512);
    auto item = pool.alloc_item(105);
    std::cout << item->get_expiretime() << std::endl;
    time_t timep;
    item->set_expiretime(time(&timep));
    std::cout << item->get_expiretime() << std::endl;
    std::cout << sizeof(*item) << std::endl; 
    pool.free_item(item);
    pool.gc_crawler();
}
