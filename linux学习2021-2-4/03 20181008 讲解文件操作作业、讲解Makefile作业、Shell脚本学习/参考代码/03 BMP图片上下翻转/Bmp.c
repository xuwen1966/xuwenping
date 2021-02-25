#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>

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
函数功能: BMP图片翻转
函数参数:
		char *src_BmpFile     :BMP图片源文件
		char *new_BmpFile     :新文件
返回值  :0表示成功，其他值失败
*/
int BMPOverturn(char *src_BmpFile,char *new_BmpFile)
{
	/*1. 打开源文件*/
	int err=0;
	FILE *bmp_file=fopen(src_BmpFile,"rb");
	if(bmp_file==NULL)
	{
		err=1;
		goto ERROR;
	}
	
	/*2. 图片参数获取*/
	struct tagBITMAP_FILE_HEADER src_head; //BMP文件头
	memset(&src_head,0,sizeof(struct tagBITMAP_FILE_HEADER));
	if(fread(&src_head,1,sizeof(struct tagBITMAP_FILE_HEADER),bmp_file)!=sizeof(struct tagBITMAP_FILE_HEADER))
	{
		err=2;
		goto ERROR;
	}
	
	if(src_head.bfType!=0x4d42) //判断类型
	{
		err=3;
		goto ERROR;
	}
	
	struct tagBITMAP_INFO_HEADER src_info; //BMP图像参数
	memset(&src_info,0,sizeof(struct tagBITMAP_INFO_HEADER));
	if(fread(&src_info,1,sizeof(struct tagBITMAP_INFO_HEADER),bmp_file)!=sizeof(struct tagBITMAP_INFO_HEADER))
	{
		err=4;
		goto ERROR;
	}
	
	if(src_info.biBitCount!=24) //判断颜色位数
	{
		err=5;
		goto ERROR;
	}
	
	/*3. 创建新图片*/
	FILE *new_file=fopen(new_BmpFile,"wb");
	if(new_file==NULL)
	{
		err=6;
		goto ERROR;
	}
	
	/*3.1 创建BMP文件头*/
	fwrite(&src_head,1,sizeof(struct tagBITMAP_FILE_HEADER),new_file);
	
	/*3.2 创建BMP图像参数*/
	fwrite(&src_info,1,sizeof(struct tagBITMAP_INFO_HEADER),new_file);

	/*3.3 实现图片上下翻转*/
	int i;
	int lineByte=src_info.biWidth*3; //一行总字节数量
	if(lineByte%4)lineByte++;
	
	int offset=lineByte*(src_info.biHeight-1)+src_head.bfOffBits;
	char *data_p=malloc(lineByte);
	if(data_p==NULL)
	{
		err=7;
		goto ERROR;
	}
	
	for(i=0;i<src_info.biHeight;i++)
	{
		fseek(bmp_file,offset,SEEK_SET);
		fread(data_p,1,lineByte,bmp_file);
		fwrite(data_p,1,lineByte,new_file);
		offset-=lineByte;
	}
	
ERROR:	
	if(data_p)free(data_p);
	if(bmp_file)fclose(bmp_file);
	if(new_file)fclose(new_file);
	return err;
}

int main(int argc,char **argv)
{
	char cmd_buff[100];
	if(argc!=3)
	{
		printf("参数格式: ./app <源bmp图片名称> <新bmp图片名称>\n");
		return 0;
	}
	
	//上下翻转图片
	int err=BMPOverturn(argv[1],argv[2]);
	if(err)
	{
		printf("图片上下翻转失败!\n");
	}
	else
	{
		printf("图片上下翻转处理成功,新图片名称:%s\n",argv[2]);
		sprintf(cmd_buff,"eog %s",argv[2]);
		system(cmd_buff);
	}
	return 0;
}

