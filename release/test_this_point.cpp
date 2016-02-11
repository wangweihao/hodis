/*======================================================
    > File Name: test_this_point.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月11日 星期四 19时53分31秒
 =======================================================*/

#include <iostream>
#include <vector>

class A{
    public:
        A();
        void func();
    public:
        A *p;
};

A::A(){
    p = this;
}

void
A::func(){
    std::cout << "hello world" << std::endl;
}


int main(){
    A a;
    (a.p)->func();
}
