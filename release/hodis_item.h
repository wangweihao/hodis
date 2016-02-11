/*======================================================
    > File Name: hodis_item.h
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月08日 星期一 11时14分07秒
 =======================================================*/


#ifndef _HODIS_ITEM_H_
#define _HODIS_ITEM_H_

/*c++ header*/
#include <atomic>
#include <vector>
#include <memory>
#include <iostream>
#include <time.h>

#define ITEM_VALUE 32
#define GET_VALUE_ADDRESS(start) ((start)+(ITEM_VALUE))

namespace hodis{

class item{
    public:
        item(char* start);
        ~item();

        void set_value();
        void set_expiretime(time_t time);
        char* get_value();
        time_t get_expiretime();

    private:
        /* version number */
        std::atomic<uint_fast64_t> version;
        /* timestamp LRU */
        time_t timestamp;
        /* expire time */
        time_t expiretime;
        /* index */
        uint64_t index;
        /* item space key+date */
        void* value;
};

}; /* hodis */


#endif
