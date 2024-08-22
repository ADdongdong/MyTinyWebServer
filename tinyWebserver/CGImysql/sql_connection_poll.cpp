#include "sql_connection_poll.h"

using namespace std;

// 无参构造函数
connection_pool::connection_pool(){
    m_CurConn = 0;
    m_FreeConn = 0;
}

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
        con = mysql_real_connect(con, url.c_str(), User.c_str(), PassWord.c_str(), DBName.c_str(), port, NULL, 0);

        if (con == NULL){
            LOG_ERROR("MYSQL error, mysql_real_connect()error");
            exit(1);
        }

        connList.push_back(con);
        ++m_FreeConn;// 当前空闲链接数量+1
    }

    reverse = mytinyWebserver::Semaphore(m_FreeConn);

    m_MaxConn = m_FreeConn;
}

/**
 *@brief 当接收到链接数据库请求,从数据库连接池中返回一个可用连接，并更新已经使
         用和空闲的链接数
 */
MYSQL* connection_pool::GetConnection(){
    MYSQL* con = NULL;
    // 检查链接池中是否有足够的可用链接
    if (0 == connList.size()) {
        return NULL;
    }

    // 信号量等待
    reverse.wait();

    // 运行到这里的时候，说明互斥量已经有了
    m_mutex.lock(); //互斥量加锁

    con = connList.front();
    connList.pop_front();

    --m_FreeConn;
    ++m_CurConn;

    m_mutex.unlock();
    return con;
}

// 释放当前使用的链接
bool connection_pool::ReleaseConnection(MYSQL* con) {
    if (NULL == con) {
        return false;
    }

    m_mutex.lock();

    connList.push_back(con); // 池子中的链接+1
    ++m_FreeConn;
    --m_CurConn;

    m_mutex.unlock();

    reverse.notify(); // 信号量可用信号量数+1

    return true;
}

// 销毁数据库连接地址
void connection_pool::DestroyPool(){
    // 使用RAII
    std::lock_guard<std::mutex> lock(m_mutex);
    if (connList.size() > 0) {
        list<MYSQL *>::iterator it;
        for (it = connList.begin(); it != connList.end(); ++it) {
            // 关闭所有数据库连接
            MYSQL *con = *it;
            mysql_close(con);
        }
        m_CurConn = 0;
        m_FreeConn = 0;
        connList.clear(); // 清空链表（连接池）
    }
}

/**
 *@brief 获取当前空闲的链接数
 */
int connection_pool::GetFreeConn()
{
    return this->m_FreeConn;
}

/**
 *@brief 析构函数，清空连接池
 */
connection_pool::~connection_pool() {
    DestroyPool();
}

/**
 *@brief 构造函数，获取到一个线程池的链接。
         在离开该代码段的时候，调用析构函数自动释放该连接。
 *
 */
connectionRAII::connectionRAII(MYSQL **SQL, connection_pool *connPool) {
    // 从连接池中获取到一个链接
    *SQL = connPool->GetConnection();

    conRAII = *SQL;
    poolRAII = connPool;
}

connectionRAII::~connectionRAII() {
    // 释放当前链接
    poolRAII->ReleaseConnection(conRAII);
}
