/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												���ݹ�ҵ���ӿƼ����޹�˾
$										Shawyoo Electronics (GuangZhou) R&D Center
$                       	http://www.shawyoo.com		 Email: autoele@qq.com
$								 QQȺ��9069471/9069433/9524253		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
	�ļ���: 	 SMS.cpp
	����������SMS��غ���
	���뻷����
	Ŀ��CPU: 
	���ߣ�
	����ʱ�䣺
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  


#define		SMS_GLOBALS			1
#include 	"Sms.h"
#include		<string.h>
#include    "..\GSM_GPRS\ATCmd.h"
#include		"..\UART\Uart1.h"
#include		"..\Common\Algol.h"
#include		"..\GSM_GPRS\GSM_GPRS.h"




uchar    	SMS_cBuffer[512];//		__attribute__((at(0x7fe02800)));		// ���Ż����� 512B

uchar    	SMS_cPduBuffer[512];//	__attribute__((at(0x7fe02a00)));		// ���Ż����� 512B



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ��������: �ɴ�ӡ�ַ���ת��Ϊ�ֽ�����
   			 �磺"C8329BFD0E01" --> {0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01}
   ��ڲ�����pSrc - Դ�ַ���ָ��,nSrcLength - Դ�ַ�������,pDst - Ŀ������ָ��
   ���ڲ�����Ŀ�����ݳ���
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺	
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
unsigned int    SMSCls::String2Bytes(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
	for (int i = 0; i < nSrcLength; i += 2)
	{
		// �����4λ
		if ((*pSrc >= '0') && (*pSrc <= '9'))
		{
			*pDst = (*pSrc - '0') << 4;
		}
		else
		{
			*pDst = (*pSrc - 'A' + 10) << 4;
		}

		pSrc++;

		// �����4λ
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

	// ����Ŀ�����ݳ���
	return (nSrcLength / 2);
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ��������: �ֽ�����ת��Ϊ�ɴ�ӡ�ַ���
   			 �磺{0xC8, 0x32, 0x9B, 0xFD, 0x0E, 0x01} --> "C8329BFD0E01" 
   ��ڲ�����pSrc - Դ����ָ��,nSrcLength - Դ���ݳ���,pDst - Ŀ���ַ���ָ��
   ���ڲ�����Ŀ���ַ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺	
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
unsigned int    SMSCls::Bytes2String(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
	const char tab[]="0123456789ABCDEF";	// 0x0-0xf���ַ����ұ�

	for (int i = 0; i < nSrcLength; i++)
	{
		*pDst++ = tab[*pSrc >> 4];		// �����4λ
		*pDst++ = tab[*pSrc & 0x0f];	// �����4λ
		pSrc++;
	}

	// ����ַ����Ӹ�������
	*pDst = '\0';

	// ����Ŀ���ַ�������
	return (nSrcLength * 2);
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ��������: 7bit����
   ��ڲ�����pSrc - Դ�ַ���ָ��,nSrcLength - Դ�ַ�������,pDst - Ŀ����봮ָ��
   ���ڲ�����Ŀ����봮����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺	
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
unsigned int    SMSCls::Encode7bit(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
	int nSrc;		// Դ�ַ����ļ���ֵ
	int nDst;		// Ŀ����봮�ļ���ֵ
	int nChar;		// ��ǰ���ڴ���������ַ��ֽڵ���ţ���Χ��0-7
	unsigned char nLeft=0;	// ��һ�ֽڲ��������

	// ����ֵ��ʼ��
	nSrc = 0;
	nDst = 0;

	// ��Դ��ÿ8���ֽڷ�Ϊһ�飬ѹ����7���ֽ�
	// ѭ���ô�����̣�ֱ��Դ����������
	// ������鲻��8�ֽڣ�Ҳ����ȷ����
	while (nSrc < nSrcLength)
	{
		// ȡԴ�ַ����ļ���ֵ�����3λ
		nChar = nSrc & 7;

		// ����Դ����ÿ���ֽ�
		if(nChar == 0)
		{
			// ���ڵ�һ���ֽڣ�ֻ�Ǳ�����������������һ���ֽ�ʱʹ��
			nLeft = *pSrc;
		}
		else
		{
			// ���������ֽڣ������ұ߲��������������ӣ��õ�һ��Ŀ������ֽ�
			*pDst = (*pSrc << (8-nChar)) | nLeft;

			// �����ֽ�ʣ�µ���߲��֣���Ϊ�������ݱ�������
			nLeft = *pSrc >> nChar;

			// �޸�Ŀ�괮��ָ��ͼ���ֵ
			pDst++;
			nDst++;
		}

		// �޸�Դ����ָ��ͼ���ֵ
		pSrc++;
		nSrc++;
	}

	// ����Ŀ�괮����
	return nDst;
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ��������: 7bit����
   ��ڲ�����pSrc - Դ���봮ָ��,nSrcLength - Դ���봮����,pDst - Ŀ���ַ���ָ��
   ���ڲ�����Ŀ���ַ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺	
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
unsigned int    SMSCls::Decode7bit(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
	int nSrc;		// Դ�ַ����ļ���ֵ
	int nDst;		// Ŀ����봮�ļ���ֵ
	int nByte;		// ��ǰ���ڴ���������ֽڵ���ţ���Χ��0-6
	unsigned char nLeft;	// ��һ�ֽڲ��������

	// ����ֵ��ʼ��
	nSrc = 0;
	nDst = 0;
	
	// �����ֽ���źͲ������ݳ�ʼ��
	nByte = 0;
	nLeft = 0;

	// ��Դ����ÿ7���ֽڷ�Ϊһ�飬��ѹ����8���ֽ�
	// ѭ���ô�����̣�ֱ��Դ���ݱ�������
	// ������鲻��7�ֽڣ�Ҳ����ȷ����
	while(nSrc<nSrcLength)
	{
		// ��Դ�ֽ��ұ߲��������������ӣ�ȥ�����λ���õ�һ��Ŀ������ֽ�
		*pDst = ((*pSrc << nByte) | nLeft) & 0x7f;

		// �����ֽ�ʣ�µ���߲��֣���Ϊ�������ݱ�������
		nLeft = *pSrc >> (7-nByte);

		// �޸�Ŀ�괮��ָ��ͼ���ֵ
		pDst++;
		nDst++;

		// �޸��ֽڼ���ֵ
		nByte++;

		// ����һ������һ���ֽ�
		if(nByte == 7)
		{
			// ����õ�һ��Ŀ������ֽ�
			*pDst = nLeft;

			// �޸�Ŀ�괮��ָ��ͼ���ֵ
			pDst++;
			nDst++;

			// �����ֽ���źͲ������ݳ�ʼ��
			nByte = 0;
			nLeft = 0;
		}

		// �޸�Դ����ָ��ͼ���ֵ
		pSrc++;
		nSrc++;
	}

	// ����ַ����Ӹ�������
	*pDst = '\0';

	// ����Ŀ�괮����
	return nDst;
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ��������: 8bit����
   ��ڲ�����pSrc - Դ�ַ���ָ��,nSrcLength - Դ�ַ�������,pDst - Ŀ����봮ָ��
   ���ڲ�����Ŀ����봮����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺	
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
unsigned int    SMSCls::Encode8bit(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
	// �򵥸���
	memmove(pDst, pSrc, nSrcLength);

	return nSrcLength;
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ��������: 8bit����
   ��ڲ�����pSrc - Դ���봮ָ��,nSrcLength -  Դ���봮����,pDst -  Ŀ���ַ���ָ��
   ���ڲ�����Ŀ���ַ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺	
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
unsigned int    SMSCls::Decode8bit(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
	// �򵥸���
	memmove(pDst, pSrc, nSrcLength);

	// ����ַ����Ӹ�������
	*(pDst+nSrcLength) = '\0';

	return nSrcLength;
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ��������: UCS2����
   ��ڲ�����pSrc - Դ�ַ���ָ��,nSrcLength - Դ�ַ�������,pDst - Ŀ����봮ָ��
   ���ڲ�����Ŀ����봮����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺	
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
unsigned int    SMSCls::EncodeUcs2(const char* pSrc, unsigned char* pDst, int nSrcLength)
{
	
	//int  nDstLength=0;		// UNICODE���ַ���Ŀ

	if(nSrcLength>LEN_SMS_LIMIT_BYTES)	return 0;

   memmove(pDst, pSrc, nSrcLength);
	/*
	WCHAR wchar[128];	// UNICODE��������

	// �ַ���-->UNICODE��
	nDstLength = MultiByteToWideChar(CP_ACP, 0, pSrc, nSrcLength, wchar, 128);

	// �ߵ��ֽڶԵ������
	for(int i=0; i<nDstLength; i++)
	{
		*pDst++ = wchar[i] >> 8;		// �������λ�ֽ�
		*pDst++ = wchar[i] & 0xff;		// �������λ�ֽ�
	}

	// ����Ŀ����봮����			*/
	//return nDstLength * 2;
	return  nSrcLength;
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ��������: UCS2����
   ��ڲ�����pSrc - Դ���봮ָ��,nSrcLength -  Դ���봮����,pDst -  Ŀ���ַ���ָ��
   ���ڲ�����Ŀ���ַ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺	
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
unsigned int    SMSCls::DecodeUcs2(const unsigned char* pSrc, char* pDst, int nSrcLength)
{
	
//	int nDstLength=0;		// UNICODE���ַ���Ŀ
	memmove(pDst, pSrc, nSrcLength);

 /*	if(nSrcLength>LEN_SMS_LIMIT_BYTES)	return 0;
	
	WCHAR wchar[128];	// UNICODE��������

	// �ߵ��ֽڶԵ���ƴ��UNICODE
	for(int i=0; i<nSrcLength/2; i++)
	{
		wchar[i] = *pSrc++ << 8;	// �ȸ�λ�ֽ�
		wchar[i] |= *pSrc++;			// ���λ�ֽ�
	}

	// UNICODE��-->�ַ���
	nDstLength = WideCharToMultiByte(CP_ACP, 0, wchar, nSrcLength/2, pDst, 160, NULL, NULL);

	// ����ַ����Ӹ�������
	pDst[nDstLength] = '\0';

	// ����Ŀ���ַ�������			*/
	//return nDstLength;
	return  nSrcLength;
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
	��������: ����˳����ַ���ת��Ϊ�����ߵ����ַ�����������Ϊ��������'F'�ճ�ż��
				 �磺"8613851872468" --> "683158812764F8"   
   ��ڲ�����pSrc - Դ�ַ���ָ��,nSrcLength - Դ�ַ�������,pDst - Ŀ���ַ���ָ��
   ���ڲ�����Ŀ���ַ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺	
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
unsigned int    SMSCls::InvertNumbers(const char* pSrc, char* pDst, int nSrcLength)
{
	int nDstLength=0;		// Ŀ���ַ�������
	char ch;					// ���ڱ���һ���ַ�

	// ���ƴ�����
	nDstLength = nSrcLength;

	// �����ߵ�
	for(int i=0; i<nSrcLength;i+=2)
	{
		ch = *pSrc++;			// �����ȳ��ֵ��ַ�
		*pDst++ = *pSrc++;	// ���ƺ���ֵ��ַ�
		*pDst++ = ch;			// �����ȳ��ֵ��ַ�
	}

	// Դ��������������
	if(nSrcLength & 1)
	{
		*(pDst-2) = 'F';		// ��'F'
		nDstLength++;			// Ŀ�괮���ȼ�1
	}

	// ����ַ����Ӹ�������
	*pDst = '\0';

	// ����Ŀ���ַ�������
	return nDstLength;
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ��������: �����ߵ����ַ���ת��Ϊ����˳����ַ��� �磺"683158812764F8" --> "8613851872468"
   ��ڲ�����pSrc - Դ�ַ���ָ��,nSrcLength - Դ�ַ�������,pDst - Ŀ���ַ���ָ��
   ���ڲ�����Ŀ���ַ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺	
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
unsigned int    SMSCls::SerializeNumbers(const char* pSrc, char* pDst, int nSrcLength)
{
	int nDstLength=0;		// Ŀ���ַ�������
	char ch;			// ���ڱ���һ���ַ�

	// ���ƴ�����
	nDstLength = nSrcLength;

	// �����ߵ�
	for(int i=0; i<nSrcLength;i+=2)
	{
		ch = *pSrc++;		// �����ȳ��ֵ��ַ�
		*pDst++ = *pSrc++;	// ���ƺ���ֵ��ַ�
		*pDst++ = ch;		// �����ȳ��ֵ��ַ�
	}

	// �����ַ���'F'��
	if(*(pDst-1) == 'F')
	{
		pDst--;
		nDstLength--;		// Ŀ���ַ������ȼ�1
	}

	// ����ַ����Ӹ�������
	*pDst = '\0';

	// ����Ŀ���ַ�������
	return nDstLength;
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ��������: PDU���룬���ڱ��ơ����Ͷ���Ϣ
   ��ڲ�����pSrc - ԴPDU����ָ��,pDst - Ŀ��PDU��ָ��
   ���ڲ�����Ŀ��PDU������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺	
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
unsigned int    SMSCls::EncodePdu(const SM_PARAM* pSrc, char* pDst,unsigned short DataLen)
{
	int nLength=0;			// �ڲ��õĴ�����
	int nDstLength=0;			// Ŀ��PDU������	

	if(DataLen>LEN_SMS_LIMIT_BYTES)	return 0;

	 ///nLength = strlen(pSrc->SCA);    // SMSC��ַ�ַ����ĳ���   
    //SMS_cBuffer[0] = (char)((nLength & 1) == 0 ? nLength : nLength + 1) / 2 + 1;    // SMSC��ַ��Ϣ����
    //SMS_cBuffer[1] = 0x91;        // �̶�: �ù��ʸ�ʽ����
    //nDstLength = Bytes2String(SMS_cBuffer, pDst, 2);        // ת��2���ֽڵ�Ŀ��PDU��
    //nDstLength += InvertNumbers(pSrc->SCA, &pDst[nDstLength], nLength);    // ת��SMSC��Ŀ��PDU��
	 pDst[0] = '0';
	 pDst[1] = '0';
	 nDstLength = 2;

   
    // TPDU�λ���������Ŀ���ַ��
    nLength = strlen(pSrc->TPA);    // TP-DA��ַ�ַ����ĳ���
    SMS_cBuffer[0] = 0x11;            // �Ƿ��Ͷ���(TP-MTI=01)��TP-VP����Ը�ʽ(TP-VPF=10)
    SMS_cBuffer[1] = 0;               // TP-MR=0
    SMS_cBuffer[2] = (char)nLength;   // Ŀ���ַ���ָ���(TP-DA��ַ�ַ�����ʵ����)
    SMS_cBuffer[3] = 0x91;            // �̶�: �ù��ʸ�ʽ����
    nDstLength += Bytes2String(SMS_cBuffer, &pDst[nDstLength], 4); // ת��4���ֽڵ�Ŀ��PDU��
    nDstLength += InvertNumbers(pSrc->TPA, &pDst[nDstLength], nLength); // ת��TP-DA��Ŀ��PDU��
   
    // TPDU��Э���ʶ�����뷽ʽ���û���Ϣ��
    nLength = DataLen;	//strlen(pSrc->TP_UD);    // �û���Ϣ�ַ����ĳ���
    SMS_cBuffer[0] = pSrc->TP_PID;        // Э���ʶ(TP-PID)
    SMS_cBuffer[1] = pSrc->TP_DCS;        // �û���Ϣ���뷽ʽ(TP-DCS)
    SMS_cBuffer[2] = 0;            // ��Ч��(TP-VP)Ϊ5����
	if(pSrc->TP_DCS == SMS_7BIT)	
	{
		// 7-bit���뷽ʽ
		SMS_cBuffer[3] = nLength;			// ����ǰ����
		nLength = Encode7bit(pSrc->TP_UD, &SMS_cBuffer[4], nLength+1) + 4;	// ת��TP-DA��Ŀ��PDU��
	}
	else if(pSrc->TP_DCS == SMS_UCS2)
	{
		// UCS2���뷽ʽ
		SMS_cBuffer[3] = EncodeUcs2(pSrc->TP_UD, &SMS_cBuffer[4], nLength);	// ת��TP-DA��Ŀ��PDU��
		nLength = SMS_cBuffer[3] + 4;		// nLength���ڸö����ݳ���
	}
	else
	{
		// 8-bit���뷽ʽ
		SMS_cBuffer[3] = Encode8bit(pSrc->TP_UD, &SMS_cBuffer[4], nLength);	// ת��TP-DA��Ŀ��PDU��
		nLength = SMS_cBuffer[3] + 4;		// nLength���ڸö����ݳ���
	}
	nDstLength += Bytes2String(SMS_cBuffer, &pDst[nDstLength], nLength);		// ת���ö����ݵ�Ŀ��PDU��

	
	// ����Ŀ���ַ�������
	return nDstLength;
}




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ��������: PDU���룬���ڽ��ա��Ķ�����Ϣ
   ��ڲ�����pSrc - ԴPDU��ָ��,pDst - Ŀ��PDU����ָ��
   ���ڲ������û���Ϣ������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺	
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
unsigned int    SMSCls::DecodePdu(const char* pSrc, SM_PARAM* pDst)
{
	int nDstLength=0;			// Ŀ��PDU������
	unsigned char tmp;		// �ڲ��õ���ʱ�ֽڱ���
	
	if(strlen(pSrc)>LEN_SMS_LIMIT_BYTES)	return 0;
	// SMSC��ַ��Ϣ��
	String2Bytes(pSrc, &tmp, 2);	// ȡ����
	tmp = (tmp - 1) * 2;	// SMSC���봮����
	pSrc += 4;			// ָ����ƣ�������SMSC��ַ��ʽ
	SerializeNumbers(pSrc, pDst->SCA, tmp);	// ת��SMSC���뵽Ŀ��PDU��
	pSrc += tmp;		// ָ�����
       
	// TPDU�λ�������
	String2Bytes(pSrc, &tmp, 2);	// ȡ��������
	pSrc += 2;		// ָ�����
	pDst->index = tmp;			  //bit6 ��λ��pDst->TP_DCS�зְ�Э��ͷ
	//pDst->index <<= 8;			  //
	// ȡ�ظ�����
	String2Bytes(pSrc, &tmp, 2);	// ȡ����
	if(tmp & 1) tmp += 1;	// ������ż��
	pSrc += 4;			// ָ����ƣ������˻ظ���ַ(TP-RA)��ʽ
	SerializeNumbers(pSrc, pDst->TPA, tmp);	// ȡTP-RA����
	pSrc += tmp;		// ָ�����

	// TPDU��Э���ʶ�����뷽ʽ���û���Ϣ��
	String2Bytes(pSrc, (unsigned char*)&pDst->TP_PID, 2);	// ȡЭ���ʶ(TP-PID)
	pSrc += 2;		// ָ�����
	String2Bytes(pSrc, (unsigned char*)&pDst->TP_DCS, 2);	// ȡ���뷽ʽ(TP-DCS)
	pSrc += 2;		// ָ�����
	SerializeNumbers(pSrc, pDst->TP_SCTS, 14);		// ����ʱ����ַ���(TP_SCTS) 
	pSrc += 14;		// ָ�����
	String2Bytes(pSrc, &tmp, 2);	// �û���Ϣ����(TP-UDL)
	pSrc += 2;		// ָ�����
	
	////////////////////////////////////////////////// ���ֱ��뷽ʽ�Ľ���
	if(pDst->TP_DCS == SMS_7BIT)	
	{
		// 7-bit����
		nDstLength = String2Bytes(pSrc, SMS_cBuffer, tmp & 7 ? (int)tmp * 7 / 4 + 2 : (int)tmp * 7 / 4);	// ��ʽת��
		Decode7bit(SMS_cBuffer, pDst->TP_UD, nDstLength);	// ת����TP-DU
		nDstLength = tmp;
}
	else if(pDst->TP_DCS == SMS_UCS2)
	{
		// UCS2����
		nDstLength = String2Bytes(pSrc, SMS_cBuffer, tmp * 2);			// ��ʽת��
		nDstLength = DecodeUcs2(SMS_cBuffer, pDst->TP_UD, nDstLength);	// ת����TP-DU
	}
	else
	{
		// 8-bit����
		nDstLength = String2Bytes(pSrc, SMS_cBuffer, tmp * 2);			// ��ʽת��
		nDstLength = Decode8bit(&SMS_cBuffer[tmp], pDst->TP_UD, nDstLength);	// ת����TP-DU
	}
	tmp = 0;
	if((pDst->index>>6)&0x0001)	//�Ƿ��зְ�
	{
	   tmp = pDst->TP_UD[0];
		tmp++;
		nDstLength -= tmp;
		memmove(SMS_cBuffer, &pDst->TP_UD[tmp], nDstLength);
		memmove(pDst->TP_UD, SMS_cBuffer, nDstLength);
	}

	// ����Ŀ���ַ�������
	return nDstLength;
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ��������: // ��ʼ��GSM״̬
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺	
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
unsigned int		SMSCls::Init(void)
{

	return TRUE;
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ��������: ���Ͷ���Ϣ���������������ȡӦ��
   ��ڲ�����pSrc - ԴPDU����ָ�루δ���룩
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺	
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
unsigned int    SMSCls::SendPduMessage(SM_PARAM* pSrc,unsigned short DataLen)
{
	//SM_PARAM		Sms;
	unsigned short		Len=0;
	int nPduLength=0;						// PDU������
	//unsigned char nSmscLength=0;		// SMSC������
	//int nLength;							// �����յ������ݳ���
	char cmd[16];							// ���	
	char ans[16];							// Ӧ��
	uchar	s=0;
	char  *pS;


	//s = GSM.AT_CommandWaitRetStatus(eAT_CMGF0,NULL,(uchar *)ans);			 //
	//if(s!=OK) return ERROR;


	if(DataLen>LEN_SMS_LIMIT_BYTES)	return ERROR;
	SetBufferValue(SMS_cPduBuffer, 512, 0);
	
	nPduLength = EncodePdu(pSrc, (char*)SMS_cPduBuffer,DataLen);		// ����PDU����������PDU��
	strcat((char*)SMS_cPduBuffer, "\x01a");							// ��Ctrl-Z����
	//SMS_cPduBuffer[nPduLength] = 0x1A;
	//SMS_cPduBuffer[nPduLength+1] = 0x1A; 
	//SMS_cPduBuffer[nPduLength+2] = 0x1A;
	
   //Uart.putb(1, nPduLength+1, SMS_cPduBuffer);	  os_dly_wait(10);

	//Len = SMS.DecodePdu((char*)SMS_cPduBuffer,(SM_PARAM*)&Sms);
	

	//String2Bytes((char*)SMS_cPduBuffer, &nSmscLength, 2);		// ȡPDU���е�SMSC��Ϣ����
	//nSmscLength++;										// ���ϳ����ֽڱ���	
	
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
					//pS = MyStrStr((char *)ans,(char*)"OK\0");					 //PDUģʽû���ҵ� OK����
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
	�������ƣ�SendPduDataMessage
   ��������: ���Ͷ���Ϣ���������������ȡӦ��
   ��ڲ�����pSrc - ԴPDU����ָ�루��
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺	
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
unsigned char    SMSCls::SetPduDataMessage(uchar CodeType, char *number,uchar *msg, uchar MsgLen)
{
	//strcpy((char *)PDU_SMS.SCA, (char *)sTelNum.SmsCenter); 
	SetBufferValue((uchar *)PDU_SMS.SCA, 16, 0);  // ����GSMģ��Ҫ��pdu ģʽ���������ĺ�������
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
**	�������ƣ�SendTextMessage()
** ��������: ����TEXT��ʽ��Ϣ
** ��ڲ������绰���룬ָ��Ҫ�������ݵ�ָ��
** ���ڲ���������״̬
** ȫ�ֱ���: ��,��Ҫ������Ϣ���ĺ���
** ����ʱ�䣺2009.11.11
** �޸�ʱ�䣺	
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
 
