#include "log.h"

Log::Log(/* args */)
{
    m_count = 0;
    m_is_async = false;
}

Log::~Log()
{
    if (m_fp != NULL) {
        fclose(m_fp);
    }
}

// 初始化static的单例对象, static要在类外初始化
Log* Log::m_instance = new Log;

// 获取单例对象
Log* Log::getInstance(){
    return m_instance;
}

// 异步需要设置阻塞队列的长度，同步不需要设置
bool Log::init(const char *filename, int close_log, int log_buf_size , int split_lines , int max_queue_size ){
    // 如果设置了max_queue_size，则设置为异步
    if (max_queue_size >= 1) {
        m_is_async = true;
        m_log_queue = new block_queue<std::string>(max_queue_size);
        pthread_t tid;
        // flush_log_thread为回调函数，这里表示创建线程异步写日志
        // 创建异步写日志的线程, 创建另一个线程专门用来写日志
        pthread_create(&tid, NULL, flush_log_thread, NULL);
    }

    m_close_log = close_log;
    m_log_buf_size = log_buf_size;
    m_buf = new char[m_log_buf_size];
    memset(m_buf, '\0', m_log_buf_size);
    m_split_lines = split_lines;

    time_t t = time(NULL);
    struct tm *sys_tm = localtime(&t);
    struct tm my_tm = *sys_tm;

    const char *p = strrchr(filename, '/');
    char log_full_name[256] = {0};

    if (p == NULL){
        snprintf(log_full_name, 255, "%d_%02d_%02d_%s", my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday, filename);
    } else {
        strcpy(log_name, p + 1);
        strncpy(dir_name, filename, p - filename + 1);
        snprintf(log_full_name, 255, "%s%d_%02d_%02d_%s", dir_name, my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday, log_name);
    }

    m_today = my_tm.tm_mday;

    m_fp = fopen(log_full_name, "a");
    if (m_fp == NULL) {
        return false;
    }

    return true;
}

void Log::writeLog(int level, const char *format, ...){
    struct timeval now = {0, 0};
    gettimeofday(&now, NULL);
    time_t t = now.tv_sec;
    struct tm *sys_tm = localtime(&t);
    struct tm my_tm = *sys_tm;
    char s[16] = {0};

    switch (level)
    {
    case 0:
        strcpy(s, "[error]:");
        break;
    case 100:
        strcpy(s, "[wanr]:");
        break;
    case 200:
        strcpy(s, "[debug]:");
        break;
    case 300:
        strcpy(s, "[info]:");
        break;
    default:
        strcpy(s, "[info]:");
        break;
    }


    // 每写入一次log,就对m_count++, m_split_lines表示最大行数
    std::unique_lock<std::mutex> lock(m_mutex);
    m_count++; // 这里如果没有m_count++则会产生两个log文件，并且，log文件多了以后也不会创建新的log文件

    // 每日创建新的log文件
    if (m_today != my_tm.tm_mday || m_count % m_split_lines == 0){
        char new_log[256] = {0};
        fflush(m_fp); // 刷新缓冲区
        fclose(m_fp); // 关闭缓冲区
        char tail[16] = {0};

        snprintf(tail, 16, "%d_%02d_%02d_", my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday);

        // 如果是新的一天，要创建新的日志文件
        if (m_today != my_tm.tm_mday) {
            snprintf(new_log, 255, "%s%s%s", dir_name, tail, log_name);
            m_today = my_tm.tm_mday; // 更新m_today
            m_count = 0; // 刷新行数
        } else {
        // 如果是日志文件行数到达最大行数了，要创建当天的新的日志文件
            snprintf(new_log, 255, "%s%s%s.%lld", dir_name, tail, log_name, m_count / m_split_lines);
        }
        m_fp = fopen(new_log, "a"); // 文件名为new_log的文件
    }

    lock.unlock();

    va_list valst;
    va_start(valst, format);

    std::string log_str;
    lock.lock();

    // 写入具体的时间内容格式
    int n = snprintf(m_buf, 48, "%d-%02d-%02d %02d:%-2d:%02d.%06ld %s",
                    my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday,
                    my_tm.tm_hour, my_tm.tm_min, my_tm.tm_sec, now.tv_usec, s);
    int m = vsnprintf(m_buf + n, m_log_buf_size - n - 1, format, valst);

    m_buf[n+m] = '\n';
    m_buf[n + m + 1] = '\0';
    log_str = m_buf;

    // 循环数组实现阻塞队列, 完成异步写日志部分
    lock.unlock();

   // 如果标记为异步写日志，并且，阻塞队列没满，那么就将日志信息加入到阻塞队列中，等待写日志
    if (m_is_async && !m_log_queue->full()){
        m_log_queue->push(log_str);
    } else {
        // 否则，直接将日志写入, 也就是说，如果日志队列满了，也进入同步写日志
        std::lock_guard<std::mutex> lock(m_mutex);
        fputs(log_str.c_str(), m_fp);
    }

    va_end(valst);
}

void Log::flush(void){
    std::lock_guard<std::mutex> lock(m_mutex);
    fflush(m_fp);
}
