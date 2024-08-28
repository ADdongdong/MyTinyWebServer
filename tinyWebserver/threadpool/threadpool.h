#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

#include <list>
#include <vector>
#include <cstdio>
#include <exception>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "../Semaphore.h"
#include "../CGImysql/sql_connection_poll.h"

template <typename T>
class threadpool {
public:
    /**
     *@brief 线程池构造函数
     *@param[in] thread_number 线程池中线程的数量
     *@param[in] max_requestq 请求队列中最多允许的等待处理的请求数量
     *@param[in] actor_model
     *@param[in] connPool 指向链接池的指针(连接池)
     */
    threadpool(int actor_model, connection_pool *connPool, int thread_number = 8, int max_request = 10000);
    // 析构函数
    ~threadpool();

    // 往请求队列中添加新的请求
    bool append(T *request, int state);
    // 往请求队列中添加新的请求
    bool append_p(T *request);
private:
    /**
     *@brief 工作线程运行的函数，它不断从工作队列中取出任务并执行
     */
    static void *worker(void *arg);
    void run();

private:
    int m_thread_number;                    // 线程池中线程的数量
    int m_max_requests;                     // 请求队列中允许的最大请求
    std::vector<std::thread> m_threads;     // 线程池数组，线程池用数组实现
    std::list<T *> m_workqueue;             // 请求队列
    std::mutex m_queuelocker;               // 保护请求队列的互斥锁
    mytinyWebserver::Semaphore m_queuestat; // 是否有任务需要处理，这里采用信号量
    connection_pool *m_connPool;            // 数据库连接池
    int m_actor_model;                      // 模式切换
};

template <typename  T>
threadpool<T>::threadpool(int actor_model, connection_pool *connPool, int thread_number, int max_request){
    // 如果传入的线程池中的数量和请求队列允许的最大请求数小于等于0，则返回异常
    if (thread_number <= 0 || max_request <= 0) {
        throw std::exception();
    }

    // 创建线程池中的所有线程
    for (int i=0; i < thread_number; ++i) {
        // 线程的代码段为threadpool::worker,可以为lambad、函数等可以被调用的
        m_threads.emplace_back(&threadpool::worker, this); // 使用std::thread创建线程
                                                           // 这里this表示传入threadpool::worker的参数，
                                                           // 传入this表示，对当前对象执行操作
    }
}

template<typename T>
threadpool<T>::~threadpool() {
    for (std::thread &thread : m_threads){
        if (thread.joinable()){ // 检查线程是否可以被join
            thread.join();// 等待线程完成, 线程完成后，释放线程资源
        }
    }
}

template <typename T>
bool threadpool<T>::append(T *request, int state) {
    {
        std::lock_guard<std::mutex> lock(m_queuelocker); // 保护工作队列(请求队列)，在添加任务到工作队列的时候，要加锁
        if (m_workqueue.size() >= m_max_requests) {
            return false;
        }
        m_workqueue.push_back(request);
        request->m_state = state; // 这一行是什么意思？应该是设置这个请求的状态，哪种状态呢
    }
    m_queuestat.notify(); // 信号量就有多余了，就通知一个等待的线程
    return true;
}

template <typename T>
bool threadpool<T>::append_p(T *request) {
    // 与append类似逻辑，可按需定制
    return append(request, 0);
}

// 现成要执行的内容，要调用的内容
template <typename T>
void *threadpool<T>::worker(void *arg) {
    // 让arg去执行run函数
    threadpool *pool = (threadpool*)arg;
    pool->run();
    return pool;
}

template<typename T>
void threadpool<T>::run() {
    while (true){
        m_queuestat.wait();
        std::unique_lock<std::mutex> lock(m_queuelocker);
        if (m_workqueue.empty()) {
            lock.unlock();
            continue;
        }
        T *request = m_workqueue.front(); // 获取工作队列的首个元素
        m_workqueue.pop_front();
        lock.unlock();

        if (!request)
            continue;
        /* 这里m_actor_mode应该是来控制，这个webserver是否要设施ET/LT,如果是1就是要设置ET和LT
         *
         */
        if (1 == m_actor_model) {
            // 然后这里的request->m_state就是来空时是et还是LT的, 这都是猜测哈
            if (0 == request->m_state) {
                if (request->read_once()) {
                    request->improv = 1;
                    connectionRAII mysqlcon(&request->mysql, m_connPool);
                    request->process();
                } else {
                    request->improv = 1;
                    request->timer_flag = 1;
                }
            } else {
                if (request->write()) {
                    request->improv = 1;
                } else {
                    request->improv = 1;
                    request->timer_flag = 1;
                }
            }
        }
        else {
            connectionRAII mysqlcon(&request->mysql, m_connPool);
            request->process();
        }
    }
}


#endif


