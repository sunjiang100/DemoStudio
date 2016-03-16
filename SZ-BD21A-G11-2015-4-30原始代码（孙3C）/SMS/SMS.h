/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												广州国业电子科技有限公司
$										Shawyoo Electronics (GuangZhou) R&D Center
$                       	http://www.shawyoo.com		 Email: autoele@qq.com
$								 QQ群：9069471/9069433/9524253		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 	 SMS.h
	功能描述：
	编译环境：
	目标CPU: 
	作者：
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  




#if !defined(__SMS_H__)
#define __SMS_H__



#include		"..\Config\RTOS.h"




#ifdef      SMS_GLOBALS
#define     SMS_EXT
#else
#define     SMS_EXT    extern
#endif




typedef		unsigned short		WCHAR;







// 用户信息编码方式
#define 		SMS_7BIT			0
#define 		SMS_8BIT			4
#define 		SMS_UCS2			8

// 应答状态
#define 		SMS_WAIT			0		// 等待，不确定
#define 		SMS_OK			1		// OK
#define 		SMS_ERR			-1		// ERROR

#define		LEN_SMS_NUMBER_EXT		24				 // 增强短信号码长度


#define		LEN_SMS_LIMIT_BYTES			448 //456		// 短信限长


/*
+CMT: ,39 \x0D 17:51:22★ \x0A
0891683108200005F0040D91683129220885F600009040327115712315683A1DAE7BBDEEF7BBEB5C7FE7CB65D7F8DD06 \x0D 17:51:22★ \x0A	
*/
// 短消息参数结构，编码/解码共用
// 其中，字符串以'\0'结尾
//#define		LEN_SMS_ALL_PARAM			(16+16+2+16+456+2+4)	//	 512B
#define		LEN_SMS_ALL_PARAM			(16+24+2+16+448+2+4)	//	 512B
typedef struct 
{
	char SCA[16];			// 短消息服务中心号码(SMSC地址)
	//char TPA[16];			// 目标号码或回复号码(TP-DA或TP-RA)
	char TPA[LEN_SMS_NUMBER_EXT];			// 目标号码或回复号码(TP-DA或TP-RA)  飞信的号码有20位
	char TP_PID;			// 用户信息协议标识(TP-PID)
	uchar TP_DCS;			// 用户信息编码方式(TP-DCS)  00 04 08
	char TP_SCTS[16];		// 服务时间戳字符串(TP_SCTS), 接收时用到
	char TP_UD[LEN_SMS_LIMIT_BYTES];		// 原始用户信息(编码前或解码后的TP-UD)
	short index;			// 短消息序号，在读取时用到
	uchar TP_UD_Len;				//
	uchar PDU_SendFlag;
	uchar Ack;
	uchar SendTimes;        
} SM_PARAM,*pSM_PARAM;

SMS_EXT  SM_PARAM  PDU_SMS;

//#define		LEN_SMS_NUMBER			32
#define		LEN_SMS_BUFFER			160
SMS_EXT		char		SMS_SendNumber[LEN_SMS_NUMBER_EXT];
SMS_EXT		struct
{
	uint16	Length;								// 发送短信内容长度
	uchar		SendFlag;
	uchar    SendTimes; 
	uchar		Ack;							// 	
	uchar		CodeType;					//   0:已经编码的ＰＤＵ，１：文本
	//char		sNumber[LEN_SMS_NUMBER];		// 源号码
	char		sNumber[LEN_SMS_NUMBER_EXT];		// 源号码
	//char		dNumber[LEN_SMS_NUMBER];		// 目的号码
	char		dNumber[LEN_SMS_NUMBER_EXT];		// 目的号码

	char		Buffer[LEN_SMS_BUFFER];	// 
}TextSMS,TextSMS1;

//SMS_EXT		struct
//{
//	uint16	Length;								// 发送短信内容长度
//	uchar		SendFlag;
//	uchar		Ack;							// 	
//	uchar		CodeType;					//   0:已经编码的ＰＤＵ，１：文本
//}PDUSMS,PDUSMS1;


class			SMSCls
{
public:
	unsigned int 	EncodePdu(const SM_PARAM* pSrc, char* pDst,unsigned short DataLen);
	unsigned int 	DecodePdu(const char* pSrc, SM_PARAM* pDst);
	unsigned int 	SendPduMessage(SM_PARAM* pSrc,unsigned short DataLen);
	uchar    		SetTextBuffer(uchar CodeType,char *number,char *msg);
	unsigned char 	SendTextMessage(char *number,char *msg);
	unsigned char 	SetPduDataMessage(uchar CodeType, char *number,uchar *msg, uchar MsgLen);
	unsigned char 	SendMessage(char *number,char *msg);
	unsigned int	Init();
	unsigned int 	Bytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength);
	unsigned int 	String2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength);
private:
	//unsigned int 	Bytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength);
	//unsigned int 	String2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength);
	unsigned int 	Encode7bit(const char* pSrc, unsigned char* pDst, int nSrcLength);
	unsigned int 	Decode7bit(const unsigned char* pSrc, char* pDst, int nSrcLength);
	unsigned int 	Encode8bit(const char* pSrc, unsigned char* pDst, int nSrcLength);
	unsigned int 	Decode8bit(const unsigned char* pSrc, char* pDst, int nSrcLength);
	unsigned int 	EncodeUcs2(const char* pSrc, unsigned char* pDst, int nSrcLength);
	unsigned int 	DecodeUcs2(const unsigned char* pSrc, char* pDst, int nSrcLength);
	unsigned int 	InvertNumbers(const char* pSrc, char* pDst, int nSrcLength);
	unsigned int 	SerializeNumbers(const char* pSrc, char* pDst, int nSrcLength);	
	
		
};

SMS_EXT	__noinit__		SMSCls		SMS;	// 


#endif // !defined(__SMS_H__)
