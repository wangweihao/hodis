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
    //设置虚拟节点个数
    memcachedClient.setVirtualNodeNum(50);
    //添加服务器列表
    memcachedClient.appendServerList({{"127.0.0.1", 10000}, {"127.0.0.1", 10001}});
    
    //测试 set 命令
    std::string key = "wangweihao";
    std::string value = "21";

    std::string s = memcachedClient.Set(key, value, 0, 0);
    std::cout << "s:" << s << std::endl;
    if (s == "STORED\r\n") {
        std::cout << "hello world" << std::endl;
    }else {
        std::cout << "no " << std::endl;
    }
}
