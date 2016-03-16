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


#define		OVER_TIME_MAX_CNT				8		// ����¼��ʱ��¼������
#define		OVER_SPEED_MAX_CNT			20		// ����¼���ټ�¼������
#define		ID_CARD_REC_MAX_CNT			150	// ����¼��ʻԱ�����֤����
#define		KV_OP_REC_MAX_CNT				100	// ����¼���س�������ϵ����¼


////////////	ͨѶ�����־	///////////////////////////////////////////////////////
#define		UP_LOAD_ERROR					0xfa	// �ϴ������־��
#define		DOWN_LOAD_ERROR				0xfb	// �´������־��

#define		RESERVER_WORD					0x00	// 

#define		LEN_U0_GB_PACK_HEAD			6

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	  			�����ֶ���
*/
GB19056_EXT    enum
{
	//����������	//////////////////////////////////////////////////////////
	eGB_rCMD_GET_STD_VERSION				= 0x00,	// �ɼ���¼��ִ�б�׼
	eGB_rCMD_GET_DRIVER_CODE				= 0x01,	// ��ʻԱ���뼰���Ӧ�Ļ�������ʻ֤��
	eGB_rCMD_GET_REAL_TIME					= 0x02,	// �ɼ�ʵʱʱ��
	eGB_rCMD_GET_TOTAL_MILAGE				= 0x03,	// �ɼ��ۼ���ʻ���
	eGB_rCMD_GET_SPEED_KV					= 0x04,	// �ɼ���¼������ϵ��				  
	eGB_rCMD_GET_VEHICLE_VIN				= 0x05,	// �ɼ�������Ϣ		*����VIN�ţ����ƺ��룬��������	   
	eGB_rCMD_GET_SIGNAL_CONFIG				= 0x06,	// �ɼ�״̬�ź�������Ϣ	 
	eGB_rCMD_GET_SERIAL_NUMBER		      = 0x07, 	// �ɼ���¼��Ψһ���кţ�����ʹ�ã�ʵ�ʲ�ʹ�ã�
	eGB_rCMD_GET_SPEED_PER_SECOND			= 0x08,	// �ɼ�ָ������ʻ�ٶȼ�¼		ÿ����ƽ���ٶ�							
	eGB_rCMD_GET_POSITION_INFO				= 0x09,	// �ɼ�ָ����λ����Ϣ��¼
	eGB_rCMD_GET_PARKING_DATA				= 0x10,	// �ɼ�ָ�����¹��ɵ��¼��ͣ��ǰ20�����ݣ�
	eGB_rCMD_GET_OVER_TIME_DRIVE			= 0x11,	// �ɼ�ָ���ĳ�ʱ��ʻ��¼	ƣ�ͼ�ʻ����	
	eGB_rCMD_GET_GET_LOG_IN_OUT_RECORD	= 0x12,	// �ɼ�ָ���ļ�ʻ����ݼ�¼		���������ĵ�¼�˳���¼						 
	eGB_rCMD_GET_POWER_RECORD				= 0x13,	// �ɼ��ⲿ�����¼
	eGB_rCMD_GET_MODIFY_PARA_RECORD		= 0x14,	// �ɼ�ָ���Ĳ����޸ļ�¼
	eGB_rCMD_GET_SPEED_STATUS_LOG			= 0x15,	// �ɼ�ָ�����ٶ�״̬��־


	eGB_rCMD_GET_SPEED_PER_MIN				= 0x55,		// ÿ����ƽ���ٶ�	  Ϊ�˱���ͨ�����Ȳ�ע����

	//����������	//////////////////////////////////////////////////////////
	eGB_rCMD_GET_SetUp_PARA       	   = 0x20,	// ��¼�ǰ�װ����	     ��eGB_rCMD_GET_SPEED_PULSE_COEFFICIENT   �ظ� �Ƿ��׼�����⣿
	eGB_rCMD_GET_Posicion_PARA     	   = 0x21,	// ��λ�澯��ز�������
	eGB_rCMD_GET_ElePen_Data     	      = 0x22,	// ����Χ������
	eGB_rCMD_GET_ElePen_List     	      = 0x23,	// ����Χ��Ŀ¼
   eGB_rCMD_GET_RoadPlan_Data     	   = 0x2A,	// ��·�������
	eGB_rCMD_GET_RoadPlan_List    	   = 0x2B,	// ��·���Ŀ¼
	

	//���У�����涨������ 0x01~0x15��7�������������������ʾ����ʾ
	
	////////////	����������	//////////////////////////////////////////////////////////
	eGB_rCMD_SET_DRIVER_CODE				= 0x81,	// ���ü�ʻԱ���룬��ʻ֤����			  GB2012����

	eGB_rCMD_SET_VEHICLE_VIN				= 0x82,	// ���ó���VIN�ţ����ƺţ���������	   
	eGB_rCMD_SET_SETUP_DATE					= 0x83,	// ��װ����
	eGB_rCMD_SET_SIGNAL_STATUS				= 0x84,	// ����״̬��������Ϣ						
	eGB_rCMD_SET_REAL_TIME					= 0xc2,	// ����ʵʱʱ��								
	eGB_rCMD_SET_VEHICLE_PULSE_KV			= 0xc3,	// ���ó�������ϵ��						
	eGB_rCMD_SET_INIT_MIL					= 0xc4,  // ���ó�ʼ���
	
	eGB_rCMD_SET_SERIAL_NUMBER				= 0xc7,	// ����Ψһ���кţ�����ʹ�ã�ʵ�ʲ�ʹ�ã�
	
	////////////	�춨��������	//////////////////////////////////////////
	eGB_rCMD_SET_CHECK						= 0xE0,	//�춨����
	eGB_rCMD_SET_CHECK_CONTINUSE        = 0xE2,	//�춨ģʽ����
	
} enGB_RS232_CMD;



//GB19056_EXT    enum
//{
//	//USB���ݿ����	//////////////////////////////////////////////////////////
//	eGB_GET_STD_VERSION				= 0,	// �ɼ���¼��ִ�б�׼
//	eGB_GET_DRIVER_CODE				= 1,	// ��ʻԱ���뼰���Ӧ�Ļ�������ʻ֤��
//	eGB_GET_REAL_TIME					= 2,	// �ɼ�ʵʱʱ��
//	eGB_GET_TOTAL_MILAGE				= 3,	// *���360h�ڵ��ۼ���ʻ�������
//	eGB_GET_SPEED_PULSE_COEFFICIENT= 4,	// *Vehicle Characteristic Coefficients	
//	eGB_GET_VEHICLE_VIN				= 5,	// *����VIN�ţ����ƺ��룬��������
//	eGB_GET_SIGNAL_CONFIG			= 6,	// �ɼ�״̬�ź�������Ϣ
//	eGB_GET_SERIAL_NUMBER		   = 7,  // ��ƷΨһ���к�
//	eGB_GET_SPEED_PER_SECOND		= 8,	// ÿ����ƽ���ٶ�
//	eGB_GET_SPEED_PER_MIN			= 9,	// ÿ����ƽ���ٶ�
//	eGB_GET_POSITION_INFO			= 10,	// �ɼ�ָ����λ����Ϣ
//	eGB_GET_STOP_DATA					= 11,	// *�¹��ɵ����ݣ�ͣ��ǰ20�����ݣ�
//	eGB_GET_OVER_TIME_3H				= 12,	// ƣ�ͼ�ʻ����
//	eGB_GET_GET_LOG_IN_OUT_RECORD	= 13,	// ���������ĵ�¼�˳���¼
//	eGB_GET_POWER_RECORD				= 14,	// �ɼ��ⲿ�����¼
//	eGB_GET_MODIFY_PARA_RECORD	   = 15,	// �ɼ�ָ���Ĳ����޸ļ�¼
//	eGB_GET_SetUp_PARA       	   = 16,	// ��¼�ǰ�װ����
//	eGB_GET_Posicion_PARA     	   = 17,	// ��λ�澯��ز�������
//	eGB_GET_ElePen_Data     	   = 18,	// ����Χ������
//	eGB_GET_ElePen_List     	   = 19,	// ����Χ��Ŀ¼
//   eGB_GET_RoadPlan_Data     	   = 20,	// ��·�������
//	eGB_GET_RoadPlan_List    	   = 21,	// ��·���Ŀ¼
//	
//} enGB_USB_SierierNb;

GB19056_EXT    enum
{
	//USB���ݿ����	//////////////////////////////////////////////////////////
	eGB_GET_STD_VERSION				= 0,	// �ɼ���¼��ִ�б�׼
	eGB_GET_DRIVER_CODE				= 1,	// ��ʻԱ���뼰���Ӧ�Ļ�������ʻ֤��
	eGB_GET_REAL_TIME					= 2,	// �ɼ�ʵʱʱ��
	eGB_GET_TOTAL_MILAGE				= 3,	// *���360h�ڵ��ۼ���ʻ�������
	eGB_GET_SPEED_PULSE_COEFFICIENT= 4,	// *Vehicle Characteristic Coefficients	
	eGB_GET_VEHICLE_VIN				= 5,	// *����VIN�ţ����ƺ��룬��������
	eGB_GET_SIGNAL_CONFIG			= 6,	// �ɼ�״̬�ź�������Ϣ
	eGB_GET_SERIAL_NUMBER		   = 7,  // ��ƷΨһ���к�
	eGB_GET_SPEED_PER_SECOND		= 8,	// ÿ����ƽ���ٶ�	  ��ʻ�ٶȼ�¼
	eGB_GET_POSITION_INFO			= 9,	// �ɼ�ָ����λ����Ϣ
	eGB_GET_STOP_DATA					= 10,	// *�¹��ɵ����ݣ�ͣ��ǰ20�����ݣ�
	eGB_GET_OVER_TIME_3H				= 11,	// ƣ�ͼ�ʻ����	  ��ʱ��ʻ��¼
	eGB_GET_GET_LOG_IN_OUT_RECORD	= 12,	// ���������ĵ�¼�˳���¼	  ��ʻ����ݼ�¼
	eGB_GET_POWER_RECORD				= 13,	// �ɼ��ⲿ�����¼
	eGB_GET_MODIFY_PARA_RECORD	   = 14,	// �ɼ�ָ���Ĳ����޸ļ�¼
	eGB_GET_SPEED_STATUS_LOG		= 15,  // �ٶ�״̬��־
	
} enGB_USB_SierierNb;




////////////  ���ظ�������ǹ��꣩ ////////////////////////////////////////////////

#define		SET_USER_VEHICLE_NUM			0xa1	// �����û�������ţ�4�ֽ�10�����ַ�
#define		SET_TIME_SPEED					0xa2	// ����ʱ�����ֵ���ٶ����ޡ��Ӽ��ٶ�����
#define		SET_SERIAL_NUMBER				0xa3	// ���ò�ƷΨһ���к�
#define		SET_USER_FLAG					0xa6	// �����û���ʶ
#define		SET_GPS_TIME_DIFFERENCE		0xac	// ����GPSʱ���뱾��ʱ���ʱ��
//#define		SET_INIT_SYS					0xaf	// ��ʼ���ն˴洢�������ã���

////////////  �춨���� ////////////////////////////////////////////////
#define     rCMD_CHECK_SPEED_MILAGE    0xE1  //�ٶ���̼춨ģʽ�·����ٶ����������
#define     CHECK_SPEED_MILAGE_MODE    0x01    	  //�ٶ���̲���ģʽ
#define     CHECK_VEHICLE_MODE         0x02    	  //�ٶ��������ģʽ
#define     CHECK_RTC_TIME_MODE        0x03    	  //ʵʱʱ�Ӳ���ģʽ


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	  			����������洢���е���ʼ��ַ(�͵�ַ��ǰ)
*/
////////////	����������洢���е���ʼ��ַ  ///////////////////////////////


#define		FM_INIT_MILAGE_BCD			0x20			// ��ʼ���		4B
#define		FM_LAST_SECOND					0x24		// ��¼������״̬�����һ����(������ʱ����)	 6B



#define     FM_ICCardInOrOut_FLAG   	0x2B      //IC���Ƿ��ڲ忨�۱�־	1B
#define     FM_SpeeMode_FLAG        	0x2C      //�ٶ����ͱ�־1 GPS�ٶȣ�0 �����ٶ�  Ĭ�������ٶ�		1B
#define		FM_USE_GPS_MIL					0x2D		// ʹ��GPS�ۼ����
#define		FM_ENABLE_SYS_SLEEP			0x2E		// ����ϵͳ���߱�ʶ
#define		FM_ENABLE_VDR_UPLOAD			0x2F		// ����ϵͳ���߱�ʶ
////////// ��Ȼָ�����״̬�� ����� 0x30 - 0x42 ������


/////////

#define		FM_RESERVE_3					0x46			// ����3	 6B		
																												
#define		FM_DRIVER_NUMBER				0x4C		  // ��ʻԱ����  4B
#define		FM_DRIVER_CODE					0x50		  // ��ʻԱ��������ʻ֤��	  18B
//#define		FM_DRIVER_NAME					0x62		  // ��ʻԱ����				 20B

#define		FM_RUN_TIME_START		   	0x76		  // ��ǰ��ʻԱ��ʼ��ʻʱ��		6B

#define		FM_RESERVE_4					0x7C			// ����4	4B	
                                    	
                                    	
#define		FM_RESERVE_5					0xAA		  // ����5    	12B
                                    	
#define		FM_VEHICLE_PULSE_KV			0xB0		  // ������ÿ����������(Kֵ)   4B	
                                    	
                                    	
#define		FM_RESERVE_6					0xBE		  // ����6  2B

#define		FM_PLUS_POWROFF				0xC0		  // 		4B	

#define		FM_RESERVE_7					0xC4		  // ����7		60B  

#define		FM_SERIAL_NUMBER				0x100		  //��¼��Ψһ���		36B

#define		FM_RESERVE_8					0x124			// ����8	12B	

#define		FM_GPS_Standard				0x130		  //ִ�еı�׼�汾�ź��޸ĵ���	   2B


#define		FM_RESERVE_8_1					0x132			// ����8_1	14B

//////////	��Ȼָ�����״̬�� ��� ���� 0x140 - 0x1ff������
#define     FM_FACTORY_SET_BASE     	0x140      //�ָ�����������ʼ��ַ 
#define		FM_LAST_5S_PULS				0x140		  // ���5����ÿ���������		20B

#define		FM_15MIN_SPEED		      	0x160		  // 15����ÿ����ƽ���ٶ� 15B

#define		FM_STOP_TIME      			0x16F		  // ͣ��ʱ�� 6B
#define		FM_STOP_LAST15_SPEED			0x175		  // ͣ��ǰ15����ÿ����ƽ���ٶ� 15B

#define     FM_TWO_DAY_MILAGE_HEX		0x185  	  //2�������������HEX��		 4byte
#define     FM_TWO_DAY_MILAGE_BCD		0x189  	  //2�������������BCD��		 4byte

#define     FM_TWO_DAY_1_MILAGE_HEX		0x18D  	  //2���������ٵ�ǰСʱ�����	          4byte
#define     FM_48_HOUR_CNT					0x191 	  //����������Сʱ������

#define     FM_360H_1_MILAGE_HEX	   	0x192  	  //360���ٵ�ǰСʱ�����	          4byte

#define		FM_INSTALL_DATE				0x1E5		  // ���ΰ�װ����(������ʱ����)				6byte
#define		FM_TOTAL_SPEED_PULS			0x1E0		  //�Ӱ�װ���ٶȴ�������������������		5byte
#define     FM_MILAGE_BCD					0x1EB  	  //�Ӱ�װ��¼��������, ������ʻ�������BCD��		 4byte
#define     FM_MILAGE_HEX					0x1EF  	  //�Ӱ�װ��¼��������, ������ʻ�������HEX��		 4byte

#define     FM_360_HOUR_CNT        		0x1F3  	  //360Сʱ��ʱ��		 4byte
#define     FM_360H_MILAGE_HEX			0x1F7  	  //360Сʱ�����HEX��		 4byte
#define     FM_360H_MILAGE_BCD			0x1FB  	  //360Сʱ�����BCD��		 4byte
//////////////////


#define		FM_DRIVER_OUT_IN           0x200 			// 250B // ��¼10����ʻ�˵�¼�˳���¼

#define		FM_DRIVER_OUT_IN_CNT				0x2FA				//  ��ʻ�˵�¼�˳���¼����		4B

#define		FM_RESERVE_9					0x2FE		  // ����9�� 4B


// 2013-06-21 9:23 �ڼ�������޸ģ���Ϊ�к󱸵�أ�ͣ�����ݲ��洢�����磬�����޸�ΪFlashҳ�洢������
#define		FM_FLASH_PAGE_CNT				0x300			// �������洢��Flashҳ��������Ԥ��256B
#define		FM_FLASH_PAGE_CNT_INIT_FLAG	0x3F0		// 

#define		FM_OVER_SPEED_PARA			0x4E4			// 
#define		FM_ENABLE_OVER_SPEED			0x4E4			//	1B
#define		FM_HIGH_SPEED_LIMIT			0x4E5			// 1B
#define		FM_LOW_SPEED_LIMIT			0x4E6			// 1B
#define		FM_PRE_ALARM_OVP				0x4E7			// 1B
#define		FM_OVP_ENABLE_TIME			0x4E8			// 2B
#define		FM_OVP_SECOND_CNT				0x4EA			// 2B

#define		FM_DRIVING_PARA				0x4EC			// 
#define		FM_DRIVEING_TIME				0x4EC		  	//ƣ�ͼ�ʻ��ֵ   4B
#define		FM_REST_TIME   	      	0x4F0		  	//��С��Ϣʱ��	 	4B
#define		FM_DRIVE_ONE_DAY_LIMINT		0x4F4		  	//һ�����ʻʱ�� 4B
#define		FM_PARKING_TIME_LIMINT		0x4F8		  	//ͣ����ʱʱ��	 4B	
#define		FM_OVT_PRE_ALARM_TIME		0x4FC			// Ԥ��ʱ��,4B

#define		FM_FATIGUE_DRIVING_CNT		0x500			// 4B
#define		FM_SAVE_FATIGUE_DRIVING		0x504			// 250B(5*50B)



#define		FM_RESERVE_14					0x600		// ����14��512B

#define	   	FM_EXT_POWER_STATUS		  0x800       // �ϵ�����¼	  252B	 ѭ����36����¼������ŵ�flash
#define		   FM_EXT_POWER_STATUS_CNT    0x8FC       //    �ϵ��   4B


#define	   FM_PARA_MODIFY_RECORD		  0x900       // �����޸ļ�¼	  252B	 ѭ����36����¼������ŵ�flash
#define		 FM_PARA_MODIFY_RECORD_CNT    0x9FC       //  �����޸ļ�¼����   4B



#define		FM_SIGNAL_CONFIG				0xA00			// ״̬��������Ϣ 80B


#define		FM_SPEED_STAUTS_JUDGE	  0xA50		// �ٶ�״̬�ж�	8B


#define		FM_RESERVE_17					0xA58		// ����17�� 424B	


#define		FM_CURRENT_DRIVER_INFO_TIME		0xC00		// ��ǰ��ʻԱ��Ϣ����ʻʱ�� 56B

//////////// ����������2���ʻԱ��Ϣ  ��112�ֽ�
#define		FM_RECENT_DRIVER_INFO_TIME		0xC38		// ÿ��56�ֽ�

#define		FM_DRIVER_SERIAL_NUMBER				0xCA8		// 4B �Ա�ż�ʻԱ����

#define		FM_RESERVE_17_1			0xCAC		// ������ 54B


#define		FM_360H_LOCATION_TIME 	0xD00			// 6B  360Сʱλ�õĵ���Сʱ�Ŀ�ʼʱ��

#define		FM_360H_LOCATION_INFO	0xD06				// 660B   360Сʱλ�õĵ���Сʱ������	 �����󣬴浽flash

#define		 FM_RESERVE_18				0xF9A				//  ������ 358B



#define		FM_END_ADDR_GB_T19056					0x1100	// �洢GB19056��ص����������ַ���õ�ַ����ʹ�ã��Ѿ�����ҪJT808


#define		DISP_ONE_SCR					0xdb	// ��ʾһ����Ļ����

////////////   �������� ////////////////////////////////////////////////////////////
#define		SET_KEY_CODE					0xa5	// ģ����̲���

#define		SET_U0_U1_CLARITY				0xaf	// ���õ�2����Ա�־��Ԫ��U0��U1͸�����书��

#define		SET_SEND_TO_UART				0xce	// �����ݷ��͵�����1





///////////// ��GSM/GPRS��ص�ÿ����Ϣ�ĳ����ֽ� ////////////////////////////////////////
#define		LEN_SMS_CENTER_NUMBER		16			// ����Ϣ���ĺ���洢�ռ���ռ����
#define		LEN_GPRS_SERVER_IP			32			// GPRS������IP�Ͷ˿ںŴ洢�ռ���ռ�ĳ���





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	  			���ֲ����ĳ����ֽ���
*/

#define		LEN_GPS_Standard			2    //ִ�еı�׼�汾�ź��޸ĵ���
#define		LEN_DRIVER_CODE			18		// ��������ʻ֤��
#define		LEN_MILAGE		    		20	// �ۼ���ʻ�������
#define		LEN_VEHICLE_KV				8		// Vehicle Characteristic Coefficients
#define		LEN_VEHICLE_VCC			41		// ����VIN�ţ����ƺ��룬��������
#define		LEN_VEHICLE_VIN			17		// ����VIN��
#define		LEN_VEHICLE_CODE			12		// ���ƺ���
#define		LEN_VEHICLE_CLASS			12		// ��������
#define		LEN_RTC						6		// RTC


#define		LEN_USER_VEHICLE_NUM		4		// �û�������ţ�4�ֽ�10�����ַ�
#define		LEN_ABOUT_TIME				4		// ʱ�����ֵ
#define		LEN_TIME_SPEED				6		// ʱ�����ֵ���ٶ����ޡ��Ӽ��ٶ�����
#define		LEN_SERIAL_NUMBER			35		// ��ƷΨһ���к�
#define		LEN_OVER_SPEED				645	// ��������
#define		LEN_OP_RECORD				256	// �������������¼
#define		LEN_USER_FLAG				32		// �û���ʶ

#define		LEN_DRIVER_



#define		LEN_ALL_DATA				0x7000// �ɼ���������

#define		LEN_OVER_TIME_1REC		30		// ƣ�ͼ�ʻ��¼ÿ����Ϣ�����ֽ�		   ԭ����28				��GB��30
#define		LEN_OVER_SPEED_1REC		32		// ���ټ�¼ÿ����Ϣ�����ֽ�
#define		LEN_ID_CARD_1REC			24		// ��ʻԱ�����֤�洢�ĳ���(֤��+�忨ʱ��)
#define		LEN_RUN_STATUS_CNT	   15		// ������ʻֹͣ��ƣ�ͼ�ʻ��־λ�ĳ���

#define		LEN_LAST_5_DRIVERS_1REC	36		// ���5����ʻԱ���˴Σ���ʻ��Ϣÿ�ڵ��ֽڳ���

#define		LEN_KV_OP_1REC				8		// ��������ϵ�����ز�����¼һ���ĳ���
#define		LEN_KV_OP_REC				800	// ��������ϵ�����ز�����¼�Ĵ洢����

#define		LEN_48Hour_SPEED_DATA	(126*60*48UL)			// 48Сʱ��ʻ�ٶȳ���	126 * 60 * 48
#define		LEN_360H_POS_DATA			(666*360UL)	// 360Сʱλ����Ϣ
#define		LEN_OVER_TIME_DATA		(50*100)	// ��ʱ���ݿ鳤��
#define		LEN_DRIVER_OUT_IN_DATA	(25*200)	// ��ʻԱ�ǳ��������ݿ鳤��
#define		LEN_GET_POWER_RECORD_DATA (7*100)	// �ⲿ�������ݿ鳤��
#define		LEN_GET_MODIFY_PARA_RECORD_DATA	(7*100)		//  �����޸�
#define	   LEN_GET_SPEED_STATUS_LOG_DATA	   (133*10)	 // �ٶ�״̬��־

#define		LEN_INSTALL_DATE			6		// ���ΰ�װ���ں�ʱ��

#define		LEN_20S_DATA				234	// һ��20S���ݳ���
#define		LEN_STOP_DATA				(100*LEN_20S_DATA)	// �¹��ɵ����ݳ��� 100��
//#define		LEN_20S_DATA_DRIVER	   180	// 10���ɵ����ݶ�Ӧ��10����ʻ֤�ŵ��ܳ���
//#define		LEN_MILAGE_2DAY			3		// ���2���������ڵ��ۼ���ʻ���
//#define		LEN_360H_DATA				(65*360)	// 360Сʱ���ٶ�����

#define		LEN_CHECK_SPEED_MILAGE	45		// �û���ʶ


//#define     LEN_GPS_Standard			2    //��¼��ִ�еı�׼�汾��

#define     LEN_SETUP_TIME          6     //��װʱ��

#define		LEN_INIT_MIL			4	  // ��ʼ���

#define		LEN_GPS_TIME_DIFFERENCE	4		// 


#define     LEN_SIGNAL_STATUS_1PEC   87   //״̬�ź�������Ϣ����		  

//#define     LEN_SIGNAL_STATUS_NUMBER 4*8  //�ź�״̬������6���ֽ�48��


#define    MAX_SIGNAL_STATUS_BYTE_NUMBER 	12  	//�ź�״̬�ֽڸ���


//#define     LEN_TIRE_DRIVE_IN_FM     240		  //ƣ�ͼ�ʻ��¼��FM�����ݳ���
//#define     LEN_LAST_22S_IN_FM       244		  //�¹��ɵ��¼��FM�����ݳ���
//#define     LEN_48HSPEED_IN_FM       246 		  //48Сʱ�ٶȼ�¼��FM�����ݳ���
//#define     LEN_360HSPEED_IN_FM      245		  //360Сʱ�ٶȼ�¼��FM�����ݳ���
//#define     LEN_360HLocation_IN_FM   245		  //360Сʱλ�ü�¼��FM�����ݳ���
//#define     LEN_Drv_InOut_IN_FM		 225		  //��¼�˳���¼��FM�����ݳ���
//#define     LEN_Powr_OnOff_IN_FM		 238		  //�ϵ�����¼��FM�����ݳ���
//#define     LEN_Para_Cha_IN_FM		 245		  //�����޸ļ�¼��FM�����ݳ���
//
//#define     RD_TIRE_DRIVE_IN_FM_TIMES        8		//ƣ�ͼ�ʻ��¼��FM�ļ�¼����
//#define     RD_48HSPEED_IN_FM_TIMES          240 	//48Сʱ�ٶȼ�¼��FM�ļ�¼����
//#define     RD_360HSPEED_IN_FM_TIMES         240	//360Сʱ�ٶȼ�¼��FM�ļ�¼����
//#define     RD_360HLocation_IN_FM_TIMES      30	   //360Сʱλ�ü�¼��FM�ļ�¼����
//#define     RD_Drv_InOut_IN_FM_TIMES		   9		//��¼�˳���¼��FM�ļ�¼����
//#define     RD_Powr_OnOff_IN_FM_TIMES		   34		//�ϵ�����¼��FM�ļ�¼����
//#define     RD_Para_Cha_IN_FM_TIMES		      35		//�����޸ļ�¼��FM�ļ�¼����



#define   	USBGBDATANUMBER						16    //USB���ݿ����	���22��

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


GB19056_EXT struct {
	uchar Cmd;
	uchar StartTime[6];
	uchar EndTime[6];
	uint16 TotalItem;
	uchar GetVDRFlag;
	//uint16 TotalPack;
	//uint16 PackIndex; 
	uchar SearchFlag;   // ������־�� 0����ͷ��ʼ����flash  1�����ϴ�flash��ַ����
	uchar	OverTimeSearch; // ���	SearchFlag �ĳ�ʱ

}GB_LargeData;

// �ⲿ��Դ����״̬
#define		LEN_EXT_POWER_STATUS		7
typedef struct {
	uchar Time[6];
	uchar Status; 			 // 1:ͨ�磬2���ϵ�
}EXT_POWER_STATUS,*pEXT_POWER_STATUS;


typedef	struct
{
	uint32	FMAddr;			  // ����������ĵ�ַ
	uint32	FMCnt;			  // ������Ŀ�ܼ���
	uint32	FMSaveItemCnt;	  // ������Ŀ�ܱ��������������
	uint32	SaveItemLen;		// ������Ŀ����
	uint32	FlashStartPageAddr;			// ������Ŀ��flash�Ŀ�ʼҳ��ַ
	uint32	FlashPageSaveCnt;	 // ������Ŀ��flash�ļ���
	uint32	FlashTotalPages;	 //  ������Ŀ��flash�ܹ���ҳ�� 
	uchar    TimeBCDPos;				// ÿ��������Ŀʱ�����ĸ�λ��	 		
	uchar		*ptBuf;				   // ��ʱ����	  
	uchar 	*pSendBuffer;			 // ���ͻ���
		
} VDR_DATA_FUN_1_ST;

// �����޸�
#define		LEN_PARA_MODIFY_RECORD		7
typedef struct {
	uchar Time[6];		  // ʱ��
	uchar EventType; 			 // 	  �¼�����Ϊ���ò���������
}PARA_MODIFY_RECORD,*pPARA_MODIFY_RECORD;
 
 

#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
