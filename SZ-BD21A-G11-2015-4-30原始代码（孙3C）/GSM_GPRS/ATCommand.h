/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 		  
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
	�ļ���: 	 ATCommand.h
	����������AT�����
	���뻷����
	Ŀ��CPU: 
	���ߣ�
	����ʱ�䣺
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  

#ifndef		__ATCOMMAND_H_
#define		__ATCOMMAND_H_



#include			"..\Config\RTOS.h"

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	����GSM AT����
*/



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	����GPRS AT����
*/
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	����GPRS AT����
*/
extern	const	uchar	AT_CIPStart[];		// ���ӵ�������
extern	const	uchar	AT_CIPSend[];		// �������ݵ�������
extern	const	uchar	AT_CIPClose[];		// �ر�TCP/UDP ����
extern	const	uchar	AT_CIPShut[];		// �ر��ƶ�����
extern	const	uchar	AT_CLPort[];		// ���ñ��ض˿ں�
extern	const	uchar	AT_CSTT[];			// ������������APN, user name, password
extern	const	uchar	AT_CIIcr[];			// �����ƶ�����
extern	const	uchar	AT_CIFSR[];			// ��ñ���IP ��ַ
extern	const	uchar	AT_CIPStatus[];		// ��ѯĿǰ״̬
extern	const	uchar	AT_CDNSCfg[];		// ����DNS ��IP ��ַ
extern	const	uchar	AT_CDNSGip[];		// ��������, ��ȡIP ��ַ
extern	const	uchar	AT_CDNSSorip[];		// ��������������IP ��ַ
extern	const	uchar	AT_CIPHead[];		// ���ý��յ�����ͷ
extern	const	uchar	AT_CIPATS[];		// �����Զ�����ʱ��
extern	const	uchar	AT_CIPSprt[];		// ������AT+CIPSEND ���Ƿ���з�����ʾ��>
extern	const	uchar	AT_CIPServer[];		// ����Ϊ������
extern	const	uchar	AT_CIPCSGP[];		// ����ΪCSD ��GPRS �����Լ���ز���
extern	const	uchar	AT_CIPCCON[];		// ���õ�ǰ������
extern	const	uchar	AT_CIPFLP[];		// �Ƿ�̶����ض˿ں�
extern	const	uchar	AT_CIPSRIP[];		// �����Ƿ���ʾ���ͷ���IP ��ַ�Ͷ˿ں�






#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
