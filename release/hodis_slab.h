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
#include "hodis_item.h"

namespace hodis{

class item;

class slab{
    public:
        slab(uint64_t _slab_size, uint64_t _slab_init, uint64_t _id);
        ~slab();

        slab(const slab&) = delete;
        auto alloc_item() -> std::shared_ptr<item>;
        auto free_item(const std::shared_ptr<item> &fitem)  -> void;
        auto gc_crawler() -> void;

    private:
        /* one slab size */
        uint64_t slab_size;
        /* one item size */
        uint64_t item_size;
        /* items count per slab */
        uint64_t perslab;
        /* free item count */
        uint64_t free_count;
        /* alloc item count */
        uint64_t alloc_count;
        /* slab id */
        uint64_t id;
        /* this slab free item */
        std::list<std::shared_ptr<item>> freeitem;
        /* this slab alloc item 
         * use LRU when the memory is not enough */
        std::list<std::shared_ptr<item>> allocitem;
        /* slab start */
        char *start;

};

};  /* hodis */


#endif
