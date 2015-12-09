/* 
 * 该服务器程序提供一个最简单的服务，将字符串中的小写字母转换为大写字母。
 * 该服务器程序使用的是8000这个端口，在使用前必须保证没有其他的应用程序使用该端口。
 * 本实例采用服务器和客户端使用同一主机的方法，也就是说该通信实际是主机自己在和自己通信。
 * 服务器端的IP地址是127.0.0.1，表示本机。
 */
/*
 * 编译：运行make，得到两个可执行程序server和client
 * 运行：打开两个shell终端，分别运行两个程序，在第一个终端中运行：$./server,
 *                                                                
 *	在第二个终端中运行：$./client ChinA
 *                      recive from server: china
 *  在第一个终端（运行server的那个终端）显示为：
 *                      client IP is 127.0.0.1, port is 8000
 *                      content is : ChinA
 *
 *  How to compile:
 *    $gcc -o server server.c my_operation_sql.c $(mysql_config --libs)
 *
 */


#include "my_network.h"
#include "my_operation_sql.h"
#include <mysql/mysql.h>


void my_fun(char *p)
{
	if (p == NULL) {
		return ;
	}

	for (; *p != '\0'; p++) {
		if (*p >= 'A' && *p <= 'Z') {
			*p = *p - 'A' + 'a';
		}
	}
}














int main(void)
{
	struct sockaddr_in sin;
	struct sockaddr_in cin;
	int l_fd;
	int c_fd;
	socklen_t len;
	char buf[MAX_LINE];//username from client
	char username[MAX_LINE];//username from client
	char password[MAX_LINE];// password from client
	char addr_p[INET_ADDRSTRLEN];

	int port = 8000;
	int n;


	MYSQL *mysql = (MYSQL*)malloc(sizeof(MYSQL));
	MYSQL *conn = NULL;

    struct CONN *conn_db = (struct CONN*)malloc(sizeof(struct CONN));
	int status;
    char *db_name = NULL;
	char *table_name = NULL;
	char *sql_create_table = NULL;
	char *sql_insert_record[MAX_RECORD] = {
		"insert into test_table(id, username, password) values(1, 'Jenny','123456') ",
		"insert into test_table(id, username, password) values(2, 'Andrew','123456') ",
		"insert into test_table(id, username, password) values(3, 'Gravin','123456') ",
		"insert into test_table(id, username, password) values(4, 'Duncan','123456') ",
		"insert into test_table(id, username, password) values(5, 'Emma','123456') ",
		"insert into test_table(id, username, password) values(6, 'Alex','123456') ",
		"insert into test_table(id, username, password) values(7, 'Adrian','123456') "

	};
	char sql_select_table[MAX_LINE] = "select password from test_table where username =";
	char *single_quotes = "'";
	MYSQL_RES *res = NULL;
	MYSQL_ROW row = 0;
	unsigned int field_count;
	unsigned long long row_count;
	int i;


	/* Initialize the MYSQL structure */
	mysql_init(mysql);
		
    /* Initialize the connect database parameters */
	conn_db->mysql = mysql;
	conn_db->host = "localhost";
	conn_db->user = "root";
	conn_db->password = "";
	conn_db->db = NULL;
	conn_db->port = 0;
	conn_db->socket_file = NULL;
	conn_db->flags = 0;

	/* Initialize the database name */
	db_name = "testdb";

	/* Initialize the table name */
	sql_create_table = "create table test_table (id int not null, username char(20), password char(16)) ";


	
	conn = my_conn_db(*conn_db);
	status = my_create_db(mysql, db_name);
	status = my_select_db(mysql, db_name);
	status = my_create_table(mysql, sql_create_table);
	for (i=0; i<MAX_RECORD; i++) {
		status = my_insert_record(mysql, sql_insert_record[i]);
	}


/////////////////////////////////////////////////////////////////////////////////////



	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(port);
    
	l_fd = socket(AF_INET, SOCK_STREAM, 0);
	bind(l_fd, (struct sockaddr*)&sin, sizeof(sin));
	listen(l_fd, 10);
	printf("waiting ...\n");
	while(1) {
		c_fd = accept(l_fd, (struct sockaddr*)&cin, &len);


		n=read(c_fd, username, MAX_LINE);

		inet_ntop(AF_INET, &cin.sin_addr, addr_p, sizeof(addr_p));

		printf("client IP is %s, port is %d\n", addr_p, ntohs(sin.sin_port));
 		printf("username is :%s\n", username );	
		
		n = read(c_fd, password, MAX_LINE);			
		printf("password is :%s\n", password);
       

		
		// combine the sql query
		strcat(sql_select_table, single_quotes);
		strcat(sql_select_table, username);
		strcat(sql_select_table, single_quotes);
		// Query the database 
	 	status = my_query(mysql, sql_select_table);

		res = mysql_store_result(mysql);
		field_count = mysql_field_count(mysql);
		printf("Number of column :%u\n", field_count);
		
		row_count = res->row_count;
		printf("Number of row :%d\n", row_count);
		row = mysql_fetch_row(res);
		printf("%s's password: %s\n", username, row[0]);
	
		
		status = strcmp(row[0],password);

		my_fun(buf);
		n = 20;
		//buf = "\nlog in success\n";
        if (status == 0) {
			write(c_fd,"log OK" , n);
 		}
		else {
			write(c_fd, "log failed", n);
		}
		close(c_fd);
	} 

	if (close(l_fd) == -1) {
		perror("fail to close");
		exit(1);
	}



	mysql_free_result(res);
	mysql_close(conn);

	freee(conn_db);
	free(mysql);
	
	return 0;
} 