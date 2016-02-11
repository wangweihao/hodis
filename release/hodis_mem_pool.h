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
#include <algorithm>
#include "hodis_slab.h"

namespace hodis{

class mem_pool{
    public:
        mem_pool(uint64_t _slab_size, uint64_t _slab_num, uint64_t _slab_init, float _slab_incre);
        ~mem_pool();

        mem_pool(const mem_pool&) = delete;

        auto alloc_item(uint64_t size) -> std::shared_ptr<item>;
        auto free_item()  -> void;
        auto gc_crawler() -> void;

    private:
        std::vector<std::unique_ptr<slab>> slab;

        std::vector<uint64_t> search_slab;
        /* memory pool attribute */
        uint64_t slab_size;
        uint64_t slab_num;
        uint64_t slab_init;
        float slab_incre;

};

}; /* hodis */


#endif
