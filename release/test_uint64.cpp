/*======================================================
    > File Name: test_uint64.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月12日 星期五 09时13分22秒
 =======================================================*/

#include <iostream>

int main(){
    uint64_t i = 0;
    for(i = 0; ; i+=10000000000000){
        std::cout << i << std::endl;
    }
}
