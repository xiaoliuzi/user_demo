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
	
	conn = mysql_real_connect(mysql, "localhost", "root", "", NULL, 0, NULL, 0);

	if (conn != NULL)
		printf("Connect Database successfully.\n");
	else
		printf("Connect Datatbase failed\n");
	
	/* Close the connection to database */
	mysql_close(conn);
	
	free(mysql);
	return 0;
	
}
