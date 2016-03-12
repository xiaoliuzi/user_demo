/*
//	gcc -luv -pthread -o client echo_client.c /usr/lib/libuv.a
gcc -lmsgpackc -pthread -o user_buffer_unpack user_buffer_unpack.c /usr/lib/libmsgpackc.a
*/
#include <msgpack.h>
#include <stdio.h>
#include <assert.h>

#if 0
void msgpack_pack_string(msgpack_packer *pk, char *str)
{

	msgpack_pack_str(&pk, strlen(str));
	msgpack_pack_str_body(&pk, str, strlen(str));

}
#endif
void prepare(msgpack_sbuffer* sbuf){
	msgpack_packer pk;
	msgpack_packer_init(&pk, sbuf, msgpack_sbuffer_write);

	msgpack_pack_int(&pk, 2);

	msgpack_pack_str(&pk, 5);
	msgpack_pack_str_body(&pk, "Alice", 5);
	msgpack_pack_str(&pk, 10);
	msgpack_pack_str_body(&pk, "helloworld", 10);
}

void unpack(char const* buf, size_t len) {
	/* buf is allocated by client */
	msgpack_unpack_return ret;
	msgpack_unpacked result; 
	int i = 0;
	size_t off = 0;
	msgpack_unpacked_init(&result);
	ret = msgpack_unpack_next(&result, buf, len, &off);
	while(ret == MSGPACK_UNPACK_SUCCESS) {

		msgpack_object obj = result.data;
		obj.type = MSGPACK_OBJECT_STR;	
	//	obj.via.str.size = strlen(buf);
		/* use obj */
		printf("Object no %d:\n", ++i);
		msgpack_object_print(stdout, obj);
		printf("\n");
		printf("obj.vir.str.ptr is : %s\n", obj.via.str.ptr);
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
	char cmdstr[100]="a5abcde";	

	size_t len = strlen(cmdstr);

//	prepare(&sbuf);
	printf("sbuf.data = %d\n", sbuf.data);	
//	unpack(cmdstr, 5);
	sbuf.data = "\0xa5abcde";
	unpack(sbuf.data, 6);

	printf("----------------------\n");

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
