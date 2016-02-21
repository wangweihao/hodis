/*======================================================
    > File Name: test_class.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月11日 星期四 20时29分07秒
 =======================================================*/

#include <iostream>

class B;
class A;

class B{
    public:
        B(int _i){
            i = _i;
        }

    private:
        int i;
};

class A{
    public:
        A():b(10) {}

    private:
        B b;
};

int main(){
    A a;
}
