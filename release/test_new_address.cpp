/*======================================================
    > File Name: test_new_address.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月12日 星期五 09时44分28秒
 =======================================================*/

#include <iostream>
#include <memory>
#include <malloc.h>
#include <vector>

class A{
    public:
        A(char* _p){
            p = _p;
        }
    private:
        char *p;
};


int main(){
    std::cout << sizeof(A) << std::endl;
    char*p = (char*)malloc(64);
    if(p == NULL){
        printf("error\n");
    }
    std::vector<std::shared_ptr<A>> ivec;
    auto deleter = [](A *p){};
    for(int i = 0; i < 8; ++i){
        char *tp = p+8*i;
        std::cout << "hi" << std::endl;
        printf("%p\n", tp);
        //A* ap = reinterpret_cast<A*>(tp);
        //std::shared_ptr<A> sp(new (ap) A(tp), deleter);
        std::shared_ptr<A> sp(new (tp) A(tp), deleter);
        ivec.push_back(std::move(sp));
    }
    std::cout << "hello world" << std::endl;
    free(p);
}
