/*======================================================
    > File Name: test.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年05月03日 星期二 10时55分30秒
 =======================================================*/

#include "MemcachedClient.h"

/*
 * g++ test.cpp MemcachedClient.cpp -std=c++11 -lssl -lcrypto -fpermissive -o test
 * */

int main() {
    Memcached::MemcachedClient memcachedClient;
    //设置虚拟节点个数
    memcachedClient.setVirtualNodeNum(50);
    //添加服务器列表
    memcachedClient.appendServerList({{"127.0.0.1", 10000}, {"127.0.0.1", 10001}});
    
    //测试 set 命令
    std::string key = "wangweihao";
    std::string value = "21";

    std::string s1 = memcachedClient.Set(key, value, 0, 0);
    std::cout << "set:" << s1 << std::endl;

    //测试 get 命令
    std::string s2 = memcachedClient.Get(key);
    std::cout << "get:" << s2 << std::endl;

    //测试 add 命令
    std::string s3 = memcachedClient.Add(key, value, 0, 0);
    std::cout << "add:" << s3 << std::endl;

    //测试 replace 命令
    std::string s4 = memcachedClient.Replace(key, value, 0, 0);
    std::cout << "replace:" << s4 << std::endl;

    //测试 append 命令
    std::string s5 = memcachedClient.Append(key, value, 0, 0);
    std::cout << "append:" << s5 << std::endl;

    //测试 prepend 命令
    std::string s6 = memcachedClient.Prepend(key, value, 0, 0);
    std::cout << "prepend:" << s6 << std::endl;

    //测试 cas 命令
    std::string s7 = memcachedClient.Cas(key, value, 0, 0, 16);
    std::cout << "cas:" << s7 << std::endl;

    //测试 delete 命令
    std::string s8 = memcachedClient.Delete(key);
    std::cout << "delete:" << s8 << std::endl;

    //测试 version 命令
    std::string s9 = memcachedClient.Version();
    std::cout << "version:" << s9 << std::endl;

    //测试 quit 命令
    memcachedClient.Quit();
    std::cout << "quit" << std::endl;
}
