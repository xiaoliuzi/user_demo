#include "unp.h"



//page 99
void 
str_echo(int sockfd)
{
	ssize_t n;
	char buf[MAXLINE];
	
	again:
		while ( (n = read(sockfd, buf, MAXLINE) ) > 0)
			Writen(sockfd, buf, n);

		if (n < 0 && errno == EINTR)
			goto again;
		else if (n < 0)
			err_sys("str_echo: read error");
}

// page 72
ssize_t      /* Read "n" bytes from a descriptor. */
readn( int fd, void *vptr, size_t n)
{
	size_t nleft;
	ssize_t nread;
	char *ptr;
	
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
	
	ptr = vptr;
	nleft = n;
	while( nleft > 0) {
		if ( (nwritten = write(fd, ptr, nleft) <= 0) {
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





//page98
int 
main (int argc, char **argv)
{
	int listenfd, connfd;
	pid_t childpid;
	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	Bind(listenfd, (SA*) &servaddr, sizeof(servaddr));
	Listen(listenfd, LISTENQ);
	
	for ( ; ;) {
		clilen = sizeof(cliaddr);
		connfd = Accept(listenfd, (SA *)&cliaddr, &clilen);
		if ( (childpid = Fork() ) == 0) {	/* child proces */
			Close(listenfd);                /* close listening socket */
			str_echo(connfd);               /* process the request */
			exit(0);
		}
		Close(connfd);                      /* parent closes connected socket */

	}

}
