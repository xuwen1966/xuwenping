#include <stdio.h>
#include <string.h>

/* 必须在结构体定义之前使用,这是为了让结构体中各成员按1字节对齐 */
#pragma pack(1)

/*需要文件信息头:14个字节 */
struct tagBITMAP_FILE_HEADER
{
	unsigned short bfType;   //保存图片类似。 'BM' -- 0x4d42
	unsigned int  bfSize;      //图片的大小
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned int  bfOffBits;  //RGB数据偏移地址
};

/* 位图参数信息 */
struct tagBITMAP_INFO_HEADER { 
	unsigned long  biSize;      //结构体大小
	unsigned long  biWidth;		//宽度
	unsigned long  biHeight;	//高度
	unsigned short biPlanes;
	unsigned short biBitCount;	//颜色位数
	unsigned long  biCompression;
	unsigned long  biSizeImage;
	unsigned long  biXPelsPerMeter;
	unsigned long  biYPelsPerMeter;
	unsigned long  biClrUsed;
	unsigned long  biClrImportant;
};

/*
函数功能: 创建一张BMP图片
函数参数:
		char *name 文件名称
		int w 宽度
		int h 高度
		int c 颜色位数
函数返回值: 0表示成功
*/
int CreateBmpImage(char *name,int w,int h,int c)
{
	/*1. 创建文件*/
	FILE *file=fopen(name,"wb");
	if(file==NULL)return 1;
	
	/*2. 创建BMP文件头*/
	struct tagBITMAP_FILE_HEADER head;
	memset(&head,0,sizeof(struct tagBITMAP_FILE_HEADER));
	head.bfType=0x4d42; //BMP图片的类型
	head.bfSize=sizeof(struct tagBITMAP_FILE_HEADER)+sizeof(struct tagBITMAP_INFO_HEADER)+w*h*3;
	head.bfOffBits=sizeof(struct tagBITMAP_FILE_HEADER)+sizeof(struct tagBITMAP_INFO_HEADER);
	if(fwrite(&head,1,sizeof(struct tagBITMAP_FILE_HEADER),file)!=sizeof(struct tagBITMAP_FILE_HEADER))
	{
		return 2;
	}
	
	/*3. 创建BMP图像参数信息*/
	struct tagBITMAP_INFO_HEADER info;
	memset(&info,0,sizeof(struct tagBITMAP_INFO_HEADER));
	info.biSize=sizeof(struct tagBITMAP_INFO_HEADER);
	info.biWidth=w;
	info.biHeight=h;
	info.biBitCount=24;
	info.biPlanes=1;
	if(fwrite(&info,1,sizeof(struct tagBITMAP_INFO_HEADER),file)!=sizeof(struct tagBITMAP_INFO_HEADER))
	{
		return 3;
	}
	
	/*4. 图片颜色数据填充*/
	int i,j;
	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			if(fwrite(&c,1,3,file)!=3)
			{
				return 4;
			}
		}
	}
	
	/*5. 关闭文件*/
	fclose(file);
}


//argc :表示参数的数量
//argv :二维指针，指向传入的每一个字符串首地址
int main(int argc,char **argv)
{
	if(argc!=2)
	{
		printf("参数格式:./app <图片的名称>\n");
		return 0;
	}
	
	if(CreateBmpImage(argv[1],320,480,0xFF0033))
	{
		printf("图片创建失败!\n");
	}
	else
	{
		printf("图片创建成功!\n");
	}
	return 0;
}

