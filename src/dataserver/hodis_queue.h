/*======================================================
    > File Name: hodis_queue.h
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月16日 星期二 17时56分55秒
 =======================================================*/


#ifndef _HODIS_QUEUE_H_
#define _HODIS_QUEUE_H_

#include <list>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>

namespace hodis{

template<typename T>
class SynQueue
{
    public:
        /* 构造函数，初始化同步队列大小 */
        SynQueue(int QueueSize):
            maxSize(QueueSize), stop(false) { }

        /* 向队列中添加任务 */
        void Put(const T& x)
        {
            /* 调用private Add接口 */
            Add(x);
        }
        void Put(T && x)
        {
            Add(x);
        }

        /* 从队列中取事件 */
        void Take(T &t)
        {
            /* 早点释放锁，不加作用域每次只能单用户下载 */
            {
                std::unique_lock<std::mutex> locker(mutex);
                notEmpty.wait(locker, [this]{
                        return stop || NotEmpty();
                        });
                if(stop)
                {
                    return;
                }
                t = queue.front();
                queue.pop_front();
                notFull.notify_one();
            }
            auto task = std::get<0>(t);
            task(std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t), std::get<5>(t));
        }

        /* 停止所有线程在任务队列中存取任务 */
        void Stop()
        {
            /* 早点释放锁 */
            {
                std::lock_guard<std::mutex> locker(mutex);
                stop = true;
            }
            notFull.notify_all();
            notEmpty.notify_all();
        }

        /* 队列为空 */
        bool Empty()
        {
            std::lock_guard<std::mutex> locker(mutex);
            return queue.empty();
        }

        /* 队列为满 */
        bool Full()
        {
            std::lock_guard<std::mutex> locker(mutex);
            return queue.size() == maxSize;
        }

        /* 队列大小 */
        size_t Size()
        {
            return queue.size();
        }

    private:
        /* 内部调用添加函数 */
        template<typename F>
        void Add(F &&x)
        {
            std::unique_lock<std::mutex> locker(mutex);
            /* 条件变量，不满足则阻塞 */
            notFull.wait(locker, [this] {
                    return stop || NotFull();
                    });
            /* 如果终止标志被设定，则停止添加任务 */
            if(stop)
            {
                return;
            }
            queue.push_back(std::forward<F>(x));
            notEmpty.notify_one();
        }

        /* 测试任务队列是否满了 */
        bool NotFull() const
        {
            bool full = (queue.size() >= maxSize);
            if(full)
            {
                std::cout << "任务队列已满，请等待..." << std::endl;
            }
            return !full;
        }

        /* 测试任务队列是否空了 */
        bool NotEmpty() const
        {
            bool empty = queue.empty();
            if(empty)
            {
                std::cout << "任务队列已没任务，请等待..." << std::endl;
                std::cout << "线程ID:" << std::this_thread::get_id() << std::endl; 
            }
            return !empty;
        }

    private:
        std::mutex mutex;
        std::condition_variable notEmpty;
        std::condition_variable notFull;
        std::list<T> queue;
        int maxSize;
        bool stop;
};

}
