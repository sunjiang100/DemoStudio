/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												���ݹ�ҵ���ӿƼ����޹�˾
$										Shawyoo Electronics (GuangZhou) R&D Center
$                       	http://www.shawyoo.com		 Email: autoele@qq.com
$								 QQȺ��9069471/9069433/9524253		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
	�ļ���: 	 SMS.h
	����������
	���뻷����
	Ŀ��CPU: 
	���ߣ�
	����ʱ�䣺
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
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







// �û���Ϣ���뷽ʽ
#define 		SMS_7BIT			0
#define 		SMS_8BIT			4
#define 		SMS_UCS2			8

// Ӧ��״̬
#define 		SMS_WAIT			0		// �ȴ�����ȷ��
#define 		SMS_OK			1		// OK
#define 		SMS_ERR			-1		// ERROR

#define		LEN_SMS_NUMBER_EXT		24				 // ��ǿ���ź��볤��


#define		LEN_SMS_LIMIT_BYTES			448 //456		// �����޳�


/*
+CMT: ,39 \x0D 17:51:22�� \x0A
0891683108200005F0040D91683129220885F600009040327115712315683A1DAE7BBDEEF7BBEB5C7FE7CB65D7F8DD06 \x0D 17:51:22�� \x0A	
*/
// ����Ϣ�����ṹ������/���빲��
// ���У��ַ�����'\0'��β
//#define		LEN_SMS_ALL_PARAM			(16+16+2+16+456+2+4)	//	 512B
#define		LEN_SMS_ALL_PARAM			(16+24+2+16+448+2+4)	//	 512B
typedef struct 
{
	char SCA[16];			// ����Ϣ�������ĺ���(SMSC��ַ)
	//char TPA[16];			// Ŀ������ظ�����(TP-DA��TP-RA)
	char TPA[LEN_SMS_NUMBER_EXT];			// Ŀ������ظ�����(TP-DA��TP-RA)  ���ŵĺ�����20λ
	char TP_PID;			// �û���ϢЭ���ʶ(TP-PID)
	uchar TP_DCS;			// �û���Ϣ���뷽ʽ(TP-DCS)  00 04 08
	char TP_SCTS[16];		// ����ʱ����ַ���(TP_SCTS), ����ʱ�õ�
	char TP_UD[LEN_SMS_LIMIT_BYTES];		// ԭʼ�û���Ϣ(����ǰ�������TP-UD)
	short index;			// ����Ϣ��ţ��ڶ�ȡʱ�õ�
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
	uint16	Length;								// ���Ͷ������ݳ���
	uchar		SendFlag;
	uchar    SendTimes; 
	uchar		Ack;							// 	
	uchar		CodeType;					//   0:�Ѿ�����ģУģգ������ı�
	//char		sNumber[LEN_SMS_NUMBER];		// Դ����
	char		sNumber[LEN_SMS_NUMBER_EXT];		// Դ����
	//char		dNumber[LEN_SMS_NUMBER];		// Ŀ�ĺ���
	char		dNumber[LEN_SMS_NUMBER_EXT];		// Ŀ�ĺ���

	char		Buffer[LEN_SMS_BUFFER];	// 
}TextSMS,TextSMS1;

//SMS_EXT		struct
//{
//	uint16	Length;								// ���Ͷ������ݳ���
//	uchar		SendFlag;
//	uchar		Ack;							// 	
//	uchar		CodeType;					//   0:�Ѿ�����ģУģգ������ı�
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
