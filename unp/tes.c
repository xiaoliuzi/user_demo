#include <stdio.h>

int isLittleEnd(void)
{

	int value = 0xff;
	if  (*(unsigned char*)&value == 0xff)
		return 1;


	return 0;
}
int isLittleEndTest(void)
{

	int value = 0xff;
	if  (*(char*)&value == 0xff)
		return 1;


	return 0;
}

int main()

{
/*
	if (isLittleEnd())	
		printf("	isLittleEnd();\n");
	else
		printf("	bigEnd\n");
	if (isLittleEndTest())
		printf("    istestLittleEnd();\n");
	else
		printf("	Testbigend\n");
*/
	char s[100];
	printf("input a string:\n");
	fgets(s, 100, stdin);
	s[strlen(s)-1] = '\0';
	fputs(s, stdout);
//	printf("%s, len=%d", s, strlen(s));
	return 0;
}
