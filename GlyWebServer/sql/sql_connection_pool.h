#ifndef _CONNECTION_POOL_
#define _CONNECTION_POOL_

#include <stdio.h>
#include <list>
#include <mysql/mysql.h>
#include <error.h>
#include <string.h>
#include <iostream>
#include <string>
#include "../lock/locker.h"
#include "../log/log.h"

using namespace std;

class connection_poll
{
public:
    static connection_poll* GetInstance();
    void DestroyPool(); 
    bool ReleaseConnection(MYSQL* con);
    MYSQL *GetConnection();	
    int GetFreeconn();
    void init(string url,string User,string Password,string Databasename,int Port,int Maxconn,int close_log);
private:
    connection_poll();
    ~connection_poll();
    
    string m_url;       
    string m_Port;        
    string m_User;         
    string m_Password;     
    string m_Databasename;  
    int m_close_log;     
    locker lock;
    list<MYSQL*> connList; 
    sem reserve;
    int m_MaxConn;
    int m_CurConn; 
    int m_FreeConn;
};

class connectionRAII
{
public:
    connectionRAII(MYSQL** con,connection_poll* connpool);
    ~connectionRAII();
private:
    MYSQL* conRAII;
    connection_poll* poolRAII;
};

#endif