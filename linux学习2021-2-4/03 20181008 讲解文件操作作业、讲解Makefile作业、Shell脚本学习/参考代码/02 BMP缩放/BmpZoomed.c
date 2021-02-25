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

/* 图片缩放算法 */
/**********************************************************************
 * 函数名称： PicZoom
 * 功能描述： 近邻取样插值方法缩放图片
 *            注意该函数会分配内存来存放缩放后的图片,用完后要用free函数释放掉
 *            "近邻取样插值"的原理请参考网友"lantianyu520"所著的"图像缩放算法"
 * 输入参数： 第一部分 :内含原始图片的象素数据
 *            第二部分 :内含缩放后的图片的象素数据
 * 输出参数： 无
 * 返 回 值： 0 - 成功, 其他值 - 失败
 ***********************************************************************/
int PicZoom(unsigned char *s_buff,unsigned int s_width, unsigned int s_height,unsigned char *buff,unsigned int width, unsigned int height)
{
	unsigned long* pdwSrcXTable;
	unsigned long x;
	unsigned long y;
	unsigned long dwSrcY;
	unsigned char *pucDest;
	unsigned char *pucSrc;
	unsigned long dwPixelBytes = 24/8;
	
	pdwSrcXTable = malloc(sizeof(unsigned long) * width);
	if(NULL == pdwSrcXTable)
	{
	    printf("malloc error!\n");
	    return -1;
	}

	for (x = 0; x < width; x++)//生成表 pdwSrcXTable
	{
	    	pdwSrcXTable[x]=(x*s_width/width);
	}

	for (y = 0; y < height; y++)
	{			
	    dwSrcY = (y * s_height / height);

		pucDest = buff + y*(width*3);
		pucSrc  = s_buff + dwSrcY*(s_width*3);
		
	    for (x = 0; x <width; x++)
	    {
	        /* 原图座标: pdwSrcXTable[x]，srcy
	         * 缩放座标: x, y
			 */
			 memcpy(pucDest+x*dwPixelBytes, pucSrc+pdwSrcXTable[x]*dwPixelBytes, dwPixelBytes);
	    }
	}
	free(pdwSrcXTable);
	return 0;
}


/*
函数功能: BMP图片缩放
函数参数:
		char *src_BmpFile     :BMP图片源文件
		char *new_BmpFile     :新文件
		unsigned long  Width  :缩放之后的宽度
		unsigned long  Height :缩放之后的高度
返回值  :0表示成功，其他值失败
*/
int BmpZoomed(char *src_BmpFile,char *new_BmpFile,unsigned long  Width,unsigned long  Height)
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
	
	printf("源图片尺寸:%d*%d\n",src_info.biWidth,src_info.biHeight);
	
	/*3. 创建新图片*/
	FILE *new_file=fopen(new_BmpFile,"wb");
	if(new_file==NULL)
	{
		err=6;
		goto ERROR;
	}
	
	/*3.1 创建BMP文件头*/
	struct tagBITMAP_FILE_HEADER new_head; 
	memset(&new_head,0,sizeof(struct tagBITMAP_FILE_HEADER));
	new_head.bfType=0x4d42;
	new_head.bfSize=Width*Height*3+54;
	new_head.bfOffBits=54;
	fwrite(&new_head,1,sizeof(struct tagBITMAP_FILE_HEADER),new_file);
	
	/*3.2 创建BMP图像参数*/
	struct tagBITMAP_INFO_HEADER new_info; //BMP图像参数
	memset(&new_info,0,sizeof(struct tagBITMAP_INFO_HEADER));
	new_info.biSize=sizeof(struct tagBITMAP_INFO_HEADER);
	new_info.biWidth=Width;
	new_info.biHeight=Height;
	new_info.biPlanes=1;
	new_info.biBitCount=24;
	fwrite(&new_info,1,sizeof(struct tagBITMAP_INFO_HEADER),new_file);
	
	printf("新图片尺寸:%d*%d\n",new_info.biWidth,new_info.biHeight);
	
	/*3.3 实现图片缩放*/
	unsigned char *s_buff=malloc(src_info.biWidth*src_info.biHeight*3);
	if(s_buff==NULL)
	{
		err=7;
		goto ERROR;
	}
	
	//读取源图片数据
	if(fread(s_buff,1,src_info.biWidth*src_info.biHeight*3,bmp_file)!=src_info.biWidth*src_info.biHeight*3)
	{
		err=8;
		goto ERROR;
	}
	
	//给缩放之后的图片申请空间
	unsigned char *n_buff=malloc(Width*Height*3);
	if(n_buff==NULL)
	{
		err=9;
		goto ERROR;
	}
	
	//进行缩放
	PicZoom(s_buff,src_info.biWidth,src_info.biHeight,n_buff,Width,Height);
	
	//将缩放之后的颜色数据写入新文件
	fwrite(n_buff,1,Width*Height*3,new_file);
	
ERROR:	
	if(s_buff)free(s_buff);
	if(n_buff)free(n_buff);
	if(bmp_file)fclose(bmp_file);
	if(new_file)fclose(new_file);
	return err;
}


int main(int argc,char **argv)
{
	char cmd_buff[100];
	
	if(argc!=5)
	{
		printf("参数格式: ./app <源bmp图片名称> <新bmp图片名称> <宽度> <高度>\n");
		return 0;
	}
	
	int Width=atoi(argv[3]); //字符串转整数
	int Height=atoi(argv[4]);
	
	//缩放图片
	int err=BmpZoomed(argv[1],argv[2],Width,Height);
	if(err)
	{
		printf("图片缩放出现错误!\n");
	}
	else
	{
		printf("缩放处理成功,新图片名称:%s\n",argv[2]);
		sprintf(cmd_buff,"eog %s",argv[2]);
		system(cmd_buff);
	}
	return 0;
}

