#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define EINTR 9999
#define MAX_MAIL_LEN 1000

/* define the mail struct to contain the mailtype mail_length and mail_content */
struct buf{
	int op;
	int len;
	char data[0];
};


// page 72
ssize_t      /* Read "n" bytes from a descriptor. */
readn( int fd, void *vptr, size_t n)
{
	size_t nleft;
	ssize_t nread;
	char *ptr;
	int errno=0;
	
	ptr = vptr;
	nleft = n;
	while (nleft > 0) {
		if ( (nread = read(fd, ptr, nleft)) < 0) {
			if ( errno == EINTR)
				nread = 0;     /* and call read() again */
			else
				return (-1);

		} else if (nread == 0)
			break;            /* EOF */
		
		nleft -= nread;
		ptr += nread;
	}

	return (n - nleft);      /* return >= 0 */
}





//page 73
ssize_t                       /* Write "n" bytes to a descriptor. */
writen(int fd, const void *vptr, size_t n)
{
	size_t nleft;
	ssize_t nwritten;
	const char *ptr;
	int errno=0;


	ptr = vptr;
	nleft = n;
	while( nleft > 0) {
		if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
			if (nwritten < 0 && errno == EINTR)
				nwritten = 0;   /*adn call write() again */
			else
				return (-1);    /* error */
		}	
		
		nleft -= nwritten;
		ptr += nwritten;
	}

    return (n);
}
/*  某大神推荐的定义方法，可以根据长度动态改变长度 */
struct buf* init_buf(int op, int len){
	struct buf* that = (struct buf*)malloc(sizeof(struct buf) + sizeof(char)*len);
	that->op = op;
	that->len = len;

	return that;
}



#define MAX_LINE 100

int main(int argc, char *argv[])
{
	struct sockaddr_in sin;
	char recv_buf[MAX_LINE];
	char recv_mail[MAX_LINE]="0";
	int s_fd;
	int port=8000;
	char *str = NULL;
	int n;
	int data_len;

	char username[MAX_LINE];
	char password[MAX_LINE];

	printf("Input username:\n");
	fgets(username, MAX_LINE, stdin);
	username[strlen(username)-1] = '\0';
	fputs(username, stdout);
	printf("\n");	

	printf("Input password:\n");
	fgets(password, MAX_MAIL_LEN, stdin);
	password[strlen(password)-1] = '\0';
	fputs(password, stdout);
	printf("\n");	
	

	
	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &sin.sin_addr);
	sin.sin_port = htons(port);
	s_fd = socket(AF_INET, SOCK_STREAM, 0);
	n = connect(s_fd, (struct sockaddr *)&sin, sizeof(sin));
	printf("connect status:%d\n", n);



	data_len = strlen(username)+1;
   	n = writen(s_fd, &data_len, sizeof(data_len));
	printf("write username len status:%d\n", n);
	n = writen(s_fd, username, strlen(username)+1);
	printf("write username status:%d\n", n);

	data_len = strlen(password)+1;
   	n = writen(s_fd, &data_len, sizeof(data_len));
	printf("write password len status:%d\n", n);
	n = writen(s_fd, password, strlen(password)+1);
	printf("write password status:%d\n", n);
	
//	readn(s_fd, recv_buf, MAX_LINE);
//	printf("receive log from server:%s\n", recv_buf);

	/* send mail like this:mail xlz abc */
	struct buf *that = init_buf(1,MAX_MAIL_LEN);
	printf("input your mail like this:mail username mailcontent\n");
	fgets(that->data, MAX_MAIL_LEN, stdin);
	that->data[strlen(that->data)-1] = '\0';
	fputs(that->data, stdout);
	printf("\n");
	
	/* 除了字符串等单字节以外，传递所有参数的整数 都需要进行字节序转换，同时接收方也需进行相应的逆转换 */
	//data_len = htonl(strlen(that->data));
	
	data_len = strlen(that->data)+1;
    n = writen(s_fd, &data_len, sizeof(data_len));
	printf("write content len status:%d\n", n);
	n = writen(s_fd, that->data, strlen(that->data)+1);
	printf("write conetent status:%d\n", n);
	
		
	readn(s_fd, recv_buf, MAX_LINE);
	printf("receive mail from server:%s\n", recv_buf);
	
	readn(s_fd, recv_mail, MAX_LINE);
	printf("receive mail from server:%c\n", recv_mail[0]);
	printf("receive mail from server:%c\n", recv_mail[1]);
	printf("receive mail from server:%c\n", recv_mail[2]);
	printf("receive mail from server:%c\n", recv_mail[3]);

	for (n=0; n< 10000; n++)
	for (n=0; n< 10000; n++);
	close (s_fd);
	return 0;

}
