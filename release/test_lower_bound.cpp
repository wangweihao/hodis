/*======================================================
    > File Name: test_lower_bound.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月11日 星期四 14时12分08秒
 =======================================================*/

#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
    std::vector<int>ivec = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    auto a = std::lower_bound(ivec.begin(), ivec.end(), 110);
    if(a == ivec.end())
    {
        std::cout << "hello world" << std::endl;
    }
    std::cout << a-ivec.begin() << std::endl;
}
