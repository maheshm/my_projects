#include <stdio.h>

main()
{
	int i=0,a=2;
	char c[4];
	while(a>0)
	{
		c[i]=a%10+48;
		a/=10;i++;		
	}
	
	printf("\n%s\n",c);
}
