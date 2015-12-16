
struct buf{
	int op;
	int len;
	char *data;
};


struct buf* init_buf(int op, int len){
	struct buf* that = (struct buf*)malloc(sizeof(struct buf) + sizeof(char)*len);
	that->op = op;
	that->len = len;

	return that;
}

#define MAX_MAIL_LEN 1000
