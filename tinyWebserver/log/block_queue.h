/**
 * @file block_queue.h
 * @author 循环数组实现阻塞队列，m_back = (m_back + 1) % m_max_size;
 *
 * @brief
 * @version 0.1
 * @date 2024-07-04
 * @copyright Copyright (c) 2024
 */
#ifndef BLOCK_QUEUE_H
#define BLOCK_QUEUE_H

#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <mutex> // 互斥量
#include <condition_variable> // 条件变量
#include <sys/time.h>
#include <chrono>
#include <vector>

template <class T>
class block_queue
{
private:
    /* data */
public:

    // 初始化队列
    block_queue(int max_size = 1000) 
        : m_max_size(max_size), m_array(max_size), m_size(0), m_front(0), m_back(0){
            if (max_size <= 0) {
                throw std::invalid_argument("Queue size must be positive");
            }
    }

    // 析构
    ~block_queue(){
        // 这里使用了vector，所以，析构的时候什么都不用做了，vector会帮我们析构
    }


    void clear(){
        std::lock_guard<std::mutex> lock(m_mutex);
        m_size = 0;
        m_front = 0;
        m_back = 0;
    }


    // 判断是否队满
    bool full() {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_size >= m_max_size) {
            lock.unlock();
            return true;
        }
        return false;
    }

    // 判断是否队空
    bool empty(){
        std::unique_lock<std::mutex> lock(m_mutex);
        if (0 == m_size) {
            lock.unlock();
            return true;
        }
        return false;
    }

    // 返回队首元素, 返回值作为参数传出
    bool front(T &value){
        std::unique_lock<std::mutex> lock(m_mutex);
        if (0 == m_size){
            lock.unlock();
            return false;
        }
        value = m_array[m_front];
        return true;
    }

    // 返回队列尾元素
    bool back(T &value) {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_size == 0){
            return false;
        }
        value = m_array[m_back];
        return true;
    }

    // 返回当前队列列长度
    int size(){
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_size;
    }

    // 返回队列的最大长度
    int max_size(){
        return m_max_size;
    }

    // 当往队列添加元素，需要将所有使用队列的线程先唤醒
    // 当有元素push进队列，相当于生产者生产了一个元素
    // 若当前也有现成等待条件变量，则唤醒无意义
    bool push(const T &item) {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_size >= m_max_size) {
            m_cond.notify_all();
            return false;
        }

        m_back = (m_back + 1) % m_max_size;
        m_array[m_back] = item;
        m_size++;
        // notify_all唤醒所有等待在这个条件变量上的线程(其实只有一个线程)。
        // 调用这个函数以后，所有等待线程都会被唤醒，从阻塞态变道就绪态，等待调度器安排执行
        m_cond.notify_all();
        return true;
    }

    // 调用pop的时候，如果当前队列没有元素，将会等待条件变量
    bool pop(T &item) {
        std::unique_lock<std::mutex> lock(m_mutex);

        // 使用wait等待条件满足
        // 让线程等待，知道队列中元素个数大于0,不然就一直阻塞
        // 每次线程被唤醒的时候，都会检查m_size > 0这个条件，直到为真，才会继续执行
        m_cond.wait(lock, [this]{return m_size > 0;});

        // 如果队列为空，返回false
        if (m_size <= 0) {
            return false;
        }

        m_front = (m_front + 1) % m_max_size;
        item = m_array[m_front];
        m_size--;

        return true;
    }

    /**
     * @brief 增加了超时处理的pop, 超时处理就要用到wait_for
     * @param[in] ms_timeout 表示，超过ms_timeout了就算超时了
     */
    bool pop(T &item, int ms_timeout) {
        std::unique_lock<std::mutex> lock(m_mutex);

        // 调用wait_for时候，如果m_size >0不满足，现成会进入阻塞态，等待条件变量被通知(notify_one或者ontify_all)
        // 或者，等待超时时间到达
        // 也就是说，在阻塞态，时候，程序会阻塞挂起，等待如下两个事件发生一个
        // 1.m_size > 0
        // 2.指定的超时时间到达
        // 如果在超市时间呢，m_size > 0满足了，那条件变量会唤醒一个或者多个等待线程，线程从阻塞变为就绪，等待执行代码
        // 如果，超时时间内，m_size > 0 始终不满足，则，在超时时间到达后，条件变量会自动唤醒线程。线程从阻塞变为就绪，
        // 进入运行状态后，再检查条件是否满足
        bool condition_met = m_cond.wait_for(lock,
            std::chrono::milliseconds(ms_timeout),
            [this]{return m_size > 0;});

        // 超时了，或者队列为空，都返回fasle
        if (!condition_met || m_size <= 0) {
            return false;
        }

        m_front = (m_front + 1) % m_max_size;
        item = m_array[m_front];
        m_size--;

        return true;
    }

private:
    std::mutex m_mutex;
    std::condition_variable m_cond;

    std::vector<T>  m_array;
    int m_size;
    int m_max_size;
    int m_front;
    int m_back;
};



#endif
