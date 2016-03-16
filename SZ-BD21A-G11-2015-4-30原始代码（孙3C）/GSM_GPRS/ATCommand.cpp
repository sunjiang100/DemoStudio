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


#include			"..\Config\RTOS.h"

#include			"GSM_GPRS.h"

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	����GSM AT����
*/



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	����GPRS AT����
*/
uchar		const		AT_CIPStart[]	= {"AT+CIPSTART"};		// ���ӵ�������
uchar		const		AT_CIPSend[]	= {"AT+CIPSEND\r\n\0"};	// �������ݵ�������
uchar		const		AT_CIPClose[]	= {"AT+CIPCLOSE\r\n\0"};// �ر�TCP/UDP ����
uchar		const		AT_CIPShut[]	= {"AT+CIPSHUT\r\n\0"};	// �ر��ƶ�����
uchar		const		AT_CLPort[]		= {"AT+CLPORT"};			// ���ñ��ض˿ں�
uchar		const		AT_CSTT[]		= {"AT+CSTT"};				// ������������APN, user name, password
uchar		const		AT_CIIcr[]		= {"AT+CIICR"};			// �����ƶ�����
uchar		const		AT_CIFSR[]		= {"AT+CIFSR"};			// ��ñ���IP ��ַ
uchar		const		AT_CIPStatus[]	= {"AT+CIPSTATUS\r\n\0"};	// ��ѯĿǰ״̬
uchar		const		AT_CDNSCfg[]	= {"AT+CDNSCFG\r\0"};	// ����DNS ��IP ��ַ
uchar		const		AT_CDNSGip[]	= {"AT+CDNSGIP\r\0"};	// ��������, ��ȡIP ��ַ
uchar		const		AT_CDNSSorip[]	= {"AT+CDNSORIP\r\0"};	// ��������������IP ��ַ
uchar		const		AT_CIPHead[]	= {"AT+CIPHEAD=1\r\0"};// ���ý��յ�����ͷ
uchar		const		AT_CIPATS[]		= {"AT+CIPATS\r\0"};		// �����Զ�����ʱ��
uchar		const		AT_CIPSprt[]	= {"AT+CIPSPRT\r\0"};	// ������AT+CIPSEND ���Ƿ���з�����ʾ��>
uchar		const		AT_CIPServer[]	= {"AT+CIPSERVER\r\0"};	// ����Ϊ������
uchar		const		AT_CIPCSGP[]	= {"AT+CIPCSGP\r\0"};	// ����ΪCSD ��GPRS �����Լ���ز���
uchar		const		AT_CIPCCON[]	= {"AT+CIPCCON\r\0"};	// ���õ�ǰ������
uchar		const		AT_CIPFLP[]		= {"AT+CIPFLP\r\0"};		// �Ƿ�̶����ض˿ں�
uchar		const		AT_CIPSRIP[]	= {"AT+CIPSRIP\r\0"};	// �����Ƿ���ʾ���ͷ���IP ��ַ�Ͷ˿ں�


uchar		const		AT_CSMINS[]    = {"AT+CSMINS=1\r\n\0"};	// 
uchar		const		AT_CSMINS[]    = {"AT+CSMINS?\r\n\0"};


uchar		const		AT_CPIN[]      = {"AT+CPIN=\r\n\0"};



/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
