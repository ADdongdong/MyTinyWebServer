# MyTinyWebServer
c++11  and cmake for tinyWebServer
* 使用CMake构建
* 所有模块放在tinyWebserve目录下
* 所有测试程序放在test目录下， 可执行文件放在bin目录下
* 日志模块可选择同步/异步写入日志
* 数据库连接池模块，底层为链表，为了实现用户账户密码登录，自定义信号量保证数据库连接池在多线程使用中的线程安全
* 

## log日志模块

> Log日志模块实现可选同步日志和异步日志。
> 在异步日志中，实现了阻塞队列，在写入日志的时候，先将日志信息放入阻塞队列中，
> 异步日志只要检测到队列不空，就持续调用`pop`将队列中的日志信息异步写入到日志文件中。

  <img src="https://github.com/user-attachments/assets/526995ae-cbd8-4612-b6f0-1b2c8e31ebda" alt="description" style="display:block; margin-left:auto; margin-right:auto; width:70%;">

## 数据库连接池模块

* 数据库连接池底层使用`stl::list`实现
* 自制`Semphore`使用`std::condition_vairable`和`std::mutex`实现, `Semphore`用来实现对数据库连接池链接的线程同步。

  <img src="https://github.com/user-attachments/assets/ea5fa649-2a4f-476c-a24a-0ea55c65df44" alt="description" style="display:block; margin-left:auto; margin-right:auto; width:60%;">


## 线程池模块

* 构建线程池实现多线程处理请求
* 线程池底层为`std::vector<std::thread>`
* 线程同步采用`std::mutex`和`std::condition_variable`
* 当有请求的时候，条件变量`notify_one()`唤醒一个线程，并从请求队列中弹出请求，进入线程进行处理

<img src="https://github.com/user-attachments/assets/4da5efcd-42be-46ba-b863-e68807e704e6" alt="description" style="display:block; margin-left:auto; margin-right:auto; width:60%;">

## 定时器模块

>定时器模块用来处理那些已经关闭的客户端或长时间没有消息的客户端，将他们的信息从epoll中剔除，关闭连接，释放内存资源

