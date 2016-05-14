/*======================================================
    > File Name: 1.c
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年05月09日 星期一 14时59分57秒
 =======================================================*/

#include <stdio.h>
#include "hash.h"


int main() {
    char *p1 = "127.0.0.1:10000-1";
    char *p2 = "127.0.0.1:10000-2";
    char *p3 = "127.0.0.1:10000-3";
    char *p4 = "127.0.0.1:10000-4";
    char *p5 = "127.0.0.1:10000-5";
    char *p6 = "127.0.0.1:10001-1";
    char *p7 = "127.0.0.1:10002-2";
    char *p8 = "127.0.0.1:10003-3";
    char *p9 = "127.0.0.1:10004-4";
    char *p10 = "127.0.0.1:10005-5";
    printf("%ld\n", KETAMA_HASH(p1, 1));
    printf("%ld\n", KETAMA_HASH(p2, 2));
    printf("%ld\n", KETAMA_HASH(p3, 3));
    printf("%ld\n", KETAMA_HASH(p4, 4));
    printf("%ld\n", KETAMA_HASH(p5, 5));
    printf("%ld\n", KETAMA_HASH(p6, 1));
    printf("%ld\n", KETAMA_HASH(p7, 2));
    printf("%ld\n", KETAMA_HASH(p8, 3));
    printf("%ld\n", KETAMA_HASH(p9, 4));
    printf("%ld\n", KETAMA_HASH(p10, 5));
}

