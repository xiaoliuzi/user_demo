#include "../include/network.h"
#include "../include/buffer.h"
#include "../include/uv.h"

ssize_t      /* Read "n" bytes from a descriptor. */
readn( int fd, void *vptr, size_t n)
{
	size_t nleft;
	ssize_t nread;
	char *ptr;
	int read_errno=0;
	
	ptr = vptr;
	nleft = n;
	while (nleft > 0) {
		if ( (nread = read(fd, ptr, nleft)) < 0) {
			if ( read_errno == EINTR)
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

ssize_t                       /* Write "n" bytes to a descriptor. */
writen(int fd, const void *vptr, size_t n)
{
	size_t nleft;
	ssize_t nwritten;
	const char *ptr;
	int write_errno=0;


	ptr = vptr;
	nleft = n;
	while( nleft > 0) {
		if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
			if (nwritten < 0 && write_errno == EINTR)
				nwritten = 0;   /*adn call write() again */
			else
				return (-1);    /* error */
		}	
		
		nleft -= nwritten;
		ptr += nwritten;
	}

    return (n);
}


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

	printf("Input password:\n");
	fgets(password, MAX_MAIL_LEN, stdin);
	password[strlen(password)-1] = '\0';
	
	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &sin.sin_addr);
	sin.sin_port = htons(port);
	s_fd = socket(AF_INET, SOCK_STREAM, 0);
	n = connect(s_fd, (struct sockaddr *)&sin, sizeof(sin));

	data_len = strlen(username)+1;
   	n = writen(s_fd, &data_len, sizeof(data_len));
	n = writen(s_fd, username, strlen(username)+1);

	data_len = strlen(password)+1;
   	n = writen(s_fd, &data_len, sizeof(data_len));
	n = writen(s_fd, password, strlen(password)+1);
	
	struct buf *that = init_buf(1,MAX_MAIL_LEN);
	printf("input your mail like this:mail username mailcontent\n");
	fgets(that->data, MAX_MAIL_LEN, stdin);
	that->data[strlen(that->data)-1] = '\0';
	
	data_len = strlen(that->data)+1;
    n = writen(s_fd, &data_len, sizeof(data_len));
	n = writen(s_fd, that->data, strlen(that->data)+1);
		
	readn(s_fd, recv_buf, MAX_LINE);
	printf("receive mail from server:%s\n", recv_buf);
	
	close (s_fd);
	return 0;

}
