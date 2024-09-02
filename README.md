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

<div style="text-align: center;">
  <img src="https://github.com/user-attachments/assets/526995ae-cbd8-4612-b6f0-1b2c8e31ebda" alt="description" width="800">
</div>

## 数据库连接池模块

* 数据库连接池底层使用stl::list实现
* 自制Semphore使用std::condition_vairable和std::mutex实现, Semphore用来实现对数据库连接池链接的线程同步。
<div style="text-align: center;">
  <img src="https://github.com/user-attachments/assets/ea5fa649-2a4f-476c-a24a-0ea55c65df44" alt="description" width="800">
</div>

## 
