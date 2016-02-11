/*======================================================
    > File Name: hodis_mem_pool.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月11日 星期四 08时49分42秒
 =======================================================*/


#include "hodis_mem_pool.h"

namespace hodis{

mem_pool::
mem_pool(uint64_t _slab_size, uint64_t _slab_num, uint64_t _slab_init, float _slab_incre):
    slab_size(_slab_size), slab_num(_slab_num), slab_init(_slab_init), slab_incre(_slab_incre)
{
    uint64_t item_size = slab_init;
    for(int i = 0; i < _slab_num; ++i){
        item_size = (uint64_t)slab_init * pow(slab_incre, i);
        std::unique_ptr<slab> one_slab = std::make_unique<slab>(slab_size, item_size, i);
        slab.push_back(std::move(one_slab));    
        search_slab.push_back(item_size);
    }
}



mem_pool::
~mem_pool(){

}

std::shared_ptr<item>
mem_pool::alloc_item(uint64_t size){
    /* search appropriate item size in slab */
    auto end = std::lower_bound(search_slab.begin(), search_slab.end(), size);
    if(end == search_slab.end()){
        /* need memory is big, malloc */
        char *p = (char*)malloc(size);
        if(p == nullptr){
            printf("malloc error\n");
        }
        return std::shared_ptr<item> sp(new (p) item);
    }else{
        auto index = end - search_slab.begin();
        return slab[index]->alloc_item();
    }
}

void
mem_pool::free_item(){

}

void
mem_pool::gc_crawler(){

}

};   /* hodis */
