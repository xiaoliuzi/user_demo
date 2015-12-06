/**
 * mysql connect test!
 * compiler:
 * gcc -o mysqlcreatedb mysqlcreatedb.c $(mysql_config --libs)
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

int main()
{
	MYSQL *mysql = (MYSQL*)malloc(sizeof(MYSQL));
	
	/* Initialize MYSQL structure */
	mysql_init(mysql);
	MYSQL *conn;
	char *createstat = "create database testdb;";
	int status;

	conn = mysql_real_connect(mysql, "localhost", "root", "", NULL, 0, NULL, 0);
	
	if (conn != NULL) {
		printf("Database Connection successfully\n");
		status = mysql_query(mysql, createstat);
		if (status == 0)
			printf("Create database successfully\n");
		else /* Create database failed */
			printf("%s\n", mysql_error(conn));
			
	} else
		printf("Connect to database failed\");

	mysql_close(conn);

	free(mysql);
	return 0;

}
