/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												���ݹ�ҵ���ӿƼ����޹�˾
$										Shawyoo Electronics (GuangZhou) R&D Center
$                       	http://www.guoyee.com		 Email: guoyegps@126.com
$								 QQȺ��9069471/9069433/9524253		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
	�ļ���: 	  IC_Card.h
	���������� ͷ�ļ�
	���뻷���� Keil for ARM + ARTX
	Ŀ��CPU:   
	���ߣ�	  
	����ʱ�䣺 
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  

#ifndef        __IC_CARD_H__
#define			__IC_CARD_H__



#ifdef      IC_CARD_GLOBALS
#define     IC_CARD_EXT
#else
#define     IC_CARD_EXT    extern
#endif


#include		"TWI.h"

#include		"..\Config\RTOS.h"

// $$$$$$$$$$$$$$$$$$ <<< Use Configuration Wizard in Context Menu >>> $$$$$$$$$$$$$$$$$
// <h>IIC0�������


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>IC_BYTE_SEL: IC���ֽ� <1=> 256�ֽ� <2=> 512�ֽ� <3=> 1024�ֽ�
//   	<i> Default: 256
*/
#define			IC_BYTE_SEL				1

#if(IC_BYTE_SEL==1)
#define		IC_CARD_BYTE_NUMBER      256
#elif(IC_BYTE_SEL==2)
#define		IC_CARD_BYTE_NUMBER      512
#elif(IC_BYTE_SEL==2)
#define		IC_CARD_BYTE_NUMBER      1024
#else
#error  "IC����������"
#endif



// </h>
// $$$$$$$$$$$$$$$$$$$ <<< end of configuration section >>> $$$$$$$$$$$$$$$$$$$$$$$$$$$$



IC_CARD_EXT		enum
{
	eIC_CARD_READ_MAIN_MEMORY = 0x30,
	eIC_CARD_WRITE_MAIN_MEMORY = 0x38,
	eIC_CARD_READ_PROTECT_MEMORY = 0x34,
	eIC_CARD_WRITE_PROTECT_MEMORY = 0x3c,
	eIC_CARD_READ_SECURITY_MEMORY = 0x31,
	eIC_CARD_WRITE_SECURITY_MEMORY = 0x39,
	eIC_CARD_CHECK_SECURITY_CODE = 0x33,
} IC_CARD_CMD;




						
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    
*/
typedef	struct	_IC_CARD_
{
	uchar		CuringInfo[32];			// 0	 	оƬ���̹̻���Ϣ
	uchar		CCC[7];						// 32	 	�����������루��ҵ3C��֤���룩
	uchar		SetupYear;					// 39	  	������
	uchar		SetupMonth;					// 40		������
	uchar		SetupDay;               // 41		������
	uchar		CardSN[6];              // 42		��ˮ��
	uchar		Permissions;            // 48		Ȩ�ޣ�01��ʻ����02��ҵ����03��������05���Կ���06ִ������07��������08���̿�
	uchar		LicenseNumber[18];      // 49		��ʻ֤��
	uchar		Name[10];               // 67		��ʻԱ����
	uchar		Rsv[22];                // 77		��׼��չԤ��
	uchar		Rsv2[130];               // 99	������Ԥ��
	uchar		Company[10];            // 219	��λ����
	uchar		Department[10];         // 229	��������
	uchar		CardVer;                // 239	����IC����ʽ�汾
	uchar		GB_Ver[14];             // 240	ִ�еĹ���汾
	uchar		ChangeNum;              // 254	�޸ĵ���
	uchar		XorSum;                 // 255	���У��
} IC_CARD,*pIC_CARD;



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   
*/




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	IIC��
*/




#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
