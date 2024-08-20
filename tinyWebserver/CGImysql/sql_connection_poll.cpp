#include "sql_connection_poll.h"

using namespace std;

// 单例模式
connection_pool* connection_pool::GetInstance(){
    static connection_pool connPool;
    return &connPool;
}

// 初始化链接池
void connection_pool::init(string url, string User, string PassWord,string DBName, int port, int MaxConn, int close_log) {
    m_url = url;
    m_Port = port;
    m_User = User;
    m_PassWord = PassWord;
    m_DatabaseName = DBName;
    m_close_log = close_log;

    for (int i = 0; i < MaxConn; i++) {
        MYSQL *con = NULL;
        con = mysql_init(con);

        if (con == NULL){
            LOG_ERROR("MYSQL Error, mysql_init() error");
            exit(1);
        }

        // 链接mysql
        con = mysql_real_connect(con, url.c_str(), Usr.c_str(), PassWord.c_str(), DBName.c_str(), port, NULL, 0);

        if (con == NULL){
            LOG_ERROR("MYSQL error, mysql_real_connect()error");
            exit(1);
        }

        connList.push_back(con);
        ++m_FreeConn;// 当前空闲链接数量+1
    }
    reverse = semaphore(m_FreeConn); // 信号量如何设置呢？
    m_MaxConn = m_FreeConn;

}


connection_pool::connection_pool(){

}
