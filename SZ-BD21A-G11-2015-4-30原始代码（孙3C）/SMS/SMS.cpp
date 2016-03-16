/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												广州国业电子科技有限公司
$										Shawyoo Electronics (GuangZhou) R&D Center
$                       	http://www.shawyoo.com		 Email: autoele@qq.com
$								 QQ群：9069471/9069433/9524253		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 	 SMS.cpp
	功能描述：SMS相关函数
	编译环境：
	目标CPU: 
	作者：
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  


#define		SMS_GLOBALS			1
#include 	"Sms.h"
#include		<string.h>
#include    "..\GSM_GPRS\ATCmd.h"
#include		"..\UART\Uart1.h"
#include		"..\Common\Algol.h"
#include		"..\GSM_GPRS\GSM_GPRS.h"




uchar    	SMS_cBuffer[512];//		__attribute__((at(0x7fe02800)));		// 短信缓冲区 512B

uchar    	SMS_cPduBuffer[512];//	__attribute__((at(0x7fe02a00)));		// 短信缓冲区 512B



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述: 可打印字符串转换为字节数据
   			 如："C8329BFD0E01" --> {0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01}
   入口参数：pSrc - 源字符串指针,nSrcLength - 源字符串长度,pDst - 目标数据指针
   出口参数：目标数据长度
   全局变量: 
   创建时间：
   修改时间：	
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
unsigned int    SMSCls::String2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
	for (int i = 0; i < nSrcLength; i += 2)
	{
		// 输出高4位
		if ((*pSrc >= '0') && (*pSrc <= '9'))
		{
			*pDst = (*pSrc - '0') << 4;
		}
		else
		{
			*pDst = (*pSrc - 'A' + 10) << 4;
		}

		pSrc++;

		// 输出低4位
		if ((*pSrc>='0') && (*pSrc<='9'))
		{
			*pDst |= *pSrc - '0';
		}
		else
		{
			*pDst |= *pSrc - 'A' + 10;
		}

		pSrc++;
		pDst++;
	}

	// 返回目标数据长度
	return (nSrcLength / 2);
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述: 字节数据转换为可打印字符串
   			 如：{0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01} --> "C8329BFD0E01" 
   入口参数：pSrc - 源数据指针,nSrcLength - 源数据长度,pDst - 目标字符串指针
   出口参数：目标字符串长度
   全局变量: 
   创建时间：
   修改时间：	
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
unsigned int    SMSCls::Bytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
	const char tab[]="0123456789ABCDEF";	// 0x0-0xf的字符查找表

	for (int i = 0; i < nSrcLength; i++)
	{
		*pDst++ = tab[*pSrc >> 4];		// 输出高4位
		*pDst++ = tab[*pSrc & 0x0f];	// 输出低4位
		pSrc++;
	}

	// 输出字符串加个结束符
	*pDst = '\0';

	// 返回目标字符串长度
	return (nSrcLength * 2);
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述: 7bit编码
   入口参数：pSrc - 源字符串指针,nSrcLength - 源字符串长度,pDst - 目标编码串指针
   出口参数：目标编码串长度
   全局变量: 
   创建时间：
   修改时间：	
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
unsigned int    SMSCls::Encode7bit(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
	int nSrc;		// 源字符串的计数值
	int nDst;		// 目标编码串的计数值
	int nChar;		// 当前正在处理的组内字符字节的序号，范围是0-7
	unsigned char nLeft=0;	// 上一字节残余的数据

	// 计数值初始化
	nSrc = 0;
	nDst = 0;

	// 将源串每8个字节分为一组，压缩成7个字节
	// 循环该处理过程，直至源串被处理完
	// 如果分组不到8字节，也能正确处理
	while (nSrc < nSrcLength)
	{
		// 取源字符串的计数值的最低3位
		nChar = nSrc & 7;

		// 处理源串的每个字节
		if(nChar == 0)
		{
			// 组内第一个字节，只是保存起来，待处理下一个字节时使用
			nLeft = *pSrc;
		}
		else
		{
			// 组内其它字节，将其右边部分与残余数据相加，得到一个目标编码字节
			*pDst = (*pSrc << (8-nChar)) | nLeft;

			// 将该字节剩下的左边部分，作为残余数据保存起来
			nLeft = *pSrc >> nChar;

			// 修改目标串的指针和计数值
			pDst++;
			nDst++;
		}

		// 修改源串的指针和计数值
		pSrc++;
		nSrc++;
	}

	// 返回目标串长度
	return nDst;
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述: 7bit解码
   入口参数：pSrc - 源编码串指针,nSrcLength - 源编码串长度,pDst - 目标字符串指针
   出口参数：目标字符串长度
   全局变量: 
   创建时间：
   修改时间：	
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
unsigned int    SMSCls::Decode7bit(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
	int nSrc;		// 源字符串的计数值
	int nDst;		// 目标解码串的计数值
	int nByte;		// 当前正在处理的组内字节的序号，范围是0-6
	unsigned char nLeft;	// 上一字节残余的数据

	// 计数值初始化
	nSrc = 0;
	nDst = 0;
	
	// 组内字节序号和残余数据初始化
	nByte = 0;
	nLeft = 0;

	// 将源数据每7个字节分为一组，解压缩成8个字节
	// 循环该处理过程，直至源数据被处理完
	// 如果分组不到7字节，也能正确处理
	while(nSrc<nSrcLength)
	{
		// 将源字节右边部分与残余数据相加，去掉最高位，得到一个目标解码字节
		*pDst = ((*pSrc << nByte) | nLeft) & 0x7f;

		// 将该字节剩下的左边部分，作为残余数据保存起来
		nLeft = *pSrc >> (7-nByte);

		// 修改目标串的指针和计数值
		pDst++;
		nDst++;

		// 修改字节计数值
		nByte++;

		// 到了一组的最后一个字节
		if(nByte == 7)
		{
			// 额外得到一个目标解码字节
			*pDst = nLeft;

			// 修改目标串的指针和计数值
			pDst++;
			nDst++;

			// 组内字节序号和残余数据初始化
			nByte = 0;
			nLeft = 0;
		}

		// 修改源串的指针和计数值
		pSrc++;
		nSrc++;
	}

	// 输出字符串加个结束符
	*pDst = '\0';

	// 返回目标串长度
	return nDst;
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述: 8bit编码
   入口参数：pSrc - 源字符串指针,nSrcLength - 源字符串长度,pDst - 目标编码串指针
   出口参数：目标编码串长度
   全局变量: 
   创建时间：
   修改时间：	
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
unsigned int    SMSCls::Encode8bit(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
	// 简单复制
	memmove(pDst, pSrc, nSrcLength);

	return nSrcLength;
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述: 8bit解码
   入口参数：pSrc - 源编码串指针,nSrcLength -  源编码串长度,pDst -  目标字符串指针
   出口参数：目标字符串长度
   全局变量: 
   创建时间：
   修改时间：	
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
unsigned int    SMSCls::Decode8bit(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
	// 简单复制
	memmove(pDst, pSrc, nSrcLength);

	// 输出字符串加个结束符
	*(pDst+nSrcLength) = '\0';

	return nSrcLength;
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述: UCS2编码
   入口参数：pSrc - 源字符串指针,nSrcLength - 源字符串长度,pDst - 目标编码串指针
   出口参数：目标编码串长度
   全局变量: 
   创建时间：
   修改时间：	
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
unsigned int    SMSCls::EncodeUcs2(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
	
	//int  nDstLength=0;		// UNICODE宽字符数目

	if(nSrcLength>LEN_SMS_LIMIT_BYTES)	return 0;

   memmove(pDst, pSrc, nSrcLength);
	/*
	WCHAR wchar[128];	// UNICODE串缓冲区

	// 字符串-->UNICODE串
	nDstLength = MultiByteToWideChar(CP_ACP, 0, pSrc, nSrcLength, wchar, 128);

	// 高低字节对调，输出
	for(int i=0; i<nDstLength; i++)
	{
		*pDst++ = wchar[i] >> 8;		// 先输出高位字节
		*pDst++ = wchar[i] & 0xff;		// 后输出低位字节
	}

	// 返回目标编码串长度			*/
	//return nDstLength * 2;
	return  nSrcLength;
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述: UCS2解码
   入口参数：pSrc - 源编码串指针,nSrcLength -  源编码串长度,pDst -  目标字符串指针
   出口参数：目标字符串长度
   全局变量: 
   创建时间：
   修改时间：	
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
unsigned int    SMSCls::DecodeUcs2(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
	
//	int nDstLength=0;		// UNICODE宽字符数目
	memmove(pDst, pSrc, nSrcLength);

 /*	if(nSrcLength>LEN_SMS_LIMIT_BYTES)	return 0;
	
	WCHAR wchar[128];	// UNICODE串缓冲区

	// 高低字节对调，拼成UNICODE
	for(int i=0; i<nSrcLength/2; i++)
	{
		wchar[i] = *pSrc++ << 8;	// 先高位字节
		wchar[i] |= *pSrc++;			// 后低位字节
	}

	// UNICODE串-->字符串
	nDstLength = WideCharToMultiByte(CP_ACP, 0, wchar, nSrcLength/2, pDst, 160, NULL, NULL);

	// 输出字符串加个结束符
	pDst[nDstLength] = '\0';

	// 返回目标字符串长度			*/
	//return nDstLength;
	return  nSrcLength;
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
	功能描述: 正常顺序的字符串转换为两两颠倒的字符串，若长度为奇数，补'F'凑成偶数
				 如："8613851872468" --> "683158812764F8"   
   入口参数：pSrc - 源字符串指针,nSrcLength - 源字符串长度,pDst - 目标字符串指针
   出口参数：目标字符串长度
   全局变量: 
   创建时间：
   修改时间：	
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
unsigned int    SMSCls::InvertNumbers(const char* pSrc, char* pDst, int nSrcLength)
{
	int nDstLength=0;		// 目标字符串长度
	char ch;					// 用于保存一个字符

	// 复制串长度
	nDstLength = nSrcLength;

	// 两两颠倒
	for(int i=0; i<nSrcLength;i+=2)
	{
		ch = *pSrc++;			// 保存先出现的字符
		*pDst++ = *pSrc++;	// 复制后出现的字符
		*pDst++ = ch;			// 复制先出现的字符
	}

	// 源串长度是奇数吗？
	if(nSrcLength & 1)
	{
		*(pDst-2) = 'F';		// 补'F'
		nDstLength++;			// 目标串长度加1
	}

	// 输出字符串加个结束符
	*pDst = '\0';

	// 返回目标字符串长度
	return nDstLength;
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述: 两两颠倒的字符串转换为正常顺序的字符串 如："683158812764F8" --> "8613851872468"
   入口参数：pSrc - 源字符串指针,nSrcLength - 源字符串长度,pDst - 目标字符串指针
   出口参数：目标字符串长度
   全局变量: 
   创建时间：
   修改时间：	
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
unsigned int    SMSCls::SerializeNumbers(const char* pSrc, char* pDst, int nSrcLength)
{
	int nDstLength=0;		// 目标字符串长度
	char ch;			// 用于保存一个字符

	// 复制串长度
	nDstLength = nSrcLength;

	// 两两颠倒
	for(int i=0; i<nSrcLength;i+=2)
	{
		ch = *pSrc++;		// 保存先出现的字符
		*pDst++ = *pSrc++;	// 复制后出现的字符
		*pDst++ = ch;		// 复制先出现的字符
	}

	// 最后的字符是'F'吗？
	if(*(pDst-1) == 'F')
	{
		pDst--;
		nDstLength--;		// 目标字符串长度减1
	}

	// 输出字符串加个结束符
	*pDst = '\0';

	// 返回目标字符串长度
	return nDstLength;
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述: PDU编码，用于编制、发送短消息
   入口参数：pSrc - 源PDU参数指针,pDst - 目标PDU串指针
   出口参数：目标PDU串长度
   全局变量: 
   创建时间：
   修改时间：	
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
unsigned int    SMSCls::EncodePdu(const SM_PARAM* pSrc, char* pDst,unsigned short DataLen)
{
	int nLength=0;			// 内部用的串长度
	int nDstLength=0;			// 目标PDU串长度	

	if(DataLen>LEN_SMS_LIMIT_BYTES)	return 0;

	 ///nLength = strlen(pSrc->SCA);    // SMSC地址字符串的长度   
    //SMS_cBuffer[0] = (char)((nLength & 1) == 0 ? nLength : nLength + 1) / 2 + 1;    // SMSC地址信息长度
    //SMS_cBuffer[1] = 0x91;        // 固定: 用国际格式号码
    //nDstLength = Bytes2String(SMS_cBuffer, pDst, 2);        // 转换2个字节到目标PDU串
    //nDstLength += InvertNumbers(pSrc->SCA, &pDst[nDstLength], nLength);    // 转换SMSC到目标PDU串
	 pDst[0] = '0';
	 pDst[1] = '0';
	 nDstLength = 2;

   
    // TPDU段基本参数、目标地址等
    nLength = strlen(pSrc->TPA);    // TP-DA地址字符串的长度
    SMS_cBuffer[0] = 0x11;            // 是发送短信(TP-MTI=01)，TP-VP用相对格式(TP-VPF=10)
    SMS_cBuffer[1] = 0;               // TP-MR=0
    SMS_cBuffer[2] = (char)nLength;   // 目标地址数字个数(TP-DA地址字符串真实长度)
    SMS_cBuffer[3] = 0x91;            // 固定: 用国际格式号码
    nDstLength += Bytes2String(SMS_cBuffer, &pDst[nDstLength], 4); // 转换4个字节到目标PDU串
    nDstLength += InvertNumbers(pSrc->TPA, &pDst[nDstLength], nLength); // 转换TP-DA到目标PDU串
   
    // TPDU段协议标识、编码方式、用户信息等
    nLength = DataLen;	//strlen(pSrc->TP_UD);    // 用户信息字符串的长度
    SMS_cBuffer[0] = pSrc->TP_PID;        // 协议标识(TP-PID)
    SMS_cBuffer[1] = pSrc->TP_DCS;        // 用户信息编码方式(TP-DCS)
    SMS_cBuffer[2] = 0;            // 有效期(TP-VP)为5分钟
	if(pSrc->TP_DCS == SMS_7BIT)	
	{
		// 7-bit编码方式
		SMS_cBuffer[3] = nLength;			// 编码前长度
		nLength = Encode7bit(pSrc->TP_UD, &SMS_cBuffer[4], nLength+1) + 4;	// 转换TP-DA到目标PDU串
	}
	else if(pSrc->TP_DCS == SMS_UCS2)
	{
		// UCS2编码方式
		SMS_cBuffer[3] = EncodeUcs2(pSrc->TP_UD, &SMS_cBuffer[4], nLength);	// 转换TP-DA到目标PDU串
		nLength = SMS_cBuffer[3] + 4;		// nLength等于该段数据长度
	}
	else
	{
		// 8-bit编码方式
		SMS_cBuffer[3] = Encode8bit(pSrc->TP_UD, &SMS_cBuffer[4], nLength);	// 转换TP-DA到目标PDU串
		nLength = SMS_cBuffer[3] + 4;		// nLength等于该段数据长度
	}
	nDstLength += Bytes2String(SMS_cBuffer, &pDst[nDstLength], nLength);		// 转换该段数据到目标PDU串

	
	// 返回目标字符串长度
	return nDstLength;
}




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述: PDU解码，用于接收、阅读短消息
   入口参数：pSrc - 源PDU串指针,pDst - 目标PDU参数指针
   出口参数：用户信息串长度
   全局变量: 
   创建时间：
   修改时间：	
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
unsigned int    SMSCls::DecodePdu(const char* pSrc, SM_PARAM* pDst)
{
	int nDstLength=0;			// 目标PDU串长度
	unsigned char tmp;		// 内部用的临时字节变量
	
	if(strlen(pSrc)>LEN_SMS_LIMIT_BYTES)	return 0;
	// SMSC地址信息段
	String2Bytes(pSrc, &tmp, 2);	// 取长度
	tmp = (tmp - 1) * 2;	// SMSC号码串长度
	pSrc += 4;			// 指针后移，忽略了SMSC地址格式
	SerializeNumbers(pSrc, pDst->SCA, tmp);	// 转换SMSC号码到目标PDU串
	pSrc += tmp;		// 指针后移
       
	// TPDU段基本参数
	String2Bytes(pSrc, &tmp, 2);	// 取基本参数
	pSrc += 2;		// 指针后移
	pDst->index = tmp;			  //bit6 置位则pDst->TP_DCS有分包协议头
	//pDst->index <<= 8;			  //
	// 取回复号码
	String2Bytes(pSrc, &tmp, 2);	// 取长度
	if(tmp & 1) tmp += 1;	// 调整奇偶性
	pSrc += 4;			// 指针后移，忽略了回复地址(TP-RA)格式
	SerializeNumbers(pSrc, pDst->TPA, tmp);	// 取TP-RA号码
	pSrc += tmp;		// 指针后移

	// TPDU段协议标识、编码方式、用户信息等
	String2Bytes(pSrc, (unsigned char*)&pDst->TP_PID, 2);	// 取协议标识(TP-PID)
	pSrc += 2;		// 指针后移
	String2Bytes(pSrc, (unsigned char*)&pDst->TP_DCS, 2);	// 取编码方式(TP-DCS)
	pSrc += 2;		// 指针后移
	SerializeNumbers(pSrc, pDst->TP_SCTS, 14);		// 服务时间戳字符串(TP_SCTS) 
	pSrc += 14;		// 指针后移
	String2Bytes(pSrc, &tmp, 2);	// 用户信息长度(TP-UDL)
	pSrc += 2;		// 指针后移
	
	////////////////////////////////////////////////// 三种编码方式的解析
	if(pDst->TP_DCS == SMS_7BIT)	
	{
		// 7-bit解码
		nDstLength = String2Bytes(pSrc, SMS_cBuffer, tmp & 7 ? (int)tmp * 7 / 4 + 2 : (int)tmp * 7 / 4);	// 格式转换
		Decode7bit(SMS_cBuffer, pDst->TP_UD, nDstLength);	// 转换到TP-DU
		nDstLength = tmp;
}
	else if(pDst->TP_DCS == SMS_UCS2)
	{
		// UCS2解码
		nDstLength = String2Bytes(pSrc, SMS_cBuffer, tmp * 2);			// 格式转换
		nDstLength = DecodeUcs2(SMS_cBuffer, pDst->TP_UD, nDstLength);	// 转换到TP-DU
	}
	else
	{
		// 8-bit解码
		nDstLength = String2Bytes(pSrc, SMS_cBuffer, tmp * 2);			// 格式转换
		nDstLength = Decode8bit(&SMS_cBuffer[tmp], pDst->TP_UD, nDstLength);	// 转换到TP-DU
	}
	tmp = 0;
	if((pDst->index>>6)&0x0001)	//是否有分包
	{
	   tmp = pDst->TP_UD[0];
		tmp++;
		nDstLength -= tmp;
		memmove(SMS_cBuffer, &pDst->TP_UD[tmp], nDstLength);
		memmove(pDst->TP_UD, SMS_cBuffer, nDstLength);
	}

	// 返回目标字符串长度
	return nDstLength;
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述: // 初始化GSM状态
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间：	
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
unsigned int		SMSCls::Init(void)
{

	return TRUE;
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述: 发送短消息，仅发送命令，不读取应答
   入口参数：pSrc - 源PDU参数指针（未编码）
   出口参数：
   全局变量: 
   创建时间：
   修改时间：	
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
unsigned int    SMSCls::SendPduMessage(SM_PARAM* pSrc,unsigned short DataLen)
{
	//SM_PARAM		Sms;
	unsigned short		Len=0;
	int nPduLength=0;						// PDU串长度
	//unsigned char nSmscLength=0;		// SMSC串长度
	//int nLength;							// 串口收到的数据长度
	char cmd[16];							// 命令串	
	char ans[16];							// 应答串
	uchar	s=0;
	char  *pS;


	//s = GSM.AT_CommandWaitRetStatus(eAT_CMGF0,NULL,(uchar *)ans);			 //
	//if(s!=OK) return ERROR;


	if(DataLen>LEN_SMS_LIMIT_BYTES)	return ERROR;
	SetBufferValue(SMS_cPduBuffer, 512, 0);
	
	nPduLength = EncodePdu(pSrc, (char*)SMS_cPduBuffer,DataLen);		// 根据PDU参数，编码PDU串
	strcat((char*)SMS_cPduBuffer, "\x01a");							// 以Ctrl-Z结束
	//SMS_cPduBuffer[nPduLength] = 0x1A;
	//SMS_cPduBuffer[nPduLength+1] = 0x1A; 
	//SMS_cPduBuffer[nPduLength+2] = 0x1A;
	
   //Uart.putb(1, nPduLength+1, SMS_cPduBuffer);	  os_dly_wait(10);

	//Len = SMS.DecodePdu((char*)SMS_cPduBuffer,(SM_PARAM*)&Sms);
	

	//String2Bytes((char*)SMS_cPduBuffer, &nSmscLength, 2);		// 取PDU串中的SMSC信息长度
	//nSmscLength++;										// 加上长度字节本身	
	
	Len = nPduLength / 2 - 1;// nSmscLength;
	SetBufferValue((uchar*)ans,16,0);

	strcpy(cmd,"AT+CMGS=\0");
	Int16ToDecChar(Len,ans);
	strcat(cmd,ans);
	strcat(cmd,"\r\0");
	//Uart.puts(cmd);  os_dly_wait(10);
	Uart1.ClearReceiveFlag(0);
	Uart1.puts(cmd);		 
	SetBufferValue((uchar*)ans,16,0);
	s = Uart1.WaitRecBuffer(1000,16,(uchar*)ans);
	if(s)
	{
    	for(s=0;s<16;s++)
		{
			if(ans[s] == '>')	
			{	
				s = 0x55;
				break;
			}
		}
	   //s = GSM.SendCommandWaitRetStatus(cmd,"\r\n>");
		if(s==0x55)
		{
			SetBufferValue((uchar*)ans,16,0);
			Uart1.ClearReceiveFlag(0);			
			//Uart1.putb(1, nPduLength+3, (uchar*)SMS_cPduBuffer); 
			Uart1.putb(1, strlen((char *)SMS_cPduBuffer), (uchar*)SMS_cPduBuffer); 
			Len = Uart1.WaitRecBuffer(1000,16,(uchar*)ans);	 // +USOWR: 0,16\x0D \x0A 
			if(Len)
			{	
				//Uart.puts((char *)ans);	 os_dly_wait(5);
				pS = MyStrStr((char *)ans,(char*)"+CMGS\0");
				if( pS != NULL)
				{
				    //Uart.puts("SMS sending");	 os_dly_wait(5);
					//pS = MyStrStr((char *)ans,(char*)"OK\0");					 //PDU模式没有找到 OK返回
					//if( pS != NULL)
					{
						SystemLog.GSM.SendSms++;
						Uart.puts("SMS send OK");	 os_dly_wait(5);
						//Uart.putb(1, nPduLength+1, SMS_cPduBuffer);	 os_dly_wait(10);
						return OK;
					}
				}
			}
			//Uart.putb(1, nPduLength+1, SMS_cPduBuffer);	 os_dly_wait(10);
			//Uart.putb(1, strlen((char *)SMS_cPduBuffer), SMS_cPduBuffer);	 os_dly_wait(10);

		}
	}
   return ERROR;
}




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：SendPduDataMessage
   功能描述: 发送短消息，仅发送命令，不读取应答
   入口参数：pSrc - 源PDU参数指针（）
   出口参数：
   全局变量: 
   创建时间：
   修改时间：	
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
unsigned char    SMSCls::SetPduDataMessage(uchar CodeType, char *number,uchar *msg, uchar MsgLen)
{
	//strcpy((char *)PDU_SMS.SCA, (char *)sTelNum.SmsCenter); 
	SetBufferValue((uchar *)PDU_SMS.SCA, 16, 0);  // 根据GSM模块要求，pdu 模式，短信中心号码留空
	PDU_SMS.TP_PID = 0x00;
	strcpy(PDU_SMS.TPA, number);
	PDU_SMS.TP_DCS = CodeType;
	memmove(PDU_SMS.TP_UD, msg, MsgLen);
	PDU_SMS.TP_UD_Len = MsgLen;
	PDU_SMS.PDU_SendFlag = TRUE; 
	PDU_SMS.SendTimes = 3; 
	//Uart.putb(1, 10, (uchar *)PDU_SMS.TP_UD);	  os_dly_wait(10);
	return 0;
}






/***************************************************************************************
**	函数名称：SendTextMessage()
** 功能描述: 发送TEXT格式信息
** 入口参数：电话号码，指向要发送内容的指针
** 出口参数：操作状态
** 全局变量: 无,但要设置信息中心号码
** 创建时间：2009.11.11
** 修改时间：	
***************************************************************************************/ 
unsigned char    SMSCls::SendTextMessage(char *number,char *msg)
{
	return 0;
}



unsigned char    SMSCls::SendMessage(char *number,char *msg)
{
	
	return 0;
}





uchar    SMSCls::SetTextBuffer(uchar CodeType,char *number,char *msg)
{
	if(!TextSMS.Length)
	{
   	SetBufferValue((uchar*)TextSMS.dNumber,LEN_SMS_NUMBER,0);
		SetBufferValue((uchar*)TextSMS.sNumber,LEN_SMS_NUMBER,0);
		SetBufferValue((uchar*)TextSMS.Buffer,LEN_SMS_BUFFER,0);	
      
		if(number!=NULL)	strcpy(TextSMS.dNumber,number);
		if(msg!=NULL)
		{
			strcpy(TextSMS.Buffer,msg);		
			TextSMS.Length = strlen(TextSMS.Buffer);
			TextSMS.CodeType = CodeType;
		}
		return OK;
	}
	else if(!TextSMS1.Length)
	{
   	SetBufferValue((uchar*)TextSMS1.dNumber,LEN_SMS_NUMBER,0);
		SetBufferValue((uchar*)TextSMS1.sNumber,LEN_SMS_NUMBER,0);
		SetBufferValue((uchar*)TextSMS1.Buffer,LEN_SMS_BUFFER,0);	
   
		if(number!=NULL)	strcpy(TextSMS1.dNumber,number);
		if(msg!=NULL)
		{
			strcpy(TextSMS1.Buffer,msg);		
			TextSMS1.Length = strlen(TextSMS1.Buffer);
			TextSMS1.CodeType = CodeType;
		}
		return OK;
	}
	return ERROR;
}




/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
 
