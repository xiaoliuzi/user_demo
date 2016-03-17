#include <stdlib.h>
#include <string.h>
#include <uv.h>
#include <stdio.h>
#include <msgpack.h>
//#include <mail_body.h>
#include <assert.h>

/*
	gcc -lmsgpackc -pthread -o server libuv_server.c /usr/lib/libmsgpackc.a
*/
#define DEFAULT_PORT 7000
#define DEFAULT_BACKLOG 128


enum mail_type{
	MAIL_TYPE_LOGIN 		= 0x00,
	MAIL_TYPE_LOGOUT 		= 0x01,
	MAIL_TYPE_SINUP     	= 0x02,
	MAIL_TYPE_MAIL 			= 0x03,
	MAIL_TYPE_GET_MAIL 		= 0x04
};

struct mail_body{
	enum mail_type type;
	char username[MAX_LINE];
	char password[MAX_LINE];
};

struct symbol{
	char *single_quotes 	= " ' ";
	char *comma 			= " , ";
	char *r_parenthese		= " ) ";
};

uv_loop_t *loop;
struct sockaddr_in addr;

/*
把字符串str通过msgpack打包。

*/
void msgpack_pack_string(msgpack_packer* pk, const char *str) {
	msgpack_pack_str(pk, strlen(str));
	msgpack_pack_str_body(pk, str, strlen(str));
}

char * catenat_sql(struct mail_body m_body){

}

void add_newuser(MYSQL *mysql, struct mail_body *mb_ptr) {
	static int userid = 7;
	userid++;
	char sql_insert_mail[MAX_LINE] = "insert into user_table ( id, username, password) values(;
    int status;
    struct symbol symbol_char;

	/* convert the user_id to string type */
	char str_uid[255];
	char sql_to[MAX_LINE];
	char *sql_from;
	sprintf(str_uid, "%d", userid);

    strcat(sql_insert_mail, str_uid);
    strcat(sql_insert_mail, symbol_char->comma);
    strcat(sql_insert_mail, symbol_char->single_quotes);
    strcat(sql_insert_mail, mb->name);
    strcat(sql_insert_mail, symbol_char->single_quotes);
    strcat(sql_insert_mail, symbol_char->comma);
    strcat(sql_insert_mail, symbol_char->single_quotes);
    strcat(sql_insert_mail, mb->password);
    strcat(sql_insert_mail, symbol_char->single_quotes);
    strcat(sql_insert_mail, symbol_char->r_parenthese);

	// prevent SQL injection attacks.
	sql_from = sql_insert_mail;
	mysql_real_escape_string(mysql, sql_to, sql_from, sizeof(sql_from));

    status = my_insert_record(mysql, sql_insert_mail);	

}




void insert_mail(MYSQL *mysql, struct mail_body *mb_ptr) {
	char sql_insert_mail[MAX_LINE] = "insert into mail_table ( mailtype, receiver, content) values(";

	char sql_to[MAX_LINE];
	char *sql_from;
    int status;
    struct symbol symbol_char;
    strcat(sql_insert_mail, symbol_char->single_quotes);
    strcat(sql_insert_mail, mb->type);
    strcat(sql_insert_mail, symbol_char->single_quotes);
    strcat(sql_insert_mail, symbol_char->comma);
    strcat(sql_insert_mail, symbol_char->single_quotes);
    strcat(sql_insert_mail, mb->name);
    strcat(sql_insert_mail, symbol_char->single_quotes);
    strcat(sql_insert_mail, symbol_char->comma);
    strcat(sql_insert_mail, symbol_char->single_quotes);
    strcat(sql_insert_mail, mb->content);
    strcat(sql_insert_mail, symbol_char->single_quotes); 
    strcat(sql_insert_mail, symbol_char->r_parenthese);
	
	// prevent SQL injection attacks.
	sql_from = sql_insert_mail;
	mysql_real_escape_string(mysql, sql_to, sql_from, sizeof(sql_from));

    status = my_insert_record(mysql, sql_insert_mail);	

}

void parse_mail_body(struct mail_body *mb_ptr) {

	switch( mb_ptr->type) {
			case MAIL_TYPE_LOGIN:
					break;
			case MAIL_TYPE_LOGOUT:
					break;
			case MAIL_TYPE_SIGNUP:
					printf(" mailbody type is signup\n");
					add_newuser(mysql, mb_ptr);
					break;
			case MAIL_TYPE_MAIL:
					printf(" obj type is mail\n");
					insert_mail(mysql,mb_ptr);
					break;
			case MAIL_TYPE_GETMAIL:
					break;
				
		}

}

struct mail_body* unpack(char * buf, size_t len) {
	
	msgpack_unpack_return ret;
	msgpack_unpacked result;
	int i = 0;
	size_t off = 0;
	struct mail_body *mb;
	
	msgpack_unpacked_init(&result);
	ret = msgpack_unpack_next(&result, buf, len, &off);

	// add judge the result.type whether it is the required.
	// That is to say to identify the data type.
	judge(result.data.via.u64);

	mb->type = (unsigned long)result.data.via.u64;
	
	ret = msgpack_unpack_next(&result, buf, len, &off);	
	strncpy(mb->username, result.data.via.str.ptr, result.data.via.str.size);

	ret = msgpack_unpack_next(&result, buf, len, &off);	
	strncpy(mb->password, result.data.via.str.ptr, result.data.via.str.size);

	return mb;
}







void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = (char*) malloc(suggested_size);
    buf->len = suggested_size;
}

void echo_write(uv_write_t *req, int status) {
    if (status) {
        fprintf(stderr, "Write error %s\n", uv_strerror(status));
    }
    free(req);
}


void lib_msgpack_process(char *str)
{
	
	/* msgpack::sbuffer is a simple buffer implementation. */
	msgpack_sbuffer sbuf;
	msgpack_sbuffer_init(&sbuf);

	/* serialize values into the buffer using msgpack_sbuffer_write callback function. */	
	msgpack_packer pk;
	msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);

	

	return 0;
}
void prepare(msgpack_sbuffer* sbuf, char *mail) {
	msgpack_packer pk;
	msgpack_packer_init(&pk, sbuf, msgpack_sbuffer_write);
	
	msgpack_pack_str_body(&pk, mail, strlen(mail));
	

}

/* pack the mail content */
void prepare_sbuf(msgpack_sbuffer* sbuf) {
	msgpack_packer pk;
	
	msgpack_packer_init(&pk, sbuf, msgpack_sbuffer_write);
	
	/* mail content */
	/* 4mail5Alice10helloworld */
	msgpack_pack_array(&pk, 3);

	msgpack_pack_str(&pk, 4)
	msgpack_pack_str_body(&pk, "mail", 4);
	
	msgpack_pack_str(&pk, 5)
	msgpack_pack_str_body(&pk, "Alice", 5);

	msgpack_pack_str(&pk, 10)
	msgpack_pack_str_body(&pk, "helloworld", 10);

}


/* unpack the mail content. */
void unpack(char const* buf, size_t len) {

	/* buf is allocated by client */
	msgpack_unpacked result;
	size_t off = 0;
	msgpack_unpack_return ret;
	int i = 0;
	msgpack_unpack_init(&result);
	ret = msgpack_unpack_next(&result, buf, len, &off);
	while(ret == MSGPACK_UNPACK_SUCCESS) {
		msgpack_object obj = result.data;
	
		/* use obj */
		printf("Object no %d:\n", ++i);
		msgpack_object_print(stdout, obj);
		printf("\n");
		
		/* If you want to allocate something on the zone, you can use zone. */
		/* msgpack_zone* zone = result.zone; */
		/* The lifetime of the obj and the zone */
		ret = msgpack_unpack_next(&result, buf, len, &off);
		
	}
	msgpack_unpack_destroy(&result);
	
	if (ret == MSGPACK_UNPACK_CONTINUE) {
		printf("All msgpack_object in the buffer is consumed.\n");
	}
	else if (ret == MSGPACK_UNPACK_PARSE_ERROR) {
		printf("The data in the buf is invalid format.\n");
	}
}


void echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    if (nread < 0) {
        if (nread != UV_EOF)
            fprintf(stderr, "Read error %s\n", uv_err_name(nread));
        uv_close((uv_handle_t*) client, NULL);
    } else if (nread > 0) {1
        uv_write_t *req = (uv_write_t *) malloc(sizeof(uv_write_t));
        uv_buf_t wrbuf = uv_buf_init(buf->base, nread);

		// buf->base point the string that the client had sent.
		// such as the string "mail Alice helloworld"
		// seperate the mail to :0x02A5AliceAAhelloworld"
		
		msgpack_sbuffer sbuf;
		msgpack_sbuffer_init(&sbuf);

//		prepare(&sbuf, buf->base);
		
		unpack(sbuf.data, sbuf.size);	

		// protocol process with msgpack
		lib_msgpack_process(buf->base);

        uv_write(req, client, &wrbuf, 1, echo_write);
    }

    if (buf->base)
        free(buf->base);
}

void echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf,) {
    if (nread < 0) {
        if (nread != UV_EOF)
            fprintf(stderr, "Read error %s\n", uv_err_name(nread));
        uv_close((uv_handle_t*) client, NULL);
    } else if (nread > 0) {1
        uv_write_t *req = (uv_write_t *) malloc(sizeof(uv_write_t));
        uv_buf_t wrbuf = uv_buf_init(buf->base, nread);

		// buf->base point the string that the client had sent.
		// such as the string "mail Alice helloworld"
		// seperate the mail to :0x02A5AliceAAhelloworld"
		
		msgpack_sbuffer sbuf;
		msgpack_sbuffer_init(&sbuf);
		prepare(&sbuf, buf->base);
		
		unpack(sbuf.data, sbuf.size);	

		// protocol process with msgpack
		lib_msgpack_process(buf->base);

        uv_write(req, client, &wrbuf, 1, echo_write);
    }

    if (buf->base)
        free(buf->base);
}
void on_new_connection(uv_stream_t *server, int status) {
    if (status < 0) {
        fprintf(stderr, "New connection error %s\n", uv_strerror(status));
        // error!
        return;
    }

    uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(loop, client);
	for(;;){
    	if (uv_accept(server, (uv_stream_t*) client) == 0) {
        	uv_read_start((uv_stream_t*) client, alloc_buffer, echo_read);
    	}
    	else {
        	uv_close((uv_handle_t*) client, NULL);
    	}
	}
}

int main() {
    loop = uv_default_loop();

    uv_tcp_t server;
    uv_tcp_init(loop, &server);

    uv_ip4_addr("0.0.0.0", DEFAULT_PORT, &addr);

    uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
    	
	int r = uv_listen((uv_stream_t*) &server, DEFAULT_BACKLOG, on_new_connection);
   	if (r) {
       	fprintf(stderr, "Listen error %s\n", uv_strerror(r));
       	return 1;
   	}
    return uv_run(loop, UV_RUN_DEFAULT);
}

