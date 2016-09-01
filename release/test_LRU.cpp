/*======================================================
    > File Name: test_LRU.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年09月01日 星期四 16时45分54秒
 =======================================================*/

#include <iostream>
#include "hodis_LRU.h"

int main() {
    hodis::LRUCache lruCache(5);

    lruCache.set("1", "1");
    lruCache.set("2", "2");
    lruCache.set("3", "3");
    lruCache.set("4", "4");
    lruCache.set("5", "5");
    
    lruCache.foreach();

    lruCache.set("6", "6");
    lruCache.foreach();
    lruCache.set("5", "5");
    lruCache.foreach();

    auto s1 = lruCache.get("5");
    auto s2 = lruCache.get("10");

    std::cout << s1 << std::endl;
    std::cout << s2 << std::endl;
}
