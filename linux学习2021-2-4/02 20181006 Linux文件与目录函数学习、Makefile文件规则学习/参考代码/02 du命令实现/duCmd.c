#include <stdio.h>
#include <string.h>
/*
函数功能: 查看指定文件大小
函数参数: 文件名称
*/
long DuCmd(char *path)
{
	/*1. 打开文件*/
	FILE *file=fopen(path,"rb");
	if(file==NULL)return -1;
	
	/*2. 偏移文件指针*/
	fseek(file,0,SEEK_END);
	
	/*3. 获取文件结尾距离文件头的偏移量*/
	long len=ftell(file);
	
	/*4. 关闭文件*/
	fclose(file);
	
	return len;
}

int main(int argc,char **argv)
{
	if(argc!=2)
	{
		printf("参数格式: ./app <文件名称>\n");
		return 0;
	}
	long len=DuCmd(argv[1]);
	printf("%ld字节\n",len);
	return 0;
}

