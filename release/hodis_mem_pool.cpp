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
    for(int i = 0; i < slab_num; ++i){
        item_size = (uint64_t)slab_init * pow(slab_incre, i);
        std::unique_ptr<slab> one_slab = std::make_unique<slab>(slab_size, item_size, i);
        /* one size slab list */
        std::list<std::unique_ptr<slab>> slab_list;
        /* slab adjacent table */
        slab_list.push_back(std::move(one_slab));
        slabv.push_back(std::move(slab_list));    
        /* search_slab used appropriate slab */
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
        /* need memory is big, use malloc */
        char *p = (char*)malloc(size);
        if(p == nullptr){
            fprintf(stderr, "Warning: Failed to allocate requested memory in"
                                        " one large chunk.\nWill allocate in smaller chunks\n");
            exit(1);
        } 
        std::shared_ptr<item> sp(new (p) item(p));
        return sp;
    }else{
        auto index = end - search_slab.begin();
        /* this size slab list to search item */
        for(auto &slabs : slabv[index]){
            auto one_item = slabs->alloc_item();
            if(one_item != nullptr){
                   return one_item;
            }
        }
        /* slab list not item, allocator a new slab */
        std::unique_ptr<slab> one_slab = std::make_unique<slab>(slab_size, slab_size/slab_num, index);
        /* new slab allocate one item */
        auto ret_item = one_slab->alloc_item();
        slabv[index].push_back(std::move(one_slab));
        return ret_item;
    }
}

void
mem_pool::free_item(const std::shared_ptr<item>& item){
//    auto slab = (slab*)item->get_slab_point();
//    if(slab == nullptr){
//        free(item->get());
//    }else{
//        slab->free_item(item);
//    }
    return;
}


/* memory pool garbage collection */
void
mem_pool::gc_crawler(){
    for(auto &slab_list : slabv){
        for(auto &one_slab : slab_list){
            one_slab->gc_crawler();
        }
    }
}

};   /* hodis */
