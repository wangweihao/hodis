/*======================================================
    > File Name: test_move.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月11日 星期四 09时06分51秒
 =======================================================*/

#include <iostream>
#include <memory>
#include <vector>
#include <string>

int main()
{
    std::unique_ptr<std::string> sp = std::make_unique<std::string>("hello world");
    std::cout << *sp << std::endl;
    std::vector<std::unique_ptr<std::string>> ivec;
    ivec.push_back(std::move(sp));
    for(auto &p : ivec){
        std::cout << *p << std::endl;
    }
}
