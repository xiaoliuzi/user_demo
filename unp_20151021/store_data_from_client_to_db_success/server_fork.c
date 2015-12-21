/* 
 * 该服务器程序提供一个最简单的服务，将字符串中的小写字母转换为大写字母。
 * 该服务器程序使用的是8000这个端口，在使用前必须保证没有其他的应用程序使用该端口。
 * 本实例采用服务器和客户端使用同一主机的方法，也就是说该通信实际是主机自己在和自己通信。
 * 服务器端的IP地址是127.0.0.1，表示本机。
 */
/*
 * 编译：运行make，得到两个可执行程序server和client
 * 运行：打开两个shell终端，分别运行两个程序，在第一个终端中运行：$./server, *                                                                
 *	在第二个终端中运行：$./client ChinA
 *                      recive from server: china
 *  在第一个终端（运行server的那个终端）显示为：
 *                      client IP is 127.0.0.1, port is 8000
 *                      content is : ChinA
 *
 *  How to compile:
 *    $gcc -o server server.c my_operation_sql.c $(mysql_config --libs)
 *
 * 本程序在通信过程中，为了保证read(接收)函数,提前知道需要读取几个字节，采用先判断字符串长度，
 * 然后事先提前将字符串长度发送过去
 */


#include "my_network.h"
#include "my_operation_sql.h"
#include "my_mail.h"
#include <mysql/mysql.h>
#include <stdlib.h>
#define EINTR 9999
struct mail_body{
	char type[MAX_MAIL_LEN];
	char name[MAX_MAIL_LEN];
	char content[MAX_MAIL_LEN];

};


/* 定义一个枚举类型来标识邮件类型 */ 
enum mail_type{GET_MAIL, MAIL};

// unp page 73
ssize_t                       /* Write "n" bytes to a descriptor. */
writen(int fd, const void *vptr, size_t n)
{
	size_t nleft;
	ssize_t nwritten;
	const char *ptr;
	int errno=0;


	ptr = vptr;
	nleft = n;
	while( nleft > 0) {
		if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
			if (nwritten < 0 && errno == EINTR)
				nwritten = 0;   /*adn call write() again */
			else
				return (-1);    /* error */
		}	
		
		nleft -= nwritten;
		ptr += nwritten;
	}

    return (n);
}



//unp page 72
ssize_t      /* Read "n" bytes from a descriptor. */
readn( int fd, void *vptr, size_t n)
{
	size_t nleft;
	ssize_t nread;
	char *ptr;
	int errno=0;
	
	ptr = vptr;
	nleft = n;
	while (nleft > 0) {
		if ( (nread = read(fd, ptr, nleft)) < 0) {
			if ( errno == EINTR)
				nread = 0;     /* and call read() again */
			else
				return (-1);

		} else if (nread == 0)
			break;            /* EOF */
		
		nleft -= nread;
		ptr += nread;
	}

	return (n - nleft);      /* return >= 0 */
}




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
/* 分离mail体，并将mail类型，用户名，邮件内容别存入相应的邮件体成员变量中 */
/* 可以在客户端发送之前，来将各个消息中的分隔符（比如这里maildata中的空格
 * 更换为'\0'.
 *
 */
struct mail_body* mail_separate(char *mail_data)
{
	//struct buf *that = init_buf(1, MAX_MAIL_LEN);
	int maildata_i;/* maildata 的专用索引*/
	int j; /* 分解消息的公共索引*/
	struct mail_body *mailbody=(struct mail_body*)malloc(sizeof(struct mail_body));

	for (maildata_i=0, j=0; mail_data[maildata_i] != ' '; ++maildata_i, ++j) {
		mailbody->type[j] = mail_data[maildata_i];
	}
	mailbody->type[j] = '\0';
	printf("mail type :%s\n", mailbody->type);



	for (maildata_i += 1, j=0; mail_data[maildata_i] != ' '; ++maildata_i, ++j) {
		mailbody->name[j] = mail_data[maildata_i];
	}
	mailbody->name[j] = '\0';
	printf("username :%s\n", mailbody->name);
	for (maildata_i += 1, j=0; mail_data[maildata_i] != '\0'; ++maildata_i, ++j) {
		mailbody->content[j] = mail_data[maildata_i];
	}
	mailbody->content[j] = '\0';
	printf("mail content :%s\n", mailbody->content);
	
	return mailbody;
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
	int pid;
	int send_len;
	int recv_len;

	MYSQL *mysql = (MYSQL*)malloc(sizeof(MYSQL));
	MYSQL *conn = NULL;

    struct CONN *conn_db = (struct CONN*)malloc(sizeof(struct CONN));
	int status;
    char *db_name = NULL;
	char *table_name = NULL;
	char *sql_uname_table = NULL;
	char *sql_mail_table = NULL;
	char *sql_insert_record[MAX_RECORD] = {
		"insert into user_table(id, username, password) values(1, 'Jenny','123456') ",
		"insert into user_table(id, username, password) values(2, 'Andrew','123456') ",
		"insert into user_table(id, username, password) values(3, 'Gravin','123456') ",
		"insert into user_table(id, username, password) values(4, 'Duncan','123456') ",
		"insert into user_table(id, username, password) values(5, 'Emma','12345') ",
		"insert into user_table(id, username, password) values(6, 'Alex','12345') ",
		"insert into user_table(id, username, password) values(7, 'Adrian','123456') "

	};
    /* mailid is auto increment */
	char sql_insert_mail[MAX_LINE] = "insert into mail_table ( mailtype, receiver, content) values( ";
//	char *sql_insert_mail = "insert into mail_table (mailid, mailtype, receiver, content) values(2,'mail', 'xlz', 'abcdefg'); ";
	enum mail_type mt;
	char sql_select_table[MAX_LINE] = "select password from user_table where username =";
	char *sql_query_test = "select * from mail_table";
	char *single_quotes = "'";
	char *comma = ",";
	char *r_parenthese = ")";
	MYSQL_RES *res = NULL;
	MYSQL_ROW row = 0;
	unsigned int field_count;
	unsigned long long row_count;
	int i;
	char *mail_type_ptr[] = {"getmail", "mail"};
	char str_tmp[MAX_LINE];
	int tmp;





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
	sql_uname_table = "create table user_table (id int not null, username char(20), password char(16)) ";
//	sql_mail_table = "create table mail_table (mailid int not null, sender char(20), receiver char(20), content char(MAX_MAIL_LEN),												type int,  status int, time char(100) ) ";
/*mailtype int not null*/
	sql_mail_table = "create table mail_table ( mailid int not null primary key auto_increment, mailtype char(50), receiver char(20), content char(100) ) ";

	
	conn = my_conn_db(*conn_db);
	status = my_create_db(mysql, db_name);
	status = my_select_db(mysql, db_name);
	status = my_create_table(mysql, sql_uname_table);
	for (i=0; i<MAX_RECORD; i++) {
		status = my_insert_record(mysql, sql_insert_record[i]);
	}

//	status = my_create_table(mysql, sql_mail_table);

/////////////////////////////////////////////////////////////////////////////////////

	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(port);
    

	l_fd = socket(AF_INET, SOCK_STREAM, 0);
	bind(l_fd, (struct sockaddr*)&sin, sizeof(sin));
	listen(l_fd, 10);
	printf("waiting ...\n");
	int len_content;
	for( ; ; ) {
		c_fd = accept(l_fd, (struct sockaddr*)&cin, &len);

		if ( (pid = fork())==0 ) {
			
			close(l_fd);    /* child closes listening socket */

			/* 读取username长度 */
			n = readn(c_fd, &len_content, sizeof(len_content));
			/* 读取username */
			n = readn(c_fd, username, len_content);
			//fputs(username, stdout);
	 		printf("username is :");	
			fputs(username, stdout);
			printf("\n");
	
			/* 读取password长度 */	
			n = readn(c_fd, &len_content, sizeof(len_content));
			/* 读取password内容 */
			n = readn(c_fd, password, len_content);
			//fputs(password, stdout);
			printf("password is :");
			fputs(password, stdout);
			printf("\n");

			inet_ntop(AF_INET, &cin.sin_addr, addr_p, sizeof(addr_p));
			printf("client IP is %s, port is %d\n", addr_p, ntohs(sin.sin_port));
		
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
			printf("From db %s's password: %s\n", username, row[0]);
			mysql_free_result(res);

			/* process the end of \0 , as the field from db doesn't have \0 */
			char tmp[100]={"0"};
			strcpy(tmp, row[0]	);
			strcat(tmp, "\0");
			status = strcmp(tmp,password);


	
			/* 接收客户端发来的邮件长度和内容*/	
			struct buf *that = init_buf(1, MAX_MAIL_LEN);
			/* 读取mail长度 */	
			n = readn(c_fd, &len_content, sizeof(len_content));
			/* 读取mail内容 */
			n = readn(c_fd, that->data, len_content);
			printf("content is :");
			fputs(that->data, stdout);
			printf("\n");




			n = 20;
			//buf = "\nlog in success\n";
			printf("password identify status:%d\n", status);
			if (status == 0) {
				writen(c_fd,"log OK" , n);
 			}
			else {
				printf("status = %d\n", status);
				writen(c_fd, "log failed", n);
			}

		   	/* 解析邮件内容 */ 
			struct mail_body *mb = mail_separate(that->data);
			
#if 0
			/*如果邮件体第一个命令是mail*/
			if (strcmp(mb->type, "mail") == 0) {
				mt = GET_MAIL;
				
			} else if (strcmp(mb->type, "getmail") == 0)/*如果邮件体第一个命令是getmail*/
			{
				mt = MAIL;
			}
			
			itoa(mt, str_tmp,10);	
#endif 	


			printf("before select\n");
			status = my_select_db(mysql, db_name);
			printf("before create mail table\n");
			status = my_create_table(mysql, sql_mail_table);
			
			// combine the sql query
			printf("mb->content=%s\n", mb->type);
			printf("mb->content=%s\n", mb->name);
			printf("mb->content=%s\n", mb->content);
			//strcat(sql_insert_mail, comma);
			strcat(sql_insert_mail, single_quotes);
			strcat(sql_insert_mail, mb->type);
			strcat(sql_insert_mail, single_quotes);
			strcat(sql_insert_mail, comma);
			strcat(sql_insert_mail, single_quotes);
			strcat(sql_insert_mail, mb->name);
			strcat(sql_insert_mail, single_quotes);
			strcat(sql_insert_mail, comma);
			strcat(sql_insert_mail, single_quotes);
			strcat(sql_insert_mail, mb->content);
			strcat(sql_insert_mail, single_quotes);
			strcat(sql_insert_mail, r_parenthese);
			printf("sql_insert = %s\n", sql_insert_mail);	
			
			printf("before insert\n");
			status = my_insert_record(mysql, sql_insert_mail);
			
			printf("before select\n");
			// Query the database 
		 	//status = my_query(mysql, sql_select_table);
		 	status = my_query(mysql, sql_query_test);
			printf("after select\n");
	

			if(status == 0) {
				res = mysql_store_result(mysql);
				field_count = mysql_field_count(mysql);
				printf("number of field: %u \n", field_count);
				row_count = res->row_count;
				printf("number of record: %d \n", row_count);
			} else {
				printf("query select error\n");
			}
			
	
		//	res = mysql_store_result(mysql);
			field_count = mysql_field_count(mysql);
			printf("Number of column :%u\n", field_count);
			
			printf("before get res->row_count\n");	
			if (res == NULL)
				printf("res if null\n");
			row_count = res->row_count;
			printf("after get res->row_count\n");	
			printf("Number of row :%d\n", row_count);
			row = mysql_fetch_row(res);
			printf("From db content: %s\n",  row[0]);
			printf("From db content: %s\n",  row[1]);
			printf("From db content: %s\n",  row[2]);
			printf("From db content: %s\n",  row[3]);
			

	
			my_fun(buf);
			



			close(c_fd);  /* done with this client */
		    exit(0);     /* child terminates */	 
			} 


			close(c_fd); /* parent closes connected socket `*/
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
