/*======================================================
    > File Name: test_kfifo.c
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月13日 星期日 14时37分05秒
 =======================================================*/

#include <stdio.h>
#include "hodis_kfifo.h"

int main(){
    unsigned int i = 4097;
    unsigned int j = roundup_pow_of_two(i);
    printf("%lu\n", j);
}
