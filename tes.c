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
	if (isLittleEnd())	
		printf("	isLittleEnd();\n");
	else
		printf("	bigEnd\n");
	if (isLittleEndTest())
		printf("    istestLittleEnd();\n");
	else
		printf("	Testbigend\n");

	return 0;
}
