#include <stdio.h>
#include "my_operation_sql.h"
//#include <mysql/mysql.h>


MYSQL * my_conn_db(struct CONN conn_db)
{
	MYSQL *conn = NULL;
	
	conn = mysql_real_connect(conn_db.mysql, conn_db.host, conn_db.user, conn_db.password, conn_db.db, conn_db.port, conn_db.socket_file, conn_db.flags);
	if (conn != NULL) {
		printf("Connect Database success\n");
	}
	else{
		printf("Connect Database failed\n");
	}

	
	return conn;

}

int my_create_db(MYSQL* mysql, char *db_name)
{
	int status;
    char sql_query[MAX_SQL_LEN] = "create database ";

	
	strcat(sql_query, db_name);

	
	status = mysql_query(mysql, sql_query);

	if (status == 0) {
		printf("Create database success\n");
	}else{
		printf("Create database failed\n");
	}

	return status;
}


int my_select_db(MYSQL *mysql, char *db_name)
{
	int status;

	status = mysql_select_db(mysql, db_name);
	if (status == 0) {
		printf("Select datatbase success\n");
	} else {
		printf("Select database failed\n");
	}
	
	return status;
}


int my_create_table(MYSQL *mysql, char *sql_query)
{
	int status;
	
	status = mysql_query(mysql, sql_query);
	if (status == 0) {
		printf("Create table success\n");
	} else {
		printf("Create table failed\n");
	}
	
	return status;

}

int my_insert_record(MYSQL *mysql, char *sql_add_record) 
{
	int status;
	
	status = mysql_query(mysql, sql_add_record);

	if (status == 0) {
		printf("Insert %lu rows\n", (unsigned long)mysql_affected_rows(mysql));
	} else {
		fprintf(stderr, "Insert error %d : %s \n", mysql_errno(mysql), mysql_error(mysql));
	}

	
	return status;
}

int my_query(MYSQL *mysql, char *sql_query)
{
	int status;
	status = mysql_query(mysql, sql_query);

	if (status == 0) {
		printf("Query table success\n");
	} else {
		printf("Query table failed\n");
	}
		
	return status;
}
