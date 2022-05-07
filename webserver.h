#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <cassert>
#include <sys/epoll.h>

#include "./threadpool/threadpool.h"
#include "./http/http_conn.h"

using namespace std;

const int MAX_EVENT_NUMBER=10000;
const int MAX_FD=65536;      
const int TIMESLOT=5;      

class WebServer
{
public:
    WebServer();
    ~WebServer();

    void log_write();
    void sql_pool();
    void thread_pool();
    void eventListen();
    void eventLoop();

    void timer(int connfd,struct sockaddr_in client_address);
    void adjust_timer(util_timer* timer);
    void deal_timer(util_timer* timer,int sockfd);

    bool dealclinetdata();
    bool dealwithsignal(bool& timeout,bool& stop_server);
    void dealwithread(int sockfd);
    void dealwithwrite(int sockfd);

public:
    int m_port;
    char* m_root;
    int m_log_write;
    int m_close_log;
    int m_actormodel;
    int m_pipefd[2];
    http_conn* users;
    connection_poll* m_connpool;
    string m_user;
    string m_passWord;
    string m_databaseName;
    int m_sql_num;
    threadpool<http_conn>* m_pool;
    int m_thread_num;
    epoll_event events[MAX_EVENT_NUMBER];
    int m_epollfd;
    int m_listenfd;
    int m_OPT_LINGER;
    int m_LISTENTrigmode;
    int m_CONNTrigmode;
    client_data* users_timer;
    Utils utils;
};

#endif