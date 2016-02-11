/*======================================================
    > File Name: hodis_item.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月11日 星期四 14时38分18秒
 =======================================================*/


#include "hodis_item.h"

namespace hodis{

item::item(char* start){
    time_t timep;

    value = GET_VALUE_ADDRESS(start);
    timestamp = time(&timep);
    expiretime = 0;
}

item::~item(){

}

void
item::set_value(){

}

char*
item::get_value(){

}

void
set_expiretime(time_t time){
    expiretime = time;
}

time_t
get_expiretime(){
    return expiretime;
}

}; /* hodis */
