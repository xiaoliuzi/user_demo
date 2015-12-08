all:server client
server:server.c my_operation_sql.c
	gcc -o server server.c my_operation_sql.c $(mysql_config --libs)
client:client.c
	gcc client.c -o client
clean:
	rm *.o server client
