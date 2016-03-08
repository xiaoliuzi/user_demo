#include <stdlib.h>
#include <string.h>
#include <uv.h>
#include <stdio.h>
#include <msgpack.h>
#include <mail_body.h>
#include <assert.h>

/*
	gcc -lmsgpackc -pthread -o user_buffer_unpack user_buffer_unpack.c /usr/lib/libmsgpackc.a
*/
#define DEFAULT_PORT 7000
#define DEFAULT_BACKLOG 128


uv_loop_t *loop;
struct sockaddr_in addr;




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

void prepare(msgpack_sbuffer* sbuf) {
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
		// such as the string "mail Alice 123456"
		// seperate the mail to “mail" "Alice" "123456"
		struct mail_body *mbody;
		struct mail_body * mbody = (struct mail_body*)malloc(sizeof(struct mail_body));
		mbody = mail_separate(buf->base);


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

