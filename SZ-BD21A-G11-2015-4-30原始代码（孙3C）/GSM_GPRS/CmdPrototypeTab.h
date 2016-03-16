/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 		  
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 	 DownloadCmd.h
	功能描述：下载命令处理函数原形声明及列表
	编译环境：
	目标CPU: 
	作者：
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  

#ifndef		__CMD_PROTOTYPETAB_H__
#define		__CMD_PROTOTYPETAB_H__

#include		"GSM_GPRS.h"




#if(USE_TH_GPRS_DATA_PACK_PROTOCOL)
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	定义指针函数表。函数表里每一项都需要原形声明
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


