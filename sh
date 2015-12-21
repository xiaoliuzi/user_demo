gcc -o server server_fork.c my_operation_sql.c -L/usr/lib/x86_64-linux-gnu -lmysqlclient -lpthread -lz -lm -ldl

gcc -o server server_fork.c my_operation_sql.c $(mysql_config --libs)
