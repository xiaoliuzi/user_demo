all:server client
server:server_fork.c my_operation_sql.c
	gcc -g server_fork.c my_operation_sql.c $(mysql_config --libs) -o server
client:client.c
	gcc client.c -o client
clean:
	rm *.o server client
