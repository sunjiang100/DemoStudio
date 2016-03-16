/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 		  
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名:   Algol.c
	功能描述：一些常的小函数
	编译环境：C语言编译器
	目标CPU:  
	作者：
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  
#include    "..\Config\UserConfig.h"
#include		<string.h>
#include  	<stdlib.h>
#include		"UserGlobals.h"



unsigned short const crc_ta256[256]=
{ /* CRC 余式表 */
0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

/**********************************************************************************************
** 函数名称：CameraCls_CalCRC16()
** 功能描述：按字节查表计算序列CRC，此法占空间稍大，但计算快。
** 入口参数：数据存放指针，数据长度  			 
** 出口参数：校验值
** 全局变量: 
** 修改时间：
**********************************************************************************************/
unsigned short 		CalCRC16( unsigned char * ptr, unsigned short len) 
{
	unsigned short crc;
	unsigned char 	da;

	crc=0;
	while(len--!=0) 
	{
		da=(unsigned char) (crc/256); 				// 以8位二进制数的形式暂存CRC的高8位
		crc<<=8; 									// 左移8位,相当于CRC的低8位乘以28
		crc^=crc_ta256[da^*ptr]; 					// 高8位和当前字节相加后再查表求CRC,再加上以前的CRC
		ptr++;
	}
	return(crc);
}//End of CalCRC16() function




void  Encrypt  ( ENCRY_KEY  *pKey, unsigned char *Buffer, unsigned short size ) 
{
	
	unsigned long  Key, M1,IA1,IC1;
	unsigned short idx = 0; 
	Key = pKey->Key;
	M1 = pKey->A;
	IA1 = pKey->B;
	IC1 = pKey->C;
	
	if( Key == 0 ) 	Key = 1; 
	if( M1 ==0 ) 		M1 = 1;
	if(IA1==0)			IA1 = 1;
	if(IC1==0)			IC1 = 1;
	while( idx < size ) 
	{
		Key = IA1 * ( Key % M1 ) + IC1; 
		Buffer[idx++] ^= (unsigned char) ((Key>>20)&0xFF); 
	} 
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：ValidIP_Port
   功能描述：检测IP及端口的合法性
   入口参数：IP和端口的分隔符，IP和端口字符串
   出口参数：合法标志，TRUE为合法，非TRUE为非法
   全局变量: 
   创建时间：2008-1-15 19:28
   修改时间：
	注：
0.0.0.0:0   255.255.255.255:65535
*/
unsigned int	Str2Int(char *str);
uchar    ChekDNSorIP(uchar *sP);
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uchar			ValidIP_Port(uchar	*pIP)
{
	uint16	Len;
	uchar		i,c=0,Dot,D1,D2,D3,Seg;
	NOP();	NOP();
	
 
	c = ChekDNSorIP(pIP);
	if(c==ERROR)
	  return ERROR;
	else if(c==IP)					 
	{
		*(pIP+22) = 0;
		////////////////////////////////////////////////////
		Len = strlen((char*)pIP);
		if((Len<9)||(Len>21))		return ERROR;		// 长度出错
		////////////////////////////////////////////////////
		Dot = D1 = D2 = D3 = Seg  = 0;
		for(i=0;i<Len;i++)
		{
			c = *(pIP+i);
			if(c=='.')	
			{
				Dot++;
				if(Dot==1)	D1 = i+1;
				else if(Dot==2)	D2 = i+1;
				else if(Dot==3)	D3 = i+1;
			}
			else if( (c==',')||(c==':') )
			{
				if(Dot!=3)	return ERROR;
				if(Seg==0)	;//S1 = i+1;
				Seg++;
			}
			else if( (c<'0')||(c>'9') )
				return ERROR;		
		}
		//if( (Dot!=3)||(Seg!=1))		return ERROR;
		if( (Dot!=3))		return ERROR;
		if( ((*(pIP+D1))<'0') || ((*(pIP+D1))>'9')|| (Str2Int((char*)(pIP+D1))>255) )		
			return ERROR;
		if( ((*(pIP+D2))<'0') || ((*(pIP+D2))>'9')|| (Str2Int((char*)(pIP+D2))>255) )		
			return ERROR;	
		if( ((*(pIP+D3))<'0') || ((*(pIP+D3))>'9')|| (Str2Int((char*)(pIP+D3))>255) )		
			return ERROR;
		//if( ((*(pIP+S1))<'0') || ((*(pIP+S1))>'9')|| (Str2Int((char*)(pIP+S1))>65535) )		
		//	return ERROR;	
	}
	else if(*pIP!=0)		  //DNS
	{
		for(i=0; i<32; i++)
		{
		   if(*(pIP+i) != 0)
			{
				if((*(pIP+i)<=0x20)||(*(pIP+i)>0x80))			  //域名只支持ASCII码
				 return ERROR; 
			}
			else
			  return  OK;
		}
	}
	else
	  return ERROR; 

   return  OK;
	////////////////////////////////////////////////////	
}		


uchar    ChekDNSorIP(uchar *sP)
{
   uchar  i;
   for(i=0; i<32; i++)
	{
	   if((i<3)&&(*(sP+i) == 0))
		   return ERROR;
		if(*(sP+i) != 0)
		{
			if(((*(sP+i)<0x30)||(*(sP+i)>0x3A))&&((*(sP+i)!=0x2C)&&(*(sP+i)!=0x2E)))			  //判断是域名还是IP
			{
			   return DNS; 
			}
		}
		else
		  break;
	}
	 return IP;
	

 }

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：HexToBcd
   功能描述：把一字节小于100的十六进制变成一字节压缩的BCD码
   入口参数：待转换的十六进制数   			 
   出口参数：压缩BCD码，返回0xff表示错误
   全局变量: 
   创建时间：
   修改时间：
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uchar         HexToBcd(uchar Hex)
{
	uchar			BH,BL;
	NOP();	NOP();
	if(Hex>99)
		return 0xff;
	BH = Hex/10;
	BH <<= 4;
	BL = Hex%10;
	BH |= BL;
	return BH;	
} // End of Function




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：BcdToHex
   功能描述：把一字节小于100的压缩的BCD码变成十六进制
   入口参数：待转换的压缩的BCD码   			 
   出口参数：十六进制数，返回0xff表示错误
   全局变量: 
   创建时间：
   修改时间：
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uchar         BcdToHex(uchar Bcd)
{
	uchar			HH,HL;
	NOP();	NOP();
	HH = Bcd>>4;
	HL = Bcd&0x0f;
	if((HH>9)||(HL>9))
		return 0xff;
	return HH*10 + HL;	
} // End of Function



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：UINT32HexToBcd
   功能描述：把四字节十六进制转为BCD码
   入口参数：  			 
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void         UINT32HexToBcd(uint32 INTData, uchar *BufferBCD )
{
	uchar			DataBuffer[9];
	DataBuffer[0] = INTData/10000000;	INTData %= 10000000;
	DataBuffer[1] = INTData/1000000;	   INTData %= 1000000;
	DataBuffer[2] = INTData/100000;		INTData %= 100000;
	DataBuffer[3] = INTData/10000; 		INTData %= 10000;
	DataBuffer[4] = INTData/1000; 		INTData %= 1000;
	DataBuffer[5] = INTData/100; 		   INTData %= 100;
	DataBuffer[6] = INTData/10; 			
	DataBuffer[7] = INTData%10;
	*BufferBCD = DataBuffer[0];
	*BufferBCD <<= 4;
	*BufferBCD |=  DataBuffer[1];
	BufferBCD++;
	*BufferBCD = DataBuffer[2];
	*BufferBCD <<= 4;
	*BufferBCD |=  DataBuffer[3];
	BufferBCD++;
	*BufferBCD = DataBuffer[4];
	*BufferBCD <<= 4;
	*BufferBCD |=  DataBuffer[5];
	BufferBCD++;
	*BufferBCD = DataBuffer[6];
	*BufferBCD <<= 4;
	*BufferBCD |=  DataBuffer[7];
} // End of Function


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：UINT32HexToBcd
   功能描述：4字节的BCD码转为四字节的无符号整型
   入口参数：  			 
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uint32         BcdToUINT32Hex(uchar *BufferBCD)
{
	uint32	Value;

	if (BufferBCD==NULL)	return 0xffffffff;

	Value =  BcdToHex(BufferBCD[0]) * 1000000UL + BcdToHex(BufferBCD[1]) * 10000UL + \
			BcdToHex(BufferBCD[2]) * 100 + BcdToHex(BufferBCD[3]);


	return Value;	
	
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：HalfByteToHexChar
   功能描述：把一小于16的十六进制的数据转换成十六进制字符
   入口参数：待转换的数据   			 
   出口参数：已转换字符，为0表示错误
   全局变量: 
   创建时间：
   修改时间：
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uchar			HalfByteToHexChar(uchar Data)
{
	 if(Data>0x0f)		return 'F';
	 if(Data<=9)		return (Data+0x30);
	 if(Data>9)	 		return (Data-0x0a+'A');
	 else
	 	return '0';
}




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：ByteToHexChar
   功能描述：
   入口参数：
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			ByteToHexChar(uchar	Data,char *pBuffer)
{
	*(pBuffer+0) = HalfByteToHexChar(Data>>4);
	*(pBuffer+1) = HalfByteToHexChar(Data&0x0f);
	*(pBuffer+2) = '\0';
} // End of <ByteToHexChar> function


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：Int2Str
   功能描述：
   入口参数：
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void 		Int2Str(unsigned int Data, char *str)
{
	char *p1, *p2;
	char tmp;

	p1 = str;
	p2 = str;
	do 
	{
		*p2++ = Data%10+'0';
		Data /= 10;
	} while(Data!=0);
	*p2-- = '\0';

	while(p1<p2)  //交换两个数
	{
		tmp = *p1;
		*p1++ = *p2;
		*p2-- = tmp;
	}
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：Int2Bcd;
   功能描述：
   入口参数：
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void		Int2Bcd8(uint32 Data,uchar  *pBcd)
{
	uchar		i,j,t,Len;
	char 		tBuffer[11]={0};
	char		tBuf[11]={0};
	Int2Str(Data,tBuffer);
	Len = strlen(tBuffer);
	if( Len<8)
	{
		for(i=0;i<(8-Len);i++)
		{
			tBuf[i]= '0';
		}
		memmove( &tBuf[i],tBuffer,Len);
	}
	for(j=0,i=0;i<4;i++)
	{
		t = tBuf[j++]-'0';
		t <<= 4;
		t |= tBuf[j++]-'0';
		*(pBcd+i) = t;
	}
}

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：Int2Bcd;
   功能描述：整型转压缩BCD码，最高位在第0个字节
   入口参数：
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：Int2Bcd;
   功能描述：
   入口参数： uint32 Data,uchar  *pBcd, BCD码的字节数为uchar Len
				  BCD码的第0字节为最高位，数值不足用0填充，第Len字节为数值的最低位
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void		Int2Bcd(uint32 Data,uchar  *pBcd,uchar Len)
{
	
	uchar	DataBuffer[10]; 
	uchar   BCDBuffer[7]; 
//	uchar   tmp[7];
	uint32 i;	 //Value,  ,j
 
   //SetBufferValue(pBcd,Len,0);
      
	for(i=10;i;i--)
	{
	   DataBuffer[i-1] = Data %10;
	   Data /= 10;
	}
 
	for(i=0;i<5;i++)
	{
	  BCDBuffer[i] = DataBuffer[2*i];
	  BCDBuffer[i] <<= 4;
	  BCDBuffer[i] |= DataBuffer[2*i+1];
	}
 
	for(i=1;i<=Len;i++)

	  pBcd[Len-i] = BCDBuffer[5-i];


}




uint32 Power(uint32 Value,uchar time)
{
    uint32  RetValue,i;
    
    
    if(time == 0)
      return 1;
    RetValue = Value;  
    for(i=0;i<(time-1);i++)
    {
       RetValue *= Value;
    
    }    
    return RetValue;
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：Bcd2Int
   功能描述：压缩BCD码解压成整型
   入口参数：压缩BCD码 pBcd,  uchar Len 为pBcd 的字节个数
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用

uint32	Bcd2Int(uchar  *pBcd,uchar Len)
{
	uint32 Value,i,j;  //,valueH,valueL
	uchar DataBuffer[10],tmp;
 
	for(i=0;i<Len;i++)
	{
       tmp = pBcd[i];
	    DataBuffer[2*i+1] = tmp & 0x0F;
	    DataBuffer[2*i] = (tmp >>4) & 0x0F;
	    
	}
 
    Value = 0;
    for(i=0;i<Len*2;i++)
    {
      if(DataBuffer[i]>0)
      {

      	for(j=0;j<Len*2;j++)
       	  Value += DataBuffer[i+j] * Power(10,2*Len-i-j-1);
            return Value;
      }
    
    }
 	return 0;
}
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：Str2Int
   功能描述：
   入口参数：
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
unsigned int	Str2Int(char *str)
{
	unsigned int	Data;
	uchar	 c;
	Data = 0;
	c = *str;
	if( (c<'0')||(c>'9') )	return 0;
	while(c)
	{
		Data *= 10;		
		Data += (c-'0');		
		str++;
		c = *str;
		//if(c=='.')	{ str++;	c = *str; }
		if( (c<'0')||(c>'9') )	break;
	}
	return Data;
}




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：Int64ToDecChar
   功能描述：
   入口参数：   			 
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void				Int64ToDecChar(unsigned long long dat, char *str)
{
	char *p1, *p2;
	char tmp;

	p1 = str;
	p2 = str;
	do 
	{
		*p2++ = dat%10+'0';
		dat /= 10;
	} while(dat!=0);
	*p2-- = '\0';

	while(p1<p2) 	 //交换两个数的内容可以不用借助中间变量,使用3次异或直接交换.
	{
		tmp = *p1;
		*p1++ = *p2;
		*p2-- = tmp;
	}
}	// End of <IntToDecChar> function










/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：Int32ToDecChar
   功能描述：把一32位整型变成十进制字符串
   入口参数：待转换的数据，存储的字符串指针   			 
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
	注：输出的字符串左边对齐，右边用结束符'\0'填补
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void				Int32ToDecChar(uint32 Data,char *pBuffer)
{
  	Int2Str(Data,pBuffer);

}	// End of <IntToDecChar> function




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：Int16ToDecChar
   功能描述：把一16位整型变成十进制字符串
   入口参数：待转换的数据，存储的字符串指针   			 
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
	注：输出的字符串左边对齐，右边用结束符'\0'填补
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			Int16ToDecChar(uint16 Data,char *pBuffer)
{
	Int2Str(Data,pBuffer);

}// End of <IntToDecChar> function





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：DecCharToUint32
   功能描述：把十进制字符串(可含小数点)变成32位的整型
   入口参数：最长位数，以非数字结束(小数点除外)的十进制字符串
   出口参数：32位的整型数
   全局变量: 
   修改时间：
	注：以非数字且非小数点字符结束，含小数和结束符，最多12字节
	    如果没有小数点，可直接调用C语言标准库函数r = atol(*s)

*/ 
uint32			DecCharToUint32(uchar Len,char *str)
{
	uchar	 c;
	char		tBuffer[16];
	unsigned int	Data;

	memmove(tBuffer,str,Len);
	tBuffer[Len] = 0;
	str = tBuffer;	

	Data = 0;
	c = *str;
	while(c)
	{
		Data *= 10;		
		Data += (c-'0');		
		str++;
		c = *str;
		if(c=='.')	{ str++;	c = *str; }
	}
	return Data;
}






uint32		My_atol(char *p)
{
	return Str2Int(p);
}









/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：SetBufferValue
   功能描述：设置缓冲区值
   入口参数：缓冲区，长度，设置值   			 
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
void			SetBufferValue(uchar *Buffer,uint16 Len,uchar Data)
{
	uint16	i;
	for(i=0;i<Len;i++)	{ *(Buffer+i) = Data;	}
}








/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：NByteBcdToHex
   功能描述：多字节压缩BCD码转换成16进制
   入口参数：BCD码串，长度
   出口参数：无
   全局变量: 				
   创建时间：
   修改时间： 
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			NByteBcdToHex(uchar *pBCD,uchar Length)
{
	uchar		i,t;
	for(i=0;i<Length;i++)
	{
		t = *(pBCD+i);
		*(pBCD+i) = ( (t&0x0f) + ((t>>4)*10) );
	}
}



uchar			HalfByteHexCharToInt(uchar Hex)
{
	NOP();	NOP();
	if( (Hex>='0')&&(Hex<='9') )
		return Hex-'0';
	if( (Hex>='a')&&(Hex<='f') )
		return (Hex-'a'+0x0a);
	if( (Hex>='A')&&(Hex<='F') )
		return (Hex-'A'+0x0a);
	return 0xff;
}




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：HexCharToInt
   功能描述：把Hex字符转换为Hex
   入口参数：以'\0'结束的Hex字符串
   出口参数：转换结果
   全局变量: 无
   创建时间：
   修改时间：
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uint32		HexCharToInt(char *pBuffer)
{
	uchar		Byte;
	uint32	Data=0;
	NOP();	NOP();
	while((*pBuffer))
	{			   		
		Byte = HalfByteHexCharToInt(*pBuffer);
		if(Byte==0xff)	return Data;
		Data <<= 4;
		Data |= Byte;
		pBuffer++;
	}
	return Data;

} // End of <IntToHexChar> function


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：HexCharToByte
   功能描述：
   入口参数：2位字符
   出口参数：一个字节数值
   全局变量: 无
   创建时间：2009.11.05
   修改时间：
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uchar		HexCharToByte(char *pBuffer)
{
	char	tmp1,tmp2; 
	
	tmp1 = *pBuffer;
	tmp2 = *(pBuffer+1);
		
	if((tmp1>='0')&&(tmp1<='9'))
		tmp1 = tmp1-'0';
	if((tmp1>='a')&&(tmp1<='f'))
		tmp1 = tmp1-'a'+0x0a;
	if((tmp1>='A')&&(tmp1<='F'))
		tmp1 = tmp1-'A'+0x0a;
	if((tmp2>='0')&&(tmp2<='9'))
		tmp2 = tmp2-'0';
	if((tmp2>='a')&&(tmp2<='f'))
		tmp2 = tmp2-'a'+0x0a;
	if((tmp2>='A')&&(tmp2<='F'))
		tmp2 = tmp2-'A'+0x0a;
	
	return	((tmp1<<4) + tmp2);
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：Int32ToHexChar
   功能描述：把一32位整型变成十六进制字符串
   入口参数：待转换的数据，HEX字符数，存储的字符串指针   			 
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			IntToHexChar(uint32	Data,uchar Len, char *pBuffer)
{
	uchar		Tp[16];
	uint16	i;	

	if((Data==0)||(Len>16))
	{
		for(i=0;i<Len;i++)	*(pBuffer+i) = '0';
		return;
	}

	for(i=0;i<Len;i++)
	{	
		Tp[i] = 0x0000000f&(Data>>((Len-1-i)*4));	
		*(pBuffer+i) = HalfByteToHexChar(Tp[i]);
	}	

} // End of <IntToHexChar> function








/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：ByteTo3DecChar
   功能描述：把一字节十六进制的数据转换成三字节十进制字符
   入口参数：待转换的数据，字符指针   			 
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			ByteTo3DecChar(uchar Data,char *p)
{
	*p = Data/100 + '0';
	*(p+1) = Data%100/10 + '0';
	*(p+2) = Data%10 + '0';		
} // End of <ByteToDecChar> function


void		ByteToDecChar(uchar Data,uchar *p)
{
	Int2Str(Data,(char*)p);
}

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：ByteTo2DecChar
   功能描述：把一字节小于100的数据转换成二字节十进制字符
   入口参数：待转换的数据，字符指针   			 
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			ByteTo2DecChar(uchar Data,char *p)
{
	*(p+0) = Data%100/10 + '0';
	*(p+1) = Data%10 + '0';
} // End of <ByteToDecChar> function



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：CalAddingSum
   功能描述：计算指定长度和缓冲数据的异或和
   入口参数：长度，数据指针
   出口参数：异或和
   全局变量: 无
   创建时间：
   修改时间：	
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uint16			CalAddingSum(uint16 Len,uchar *p)
{
	uint16	i,s;
	NOP();	NOP();
	for(s=0,i=0;i<Len;i++)	{  s += *(p+i);	}
	return s;
}

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：CalAddingSum32
   功能描述：计算指定长度和缓冲数据的和
   入口参数：长度，数据指针
   出口参数：32bit和
   全局变量: 无
   创建时间：
   修改时间：	
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uint32			CalAddingSum32(uint32 Len,uchar *p)
{
	uint32	i,s;
	NOP();	NOP();
	for(s=0,i=0;i<Len;i++)	{  s += *(p+i);	}
	return s;
}
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：CalXorSum
   功能描述：计算指定长度和缓冲数据的异或和
   入口参数：长度，数据指针
   出口参数：异或和
   全局变量: 无
   创建时间：
   修改时间：	
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uchar			CalXorSum(uint16 Len,uchar *p)
{
	uchar		s;
	uint16	i;
	NOP();	NOP();
	for(s=0,i=0;i<Len;i++)	{  s ^= *(p+i);	}
	return s;
}

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：MyStrCmp
   功能描述：在一个字符串中查找另一个字符串
   入口参数：   			 
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uchar			MyStrCmp(char *pSrc,char *pDst)
{
	uint16  i,LenS,LenD;
	LenS = strlen(pSrc);
	LenD = strlen(pDst);
	if( LenS != LenD )	return NOT_OK;
	for(i=0;i<LenS;i++)
	{
		if( *(pSrc+i) != *(pDst+i) )	return NOT_OK;
	}
	return OK;
}

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：MyStrCmpNB
   功能描述：在一个字符串中查找另一个字符串
   入口参数：   			 
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uchar			MyStrCmpNB(uint16	Len,char *pSrc,char *pDst)
{
	uint32	i;
	for(i=0;i<Len;i++)
	{
		if( *(pSrc+i) != *(pDst+i) )	return NOT_OK;
	}
	return OK;
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：strstr
   功能描述：在一个字符串中查找另一个字符串
   入口参数：   			 
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
char		*MyStrStr(char *pSrc,char *pDst)
{
	uint16	i,j,LenS,LenD;
	LenS = strlen(pSrc);
	LenD = strlen(pDst);
	for(i=0;i<LenS;i++)
	{			
		if((*(pSrc+i)) == (*pDst))
		{
			for(j=0;j<LenD;j++)
			{
				if((*(pDst+j)) == (*(pSrc+i+j)))
					continue;
				else
					break;
			}
			if(j==LenD)		return (pSrc+i);
		}
	}
	return NULL;
}




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：SearchArray
   功能描述：在一个数组中查找另一个数组
   入口参数：   			 
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uchar		*SearchArray(uint16 LenS,uchar *pSrc,uint16 LenD,uchar *pDst)
{
	uint16	i,j;
	if((LenS==0)||(LenD==0))	return NULL;
	for(i=0;i<LenS;i++)
	{			
		if((*(pSrc+i)) == (*pDst))
		{
			for(j=0;j<LenD;j++)
			{
				if((*(pDst+j)) == (*(pSrc+i+j)))
					continue;
				else
					break;
			}
			if(j==LenD)		return (pSrc+i);
		}
	}
	return NULL;
}

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：SearchArrayCount
   功能描述：在一个数组中查找另一个数组个数
   入口参数：   			 
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用

uint16		SearchArrayCount(uint16 LenS,uchar *pSrc,uint16 LenD,uchar *pDst)
{
	uchar *ptr = NULL;
	uint16 count = 0;

	while (pSrc)
	{
		ptr = SearchArray(LenS, pSrc, LenD, pDst);
		if (ptr != NULL)
		{
			LenS -= (ptr - pSrc + 1);
			count++;
			ptr++;
		} 
		pSrc = ptr;
	}
	
	return count;		
}





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：MyStrChar
   功能描述：在一个字符串中查找另一个字符
   入口参数：   			 
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
char		*MyStrChar(char *pSrc,char cDst)	
{
	char		c=1;
	while(c)
	{
		c = *pSrc;
		if(c == cDst)	return pSrc;
		pSrc++;
	}
	return NULL;
}	



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：MyStrChar
   功能描述：在一个字符串中出现指定字符的长度
   入口参数：   			 
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
char		MyStrLen(char *pSrc,char cDst)	
{
	char		*sp;
	sp = MyStrChar(pSrc, cDst);


	return (sp - pSrc);
}	















/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：SwapINT32
   功能描述：对4字节数据的高低字节调换
   入口参数：长型数据   			 
   出口参数：高低字节调换过长型数据
   全局变量: 
   创建时间：
   修改时间：
	注： ARM系统的存储是按高高低低的原则，即高字节存储在高地址，低字节存储在低地址
   	  51系统的存储是按高低低高的原则，即高字节存储在低地址，低字节存储在高地址
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
unsigned long 		SwapINT32(unsigned long dData)
{
	union
	{
		uchar	 	Buf[4];
		uint16	B16[2];
		uint32	B32;
	}D,S;
	S.B32 = dData;
	D.Buf[0] = S.Buf[3];
	D.Buf[1] = S.Buf[2];
	D.Buf[2] = S.Buf[1];
	D.Buf[3] = S.Buf[0];
	return D.B32;
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：SwapINT16
   功能描述：对双字节型的数据进行高低字节调换
   入口参数：原始双字节数   			 
   出口参数：高低字节调换过的数据
   全局变量: 
   创建时间：
   修改时间：
	注： ARM系统的存储是按高高低低的原则，即高字节存储在高地址，低字节存储在低地址
   	  51系统的存储是按高低低高的原则，即高字节存储在低地址，低字节存储在高地址
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uint16     	SwapINT16(uint16 dData)
{
	union
	{
		uchar	 	Buf[2];
		uint16	B16;		
	}D,S;
	S.B16 = dData;
	D.Buf[0] = S.Buf[1];
	D.Buf[1] = S.Buf[0];	
	return D.B16;
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：ByteToINT16
   功能描述：将2字节合并为一个双字节
   入口参数：原始2个单字节数，高字节在前，低字节在后   			 
   出口参数：合并后的双字节
   全局变量: 
   创建时间：
   修改时间：
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uint16		ByteToINT16(uchar D0,uchar D1)
{
	union
	{
		uchar	 	Buf[2];
		uint16	B16;		
	}D;
	D.Buf[0] = D1;
	D.Buf[1] = D0;
	return  D.B16;
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：ByteToINT32
   功能描述：将4个单字节合并为长型变量
   入口参数：原始4个单字节数，高字节在前，低字节在后
   出口参数：合并后的长型变量，存储符合高高低低原则
   全局变量: 
   创建时间：
   修改时间：
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uint32		ByteToINT32(uchar D0,uchar D1,uchar D2,uchar D3)
{
	union
	{
		uchar	 	Buf[4];
		uint16	B16[2];
		uint32	B32;
	}D;
	D.Buf[0] = D3;
	D.Buf[1] = D2;
	D.Buf[2] = D1;
	D.Buf[3] = D0;	
	return  D.B32;
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：FloatDecCharToInt
   功能描述：浮点十进制字符串转换为整数
   入口参数：1、浮点十进制字符串，整数部分最大0xffffffff，小数部分最大0xffffffff
   			 入口参数必须是在内存的字符串，必须有结束符'\0'，
				 入口字符串整数部分长度小于10，小数部分长度小于10
				 2、结果存储指针，四字节整数 + 四字节小数
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void       	FloatDecCharToInt(char *pBuffer,char *pInt)
{
	uchar		i,j,c;
	char		Buf[12];
	union
	{
		uint32	B32;
		uchar		B8[4];
	} H,L;
	
	////////////////////////////////// 处理整数部分
	for(i=0;i<10;i++)
	{
		c = *(pBuffer+i);
		if(c != '.')
			Buf[i] = c;
		else
			break;
	}	
	Buf[i++] = '\0';	
	H.B32 = My_atol(Buf);
	////////////////////////////////// 处理小数部分
	for(j=0;j<10;j++,i++)
	{
		c = *(pBuffer+i);
		if(c != '\0')
			Buf[j] = c;
		else
			break;		
	}
	Buf[j++] = '\0';
	L.B32 = My_atol(Buf);	
	if(Buf[0]=='0')			// 小数低位为0的处理
	{
		L.B32 /= 10;
		if(Buf[1]=='0')
		{
			L.B32 /= 10;
			if(Buf[2]=='0')
				L.B32 /= 10;
		}
	}
	////////////////////////////////// 处理返回数据
	for(i=0;i<4;i++)	{	*(pInt+i) = H.B8[i];	 }
	for(i=0;i<4;i++)	{	*(pInt+i) = L.B8[i];	 }	
	NOP();	NOP();
}	// End of <FloatDecCharToInt> function











	
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：DecCharToPressBCD
   功能描述：把十进制字符串(可含小数点)变成压缩BCD码
   入口参数：十进制字符串及长度，BCD指针
   出口参数：BCD字节数
   全局变量: 
   创建时间：
   修改时间：
	注：不检测十进制字符的合法性
*/ 
uchar			DecCharToPressBCD(uchar Len,char *pDec,uchar *pBCD)
{
	uchar		i,j,th,tl;
	uchar		Dec[20] = {0};
	memmove(Dec,pDec,Len);
 	for(i=0;i<Len;i++)			// 如果有小数点，去掉之
	{
		if((Dec[i])=='.')
		{
			while(i<Len)
			{
				Dec[i] = Dec[i+1];
				i++;
			}
			Dec[Len] = 0;
			Len --;
			break;
		}
	}
	if(Len%2)	Len += 1;
	Len >>= 1;

	for(i=0;i<Len;i++)
	{
		j = i<<1;
		th = Dec[j];		
		th -= '0';
		th <<= 4;				// 字节的高4位
		tl = Dec[j+1];
		if(tl>='0')
		tl -= '0';
		*(pBCD+i) = th|tl;
	}
	return Len;
}	


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：FourByte6BitTo3Byte8Bit
   功能描述：把4字节低6Bit有效的数转换为3字节
   入口参数：源指针，目的指针
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
	注：对齐方式为高高低低
*/
//static	volatile	 	uint16	LenFree;
static	volatile	 	uchar		*pNow=NULL;
uchar		MyMallocBuf[LEN_PUBLIC_FREE_MEM];// __attribute__((at(0x7fe03c00)));
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uchar			*MyMalloc(uint16  Len)
{
	NOP();	NOP();
	if(Len>LEN_PUBLIC_FREE_MEM)	return NULL;	
	if(pNow == NULL)
	{
		pNow = MyMallocBuf;
		return MyMallocBuf;		
	}
	return NULL;	
}


void			MyFree(uchar *pFree)
{
	pNow = NULL;
}





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：IgnoreSpecifyCharacter
   功能描述：忽略指定的字符
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
	注：此函数比较耗机时，如果开个缓冲就很节省时间。无内存空间只能慢慢移动，有内存空间
只移动非０到另一个指针
	忽略空格可用i++和i--，但忽略０不能
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用	
uint16			IgnoreSpecifyCharacter(uchar cChar,uint16 Len,uchar *pBuf)
{ 	
	uchar		c;
	uchar		*pMal=NULL;
	uint16	i=0,j;
	NOP();	NOP();	
	
	while(pMal==NULL)
	{
		pMal = MyMalloc(Len);
		os_dly_wait(1);
		if(++i>100)	return 0;
	}
	for(i=0,j=0;i<Len;i++)
	{
		c = *(pBuf+i);
		if(c != cChar)
		{
			*(pMal+j) = c;
			j++;
		}							
	}	
	memmove(pBuf,pMal,j);
	MyFree(pMal);
	return j;
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：FourByte6BitTo3Byte8Bit
   功能描述：把4字节低6Bit有效的数转换为3字节
   入口参数：源指针，目的指针
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
	注：对齐方式为高高低低
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			FourByte6BitTo3Byte8Bit(char *pSrc,uchar *pDst)
{
	union
	{
		uint32	B32;
		uchar		Buf[4];
	} tl;  		
	tl.B32 =0;
	tl.B32 = (*(pSrc+0));
	tl.B32 <<= 6;
	tl.B32 |= ((*(pSrc+1))&0x3f);
	tl.B32 <<= 6;
	tl.B32 |= ((*(pSrc+2))&0x3f);
	tl.B32 <<= 6;
	tl.B32 |= ((*(pSrc+3))&0x3f);		
	*(pDst+0) = tl.Buf[0];
	*(pDst+1) = tl.Buf[1];
	*(pDst+2) = tl.Buf[2];
}








/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：转义字符编码，0x7e -> 0x7d 0x02, 0x7d -> 0x7d 0x01
   入口参数：数据源指针
   出口参数：长度				
   全局变量: 
   创建时间：
   修改时间：
	注：对齐方式为高高低低
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uint16		EscapeCharEncode(uint16 Len,uchar *pBuf)
{ 	
	uchar		c;
	uchar		*pMal=NULL;
	uint16	i=0,j;
	NOP();	NOP();	
	
	while(pMal==NULL)
	{
		pMal = MyMalloc(Len);
		os_dly_wait(1);		
	}
	for(j=0,i=0;i<Len;i++)
	{
		c = *(pBuf+i);
		if(c == 0x7e)		  	// 0x7e -> 0x7d 0x02
		{
			*(pMal+j) = 0x7d;		j++;
			*(pMal+j) = 0x02;		j++;
		}							
		else if(c == 0x7d)	// 0x7d -> 0x7d 0x01
		{
			*(pMal+j) = 0x7d;		j++;
			*(pMal+j) = 0x01;		j++;
		}
		else
		{
			*(pMal+j) = c;		j++;
		}
	}	
	memmove(pBuf,pMal,j);
	MyFree(pMal);
	return j;
}





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：转义字符编码，0x7d 0x02 -> 0x7e, 0x7d 0x01 -> 0x7d
   入口参数：数据源指针
   出口参数：长度
   全局变量: 
   创建时间：
   修改时间：
	注：对齐方式为高高低低
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uint16		EscapeCharDecode(uint16 Len,uchar *pBuf)
{ 	
	uchar		c,c1;
	uchar		*pMal=NULL;
	uint16	i=0,j;
	NOP();	NOP();	
	
	while(pMal==NULL)
	{
		pMal = MyMalloc(Len);
		os_dly_wait(1);		
	}
	for(j=0,i=0;i<Len;i++)
	{
		c = *(pBuf+i);
		c1 = *(pBuf+i+1);
		if( (c == 0x7d)&&(c1==0x02)	)	  	// 0x7d 0x02 -> 0x7e
		{
			*(pMal+j) = 0x7e;		j++;	
			i++;		
		}							
		else if( (c == 0x7d)&&(c1==0x01)	)	// 0x7d 0x01 -> 0x7d
		{
			*(pMal+j) = 0x7d;		j++;			
			i++;
		}
		else
		{
			*(pMal+j) = c;		j++;
		}
	}	
	memmove(pBuf,pMal,j);
	MyFree(pMal);
	return j;
}




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：CprDataValue
   功能描述：判断一段时间是否在某两个值之间
   入口参数：数据长度，期望最大长度，最小值，最大值，数据源指针
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
	注：
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uchar   CprDataValue(uchar Len, uchar L ,uchar MinData, uchar MaxDara, uchar *p)
{
  uchar 	 err = 0,i;
  if(Len>L)
	{
		err=1;
	}
	else
	{
		for(i=0;i<Len;i++)
		{
		 	if((*(p+i)<MinData)||(*(p+i)>MaxDara))
			{
				err=1;
			}
		}
	}
   return err;
}




/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
