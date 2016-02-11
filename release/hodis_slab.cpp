/*======================================================
    > File Name: hodis_slab.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月11日 星期四 11时08分37秒
 =======================================================*/

#include "hodis_slab.h"

namespace hodis{

slab::
slab(uint64_t _slab_size, uint64_t _slab_init, uint64_t _id):
    slab_size(_slab_size), item_size(_slab_init), id(_id)
{
    perslab = slab_size/item_size;
    free_count = perslab;
    alloc_count = 0;
    
    /* malloc slab */
    start = (char*)malloc(slab_size);
    if(start == nullptr){
        std::cout << "malloc error" << std::endl;
    }
    /* constructor object */
    for(int i = 0; i < perslab; ++i){
        std::shared_ptr<item> tpi(new (start+(i*item_size)) item(start+(i*item_size)));
        free_item.push_back(std::move(tpi));
    }
}

slab::
~slab(){
    free(start);
}

std::shared_ptr<item>
slab::alloc_item(){
    if(!free_item.empty()){
        auto item = std::move(free_item.pop_back());
        /* move in LRU list */
        alloc_item.push_back(item);
    }else{
        gc_crawler();
        if(!free_item.empty()){
            auto item = std::move(free_item.pop_back());
            alloc_item.push_back(item);
        }
    }
    return item;
}

void 
slab::alloc_slab(){

}

void 
slab::gc_crawler(){
    time_t currtime;
    time(&currtime);
    /* obtain expire time item */
    for(auto iter = alloc_item.begin(); iter != alloc_item.end(); ++iter){
        if((*iter)->get_expiretime() <= currtime){
            free_item.push_back(*iter);
            iter = alloc_item.erase(iter);
        }
    }
}

};  /* hodis */
