/*======================================================
    > File Name: hodis_slab.h
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月09日 星期二 21时11分02秒
 =======================================================*/

#ifndef _HODIS_SLAB_H_
#define _HODIS_SLAB_H_

/* c++ header */
#include <memory>
#include <list>
#include <atomic>
#include <mutex>
#include "hodis_item.h"

namespace hodis{

class item;

class slab{
    public:
        using ItemList = std::list<std::shared_ptr<item>>;
        using Item = std::shared_ptr<item>;

        slab(uint64_t _slab_size, uint64_t _slab_init, uint64_t _id);
        ~slab();

        slab(const slab&) = delete;
        
        auto alloc_item() -> Item;
        auto free_item(const Item &fitem)  -> void;
        auto gc_crawler() -> void;
        auto see() -> void;
        auto get_item_size() -> uint64_t;
        auto trylock() -> bool;
        auto unlock() -> void;

    private:
        /* one slab size */
        uint64_t slab_size;
        /* one item size */
        uint64_t item_size;
        /* items count per slab */
        uint64_t perslab;
        /* free item count */
        uint_fast64_t free_count;
        /* alloc item count */
        uint_fast64_t alloc_count;
        /* slab id */
        uint64_t id;
        /* this slab free item */
        ItemList freeitem;
        /* this slab alloc item 
         * use LRU when the memory is not enough */
        ItemList allocitem;
        /* slab start */
        char *start;
        /* mutex -> line mutex */
        std::mutex mutex;

};

};  /* hodis */


#endif
