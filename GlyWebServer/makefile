server: main.cpp  ./timer/timer.cpp ./http/http_conn.cpp ./log/log.cpp ./sql/sql_connection_pool.cpp  webserver.cpp
	g++ -o server $^ -lpthread -lmysqlclient
clean:
	rm  -r server