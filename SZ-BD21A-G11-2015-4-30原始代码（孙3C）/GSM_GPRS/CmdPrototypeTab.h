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

#include		"GSM_GPRS.h"




#if(USE_TH_GPRS_DATA_PACK_PROTOCOL)
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	����ָ�뺯������������ÿһ���Ҫԭ������
*/

 
	  





void    (GPRSCls:: *DownloadCmdProcessTab[DOWNLOAD_CMD_TOTAL_NUM])(uchar *p)= 
{ 
	(&GPRSCls::LocationQuery				   ),
	(&GPRSCls::GetSystemLog						),
	(&GPRSCls::Set3IP								),
	(&GPRSCls::SystemCoolBoot					),
	(&GPRSCls::SetCenterNumber					),
	(&GPRSCls::SetReturnTime					),
	(&GPRSCls::RemoteOilControl				),
	(&GPRSCls::SetIP_Port						),
	(&GPRSCls::SetAPN								),
	(&GPRSCls::RestoreFactorySetting			),
	(&GPRSCls::RemoteUpgrade					),
	(&GPRSCls::SetRegInfo						),
	(&GPRSCls::SetNormalPara					),
	(&GPRSCls::ReadIP_PortAndInterval		),
	(&GPRSCls::ReadSMS_Number					),
	(&GPRSCls::Calling_GPS						),
	(&GPRSCls::SetOrGetAccelerometerPara	),
	(&GPRSCls::RemoteMonitor					),
	(&GPRSCls::PhoneSchedulFunction			),
	(&GPRSCls::SetOrGetTermialPhoneNumber  ),
};	 
#endif






#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */


