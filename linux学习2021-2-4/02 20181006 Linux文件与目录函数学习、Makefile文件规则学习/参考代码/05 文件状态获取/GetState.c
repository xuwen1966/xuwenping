#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

  
// ./app <dir_path> <.mp3>

/*
函数功能: 输出指定目录下所有文件信息
./app /bmp/jpg/ .mp3
*/
int PrintDirName(char *DirName)
{
	/*1. 打开目录*/
	DIR *dirp=opendir(DirName);
	if(dirp==NULL)return 1;
	
	/*2. 循环读取目录*/
	struct dirent *file_p=NULL;
	char *findstr=NULL;
	char *addr_p=NULL; //存放最终完整的文件名称
	struct stat buf;
	while(file_p=readdir(dirp))
	{
		addr_p=malloc(strlen(DirName)+strlen(file_p->d_name)+1);
		strcpy(addr_p,DirName); //拼接目录
		strcat(addr_p,file_p->d_name);//拼接文件名称
		if(stat(addr_p,&buf)==-1)break;
		
		switch (buf.st_mode & S_IFMT) 
		{
           case S_IFBLK:  printf("块设备文件:%s\n",addr_p);            
				break;
           case S_IFCHR:  printf("字符设备文件:%s\n",addr_p);        
				break;
           case S_IFDIR:  printf("目录:%s\n",addr_p);               
				break;
           case S_IFIFO:  printf("FIFO/pipe:%s\n",addr_p);               
				break;
           case S_IFLNK:  printf("链接文件:%s\n",addr_p);                 
				break;
           case S_IFREG:  printf("普通文件:%s,%ld\n",addr_p,buf.st_size);            
				break;
           case S_IFSOCK: printf("socket文件:%s\n",addr_p);                  
				break;
           default:       printf("无效文件:%s\n",addr_p);                
				break;
        }
		
		//buf.st_ctime 秒单位的时间。
		 printf("Last status change:       %s", ctime(&buf.st_ctime));
         printf("Last file access:         %s", ctime(&buf.st_atime));
         printf("Last file modification:   %s", ctime(&buf.st_mtime));
		 printf("\n");  
		 free(addr_p); //释放空间
	}
	
	/*3. 关闭目录*/
	closedir(dirp);
	return 0;
}

int main(int argc,char **argv)
{
	if(argc!=2)
	{
		printf("./app <dir_path>\n");
		return 0;
	}
	
	PrintDirName(argv[1]);
	return 0;
}

