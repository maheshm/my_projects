#include <stdio.h>
#include <string.h>

FILE *fp;
char filename[50],extension[4];
char archivecmd[100],tar[25],mkdir[50];
char putindir[50],cdcmd[50];
char toshell1[100],actualfn[50];
char packagename[50],depends[50][50],c;


void debhandler(int i)
{
	int j=0;
	strcpy(actualfn,filename);
	filename[i]='\0';
	actualfn[strlen(actualfn)]=' ';
	strcpy(toshell1,"sh shell1.sh ");	
	strcat(toshell1,actualfn);
	strcat(toshell1,filename);
	system(toshell1);
/*	fp=fopen("temp/information","r");
	c=' ';j=0;
	while (c!='\n')
	{
		c=fgetc(fp);
		packagename[j++]=c;
	}
	packagename[j]='\0';
	c=' ';
	j=0;i=0;
	while (c!='\n')
	{
		c=fgetc(fp);
		if (c==',')
		{
			depends[i][j]='\0';
			i++;j=0;
		}
		else		
			depends[i][j++]=c;
		
	}
	printf("Package name:%s\n ",packagename); 		
	printf("Depends on following.\nPlease make sure that they are installed.\n");
	for(j=0;j<=i;j++)
		printf("%d. %s\n",j+1,depends[j]);
	printf("\n");
	printf("Are you sure that they have been installed.\n");
	fclose(fp);
*/	
			
}

void rpmhandler(int i)
{

	strcpy(actualfn,filename);
	filename[i]=' ';
	actualfn[strlen(actualfn)]=' ';
	strcpy(toshell1,"sh shell_rpm.sh ");
	strcat(toshell1,actualfn);
	strcat(toshell1,filename);
	system(toshell1);
	
}

void tarhandler()
{
	char para[50];
	int i,j;
	strcpy(actualfn,filename);
	for(i=0;i<strlen(actualfn);i++)
	{
		if(actualfn[i]=='.')
		{
			actualfn[i]='\0';
			break;
		}
	}		
	printf("Extracting shell..\n");	
	printf("Any special parameter for configuring?\n");
	printf("See the INSTALL file for details\n");
	//scanf("%s",para);
	filename[strlen(filename)]=' ';
	strcpy(toshell1,"sh shell_tar.sh ");
	strcat(toshell1,filename);
	strcat(toshell1,actualfn);
	toshell1[strlen(toshell1)]='\0';
	//strcpy(toshell1,para);
	system(toshell1);
}

void symlink()
{
	printf("debhandler.c:This has to be imlemented\n");
	//strcpy(toshell1,'\0');
	strcpy(toshell1,"sh try.sh ");
	strcat(toshell1,filename);	
	system(toshell1);
}

main()
{
	int i,j=0;
	printf("Enter the file name\n");
	scanf("%s",filename);
	for (i=strlen(filename)-1;i>0;i--)
	{
		if(filename[i]=='.')
			break;
		extension[j++]=filename[i];
	}
	extension[j]='\0';
	printf("%s %d\n",extension,strlen(extension));
	if (!strcmp(extension,"bed"))
		debhandler(i);
	else if (!strcmp(extension,"zg"))
		tarhandler();
	else if (!strcmp(extension,"mpr"))
		rpmhandler(i);
	else
		return;
	symlink();		
}
