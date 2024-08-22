myTinyWebserver项目模块简述

## Semephore模块

**要点解析**

>c++11实现

* 使用`std::condition_variable`和`std::mutex`实现信号量机制
* 实现`wait`和`notify`分别用来添加减少信号量数和添加信号量数
* 使用`RAII(Resource Acquisition Is Initialization, 资源获取即初始化)`编程理念和工具,如`unique_lock`和`lock_guard`
* 禁止拷贝构造和拷贝赋值, 因为类成员`std::mutex`和`std::condition_variable`是不支持拷贝但是支持移动move的，所以`Semaphore`也只能使用移动move，而不能使用拷贝，会报错
* 使用移动构造和移动赋值move, 这里不需要对`std::mutex`和`std::condition_variable`进行拷贝操作。因为编译器会自动将原对象中的`mtx`和`cv`的所有权移从源对象移动到新对象。*注意：这些移动式隐式的、有编译器自动处理的*
    * 原对象中的`std::mutex`和`std::condition_variable`不会被复制，也不会与新对象共享资源。新对象完全接管了他们。

**移动构造和赋值要点**

```ad-attention
title: 移动构造函数移动赋值函数编写注意事项
当我们在编写移动构造函数和移动赋值函数的时候，我们可以手动指定每个成员，包括普通成员和类成员的移动方式，重点是要显示指明指针成员和其指向内容的移动方式。
*但是，对于那些我们没有显示在移动构造和移动赋值中指明如何移动的成员对象，尤其是类成员，则会调用其默认的移动构造或者移动赋值，如果该类对象的移动构造或者移动赋值被声明为`delete`则会编译不同过。* 同理，如果我们的类成员，的拷贝构造和拷贝赋值被声明为delete，只存在移动构造和移动赋值，则我们必须在当前类中声明对应的移动构造和移动赋值，并将拷贝构造和拷贝赋值设置为`delete`。
```
