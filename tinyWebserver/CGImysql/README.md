校验 & 数据库连接池实现
========================

**数据库连接池**

* 单例模式，确保池的唯一
* list实现连接池，链接池本质上为list
* 链接池为静态大小，不会动态改变
* 互斥所引入，确保线程安全, 互斥量由c++11的`std::conditon_variable`和`std::mutex`实现

**RAII风格使用**

>RAII(Resource Acquisition Is Initialization, 资源获取即初始化)
* `connection_pool`编写的时候，多线程部分使用RAII风格编写
* 使用连接池，构造`connectionRAII`类，来使用RAII获取连接，在离开代码段时释放连接

**登录校验**

* HTTP请求采用POST方式
* 登录用户名和密码要和数据库交互进行校验
* 用户注册及多线程注册安全
