/*======================================================
    > File Name: hodis_LRU.cpp
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年09月01日 星期四 15时52分05秒
 =======================================================*/

#include "hodis_LRU.h"

namespace hodis {

LRUCache::LRUCache(uint64_t size) {
    lruCache = std::make_shared<std::list<Item>>();
    hashMap  = std::make_shared<std::unordered_map<std::string, Iter>>();
    lruCacheCurSize = 0;
    lruCacheMaxSize = size;
}

LRUCache::~LRUCache() {

}

void
LRUCache::set(const std::string &key, const std::string &value){
    std::mutex mutex;
    auto item = std::make_pair(key, value);
    
    std::lock_guard<std::mutex> lock(mutex);
    if (lruCacheCurSize < lruCacheMaxSize) {
        lruCacheCurSize++;
        lruCache->insert(lruCache->begin(), item);
        hashMap->insert({key, lruCache->begin()});
    }else {
        /* erase last item */
        lruCache->erase(--lruCache->end());
        lruCache->insert(lruCache->begin(), item);
        if (hashMap->find(key) != hashMap->end()) {
            (*hashMap)[key] = lruCache->begin();
        }else {
            hashMap->insert({key, lruCache->begin()});
        }
    }
}

std::string 
LRUCache::get(const std::string &key) {
    std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);
    if (hashMap->find(key) != hashMap->end()) {
        auto iter = (*hashMap)[key];      
        auto item = *iter;
        
        /* 注意：删除节点时，当前迭代器会失效
         * 其余迭代器不变 */
        lruCache->erase(iter);
        lruCache->insert(lruCache->begin(), std::move(item));
        (*hashMap)[key] = lruCache->begin();

        return (*(*hashMap)[key]).second;   
    }else {
        return "";
    }
}

void
LRUCache::foreach() {
    for (auto &item : *lruCache) {
        std::cout << "key:" << item.first << " value:" << item.second << std::endl;
    }
    std::cout << std::endl;
}

} /* hodis */
