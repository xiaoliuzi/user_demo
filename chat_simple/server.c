/* 
 * 该服务器程序提供一个最简单的服务，将字符串中的小写字母转换为大写字母。
 * 该服务器程序使用的是8000这个端口，在使用前必须保证没有其他的应用程序使用该端口。
 * 本实例采用服务器和客户端使用同一主机的方法，也就是说该通信实际是主机自己在和自己通信。
 * 服务器端的IP地址是127.0.0.1，表示本机。
 */
/*
 * 编译：运行make，得到两个可执行程序server和client
 * 运行：打开两个shell终端，分别运行两个程序，在第一个终端中运行：$./server,
 *                                                                
 *	在第二个终端中运行：$./client ChinA
 *                      recive from server: china
 *  在第一个终端（运行server的那个终端）显示为：
 *                      client IP is 127.0.0.1, port is 8000
 *                      content is : ChinA
 */


#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <mysql/mysql.h>
#include <string.h>
#include <stdlib.h>


#define MAX_LINE 100


void my_fun(char *p)
{
	if (p == NULL) {
		return ;
	}

	for (; *p != '\0'; p++) {
		if (*p >= 'A' && *p <= 'Z') {
			*p = *p - 'A' + 'a';
		}
	}
}


int main(void)
{
	struct sockaddr_in sin;
	struct sockaddr_in cin;
	int l_fd;
	int c_fd;
	socklen_t len;
	char buf[MAX_LINE];
	char addr_p[INET_ADDRSTRLEN];
	int port = 8000;
	int n;

	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(port);
    
	l_fd = socket(AF_INET, SOCK_STREAM, 0);
	bind(l_fd, (struct sockaddr*)&sin, sizeof(sin));
	listen(l_fd, 10);
	printf("waiting ...\n");
	while(1) {
		c_fd = accept(l_fd, (struct sockaddr*)&cin, &len);
		n=read(c_fd, buf, MAX_LINE);
		inet_ntop(AF_INET, &cin.sin_addr, addr_p, sizeof(addr_p));
		printf("client IP is %s, port is %d\n", addr_p, ntohs(sin.sin_port));
 		printf("content is :%s\n",buf );	
		

		my_fun(buf);
		write(c_fd, buf, n);
		close(c_fd);
	} 

/*    
	if( (l_fd = socket(AF_INET, SOCK_STREAM, 0))== -1 ) {
		perror("fail to create socket");
		exit(1);

	}
	if (bind(l_fd, (struct sockaddr*)&sin, sizeof(sin)) == -1){
		perror("fail to bind");
		exit(1);
	} 
	if ( listen(l_fd, 10) ) {
		perror("fail to listen");
		exit(1);
	}
	printf("waiting ...\n");
	while(1) {
		if (c_fd = accept(l_fd, (struct sockaddr*)&cin, &len) == -1) {
			perror("fail to accept");
			exit(1);
		}
		if ((n=read(c_fd, buf, MAX_LINE)) == -1) {
			perror("fail to read");
			exit(1);
		}
		else if (n == 0) {
			printf("the connect has been close\n");
			close(c_fd);
			continue;
		}
		inet_ntop(AF_INET, &cin.sin_addr, addr_p, sizeof(addr_p));
		printf("client IP is %s, port is %d\n", addr_p, ntohs(sin.sin_port));
 		printf("content is :%s\n",buf );	
		

		my_fun(buf);
		if ( (n=write(c_fd, buf, n)) == -1) {
			exit(1);
		}
		if (close(c_fd) == -1) {
			perror("fail to close");
			exit(1);
		}
	} 
*/	
	if (close(l_fd) == -1) {
		perror("fail to close");
		exit(1);
	}
	return 0;
} 
