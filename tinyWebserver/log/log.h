/**
 * @file log.h
 * @author hahadong
 * @brief log日志模块，为单例模式
 * @version 0.1
 * @date 2024-07-04
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <iostream>
#include <string>
#include <pthread.h>
#include <sys/time.h>
#include <string.h>
#include <stdarg.h>
#include <string>
#include <mutex>
#include "block_queue.h"

class Log
{
public:

    // 定义日志级别
    enum Level{
        // 错误
        ERROR = 0,
        // 警告
        WARN = 100,
        // 调试
        DEBUG = 200,
        // 信息
        INFO = 300
    };

    static Log* getInstance();

    /**
     * @brief 写日志函数
     * 
     * @param level 日志等级
     * @param format 日志格式,为字符串格式
     * @param ... 可变参数
     */
    void writeLog(int level, const char *format, ...);

    // 定义异步写日志回调函数
    static void *flush_log_thread(void *args){
        Log::getInstance()->async_write_log();
    }

    // 初始化参数
    bool init(const char *filename, int close_log, int log_buf_size = 8192, int split_lines = 5000000, int max_queue_size = 0);

    // 刷新日志缓冲区
    void flush(void);
private:
    Log(/* args */);
    virtual ~Log();

    // 定义异步写日志函数
    void *async_write_log(){
        std::string string_log;
        // 从阻塞队列中取出一个日志string,写入文件
        // 这里是将阻塞队列的队头元素写入到string_log中
        // 再将string_log写入到m_fp文件中
        while(m_log_queue->pop(string_log)) {
            m_mutex.lock();
            // 将log写入文件中
            fputs(string_log.c_str(), m_fp);
            m_mutex.unlock();
        }
    }
private:
    static Log *m_instance;

    char dir_name[128]; // 日志路径
    char log_name[128]; // 文件名
    int m_log_buf_size; // 日志缓冲区大小
    int m_count; // dang
    int m_split_lines;// 最大行数
    int m_today;
    FILE *m_fp; // 打开Log的文件指针
    char* m_buf;
    bool m_is_async; // 是否同步写入日志的标志位
    block_queue<std::string> *m_log_queue; // 阻塞队列
    std::mutex m_mutex;
    int m_close_log; // 关闭日志
public:
};

#define LOG_DEBUG(format, ...) if (0 == m_close_log) {Log::getInstance()->writeLog(200, format, ##__VA_ARGS__); Log::getInstance()->flush();}
#define LOG_INFO(format, ...) if (0 == m_close_log) {Log::getInstance()->writeLog(300, format, ##__VA_ARGS__); Log::getInstance()->flush();}
#define LOG_WARN(format, ...) if (0 == m_close_log) {Log::getInstance()->writeLog(100, format, ##__VA_ARGS__); Log::getInstance()->flush();}
#define LOG_ERROR(format, ...) if (0 == m_close_log) {Log::getInstance()->writeLog(0, format, ##__VA_ARGS__); Log::getInstance()->flush();}


#endif
