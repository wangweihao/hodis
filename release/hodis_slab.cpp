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
        fprintf(stderr, "Warning: Failed to allocate requested memory in"
                                    " one large chunk.\nWill allocate in smaller chunks\n");
    }
    /* constructor object */
    for(int i = 0; i < perslab; ++i){
        std::shared_ptr<item> tpi(new (start+i*item_size) item(start+(i*item_size)));
        tpi->set_slab_point(this);
        freeitem.push_back(std::move(tpi));
    }
}

slab::
~slab(){
    free(start);
}

std::shared_ptr<item>
slab::alloc_item(){
    if(!freeitem.empty()){
        auto item = *freeitem.end();
        freeitem.pop_back();
        /* move in LRU list */
        allocitem.push_back(item);
        return item;
    }else{
        /* garbage collection recover expire time item */
        gc_crawler();
        if(!freeitem.empty()){
            auto item = *freeitem.end();
            freeitem.pop_back();
            allocitem.push_back(item);
            return item;
        }
    }
    /* not useful item */
    return nullptr;
}

void
slab::free_item(const std::shared_ptr<item> &fitem){
    freeitem.push_back(fitem);
    for(auto iter = allocitem.begin(); iter != allocitem.end(); ++iter){
        if(*iter == fitem){
            //iter = allocitem.erase(fitem);
            break;
        }
    }
}



void 
slab::gc_crawler(){
    time_t currtime;
    time(&currtime);
    /* obtain expire time item */
    for(auto iter = allocitem.begin(); iter != allocitem.end(); ++iter){
        if((*iter)->get_expiretime() <= currtime){
            freeitem.push_back(*iter);
            iter = allocitem.erase(iter);
        }
    }
}


};  /* hodis */
