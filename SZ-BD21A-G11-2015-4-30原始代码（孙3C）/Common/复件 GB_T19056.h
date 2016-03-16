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

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	  			�����ֶ���
*/

/////////	����������	//////////////////////////////////////////////////////////
#define		GET_DRIVER_CODE				0x01	// *��ʻԱ���뼰���Ӧ�Ļ�������ʻ֤��
#define		GET_REAL_TIME					0x02	// *
#define		GET_MILAGE_360H				0x03	// *���360h�ڵ��ۼ���ʻ�������
#define		GET_VEHICLE_CC					0x04	// *Vehicle Characteristic Coefficients
#define		GET_SPEED_360H					0x05	// *���360h��ÿ���ӵ���ʻ�ٶ����ݣ�ÿ����һ����
#define		GET_VEHICLE_VIN				0x06	// *����VIN�ţ����ƺ��룬��������
#define		GET_STOP_DATA					0x07	// *�¹��ɵ����ݣ�ͣ��ǰ20�����ݣ�
#define		GET_MILAGE_2DAY				0x08	// ���2���������ڵ��ۼ���ʻ���
#define		GET_SPEED_2DAY					0x09	// ���2���������ڵ���ʻ�ٶ�(ÿ����һ��)
#define		GET_OVER_TIME_3H				0x11	// ƣ�ͼ�ʻ����
///////// 	���У�����涨������ 0x01~0x07��7�������������������ʾ����ʾ

/////////	���ظ�������ǹ��꣩ ///////////////////////////////////////////////

#define		GET_ALL_REAL_TIME				0x20	// ��ȡCPU��PCF8563ʱʵʱ��

//////////// ���Դ洢�������� ///////////////////////////////////////////////////////
#define		GET_SET_FM_DATA				0x28	// ��дָ�������ַ�ͳ��ȵ�����
#define		GET_SET_EE_DATA				0x29	// ��дEEPROM����
#define		GET_SET_NF_DATA				0x2a	// ��д���ݵ�ָ��NAND Flashҳ
#define		GET_SET_CPU_DATA				0x2b	// ��дCPU�ڲ�Flash���ݣ�IAP	

#define		GET_SENSOR_KV					0x30	// �ɼ�������ÿ������������
#define		GET_USER_VEHICLE_NUM			0x31	// �û�������ţ�4�ֽ�10�����ַ�
#define		GET_TIME_SPEED					0x32	// �ɼ�ʱ�����ֵ���ٶ����ޡ��Ӽ��ٶ�����
#define		GET_SOLE_NUMBER				0x33	// ��ƷΨһ���к�
#define		GET_OVER_SPEED					0x34	// ��������
#define		GET_OP_RECORD					0x35	// �������������¼
#define		GET_USER_FLAG					0x36	// �û���ʶ
#define		GET_STOP_DATA_DRIVER			0x37	// �ɼ��ɵ����ݶ�Ӧ�ļ�ʻ֤��
#define		GET_ID_CARD_REC				0x38	// �ɼ���ʻԱˢ����¼(100��)
#define		GET_SYE_SPECIAL_DATA			0x39	// �ɼ�ָ������
#define		GET_INSTALL_DATE				0x3a	// �ɼ����ΰ�װ����
#define		GET_KV_OP_REC					0x3b	// �ɼ�����Kֵ���ؼ�¼(100�μ�¼)
#define		GET_GPS_TIME_DIFFERENCE		0x3c	// �ɼ�GPSʱ���뱾��ʱ���ʱ��
#define		GET_ALL_DATA					0x3f	// �ɼ���¼����������








//////////// һЩ��ʱ��־    ///////////////////////////////////////////////////////
#define		SET_8_FLAGS						0xae	// ����8����־λ




////////////	����������	//////////////////////////////////////////////////////////
#define		SET_DRIVER_CODE				0x81	// ���ü�ʻԱ���룬��ʻ֤����
#define		SET_VEHICLE_VIN				0x82	// ���ó���VIN�ţ����ƺţ���������
#define		SET_REAL_TIME					0xc2	// ����ʵʱʱ��
#define		SET_VEHICLE_CC					0xc3	// ���ó�������ϵ��

////////////  ���ظ�������ǹ��꣩ ////////////////////////////////////////////////
#define		SET_SENSOR_KV					0xa0	// ���ô�����ÿ������������
#define		SET_USER_VEHICLE_NUM			0xa1	// �����û�������ţ�4�ֽ�10�����ַ�
#define		SET_TIME_SPEED					0xa2	// ����ʱ�����ֵ���ٶ����ޡ��Ӽ��ٶ�����
#define		SET_SOLE_NUMBER				0xa3	// ���ò�ƷΨһ���к�
#define		SET_USER_FLAG					0xa6	// �����û���ʶ
#define		SET_GPS_TIME_DIFFERENCE		0xac	// ����GPSʱ���뱾��ʱ���ʱ��
//#define		SET_INIT_SYS					0xaf	// ��ʼ���ն˴洢�������ã���





#define		DISP_ONE_SCR					0xdb	// ��ʾһ����Ļ����

////////////   �������� ////////////////////////////////////////////////////////////
#define		SET_KEY_CODE					0xa5	// ģ����̲���

		
#define		SET_U0_U1_CLARITY				0xaf	// ���õ�2����Ա�־��Ԫ��U0��U1͸�����书��

#define		SET_SEND_TO_UART				0xce	// �����ݷ��͵�����1





////////////  ����GSM/GPRS�����Ϣ������ ////////////////////////////////////////////////
#define		GET_SMS_CENTER_NUMBER		0x40	// ��ȡ����Ϣ�������ĺ���
#define		GET_GPRS_SERVER_IP			0x41	// ��ȡGPRS������IP�Ͷ˿ں�
#define		GET_OEM_ID						0x42	// ��ȡ�����̱�־���ն�ID
#define		GET_CENTER_NUMBER				0x43	// ����3��������ĺ���
#define		GET_PIN_CODE					0x44	// 
#define		GET_OEM_WEB						0x45	// ��ȡ�����̱�ʶ
#define		GET_GPS_PACK_TIME				0x46	// ��ȡGPS���ݴ����ز���

#define		GET_GPS_MILEAGE_ELE_RAIL	0x47	// 
#define		GET_GY_FLAG						0x48	// ��ȡ�ҵ���ϵ��ʽ
#define		GET_OUTPUT_STATUS				0x49	// ���״̬	


////////////  ����GSM/GPRS�����Ϣ������ ////////////////////////////////////////////////
#define		SET_SMS_CENTER_NUMBER		0xb0	// ���ö��ŷ���Ϣ���ĺ���
#define		SET_GPRS_SERVER_IP			0xb1	// ����GPRS������IP�Ͷ˿ں�
#define		SET_OEM_ID						0xb2	// �����̺��ն�ID
#define		SET_CENTER_NUMBER				0xb3	// ����3��������ĺ�
#define		SET_PIN_CODE					0xb4	// 

#define		SET_OEM_WEB						0xb5	// ���������̱�ʶ
#define		SET_GPS_PACK_TIME				0xb6	// ����GPS���ݴ����ز���
#define		SET_CLR_GPS_MILEAGE			0xb7	//

#define		SET_OUTPUT_STATUS				0xb9	// ���״̬	

#define		SET_IDLE_PARA					0xba	// ���õ͹��Ĳ���

#define		SET_MEMORY_FORMAT				0xfa	// 



///////////// ��GSM/GPRS��ص�ÿ����Ϣ�ĳ����ֽ� ////////////////////////////////////////
#define		LEN_SMS_CENTER_NUMBER		16			// ����Ϣ���ĺ���洢�ռ���ռ����
#define		LEN_GPRS_SERVER_IP			32			// GPRS������IP�Ͷ˿ںŴ洢�ռ���ռ�ĳ���





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
