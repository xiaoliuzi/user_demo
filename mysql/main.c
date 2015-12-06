/*
 * Write my own mysql function to operate the database.
 */


#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>

#define MAX_SQL_LEN 100
#define MAX_RECORD 10



struct CONN{
	MYSQL *mysql;
	char *host;
	char *user;
	char *password;
	char *db;
	int port;
	char *socket_file;
	int flags;
};

MYSQL * my_conn_db(struct CONN *conn_db)
{
	MYSQL *conn = NULL;
	
	conn = mysql_real_connect(conn_db->mysql, conn_db->host, conn_db->user, conn_db->password, conn_db->db, conn_db->port, conn_db->socket_file, conn_db->flags);
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

int main()
{
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
	char *sql_select_table = "select * from test_table";
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
	sql_create_table = "create table test_table (id int not null, username char(20), password char(16)";	


	
	conn = my_conn_db(conn_db);
	status = my_create_db(mysql, db_name);
	status = my_select_db(mysql, db_name);
	status = my_create_table(mysql, sql_create_table);
	for (i=0; i<MAX_RECORD; i++) {
		status = my_insert_record(mysql, sql_insert_record[i]);
	}
	status = my_query(mysql, sql_select_table);


	res = mysql_store_result(mysql);	
	field_count = mysql_field_count(mysql);
	printf("Number of column : %u \n", field_count);
	
	row_count = res->row_count;
	printf("Number of row : %d \n", row_count);	
	while(row = mysql_fetch_row(res)) {
			for (i=0; i<field_count; i++) {
				printf("%s\n", row[i]);
			}
	}	
	
	mysql_free_result(res);
	

	mysql_close(conn);
	

	free(conn_db);
	free(mysql);
	return 0;

}
