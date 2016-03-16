/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: GPS.h
	功能描述：GPS相关定义的头文件
	编译环境：Keil for ARM 2.2
	目标CPU: LPC213x
	作者：	肖建辉
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  

#ifndef		_BD_UPDATE_H_
#define		_BD_UPDATE_H_

#include		"GPS.h"


#ifdef      BD_UPDATE_GLOBALS
#define     BD_UPDATE_EXT
#else
#define     BD_UPDATE_EXT    extern
#endif


#define		NAVIGATION_CODE_ADDR		0x8000
#define		UPDATE_CODE_ADDR			0x0000
#define		WORK_PARA_ADDR				0x3e00


#define		NAVIGATION_CODE_TYPE			1
#define		UPDATE_CODE_TYPE				2
#define		WORK_PARA_TYPE					3
	


#pragma				pack(1)
typedef	struct 
{
	uint16	Head;			// 固定为0x41, 0x54("AT"的ascii码）
	uint32	FirmLength; // 升级用固件长度
	uint32	FirmStartAddr;  // 固件烧写地址，导航代码：0x8000  升级代码：0x0000		工作参数：0x3e000
	uint32	FirmInFileStartAddr; // 升级用二进制固件在*.ubf文件当前块中的起始地址
	uint16	FirmType;		// 1—导航代码，2—升级代码，3—工作参数
	uchar		ModelType[16];	//  模块型号的字符串,如:ATGM330B 
	uchar		CodeVersion[16];  // 代码版本的字符串,如:V2.0.5.0 
	uchar		CodeName[128];    // 原始代码的文件名 
	uchar		CodeDate[32];			//	 原始代码的生成日期、时间 

	
	
} BD_UPDATE_FILE_HEAD_TYPE,*pBD_UPDATE_FILE_HEAD_TYPE;


typedef	struct
{
	uchar Head;
	uint16 Len;
	uchar DevId;
	uchar Cmd;	
} AT_BUP_PROTOCOL_HEAD_TYPE, *pAT_BUP_PROTOCOL_HEAD_TYPE;

#define		AT_BUP_DATA_INDEX		5


typedef	struct
{
	uint16 CodeType;
	uint32 Length;
	uint32 StAdd;
	
} SET_GNSS_FIRW_UPDATE_PARA, *pSET_GNSS_FIRW_UPDATE_PARA;


#define		LEN_BD_UP_PACK_DATA	1024
typedef	struct
{
	uint16 TotalPk;
	uint16 PkNo;
	uint16 PkSize;
	uchar Data[LEN_BD_UP_PACK_DATA];
} SEND_BD_UP_DATA_PACK,	*pSEND_BD_UP_DATA_PACK;
	
	

BD_UPDATE_EXT enum
{
	// 命令发起方：主机
	AT_BUP_UP_UPDATE_BAUDRATE_CMD  = 0x01,		// 提升模块升级波特率
	AT_BUP_SET_FIRM_UPATE_PARA_CMD = 0x02,		// 设置固件升级参数
	AT_BUP_SEND_DATA_PACK_CMD      = 0x05,		// 发送固件数据包
	AT_BUP_UPDATE_OK_RETSET_CMD   = 0x06,		// 升级完成，重启模块


	// 命令发起方： 模块
	AT_BUP_FIRM_UPDATE_FINISH		= 0x86,		// 固件升级完成通知



} AT_BUP_Cmd_Enum;



																  




#endif

