/*======================================================
    > File Name: test_mem_pool.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月11日 星期四 20时16分11秒
 =======================================================*/


#include "hodis_mem_pool.h"

int main(){
    hodis::mem_pool pool(1024*1024, 1024, 1024, 2);
}
