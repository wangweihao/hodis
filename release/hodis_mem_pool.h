/*======================================================
    > File Name: hodis_mem_pool.h
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月09日 星期二 21时18分55秒
 =======================================================*/

#ifndef _HODIS_MEM_POOL_H_
#define _HODIS_MEM_POOL_H_

#include <vector>
#include <list>
#include <algorithm>
#include "hodis_slab.h"

class slab;

namespace hodis{

class mem_pool{
    public:
        mem_pool(uint64_t _slab_size, uint64_t _slab_num, uint64_t _slab_init, float _slab_incre, uint64_t _memory_size);
        ~mem_pool();

        mem_pool(const mem_pool&) = delete;

        /* alloc and free item */
        auto alloc_item(uint64_t size) -> std::shared_ptr<item>;
        auto free_item(const std::shared_ptr<item>& item)  -> void;

        /* garbage collection */
        auto gc_crawler() -> void;

    private:
        /* mem_pool manager all slab */
        std::vector<std::list<std::unique_ptr<slab>>> slabv;

        /* storage item size and index */
        std::vector<uint64_t> search_slab;
        /* memory pool attribute */
        uint64_t slab_size;   
        uint64_t slab_num;
        uint64_t slab_init;
        /* useful memory size */
        uint64_t memory_size;
        float slab_incre;

};

}; /* hodis */


#endif
