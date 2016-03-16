/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 		  
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
	�ļ���: 	 GPRS.c
	����������GRPS��غ���
	���뻷����
	Ŀ��CPU: 
	���ߣ�
	����ʱ�䣺
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					

#ifndef		__GB_GPRS_CMD_H__
#define		__GB_GPRS_CMD_H__

#ifdef      GB_GPRS_GLOBALS
#define     GB_GPRS_EXT
#else
#define     GB_GPRS_EXT    extern
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
	GYЭ�����ö��
*/
GB_GPRS_EXT		enum
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
	
	
	
} GB_CMD_ENUM;



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	GYЭ�鴦�������ش���
*/
GSM_GPRS_EXT	enum
{
	GB_OK = 0,
	GB_CHECK_SUM_ERROR,
	
} GB_RET_CODE;


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	GYӦ��λ����0x20ѡ��Ӧ��0x30����Ӧ������ֵ����Ӧ�� 
*/
#define		GB_SLE_ACK					0x20
#define		GB_NEED_ACK		 		 	0x30				
#define		GB_ACK_CHECK_BIT 		 	0x30

#define		GB_ACK_OK					0xf0
#define		GB_ACK_ERROR				0xf1



#define		GB_CMD_ADDR_HEAD			0
#define		GB_CMD_ADDR_SIM			3
#define		GB_CMD_ADDR_LENGTH		11
#define		GB_CMD_ADDR_INDEX			13
#define		GB_CMD_ADDR_PARA			14
#define		GB_CMD_ADDR_CMD_CODE		15
#define		GB_CMD_ADDR_DATA_BLOCK	16
#define		LEN_GB_PACK_HEAD			16


typedef		struct	_GB_MESSAGE_STRUCT_
{
	uchar		Head[3];			// 0,�̶�*GB	  2A 47 42
	uchar		SimNumber[8];	// 3,ѹ��BCD�룬16λ�ֻ����룬��λΪ0F
	uint16	Length;			// 11,
	uchar		Index;			// 13,
	uchar		Para;				// 14,
	uchar		Cmd;				// 15,
} GB_MESSAGE,*pGB_MESSAGE;


typedef		struct	_GB_MESSAGE_ACK_STRUCT_
{
	uchar		Head[3];			// �̶�*GB
	uchar		SimNumber[8];	// ѹ��BCD�룬16λ�ֻ����룬��λΪ0F
	uchar		Ack;				// �̶�Ϊ0xff
	uchar		Index;		   
	uchar		AckStatus;		// 0xf0���յ���ȷ��Ϣ��0xf1�յ��������Ϣ
	uchar		Sum;	
} GB_MESSAGE_ACK,*pGB_MESSAGE_ACK;



#endif
