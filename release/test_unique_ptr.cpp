/*======================================================
    > File Name: test_unique_ptr.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月09日 星期二 23时03分16秒
 =======================================================*/

#include <iostream>
#include <memory>
#include <string>

class A{
    public:
    A(){
        std::cout << "constructor" << std::endl;
    }
    ~A(){
        std::cout << "destructor" << std::endl;
    }
    void func()
    {
        std::cout << "hello world" << std::endl;
    }
    private:
    int i;
};

std::unique_ptr<std::string>
alloc_string()
{
    std::unique_ptr<std::string> sp = std::make_unique<std::string>("hello world");
    return sp;
}

int main()
{
    //std::unique_ptr<A> sp = std::make_unique<A>();
    //std::unique_ptr<A> spp = std::move(sp);
    //spp->func();
    std::unique_ptr<std::string> sp = std::move(alloc_string());
    std::cout << *sp << std::endl;
}
