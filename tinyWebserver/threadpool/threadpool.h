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
    bool append(T *request, int state);
    bool append_p(T *request);
private:
    /**
     *@brief 工作线程运行的函数，它不断从工作队列中取出任务并执行
     */
    static void *worker(void *arg);
    void run();

private:
    int m_thread_number;                // 线程池中线程的数量
    int m_max_requests;                 // 请求队列中允许的最大请求
    std::vector<std::thread> m_threads; // 线程池数组，线程池用数组实现
    std::list<T *> m_workqueue;         // 请求队列
    std::mutex m_queuelocker;           // 保护请求队列的互斥锁
    std::condition_variable m_queuestat;// 用户等待/通知线程处理任务
    connection_pool *m_connPool;        // 数据库连接池
    int m_actor_model;                  // 模式切换
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
        m_threads.emplace_back(&threadpool::worker, this);
    }

}


#endif


