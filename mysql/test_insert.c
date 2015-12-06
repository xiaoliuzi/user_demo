#include <stdlib.h>
#include <stdio.h>
#include <mysql.h>

int main()
{
	MYSQL my_connecyion;
	int res;

	
	mysql_init(&my_connecyion);


	if(mysql_real_connect(&my_connecyion,"localhost", "root", "nfs123", "foo", 0, NULL, 0))
	{
		printf("Connection success\n");

		// execute the SQL cmd
		res = mysql_query(&my_connecyion, "INSERT INTO children(fname, age) VALUES('Ann', 3)");
	
		if (!res)
			printf("Inserted %lu rows\n", (unsigned long)mysql_affected_rows(&my_connecyion));
		else
			fprintf(stderr, "Insert error %d : %s \n", mysql_errno(&my_connecyion), mysql_error(&my_connecyion));

		mysql_close(&my_connecyion);
	}	

	else {
		fprintf(stderr, "Connection failed\n");
		if (mysql_errno(&my_connecyion));
			fprintf(stderr, "Connection error %d: %s\n", mysql_errno(&my_connecyion), mysql_error(&my_connecyion));
	}
	return EXIT_SUCCESS;

}
