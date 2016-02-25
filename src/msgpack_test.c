#include <stdio.h>
#include <msgpack.h>


/*
 *	msgpack for c/c++
 *	version 1.4.0
 *	It's like JSON but small and fast.
 *	MessagePack is an efficient binary serialization format, which lets you exchange data among multiple languages like JSON, except that it's faster and smaller. Small integers are encoded into a single byte while typical short string require only one extra byte in addition to the strings themselves.
*/

int main(void)
{
	
	/* msgpack::sbuffer is a simple buffer implementation. */
	msgpack_sbuffer sbuf;
	msgpack_sbuffer_init(&sbuf);
	
	/* serialize values into the buffer using msgpack_sbuffer_write callback function */
	msgpack_packer pk;
	msgpack_packer_init(&pk, &sbuf, msgpack_sbuffer_write);
	
	msgpack_pack_array(&pk, 3);
	msgpack_pack_int(&pk, 1);
	msgpack_pack_true(&pk);
	msgpack_pack_str(&pk, 7);
	msgpack_pack_str_body(&pk, "example", 7);
	
	/* deserialize the buffer into msgpack_object instance. */
	/* deserialized object is valid during the msgpack_zone instance alive. */
	msgpack_zone mempool;
	msgpack_zone_init(&mempool, 2048);
	
	msgpack_object deserialized;
	msgpacke_unpack(sbuf.data, sbuf.size, NULL, &mempool, &deserialized);
	
	/* print the serialized object.  */
	msgpack_object_print(stdout, deserialized);
	put("");

	msgpack_zone_destroy(&mempool);
	msgpack_sbuffer_destroy(&sbuf);

	return 0;
}



#if 0
int main(void)
{
	/* Create buffer and serializer instance. */
	msgpack_sbuffer* buffer = msgpack_sbuffer_new();
	msgpack_packer* pk = msgpack_packer_new(buffer, msgpack_sbuffer_write);
	
	/* serializes ["Hello", "MessagePackk"]. */
	msgpack_pack_array(pk, 2);
	msgpack_pack_raw(pk, 5);
	msgpack_pack_raw_body(pk, "Hello", 5);
	msgpack_pack_raw(pk, 11);
	msgpack_pack_raw_body(pk, "MessagePack", 11);
	
	// do somethin with buffer->data and buffer->size

	/* cleaning */
	msgpack_sbuffer_free(buffer);
	msgpack_packer_free(pk);

	return 0;
}
#endif
