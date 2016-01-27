#include "../include/network.h"
#include "../include/buffer.h"
#include "../include/uv.h"


void add_action_to_queue(uv_stream_t* client, char *content, uv_write_cb on_write_cb);


uv_loop_t *loop;

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

void input_string(char *str)
{
	// clear input buffer
	fflush(stdin);
	fgets(str, MAX_LINE, stdin);
	str[strlen(str)-1] = '\0';
	
	return str;
}


void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
        buf->base = (char*)malloc(suggested_size);
        buf->len = suggested_size;

}

void echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
     char *str = malloc(nread + 1);
     strncpy(str, buf->base, nread);
     str[nread] = '\0';
     printf("%s\n", str);
     free(str);
     uv_stop(loop);
}

void echo_write(uv_write_t *req, int status)
{
        if(status) {
                fprintf(stderr, "Write error %s\n", uv_strerror(status));
        }
        uv_stream_t *client = req->handle;
        uv_read_start(client, alloc_buffer, echo_read);

        free(req);
}

void add_action_to_queue(uv_stream_t* client, char *content, uv_write_cb on_write_cb) {
	uv_write_t *req_write = (uv_write_t*)malloc(sizeof(uv_write_t));
	uv_buf_t wrbuf = uv_buf_init(content, strlen(content));
	uv_write(req_write, client, &wrbuf, 1, on_write_cb);
}



// call back function
void on_connect(uv_connect_t *req, int status) {
        if (status < 0) {
                fprintf(stderr, "New connection error %s\n", uv_strerror(status));
                return;
        }
        uv_stream_t *client = req->handle;
        uv_write_t *reqWrite = (uv_write_t*)malloc(sizeof(uv_write_t));

        char *str ="hello libuv";
        uv_buf_t wrbuf = uv_buf_init(str, strlen(str));
        uv_write(reqWrite, client, &wrbuf, 1, echo_write);

}



int main(int argc, char *argv[])
{
	struct sockaddr_in dest;
	char recv_buf[MAX_LINE];
	char recv_mail[MAX_LINE]="0";
	char *str = NULL;
	int n;
	int data_len;
	char *username = NULL;
	char *password = NULL;
	char *cmd_type = NULL;

	username = (char*)malloc(MAX_LINE*sizeof(char));
	password = (char*)malloc(MAX_LINE*sizeof(char));
	cmd_type = (char*)malloc(MAX_LINE*sizeof(char));

	loop = uv_default_loop();
	uv_tcp_t* socket = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
	if (socket == NULL) {
		printf("socket malloc failed.\n");	
	}

	uv_tcp_init(loop, socket);

	printf("Input username:\n");
	username = input_string(username);
	printf("Input password:\n");
	password= input_string(password);

	printf("Input cmd_type:\n");
	cmd_type = input_string(cmd_type);


	switch(interpret_mail_type(cmd_type))
	{
		case 0:
	
				uv_connect_t* connect = (uv_connect_t*)malloc(sizeof(uv_connect_t));
				if (connect == NULL) {
					printf("connect malloc failed.\n");	
				}

				uv_ip4_addr("127.0.0.1", DEFAULT_PORT, &sin);
				uv_tcp_connect(connect, socket, (struct sockaddr*)&dest, on_connect);

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
	
	}
	uv_run(loop, UV_RUN_DEFAULT);
		
	return 0;

}
