/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$									���ݹ�ҵ���ӿƼ����޹�˾
$									
$                       	http://www.guoyee.com		 Email: guoyegps@126.com	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
�ļ���:   gsm_send.cPP
����������GSM��������
���뻷����
Ŀ��CPU: 
���ߣ�
����ʱ�䣺
����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/		
#ifndef		__ATCMD_H__
#define		__ATCMD_H__

#ifdef      AT_CMD_GLOBALS
#define     AT_CMD_EXT
#else
#define     AT_CMD_EXT    extern
#endif
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	AT����
*/
#define	MAX_AT_CMD_NUM			58
AT_CMD_EXT		enum					// �˱�˳�������ATCmd.cpp��AT������Ӧ���������
{						// �����ַ���
	eAT_OK0 = 0,		// 
	eAT_OK,          // \r\nOK\r\n	   or		\r\nERROR\r\n
	eAT_E0,          // \r\nOK\r\n
	eAT_IFC,
	eAT_CIPHEAD,
	eAT_QIPROMPT,
	/////////////// ///////////////
	eAT_COPS,        // \r\n++COPS: 0,0,"China Mobile"\r\n \r\nOK\r\n
	eAT_QIMUX,
	eAT_QICSGP,
	eAT_QIFGCNT,
	eAT_QIREGAPP,
	eAT_QIACT,
	eAT_QIDEACT,
	eAT_QINDI,
	eAT_QICLOSE,
	eAT_QIOPEN_TCP,
	eAT_QIOPEN_UDP,
	eAT_QISTAT,
	eAT_QILOCIP,     //	 �鱾��IP	
    eAT_QIMODE,
	eAT_QIDNSGIP,
	eAT_QIDNSIP,
	/////////////// ///////////////
	eAT_CMGF0,       // \r\nOK\r\n
	eAT_CMGF1,       // \r\nOK\r\n
	eAT_CSCS,
	eAT_CPMS,
	eAT_CMGD_ALL,    // \r\nOK\r\n
	eAT_CNMI,        // \r\nOK\r\n
	eAT_CSCA_SET,    // \r\nOK\r\n
	eAT_CSCA_GET,    // \r\n+CSCA: "+8613800200500",145\r\n   \r\nOK\r\n
	/////////////// ///////////////
	eAT_CLIP,        // \r\nOK\r\n
	eAT_CRC,
	eAT_S0,          // \r\nOK\r\n
	eAT_D,           // \r\nOK\r\n   �޷ֺ�Ҳ�ܴ�ͨ���ܻ���������ʾ"����ͨ��"
	eAT_DL,          // \r\nOK\r\n
	eAT_H,           // \r\nOK\r\n
	eAT_A,        
	eAT_CLVL_SET,    // \r\nOK\r\n
	eAT_CLVL_GET,    // \r\n+CLVL: 80\r\n    \r\nOK\r\n
	/////////////// ///////////////
	eAT_CPIN,        // \r\n+CPIN: READY\r\n  \r\nOK\r\n
	eAT_CSQ,         // \r\n+CSQ: 19,99\r\n   \r\nOK\r\n
	eAT_CREG,        // \r\n+CREG: 0,1\r\n   \r\nOK\r\n
	/////////////// ///////////////
	eAT_CNUM,        // \r\nOK\r\n
	eAT_QCCID,        // \r\n+CCID: 89860080190739733825\r\n   \r\nOK\r\n
	eAT_CGSN,        // \r\n357852037769893\r\n   \r\nOK\r\n
	eAT_CGMR,        // \r\n07.60.00\r\n   \r\nOK\r\n
	eAT_CIMI,		  // \r\n460002037769893\r\n   \r\nOK\r\n
	eAT_QMIC,		  // \r\nOK\r\n
	eAT_QISACK,			// 
	eAT_NOP,         // 
	eAT_ERROR,       // 
	
} ENUM_AT_CMD;	


#endif


/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */


