/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 		  
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 	 Algol.h
	功能描述：一些常的小函数的原形声明
	编译环境：
	目标CPU: 
	作者：
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  



extern		"C"	unsigned short 		CalCRC16( unsigned char * ptr, unsigned short len) ;


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：HexToBcd
   功能描述：把一字节小于100的十六进制变成一字节压缩的BCD码
   入口参数：待转换的十六进制数   			 
   出口参数：压缩BCD码，返回0xff表示错误
   全局变量: 
   创建时间：
   修改时间：
*/
extern		"C"		uchar         HexToBcd(uchar Hex);


extern		"C"		uchar			ValidIP_Port(uchar	*pIP);

extern		"C"	  uchar       ChekDNSorIP(uchar *sP);    //判断是IP地址还是DNS，或是无效地址


extern		"C"      void         UINT32HexToBcd(uint32 INTData, uchar *BufferBCD );
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：BcdToHex
   功能描述：把一字节小于100的压缩BCD码变成十六进制
   入口参数：待转换的压缩的BCD码   			 
   出口参数：十六进制数，返回0xff表示错误
   全局变量: 
   创建时间：
   修改时间：
*/
extern		"C"		uchar         BcdToHex(uchar Bcd);


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：NByteBcdToHex
   功能描述：多字节压缩BCD码转换成16进制
   入口参数：BCD码串，长度
   出口参数：无
   全局变量: 				
   创建时间：
   修改时间： 
*/ 	
extern		"C"		void			NByteBcdToHex(uchar *pBCD,uchar Length);


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：ByteToChar
   功能描述：把一小于16的十六进制的数据转换成十六进制字符
   入口参数：待转换的数据   			 
   出口参数：已转换字符，为0表示错误
   全局变量: 
   创建时间：
   修改时间：
*/
extern		"C"		uchar			HalfByteToHexChar(uchar Data);

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：ByteToHexChar
   功能描述：把单字节十六进制变成双字节十六进制字符串
   入口参数：待转换的数据，存储的字符串指针   			 
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/
extern		"C"		void			ByteToHexChar(uchar	Data,char *pBuffer);


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：IntToHexChar
   功能描述：把一32位整型变成十六进制字符串
   入口参数：待转换的数据，存储的字符串指针   			 
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/
extern		"C"		void			IntToHexChar(uint32	Data,uchar Len,char *pBuffer);


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：ByteTo3DecChar
   功能描述：把一字节十六进制的数据转换成三字节十进制字符
   入口参数：待转换的数据，字符指针   			 
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/
extern		"C"		void			ByteTo3DecChar(uchar Data,uchar *p);

extern		"C"		void			ByteToDecChar(uchar Data,uchar *p);
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：ByteTo2DecChar
   功能描述：把一字节小于100的数据转换成二字节十进制字符
   入口参数：待转换的数据，字符指针   			 
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/
extern		"C"		void			ByteTo2DecChar(uchar Data,char *p);


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
extern		"C"		void				Int32ToDecChar(uint32 Data,char *pBuffer);


extern		"C"		void				Int64ToDecChar(unsigned 	long long  Data,char *pBuffer);

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
extern		"C"		void		Int16ToDecChar(uint16 Data,char *pBuffer);


extern		"C"		void 		Int2Str(unsigned int dat, char *str);

extern		"C"		void		Int2Bcd8(uint32 Data,uchar  *pBcd);

extern		"C"		unsigned int	Str2Int(char *str);
extern		"C"      uint32	Bcd2Int(uchar  *pBcd,uchar Len);
extern		"C"		unsigned int	Str2Int(char *str);
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：CalXorSum
   功能描述：计算指定长度和缓冲数据的异或和
   入口参数：长度，数据指针
   出口参数：异或和
   全局变量: 无
   创建时间：
   修改时间：	
*/
extern		"C"		uchar			CalXorSum(uint16 Len,uchar *p);

extern		"C"		uint16			CalAddingSum(uint16 Len,uchar *p);	 

extern		"C"      uint32			CalAddingSum32(uint32 Len,uchar *p);


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：SetBufferValue
   功能描述：设置缓冲区值
   入口参数：缓冲区，长度，设置值   			 
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/
extern		"C"		void			SetBufferValue(uchar *Buffer,uint16 Len,uchar Data);

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：WordSwap
   功能描述：对双字节型的数据进行高低字节调换
   入口参数：原始双字节数   			 
   出口参数：高低字节调换过的数据
   全局变量: 
   创建时间：
   修改时间：
	注： ARM系统的存储是按高高低低的原则，即高字节存储在高地址，低字节存储在低地址
   	  51系统的存储是按高低低高的原则，即高字节存储在低地址，低字节存储在高地址
*/
extern		"C"		unsigned short 	WordSwap(unsigned short input);


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
extern		"C"		unsigned long 		SwapINT32(unsigned long dData);


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
extern		"C"		uint16     	SwapINT16(uint16 dData);


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：ByteToINT16
   功能描述：将2字节合并为一个双字节
   入口参数：原始2个单字节数，高字节在前，低字节在后   			 
   出口参数：合并后的双字节
   全局变量: 
   创建时间：
   修改时间：
*/
extern		"C"		uint16		ByteToINT16(uchar D0,uchar D1);

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：ByteToINT32
   功能描述：将4个单字节合并为长型变量
   入口参数：原始4个单字节数，高字节在前，低字节在后
   出口参数：合并后的长型变量，存储符合高高低低原则
   全局变量: 
   创建时间：
   修改时间：
*/
extern		"C"		uint32		ByteToINT32(uchar D0,uchar D1,uchar D2,uchar D3);

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
extern		"C"		void       	FloatDecCharToInt(uchar *pBuffer,uchar *pInt);


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：strstr
   功能描述：在一个字符串中查找另一个字符串
   入口参数：源字符串，目的字符串   			 
   出口参数：目的字符串在源字符串中的位置，没找到则为空指针
   全局变量: 
   创建时间：
   修改时间：
*/
extern		"C"		char		*MyStrStr(char *pSrc,char *pDst);

extern		"C"		char		*MyStrChar(char *pSrc,char cDst)	;	
	
extern		"C"		uchar		MyStrCmp(char *pSrc,char *pDst);

extern		"C"		uchar		MyStrCmpNB(uint16	Len,char *pSrc,char *pDst);

extern		"C"		uchar		*SearchArray(uint16 LenS,uchar *pSrc,uint16 LenD,uchar *pDst);

extern		"C"		uint16		SearchArrayCount(uint16 LenS,uchar *pSrc,uint16 LenD,uchar *pDst);

extern		"C"		uint32		My_atol(char *p);
	
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
extern		"C"		uchar			DecCharToPressBCD(uchar Len,char *pDec,uchar *pBCD);
		
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：DecCharToUint32
   功能描述：把十进制字符串(可含小数点)变成32位的整型
   入口参数：以非数字结束(小数点除外)的十进制字符串
   出口参数：32位的整型数
   全局变量: 
   创建时间：
   修改时间：
	注：以非数字且非小数点字符结束，含小数和结束符，最多12字节
	    如果没有小数点，可直接调用C语言标准库函数r = atol(*s)
*/ 
extern		"C"		uint32			DecCharToUint32(uchar Len,char *pDec);

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：IgnoreSpecifyCharacter
   功能描述：忽略指定的字符
   入口参数：字符串指针
   出口参数：字符长度
   全局变量: 
   创建时间：
   修改时间：
	注：此函数比较耗机时，如果开个缓冲就很节省时间
*/
extern		"C"		uint16	IgnoreSpecifyCharacter(uchar cChar,uint16 Len,uchar *pBuf);

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
extern		"C"		void			FourByte6BitTo3Byte8Bit(uchar *pSrc,uchar *pDst);

extern		"C"		uint32		HexCharToInt(char *pBuffer);

extern		"C"		uchar			HexCharToByte(char *pBuffer);

extern		"C"		uchar			HalfByteHexCharToInt(uchar Hex);



extern		"C"		uint16		EscapeCharEncode(uint16 Len,uchar *pBuf);

extern		"C"		uint16		EscapeCharDecode(uint16 Len,uchar *pBuf);
extern		"C"		uchar			*MyMalloc(uint16  Len);
extern		"C"		void			MyFree(uchar *pFree);

extern		"C"	   char		   MyStrLen(char *pSrc,char cDst);

extern		"C"	   uchar   CprDataValue(uchar Len, uchar L ,uchar MinData, uchar MaxDara, uchar *p);

extern		"C"	  uint32         BcdToUINT32Hex(uchar *BufferBCD);

extern		"C"		void  Encrypt  ( ENCRY_KEY  *pKey, unsigned char *Buffer, unsigned short size ) ;

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
