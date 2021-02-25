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
函数功能: 图片取模
函数参数:
	char *FileName: BMP图片源文件
	char *ByteFile: 新生成的取模文件
函数返回值: 0表示成功
*/
int GetBmpModel(char *FileName,char *ByteFile)
{
	int err=0;
	
	/*1. 打开图片*/
	FILE *bmp_file=fopen(FileName,"rb");
	if(bmp_file==NULL)return 1;
	
	/*2. 图片参数获取*/
	struct tagBITMAP_FILE_HEADER head; //BMP文件头
	memset(&head,0,sizeof(struct tagBITMAP_FILE_HEADER));
	if(fread(&head,1,sizeof(struct tagBITMAP_FILE_HEADER),bmp_file)!=sizeof(struct tagBITMAP_FILE_HEADER))
	{
		err=2;
		goto ERROR;
	}
	
	if(head.bfType!=0x4d42) //判断类型
	{
		err=3;
		goto ERROR;
	}
	
	struct tagBITMAP_INFO_HEADER info; //BMP图像参数
	memset(&info,0,sizeof(struct tagBITMAP_INFO_HEADER));
	if(fread(&info,1,sizeof(struct tagBITMAP_INFO_HEADER),bmp_file)!=sizeof(struct tagBITMAP_INFO_HEADER))
	{
		err=4;
		goto ERROR;
	}
	
	if(info.biBitCount!=24) //判断颜色位数
	{
		err=5;
		goto ERROR;
	}
	
	/*3. 实现图片颜色数据取模*/
	FILE *byte_file=fopen(ByteFile,"wb");
	if(byte_file==NULL)
	{
		err=6;
		goto ERROR;
	}
	
	/*3.1 申请空间,存放临时数据*/
	char *basename_p=basename(FileName);
	char *data_p=malloc(100+strlen(basename_p));
	if(data_p==NULL)
	{
		err=7;
		goto ERROR;
	}
	
	sprintf(data_p,"const unsigned char %s[%d]={\n",basename_p,info.biWidth*info.biHeight*3);
	if(fwrite(data_p,1,strlen(data_p),byte_file)!=strlen(data_p))
	{
		err=8;
		goto ERROR;
	}
	free(data_p); //释放空间
	
	/*3.2 开始颜色取模*/
	unsigned int lineByte=info.biWidth*3; //一行的字节数必须是4的倍数
	while(lineByte%4)lineByte++; //保证一行字节数必须是4的倍数
	
	unsigned int OffsetByte=head.bfOffBits+lineByte*(info.biHeight-1);
	unsigned int i,j,n;
	unsigned char c_data;
	unsigned char c_buff[6]; //0xAb,
	unsigned int w_cnt=0;
	for(i=0;i<info.biHeight;i++)
	{
		fseek(bmp_file,OffsetByte,SEEK_SET); //偏移文件指针的位置
		for(j=0;j<info.biWidth;j++) //320
		{
			for(n=0;n<3;n++)
			{
				w_cnt++;
				fread(&c_data,1,1,bmp_file);
				sprintf(c_buff,"0x%X,",c_data); //格式化打印数据
				if(fwrite(c_buff,1,strlen(c_buff),byte_file)!=strlen(c_buff))
				{
					err=9;
					goto ERROR;
				}
				if(w_cnt==16) //换行
				{
					w_cnt=0;
					if(fwrite("\n",1,1,byte_file)!=1)
					{
						err=10;
						goto ERROR;
					}
				}
			}
		}
		OffsetByte-=lineByte; //指针位置移动到上一行
	}
	
	if(fwrite("};",1,2,byte_file)!=2)
	{
		err=11;
		goto ERROR;
	}	
ERROR:
	//关闭文件
	if(byte_file)fclose(byte_file);
	if(bmp_file)fclose(bmp_file);
	return err;
}


int main(int argc,char **argv)
{
	char cmd_buff[100];
	
	if(argc!=3)
	{
		printf("参数格式: ./app <bmp图片> <存放取模的文件>\n");
		return 0;
	}
	
	int err=GetBmpModel(argv[1],argv[2]);
	if(err)printf("图片取模错误，错误值=%d\n",err);
	else
	{
		printf("图片取模成功!\n");
		sprintf(cmd_buff,"gedit %s &",argv[2]);
		system(cmd_buff); //系统函数。可直接执行shell命令行任何指令
	}
	return 0;
}

