/**
 * mysql connect test!
 * compiler:
 * gcc -o mysqlquery mysqlquery.c $(mysql_config --libs)
 */

#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

int main()
{
	MYSQL *mysql = (MYSQL*)malloc(sizeof(MYSQL));
	
	/* 初始化MYSQL结构体 */
	mysql_init(mysql);
	MYSQL *conn;
	char *selectsql = "select * from testtable";
	int status;
	MYSQL_RES *res;
	MYSQL_ROW row;
	unsigned int field_count;
	unsigned long long row_count;
									/* 主机     用户名  密码 数据库  端口   socket文件 flags */
	conn = mysql_real_connect(mysql, "localhost", "root", "", NULL,    0,     NULL,      0);	
	
	if (conn != NULL) {
		printf("数据库连接成功！\n");

		/* 选择数据库testdb */
		status = mysql_select_db(mysql, "testdb");
		
	}
}
