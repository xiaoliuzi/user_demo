/* buffer.h */
#ifndef buffer_h
#define buffer_h



#define MY_BUFFERSIZE 1024
#define MAX_MAIL_LEN 1000

struct buf{
	int op; 					/* the request type from client */
	int len;                	/* the dynamic-length allocuated buffer*/
	char data[0];				/* the offset is equal to pointer to len buffer */
};


/* Initialize a buffer */
struct buf* init_buf(int op, int len);



#endif /* buffer.h */
