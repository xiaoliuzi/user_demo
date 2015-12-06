/**
 * mysql connect test!
 * compiler:
 * gcc -o mysqlquery mysqlquery.c $(mysql_config --libs)
 */


/*   http://dev.mysql.com/doc/refman/5.6/en/c-api-function-overview.html  */


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
		if (status == 0) {
			printf("选择数据库成功!\n");
			status = mysql_query(mysql, selectsql);
			if (status == 0){
				
				/* mysql_store_result函数获取上一次mysql_query的执行结果，返回的是个结果指针结构 */
                /* mysql_fetch_row的第一个参数就是要结果指针结构，mysql_fetch_row是获取行的意思*/
				res = mysql_store_result(mysql);
				/*
				 * mysql_field_count 是获取到你设计的表被select语句执行后的列数。 
				 */
				field_count = mysql_field_count(mysql);
				printf("字段个数: %u \n", field_count);
				/*
				printf("%s\n", row[0]);
				printf("%s\n", row[1]);
				printf("%s\n", row[2]);
				*/
				row_count = res->row_count;
				printf("记录行数: %d \n", row_count);
				/* 输出所有行 */
				while (row = mysql_fetch_row(res)){
					for (i=0; i<field_count; i++){
						printf("%s\n", row[i]);
					}	

				}
				mysql_free_result(res);

			} else {
					printf("%s\n", mysql_error(conn));	
			}

		}else{
			printf("%s\n", mysql_error(conn));
		}
		
	} else {
		printf("数据库连接失败!\n");
	}

	/* 关闭数据库连接 */
	mysql_close(conn);
	
	free(mysql);


	return 0;
}
