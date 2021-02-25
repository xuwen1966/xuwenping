数据的压缩存储与解压缩算法实现(C语言)

在一些嵌入式的项目设计中，空间是相当宝贵的，因为一个CPU的存储是有限的，所以此时我们在保存数据的时候，喜欢来进行压缩保存，著名的有哈夫曼树算法，专门用来做压缩的算法，当然，本节我们不讨论这些稍微高级的算法，写一个简单的例子，开发中遇见的，比如,我想把0xfe,0xf1,0x3e,0x3c这四个字节的数据存放在8个字节的buf里去，低4位在前，高4位在后，依次类推，存够8个byte。这时第一反应就是采用数组来进行设计这样的一个算法，如何设计？
	假设,uchar tab[] = {0xfe , 0xf1 , 0x3e , 0x3c} ; uchar table[8] ; 我需要将tab的数据存放到table中去。
[cpp] view plain copy print?
/* 
    table[0] = tab[0] & 0x0f ;  //取低位  
    table[1] = tab[0] & 0xf0 ； //取高位 
    table[2] = tab[1] & 0x0f ;  //取低位  
    table[3] = tab[1] & 0xf0 ;  //取高位 
    ... 
    table[6] = tab[3] & 0x0f ;  //取低位  
    table[7] = tab[3] & 0xf0 ;  //取高位 
 
*/  
由数据规律可以得知,当i为奇数时，取高位，为偶数时，取低位,并且，当为奇数时，取到tab的下一个buf里的值 。
我们可以用如下代码来实现,即可得到相应的把4个byte的数据存储到8个byte的数组里去.
[cpp] view plain copy print?
#include <stdio.h>  
unsigned char tab[4] = {0xfe,0xf1,0x3e,0x3c} ;  
unsigned char table[8] ;  
int main(void)  
{  
    int i ;  
    int j ;  
    int count = 0 ;  
    for(i = 0 ; i < 8 ; i++)  
    {  
        if(i % 2 == 0)  
            table[i] = tab[count] & 0x0f ;  
        if(i % 2 != 0)  
        {  
            table[i] = tab[count] & 0xf0 ;  
            count++ ;  
        }  
    }  
    for(i = 0 ; i < 8 ; i++)  
        printf("table[%d]:0x%x\n",i,table[i]) ;  
}  
[plain] view plain copy print?
运行结果:  
table[0] : 0xe   
table[1] : 0xf0   
table[2] : 0x1  
table[3] : 0xf0   
table[4] : 0xe  
table[5] : 0x30   
table[6] : 0xc   
table[7] : 0x30   
如果这个过程反过来，需要将8个byte的数据存到4个byte里去呢？由此我们可以有下面的规律:
假设savebuf用来存储4个byte的数据:
[cpp] view plain copy print?
/* 
    savebuf[0] = table[0] | table[1] ; 
    savebuf[1] = table[2] | table[3] ; 
    savebuf[2] = table[4] | table[5] ;  
    savebuf[3] = table[6] | table[7] ; 
*/  
同样观察出规律，由以下代码实现:
[cpp] view plain copy print?
#include <stdio.h>  
unsigned char tab[4] = {0xfe,0xf1,0x3e,0x3c} ;  
unsigned char table[8] ;  
int savebuf[4] ;  
  
int main(void)  
{  
    int i ;  
    int j ;  
    int low , high , count = 0 , beat_count ,value ;      
    count = 0 ;   
    beat_count = 0 ;  
    for(j = 0 ; j < 8 ; j++)  
    {  
        if(j % 2 == 0)  
            low = table[j] ;  
        else   
        {  
            high = table[j] ;  
            count++ ;  
            savebuf[beat_count] = low | high ;  
            beat_count++;  
        }  
    }  
    for(i = 0 ; i < 4 ; i++)  
        printf("savebuf[%d]:%x\n",i , savebuf[i]) ;  
}   
[plain] view plain copy print?
运行结果:  
savebuf[0]:fe  
savebuf[1]:f1  
savebuf[2]:3e  
savebuf[3]:3c  