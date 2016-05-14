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
#include <math.h>
#include "md5.h"

#include <functional>

#define HASH KETAMA_HASH

namespace Memcached {
    
MemcachedClient::MemcachedClient() {}

MemcachedClient::~MemcachedClient() {}

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
        char port[64];
        bzero(port, 64);
        sprintf(port, "%d", one.second);
        int socket = createConnect(one.first.c_str(), one.second);
        for (int i = 0; i < virtualNodeNum/4; ++i) {
            //组装获取虚拟节点 ip
            char n[64];
            bzero(n, 64);
            sprintf(n, "%d", i);
            auto node_ip = one.first;
            node_ip.append(":");
            node_ip.append(port);
            node_ip.append("-");
            node_ip.append(n);
            std::cout << "node_ip:" << node_ip << std::endl;
            //获取对应虚拟节点的 MD5
            char buf[33] = {'\0'};
            MyMD5(node_ip.c_str(), buf);
            //获取虚拟节点的 hash 值
            for (int h = 0; h < 4; ++h) {
                long k = HASH(buf, h);
                //存储到 ip->socket map
                connects.insert({node_ip, socket});
                //存储到 hashValue->VirtualNode map
                auto ret = nodes.insert({k, node_ip});
                if (ret.second == false) {
                    std::cout << "insert false" << std::endl;
                }
            }
        }
    }
    std::cout << "size:" << nodes.size() << std::endl;
    for (auto &node : nodes) {
        std::cout << "Hash:" << node.first << " ip:" << node.second << std::endl;
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

std::string
MemcachedClient::Set(std::string &key, std::string &value, uint64_t time, int flags) {
    std::hash<std::string> Hash;
    char n[64];
    char buffer[256];
    bzero(n, 64);
    bzero(buffer, 64);
    //获得虚拟节点
    //hash = HASH(key);
    //需要找到一个hash函数来hash key
    auto one = nodes.upper_bound(Hash(key)%(uint64_t)(pow(2,32)-1));
    //如果没找到，则默认归入第一个虚拟节点
    if (one == nodes.end()) {
        std::cout << "not found" << std::endl;
        one = nodes.begin();
    }
    //获得节点对应的socket
    int fd = connects[one->second];
    std::cout << "fd:" << fd << std::endl;
    std::string newKey = "set ";
    newKey.append(key);
    newKey.append(" ");
    bzero(n, 64);
    sprintf(n, "%lu", time);
    newKey.append(n);
    newKey.append(" ");
    bzero(n, 64);
    sprintf(n, "%d", flags);
    newKey.append(n);
    newKey.append(" ");
    bzero(n, 64);
    sprintf(n, "%lu", value.length());
    newKey.append(n);
    newKey.append("\r\n");
    value.append("\r\n");
    
    //发送给服务器
    send(fd, newKey.c_str(), newKey.length(), 0);
    send(fd, value.c_str(), value.length(), 0);

    recv(fd, buffer, 256, 0);
    printf("buffer:%s\n", buffer);
    std::string ret(buffer);

    return ret;
}

std::string 
MemcachedClient::Add(std::string &key, std::string &value, uint64_t time, int flags) {
    std::hash<std::string> Hash;
    char n[64];
    char buffer[256];
    bzero(n, 64);
    bzero(buffer, 64);
    //获得虚拟节点
    //hash = HASH(key);
    //需要找到一个hash函数来hash key
    auto one = nodes.upper_bound(Hash(key)%(uint64_t)(pow(2,32)-1));
    //如果没找到，则默认归入第一个虚拟节点
    if (one == nodes.end()) {
        std::cout << "not found" << std::endl;
        one = nodes.begin();
    }
    //获得节点对应的socket
    int fd = connects[one->second];
    std::cout << "fd:" << fd << std::endl;
    std::string newKey = "add ";
    newKey.append(key);
    newKey.append(" ");
    bzero(n, 64);
    sprintf(n, "%lu", time);
    newKey.append(n);
    newKey.append(" ");
    bzero(n, 64);
    sprintf(n, "%d", flags);
    newKey.append(n);
    newKey.append(" ");
    bzero(n, 64);
    sprintf(n, "%lu", value.length());
    newKey.append(n);
    newKey.append("\r\n");
    value.append("\r\n");
    
    //发送给服务器
    send(fd, newKey.c_str(), newKey.length(), 0);
    send(fd, value.c_str(), value.length(), 0);

    recv(fd, buffer, 256, 0);
    printf("buffer:%s\n", buffer);
    std::string ret(buffer);

    return ret;
}

std::string 
MemcachedClient::Replace(std::string &key, std::string &value, uint64_t time, int flags) {
    std::hash<std::string> Hash;
    char n[64];
    char buffer[256];
    bzero(n, 64);
    bzero(buffer, 64);
    //获得虚拟节点
    //hash = HASH(key);
    //需要找到一个hash函数来hash key
    auto one = nodes.upper_bound(Hash(key)%(uint64_t)(pow(2,32)-1));
    //如果没找到，则默认归入第一个虚拟节点
    if (one == nodes.end()) {
        std::cout << "not found" << std::endl;
        one = nodes.begin();
    }
    //获得节点对应的socket
    int fd = connects[one->second];
    std::cout << "fd:" << fd << std::endl;
    std::string newKey = "replace ";
    newKey.append(key);
    newKey.append(" ");
    bzero(n, 64);
    sprintf(n, "%lu", time);
    newKey.append(n);
    newKey.append(" ");
    bzero(n, 64);
    sprintf(n, "%d", flags);
    newKey.append(n);
    newKey.append(" ");
    bzero(n, 64);
    sprintf(n, "%lu", value.length());
    newKey.append(n);
    newKey.append("\r\n");
    value.append("\r\n");
    
    //发送给服务器
    send(fd, newKey.c_str(), newKey.length(), 0);
    send(fd, value.c_str(), value.length(), 0);

    recv(fd, buffer, 256, 0);
    printf("buffer:%s\n", buffer);
    std::string ret(buffer);

    return ret;
}

std::string
MemcachedClient::Append(std::string &key, std::string &value, uint64_t time, int flags) {

    std::hash<std::string> Hash;
    char n[64];
    char buffer[256];
    bzero(n, 64);
    bzero(buffer, 64);
    //获得虚拟节点
    //hash = HASH(key);
    //需要找到一个hash函数来hash key
    auto one = nodes.upper_bound(Hash(key)%(uint64_t)(pow(2,32)-1));
    //如果没找到，则默认归入第一个虚拟节点
    if (one == nodes.end()) {
        std::cout << "not found" << std::endl;
        one = nodes.begin();
    }
    //获得节点对应的socket
    int fd = connects[one->second];
    std::cout << "fd:" << fd << std::endl;
    std::string newKey = "append ";
    newKey.append(key);
    newKey.append(" ");
    bzero(n, 64);
    sprintf(n, "%lu", time);
    newKey.append(n);
    newKey.append(" ");
    bzero(n, 64);
    sprintf(n, "%d", flags);
    newKey.append(n);
    newKey.append(" ");
    bzero(n, 64);
    sprintf(n, "%lu", value.length());
    newKey.append(n);
    newKey.append("\r\n");
    value.append("\r\n");
    
    //发送给服务器
    send(fd, newKey.c_str(), newKey.length(), 0);
    send(fd, value.c_str(), value.length(), 0);

    recv(fd, buffer, 256, 0);
    printf("buffer:%s\n", buffer);
    std::string ret(buffer);

    return ret;
}

std::string
MemcachedClient::Prepend(std::string &key, std::string &value, uint64_t time, int flags) {
    std::hash<std::string> Hash;
    char n[64];
    char buffer[256];
    bzero(n, 64);
    bzero(buffer, 64);
    //获得虚拟节点
    //hash = HASH(key);
    //需要找到一个hash函数来hash key
    auto one = nodes.upper_bound(Hash(key)%(uint64_t)(pow(2,32)-1));
    //如果没找到，则默认归入第一个虚拟节点
    if (one == nodes.end()) {
        std::cout << "not found" << std::endl;
        one = nodes.begin();
    }
    //获得节点对应的socket
    int fd = connects[one->second];
    std::cout << "fd:" << fd << std::endl;
    std::string newKey = "prepend ";
    newKey.append(key);
    newKey.append(" ");
    bzero(n, 64);
    sprintf(n, "%lu", time);
    newKey.append(n);
    newKey.append(" ");
    bzero(n, 64);
    sprintf(n, "%d", flags);
    newKey.append(n);
    newKey.append(" ");
    bzero(n, 64);
    sprintf(n, "%lu", value.length());
    newKey.append(n);
    newKey.append("\r\n");
    value.append("\r\n");
    
    //发送给服务器
    send(fd, newKey.c_str(), newKey.length(), 0);
    send(fd, value.c_str(), value.length(), 0);

    recv(fd, buffer, 256, 0);
    printf("buffer:%s\n", buffer);
    std::string ret(buffer);

    return ret;
}

std::string 
MemcachedClient::Cas(std::string &key, std::string &value, uint64_t time, int flags, uint64_t version) {
    std::hash<std::string> Hash;
    char n[64];
    char buffer[256];
    bzero(n, 64);
    bzero(buffer, 64);
    //获得虚拟节点
    //hash = HASH(key);
    //需要找到一个hash函数来hash key
    auto one = nodes.upper_bound(Hash(key)%(uint64_t)(pow(2,32)-1));
    //如果没找到，则默认归入第一个虚拟节点
    if (one == nodes.end()) {
        std::cout << "not found" << std::endl;
        one = nodes.begin();
    }
    //获得节点对应的socket
    int fd = connects[one->second];
    std::cout << "fd:" << fd << std::endl;
    std::string newKey = "cas ";
    newKey.append(key);
    newKey.append(" ");
    bzero(n, 64);
    sprintf(n, "%lu", time);
    newKey.append(n);
    newKey.append(" ");
    bzero(n, 64);
    sprintf(n, "%d", flags);
    newKey.append(n);
    newKey.append(" ");
    bzero(n, 64);
    sprintf(n, "%lu", value.length());
    newKey.append(n);
    newKey.append(" ");
    bzero(n, 64);
    sprintf(n, "%lu", version);
    newKey.append(n);
    newKey.append("\r\n");
    value.append("\r\n");
    
    //发送给服务器
    send(fd, newKey.c_str(), newKey.length(), 0);
    send(fd, value.c_str(), value.length(), 0);

    recv(fd, buffer, 256, 0);
    printf("buffer:%s\n", buffer);
    std::string ret(buffer);

    return ret;
}

std::string 
MemcachedClient::Get(std::string &key) {
    std::hash<std::string> Hash;
    char buffer[256];
    bzero(buffer, 256);
    //获得虚拟节点
    //hash = HASH(key);
    //需要找到一个hash函数来hash key
    auto one = nodes.upper_bound(Hash(key)%(uint64_t)(pow(2,32)-1));
    //如果没找到，则默认归入第一个虚拟节点
    if (one == nodes.end()) {
        std::cout << "not found" << std::endl;
        one = nodes.begin();
    }
    //获得节点对应的socket
    int fd = connects[one->second];
    std::cout << "fd:" << fd << std::endl;
    std::string newKey = "get ";
    newKey.append(key);
    newKey.append("\r\n");
    
    //发送给服务器
    send(fd, newKey.c_str(), newKey.length(), 0);

    recv(fd, buffer, 256, 0);
    std::string ret(buffer);

    return ret;
}

std::string 
MemcachedClient::Delete(std::string &key) {
    std::hash<std::string> Hash;
    char buffer[256];
    bzero(buffer, 256);
    //获得虚拟节点
    //hash = HASH(key);
    //需要找到一个hash函数来hash key
    auto one = nodes.upper_bound(Hash(key)%(uint64_t)(pow(2,32)-1));
    //如果没找到，则默认归入第一个虚拟节点
    if (one == nodes.end()) {
        std::cout << "not found" << std::endl;
        one = nodes.begin();
    }
    //获得节点对应的socket
    int fd = connects[one->second];
    std::cout << "fd:" << fd << std::endl;
    std::string newKey = "delete ";
    newKey.append(key);
    newKey.append("\r\n");
    
    //发送给服务器
    send(fd, newKey.c_str(), newKey.length(), 0);

    recv(fd, buffer, 256, 0);
    std::string ret(buffer);

    return ret;
}

std::string
MemcachedClient::Version() {
    std::hash<std::string> Hash;
    char buffer[256];
    bzero(buffer, 256);
    //获得虚拟节点
    //hash = HASH(key);
    //需要找到一个hash函数来hash key
    auto one = nodes.upper_bound(100);
    //如果没找到，则默认归入第一个虚拟节点
    if (one == nodes.end()) {
        std::cout << "not found" << std::endl;
        one = nodes.begin();
    }
    //获得节点对应的socket
    int fd = connects[one->second];
    std::cout << "fd:" << fd << std::endl;
    std::string newKey = "version";
    newKey.append("\r\n");
    
    //发送给服务器
    send(fd, newKey.c_str(), newKey.length(), 0);

    recv(fd, buffer, 256, 0);
    std::string ret(buffer);

    return ret;
}

std::string
MemcachedClient::Quit() {
    std::hash<std::string> Hash;
    char buffer[256];
    bzero(buffer, 256);
    //获得虚拟节点
    //hash = HASH(key);
    //需要找到一个hash函数来hash key
    auto one = nodes.upper_bound(100);
    //如果没找到，则默认归入第一个虚拟节点
    if (one == nodes.end()) {
        std::cout << "not found" << std::endl;
        one = nodes.begin();
    }
    //获得节点对应的socket
    int fd = connects[one->second];
    std::cout << "fd:" << fd << std::endl;
    std::string newKey = "quit";
    newKey.append("\r\n");
    
    //发送给服务器
    send(fd, newKey.c_str(), newKey.length(), 0);
}

}; /* memcached client */
