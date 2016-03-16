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






#if(USE_TH_GPRS_DATA_PACK_PROTOCOL)
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	定义指针函数表。函数表里每一项都需要原形声明
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


