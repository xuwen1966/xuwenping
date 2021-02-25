#include <stdio.h>

/*
函数功能: 加密/解密
函数参数:
    char *SrcFile :加密/解密源文件
	char *newFile :创建加密/解密之后生成的文件
	unsigned int password
方法: 使用^异或
*/
int FileDecode(char *SrcFileName,char *newFileName,unsigned int password)
{
	/*1. 打开源文件*/
	FILE *src_file=fopen(SrcFileName,"rb");
	if(src_file==NULL)return 1;
	
	/*2. 创建新文件*/
	FILE *new_file=fopen(newFileName,"wb");
	if(new_file==NULL)return 2;
	
	/*3. 进行加密*/
	unsigned int tmp;
	unsigned int len;
	while(1)
	{
		len=fread(&tmp,1,4,src_file);
		tmp^=password; //加密/解密
		fwrite(&tmp,1,len,new_file);
		if(len!=4)break;
	}
	
	/*4. 关闭文件*/
	fclose(src_file);
	fclose(new_file);
	return 0;
}

int main(int argc,char **argv)
{
	if(argc!=3)
	{
		printf("(加密解密): ./app <源文件> <新文件>\n");
		return 0;
	}
	
	int state=FileDecode(argv[1],argv[2],123456);
	if(state==0)printf("加密/解密执行成功!\n");
	else printf("加密/解密执行失败!\n");
}


