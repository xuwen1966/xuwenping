#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

// ./app <dir_path> <.mp3>

/*
函数功能: 输出指定目录下指定指定后缀的文件名称+路径
./app /bmp/jpg/ .mp3
*/
int PrintDirName(char *DirName,char *str)
{
	/*1. 打开目录*/
	DIR *dirp=opendir(DirName);
	if(dirp==NULL)return 1;
	
	/*2. 循环读取目录*/
	struct dirent *file_p=NULL;
	char *findstr=NULL;
	char *addr_p=NULL; //存放最终完整的文件名称
	while(file_p=readdir(dirp))
	{
		char *findstr=strstr(file_p->d_name,str);
		if(findstr)//123.mp3  123.mp3.mp4 123.mp3.txt
		{
			//比较后缀
			if(strcmp(findstr,str)==0)
			{
				addr_p=malloc(strlen(DirName)+strlen(file_p->d_name)+1);
				strcpy(addr_p,DirName); //拼接目录
				strcat(addr_p,file_p->d_name);//拼接文件名称
				printf("文件完整路径=%s\n",addr_p);
				free(addr_p); //释放空间
			}
		}
	}
	/*3. 关闭目录*/
	closedir(dirp);
	return 0;
}

int main(int argc,char **argv)
{
	if(argc!=3)
	{
		printf("./app <dir_path> <.mp3>\n");
		return 0;
	}
	
	PrintDirName(argv[1],argv[2]);
	return 0;
}

