#pragma once

#ifndef _INC_MD5_
#define _INC_MD5_

#include <stdio.h>


/**
**MD5.h
**/
typedef struct {
	unsigned long state[4];   /* state (ABCD) */
	unsigned long count[2];   /* number of bits, modulo 2^64 */
	unsigned char buffer[64];  /* input buffer */
} MD5;


MD5 *GetMD5();
void MD5Init (MD5 *context);
void MD5Update (MD5 *context, unsigned char *input, unsigned int inputLen);
void MD5Final (MD5 *context, unsigned char digest[16]);
void MD5Out (MD5 *md5, unsigned char *input, unsigned int inputLen, unsigned char out[16]);

#endif