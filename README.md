# MyTinyWebServer
c++11  and cmake for tinyWebServer
* 使用CMake构建
* 所有模块放在tinyWebserve目录下
* 所有测试程序放在test目录下， 可执行文件放在bin目录下
* 日志模块可选择同步/异步写入日志
* 数据库连接池模块，底层为链表，为了实现用户账户密码登录，自定义信号量保证数据库连接池在多线程使用中的线程安全


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

## 定时器模块和信号处理
### 定时器模块

>定时器模块用来处理那些已经关闭的客户端或长时间没有消息的客户端，将他们的信息从epoll中剔除，关闭连接，释放内存资源。
>先理解了定时器处理的底层逻辑，再来看整个服务器是如何通过信号处理来调用定时器模块实现删除节点关闭连接的。

具体运行逻辑如下：
* 主线程定时通过alarm定时器发送SIGALRM出发自定义的扫描函数堆定时器双链表进行扫描
* 每个客户端链接的信息都保存在定时器链表的一个节点上，包括链接的socket文件描述符，连接时间
* 客户端和服务器通信一次，就会增加客户端在定时器链表上结点的链接时间，连接时间长的放在定时器链表后面，短的放前面
* 主线程在扫描的时候，连接结点的链接时间小于TIMESLOT值的，就会被清除，节点删除，对应文件描述符关闭

<img src="https://github.com/user-attachments/assets/cc74604b-dc2f-41f6-b6c7-3b272472de9f" alt="description" style="display:block; margin-left:auto; margin-right:auto; width:80%;">

### 信号处理逻辑

问题1：主线程是如何捕获到信号的呢？
>首先信号的发出与捕获都是在主线程中进行的，不牵扯到负责逻辑处理的子线程。
>首先，注册信号处理函数，关键是要捕获`SIGALRM`和`SIGTERM`信号。第一个表示定时器结束后的信号，这是关键。这两个信号的注册函数都是通过管道发送信息，发送的信息本身就是信号字符串比如`SIGALRM`信号就发送`"SIGALRM"`字符串。这样就可以不断调用`alarm`函数来触发`SIGALRM`信号了，触发了信号就会通过管道发送消息。

问题2：为什么要通过管道进行通信呢？
>这里我们将管道的读端`m_pipfd[0]`也注册进了`epoll`中，这样有什么好处呢？我们可以像处理普通的客户端数据一样去查看`m_pipfd[0]`是否有信息发送，并处理发送来的信息。可以将`m_pipfd[0]`对应的检测属性设置为`EPOLLIN`每当每当有数据发送过来，`m_pipfd[0]`就会进入`epoll`的就绪队列，然后像普通的客户端socketfd一样被处理。

问题3：那`m_pipfd[0]``recv`到的信息是如何被处理的呢？
>我们知道，m_pipfd[0]这个管道读端会收到两种信号`SIGALRM`和`SIGTERM`信号，如果收到`SIGTERM`就让服务器停止就行了，这里就不多说了，如果收到`SIGALRM`的话，就轮到我们的定时器出马了。这时候，就要去检查我们的定时器链表，挨个看每个节点是不是到时见了，到时间了的清除。遍历并清除完一遍定时器链表后，立马重新设置定时器`alarm`等待下一次收到信号来检查链表。

![信号处理和定时器总览 excalidraw](https://github.com/user-attachments/assets/9de814d4-ab85-479c-adca-552939b4e724)




