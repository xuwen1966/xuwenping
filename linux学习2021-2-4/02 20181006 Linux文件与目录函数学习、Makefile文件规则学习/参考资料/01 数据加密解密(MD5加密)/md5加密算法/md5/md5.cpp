//
////获得MD5的二个数组和一个buffer并初始化
//MD5 *GetMD5();
////初始化MD5的二个数据和一个buffer
//void MD5Init (MD5 *context);
////用于计算MD5值的函数
//void MD5Update (MD5 *context, unsigned char *input, unsigned int inputLen);
////输出结果
//void MD5Final (MD5 *context, unsigned char digest[16]);
////对input数据做一次完整的MD5运算
//void MD5Out (MD5 *md5, unsigned char *input, unsigned int inputLen, unsigned char out[16]);
//
//
////计算一个文件的MD5值
//int 计算一个文件的MD5值(TCHAR* 文件路径, unsigned char md5值[16])
//{
//	MD5 context;
//	int 缓冲区长度 = 1024,
//		读取到的字节数;
//	unsigned char *缓冲区 = new unsigned char[缓冲区长度];
//	FILE *文件指针        = fopen(文件路径, "rb");
//	if(文件指针 == NULL)
//		return 1;
//	MD5Init(&context);
//	while ( (读取到的字节数 = fread (
//		缓冲区, 1, 缓冲区长度, 文件指针 )) !  =EOF)
//	{
//		MD5Update (&context, 缓冲区, 读取到的字节数);
//		//判断是否为已经读到文件尾
//		if ( 读取到的字节数 < 缓冲区长度 )
//			break;
//	}
//	MD5Final (&context, md5值);
//	free ( 缓冲区 );
//	return 0;
//}

/**
*MD5.cpp
*/

#include <string.h>
#include "md5.h"
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21
static void MD5Transform (unsigned long int state[4], unsigned char block[64]);
static void Encode (unsigned char *output, unsigned long int *input,unsigned int len);
static void Decode (unsigned long int *output, unsigned char *input, unsigned int len);
static void MD5_memcpy (unsigned char* output, unsigned char* input,unsigned int len);
static void MD5_memset (unsigned char* output,int value,unsigned int len);
static unsigned char PADDING[64] = {
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
/* F, G, H and I are basic MD5 functions.
*/
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))
/* ROTATE_LEFT rotates x left n bits.
*/
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))
/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
Rotation is separate from addition to prevent recomputation.
*/
#define FF(a, b, c, d, x, s, ac) {\
	(a) += F ((b), (c), (d)) + (x) + (unsigned long int)(ac); \
	(a) = ROTATE_LEFT ((a), (s));\
	(a) += (b);\
	}
#define GG(a, b, c, d, x, s, ac) {\
	(a) += G ((b), (c), (d)) + (x) + (unsigned long int)(ac);\
	(a) = ROTATE_LEFT ((a), (s));\
	(a) += (b);\
	}
#define HH(a, b, c, d, x, s, ac) {\
	(a) += H ((b), (c), (d)) + (x) + (unsigned long int)(ac);\
	(a) = ROTATE_LEFT ((a), (s));\
	(a) += (b);\
	}

#define II(a, b, c, d, x, s, ac) {\
	(a) += I ((b), (c), (d)) + (x) + (unsigned long int)(ac);\
	(a) = ROTATE_LEFT ((a), (s));\
	(a) += (b);\
	}

/* Note: Replace "for loop" with standard memcpy if possible.
*/
static void MD5_memcpy(char* output,char* input,unsigned int len)
{
	memcpy( output, input, len );
}


/* Note: Replace "for loop" with standard memset if possible.
*/
static void MD5_memset(char* output,int value,unsigned int len)
{
	memset( output, value, len );
}


MD5 *GetMD5()
{
	MD5 *md5 = new MD5();
	MD5Init(md5);
	return md5;
}


void MD5Init (MD5 *context)
{
	context->count[0] = context->count[1] = 0;
	/* Load magic initialization constants.*/
	context->state[0] = 0x67452301;
	context->state[1] = 0xefcdab89;
	context->state[2] = 0x98badcfe;
	context->state[3] = 0x10325476;
}


/* MD5 block update operation. Continues an MD5 message-digest
operation, processing another message block, and updating the
context.
*/
void MD5Update (MD5 *context, unsigned char *input, unsigned int inputLen)
{
	unsigned int i, index, partLen;

	/* Compute number of bytes mod 64 */
	index = (unsigned int)((context->count[0] >> 3) & 0x3F);

	/* Update number of bits */
	if ((context->count[0] += ((unsigned long int)inputLen << 3))
		< ((unsigned long int)inputLen << 3))
		context->count[1]++;
	context->count[1] += ((unsigned long int)inputLen >> 29);

	partLen = 64 - index;

	/* Transform as many times as possible.
	*/
	if (inputLen >= partLen) {
		memcpy((unsigned char*)&context->buffer[index],
			(unsigned char*)input, partLen);
		MD5Transform (context->state, context->buffer);

		for (i = partLen; i + 63 < inputLen; i += 64)
			MD5Transform (context->state, &input[i]);

		index = 0;
	}
	else
		i = 0;

	/* Buffer remaining input */
	memcpy((unsigned char*)&context->buffer[index], (unsigned char*)&input[i], inputLen-i);
}


/* MD5 finalization. Ends an MD5 message-digest operation, writing the
the message digest and zeroizing the context.
*/
void MD5Final (MD5 *context, unsigned char digest[16])
{
	unsigned char bits[8];
	unsigned int index, padLen;

	/* Save number of bits */
	Encode (bits, context ->count, 8);

	/* Pad out to 56 mod 64.
	*/
	index = (unsigned int)((context->count[0] >> 3) & 0x3f);
	padLen = (index < 56) ? (56 - index) : (120 - index);
	MD5Update ( context, PADDING, padLen);

	/* Append length (before padding) */
	MD5Update (context, bits, 8);
	/* Store state in digest */
	Encode (digest, context->state, 16);

	MD5Init(context);
}

void MD5Out(MD5 *md5, unsigned char *input, unsigned int inputLen, unsigned char out[16])
{
	MD5Update(md5, input, inputLen);
	MD5Final(md5, out);
}

/* MD5 basic transformation. Transforms state based on block.
*/
void MD5Transform (unsigned long int state[4], unsigned char block[64])
{
	unsigned long int a = state[0], b = state[1], c = state[2], d = state[3], x[16];

	Decode (x, block, 64);

	/* Round 1 */
	FF (a, b, c, d, x[ 0], S11, 0xd76aa478); /* 1 */
	FF (d, a, b, c, x[ 1], S12, 0xe8c7b756); /* 2 */
	FF (c, d, a, b, x[ 2], S13, 0x242070db); /* 3 */
	FF (b, c, d, a, x[ 3], S14, 0xc1bdceee); /* 4 */
	FF (a, b, c, d, x[ 4], S11, 0xf57c0faf); /* 5 */
	FF (d, a, b, c, x[ 5], S12, 0x4787c62a); /* 6 */
	FF (c, d, a, b, x[ 6], S13, 0xa8304613); /* 7 */
	FF (b, c, d, a, x[ 7], S14, 0xfd469501); /* 8 */
	FF (a, b, c, d, x[ 8], S11, 0x698098d8); /* 9 */
	FF (d, a, b, c, x[ 9], S12, 0x8b44f7af); /* 10 */
	FF (c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
	FF (b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
	FF (a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
	FF (d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
	FF (c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
	FF (b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

	/* Round 2 */
	GG (a, b, c, d, x[ 1], S21, 0xf61e2562); /* 17 */
	GG (d, a, b, c, x[ 6], S22, 0xc040b340); /* 18 */
	GG (c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
	GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa); /* 20 */
	GG (a, b, c, d, x[ 5], S21, 0xd62f105d); /* 21 */
	GG (d, a, b, c, x[10], S22,  0x2441453); /* 22 */
	GG (c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
	GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */
	GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */
	GG (d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
	GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */
	GG (b, c, d, a, x[ 8], S24, 0x455a14ed); /* 28 */
	GG (a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
	GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */
	GG (c, d, a, b, x[ 7], S23, 0x676f02d9); /* 31 */
	GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

	/* Round 3 */
	HH (a, b, c, d, x[ 5], S31, 0xfffa3942); /* 33 */
	HH (d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */
	HH (c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
	HH (b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
	HH (a, b, c, d, x[ 1], S31, 0xa4beea44); /* 37 */
	HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9); /* 38 */
	HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); /* 39 */
	HH (b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
	HH (a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
	HH (d, a, b, c, x[ 0], S32, 0xeaa127fa); /* 42 */
	HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); /* 43 */
	HH (b, c, d, a, x[ 6], S34,  0x4881d05); /* 44 */
	HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */
	HH (d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
	HH (c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
	HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 */

	/* Round 4 */
	II (a, b, c, d, x[ 0], S41, 0xf4292244); /* 49 */
	II (d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */
	II (c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
	II (b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */
	II (a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
	II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */
	II (c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
	II (b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */
	II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */
	II (d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
	II (c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */
	II (b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
	II (a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */
	II (d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
	II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */
	II (b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;

	/* Zeroize sensitive information.
	*/
	memset ((unsigned char*)x, 0, sizeof (x));
}

/* Encodes input (unsigned long int) into output (unsigned char).
* Assumes len is a multiple of 4.
*/
void Encode (unsigned char *output, unsigned long int *input,unsigned int len)
{
	unsigned int i, j;

	for (i = 0, j = 0; j < len; i++, j += 4) {
		output[j] = (unsigned char)(input[i] & 0xff);
		output[j+1] = (unsigned char)((input[i] >> 8) & 0xff);
		output[j+2] = (unsigned char)((input[i] >> 16) & 0xff);
		output[j+3] = (unsigned char)((input[i] >> 24) & 0xff);
	}
}


/* Decodes input (unsigned char) into output (unsigned long int). 
* Assumes len is a multiple of 4.
*/
void Decode (unsigned long int *output, unsigned char *input, unsigned int len)
{
	unsigned int i, j;
	for (i = 0, j = 0; j < len; i++, j += 4)
		output[i] = ((unsigned long int)input[j]) | (((unsigned long int)input[j+1]) << 8) |
		(((unsigned long int)input[j+2]) << 16) | (((unsigned long int)input[j+3]) << 24);
}

/*
使用说明：
将斜杠下面的源码复制到自己的源码中，在主函数之前加上函数声明，
之后便可在主函数中调用调用该函数了。
参数add是需要散列的数据的地址；
参数Length是需要散列的数据的长度（单位字节）；
参数add1是用来接受散列值的内存块的地址，该内存块的长度必须大于等于20字节；
//////////////////////////////////////////////////////////////////////////////////////
*/
void sha1( void *add,unsigned long length,void *add1 )
{
	unsigned char tempa;
	unsigned long hash[5];
	unsigned long long raw;
	unsigned long a,b,c,d,e;
	unsigned long buffer[20],temp;
	unsigned long offset=0,offset1,offset2;
	unsigned long count,count1,count2,count3=7;
	hash[0]=a=0x67452301;
	hash[1]=b=0xEFCDAB89;
	hash[2]=c=0x98BADCFE;
	hash[3]=d=0x10325476;
	hash[4]=e=0xC3D2E1F0;
	raw=length*8;
	offset1=length;
	offset2=length*8%512;
	if(offset2>=0&&offset2<=447)
		offset2=(512-offset2)/8;
	else
		offset2=(1024-offset2)/8;
	count=(offset2+length)/64;
	offset2=offset2+length-8;
	for(count1=1;count1<=count;count1++)
	{
		for(count2=0;count2<=63;count2++,offset++)
		{
			if(offset>=0&&offset<offset1)
			{*((unsigned char *)buffer+count2)=*((unsigned char *)add+offset);}
			else if (offset==offset1)
			{*((unsigned char *)buffer+count2)=128;}
			else if (offset>offset1&&offset<offset2)
			{*((unsigned char *)buffer+count2)=0;}
			else if (offset>=offset2)
			{
				*((unsigned char *)buffer+count2)=*((unsigned char *)(&raw)+count3);
				count3--;
			}
			else
			{;}
		}
		for(count2=0;count2<=15;count2++)
		{
			tempa=*((unsigned char *)(&buffer[count2])+3);
			*((unsigned char *)(&buffer[count2])+3)=*((unsigned char *)(&buffer[count2]));
			*((unsigned char *)(&buffer[count2]))=tempa;
			tempa=*((unsigned char *)(&buffer[count2])+2);
			*((unsigned char *)(&buffer[count2])+2)=*((unsigned char *)(&buffer[count2])+1);
			*((unsigned char *)(&buffer[count2])+1)=tempa;
		}
		temp=buffer[13]^buffer[8]^buffer[2]^buffer[0];
		buffer[16]=temp<<1|temp>>31;
		temp=buffer[14]^buffer[9]^buffer[3]^buffer[1];
		buffer[17]=temp<<1|temp>>31;
		temp=buffer[15]^buffer[10]^buffer[4]^buffer[2];
		buffer[18]=temp<<1|temp>>31;
		temp=buffer[16]^buffer[11]^buffer[5]^buffer[3];
		buffer[19]=temp<<1|temp>>31;
		for(count2=0;count2<=19;count2++)
		{
			temp=b&c|~b&d;
			temp=(a<<5|a>>27)+temp+e+buffer[count2]+0x5A827999;
			e=d;d=c;c=b<<30|b>>2;b=a;a=temp;
		}
		for(count2=0;count2<=19;count2++)
		{
			temp=buffer[(17+count2)%20]^buffer[(12+count2)%20];
			temp=temp^buffer[(6+count2)%20]^buffer[(4+count2)%20];
			buffer[count2]=temp<<1|temp>>31;
		}
		for(count2=0;count2<=19;count2++)
		{
			temp=b^c^d;
			temp=(a<<5|a>>27)+temp+e+buffer[count2]+0x6ED9EBA1;
			e=d;d=c;c=b<<30|b>>2;b=a;a=temp;
		}
		for(count2=0;count2<=19;count2++)
		{
			temp=buffer[(17+count2)%20]^buffer[(12+count2)%20];
			temp=temp^buffer[(6+count2)%20]^buffer[(4+count2)%20];
			buffer[count2]=temp<<1|temp>>31;
		}
		for(count2=0;count2<=19;count2++)
		{
			temp=b&c|b&d|c&d;
			temp=(a<<5|a>>27)+temp+e+buffer[count2]+0x8F1BBCDC;
			e=d;d=c;c=b<<30|b>>2;b=a;a=temp;
		}
		for(count2=0;count2<=19;count2++)
		{
			temp=buffer[(17+count2)%20]^buffer[(12+count2)%20];
			temp=temp^buffer[(6+count2)%20]^buffer[(4+count2)%20];
			buffer[count2]=temp<<1|temp>>31;
		}
		for(count2=0;count2<=19;count2++)
		{
			temp=b^c^d;
			temp=(a<<5|a>>27)+temp+e+buffer[count2]+0xCA62C1D6;
			e=d;d=c;c=b<<30|b>>2;b=a;a=temp;
		}
		hash[0]=hash[0]+a;
		hash[1]=hash[1]+b;
		hash[2]=hash[2]+c;
		hash[3]=hash[3]+d;
		hash[4]=hash[4]+e;
		a=hash[0];b=hash[1];c=hash[2];d=hash[3];e=hash[4];
	}
	for(count2=0;count2<=4;count2++)
	{
		tempa=*((unsigned char *)(&hash[count2])+3);
		*((unsigned char *)(&hash[count2])+3)=*((unsigned char *)(&hash[count2]));
		*((unsigned char *)(&hash[count2]))=tempa;
		tempa=*((unsigned char *)(&hash[count2])+2);
		*((unsigned char *)(&hash[count2])+2)=*((unsigned char *)(&hash[count2])+1);
		*((unsigned char *)(&hash[count2])+1)=tempa;
	}
	*((unsigned long *)add1)=hash[0];
	*((unsigned long *)add1+1)=hash[1];
	*((unsigned long *)add1+2)=hash[2];
	*((unsigned long *)add1+3)=hash[3];
	*((unsigned long *)add1+4)=hash[4];
	return;

}
/* 函数调用实现 */
static MD5 hash_context={0};
static unsigned char hash_result[16]={0};
static unsigned char hash_sha1[100]={0};
void main(void)
{
	int i;
	/* ab 187EF4436122D1CC2F40DC2B92F0EBA0 */
	/* a 0CC175B9C0F1B6A831C399E269772661 */
	/* b 92EB5FFEE6AE2FEC3AD71C777531578F */
	/* c 4A8A08F09D37B73795649038408B5F33 */
	/* ac E2075474294983E013EE4DD2201C7A73 */
	/* bc 5360AF35BDE9EBD8F01F492DC059593C */
	/* ca 5435C69ED3BCC5B2E4D580E393E373D3 */
	/* cb D0D7FDB6977B26929FB68C6083C0B439 */
	/* ba 07159C47EE1B19AE4FB9C40D480856C4 */ 
	/* abcdefghijklmnopqrstuvwxyz C3FCD3D76192E4007DFB496CCA67E13B */
	/* ABCDEFGHIJKLMNOPQRSTUVWXYZ 1E097BEA4D87C1741ADBEE7DDF0C259A */ 
	char *pName = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int len = strlen((char*)pName);

	MD5Init (&hash_context);

	// 使用一次和多次结果是一样的
	MD5Update (&hash_context, (unsigned char *)pName, len);
	//while (len>0)
	//{
	//	/* Perform hash calculation for the latest data-read. */
	//	MD5Update (&hash_context, (unsigned char *) pName, 1);
	//	pName++;
	//	len--;
	//}

	/* 运算结果hash_result */
	MD5Final (&hash_context,hash_result);
	printf("md5:\r\n");
	for(i=0;i<16;i++)
	{
		printf("%02X",hash_result[i]);
	}
	printf("\r\n");

	printf("sha1:\r\n");
	sha1(pName,len,hash_sha1 );
	for(i=0;i<20;i++)
	{
		printf("%02X",hash_result[i]);
	}
	printf("\r\n");

}