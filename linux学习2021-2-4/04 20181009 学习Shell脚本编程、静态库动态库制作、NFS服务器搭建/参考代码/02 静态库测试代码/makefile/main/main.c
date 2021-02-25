#include "print.h"
#include "sum.h"
int main(int argc,char **argv)
{
		if(argc!=3)
		{
				printf("./app <val1> <val2>\n");
				return 0;
		}
		int a=atoi(argv[1]);
		int b=atoi(argv[2]);
		int c=sum(a,b);	
		print(c);
		c=sum(a,b);	
		print(c);
		return 0;
}
