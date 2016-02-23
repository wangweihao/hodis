/*======================================================
    > File Name: test_thread_class_func.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月23日 星期二 11时53分59秒
 =======================================================*/

#include <iostream>
#include <thread>
#include <memory>

class A{
    public:
    A(){
        ptr = std::make_unique<std::thread>(&A::func, std::ref(*this), 10);
        ptr->join();
    }
    void func(int i){
        std::cout << "function:" << i << std::endl;
    }
    private:
    std::unique_ptr<std::thread> ptr;  
};


int main(){
    A a;

    //std::thread tid(&A::func, std::ref(a), 10);
    //tid.join();
}
