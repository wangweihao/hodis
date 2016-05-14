/*======================================================
    > File Name: md5.h
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年05月14日 星期六 23时56分32秒
 =======================================================*/

#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>

char* MyMD5(const char *data, char *buf) {
    unsigned char md[16];
    char tmp[3] = {'\0'};
    int i;

    MD5(data, strlen(data), reinterpret_cast<unsigned char*>(md));

    for (i = 0; i < 16; i++) {
        sprintf(tmp, "%02X", md[i]);
        strcat(buf, tmp);
    }

    return buf;
}
