#include "buffer.h"

struct buf* init_buf(int op, int len){
	struct buf* that = (struct buf*)malloc(sizeof(struct buf) + sizeof(char)*len);
	that->op = op;
	that->len = len;

	return that;
}

