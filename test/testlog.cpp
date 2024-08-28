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
    // 初始化日志系统, 最后一个参数设置为0，表示为同步写日志
    if (Log::getInstance()->init("./test_log", 0, 2000, 800000, 0)){
        std::cout << "Log system initialized successfully" << std::endl;
    } else {
        std::cerr <<"Faile to initialized log system" <<std::endl;
        return 1;
    }

    std::cout << "Starting log tests..." << std::endl;

    // 测试同步日志
    std::cout << "Testing synchronous logging:" <<std::endl;
    LOG_INFO("同步日志写入");
    test_log();

    // 等待一秒钟，确保同步日志已经写入
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 测试异步写入日志 这里设置了最后一个参数为800，表示这就是异步日志
    if (Log::getInstance()->init("./test_log", 0, 2000, 800000, 800)){
        std::cout << "Log system initialized successfully" << std::endl;
    } else {
        std::cerr <<"Faile to initialized log system" <<std::endl;
        return 1;
    }
    // 再次调用日志测试函数数
    LOG_INFO("异步日志写入");
    test_log();

    // 等待异步日志线程完成
    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::cout << "Log tests completed, check the log file for results." << std::endl;

    return 0;
}
