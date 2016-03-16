/*======================================================
    > File Name: hodis_mem_pool.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月11日 星期四 08时49分42秒
 =======================================================*/


#include "hodis_mem_pool.h"
#include "hodis_item.h"


namespace hodis{

mem_pool::
mem_pool(uint64_t _slab_size, uint64_t _slab_num, uint64_t _slab_init, float _slab_incre, uint64_t _memory_size, uint64_t _thread_number):
    slab_size(_slab_size), slab_num(_slab_num), slab_init(_slab_init), slab_incre(_slab_incre), memory_size(_memory_size)
{
    uint64_t item_size = slab_init + sizeof(hodis::item);
    uint64_t all_alloc_mem = 0;
    for(int i = 0; i < slab_num; ++i){
        item_size = (uint64_t)slab_init * pow(slab_incre, i);
        /* 
         * acculate allocator memory size 
         * */
        all_alloc_mem += item_size;
        if(all_alloc_mem > memory_size){
            fprintf(stderr, "Error: Beyond the memory limit\n");
            exit(1);
        } 
        /* 
         * a speci size slab list 
         * */
        std::list<std::unique_ptr<slab>> slab_list;
        for(int i = 0; i < _thread_number; ++i){
            std::unique_ptr<slab> one_slab = std::make_unique<slab>(slab_size, item_size, i);
            slab_list.push_back(std::move(one_slab));
        }
        /* 
         * slab adjacent table 
         * */
        slabv.push_back(std::move(slab_list));    
        /* 
         * search_slab used appropriate slab
         * */
        search_slab.push_back(item_size);
    }
}



mem_pool::
~mem_pool(){

}

std::shared_ptr<item>
mem_pool::alloc_item(uint64_t size){
    /* 
     * search appropriate item size in slab 
     * */
    //std::cout << "alloc_item" << std::endl;
    auto end = std::lower_bound(search_slab.begin(), search_slab.end(), size);
    if(end == search_slab.end()){
        /* 
         * need memory is big, use malloc 
         * */
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
        /* 
         * this size slab list to search item 
         * */

        /* 
         * Slab quantity is greater than the number of
         * threads, success is sure to get lock allocation
         * */
        for(auto &slabs : slabv[index]){
            if(slabs->trylock() == true){
                auto one_item = slabs->alloc_item();
                slabs->unlock();
                if(one_item != nullptr){
                       return one_item;
                }
            }
        }
        std::cout << "failed" << std::endl;
        //std::cout << "alloc new slab" << std::endl;
        /* 
         * slab list not item, allocator a new slab 
         * */
        std::unique_ptr<slab> one_slab = std::make_unique<slab>(slab_size, *end, index);
        /* 
         * new slab allocate one item 
         * */
        auto ret_item = one_slab->alloc_item();
        slabv[index].push_back(std::move(one_slab));
        return ret_item;
    }
}

void
mem_pool::free_item(const std::shared_ptr<item>& item){
    hodis::slab* const item_slab = reinterpret_cast<slab* const>(item->get_slab_point());
    item_slab->see();
    if(item_slab == nullptr){
        free(item.get());
    }else{
        item_slab->free_item(item);
    }
    //item_slab->see();
    return;
}


/* 
 * A background thread for
 * memory pool garbage collection 
 * */
void
mem_pool::gc_crawler(){
    for(auto &slab_list : slabv){
        for(auto &one_slab : slab_list){
            one_slab->gc_crawler();
        }
    }
}

};   /* hodis */
