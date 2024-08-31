# MyTinyWebServer
c++11  and cmake for tinyWebServer

## log日志模块

> Log日志模块实现可选同步日志和异步日志。
> 在异步日志中，实现了阻塞队列，在写入日志的时候，先将日志信息放入阻塞队列中，
> 异步日志只要检测到队列不空，就持续调用`pop`将队列中的日志信息异步写入到日志文件中。

![日志模块实现逻辑 excalidraw](https://github.com/user-attachments/assets/fe3e560f-d3dd-4c3b-b66d-286e48740df7)
