/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 		  
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
	�ļ���: 
	����������
	���뻷����
	Ŀ��CPU: 
	���ߣ�
	����ʱ�䣺
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  

#ifndef      __GB_T19056_H__
#define      __GB_T19056_H__


#ifdef      GB19056_GLOBALS
#define     GB19056_EXT
#else
#define     GB19056_EXT    extern
#endif


// $$$$$$$$$$$$$$$$$$ <<< Use Configuration Wizard in Context Menu >>> $$$$$$$$$$$$$$$$$
// <h>�����ļ��������

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>ENABLE_MODIFY_VEHICLE_INFO: �������޸ĳ�����Ϣ <0=> ������ <1=> ����
//   	<i> Default: ������
*/
#define		ENABLE_MODIFY_VEHICLE_INFO		1	// �������޸ĳ�����Ϣ
				 
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>TWO_DAY_OVT: ֻ�������ͣ��ǰ2��ƣ�ͼ�ʻ <0=> ����ȫ�� <1=> ����2����
//   	<i> Default: ����2����
*/
#define		TWO_DAY_OVT			1


// </h>

// $$$$$$$$$$$$$$$$$$$ <<< end of configuration section >>> $$$$$$$$$$$$$$$$$$$$$$$$$$$$


#define		OVER_TIME_MAX_CNT				20		// ����¼��ʱ��¼������
#define		OVER_SPEED_MAX_CNT			20		// ����¼���ټ�¼������
#define		ID_CARD_REC_MAX_CNT			150	// ����¼��ʻԱ�����֤����
#define		KV_OP_REC_MAX_CNT				100	// ����¼���س�������ϵ����¼



////////////	ͨѶ�����־	///////////////////////////////////////////////////////
#define		UP_LOAD_ERROR					0xfa	// �ϴ������־��
#define		DOWN_LOAD_ERROR				0xfb	// �´������־��

#define		RESERVER_WORD					0x00	// 

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	  			�����ֶ���
*/
GB19056_EXT    enum
{
	//����������	//////////////////////////////////////////////////////////
	eGB_rCMD_GET_STD_VERSION				= 0x00,	// �ɼ���¼��ִ�б�׼
	eGB_rCMD_GET_DRIVER_CODE				= 0x01,	// ��ʻԱ���뼰���Ӧ�Ļ�������ʻ֤��
	eGB_rCMD_GET_REAL_TIME					= 0x02,	// �ɼ�ʵʱʱ��
	eGB_rCMD_GET_TOTAL_MILAGE				= 0x03,	// *���360h�ڵ��ۼ���ʻ�������
	eGB_rCMD_GET_SPEED_PULSE_COEFFICIENT= 0x04,	// *Vehicle Characteristic Coefficients	
	eGB_rCMD_GET_SPEED_PER_MIN				= 0x05,	// ÿ����ƽ���ٶ�
	eGB_rCMD_GET_VEHICLE_VIN				= 0x06,	// *����VIN�ţ����ƺ��룬��������
	eGB_rCMD_GET_STOP_DATA					= 0x07,	// *�¹��ɵ����ݣ�ͣ��ǰ20�����ݣ�
	eGB_rCMD_GET_SIGNAL_CONFIG				= 0x08,	// 
	eGB_rCMD_GET_SPEED_PER_SECOND			= 0x09,	// ÿ����ƽ���ٶ�
	eGB_rCMD_GET_GET_LOG_IN_OUT_RECORD	= 0X10,	// 
	eGB_rCMD_GET_OVER_TIME_3H				= 0x11,	// ƣ�ͼ�ʻ����
	eGB_rCMD_GET_POSITION_INFO				= 0x13,	// �ɼ�ָ����λ����Ϣ
	eGB_rCMD_GET_POWER_RECORD				= 0x14,	// �ɼ��ⲿ�����¼
	eGB_rCMD_GET_MODIFY_PARA_RECORD		= 0x15,	// 
	//���У�����涨������ 0x01~0x07��7�������������������ʾ����ʾ
	
	////////////	����������	//////////////////////////////////////////////////////////
	eGB_rCMD_SET_DRIVER_CODE				= 0x81,	// ���ü�ʻԱ���룬��ʻ֤����
	eGB_rCMD_SET_VEHICLE_VIN				= 0x82,	// ���ó���VIN�ţ����ƺţ���������
	eGB_rCMD_SET_SETUP_DATE					= 0x83,	// ��װ����
	eGB_rCMD_SET_SIGNAL_STATUS				= 0x84,	// 
	eGB_rCMD_SET_REAL_TIME					= 0xc2,	// ����ʵʱʱ��
	eGB_rCMD_SET_VEHICLE_CC					= 0xc3,	// ���ó�������ϵ��
	
	////////////	�춨��������	//////////////////////////////////////////
	eGB_rCMD_SET_CHECK
	
} enGB_RS232_CMD;
	




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	  			���ֲ����ĳ����ֽ���
*/

#define		LEN_RTC						6		// RTC

#define		LEN_GPS_TIME_DIFFERENCE	4		// 


////////////	�����ⲿ����	/////////////////////////////////////////////////////
#pragma		pack(1)
typedef		struct	_RW_FM
{
	uchar		RW;				// ��д��־��'R'����'W'д
	uchar		DS;				// ѡ������������ΪGSM��EEPROM
	uint16	Addr;				// �洢��ַ
	uint16	Length;			// ��д����
	uchar		SerialNum[8];	// дʱ��Ҫ���к�
	uchar		Buffer[256];	// д����
} RW_FM,*pRW_FM; 
 

#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
