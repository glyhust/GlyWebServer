#include <mysql/mysql.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <list>
#include <pthread.h>
#include <iostream>
#include "sql_connection_pool.h"

using namespace std;

connection_poll::connection_poll()
{
    m_CurConn=0;
    m_FreeConn=0;
}

connection_poll* connection_poll::GetInstance()
{
    static connection_poll connPool;
    return &connPool;
}

void connection_poll::init(string url,string User,string Password,string Databasename,int Port,int Maxconn,int close_log)
{
    m_url=url;
    m_Port=Port;
    m_User=User;
    m_Password=Password;
    m_Databasename=Databasename;
    m_close_log=close_log;

    for(int i=0;i<Maxconn;i++)
    {
        MYSQL* con=NULL;
        con=mysql_init(con);
        if(con==NULL)
        {
            LOG_ERROR("MySQL Error");
            exit(1);
        }
        con=mysql_real_connect(con,url.c_str(),User.c_str(),Password.c_str(),Databasename.c_str(),Port,NULL,0);
        if(con==NULL)
        {
            LOG_ERROR("MySQL Error");
            exit(1);
        }
        connList.push_back(con);
        m_FreeConn++;
    }
    reserve=sem(m_FreeConn);
    m_MaxConn=m_FreeConn;
}

MYSQL* connection_poll::GetConnection()
{
    MYSQL* con=NULL;
    if(connList.size()==0)
        return NULL;
    
    reserve.wait();
    lock.lock();
    con=connList.front();
    connList.pop_front();

    m_FreeConn--;
    m_CurConn++;

    lock.unlock();
    return con;
}

bool connection_poll::ReleaseConnection(MYSQL* con)
{
    if(con==NULL)
        return false;
    
    lock.lock();
    connList.push_back(con);
    m_FreeConn++;
    m_CurConn--;
    lock.unlock();

    reserve.post();
    return true;
}

void connection_poll::DestroyPool()
{
    lock.lock();
    if(connList.size()>0)
    {
        list<MYSQL*>::iterator it;
        for(it=connList.begin();it!=connList.end();it++)
        {
            MYSQL* con=*it;
            mysql_close(con);
        }
        m_CurConn=0;
        m_FreeConn=0;
        connList.clear();
    }
    lock.unlock();
}

int connection_poll::GetFreeconn()
{
    return this->m_FreeConn;
}

connection_poll::~connection_poll()
{
    DestroyPool();
}

connectionRAII::connectionRAII(MYSQL** SQL,connection_poll* connpool)
{
    *SQL=connpool->GetConnection();
    conRAII=*SQL;
    poolRAII=connpool;
}
connectionRAII::~connectionRAII()
{
    poolRAII->ReleaseConnection(conRAII);
}