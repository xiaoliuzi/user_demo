/* 分离mail体，并将mail类型，用户名，邮件内容分别存入相应的邮件体成员变量中
 * 可以在客户端发送之前，来将各个消息中的分隔符( 比如这里的maildata中的空格)
 * 更换为'\0'.
 */
#include "mail_body.h"

struct mail_body* mail_separate(char *mail_data)
{
        int maildata_i;/* maildata 的专用索引*/
        int j; /* 分解消息的公共索引*/
        struct mail_body *mailbody=(struct mail_body*)malloc(sizeof(struct mail_body));

        for (maildata_i=0, j=0; mail_data[maildata_i] != ' '; ++maildata_i, ++j) {
                mailbody->type[j] = mail_data[maildata_i];
        }
        mailbody->type[j] = '\0';


        for (maildata_i += 1, j=0; mail_data[maildata_i] != ' '; ++maildata_i, ++j) {
                mailbody->name[j] = mail_data[maildata_i];
        }
        mailbody->name[j] = '\0';
        for (maildata_i += 1, j=0; mail_data[maildata_i] != '\0'; ++maildata_i, ++j) {
                mailbody->content[j] = mail_data[maildata_i];
        }
        mailbody->content[j] = '\0';

        return mailbody;
}

