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

#ifndef		__ATCOMMAND_H_
#define		__ATCOMMAND_H_



#include			"..\Config\RTOS.h"

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	定义GSM AT命令
*/



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	定义GPRS AT命令
*/
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	定义GPRS AT命令
*/
extern	const	uchar	AT_CIPStart[];		// 连接到服务器
extern	const	uchar	AT_CIPSend[];		// 发送数据到服务器
extern	const	uchar	AT_CIPClose[];		// 关闭TCP/UDP 连接
extern	const	uchar	AT_CIPShut[];		// 关闭移动场景
extern	const	uchar	AT_CLPort[];		// 设置本地端口号
extern	const	uchar	AT_CSTT[];			// 启动任务并设置APN, user name, password
extern	const	uchar	AT_CIIcr[];			// 激活移动场景
extern	const	uchar	AT_CIFSR[];			// 获得本地IP 地址
extern	const	uchar	AT_CIPStatus[];		// 查询目前状态
extern	const	uchar	AT_CDNSCfg[];		// 配置DNS 的IP 地址
extern	const	uchar	AT_CDNSGip[];		// 域名解析, 获取IP 地址
extern	const	uchar	AT_CDNSSorip[];		// 设置连接域名或IP 地址
extern	const	uchar	AT_CIPHead[];		// 设置接收的数据头
extern	const	uchar	AT_CIPATS[];		// 设置自动发送时间
extern	const	uchar	AT_CIPSprt[];		// 设置在AT+CIPSEND 后是否跟有发送提示符>
extern	const	uchar	AT_CIPServer[];		// 配置为服务器
extern	const	uchar	AT_CIPCSGP[];		// 设置为CSD 或GPRS 连接以及相关参数
extern	const	uchar	AT_CIPCCON[];		// 设置当前的连接
extern	const	uchar	AT_CIPFLP[];		// 是否固定本地端口号
extern	const	uchar	AT_CIPSRIP[];		// 设置是否显示发送方的IP 地址和端口号






#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
