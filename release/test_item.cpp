/*======================================================
    > File Name: test_item.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月08日 星期一 14时35分25秒
 =======================================================*/

#include "hodis_item.h"
#include <iostream>
#include <time.h>

int main()
{
    hodis::item i(100);
    time_t timep;
    time(&timep);
    printf("%ld\n", timep);   
    printf("%ld\n", sizeof(timep));
    printf("%ld\n", sizeof(hodis::item));
    printf("%ld\n", sizeof(i));

    return 0;
}
