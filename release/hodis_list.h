/*======================================================
    > File Name: hodis_list.h
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年03月16日 星期三 21时27分54秒
 =======================================================*/

#ifndef _HODIS_LIST_H_
#define _HODIS_LIST_H_

#include <list>
#include <mutex>

namespace hodis {

template <typename T>
class lockList {
    public:
        lockList();
        ~lockList();

        size_t getSize();
        void putConnect(T t);
        T& getConnect();
        bool empty();
        

    private:
        bool try_lock();
        void unlock();

    private:
        std::list<T> list_;
        std::mutex mutex;
};

template<typename T>
lockList<T>::lockList() {

}

template<typename T> 
lockList<T>::~lockList() {

}

template<typename T>
bool lockList<T>::try_lock() {
    if(mutex.try_lock() == true) {
        return true;
    }else {
        return false;
    }
}

template<typename T>
void lockList<T>::unlock() {
    mutex.unlock();
}

template<typename T>
void lockList<T>::putConnect(T t) {
    while(1) {
        if(try_lock() == true) {
            list_.push_back(t);
            unlock();
            return;
        }
    }
}

template<typename T>
T& lockList<T>::getConnect() {
    while(1) {
        if(try_lock() == true) {
            auto &t = *list_.begin();
            list_.pop_front();
            unlock();
            return t;
        }
    }
}

template<typename T>
size_t lockList<T>::getSize() {
    return list_.size();
}

template<typename T>
bool lockList<T>::empty() {
    return list_.empty();
}

}; /* hodis */

#endif
