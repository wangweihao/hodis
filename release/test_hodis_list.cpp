/*======================================================
    > File Name: test_hodis_list.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月16日 星期三 21时35分02秒
 =======================================================*/

#include "hodis_list.h"
#include <vector>
#include <memory>
#include <thread>
#include <iostream>

void func(hodis::lockList<int> &list_) {
    list_.putConnect(10);
    list_.putConnect(20);
    list_.putConnect(20);
    list_.putConnect(20);
    list_.putConnect(20);
    int fd = list_.getConnect();
    std::cout << fd << std::endl;
}

int main() {
    hodis::lockList<int> list_;
    std::vector<std::shared_ptr<std::thread>> ivec;

    for(int i = 0; i < 16; ++i){
        auto p = std::make_shared<std::thread>(func, std::ref(list_));
        ivec.push_back(std::move(p));
    }

    for(auto &p : ivec) {
        p->join();
    }
    std::cout << "size:" << list_.getSize() << std::endl;
    return 0;
}
