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


