/*======================================================
    > File Name: test_list.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月11日 星期四 15时37分15秒
 =======================================================*/

#include <iostream>
#include <list>

int main(){
    std::list<int> list = {1,2,3,4,5};
    for(auto iter = list.begin(); iter != list.end(); iter++){
        if(*iter %2 ==0 ){
            iter = list.erase(iter);
        }
    }
    for(auto &p : list){
        std::cout << p << std::endl;
    }
}
