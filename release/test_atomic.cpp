/*======================================================
    > File Name: heap.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月09日 星期二 21时40分03秒
 =======================================================*/

#include <iostream>
#include <queue>
#include <functional>
#include <vector>
#include <atomic>

int main()
{
    std::priority_queue<int, std::vector<int>, std::greater<int>> q;
    q.push(1);
    std::atomic<uint_fast64_t> ai;
    ai = 0;
    if(ai == 0){
        std::cout << "hello world " << std::endl;
    }
    ai++;
    std::cout << ai << std::endl;
}
