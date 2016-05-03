/*======================================================
    > File Name: MemcachedClient.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年05月03日 星期二 10时53分37秒
 =======================================================*/

#include "MemcachedClient.h"
#include "hash.h"

namespace Memcached {
    
MemcachedClient::MemcachedClient() {
    
}

MemcachedClient::~MemcachedClient() {

}

bool
MemcachedClient::setVirtualNodeNum(uint16_t _virtualNodeNum) {
    if (_virtualNodeNum <= 0 || _virtualNodeNum >= VIRTUAL_NODE_MAX) {
        virtualNodeNum = 1;
        return false;
    }else {
        virtualNodeNum = _virtualNodeNum;
        return true;
    }
}

bool
MemcachedClient::appendServerList(ServerList &&serverList) {
    for (const VirtualNode &one : serverList) {
        for (int i = 0; i < virtualNodeNum; ++i) {
            char n[64];
            bzero(n, 64);
            sprintf(n, "%d", i);
            auto node_ip = one.first;
            node_ip.append("-");
            node_ip.append(n);
            std::cout << "node_ip:" << node_ip << std::endl;
            nodes.insert({KETAMA_HASH(node_ip.c_str(), i), one});
        }
    }
    for (auto &node : nodes) {
        std::cout << "Hash:" << node.first << " ip:" << node.second.first << " port:" << node.second.second << std::endl;
    }
    return true;
}

};
