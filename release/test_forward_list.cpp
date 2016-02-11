/*======================================================
    > File Name: test_forward_list.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月11日 星期四 15时12分34秒
 =======================================================*/


#include <iostream>
#include <forward_list>
#include <memory>


int main(){
    std::forward_list<std::shared_ptr<int>> slist;
    slist.push_front(std::move(std::make_shared<int>(20)));
    if(slist.empty()){
        std::cout << "list is empty" << std::endl;
    }else{
        std::cout << "list is not empty" << std::endl;
    }
//    for(int i =0; i < 10; i++){
//        slist.push_front(std::move(std::make_shared<int>(2*i)));
//    }
//    for(auto &p : slist){
//        std::cout << *p << std::endl;
//    }

    return 0;
}
