/* mail_body.h */
#ifndef _MAIL_BODY_H_
#define  _MAIL_BODY_H_

#include "buffer.h"

/* mail type */
enum mail_type{GET_MAIL, MAIL};


/* mail body structure */
struct mail_body{
	char type[MAX_MAIL_LEN];		/* mail request type */
	char name[MAX_MAIL_LEN];		/* receiver name */
	char content[MAX_MAIL_LEN];		/* mail content */
};


/* Separate the mail body and put the mail-type, receiver, mail-content */
struct mail_body* mail_separate(char *mail_data);


#endif /* mail_body.h */
