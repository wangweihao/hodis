/*======================================================
    > File Name: hodis_rootserver.h
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月15日 星期一 10时25分17秒
 =======================================================*/

#ifndef _HODIS_ROOT_SERVER_H_
#define _HODIS_ROOT_SERVER_H_

#include <unordered_map>
#include <string>

namespace hodis{

class rootserver{
    public:
        rootserver();
        ~rootserver();

        rootserver(const rootserver&root) = delete;

    private:
        /* object hash virtual node */
        std::unordered_map<uint32_t, uint32_t> object_virtual;
        /* virtual node hash cache node */
        std::unordered_map<std::string, std::string> virtual_cache;
}

};

#endif

