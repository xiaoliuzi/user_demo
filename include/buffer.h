/* buffer.h */
#ifndef _BUFFER_H_
#define _BUFFER_H_


#define EINTR 9999
#define MAX_LINE 100
#define MAX_MAIL_LEN 1000

struct buf{
	int op;						/* the request type from client */
	int len;					/* the dynamic-length allocuated buffer*/
	char data[0];				/* the offset is equal to pointer to len buffer */
};


/* Initialize a buffer */
struct buf* init_buf(int op, int len);



#endif /* buffer.h */
