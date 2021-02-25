#include <stdio.h>
#include <dlfcn.h>

int main(int argc,char **argv)
{
		if(argc!=2)
		{
				printf("./app <动态库的路径>\n");
				return 0;
		}
		
		/*1. 打开指定的动态库*/
		void *handle=dlopen(argv[1],RTLD_LAZY);
		if(handle==NULL)
		{
			printf("%s库文件打开失败!\n",argv[1]);
		}
		
		/*2. 根据符号找到需要调用的函数的首地址*/
		int(*sum1)(int,int); //定义一个指向函数的指针
		sum1=dlsym(handle,"sum"); //获取sum函数的首地址
		
		void(*print1)(int); //定义一个指向函数的指针
		print1=dlsym(handle,"print"); //获取print函数的首地址
		
		int c=sum1(100,100);	
		print1(c);
		
		c=sum1(200,200);	
		print1(c);
		
		
		int *p; //定义一个整型的指针
		p=dlsym(handle,"abcd"); //获取abcd这个变量的首地址
		printf("abcd=%d\n",*p);
		
		/*3. 关闭动态库*/
		dlclose(handle);
		return 0;
}
