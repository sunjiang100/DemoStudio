/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 		  
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 	 ATCommand.h
	功能描述：AT命令定义
	编译环境：
	目标CPU: 
	作者：
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  


#include			"..\Config\RTOS.h"

#include			"GSM_GPRS.h"

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	定义GSM AT命令
*/



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	定义GPRS AT命令
*/
uchar		const		AT_CIPStart[]	= {"AT+CIPSTART"};		// 连接到服务器
uchar		const		AT_CIPSend[]	= {"AT+CIPSEND\r\n\0"};	// 发送数据到服务器
uchar		const		AT_CIPClose[]	= {"AT+CIPCLOSE\r\n\0"};// 关闭TCP/UDP 连接
uchar		const		AT_CIPShut[]	= {"AT+CIPSHUT\r\n\0"};	// 关闭移动场景
uchar		const		AT_CLPort[]		= {"AT+CLPORT"};			// 设置本地端口号
uchar		const		AT_CSTT[]		= {"AT+CSTT"};				// 启动任务并设置APN, user name, password
uchar		const		AT_CIIcr[]		= {"AT+CIICR"};			// 激活移动场景
uchar		const		AT_CIFSR[]		= {"AT+CIFSR"};			// 获得本地IP 地址
uchar		const		AT_CIPStatus[]	= {"AT+CIPSTATUS\r\n\0"};	// 查询目前状态
uchar		const		AT_CDNSCfg[]	= {"AT+CDNSCFG\r\0"};	// 配置DNS 的IP 地址
uchar		const		AT_CDNSGip[]	= {"AT+CDNSGIP\r\0"};	// 域名解析, 获取IP 地址
uchar		const		AT_CDNSSorip[]	= {"AT+CDNSORIP\r\0"};	// 设置连接域名或IP 地址
uchar		const		AT_CIPHead[]	= {"AT+CIPHEAD=1\r\0"};// 设置接收的数据头
uchar		const		AT_CIPATS[]		= {"AT+CIPATS\r\0"};		// 设置自动发送时间
uchar		const		AT_CIPSprt[]	= {"AT+CIPSPRT\r\0"};	// 设置在AT+CIPSEND 后是否跟有发送提示符>
uchar		const		AT_CIPServer[]	= {"AT+CIPSERVER\r\0"};	// 配置为服务器
uchar		const		AT_CIPCSGP[]	= {"AT+CIPCSGP\r\0"};	// 设置为CSD 或GPRS 连接以及相关参数
uchar		const		AT_CIPCCON[]	= {"AT+CIPCCON\r\0"};	// 设置当前的连接
uchar		const		AT_CIPFLP[]		= {"AT+CIPFLP\r\0"};		// 是否固定本地端口号
uchar		const		AT_CIPSRIP[]	= {"AT+CIPSRIP\r\0"};	// 设置是否显示发送方的IP 地址和端口号


uchar		const		AT_CSMINS[]    = {"AT+CSMINS=1\r\n\0"};	// 
uchar		const		AT_CSMINS[]    = {"AT+CSMINS?\r\n\0"};


uchar		const		AT_CPIN[]      = {"AT+CPIN=\r\n\0"};



/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
