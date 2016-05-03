/*======================================================
    > File Name: MemcachedClient.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年05月03日 星期二 10时53分37秒
 =======================================================*/

#include "MemcachedClient.h"
#include "hash.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

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
    for (const Node &one : serverList) {
        int socket = createConnect(one.first.c_str(), one.second);
        for (int i = 0; i < virtualNodeNum; ++i) {
            char n[64];
            bzero(n, 64);
            sprintf(n, "%d", i);
            auto node_ip = one.first;
            node_ip.append("-");
            node_ip.append(n);
            //std::cout << "node_ip:" << node_ip << std::endl;
            connects.insert({node_ip, socket});
            nodes.insert({KETAMA_HASH(node_ip.c_str(), i), one});
        }
    }
    for (auto &node : nodes) {
        std::cout << "Hash:" << node.first << " ip:" << node.second.first << " port:" << node.second.second << std::endl;
    }
    return true;
}

int
MemcachedClient::createConnect(const char *ip, int port) {
    int sockfd;
    struct sockaddr_in server;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket() error\n");
        return -1;
    }

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server.sin_addr);

    if(connect(sockfd, (struct sockaddr*)&server, sizeof(server)) == -1) {
        perror("connect() error\n");
        return -1;
    }
    
    return sockfd;
}

bool
MemcachedClient::set(std::string key, std::string value) {
    /*
     * auto &one = nodes.upper_bound(hash(key))
     * int fd = connects[one.frist];
     * send(fd, "hello world", sizeof());
     * */
}

};
