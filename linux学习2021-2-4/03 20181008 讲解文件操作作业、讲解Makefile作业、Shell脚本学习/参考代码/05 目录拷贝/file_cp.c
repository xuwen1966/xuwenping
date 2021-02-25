#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
	   
// ./app <dir_path> <.mp3>

int Copy_File(char *SrcFileName,char *NewFileName)
{
	int err=0;
	FILE *srcfile=fopen(SrcFileName,"rb");
	if(srcfile==NULL)
	{
		err=1;
		goto ERROR;
	}
	FILE *newfile=fopen(NewFileName,"wb");
	if(newfile==NULL)
	{
		err=2;
		goto ERROR;
	}
	
	int i;
	unsigned char buff[1024];
	unsigned int len;
	while(1)
	{
		len=fread(buff,1,1024,srcfile);
		fwrite(buff,1,len,newfile);
		if(len!=1024)break;
	}
	
ERROR:
	if(srcfile)fclose(srcfile);
	if(newfile)fclose(newfile);
	return err;
}


/*
函数功能: 输出指定目录下所有文件信息
./app /bmp/jpg/ .mp3
*/
int Copy_Dir(char *SrcDirName,char *NewDirName)
{
	/*1. 打开目录*/
	DIR *Src_dirp=opendir(SrcDirName);
	if(Src_dirp==NULL)return 1;
	
	DIR *NEW_dirp=opendir(NewDirName);
	if(NEW_dirp==NULL)
	{
		printf("目录不存在,创建新目录\n");
		if(mkdir(NewDirName,0777)!=0)
		{
			printf("创建新目录出现错误!\n");
			return 1;
		}
	}
	
	/*2. 循环读取目录*/
	struct dirent *file_p=NULL;
	char *findstr=NULL;
	char *addr_p=NULL; //存放最终完整的文件名称
	struct stat buf;
	
	char *NewFileName=NULL;
	while(file_p=readdir(Src_dirp))
	{
		addr_p=malloc(strlen(SrcDirName)+strlen(file_p->d_name)+1);
		strcpy(addr_p,SrcDirName); //拼接目录
		strcat(addr_p,file_p->d_name);//拼接文件名称
		
		NewFileName=malloc(strlen(NewDirName)+strlen(file_p->d_name)+1);
		strcpy(NewFileName,NewDirName);
		strcat(NewFileName,file_p->d_name);
		
		if(stat(addr_p,&buf)==-1)break;
		switch (buf.st_mode & S_IFMT) 
		{
           case S_IFREG:  
				if(Copy_File(addr_p,NewFileName)==0)
				{
					printf("成功:%s -> %s\n",addr_p,NewFileName);
				}
				else
				{
					printf("失败:%s -> %s\n",addr_p,NewFileName);
				}
				//printf("普通文件:%s,%ld\n",addr_p,buf.st_size);            
				break;
           default:             
				break;
        }  
		free(addr_p); //释放空间
		free(NewFileName); //释放空间
	}
	
	/*3. 关闭目录*/
	closedir(Src_dirp);
	return 0;
}


int main(int argc,char **argv)
{
	if(argc!=3)
	{
		printf("./app <源目录> <目标目录>\n");
		return 0;
	}
	
	Copy_Dir(argv[1],argv[2]);
	return 0;
}

