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

#ifndef      __GY_T19056_H__
#define      __GY_T19056_H__


#ifdef      GY19056_GLOBALS
#define     GY19056_EXT
#else
#define     GY19056_EXT    extern
#endif


// $$$$$$$$$$$$$$$$$$ <<< Use Configuration Wizard in Context Menu >>> $$$$$$$$$$$$$$$$$
// <h>�����ļ��������




// </h>

// $$$$$$$$$$$$$$$$$$$ <<< end of configuration section >>> $$$$$$$$$$$$$$$$$$$$$$$$$$$$



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	  			�����ֶ���
*/


GY19056_EXT    enum
{
	/////////	����������	//////////////////////////////////////////////////////////
	
	eGY_rCMD_SELECT_SIM_CARD				= 0X01,	// 
	
	eGY_rCMD_GET_REAL_TIME					= 0x02,	// �ɼ�ʵʱʱ��
	
	eGY_rCMD_GET_ALL_REAL_TIME				= 0x20,	// ��ȡCPU��PCF8563ʱʵʱ��	
	// ���Դ洢�������� ///////////////////////////////////////////////////////
	eGY_rCMD_GET_SET_EE_DATA				= 0x28,	// ��дָ�������ַ�ͳ��ȵ�����
	eGY_rCMD_GET_SET_DF_DATA				= 0x29,	// ��дEEPROM����
	eGY_rCMD_GET_SET_NF_DATA				= 0x2a,	// ��д���ݵ�ָ��NAND Flashҳ
	eGY_rCMD_GET_SET_CPU_DATA				= 0x2b,	// ��дCPU�ڲ�Flash���ݣ�IAP	
	eGY_rCMD_GET_SENSOR_KV					= 0x30,	// �ɼ�������ÿ������������
	eGY_rCMD_GET_USER_VEHICLE_NUM			= 0x31,	// �û�������ţ�4�ֽ�10�����ַ�
	eGY_rCMD_GET_TIME_SPEED					= 0x32,	// �ɼ�ʱ�����ֵ���ٶ����ޡ��Ӽ��ٶ�����
	eGY_rCMD_GET_SOLE_NUMBER				= 0x33,	// ��ƷΨһ���к�
	eGY_rCMD_GET_OVER_SPEED					= 0x34,	// ��������
	eGY_rCMD_GET_OP_RECORD					= 0x35,	// �������������¼
	eGY_rCMD_GET_USER_FLAG					= 0x36,	// �û���ʶ
	eGY_rCMD_GET_STOP_DATA_DRIVER			= 0x37,	// �ɼ��ɵ����ݶ�Ӧ�ļ�ʻ֤��
	eGY_rCMD_GET_ID_CARD_REC				= 0x38,	// �ɼ���ʻԱˢ����¼(100��)
	eGY_rCMD_GET_SYE_SPECIAL_DATA			= 0x39,	// �ɼ�ָ������
	eGY_rCMD_GET_INSTALL_DATE				= 0x3a,	// �ɼ����ΰ�װ����
	eGY_rCMD_GET_KV_OP_REC					= 0x3b,	// �ɼ�����Kֵ���ؼ�¼(100�μ�¼)
	eGY_rCMD_GET_GPS_TIME_DIFFERENCE		= 0x3c,	// �ɼ�GPSʱ���뱾��ʱ���ʱ��
	eGY_rCMD_GET_ALL_DATA					= 0x3f,	// �ɼ���¼����������

	//////////// һЩ��ʱ��־    ///////////////////////////////////////////////////////
	eGY_rCMD_SET_8_FLAGS						= 0xae,	// ����8����־λ
	////////////  ���ظ�������ǹ��꣩ ////////////////////////////////////////////////
	eGY_rCMD_SET_SENSOR_KV					= 0xa0,	// ���ô�����ÿ������������
	eGY_rCMD_SET_USER_VEHICLE_NUM			= 0xa1,	// �����û�������ţ�4�ֽ�10�����ַ�
	eGY_rCMD_SET_TIME_SPEED					= 0xa2,	// ����ʱ�����ֵ���ٶ����ޡ��Ӽ��ٶ�����
	eGY_rCMD_SET_SOLE_NUMBER				= 0xa3,	// ���ò�ƷΨһ���к�
	eGY_rCMD_SET_USER_FLAG					= 0xa6,	// �����û���ʶ
	eGY_rCMD_SET_GPS_TIME_DIFFERENCE		= 0xac,	// ����GPSʱ���뱾��ʱ���ʱ��
	eGY_rCMD_SET_INIT_SYS					= 0xaf,	// ��ʼ���ն˴洢�������ã���
	
	
	////////////   �������� ////////////////////////////////////////////////////////////	
	eGY_rCMD_SET_U0_U1_CLARITY				= 0xaf,	// ���õ�2����Ա�־��Ԫ��U0��U1͸�����书��
	eGY_rCMD_SET_SEND_TO_UART				= 0xce,	// �����ݷ��͵�����1

	////////////  ����GSM/GPRS�����Ϣ������ ////////////////////////////////////////////////
	eGY_rCMD_GET_SMS_CENTER_NUMBER		= 0x40,	// ��ȡ����Ϣ�������ĺ���
	eGY_rCMD_GET_GPRS_SERVER_IP			= 0x41,	// ��ȡGPRS������IP�Ͷ˿ں�
	eGY_rCMD_GET_OEM_ID						= 0x42,	// ��ȡ�����̱�־���ն�ID
	eGY_rCMD_GET_CENTER_NUMBER				= 0x43,	// ����3��������ĺ���
	eGY_rCMD_GET_PIN_CODE					= 0x44,	// 
	eGY_rCMD_GET_OEM_WEB						= 0x45,	// ��ȡ�����̱�ʶ
	eGY_rCMD_GET_GPS_PACK_TIME				= 0x46,	// ��ȡGPS���ݴ����ز���
	eGY_rCMD_GET_GPS_MILEAGE_ELE_RAIL	= 0x47,	// 
	eGY_rCMD_GET_SIM_NUMBER					= 0x48,	// ��ȡSIM����
	eGY_rCMD_GET_OUTPUT_STATUS				= 0x49,	// ���״̬
	eGY_rCMD_GET_GY_FLAG						= 0x4b,	// 	

	////////////  ����GSM/GPRS�����Ϣ������ ////////////////////////////////////////////////
	eGY_rCMD_SET_SMS_CENTER_NUMBER		= 0xb0,	// ���ö��ŷ���Ϣ���ĺ���
	eGY_rCMD_SET_GPRS_SERVER_IP			= 0xb1,	// ����GPRS������IP�Ͷ˿ں�
	eGY_rCMD_SET_OEM_ID						= 0xb2,	// �����̺��ն�ID
	eGY_rCMD_SET_CENTER_NUMBER				= 0xb3,	// ����3��������ĺ�
	eGY_rCMD_SET_PIN_CODE					= 0xb4,	// 
	eGY_rCMD_SET_OEM_WEB						= 0xb5,	// ���������̱�ʶ
	eGY_rCMD_SET_GPS_PACK_TIME				= 0xb6,	// ����GPS���ݴ����ز���
	eGY_rCMD_SET_CLR_GPS_MILEAGE			= 0xb7,	//
	eGY_rCMD_SET_SIM_NUMBER					= 0xb8,	// ����SIM����
	eGY_rCMD_SET_OUTPUT_STATUS				= 0xb9,	// ���״̬	
	eGY_rCMD_SET_IDLE_PARA					= 0xba,	// ���õ͹��Ĳ���
	eGY_rCMD_SET_MEMORY_FORMAT				= 0xfa,	// 
	
	eGY_rCMD_SET_REAL_TIME					= 0xc2,	// ����ʵʱʱ��
	
	////////////	�������	//////////////////////////////////////////////////////////
	eGY_rCMD_UP_LOAD_ERROR					= 0xfa,	// �ϴ������־��
	eGY_rCMD_DOWN_LOAD_ERROR				= 0xfb,	// �´������־��
	
	eGY_rCMD_GET_SYSTEM_LOG					= 0xfe,	// ��ϵͳ��־����
	
	eGY_rCMD_SYSTEM_RESTART					= 0xff,	// ������
	
} enGY_RS232_CMD;

///////////// ��GSM/GPRS��ص�ÿ����Ϣ�ĳ����ֽ� ////////////////////////////////////////

#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */




