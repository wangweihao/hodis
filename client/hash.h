/*======================================================
    > File Name: hash.h
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年05月03日 星期二 11时08分31秒
 =======================================================*/

#ifndef _HASH_H_
#define _HASH_H_

/* KETAMA hastrh algorithm */
long KETAMA_HASH(const char *str, int nTime) {
    long rv = ((long) (str[3+nTime*4] & 0xff) << 24)
            | ((long) (str[2+nTime*4] & 0xff) << 16)
            | ((long) (str[1+nTime*4] & 0xff) << 8)
            | (str[0+nTime] & 0xff);
    return rv & 0xffffffffL;
}

#endif
