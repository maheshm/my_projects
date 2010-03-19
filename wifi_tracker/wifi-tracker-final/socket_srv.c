#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>

main()
{
	struct sockaddr_in server, client;
	int sock_fd, port=3000,len=sizeof(server),cli_len=sizeof(client), sd;
	char buf='a';
	FILE *fp;
	printf("Hello\n");
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);

	bzero((void *)&server,len);
	bzero((void *)&client,cli_len);
	printf("Hello\n");
	server.sin_family=AF_INET;
	server.sin_port=htons(port);
	printf("Hello\n");
//	inet_pton(AF_INET,INADDR_ANY,&server.sin_addr.s_addr);
	server.sin_addr.s_addr=INADDR_ANY;
	printf("Hello\n");
	fp = fopen("helper_iw_data_help","r");

	bind(sock_fd, (struct sockaddr *)&server, len);
	listen (sock_fd,1);
	printf("Listening..\n");
	sd = accept(sock_fd, (struct sockaddr *)&client, &cli_len);
	printf("Connected\n");

	do
	{
		buf = fgetc(fp);	
		printf("%c ",buf);
		send(sd, (void *)&buf, 1, 0);
	}
	while(buf!='*');
	
	fclose(fp);
}
