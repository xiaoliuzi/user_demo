#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define EINTR 9999

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





#define MAX_LINE 100

int main(int argc, char *argv[])
{

	struct sockaddr_in sin;
	char buf[MAX_LINE];
	int s_fd;
	int port=8000;
	char *str = NULL;
	int n;

	char username[MAX_LINE];
	char password[MAX_LINE];


	printf("Input username:\n");
	scanf("%s", username);
	
	printf("Input password:\n");
	scanf("%s", password);
	
	str = username;	
		
	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &sin.sin_addr);
	sin.sin_port = htons(port);
	s_fd = socket(AF_INET, SOCK_STREAM, 0);
	connect(s_fd, (struct sockaddr *)&sin, sizeof(sin));

	// send the length of the string.
	//write(s_fd, username,
	// send the content
	writen(s_fd, str, strlen(str)+1);

	printf("before send paasword\n");
//	fflush(s_fd);
	writen(s_fd, password, strlen(password)+2);
	printf("after send paasword\n");
	
	readn(s_fd, buf, MAX_LINE);
	printf("receive from server:\n%s\n", buf);

	for (n=0; n< 10000; n++);
	close (s_fd);
	return 0;

}
