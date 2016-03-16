/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 		  
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
	�ļ���: 	 DownloadCmd.h
	���������������������ԭ���������б�
	���뻷����
	Ŀ��CPU: 
	���ߣ�
	����ʱ�䣺
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  

#ifndef		__CMD_PROTOTYPETAB_H__
#define		__CMD_PROTOTYPETAB_H__






#if(USE_TH_GPRS_DATA_PACK_PROTOCOL)
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	����ָ�뺯������������ÿһ���Ҫԭ������
*/








































void  	  (GPRSCls::*TH_DownloadCmdProcessTab[3])(unsigned char *p)= 
{ 
	(&GPRSCls::AckAlarm						),
	(&GPRSCls::ClearWorkTime					),
	(&GPRSCls::LocationQuery				   ),	
};




class			GPRSCls
{
public:
	GPRSCls(void);

	void			ProcessReceiveData(uint16 CmdLen,char *pData);

	void			AckAlarm						(uchar *pPS);
	void			ClearWorkTime				(uchar *pPS);
	void			LocationQuery				(uchar *pPS);
};


GSM_GPRS_EXT	__noinit__		GPRSCls		GPRS;		//






#endif






#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */


