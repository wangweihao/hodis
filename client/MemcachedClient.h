/*======================================================
    > File Name: MemcachedClient.h
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年05月01日 星期日 16时29分14秒
 =======================================================*/

#ifndef _MEMCACHEDCLIENT_H_
#define _MEMCACHEDCLIENT_H_

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <initializer_list>

#include <stdint.h>
#include <strings.h>
#include <stdio.h>

#define VIRTUAL_NODE_MAX 1000

namespace Memcached {

class MemcachedClient {
    public:
        /* std::pair<ip, port> */
        using Node = std::pair<std::string, uint16_t>;
        using VirtualNode = std::pair<std::string, uint16_t>;
        using ServerList = std::initializer_list<VirtualNode>;
        
        MemcachedClient();
        ~MemcachedClient();

        bool setVirtualNodeNum(uint16_t _virtualNodeNum);
        /* {{"127.0.0.1", 10000}, {"192.168.0.1", 10000}} */
        bool appendServerList(ServerList &&serverList);

        std::string Set(std::string &key, std::string &value, uint64_t time, int flags);
        std::string Add(std::string &key, std::string &value, uint64_t time, int flags);
        std::string Replace(std::string &key, std::string &value, uint64_t time, int flags);
        std::string Append(std::string &key, std::string &value, uint64_t time, int flags);
        std::string Prepend(std::string &key, std::string &value, uint64_t time, int flags);
        std::string Cas(std::string &key, std::string &value, uint64_t time, int flags, uint64_t version);
        std::string Get(std::string &key);
        std::string Delete(std::string &key);
        std::string Version();
        std::string Quit();


    private:
        int createConnect(const char *ip, int port);

    private:
        /* virtual node number */
        uint16_t virtualNodeNum;
        /* server socket */
        std::map<std::string, int> connects; 
        std::map<long, std::string> nodes;
};

}  /* memcached */

#endif
