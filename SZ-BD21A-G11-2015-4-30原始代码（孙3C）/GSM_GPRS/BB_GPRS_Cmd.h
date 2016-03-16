/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 		  
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 	 GPRS.c
	功能描述：GRPS相关函数
	编译环境：
	目标CPU: 
	作者：
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					

#ifndef		__BB_GPRS_CMD_H__
#define		__BB_GPRS_CMD_H__

#ifdef      BB_GPRS_GLOBALS
#define     BB_GPRS_EXT
#else
#define     BB_GPRS_EXT    extern
#endif


#include			<stdlib.h>
#include			<string.h>
#include			"..\GPS\GPS.h"
#include			"GSM_GPRS.h"  
#include			"..\Common\FLASH_ADDR.H"
#include			"..\ADC\ADC.h"
#include			"..\Config\RTOS.h"
#include			"..\UART\UART1.h"
#include			"..\UART\UART0.h"
#include			"..\Switch\switch.h"
#include			"..\Common\Algol.h"
#include			"..\camera\camera.h"

#include			"..\KeyDisp\KeyDisp.h"
#include			"..\FLASH\DFE_25xxx.H"
#include			"..\FLASH\DFF_25xxx.H"

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	GY协议代码枚举
*/
BB_GPRS_EXT		enum
{
	eGB_gCMD_REG 								= 0,
	eGB_gCMD_REG_ACK,
	eGB_gCMD_SET_SETUP_PARA,
	eGB_gCMD_GET_SETUP_PARA,
	eGB_gCMD_SEND_PARA,
	eGB_gCMD_RS232 							= 0x0f,
	eGB_gCMD_SET_GPS_ALARM_PARA,
	eGB_gCMD_GET_GPS_ALARM_PARA,
	eGB_gCMD_SEND_GPS_ALARM_PARA,
	eGB_gCMD_SEND_GPS_INFO,
	eGB_gCMD_SET_ELE_RAIL 					= 0x20,
	eGB_gCMD_SET_GET_ELE_RAIL,
	eGB_gCMD_SEND_ELE_RAIL_DATA,
	eGB_gCMD_SEND_ELE_RAIL_DIR,
	eGB_gCMD_SET_ROAD 						= 0x28,
	eGB_gCMD_SET_GET_ROAD,
	eGB_gCMD_SEND_ROAD_DATA,
	eGB_gCMD_SEND_ROAD_DIR,
	eGB_gCMD_TEXT_TTS_MESSAGE,
	eGB_gCMD_SET_PHONE_LISTNING,		
	
	
	
} BB_CMD_ENUM;



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	GY协议处理函数返回代码
*/
GSM_GPRS_EXT	enum
{
	BB_OK = 0,
	BB_CHECK_SUM_ERROR,
	
} BB_RET_CODE;


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	GY应答位处理，0x20选择应答，0x30必须应答，其它值无需应答 
*/
#define		BB_SLE_ACK					0x20
#define		BB_NEED_ACK		 		 	0x30				
#define		BB_ACK_CHECK_BIT 		 	0x30

#define		BB_ACK_OK					0xf0
#define		BB_ACK_ERROR				0xf1



#define		BB_CMD_ADDR_HEAD			0
#define		BB_CMD_ADDR_SIM			3
#define		BB_CMD_ADDR_LENGTH		11
#define		BB_CMD_ADDR_INDEX			13
#define		BB_CMD_ADDR_PARA			14
#define		BB_CMD_ADDR_CMD_CODE		15
#define		BB_CMD_ADDR_DATA_BLOCK	16
#define		LEN_BB_PACK_HEAD			16


typedef		struct	_BB_MESSAGE_STRUCT_
{
	uchar		Head[3];			// 0,固定*GB	  2A 47 42
	uchar		SimNumber[8];	// 3,压缩BCD码，16位手机号码，空位为0F
	uint16	Length;			// 11,
	uchar		Index;			// 13,
	uchar		Para;				// 14,
	uchar		Cmd;				// 15,
} BB_MESSAGE,*pBB_MESSAGE;


typedef		struct	_BB_MESSAGE_ACK_STRUCT_
{
	uchar		Head[3];			// 固定*GB
	uchar		SimNumber[8];	// 压缩BCD码，16位手机号码，空位为0F
	uchar		Ack;				// 固定为0xff
	uchar		Index;		   
	uchar		AckStatus;		// 0xf0：收到正确消息，0xf1收到错误的消息
	uchar		Sum;	
} BB_MESSAGE_ACK,*pBB_MESSAGE_ACK;



#endif
