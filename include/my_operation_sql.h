/* my_operation_sql.h
 * Write my own mysql function to operate the database.
 */
#ifndef _MY_OPERATION_SQL_H_
#define _MY_OPERATION_SQL_H_

#include <mysql/mysql.h>

#define MAX_SQL_LEN 100

struct CONN{
	MYSQL *mysql;
	char *host;				/* host machine */
	char *user;				/* username */
	char *password;			/* password */
	char *db;				/* database */
	int port;				/* port */
	char *socket_file;		/* socket file */
	int flags;				/* flags */
};

#endif /* my_operation_sql.h */

