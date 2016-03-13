/*
//	gcc -luv -pthread -o client echo_client.c /usr/lib/libuv.a
gcc -lmsgpackc -pthread -o user_buffer_unpack user_buffer_unpack.c /usr/lib/libmsgpackc.a
*/
#include <msgpack.h>
#include <stdio.h>
#include <assert.h>


#define MAX_LINE 100

struct mail_body{
	unsigned long type;
	char username[MAX_LINE];
	char password[MAX_LINE];

};


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
//	msgpack_pack_str(&pk, 4);
//	msgpack_pack_str_body(&pk, "mail", 4);
	msgpack_pack_int(&pk,2);
	
	msgpack_pack_str(&pk, 5);
	msgpack_pack_str_body(&pk, "abcde", 5);

	msgpack_pack_str(&pk, 10);
	msgpack_pack_str_body(&pk, "abcdefghij", 10);
#if 0	
	/* 3rd objcet */
	msgpack_pack_array(&pk, 2);
	msgpack_pack_int(&pk, 42);
	msgpack_pack_false(&pk);

#endif
}

void unpack(char const* buf, size_t len) {
	/* buf is allocated by client */
	msgpack_unpack_return ret;
	msgpack_unpacked result; 
	int i = 0;
	size_t off = 0;
	struct mail_body mb;
	msgpack_unpacked_init(&result);
	

	ret = msgpack_unpack_next(&result, buf, len, &off);
	msgpack_object obj = result.data;
	/* use obj */
//	printf("Object no %d:\n", ++i);
	//printf(" obj is : ");
	msgpack_object_print(stdout, obj);
	printf("\n");
	mb.type = (unsigned long)obj.via.u64;

	printf("mb.type is :%d\n", obj.via.u64);	



	ret = msgpack_unpack_next(&result, buf, len, &off);
	strncpy(mb.username, result.data.via.str.ptr, result.data.via.str.size);
//	printf("obj.via.str.ptr %d: %s\n", i, result.data.via.str.ptr);
//	printf("o.via.size is :%d\n", (unsigned long)result.data.via.str.size);	
	printf("mb.username is : %s\n", mb.username);

	ret = msgpack_unpack_next(&result, buf, len, &off);
	strncpy(mb.password, result.data.via.str.ptr, result.data.via.str.size);
//	printf("obj.via.str.ptr %d: %s\n", i, result.data.via.str.ptr);
//	printf("o.via.size is :%d\n", (unsigned long)result.data.via.str.size);	
	printf("mb.password is : %s\n", mb.password);



//		printf(" mail_type is : ");
//		msgpack_object_print(stdout, mail_type.data);
	printf("\n");
	#if 0	
		if ( (msgpack_object_equal(obj, mail_type.data)) == true ) {
			printf(" the mail type is mail\n");
		} 
		else {
			printf("the mail type is not mail\n");
		}
		
#endif

		printf("\n");
		/* If you wnat to allocate something on the zone, you can use zone. */

		/* msgpack_zone* zone = result.zone; */
		/* The lifetime of the obj and the zone. */
		
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
