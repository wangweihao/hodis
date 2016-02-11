/*======================================================
    > File Name: test_reinterpret_cast.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月11日 星期四 12时04分51秒
 =======================================================*/

#include <iostream>
#include <malloc.h>

class A{
    public:
        A(int t):i(t){}
        void func();
    private:
    int i = 0;
    public:
    int j = 1;
};

void 
A::func(){
    std::cout << "hello world" << std::endl;
    std::cout << i << " " << j << std::endl;
}
int main(){
    A a(100);
    a.func();
    std::cout << sizeof(a) << std::endl;
    char *p = (char*)malloc(16);
    A *pa = new (p) A(100);
    pa->func();
    std::cout << pa->j << std::endl;
    A *pa2 = new (p+8) A(10000);
    pa2->func();
    std::cout << pa2->j << std::endl;

}
