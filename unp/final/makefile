all:server client
server:server_fork.c my_operation_sql.c
	gcc -o server server_fork.c my_operation_sql.c $(mysql_config --libs)
client:client.c
	gcc client.c -o client
clean:
	rm *.o server client
