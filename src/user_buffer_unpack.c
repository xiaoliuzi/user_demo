/*
//	gcc -luv -pthread -o client echo_client.c /usr/lib/libuv.a
gcc -lmsgpackc -pthread -o user_buffer_unpack user_buffer_unpack.c /usr/lib/libmsgpackc.a
*/
#include <msgpack.h>
#include <stdio.h>
#include <assert.h>

void prepare(msgpack_sbuffer* sbuf){
	
	msgpack_packer pk;
	
	msgpack_packer_init(&pk, sbuf, msgpack_sbuffer_write);


#if 0	
	/* 1st object */
	msgpack_pack_array(&pk, 3);
	msgpack_pack_int(&pk, 1);
	msgpack_pack_true(&pk);
	msgpack_pack_str(&pk, 7);
	msgpack_pack_str_body(&pk, "example", 7);
#endif	
	/* 2nd objcet */
	msgpack_pack_str(&pk, 4);
	msgpack_pack_str_body(&pk, "mail", 4);
	
	msgpack_pack_str(&pk, 5);
	msgpack_pack_str_body(&pk, "Alice", 5);

	msgpack_pack_str(&pk, 10);
	msgpack_pack_str_body(&pk, "helloworld", 10);
#if 0	
	/* 3rd objcet */
	msgpack_pack_array(&pk, 2);
	msgpack_pack_int(&pk, 42);
	msgpack_pack_false(&pk);

#endif
}

void unpack(char const* buf, size_t len) {
	/* buf is allocated by client */
//#if 0
	msgpack_unpack_return ret;
	msgpack_unpacked result; 
	int i = 0;
	size_t off = 0;
	char *mt_content;// = "ab";
	msgpack_unpacked_init(&result);

	printf("buf is %s\n", buf);
//	ret = msgpack_unpack_next(&result, buf, len, &off);
	ret = msgpack_unpack_next(&result, buf, len, &off);
//#endif


	msgpack_unpacked mail_type;// wroten by xiaoliuzi 
	size_t off_mt = 0;
	msgpack_unpacked_init(&mail_type);
	msgpack_unpack_next(&mail_type, mt_content, strlen(mt_content), &off_mt); 

	while(ret == MSGPACK_UNPACK_SUCCESS) {
		msgpack_object obj = result.data;
	
		/* use obj */
		printf("Object no %d:\n", ++i);
//		msgpack_object_print(stdout, obj);
		printf(" obj is : ");
		msgpack_object_print(stdout, obj);
		printf("\n");

		
//		printf(" mail_type is : ");
//		msgpack_object_print(stdout, mail_type.data);
		printf("\n");
		
		if ( (msgpack_object_equal(obj, mail_type.data)) == true ) {
			printf(" the mail type is mail\n");
		} 
		else {
			printf("the mail type is not mail\n");
		}
		


		printf("\n");
		/* If you wnat to allocate something on the zone, you can use zone. */

		/* msgpack_zone* zone = result.zone; */
		/* The lifetime of the obj and the zone. */
		ret = msgpack_unpack_next(&result, buf, len, &off);
		
	}
	msgpack_unpacked_destroy(&result);

	if (ret == MSGPACK_UNPACK_CONTINUE) {
		printf("All msgpack_objcet in the buffer is consumed.\n");
	
	}
	else if (ret == MSGPACK_UNPACK_PARSE_ERROR) {
		printf("The data in the buf is invalid format.\n");
	}
}

int main(void) {

	msgpack_sbuffer sbuf;
	msgpack_sbuffer_init(&sbuf);
	
	prepare(&sbuf);
	
	unpack(sbuf.data, sbuf.size);
	printf("----------------------\n");

	printf("sbuf.data is : %x\n", sbuf.data);
	msgpack_sbuffer_destroy(&sbuf);
	
	return 0;

}

/* output */
/*
	Object no 1:
	[1,true, "example"]
	Ojbect no 2:
	"second"
	Object no 3:
	[42, false]
	All msgpcak_object in the buffer is consumed.

*/
