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
        using SlabVector = std::vector<std::list<std::unique_ptr<slab>>>;
        using SlabSearch = std::vector<uint64_t>;
        
        
        
        mem_pool(uint64_t _slab_size, uint64_t _slab_num, uint64_t _slab_init, 
                float _slab_incre, uint64_t _memory_size, uint64_t _thread_number);
        ~mem_pool();

        mem_pool(const mem_pool&) = delete;
        mem_pool& operator=(const mem_pool&) = delete;

        /* alloc and free item */
        auto alloc_item(uint64_t size) -> std::shared_ptr<item>;
        auto free_item(const std::shared_ptr<item>& item)  -> void;

        /* garbage collection */
        auto gc_crawler() -> void;

    private:
        /* mem_pool manager all slab */
        SlabVector slabv;

        /* storage item size and index */
        SlabSearch search_slab;
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
