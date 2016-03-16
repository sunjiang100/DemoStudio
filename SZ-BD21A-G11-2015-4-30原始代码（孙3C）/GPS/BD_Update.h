/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
	�ļ���: GPS.h
	����������GPS��ض����ͷ�ļ�
	���뻷����Keil for ARM 2.2
	Ŀ��CPU: LPC213x
	���ߣ�	Ф����
	����ʱ�䣺
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
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
	uint16	Head;			// �̶�Ϊ0x41, 0x54("AT"��ascii�룩
	uint32	FirmLength; // �����ù̼�����
	uint32	FirmStartAddr;  // �̼���д��ַ���������룺0x8000  �������룺0x0000		����������0x3e000
	uint32	FirmInFileStartAddr; // �����ö����ƹ̼���*.ubf�ļ���ǰ���е���ʼ��ַ
	uint16	FirmType;		// 1���������룬2���������룬3����������
	uchar		ModelType[16];	//  ģ���ͺŵ��ַ���,��:ATGM330B 
	uchar		CodeVersion[16];  // ����汾���ַ���,��:V2.0.5.0 
	uchar		CodeName[128];    // ԭʼ������ļ��� 
	uchar		CodeDate[32];			//	 ԭʼ������������ڡ�ʱ�� 

	
	
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
	// ����𷽣�����
	AT_BUP_UP_UPDATE_BAUDRATE_CMD  = 0x01,		// ����ģ������������
	AT_BUP_SET_FIRM_UPATE_PARA_CMD = 0x02,		// ���ù̼���������
	AT_BUP_SEND_DATA_PACK_CMD      = 0x05,		// ���͹̼����ݰ�
	AT_BUP_UPDATE_OK_RETSET_CMD   = 0x06,		// ������ɣ�����ģ��


	// ����𷽣� ģ��
	AT_BUP_FIRM_UPDATE_FINISH		= 0x86,		// �̼��������֪ͨ



} AT_BUP_Cmd_Enum;



																  




#endif

