/*======================================================
    > File Name: test.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年05月03日 星期二 10时55分30秒
 =======================================================*/

#include "MemcachedClient.h"

int main() {
    Memcached::MemcachedClient memcachedClient;
    memcachedClient.setVirtualNodeNum(50);
    memcachedClient.appendServerList({{"127.0.0.1", 10000}, {"121.42.210.40", 10000}});
}
