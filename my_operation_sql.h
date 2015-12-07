/*
 * Write my own mysql function to operate the database.
 */
#ifndef _MY_OPERATION_SQL_H
#define _MY_OPERATION_SQL_H



#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>

#define MAX_SQL_LEN 100
#define MAX_RECORD 7



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

#endif
