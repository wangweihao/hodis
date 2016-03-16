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
    //std::cout << "id:" << id << " item number:" << perslab << std::endl;
    free_count = perslab;
    alloc_count = 0;
    
    /* 
     * allocate slab, if slab's item equal zero, 
     * not allocate
     * */
    start = (char*)malloc(slab_size);
    if(start == nullptr){
        fprintf(stderr, "Warning: Failed to allocate requested memory in"
                                    " one large chunk.\nWill allocate in smaller chunks\n");
    }
    /* 
     * item deleter 
     * */
    auto deleter = [](item *p){};
    /* 
     * constructor object in slab 
     * */
    for(int i = 0; i < perslab; ++i){
        item* iptr = reinterpret_cast<item*>(start+i*item_size);
        std::shared_ptr<item> tpi(new (iptr) item(start+(i*item_size)), deleter);
        tpi->set_slab_point(this);
        freeitem.push_back(std::move(tpi));
    }
}


slab::
~slab(){
    if(start != nullptr){
        free(start);
    }
}


std::shared_ptr<item>
slab::alloc_item(){
    //std::cout << "free list size:" << freeitem.size() << std::endl;
    //std::cout << "alloc list size:" << allocitem.size() << std::endl;
    //std::lock_guard<std::mutex> locker(mutex);
    if(!freeitem.empty()){
        auto item = *(--freeitem.end());
        freeitem.pop_back();
        --free_count;
        /*
         *  move in LRU list 
         *  */
        allocitem.push_back(item);
        ++alloc_count;
        return item;
    }else{
        /* 
         * garbage collection recover expire time item 
         * */
        //std::cout << "------------------gc-------------------" << std::endl;
        //gc_crawler();
        if(!freeitem.empty()){
            auto item = *(--freeitem.end());
            freeitem.pop_back();
            --free_count;
            allocitem.push_back(item);
            ++alloc_count;
            return item;
        }
    }
    /* 
     * not useful item 
     * */
    return nullptr;
}


void
slab::free_item(const std::shared_ptr<item> &fitem){
    std::lock_guard<std::mutex> locker(mutex);
    freeitem.push_back(fitem);
    ++free_count;
    for(auto iter = allocitem.begin(); iter != allocitem.end(); ++iter){
        if(*iter == fitem){
            iter = allocitem.erase(iter);
            --alloc_count;
            break;
        }
    }
}


void 
slab::gc_crawler(){
    time_t currtime;
    time(&currtime);
    /* 
     * obtain expire time item 
     *
     * */
    for(auto iter = allocitem.begin(); iter != allocitem.end(); ++iter){
        time_t expiretime = (*iter)->get_expiretime();
        if(expiretime <= currtime && expiretime != 0){
            freeitem.push_back(*iter);
            ++free_count;
            iter = allocitem.erase(iter);
            --alloc_count;
        }
    }
}

uint64_t
slab::get_item_size(){
    return item_size;
}


void
slab::see(){
    std::cout << "free item" << std::endl;
    for(auto &p : freeitem){
        std::cout << p->get_expiretime() << " ";
    }

    std::cout << "alloc item" << std::endl;
    for(auto &p : allocitem){
        std::cout << p->get_expiretime() << " ";
    }
}

bool 
slab::trylock() {
    if(mutex.try_lock() == false) {
        return false;
    }
    return true;
}

void
slab::unlock() {
    mutex.unlock();
}

};  /* hodis */
