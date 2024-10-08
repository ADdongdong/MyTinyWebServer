#ifndef  _CONNECTION_POOL_
#define _CONNECTION_POOL_

#include <stdio.h>
#include <list>
#include <mysql/mysql.h>
#include <error.h>
#include <string>
#include <iostream>
#include <mutex>
#include <semaphore> // 信号量sem
#include "../log/log.h"
#include "../Semaphore.h"

using namespace std;

class connection_pool
{
public:
    MYSQL *GetConnection(); // 获取数据库连接
    bool ReleaseConnection(MYSQL *conn); // 释放数据库连接
    int GetFreeConn(); // 获取链接
    void DestroyPool(); // 销毁所有链接

    // 单例模式
    static connection_pool *GetInstance();

    // 初始化连接池
    void init(string url, string User, string PassWord, string DBName, int port, int MaxConn, int close_log);
private:
    connection_pool();
    ~connection_pool();

    int m_MaxConn; // 最大链接数
    int m_CurConn; // 当前已经使用的链接数
    int m_FreeConn; // 当前空闲的链接数
    mutex m_mutex; // 定义互斥量

    list<MYSQL *> connList; // 链接池
    mytinyWebserver::Semaphore reverse; // 定义信号量

public:
    string m_url; // 主机地址
    string m_Port; // 数据库端口号
    string m_User; // 登录数据库的用户名，一般为root
    string m_PassWord; // 登录数据库的密码
    string m_DatabaseName; // 使用的数据库的名称
    int m_close_log; // 日志开关
};


// RAII风格代码编写
class connectionRAII{
public:
    // 构造函数
    connectionRAII(MYSQL **con, connection_pool *connPool);
    // 析构函数
    ~connectionRAII();
private:
    MYSQL *conRAII;
    connection_pool *poolRAII;
};

#endif
