#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <errno.h>
#include <cassert>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/epoll.h>

#include "../threadpool/threadpool.h"

const int MAX_FD = 65536;           // 最大文件描述符
const int MAX_EVENT_NUMBER = 10000; // 最大事件数
const int TIMESLOT = 5;             // 最小超时单位

class WebServer{
public:
    WebServer();
    ~WebServer();

    void init(int port, string user, string passWord, string databaseName,
              int log_write, int opt_linger, int trigmod, int sql_num,
              int thread_num, int close_log, int actor_model);

    // WebServer日志器
    void log_write();

    // WebServer 数据库（链接池)
    void sql_pool();

    // 线程池
    void thread_pool();

public:
    // 基础数据
    int m_port;         // 端口号
    char* m_root;       // ?
    int m_log_write;    // 日志
    int m_close_log;    // 是否关闭日志
    int m_actormodel;   // ?

    int m_pipefd[2];    // 管道通信相关
    int m_epollfd;      // epoll相关
    // http_conn *user;

    // 数据库相关
    connection_pool *m_connPool; // 数据库连接池
    string m_user;               // 登录数据库的用户名（默认为root)
    string m_passWord;           // 登录数据库的密码
    string m_databaseName;       // 所使用的数据库的名称
    int m_sql_num;               // 数据库连接个数

    // 线程池相关
    threadpool<http_conn> *m_pool; // 线程池，池中元素为http_conn
    int m_thread_num;              // 线程池中线程个数？
};

#endif
