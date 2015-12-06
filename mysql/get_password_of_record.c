
#include <stdio.h>

#include <mysql.h>

#include <string.h>

int main()

{

MYSQL mysql;    //mysql连接

MYSQL_RES *res; //这个结构代表返回行的一个查询结果集

MYSQL_ROW row; //一个行数据的类型安全(type-safe)的表示

char *query;  //查询语句   

int t,r;

mysql_init(&mysql);

if (!mysql_real_connect(&mysql,"localhost", "root", NULL, "test",0,NULL,0))

{

printf( "Error connecting to database: %s",mysql_error(&mysql));

}

else

{

printf("Connected...\n");

}

printf("Input username:\n");

scanf("%s", username);

/* query=" select * from qq"; */
query = " select password from qq where username = 'Ann' ";

t=mysql_real_query(&mysql,query,(unsigned int)strlen(query));//执行指定为计数字符串的SQL查询。

if(t)

{

printf("执行显示时出现异常: %s",mysql_error(&mysql));

}

res=mysql_store_result(&mysql);//检索完整的结果集至客户端。

printf("password\n");

while(row=mysql_fetch_row(res))

{                           

for(t=0;t<mysql_num_fields(res);t++)

{

printf("%s\t" ,row[t]);
// 这里的row[t]中的元素，就是查询到的字符串，也就是密码

}

printf("\n");

}

mysql_free_result(res);//释放结果集使用的内存。

mysql_close(&mysql);

return 0;

}



