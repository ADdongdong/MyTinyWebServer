#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

/**
 *@brief c++11没有信号量，故使用c++11中的std::mutex和std::condition_variable
 *       来实现信号量
 *@notify std::mutex和std::condition_variable都是不可靠北的，这两个的拷贝构造
 和拷贝赋值都是delete的（不可使用）的。所以，用到这两个类的Semaphore也是不能使
 用拷贝构造和拷贝赋值的。因为，如果使用拷贝构造和拷贝赋值，其成员对象也要调用
 对应的拷贝构造和拷贝赋值，这样会报错。所以，这里只能使用move移动赋值和移动拷贝。
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace mytinyWebserver{

class Semaphore{
public:
    // 无参构造
    Semaphore():count(0){}

    // 带参构造，指定初始值count的值, 注意，这里有默认参数就会和默认无参构造混淆
    explicit Semaphore(int count) :count(count) {}

    // 禁用拷贝构造函数
    Semaphore(const Semaphore&) = delete;
    // 禁用拷贝赋值运算
    Semaphore& operator=(const Semaphore&) = delete;

    // 允许移动构造和移动赋值运算符
    Semaphore(Semaphore&& other) noexcept : count(other.count){
        // 这里不用移动std::mutex和std::condition_variable
        // 因为他们不需要共享状态
        other.count = 0;
    }

    // 允许移动赋值运算
    Semaphore& operator=(Semaphore&& other) noexcept {
        if (this != &other) {
            // 进行移动的时候，加锁
            std::unique_lock<std::mutex> lock(mtx);
            count = other.count;
            other.count = 0;
        }
        return *this;
    }


    void notify() {
        std::unique_lock<std::mutex> lock(mtx);
        ++count;
        cv.notify_one();
    }

    void wait() {
        // 信号量等待，等待count>0的时候才能加锁
        std::unique_lock<std::mutex> lock(mtx);
        // 这里捕获this，方便在lambda中访问成员变量count
        cv.wait(lock, [this](){return count > 0;});
        --count;
    }

private:
    std::mutex mtx;
    std::condition_variable cv;
    int count;
};

}

#endif
