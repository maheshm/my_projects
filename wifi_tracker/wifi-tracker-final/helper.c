#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>

main()
{
	struct sockaddr_in server;
	int sock_fd, port=3000,len=sizeof(server),i;
	char buf;
	FILE *fp;
	
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	bzero((void *)&server,len);
	server.sin_family=AF_INET;
	server.sin_port=htons(port);
	//inet_pton(AF_INET,INADDR_ANY,&server.sin_addr.s_addr);
	server.sin_addr.s_addr=INADDR_ANY;

	fp = fopen("iw_data_help","r");

	i=connect(sock_fd,(struct sockaddr *)&server,len);
	
	printf("%d \n",i);
	do
	{
		recv(sock_fd, (void *)&buf, 1, 0);
		fputc(buf, fp);
		printf("%c ",buf);
	}
	while(buf!='*');
	
	fclose(fp);
}
