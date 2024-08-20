/**
 * @file testlog.cpp
 * @brief 日志测试类
 * @version 0.1
 * @data 2024-7-31
 */

#include "../tinyWebserver/log/log.h"
#include <thread>
#include <chrono>
#include <iostream>

int m_close_log = 0;

void test_log() {
    // 测试不同级别的日志
    LOG_DEBUG("This is a debug message: %d", 10);
    LOG_INFO("This is an info message: %s", "Hello, World!");
    LOG_WARN("This is a warning message: %.2f", 3.14);
    LOG_ERROR("This is an error message: %x", 0xDEADBEEF);
}

int main(){
    // 初始化日志系统
    if (Log::getInstance()->init("./test_log", 0, 2000, 800000, 800)){
        std::cout << "Log system initialized successfully" << std::endl;
    } else {
        std::cerr <<"Faile to initialized log system" <<std::endl;
        return 1;
    }

    std::cout << "Starting log tests..." << std::endl;

    // 测试同步日志
    std::cout << "Testing synchronous logging:" <<std::endl;
    test_log();

    // 等待一秒钟，确保同步日志已经写入
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 测试异步日志
    std::cout << "Testing asynchronous logging:" << std::endl;

    // 创建一个线程来异步写日志
    pthread_t pid;
    pthread_create(&pid, NULL, Log::flush_log_thread, NULL);

    // 再次调用日志测试哈数
    test_log();

    // 等待异步日志线程完成
    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "Log tests completed, check the log file for results." << std::endl;

    return 0;
}
