/*======================================================
    > File Name: hodis_LRU.h
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年09月01日 星期四 15时20分44秒
 =======================================================*/

#include <iostream>
#include <list>
#include <map>
#include <unordered_map>
#include <memory>
#include <string>
#include <mutex>
#include <utility>

namespace hodis {

class LRUCache {
    public:
        using Item = std::pair<std::string, std::string>;
        using Iter = std::list<Item>::iterator;
        using LRUCacheList = std::shared_ptr<std::list<Item>>;
        using LRUHashMap   = std::shared_ptr<std::unordered_map<std::string, Iter>>;

        LRUCache();
        LRUCache(uint64_t size);
        ~LRUCache();

    public:
        void set(const std::string &key, const std::string &value);       
        std::string get(const std::string &key);
        void foreach();

    private:
        /* LRUCache List */
        LRUCacheList lruCache;
        /* hashmap 
         * key:string 
         * value:item* 
         * */
        LRUHashMap hashMap;
        uint64_t lruCacheCurSize;
        uint64_t lruCacheMaxSize;
};

} /* hodis */
