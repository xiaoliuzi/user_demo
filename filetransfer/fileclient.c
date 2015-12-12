#include "common.h"

int main(int argc, char **argv[])
{
	int clientfd;
	
	if (argc != 2) {
		fprintf(stderr, "Usage:./fileclient <IP_Address>\n");
		exit(1);
	}
	struct sockaddr_in clientaddr;
	bzero(&clientaddr, sizeof(clientaddr));
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_addr.s_addr = htons(INADDR_ANY);
	clientaddr.sin_port = htons(0);
	
	clientfd = socket(AF_INET, SOCK_STREAM, 0);
	if (clientfd < 0) {
		perror("socket");
		exit(1);
	}
	if (bind(clientfd,(struct sockaddr*)&clientaddr, sizeof(clientaddr)) < 0) {
		perror("bind");
		exit(1);
	}
	struct sockaddr_in svraddr;  
    bzero(&svraddr,sizeof(svraddr));  
    if(inet_aton(argv[1],&svraddr.sin_addr)==0)  
    {  
        perror("inet_aton");  
        exit(1);  
    }  
    svraddr.sin_family=AF_INET;  
    svraddr.sin_port=htons(PORT);  
      
    socklen_t svraddrlen=sizeof(svraddr);  
    if(connect(clientfd,(struct sockaddr*)&svraddr,svraddrlen)<0)      
    {  
        perror("connect");  
        exit(1);  
    }  
      
    //recv file imformation  
    char buff[BUFFSIZE];  
    char filename[FILE_NAME_MAX_SIZE];  
    int count;  
    bzero(buff,BUFFSIZE);  
  
    count=recv(clientfd,buff,BUFFSIZE,0);  
    if(count<0)  
    {  
        perror("recv");  
        exit(1);  
    }  
    strncpy(filename,buff,strlen(buff)>FILE_NAME_MAX_SIZE?FILE_NAME_MAX_SIZE:strlen(buff));  
  
    printf("Preparing recv file : %s from %s \n",filename,argv[1]);   
  
    //recv file  
    FILE *fd=fopen(filename,"wb+");  
    if(NULL==fd)  
    {  
        perror("open");  
        exit(1);  
    }  
    bzero(buff,BUFFSIZE);  
  
    int length=0;  
    while(length=recv(clientfd,buff,BUFFSIZE,0))  
    {  
        if(length<0)  
        {  
            perror("recv");  
            exit(1);  
        }  
        int writelen=fwrite(buff,sizeof(char),length,fd);  
        if(writelen<length)  
        {  
            perror("write");  
            exit(1);  
        }  
        bzero(buff,BUFFSIZE);  
    }  
    printf("Receieved file:%s from %s finished!\n",filename,argv[1]);  
    fclose(fd);  
    close(clientfd);  
    return 0;  
}  

