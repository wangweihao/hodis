/*======================================================
    > File Name: hodis_threadpool.h
    > Author: wwh
    > E-mail:  
    > Other :  
    > Created Time: 2016年02月16日 星期二 18时02分02秒
 =======================================================*/


#ifndef _HODIS_THREADPOOL_H_
#define _HODIS_THREADPOOL_H_

#include "SynQueue.h"
#include <functional>
#include <thread>
#include <memory>
#include <atomic>
#include <tuple>

namespace hodis{

/* 上传下载函数需要socketfd和MD5值 */
using Task = std::function<bool(int, std::string, long, long, int)>;
using Handler = std::tuple<Task, int, std::string, long, long, int>;

/* 默认线程任务队列大小 */
const int MaxTaskCount = 100;

class ThreadPool
{

    public:
        /* 上传下载任务 */
        //using Task = std::function<void()>;

        /* 线程默认数量为cpu核心数x2 */
        ThreadPool(int ThreadNum = std::thread::hardware_concurrency()):
            queue(MaxTaskCount)
        {
            /* 初始化线程池 */
            Start(ThreadNum);
        }

        /* 销毁线程池 */
        ~ThreadPool()
        {
            Stop();
        }

        /* 终止所有线程，call_one保证函数只调用一次 */
        void Stop()
        {
            std::call_once(flag, [this] {StopThreadGroup(); });
        }

        /* 添加任务 */
        void AddTask(const Handler&hand)
        {
            queue.Put(hand);
        }

        /* 添加任务右值版本 */
        void AddTask(Handler &&hand)
        {
            queue.Put(hand);
        }

        int getSize()
        {
            return threadGroup.size();
        }

    private:
        /* 停止线程池 */
        void StopThreadGroup()
        {
            queue.Stop();
            running = false;
            for(auto thread : threadGroup)
            {
                if(thread)
                {
                    thread->join();
                }
            }
            threadGroup.clear();
        }

        /* 线程池开始运行 */
        void Start(int threadNum)
        {
            std::cout << "创建线程池" << std::endl;
            running = true;
            for(int i = 0; i < threadNum; i++)
            {
                threadGroup.push_back(std::make_shared<std::thread> (&ThreadPool::RunInThread, this));
            }
        }

        /* 从队列中取出一个事件 */
        void RunInThread()
        {
            while(running)
            {
                Handler hand;
                if(!running)
                {
                    return;
                }
                queue.Take(hand);
            }
        }

    private:
        /* 线程池 */
        std::list<std::shared_ptr<std::thread>> threadGroup;
        /* 任务队列 */
        SynQueue<Handler> queue;
        /* 原子布尔值 */
        std::atomic_bool running;
        /* 辅助变量 */
        std::once_flag flag;
};

};


#endif
