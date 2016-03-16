/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
	�ļ���: 	 EEPROM_Addr.h
	����������EEPROM�洢��ַ����
	���뻷����
	Ŀ��CPU: 
	���ߣ�
	����ʱ�䣺
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  

#ifndef      __EEPROM_ADDR_H__
#define      __EEPROM_ADDR_H__


#ifdef      EEPROM_GLOBALS
#define     EEPROM_EXT
#else
#define     EEPROM_EXT    extern
#endif

#include    "..\Config\UserConfig.h"


// EEEPROM\x00 W \xa0 \xd0\x00 \x20\x00 

// $$$$$$$$$$$$$$$$$$ <<< Use Configuration Wizard in Context Menu >>> $$$$$$$$$$$$$$$$$
// <h>EEPROM��س������������

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>EEPROM_TYPE: ѡ��EEPROM�ͺ� <0=> AT24C02 <1=> AT24C04  <2=> AT24C512 <3=> FM24CL04
//   	<i> Default: FM24LC04
*/
#define			EEPROM_TYPE			3

#define			AT24C02				0
#define			AT24C04				1
#define			AT24C512				2
#define			FM24LC04				3


// </h>

// $$$$$$$$$$$$$$$$$$$ <<< end of configuration section >>> $$$$$$$$$$$$$$$$$$$$$$$$$$$$


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   EEPROM�ͺż���������
	ע����ʹ��ҳ����Ϊ32�Ĵ洢�����ɴ���32,������С��32��ϵͳ��־�ǰ�32�ֽ�Ϊ��洢�ġ�
*/

#define			EE_TOTAL_BYTE							8192                          //4096									//	8k Fram
#define			EE_PAGE_LEN								32										// ÿҳ����
#define			EE_TOTAL_PAGE							(EE_TOTAL_BYTE/EE_PAGE_LEN)  	// ʵ��ҳ��		


/////  	ϵͳ��һ�����б�־����ʹ������ͷ32�ֽ�  ʵ����16�ֽڣ���16�ֽ��Ǳ���,������лָ�����״̬///////
/////    �޸Ĵ洢��λ��ʱ����ǿ�ƻָ�����Ĭ��ֵ����������ʱ�Զ��ָ�����ֵ��
#define			EE_SYS_FIRST_RUN_FLAG_ADDR			0
#if(EE_SYS_FIRST_RUN_FLAG_ADDR==0)
#define			EE_SYS_FIRST_RUN						0x0000		// ϵͳ��һ�����б�־	,
#else
#define			EE_SYS_FIRST_RUN						0x1FF0		// 
#endif


/////////// change by wrc  ///////////////
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	GPS���ݴ���������
	
*/
#define        EE_OBL_BASE_ADDR                 0x1100 
////#define			EE_GPS_SEND_CONFIG					(EE_OBL_BASE_ADDR+0x10)		// 16B����������
//#define			EE_SEND_TIMES						(EE_OBL_BASE_ADDR+0x10)		// 4B��
//#define			EE_SEND_INTERVAL					(EE_OBL_BASE_ADDR+0x14)		// 2B��
//#define			EE_STOP_SEND_INTERVAL				(EE_OBL_BASE_ADDR+0x16)		// 2B��
//#define			EE_SEND_DISTANCE					(EE_OBL_BASE_ADDR+0x18)		// 2B��
//#define			EE_T_SEND_INTERVAL					(EE_OBL_BASE_ADDR+0x1a)		// 2B��
//#define			EE_T_SEND_TIME						(EE_OBL_BASE_ADDR+0x1c)		// 2B��
//#define			EE_SEND_TYPE						(EE_OBL_BASE_ADDR+0x1e)		// 1B��
//#define			EE_DELAY_ALARM						(EE_OBL_BASE_ADDR+0x1f)	// 1B

// 16B   left 


#define			EE_ELE_RAIL_TOTAL_NUM				(EE_OBL_BASE_ADDR+0x20)		// 2B
#define			EE_NOW_ELE_RAIL_NUM					(EE_OBL_BASE_ADDR+0x22)		// 2B
#define			EE_ALARM_ELE_RAIL_NUM				(EE_OBL_BASE_ADDR+0x24)		// 1B

// 1 Byte left

#define			EE_SELECT_SIM_CARD					(EE_OBL_BASE_ADDR+0x26)		// 1B
#define			EE_ENABLE_SIMU_SPEED		   		(EE_OBL_BASE_ADDR+0x27)		// 1B

#define			EE_SET_PARA_PASSWORD					(EE_OBL_BASE_ADDR+0x28)		// 4B, ���ò�������

// 	4 Bytes left


#define			EE_UP_DATA_FRAME_CNT1				(EE_OBL_BASE_ADDR+0x30)		// 4B,����֡��
#define			EE_UP_DATA_FRAME_CNT2				(EE_OBL_BASE_ADDR+0x34)		// 4B
#define			EE_UP_DATA_DOWN_OVER_FLAG    		(EE_OBL_BASE_ADDR+0x38)		// 1B	

// 1 Byte 	left	  

#define			EE_PACK_HEAD						(EE_OBL_BASE_ADDR+0x40)		// 16B

#define			EE_PARA_BLOCK_STATUS				(EE_OBL_BASE_ADDR+0x50)		// 16B=2B*8�����������ñ�־
#define			EE_GPS_SAVE_PAGE					(EE_OBL_BASE_ADDR+0x60)		// 16B��4��

#define			LEN_LOCK_CONTROL_OUT					16
#define			EE_LOCK_FLAG						(EE_OBL_BASE_ADDR+0x70)		// 2B��������־
#define			EE_FORCE_LOCK_FLAG					(EE_OBL_BASE_ADDR+0x72)		// 2B, 
#define			EE_CONTROL_OUT1						(EE_OBL_BASE_ADDR+0x74)		// 2B, ������ƣ�
#define			EE_CONTROL_OUT2						(EE_OBL_BASE_ADDR+0x76)		// 2B, �������2
#define			EE_CONTROL_OUT3						(EE_OBL_BASE_ADDR+0x78)		// 2B, �������3
#define			EE_CONTROL_OUT4						(EE_OBL_BASE_ADDR+0x7a)		// 2B, �������3
#define			EE_CONTROL_OUT5						(EE_OBL_BASE_ADDR+0x7c)		// 2B, �������4
#define			EE_CONTROL_OUT_RSV					(EE_OBL_BASE_ADDR+0x7e)		// 2B, 


#define			EE_ALL_DRIVER_INFO					(EE_OBL_BASE_ADDR+0x80)		// ȫ����ʻԱ�����Ϣ
#define			EE_DRIVER_ID						(EE_OBL_BASE_ADDR+0x80)		// 16B,4����ʻԱ��	
#define			EE_DRIVER_INFO						(EE_OBL_BASE_ADDR+0x90)		// 16B,���ṹDRIVER_INFO
#define			EE_DRIVER_INFO_1					(EE_OBL_BASE_ADDR+0xa0)		// 16B,���ṹDRIVER_INFO
#define			EE_DRIVER_INFO_2					(EE_OBL_BASE_ADDR+0xb0)		// 16B,���ṹDRIVER_INFO
#define			EE_DRIVER_INFO_3					(EE_OBL_BASE_ADDR+0xc0)		// 16B,���ṹDRIVER_INFO
#define			EE_TIRED_DRIVE						(EE_OBL_BASE_ADDR+0xd0)		// 8B�����ṹTIRE_DRIVE

#define			EE_BAUDRATE_U0_232					(EE_OBL_BASE_ADDR+0xe0)		// 4B
#define			EE_BAUDRATE_U2_232					(EE_OBL_BASE_ADDR+0xe4)		// 4B
#define			EE_BAUDRATE_U2_485					(EE_OBL_BASE_ADDR+0xe8)		// 4B
#define			EE_BAUDRATE_CAN_BUS					(EE_OBL_BASE_ADDR+0xec)		// 4B
//#define			EE_BAUDRATE_U1_GSM				�����޸�
//#define			EE_BAUDRATE_U0_GPS				�����޸�
#define			EE_BAUDRATE_U1_232					(EE_OBL_BASE_ADDR+0xf0)	    // 4B	 ������U1��	���ڲ���
#define			EE_BAUDRATE_U3_232					(EE_OBL_BASE_ADDR+0xf4)		// 4B   	 // GNSS ��λʹ��


#define			EE_GPRS_DISP_PROTOCOL				(EE_OBL_BASE_ADDR+0xf8)		//  8B


/////////////// ///////////////////////////////

#define			EE_HANDLE_WATCH_CAR_PASSWORD		(EE_OBL_BASE_ADDR+0x100)	   // 8B,�ֱ���������
#define			EE_HANDLE_FACTORY_PASSWORD			(EE_OBL_BASE_ADDR+0x108)		// 8B,�ֱ��ĳ�����������

#define			EE_ENABLE_HANDLE_WATCH_CAR			(EE_OBL_BASE_ADDR+0x110)		// 1B,�Ƿ�ʹ���ֱ�����
#define			EE_HANDLE_WATCH_CAR_MODE			(EE_OBL_BASE_ADDR+0x111)		// 1B,�ֱ�������ʽ���Զ�/�ֶ���
#define			EE_DIAL_ANSWER_ENABLE_FLAG			(EE_OBL_BASE_ADDR+0x112)		// 1B,����ͨ����־λ(��0λ:���;��1λ:����.)
#define			EE_SET_SCAN_FLAG					(EE_OBL_BASE_ADDR+0x113)		// 1B

// 12 Bytes left

#define			EE_PARK_OVER_TIME						(EE_OBL_BASE_ADDR+0x120)		//  2B,ͣ����ʱ����ʱ��
#define			EE_USE_PARK_OVER_TIME				(EE_OBL_BASE_ADDR+0x121)		//  1B,ʹ��ͣ����ʱ���

#define			EE_SYSTEM_ON_IDLE_MODE_FLAG			(EE_OBL_BASE_ADDR+0x129)		//  1B
																					  
#define			EE_ENABLE_FLAG						(EE_OBL_BASE_ADDR+0x12A)		//  4B���꿴Enableȫ�ֱ����ļ���������

// 2 Bytes left

/////////////// ʡ����ر�־�Ĵ洢��ַ�������ռ�
#define			LEN_EE_IDLE								8			//
#define			EE_IDLE_STR							(EE_OBL_BASE_ADDR+0x130)		// 
#define			EE_ENABLE_INTER_IDLE_MODE 			(EE_OBL_BASE_ADDR+0x130)		// 1B,�������ʡ��ģʽ
#define			EE_IDLE_FLAG						(EE_OBL_BASE_ADDR+0x131)		// 1B
#define			EE_INTER_ILDE_TIME					(EE_OBL_BASE_ADDR+0x132)		// 2B,����ʱ������ʡ��ģʽ
#define			EE_WAKE_UP_TIME						(EE_OBL_BASE_ADDR+0x134)	   // 2B,����ʱ�����
#define			EE_WAKE_UP_LONG_TIME				(EE_OBL_BASE_ADDR+0x136)		// 2B,���Ѻ����ʱ��
#define			EE_WAKE_UP_TYPE						(EE_OBL_BASE_ADDR+0x138)		// 1B,����ԭ��


																				  
#define			EE_HIGH_LOW_LEVEL					(EE_OBL_BASE_ADDR+0x160)		// 4B,�ߵ͵�ƽʶ��
#define			EE_POWER_LOW_ALARM_VALUE			(EE_OBL_BASE_ADDR+0x164)		// 4B
#define			EE_VEHICLE_STATUS					(EE_OBL_BASE_ADDR+0x168)		// 4B,����״̬ 
#define			EE_VEHICLE_STATUS1					(EE_OBL_BASE_ADDR+0x16c)		// 4B,����״̬ 

#define			EE_PULSE_FACTOR						(EE_OBL_BASE_ADDR+0x170)		// 4B,����ϵ��

#define			EE_TEST_MILEAGE					(EE_OBL_BASE_ADDR+0x178)
#define			EE_TEST_GPS_MILEAGE				(EE_OBL_BASE_ADDR+0x178)			// �������
#define			EE_TEST_PULS_MILEAGE				(EE_OBL_BASE_ADDR+0x17C)			// �������
// 6 Bytes left

#define			EE_PIN_CODE							(EE_OBL_BASE_ADDR+0x188)		// 8B

#define			EE_SERVER_LOGIN_USER				(EE_OBL_BASE_ADDR+0x190)		// 10B
#define			EE_SERVER_LOGIN_PASSWORD			(EE_OBL_BASE_ADDR+0x19a)		// 6B

//#define			EE_HEARTBEAT_TIME					(EE_OBL_BASE_ADDR+0x1a0)		// ���������ͼ�� 4B

#define			EE_SEND_INTERVAL_BASE					(EE_OBL_BASE_ADDR+0x1A0)		// 
#define			EE_JT_HEART_BEAT_TIME					(EE_OBL_BASE_ADDR+0x1A0)		// 4B ���������ͼ��
#define			EE_JT_SEND_TIME_NO_DRIVER	      	(EE_OBL_BASE_ADDR+0x1A4)		// 4B �޼�ʻԱʱ�㱨ʱ����
#define			EE_JT_SEND_TIME_IDEL     	      	(EE_OBL_BASE_ADDR+0x1A8)		// 4B ����ʱ�㱨ʱ����
#define			EE_JT_SEND_TIME_ALARM		 	   	(EE_OBL_BASE_ADDR+0x1AC)		// 4B ��������ʱʱ��㱨���
#define			EE_JT_SEND_TIME_DEFAULET         	(EE_OBL_BASE_ADDR+0x1B0)		// 4B ȱʡʱ��㱨���
#define			EE_JT_SEND_DISTANCE_DEFAULET     	(EE_OBL_BASE_ADDR+0x1B4)		// 4B ȱʡ����㱨���
#define			EE_JT_SEND_DISTANCE_NO_DRIVER	   	(EE_OBL_BASE_ADDR+0x1B8)		// 4B �޼�ʻԱʱ�㱨������
#define			EE_JT_SEND_DISTANCE_IDEL         	(EE_OBL_BASE_ADDR+0x1BC)		// 4B ����ʱ�㱨������
#define			EE_JT_SEND_DISTANCE_ALARM	      	(EE_OBL_BASE_ADDR+0x1C0)		// 4B ��������ʱ����㱨���
#define			EE_JT_SEND_INTERVAL_MODE			   (EE_OBL_BASE_ADDR+0x1C4)		// 1B λ�û㱨����
#define			EE_JT_SEND_INTERVAL_METHOD		   	(EE_OBL_BASE_ADDR+0x1C5)		// 1B λ�û㱨����
#define			EE_JT_SEND_ANGLE	               	(EE_OBL_BASE_ADDR+0x1C6)		// 1B �յ㲹���Ƕ�
#define			EE_JT_SEND_RSV_1BYTE						(EE_OBL_BASE_ADDR+0x1C7)
#define			EE_JT_SEND_T_SECOND						(EE_OBL_BASE_ADDR+0x1C8)
#define			EE_JT_SEND_T_CONTINUED					(EE_OBL_BASE_ADDR+0x1CC)

#define			EE_JT_SHIELD_WORD	               	(EE_OBL_BASE_ADDR+0x1D3)		// 4B ����������
#define			EE_JT_TEXT_SMS_WORD	            	(EE_OBL_BASE_ADDR+0x1D7)		// 4B �����ı�����SMS����
#define			EE_JT_TAKE_PHOTO_WORD	         	(EE_OBL_BASE_ADDR+0x1DB)		// 4B �������տ���
#define			EE_JT_SAVE_PHOTO_WORD	         	(EE_OBL_BASE_ADDR+0x1DF)		// 4B ���汨��������Ƭ����
#define			EE_JT_KEY_WORD	                 	(EE_OBL_BASE_ADDR+0x1E3)		// 4B �ؼ�������ʶ

#define			EE_JT_VIDIO_QUALITY	            	(EE_OBL_BASE_ADDR+0x1E7)		// 1B ��Ƶ/��Ƭ����
#define			EE_JT_VIDIO_LIGHT	              	(EE_OBL_BASE_ADDR+0x1E8)		// 1B ��Ƶ/��Ƭ����
#define			EE_JT_VIDIO_CONTRAST	            (EE_OBL_BASE_ADDR+0x1E9)		// 1B ��Ƶ/��Ƭ�Աȶ�
#define			EE_JT_VIDIO_SATULATION	         	(EE_OBL_BASE_ADDR+0x1EA)		// 1B ��Ƶ/��Ƭ���Ͷ�
#define			EE_JT_VIDIO_CALOUR	            	(EE_OBL_BASE_ADDR+0x1EB)		// 1B ��Ƶ/��Ƭɫ��

// 14 Bytes left

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	��ַ0x200~0x2ffΪIP�������洢��
*/
#define			EE_IP_PORT							(EE_OBL_BASE_ADDR+0x200)		// 4��IP��ַ��128B
#define			EE_IP_PORT_TCP						(EE_OBL_BASE_ADDR+0x200)		// 32B,IP��ַ	,������1 TCPͨ��
#define			EE_IP_PORT_UDP						(EE_OBL_BASE_ADDR+0x220)		// 32B,IP��ַ1	,������1 UDPͨ��
#define			EE_IP_PORT_TCP1						(EE_OBL_BASE_ADDR+0x240)		// 32B,IP��ַ2	,������2 TCPͨ��	
#define			EE_IP_PORT_UDP1						(EE_OBL_BASE_ADDR+0x260)		// 32B,IP��ַ2 ,������2 UDPͨ��
#define			EE_DOMAIN_ADDR						(EE_OBL_BASE_ADDR+0x280)		// 32B,�������������ַ
#define			EE_APN								(EE_OBL_BASE_ADDR+0x2a0)		// 16B,ANP
#define			EE_CENTER_NUMBER					(EE_OBL_BASE_ADDR+0x2b0)		// 16B,������ķ���绰����0
#define			EE_CENTER_NUMBER1					(EE_OBL_BASE_ADDR+0x2c0)		// 16B,������ķ���绰����1
#define			EE_USER_NUMBER						(EE_OBL_BASE_ADDR+0x2d0)		// 16B,�û������绰����0
#define			EE_USER_NUMBER1						(EE_OBL_BASE_ADDR+0x2e0)		// 16B,�û������绰����1
#define			EE_DOMAIN_PORT						(EE_OBL_BASE_ADDR+0x2f0)		// 5B, ����������IP�˿ںţ�
                                                  
#define			EE_ENABLE_IP						(EE_OBL_BASE_ADDR+0x2f8)		// 16B,����16�ֽ�
#define			EE_ENABLE_TCP						(EE_OBL_BASE_ADDR+0x2f8)		// 1B, 
#define			EE_ENABLE_UDP						(EE_OBL_BASE_ADDR+0x2f9)		// 1B, 
#define			EE_ENABLE_TCP1						(EE_OBL_BASE_ADDR+0x2fa)		// 1B, 
#define			EE_ENABLE_UDP1						(EE_OBL_BASE_ADDR+0x2fb)		// 1B, 
#define			EE_ENABLE_TCP2						(EE_OBL_BASE_ADDR+0x2fc)		// 1B, 
#define			EE_ENABLE_UDP2						(EE_OBL_BASE_ADDR+0x2fd)		// 1B, ����ʹ�ܱ�־
#define			EE_ENABLE_APN						(EE_OBL_BASE_ADDR+0x2fe)		// 1B, APNʹ�ܱ�־
//#define			EE_NEED_REG_SERVER					    (EE_OBL_BASE_ADDR+0x3FF)		// 1B, ��Ҫע��
#define			EE_NEED_REG_SERVER					(EE_OBL_BASE_ADDR+0x2FF)		// 1B, ��Ҫע��

#define			EE_ENABLE_IC_AUTH_TCP				(EE_OBL_BASE_ADDR+0x300)		// 1B, 
#define			EE_ENABLE_IC_AUTH_UDP				(EE_OBL_BASE_ADDR+0x301)		// 1B, 
#define			EE_ENABLE_IC_AUTH_B_TCP				(EE_OBL_BASE_ADDR+0x302)		// 1B, 
#define			EE_ENABLE_IC_AUTH_B_UDP				(EE_OBL_BASE_ADDR+0x303)		// 1B, 

//#define			EE_JT_ENABLE_M_DNS					(EE_OBL_BASE_ADDR+0x300)		// 1B,  ��������
//#define			EE_JT_ENABLE_M_DOMAIN				(EE_OBL_BASE_ADDR+0x301)		// 1B, ����ʹ�ܱ�
//#define			EE_JT_ENABLE_B_DNS					(EE_OBL_BASE_ADDR+0x302)		// 1B,  ���ݷ�����
//#define			EE_JT_ENABLE_B_DOMAIN				(EE_OBL_BASE_ADDR+0x303)		// 1B, ����ʹ�ܱ�
//#define			EE_JT_ENABLE_C_DNS					(EE_OBL_BASE_ADDR+0x304)		// 1B, ��ʱ������������
//#define			EE_JT_ENABLE_C_DOMAIN				(EE_OBL_BASE_ADDR+0x305)		// 1B, ����ʹ�ܱ�

#define			   EE_CREATE_USER_FLAG				 (EE_OBL_BASE_ADDR+0x304)			// 1B, ʹ��ǰ���������ٿ�����

#define			   EE_OVERTIME_DRIVE_UPDATE_FLAG	 (EE_OBL_BASE_ADDR+0x306) //	 1B // ��ʱ��ʻ���±�־ 0,���³�ʱ������1�������¼���
// 10 Bytes left



#define			EE_SIM_NUMBER						(EE_OBL_BASE_ADDR+0x310)		// 16B,����SIM���ֻ�����
#define			EE_HELP_NUMBER						(EE_OBL_BASE_ADDR+0x320)		// 16B,��������
#define			EE_SMS_CENTER_NUMBER				(EE_OBL_BASE_ADDR+0x330)		// 16B,�ƶ���˾����Ϣ���ĺ���

// ���ٶȴ���������
#define			EE_ACCEL_PARA_BASE					(EE_OBL_BASE_ADDR+0x340)		// ���ٶȴ��в�������ַ
#define			EE_ACCEL_RESOLUTION					(EE_ACCEL_PARA_BASE+0x00)		// �ֱ���	 1B
#define			EE_ACCEL_G_RANG						(EE_ACCEL_PARA_BASE+0x01)		// �������ٶȷ�Χ	 1B
#define			EE_ACCEL_TILT_PARA					(EE_ACCEL_PARA_BASE+0x02)		// ��б����	 5B
#define			EE_ACCEL_WUF_PARA					(EE_ACCEL_PARA_BASE+0x07)		// �˶����ƶ�������	 4B
#define			EE_ACCEL_TAP_PARA					(EE_ACCEL_PARA_BASE+0x0b)		// ��ײ����ߵ��˫ߵ������	 9B
#define			EE_ACCEL_MASK_PARA					(EE_ACCEL_PARA_BASE+0x14)		// ���β�������б����ײ���������Լ��ƶ���������������Σ�  4B
#define			EE_ACCEL_INT_PARA					(EE_ACCEL_PARA_BASE+0x18)		// �ж����ò���	 4B

// 24 Bytes left

// www.guangzhouguoyedianzi.com.cn
	
#define			EE_HEARTBEAT_PACK					(EE_OBL_BASE_ADDR+0x380)		// 64B,����������
//#define			EE_REGISTER_PACK					(EE_OBL_BASE_ADDR+0x3c0)		// 64B,ע�������

#define			LEN_EE_ACCEL_ADD_DEC_PARA		30
#define			EE_ACCEL_ADD_DEC_PARA			(EE_OBL_BASE_ADDR+0x3c0)	  	// 18*2 = 36B



#define			EE_CAN_SPECIFY_ID					(EE_OBL_BASE_ADDR+0x400)		// 16*4B��ָ����16��CAN����֡ID
#define			EE_CAN_ID_NUMBER					(EE_OBL_BASE_ADDR+0x440)		// 4B��ID����
#define			EE_CAN_GET_INTERVAL					(EE_OBL_BASE_ADDR+0x444)		// 4B��

 

// add 2013.3
#define			EE_JT_ELE_RAID_RADIUS				(EE_OBL_BASE_ADDR+0x446)	 // ����Χ���뾶	���Ƿ�λ����ֵ�� 2B
#define			EE_JT_OVER_SPEED_PRE_ALARM			(EE_OBL_BASE_ADDR+0x448)	 // ���ٱ���Ԥ����ֵ 				 2B
#define			EE_JT_FATIGUE_DRIVE_PRE_ALARM       (EE_OBL_BASE_ADDR+0x44A)	 	 // ƣ�ͼ�ʻԤ����ֵ				 2B
#define			EE_JT_CRASH_ALARM_PARA				(EE_OBL_BASE_ADDR+0x44C)	 // ��ײ������������				 2B
#define			EE_JT_TILT_ALARM_PARA				(EE_OBL_BASE_ADDR+0x44E)	 // �෭������������				 2B

#define			EE_JT_FIXED_TIME_TAKE_PHOTO_CTL		(EE_OBL_BASE_ADDR+0x450)	 // ��ʱ���տ���					 4B
#define			EE_JT_DISTANCE_TIME_TAKE_PHOTO_CTL	(EE_OBL_BASE_ADDR+0x454)	 // �������տ���					 4B

#define			EE_JT_GNNS_POS_MODE					(EE_OBL_BASE_ADDR+0x458)	 // GNSS ��λģʽ					 1B
#define			EE_JT_GNSS_BAUD_RATE 				(EE_OBL_BASE_ADDR+0x459)	 // GNSS������						 1B
#define			EE_JT_GNSS_DATA_OUTPUT_FREQUENCY 	(EE_OBL_BASE_ADDR+0x45A)	 // GNSSģ����ϸ��λ�������Ƶ��	 1B
#define			EE_JT_GNSS_DATA_COLLECT_FREQUENCY 	(EE_OBL_BASE_ADDR+0x45B)	 // GNSSģ����ϸ��λ���ݲɼ�Ƶ��	 4B
#define			EE_JT_GNSS_DATA_UPLOAD_MODE 		(EE_OBL_BASE_ADDR+0x45F)	 // GNSSģ����ϸ��λ�����ϴ���ʽ	 1B
#define			EE_JT_GNSS_DATA_UPLOAD_SET 			(EE_OBL_BASE_ADDR+0x460)     // GNSSģ����ϸ��λ�����ϴ�����	 4B

#define		EE_JT_CAN_CHAN_1_COLLECT_TIME_INTERVAL  (EE_OBL_BASE_ADDR+0x464) 	 // CAN����ͨ��1 �ɼ�ʱ����(ms)��0��ʾ���ɼ� 4B
#define		EE_JT_CAN_CHAN_1_UPLOAD_TIME_INTERVAL 	(EE_OBL_BASE_ADDR+0x468)     // CAN����ͨ��1 �ϴ�ʱ����(s)��0��ʾ���ɼ� 2B
#define		EE_JT_CAN_CHAN_2_COLLECT_TIME_INTERVAL 	(EE_OBL_BASE_ADDR+0x46A)	 // CAN����ͨ��2 �ɼ�ʱ����(ms)��0��ʾ���ɼ� 4B
#define		EE_JT_CAN_CHAN_2_UPLOAD_TIME_INTERVAL 	(EE_OBL_BASE_ADDR+0x46E)     // CAN����ͨ��2 �ϴ�ʱ����(s)��0��ʾ���ɼ� 2B

		

// 17 Bytes left

#define			EE_PICTURE_TOTAL_NUM				(EE_OBL_BASE_ADDR+0x480)		// 4B+4B	������4B,�ֱ���1B,�Աȶ�1B,���1B,1B

#define		EE_MANUL_ALARM_EVENT_ID				(EE_OBL_BASE_ADDR+0x4F0)		//  �˹������¼�ID, 2B

// add by xzb			���� �ն�ע��������Ϣ
#define 		EE_PROVINCE_REGION_ID				(EE_OBL_BASE_ADDR+0x500)      // ʡ��ID	    2B
#define 		EE_PREFECTURE_REGION_ID			   (EE_OBL_BASE_ADDR+0x502)      // ������ID	    2B
#define 		EE_VENDOR_ID							(EE_OBL_BASE_ADDR+0x504)      // ������ID		 5B
#define 		EE_TERMINAL_TYPE						(EE_OBL_BASE_ADDR+0x509)		// �ն��ͺ�		 20B
#define 		EE_TERMINAL_ID							(EE_OBL_BASE_ADDR+0x51D)		// �ն�ID		 7B
#define		FM_CAR_COLOUR			   			(EE_OBL_BASE_ADDR+0x524)		  // ������ɫ	   1B
#define		FM_VEHICLE_CODE						(EE_OBL_BASE_ADDR+0x525)		  // ���ƺ���	   12B 
#define		FM_VEHICLE_VIN							(EE_OBL_BASE_ADDR+0x531)		  // ����VIN��	  17B
#define		FM_VEHICLE_CLASS						(EE_OBL_BASE_ADDR+0x542)		  // ��������	   12B


// 2 Bytes left

#define			EE_AUTH_CODE						(EE_OBL_BASE_ADDR+0x550)      // ��Ȩ��       32B




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	JT808  ��������
*/
#define			EE_JT_MAIN_SERVER				    (EE_OBL_BASE_ADDR+0x570)	   // 96B �����������Ӳ���
#define			EE_JT_M_APN						   	(EE_OBL_BASE_ADDR+0x570)		// 32B ������������APN
#define			EE_JT_M_USER						(EE_OBL_BASE_ADDR+0x590)		// 16B �������������û���
#define			EE_JT_M_SERIERS						(EE_OBL_BASE_ADDR+0x5A0)		// 8B ����������������
#define			EE_JT_M_IP_DNS					   	(EE_OBL_BASE_ADDR+0x5A8)		// 32B IP��������ַ��DNS��

#define			EE_JT_BACK_SERVER				    (EE_OBL_BASE_ADDR+0x5C8)		// 96B ���ݷ��������Ӳ���
#define			EE_JT_B_APN						   	(EE_OBL_BASE_ADDR+0x5C8)		// 32B ���ݷ���������APN
#define			EE_JT_B_USER						(EE_OBL_BASE_ADDR+0x5E8)		// 16B ���ݷ����������û���
#define			EE_JT_B_SERIERS						(EE_OBL_BASE_ADDR+0x5F8)		// 8B ���ݷ�������������
#define			EE_JT_B_IP_DNS					   	(EE_OBL_BASE_ADDR+0x600)		// 32B ���ݷ�����IP��ַ��DNS

#define			EE_JT_TCP_PORT					   	(EE_OBL_BASE_ADDR+0x620)		// 4B ������TCP�˿�
#define			EE_JT_UDP_PORT					   	(EE_OBL_BASE_ADDR+0x624)		// 4B ������UDP�˿�


#define			EE_JT_TMP_TCP_IP					(EE_OBL_BASE_ADDR+0x628)	   // 20B	�����ʱIP	 // added 2013.4
#define			EE_JT_TMP_TCP_PORT					(EE_OBL_BASE_ADDR+0x63C)	   // 4B	�����ʱ�˿�

#define			EE_JT_CENTER_NUMBER			   		(EE_OBL_BASE_ADDR+0x640)		// 16B ƽ̨�绰����
#define			EE_JT_RESET_NUMBER					(EE_OBL_BASE_ADDR+0x650)		// 16B ��λ����
#define			EE_JT_RESET_FACTORY_NUMBER	   		(EE_OBL_BASE_ADDR+0x660)		// 16B �ָ��������ú���
#define			EE_JT_CENTER_SMS_NUMBER		   		(EE_OBL_BASE_ADDR+0x670)		// 16B ƽ̨���ź���
#define			EE_JT_TEXT_SMS_NUMBER				(EE_OBL_BASE_ADDR+0x680)		// 16B ����SMS�ı���������
#define			EE_JT_GET_CALL_MODE				   	(EE_OBL_BASE_ADDR+0x690)		// 1B �绰����1����
#define			EE_JT_CALL_TIME_LIMIT				(EE_OBL_BASE_ADDR+0x691)		// 4B ÿ��ͨ��ʱ������
#define			EE_JT_CALL_TIME_LIMIT_MONTH			(EE_OBL_BASE_ADDR+0x695)		// 4B ÿ��ͨ��ʱ������
#define			EE_JT_JT_NUMBER						(EE_OBL_BASE_ADDR+0x699)		// 16B �����绰����
#define			EE_JT_PRIVILILEGE_SMS_NUMBER   		(EE_OBL_BASE_ADDR+0x6A9)		// 16B ���ƽ̨��Ȩ���ź���

// 7 Bytes  left

#define			EE_JT_RESEND_MACHINING				(EE_OBL_BASE_ADDR+0x6C0)			// 4B �ش�����
#define			EE_JT_TCP_ACK_TIME				    (EE_OBL_BASE_ADDR+0x6C0)			// 4B TCPӦ��ʱʱ��
#define			EE_JT_TCP_RESEND_TIMES			    (EE_OBL_BASE_ADDR+0x6C4)			// 4B TCP�ش�����
#define			EE_JT_UDP_ACK_TIME     	         	(EE_OBL_BASE_ADDR+0x6C8)		// 4B UDPӦ��ʱʱ��
#define			EE_JT_UDP_RESEND_TIMES		   		(EE_OBL_BASE_ADDR+0x6CC)		// 4B UDP�ش�����
#define			EE_JT_SMS_ACK_TIME     	         	(EE_OBL_BASE_ADDR+0x6D0)		// 4B SMSӦ��ʱʱ��
#define			EE_JT_SMS_RESEND_TIMES		   		(EE_OBL_BASE_ADDR+0x6D4)		// 4B SMS�ش�����

// 14 Bytes left 
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	JT808  �ն����ӿ���
*/
#define			EE_JT_NET_CTR				        (EE_OBL_BASE_ADDR+0x6F0)	   // 1B ���ӿ���
#define			EE_JT_NET_CTR_APN					(EE_OBL_BASE_ADDR+0x6F1)		// 32B ������ָ������������APN
#define			EE_JT_NET_CTR_USER					(EE_OBL_BASE_ADDR+0x711)		// 16B ������ָ�������������û���
#define			EE_JT_NET_CTR_SERIERS				(EE_OBL_BASE_ADDR+0x721)		// 8B ������ָ����������������
#define			EE_JT_NET_CTR_IP_DNS				(EE_OBL_BASE_ADDR+0x729)		// 32B ������ָ��������IP��ַ��DNS�� 
#define			EE_JT_NET_CTR_TCP_PORT			  	(EE_OBL_BASE_ADDR+0x749)		// 2B ������ָ��������TCP�˿�
#define			EE_JT_NET_CTR_UDP_PORT			 	(EE_OBL_BASE_ADDR+0x74B)		// 2B ������ָ��������UDP�˿�
#define			EE_JT_NET_CTR_CPY_ID				(EE_OBL_BASE_ADDR+0x74D)	   // 5B ������ID
#define			EE_JT_NET_CTR_JQM					(EE_OBL_BASE_ADDR+0x752)		// 16B ָ���������ļ�Ȩ��
#define			EE_JT_NET_CTR_HVS					(EE_OBL_BASE_ADDR+0x762)		// 16B Ӳ���汾��
#define			EE_JT_NET_CTR_HWVS				    (EE_OBL_BASE_ADDR+0x772)		// 16B �̼��汾��
#define			EE_JT_NET_CTR_URL_ADDR			  	(EE_OBL_BASE_ADDR+0x782)		// 32B URL��ַ 
#define			EE_JT_NET_CTR_NET_TIME			  	(EE_OBL_BASE_ADDR+0x7A2)		// 2B ����ʱ��			  ��λ����
 
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	JT808  λ�ø��ٿ���
*/
#define			EE_JT_PF_TIME				         (EE_OBL_BASE_ADDR+0x7A4)	   // 2B ����ʱ����
#define			EE_JT_PF_TIME_LIMIT		             (EE_OBL_BASE_ADDR+0x7A6)	   // 4B ����ʱ��

//#define	      EE_JT_ADSW_SET							(EE_OBL_BASE_ADDR+0x7AA)		// 2B AD����ͨ�� ѡ���/�Ϳ��������

#define			EE_JT_IC_AUTH_M_IP_DNS				(EE_OBL_BASE_ADDR+0x7AA)		// 32B IC����֤��IP��������ַ��DNS
#define			EE_JT_IC_AUTH_B_IP_DNS				(EE_OBL_BASE_ADDR+0x7CA)		// 32B IC����֤����IP��������ַ��DNS
#define			EE_JT_IC_AUTH_TCP_PORT				(EE_OBL_BASE_ADDR+0x7EA)		// 4B ������TCP�˿�
#define			EE_JT_IC_AUTH_UDP_PORT				(EE_OBL_BASE_ADDR+0x7EE)		// 4B ������UDP�˿�

// 14 Bytes left




#define 			LEN_PROVINCE_REGION_ID				2    // ʡ��ID����
#define 			LEN_PREFECTURE_REGION_ID			2    // ������ID����
#define 			LEN_EE_VENDOR_ID					5	  // ������ID����		 5B




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	ÿ���Ӵ洢������,�൱��ϵͳ��־,�Է���ϵͳά��,���Ϸ���. ռ��256�ֽ� 
*/
#define			EE_SYSTEM_LOG_BASE					(EE_OBL_BASE_ADDR+0x800)		// 32+32+32+32+32	�������£����ṹSystemLog (ϵͳ��־)

#define			EE_SYSTEM_LOG_GPRS_BASE				 EE_SYSTEM_LOG_BASE
#define			EE_SYS_GPRS_DIAL_CNT					(EE_SYSTEM_LOG_GPRS_BASE+0*4)			// GPRS
#define			EE_SYS_GPRS_DIAL_OK					(EE_SYSTEM_LOG_GPRS_BASE+1*4)
#define			EE_SYS_GPRS_ONLINE_TIME				(EE_SYSTEM_LOG_GPRS_BASE+2*4)
#define			EE_SYS_GPRS_REC_TOTAL_PACK			(EE_SYSTEM_LOG_GPRS_BASE+3*4)
#define			EE_SYS_GPRS_REC_TOTAL_BYTES		(EE_SYSTEM_LOG_GPRS_BASE+4*4)
#define			EE_SYS_GPRS_PUSH_TOTAL_BYTES		(EE_SYSTEM_LOG_GPRS_BASE+5*4)
#define			EE_SYS_GPRS_SEND_TOTAL_BYTES		(EE_SYSTEM_LOG_GPRS_BASE+6*4)
#define			EE_SYS_GPRS_SEND_TOTAL_PACK		(EE_SYSTEM_LOG_GPRS_BASE+7*4)

#define			EE_SYSTEM_LOG_GSM_BASE				(EE_SYSTEM_LOG_GPRS_BASE+32)			// GSM
#define			EE_SYS_GSM_COOL_BOOT					(EE_SYSTEM_LOG_GSM_BASE+0*4)
#define			EE_SYS_GSM_SIM_ERROR					(EE_SYSTEM_LOG_GSM_BASE+1*4)
#define			EE_SYS_GSM_UNREG						(EE_SYSTEM_LOG_GSM_BASE+2*4)
#define			EE_SYS_GSM_EBB_SIGNAL				(EE_SYSTEM_LOG_GSM_BASE+3*4)
#define			EE_SYS_GSM_REC_SMS					(EE_SYSTEM_LOG_GSM_BASE+4*4)
#define			EE_SYS_GSM_SEND_SMS					(EE_SYSTEM_LOG_GSM_BASE+5*4)
#define			EE_SYS_GSM_REG_SERVER				(EE_SYSTEM_LOG_GSM_BASE+6*4)
#define			EE_SYS_GSM_REG_SERVER_OK			(EE_SYSTEM_LOG_GSM_BASE+7*4)

#define			EE_SYSTEM_LOG_GG_BASE				(EE_SYSTEM_LOG_GSM_BASE+32)			// GG
#define			EE_SYS_GG_CALLER_TIMES				(EE_SYSTEM_LOG_GG_BASE+0*4)
#define			EE_SYS_GG_CALLER_TIME				(EE_SYSTEM_LOG_GG_BASE+1*4)
#define			EE_SYS_GG_CALLED_TIMES				(EE_SYSTEM_LOG_GG_BASE+2*4)
#define			EE_SYS_GG_CALLED_TIME				(EE_SYSTEM_LOG_GG_BASE+3*4)
#define			EE_SYS_GG_SAVE_POS_INDEX			(EE_SYSTEM_LOG_GG_BASE+4*4)
#define			EE_SYS_GG_SEND_POS_INDEX			(EE_SYSTEM_LOG_GG_BASE+5*4)
#define			EE_SYS_GG_SAVE_ALARM_INDEX			(EE_SYSTEM_LOG_GG_BASE+6*4)
#define			EE_SYS_GG_SEND_ALARM_INDEX			(EE_SYSTEM_LOG_GG_BASE+7*4)

#define			EE_SYSTEM_LOG_GPS_BASE				(EE_SYSTEM_LOG_GG_BASE+32)				// GPS
#define			EE_SYS_GPS_SPEED_CALCULUS			(EE_SYSTEM_LOG_GPS_BASE+0*4)			// 8B, �ٶȻ���ֵ(0.01km)
#define			EE_SYS_GPS_TOTAL_MILEAGE			(EE_SYSTEM_LOG_GPS_BASE+2*4)			// 4B, �����(����)
#define			EE_SYS_GPS_PACK_TOTAL_NUMBER		(EE_SYSTEM_LOG_GPS_BASE+3*4)			// 4B��GPS�������
#define			EE_SYS_GPS_PACK_VALID_NUMBER		(EE_SYSTEM_LOG_GPS_BASE+4*4)			// 4B, �����λ������
#define			EE_SYS_GPS_RESTART_COUNT       	(EE_SYSTEM_LOG_GPS_BASE+5*4)			// 4B, GPSģ����������
#define			EE_SYS_GPS_VALID_TIME				(EE_SYSTEM_LOG_GPS_BASE+6*4)			// 4B����Ч��λʱ��
#define			EE_SYS_GPS_DATA_ERROR_COUNT		(EE_SYSTEM_LOG_GPS_BASE+7*4)			// 4B, GPS���ݴ������


#define			EE_SYSTEM_LOG_VDR_BASE				(EE_SYSTEM_LOG_GPS_BASE+32)			// VDR
#define			EE_SYS_VDR_HIGH_SPEED_PULS_CNT	(EE_SYSTEM_LOG_VDR_BASE+0*4)
#define			EE_SYS_VDR_SPEED_PULS_CNT			(EE_SYSTEM_LOG_VDR_BASE+1*4)
#define			EE_SYS_VDR_MOTOR_PULS_CNT			(EE_SYSTEM_LOG_VDR_BASE+2*4)
#define			EE_SYS_VDR_MOTOR_RUN_TIME			(EE_SYSTEM_LOG_VDR_BASE+3*4)
#define			EE_SYS_VDR_SYS_WORK_TIME			(EE_SYSTEM_LOG_VDR_BASE+4*4)		
#define			EE_SYS_VDR_SYS_LAST_RUN_TIME		(EE_SYSTEM_LOG_VDR_BASE+5*4)			
#define			EE_SYS_VDR_POWER1_2					(EE_SYSTEM_LOG_VDR_BASE+6*4)			// ����Դ1�����õ�Դ2
#define			EE_SYS_VDR_POWER3_4					(EE_SYSTEM_LOG_VDR_BASE+7*4)		   // ���õ�Դ3�� ���õ�Դ4


#define			EE_SYSTEM_LOG_RESET_BASE			(EE_SYSTEM_LOG_VDR_BASE+32)			// ResetCnt
#define			EE_SYS_RESET_SYSTEM_CNT	         (EE_SYSTEM_LOG_RESET_BASE+0*4)	   // ϵͳ�����ܴ���
#define			EE_SYS_RESET_UPDATE_CNT			   (EE_SYSTEM_LOG_RESET_BASE+1*4)		// ����������������
#define			EE_SYS_RESET_GPRS_CNT				(EE_SYSTEM_LOG_RESET_BASE+2*4)		// GPRS������������
#define			EE_SYS_RESET_TASK_OVERTIME_CNT	(EE_SYSTEM_LOG_RESET_BASE+3*4)		// ����ʱ��������
#define			EE_SYS_RESET_POWER_ON_CNT			(EE_SYSTEM_LOG_RESET_BASE+4*4)		// �ϵ���������
#define			EE_SYS_RESET_WATCHDOG_CNT			(EE_SYSTEM_LOG_RESET_BASE+5*4)	   // ���Ź���������
#define			EE_SYS_RESET_WAKEUP_CNT				(EE_SYSTEM_LOG_RESET_BASE+6*4)		// ���ߺ�����������
#define			EE_SYS_RESET_PIN_RESET_CNT			(EE_SYSTEM_LOG_RESET_BASE+7*4)		// ��λ�ſ��Ƶ�����


#define			EE_SYSTEM_LOG_GG1_BASE				(EE_SYSTEM_LOG_RESET_BASE+32)			// GG1
#define			EE_SYS_GG1_LON_VAL	         	(EE_SYSTEM_LOG_GG1_BASE+0*4)			// 4B�����ȣ��Ŵ���10e7������Ϊ��������Ϊ����
#define			EE_SYS_GG1_LAT_VAL	            (EE_SYSTEM_LOG_GG1_BASE+1*4)		   // 4B��γ�ȣ��Ŵ���10e7������Ϊ��γ����Ϊ��γ
#define			EE_SYS_GG1_RTC_SECOND	         (EE_SYSTEM_LOG_GG1_BASE+2*4)			// 4B��ʱ�䣬ͳһΪ�룬2000��1��1��00:00:00Ϊ��0��
#define			EE_SYS_GG1_SPEED_INFO	         (EE_SYSTEM_LOG_GG1_BASE+3*4)	      // 1�ֽ�GPS�ٶȣ�1�ֽڸ߶ȣ�1�ֽ�GPSˮƽ�������ӣ�1�ֽ����Ǹ���
#define			EE_SYS_GG1_GSM_BASK_STATION	   (EE_SYSTEM_LOG_GG1_BASE+4*4)		   // 2�ֽڻ�վ��Ϣ1��2�ֽڻ�վ��Ϣ2
#define			EE_SYS_GG1_GSM_INFO	         	(EE_SYSTEM_LOG_GG1_BASE+5*4)		   // 1�ֽڻ�վ��Ϣ3, 1�ֽ�GSM��Ϣǿ�ȣ�1�ֽڻ����¶ȣ�1�ֽڱ���
#define			EE_SYS_GG1_VEHICLE_STATUS1	     	(EE_SYSTEM_LOG_GG1_BASE+6*4)			// ����״̬��־λ��ǰ32λ
#define			EE_SYS_GG1_VEHICLE_STATUS2	      (EE_SYSTEM_LOG_GG1_BASE+7*4)			// ����״̬��־λ��ǰ32λ


#define			EE_SYSTEM_LOG_RSV_BASE				(EE_SYSTEM_LOG_GG1_BASE+32)			// Rsv8   32B	 ����

	

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   ����Χ���������ݣ�����Χ�����ݴ洢��Flash����Բ�Ρ����Ρ�����Ρ�·�ߣ���4�࣬ÿ��������Ϊ128
   ÿ�����Դ洢������ĳ���Ϊ32B
   ǰ16B���ܸ�������ǰ����Χ���š�
   ��16B��128Bits��ÿ��Bits����Χ������Ч�ԣ�0����Ч���������Χ����1����Ч������
   ȫ�ֱ����ṹ���壺RoundFenceAttr,RectFenceAttr,PolygonFenceAttr,RoadAttr
*/
// #define			LEN_EE_FENCE_ATTR						64 	// ��UserGobals.h�ﶨ��
#define			EE_ELE_FENCE_ATTR_BASE_ADDR		(EE_OBL_BASE_ADDR+0x900)
#define			EE_ROUND_FENCE_ATTR					(EE_OBL_BASE_ADDR+0x900)	// 64B
#define			EE_RECTANGLE_FENCE_ATTR				(EE_OBL_BASE_ADDR+0x940)	// 64B������Χ�����ԣ�ͬ��
#define			EE_POLYGON_FENCE_ATTR				(EE_OBL_BASE_ADDR+0x980)	// 64B�������Χ����
#define			EE_ROAD_ATTR							(EE_OBL_BASE_ADDR+0x9C0)	// 64B��·�����ԣ�




#define        EE_IN_RECT_AREA_ALARM_MSG			(EE_OBL_BASE_ADDR+0xA00)		// Ӧ������ʹ�ã�����Χ��������Ϣ
#define        EE_OUT_RECT_AREA_ALARM_MSG			(EE_OBL_BASE_ADDR+0xA20)		//
#define        EE_ROAD_LINE_ALARM_MSG				(EE_OBL_BASE_ADDR+0xA40)		// Ӧ������ʹ�ã�ƫ��·�߱�����Ϣ


#define			EE_ONE_ROAD_LINE_DATA				(EE_OBL_BASE_ADDR+0xC00)



#define			EE_CALL_TIME_MONTH				  (EE_OBL_BASE_ADDR+0xCF0)		//  �¶�ͨ��ʱ����  4B
#define			EE_TIME_MONTH				        (EE_OBL_BASE_ADDR+0xCF4)		//  һ�������£�   4B

#define			EE_GSM_IMEI							(EE_OBL_BASE_ADDR+0xCF8)		// GSMģ�� IMEI	��15λAsccii��	  15B
#define			EE_SIM_CARD_ICCID					(EE_OBL_BASE_ADDR+0xD07)		// SIM������ 20λAsccii��		   20B

#define			EE_JT_DRIVER_NAME_LEN				(EE_OBL_BASE_ADDR+0xD1B)		// ��ʻԱ��������  1B
#define			EE_JT_DRIVER_NAME					(EE_OBL_BASE_ADDR+0xD1C)		// ��ʻԱ����  20B
#define			EE_JT_DRIVER_CARD_ID				(EE_OBL_BASE_ADDR+0xD30)		//	��ҵ�ʸ�֤��   20B
#define			EE_JT_CERTIFICATE_NAME_LEN			(EE_OBL_BASE_ADDR+0xD44)	    // ��֤�������Ƴ��� 1B
#define			EE_JT_CERTIFICATE_NAME				(EE_OBL_BASE_ADDR+0xD45)	    // ��֤�������Ƴ��� 60B
#define			EE_JT_DRIVER_CARD_VALID_DATE		(EE_OBL_BASE_ADDR+0xD81)		// ֤����Ч�� 4B

#define			LEN_JT_DRIVER_IC_CARD_INFO					106



///  CAN����ʹ��128B����16��ID
#define		EE_JT_CAN_ID_COLLECT_SET_BASE 				(EE_OBL_BASE_ADDR+0xD85)	 
#define		EE_JT_CAN_ID_COLLECT_SET_0					(EE_JT_CAN_ID_COLLECT_SET_BASE+0*8)		// CAN����ID�����ɼ�����1	8B
#define		EE_JT_CAN_ID_COLLECT_SET_1					(EE_JT_CAN_ID_COLLECT_SET_BASE+1*8)		// CAN����ID�����ɼ�����2	8B
#define		EE_JT_CAN_ID_COLLECT_SET_2					(EE_JT_CAN_ID_COLLECT_SET_BASE+2*8)		// CAN����ID�����ɼ�����3	8B
#define		EE_JT_CAN_ID_COLLECT_SET_3					(EE_JT_CAN_ID_COLLECT_SET_BASE+3*8)		// CAN����ID�����ɼ�����4	8B
#define		EE_JT_CAN_ID_COLLECT_SET_4					(EE_JT_CAN_ID_COLLECT_SET_BASE+4*8)		// CAN����ID�����ɼ�����5	8B
#define		EE_JT_CAN_ID_COLLECT_SET_5					(EE_JT_CAN_ID_COLLECT_SET_BASE+5*8)		// CAN����ID�����ɼ�����6	8B
#define		EE_JT_CAN_ID_COLLECT_SET_6					(EE_JT_CAN_ID_COLLECT_SET_BASE+6*8)		// CAN����ID�����ɼ�����7	8B
#define		EE_JT_CAN_ID_COLLECT_SET_7					(EE_JT_CAN_ID_COLLECT_SET_BASE+7*8)		// CAN����ID�����ɼ�����8	8B
#define		EE_JT_CAN_ID_COLLECT_SET_8					(EE_JT_CAN_ID_COLLECT_SET_BASE+8*8)		// CAN����ID�����ɼ�����9	8B
#define		EE_JT_CAN_ID_COLLECT_SET_9					(EE_JT_CAN_ID_COLLECT_SET_BASE+9*8)		// CAN����ID�����ɼ�����10	8B
#define		EE_JT_CAN_ID_COLLECT_SET_10					(EE_JT_CAN_ID_COLLECT_SET_BASE+10*8)		// CAN����ID�����ɼ�����11	8B
#define		EE_JT_CAN_ID_COLLECT_SET_11					(EE_JT_CAN_ID_COLLECT_SET_BASE+11*8)		// CAN����ID�����ɼ�����12	8B
#define		EE_JT_CAN_ID_COLLECT_SET_12					(EE_JT_CAN_ID_COLLECT_SET_BASE+12*8)		// CAN����ID�����ɼ�����13	8B
#define		EE_JT_CAN_ID_COLLECT_SET_13					(EE_JT_CAN_ID_COLLECT_SET_BASE+13*8)		// CAN����ID�����ɼ�����14	8B
#define		EE_JT_CAN_ID_COLLECT_SET_14					(EE_JT_CAN_ID_COLLECT_SET_BASE+14*8)		// CAN����ID�����ɼ�����15	8B
#define		EE_JT_CAN_ID_COLLECT_SET_15					(EE_JT_CAN_ID_COLLECT_SET_BASE+15*8)		// CAN����ID�����ɼ�����16	8B

// �ӵ�������ȡ
#define		EE_JT_PRODUCTOR_CODE						(EE_OBL_BASE_ADDR+0xE05)	// ���̱���	 4B
#define		EE_JT_CAR_OWNER_PHONE_NUM					(EE_OBL_BASE_ADDR+0xE09)    // ��������	 12B
#define		EE_JT_CAR_OWNER_NAME						(EE_OBL_BASE_ADDR+0xE15) 	// ��������  20B
#define		EE_TJP_VESION								(EE_OBL_BASE_ADDR+0xE29)    // �������汾 2B


	


/////////////  ����Χ������() //////////////////////////
#define			LEN_ELE_RAIL_PARA						32
#define			ELE_TOTAL_NUM							4
#define			EE_ELE_SET_PARAMETER					(EE_OBL_BASE_ADDR+0xEF0)		//
  	
#define			EE_ELE_BASE_ADDR						(EE_OBL_BASE_ADDR+0xF00)	//	 64B����
#define			EE_ELE_RAIL1							(EE_ELE_BASE_ADDR+0*LEN_ELE_RAIL_PARA)		//  16B,Χ��1
#define			EE_ELE_RAIL2							(EE_ELE_BASE_ADDR+1*LEN_ELE_RAIL_PARA)		//  16B,Χ��2
#define			EE_ELE_RAIL3							(EE_ELE_BASE_ADDR+2*LEN_ELE_RAIL_PARA)		//  16B
#define			EE_ELE_RAIL4							(EE_ELE_BASE_ADDR+3*LEN_ELE_RAIL_PARA)		//  16B


#define			EE_RSV_1FF0								0x1fe0			// ������FIRST_RUNʹ��

																													
#pragma		pack(1)
typedef		struct	_RW_EEP
{
	uchar		Type;				// �洢������ S:SPI,I:I2C
	uchar		PageLen;			// ����
	uchar		RW;				// ��д��־��'R'����'W'д
	uchar		DS;				// ѡ������
	uint16	Addr;				// �洢��ַ
	uint16	Length;			// ��д����	
	uchar		Buffer[256];	// д����
} *pRW_EEP;


#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */

