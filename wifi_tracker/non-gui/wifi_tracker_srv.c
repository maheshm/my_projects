#include <stdio.h>
	
char our_ip[20],helper_ip[20],track_ip[50][20];
int helper_dist;
int no_of_ip,dist1[50], dist2[50];

void measure_strength_ip()
{

	/* Read the data from the file written by iwspy*/
	int db=0,i=0;
	char temp_ip[20],temp_ch,db_ch[5];
	FILE *fp;
	fp = fopen("iwspy_data","r");
	temp_ch = fgetc(fp);
	
	if (temp_ch != '\"')
	{
		printf("Errorn in iwspy_data\n");
		return 0;
	}
next:	i=0;
	do
	{
		temp_ch = fgetc(fp);
		if (temp_ch != '\"')
			temp_ip[i++]=temp_ch;
	}
	while(temp_ch != '\"');
	temp_ip[i]='\0';
	
	for (i=0; i<no_of_ip; i++)
	{	
		if (strcmp(temp_ip,track_ip[i])==0)
		{
			while(1)	
			{
				db = (db * 10) + (int) temp_ch - 48 ;	
				temp_ch = fgetc(fp);
				if (temp_ch=='d')
				{
					break;
				}
			}
			found =1;
			dist1[i]=db;
			break;
		}
	}
	if (found==0)
	{
			
	
		while((temp_ch=fgetc(fp))!='\"');
		goto next;
	}
	temp_ch = fgetc(fp);
	temp_ch = fgetc(fp);
	
	fclose(fp);
	printf("Server: db = %d; ip = %s   ",db,temp_ip);
	distance = db;
	return distance;
}

int send_rcv_from_helper(char ip[20])
{
	/*Use socket here to send the ip to helper and receive the data from there*/
	int distance,db=0,i=0;
	char temp_ip[20],temp_ch,db_ch[5];
	FILE *fp;
	fp = fopen("iwspy_data_hlp","r");
	temp_ch = fgetc(fp);
	
	if (temp_ch != '\"')
	{
		printf("Error in iwspy_data_hlp\n");
		return 0;
	}
next:	i=0;
	do
	{
		temp_ch = fgetc(fp);
		if (temp_ch != '\"')
			temp_ip[i++]=temp_ch;
	}
	while(temp_ch != '\"');
	temp_ip[i]='\0';
	if (strcmp(temp_ip,ip)!=0)
	{
		while((temp_ch=fgetc(fp))!='\"');
		goto next;
	}
	temp_ch = fgetc(fp);
	temp_ch = fgetc(fp);
	while(1)	
	{
		db = (db * 10) + (int) temp_ch - 48 ;	
		temp_ch = fgetc(fp);
		if (temp_ch=='d')
		{
			break;
		}
	}
	fclose(fp);
	printf("Helper: db = %d; ip = %s\n",db,temp_ip);
	distance = db;
	return distance;
}

void helper_init()
{
	int a;
	a = send_rcv_from_helper(helper_ip);
	if (a!=0)
	{
		helper_dist = 10;//a;
		return;
	}
	else
	{
		printf("Error\n");
		exit(1);
	}
}

void locate_and_record(int who,int d1, int d2)
{
	/*DataBase.. Better do it in simple file. SQL might be good. Try that also.
	Pretty bad equations:
		(x*x) + (y*y) = (d1*d1)
		((b-x)*(b-x)) + (y*y) = (d2*d2) 

		y = sqrt(d1^2 - x^2)
		(b-x)^2 + (d1^2 -x^2) = d2^2 
		=>b^2 - 2bx + x^2 + d1^2 -x^2 = d2^2
		=>-2bx = d2^2 - d1^2 - b^2
		=>x = (-d2^2 + d1^2 + b^2)/2b

	  Assume that the location is towards one side of the cordinates (I). This simplifies a lot of things.
		
	  Arrange the server in a corner of the room and helper at another corner	
		^
		|
		| x
		|----*
		|d1/ | \  d2  
		| /  |y   \
		|/   |       \  
		0--------------0------>
		       b
	*/
	int x, y, b = helper_dist;
	x = (d1*d1 + b*b - d2*d2)/(2*b);
	y = sqrt(d1*d1 - x*x);
	
	printf("The location of %s is %d,%d wrt the two servers\n\n\n",track_ip[who],x,y);
		
}

main()
{
	int i ;
	char interface[10];
	
	printf("Enter this PC's IP: ");
	scanf("%s",our_ip);
	printf("Enter the IP of helper: ");
	scanf("%s",helper_ip);
	printf("Trying to connect to the given helpers...\n");
	helper_init();
	
	printf("Connected.\n");
	printf("Enter the no of IPs to scan\n");
	scanf("%d",&no_of_ip);
	printf("Enter the IPs to scan..\n");
	for (i=0; i<no_of_ip; i++)
	{
		scanf("%s",track_ip[i]);
	}

	printf("MONITORING\n");
	while(1)
	{
		
		measure_strength_ip();
		send_rcv_from_helper();
		locate_and_record();
		sleep(5);
	}
}
