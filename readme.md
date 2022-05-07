# GlyWebServer
使用C++语言在Linux环境下搭建轻量级Web服务器，属于网络编程的简单实现。
## 功能
* 结合线程池、socket套接字、epoll函数实现多线程的并发模型，通过标志位切换LT/ET模式、Reactor/模拟Proactor模式
* 结合主状态机、从状态机解析HTTP请求报文，可以处理GET和POST请求，返回服务器图片、视频资源
* 使用服务器内部数据库保存用户名、登录密码，实现用户通过浏览器进行注册、登录功能
* 基于有序链表实现定时器，关闭超时的非活跃连接
* 结合单例模式和阻塞队列实现异步写日志模式，记录服务器的运行状态

## 学习博客
* [main主函数](https://glyhust.github.io/2022/03/01/main主函数/)
* [自封装锁](https://glyhust.github.io/2022/03/02/自封装锁/)
* [线程池](https://glyhust.github.io/2022/03/05/线程池/)
* [数据库连接池](https://glyhust.github.io/2022/03/10/数据库连接池/)
* [日志系统](https://glyhust.github.io/2022/03/11/日志/)
* [定时器](https://glyhust.github.io/2022/03/15/定时器/)
* [http处理类](https://glyhust.github.io/2022/03/20/http处理类/)
* [WebServer类](https://glyhust.github.io/2022/03/24/WebServer类/)

## 运行环境
* Linux
* C++
* Mysql

## 服务器启动
* 安装MySQL数据库
```mysql
// 建立目标库
create database yourdb;
// 创建user表
USE yourdb;
CREATE TABLE user
(
    username char(50) NULL,
    passwd char(50) NULL
)ENGINE=InnoDB;
```
* 修改Webserver.cpp中数据库初始化信息
```cpp
m_user = "root";             //登录数据库用户名
m_passWord = "1";            //登录数据库密码
m_databaseName = "yourdb";   //数据库名
```
* 编译及启动
```
 make
 ./server
```
* 浏览器端
```
ip:9006  //默认端口号
```
## 压力测试
**测试环境**：Ubuntu 18  内存4G  处理器4核
```
./webbench-1.5/webbench -c 8000 -t 5 http://ip:port/
```
默认listenfd和connfd都处于LT模式
[![测试结果]

(https://s1.ax1x.com/2022/05/07/OlDMgf.png)](https://imgtu.com/i/OlDMgf)
## 致谢
Linux高性能服务器编程，游双著.
[@qinguoyi](https://github.com/qinguoyi/TinyWebServer)
