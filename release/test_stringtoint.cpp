/*======================================================
    > File Name: test_stringtoint.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月22日 星期一 12时20分39秒
 =======================================================*/

#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <string>


int main(){
    std::string value("1231231123123");
    uint64_t i;
    char *end;
    i = strtoull(value.c_str(), nullptr, 10);
    std::cout << i+100 << std::endl;
}
