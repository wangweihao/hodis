/*======================================================
    > File Name: test_dataserver.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月22日 星期一 11时35分50秒
 =======================================================*/


#include "hodis_dataserver.h"
#include <fstream>

int main(){
    std::ifstream in("./hodis.conf");

    hodis::dataserver dataserver(in);
    dataserver.run();

    return 0;
}
