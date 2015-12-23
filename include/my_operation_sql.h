/* my_operation_sql.h
 * Write my own mysql function to operate the database.
 */
#ifndef _MY_OPERATION_SQL_H_
#define _MY_OPERATION_SQL_H_

#include <mysql/mysql.h>

#define MAX_SQL_LEN 100


/* MYSQL structure */
struct CONN{
	MYSQL *mysql;
	char *host;				/* host machine */
	char *user;				/* username */
	char *password;				/* password */
	char *db;				/* database */
	int port;				/* port */
	char *socket_file;			/* socket file */
	int flags;				/* flags */
};

/* my connect database */
MYSQL* my_conn_db(struct CONN conn_db);

/* my create database */
int my_create_db(MYSQL *mysql, char *db_name);

/* my select databse */
int my_select_db(MYSQL *mysql, char *db_name);

/* my create table */
int my_create_table(MYSQL *mysql, char *sql_query);

/* my insert record */
int my_insert_record(MYSQL *mysql, char *sql_add_record);

/* my query database */
int my_query(MYSQL *mysql, char *sql_query);

#endif /* my_operation_sql.h */

