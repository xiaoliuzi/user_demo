/**
 * mysql connect test!
 * compiler:
 * gcc -o mysqlconn mysqlconn.c $(mysql_config --libs)
 */

#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

int main()
{
	MYSQL *mysql = (MYSQL*)malloc(sizeof(MYSQL));
	
	/* Initialize the MYSQL structure */
	mysql_init(mysql);
	MYSQL *conn;
								/* host machine   username password     database     port    socket file    flags  */
	conn = mysql_real_connect(mysql, "localhost", "root", "",          NULL,         0,        NULL,         0);

	if (conn != NULL)
		printf("Connect Database successfully.\n");
	else
		printf("Connect Datatbase failed\n");
	
	/* Close the connection to database */
	mysql_close(conn);
	
	free(mysql);
	return 0;
	
}

/*
 * This program is to connect the database with username "root" and the password is null "". 
 * Before you can operate the database, you must to connect the database successfully.
 * There are two ways to connect database, one is using tcp protocol, the other is with socket files.
 * In this program, we use the tcp protocol, so the socket parameters is NULL.
 * If you want to establish the connection by socket file,  the host machine should be NULL.
 * In this program, we don't have an database, so the corresponding parameter of database is NULL.
 * Generally, the configure file is located in /etc/mysql/my.cnf, and the content of the my.cnf is like this below:
 * 
 * 		[client]
        port		=3306
		socket		=/var/run/mysqld/mysqld.sock 
 * 
 * So, the default port is 3306, in this program, set port to 0 is equal to 3306, both of them are right. 
 * If you use socket to connect, you can set host machine to NULL, and set the port to 0 directly.
 * This program does not select database, as there is not still any availabel database.
 * The flags parameters is a little comprehensive, so just set it to 0 generally.
 */
