#include <string>
#include "webserver.h"

int main()
{
    WebServer server;

    server.log_write();

    server.sql_pool();

    server.thread_pool();

    server.eventListen();

    server.eventLoop();

    return 0;
}