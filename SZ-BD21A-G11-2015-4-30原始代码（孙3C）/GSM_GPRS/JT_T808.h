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

#ifndef   	__JT_T808_H__
#define   	__JT_T808_H__

#include		"Config\UserConfig.h"


#ifdef      JT808_GLOBALS
#define     JT808_EXT
#else
#define     JT808_EXT    extern
#endif

#define		BB_CMD_ADDR_HEAD						0		        // 	��ʶλ��ʼ�ֽ� 
#define		BB_CMD_ADDR_MSG_ID					1				  //	��ϢID��ʼ�ֽ�
#define		BB_CMD_ADDR_MSG_ATTR					3				  // 	��Ϣ��������ʼ�ֽ�
#define		BB_CMD_ADDR_SIM						5				  //	�ֻ�����ʼ�ֽ�
#define		BB_CMD_ADDR_INDEX						11				  //	��Ϣ��ˮ����ʼ�ֽ�
#define		BB_CMD_ADDR_MSG_PACKETS				13				  //	��Ϣ�ܰ�����ʼ�ֽ�
#define		BB_CMD_ADDR_MSG_PACKET_INDEX		15				  // 	�������ʼ�ֽ�
#define		BB_CMD_ADDR_DATA_BLOCK				17				  //	��Ϣ����ʼ�ֽ�
#define		LEN_BB_PACK_HEAD						17				  // 	��ʶλ����Ϣͷ���ܳ���

#define		LEN_BB_PACK_ITEM						4             // ��Ϣ����װ���


//#define		BB_CMD_ADDR_MSG_ID					0				  //	��ϢID��ʼ�ֽ�
//#define		BB_CMD_ADDR_MSG_ATTR					2				  // 	��Ϣ��������ʼ�ֽ�
//#define		BB_CMD_ADDR_SIM						4				  //	�ֻ�����ʼ�ֽ�
//#define		BB_CMD_ADDR_INDEX						10				  //	��Ϣ��ˮ����ʼ�ֽ�
//#define		BB_CMD_ADDR_MSG_PACKETS				12				  //	��Ϣ�ܰ�����ʼ�ֽ�
//#define		BB_CMD_ADDR_MSG_PACKET_INDEX		14				  // 	�������ʼ�ֽ�
//#define		BB_CMD_ADDR_DATA_BLOCK				16				  //	��Ϣ����ʼ�ֽ�
//#define		LEN_BB_PACK_HEAD						17				  // 	��ʶλ����Ϣͷ���ܳ���


#define 		LEN_BB_REG_PACKET					49		//	37			 modified 2013.3
#define			TER_SUPPORT_CAR_TYPE			((1<<1) | (1<<2) | (1 << 7))		 // ����Σ��Ʒ������������ͨ������һ���
#define			TER_GNSS_PRO					(1<<1) 								 // ֧�ֱ���
#define			TER_GSM_PRO						(1<<0)	 							 //	֧��GPRS


#ifndef 		MSG_ERROR
#define 		MSG_ERROR 							0x02		  // ��Ϣ����
#endif
#ifndef 		NOT_SUPPORT
#define 		NOT_SUPPORT 						0x03		  // ��֧�ֵ���Ϣ
#endif
#ifndef 		MSG_ALARM_CONFIRM
#define 		MSG_ALARM_CONFIRM 				0x04		  // ��������ȷ��
#endif		



// modified 2013.3
#define	 LEN_BASE_POS							28          				 // λ�û�����Ϣ����
#define  LEN_POSITION_REPORT   				85		   					 // λ�û�����Ϣ�͸�����Ϣ�ܳ�	 (Ԥ����������Ϣ�ĳ���)
#define	 LEN_POSITION_REPORT_NOW    		(57+22)         					 // ����ʹ�õ�λ�û㱨��Ϣ���ܳ�  (��ע�����������Լ�·����ʻʱ�䲻��/��������������Ϣ��)                      					
#define  LEN_JT_POS_RETRY           		85        				 //  λ����Ϣ�ط������ݶγ���	Ҫ��λ�û�����Ϣ�͸�����Ϣ�ܳ�һ��
//



#define 	 LEN_AUTH_CODE		  	  				32

#define	 BB_DEFAULT_TIME		  				10
#define  BB_SEND_PIC_TIME      				40
#define  BB_POS_TIME		  	  					60			 // 60�� �ϱ�λ��
#define	 BB_HEARTBEAT_TIME	  				120        // ����ʱ�� ��
#define	 BB_RETRY_TIME			  				2          //  �ش�����

#define	 LEN_MEDIA_EVENT_REPORT      		8          					 // ��ý���¼��ϱ�����
#define	 LEN_MEDIA_SEARCH_FORMAT     		35         					 // ��ý�������ʽ����
#define	 LEN_SET_MEDIA_DATA_UPLOAD   		16   		 					 // �洢��ý�������ϴ�����

#define	 OVERSPEED_TYPE_POS_TYPE_INDEX	(16 + LEN_BASE_POS)         // ���ٱ���������Ϣ�����е�λ������������ַ


#define		IC_CARD_IS_OUT						0x02	   // IC���γ�	 (��ʻԱ�ϰ�)
#define		IC_CARD_INSERT						0x01	   // IC������	 (��ʻԱ�°�)

#define		LEN_GSM_IMEI						15            // IMEI�ų���
#define		LEN_SIM_CARD_ICCID				20			  // ICCID�ų���

#define		PROVINCE_REGION_ID			 	0x00 //0x0044 	// ʡ��  0x44 :�㶫ʡ
#define		PREFECTURE_REGION_ID		 		0x00  //0x0064	 // ���������  0x0064 : ������

#define		JT_DRIVER_NAME_LEN_INDEX	(INDEX_DEVICE_PROTOCOL_DATA_BLOCK+1)	
	


#endif    // 

JT808_EXT		enum
{
	eBB_ACK_OK = 0,
	eBB_ACK_FAILURE,
	eBB_ACK_MSG_ER,
	eBB_ACK_NOT_SUPPORT,
	eBB_ACK_ALARM_ACK,
}eBB_ACK_RESULT;



JT808_EXT		enum
{
	eBB_gFLAG_REG_INDEX,								                       //  jt_recv_flag �е�bit0  λ	  �յ�ע��Ӧ��
	eBB_gFLAG_AUTH_INDEX,														  //	jt_recv_flag �е�bit1  λ	  �յ���ȨӦ��
	eBB_gFLAG_HEART_INDEX,														  //  jt_recv_flag �е�bit2  λ	  �յ�����Ӧ��
	eBB_gFLAG_POS_INDEX,															  //  jt_recv_flag �е�bit3  λ	  �յ�λ���ϱ�Ӧ��
	eBB_gFLAG_OTHER_INDEX,														  //  jt_recv_flag �е�bit4  λ	  �յ���������Ӧ��
   eBB_gFlag_DEREG_INDEX, 
} BB_FLAG_ENUM;


// ������Ϣ
JT808_EXT		enum
{
	eBB_POS_ADD_MIL					        = 0x01,       // λ����Ϣ�ϱ� ���
	eBB_POS_ADD_OIL,								             // ����
	eBB_POS_ADD_SPEED,                                  // �ٶ�
	eBB_POS_ADD_MANUL_ALARM_EVENT_ID,								 // ��Ҫ�˹�ȷ�ϱ����¼���ID
	eBB_POS_ADD_OVER_SPEED_ALARM 			  = 0x11,       // ���ٱ���
	eBB_POS_ADD_INOUT_AREA_ROAD_ALARM,                  // ��������/·�߱���
	eBB_POS_ADD_ROAD_SECTION_DRIVE_ALARM,               // ·����ʻʱ�䲻��/��������
	//added 2013.3
	eBB_POS_ADD_EXT_ALARM					 = 0x24,
	eBB_POS_ADD_EXTEN_VS_STATUS			  = 0x25,	// ��չ�����ź�״̬
	eBB_POS_ADD_IO_STATUS					  = 0x2A,   // IO״̬λ
	eBB_POS_ADD_ANALOG,									// ģ���� bit0-15: AD0; bit16-31, AD1
	eBB_POS_ADD_GSM_SIGNAL					  = 0x30,   // ����ͨ�������ź�ǿ��
	eBB_POS_ADD_GNSS_SATELLITE_NUMS						// GNSS��λ������

} BB_POS_REPORT_ADD_ENUM;

JT808_EXT		enum
{
	eBB_AREA_ALARM_NONE,					// 0�����ض�λ��
	eBB_AREA_ALARM_ROUND,				// 1��Բ������
	eBB_AREA_ALARM_RECT,					// 2����������
	eBB_AREA_ALARM_POLYGON,				// 3: ���������
	eBB_AREA_ALARM_ROAD_SECTION,		// 4��·��
	
} BB_AREA_ALARM_ENUM;


JT808_EXT		enum
{
	eBB_IN_AREA = 0,
	eBB_OUT_AREA = 1,
	eBB_NO_IN_OUT = 2,
}ENUM_IN_OUT_TYPE;



JT808_EXT	  enum
{
	eBB_POS_NORMAL,

	eBB_POS_HAS_ALARM        			     = 0xFA,
	eBB_POS_NETWORK_OFF                   = 0xFB,
	eBB_POS_SEND_SAVED                    = 0xFC,
} BB_POS_SAVE_FLAG;



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	����Э�����ö��
*/
JT808_EXT		enum
{
	/* �ն�(termial)��ϢID */
	eBB_gCMD_TER_ACK 								= 0x0001,	  // �ն�ͨ��Ӧ��
	eBB_gCMD_TER_HEARTBEAT,										  // �ն�����
	eBB_gCMD_TER_DEREG,	 										  // �ն�ע��  
	eBB_gCMD_TER_REG								= 0x0100,	  // �ն�ע��
	eBB_gCMD_TER_AUTH								= 0x0102,		  // �ն˼�Ȩ
	eBB_gCMD_TER_QUERY_PARAM_ACK				= 0x0104,	  // ��ѯ�ն˲���Ӧ��
	eBB_gCMD_TER_QUERY_PRO_ACK					= 0x0107,	  // ��ѯ�ն�����Ӧ�� added 2013.4
	eBB_gCMD_TER_CREATE_USER					= 0x0110,	  // ʹ��ǰ���������ٿ�����
	eBB_gCMD_TER_POS_REPORT 					= 0x0200,	  // λ����Ϣ�㱨
	eBB_gCMD_TER_POS_QUERY_ACK,								  // λ����Ϣ��ѯӦ��
	eBB_gCMD_TER_EVENT_REPORT					= 0x0301,	  // �¼�����
	eBB_gCMD_TER_ASK_ACK,							 			  // ����Ӧ��
	eBB_gCMD_TER_GET_INFO,										  // ��Ϣ�㲥/ȡ��																				  // K
	eBB_gCMD_TER_VEH_CTRL_ACK				   = 0x0500,	  // ����(vehicle)����Ӧ��
	eBB_gCMD_TER_VDR_DATA_UPLOAD           = 0x0700,     // ��ʻ��¼�������ϴ�
	eBB_gCMD_TER_ELE_WAYBILL_UPLOAD,							  // �����˵��ϴ�
	eBB_gCMD_TER_DRIVER_INFO_REPORT,							  // ��ʻԱ�����Ϣ�ɼ��ϱ�
	eBB_gCMD_TER_POS_INFO_MULTI_UPLOAD		= 0x0704,			// ��λ��Ϣ�����ϴ�
	eBB_gCMD_TER_CAN_DATA_UPLOAD		    	= 0x0705,			// CAN���������ϴ�
	eBB_gCMD_TER_MEDIA_EVENT_UPLOAD			= 0x0800,     // ��ý���¼���Ϣ�ϴ�
	eBB_gCMD_TER_MEDIA_DATA_UPLOAD,      					  // ��ý�������ϴ�
	eBB_gCMD_TER_MEDIA_SERCH_ACK,                       // �洢��ý�����ݼ���Ӧ��
	eBB_gCMD_TER_TAKE_PHOTO_ACK				= 0x0805,		// ����ͷ������������Ӧ��
	eBB_gCMD_TER_DATA_TRAN_UPLOAD				= 0x0900,	  // ��������͸�� 
	eBB_gCMD_TER_DATA_COMPRESS_UPLOAD,						  // ����ѹ���ϱ�
	eBB_gCMD_TER_RSA_PUB_KEY					= 0x0A00,     // �ն�RSA��Կ
	

	/* ƽ̨(platform)��ϢID	*/
	eBB_gCMD_PLAT_ACK								= 0x8001,     // ƽ̨ͨ��Ӧ��
	eBB_gCMD_RESEND_PART_PACK_REQUEST		= 0x8003,		//  �����ְ�����	 added  2013.3
	eBB_gCMD_PLAT_TO_TER_REG_ACK				= 0x8100,	  // �ն�ע��Ӧ��		
	eBB_gCMD_PLAT_SET_TER_PARA					= 0x8103,     // �����ն˲���
	eBB_gCMD_PLAT_QUERY_TER_PARA,								  // ��ѯ�ն˲���
	eBB_gCMD_PLAT_CTRL_TER,										  // �ն˿���
	eBB_gCMD_PLAT_QUERY_SOME_TER_PARA,							// ��ѯָ���ն˲���		 addded 2013.3
	eBB_gCMD_PLAT_QUERY_TER_PRO,								// ��ѯ�ն�����			added 2013.4

	eBB_gCMD_PLAT_QUERY_TER_POS				= 0x8201,	  // λ����Ϣ��ѯ
	eBB_gCMD_PLAT_TRACE_TER_POS,						        // ��ʱλ�ø��ٿ���
	eBB_gCMD_PLAT_SEND_TEXT_INFO				= 0x8300,     // �ı���Ϣ�·�
	eBB_gCMD_PLAT_SET_EVENT,									  // �¼�����
	eBB_gCMD_PLAT_SEND_ASK,										  // �����·�
	eBB_gCMD_PLAT_SET_INFO_MENU,								  // ��Ϣ�㲥�˵�����
	eBB_gCMD_PLAT_INFO_SERVICE,								  // ��Ϣ����
	eBB_gCMD_PLAT_TEL_DIAL						= 0x8400,	  // �绰�ذ�
	eBB_gCMD_PLAT_SET_TEL_BOOK,								  // ���õ绰��
	eBB_gCMD_PLAT_VEH_CTRL						= 0x8500,	  // ��������
	eBB_gCMD_PLAT_SET_ROUND_REGION  			= 0x8600,	  // ����Բ������
	eBB_gCMD_PLAT_DEL_ROUND_REGION,							  // ɾ��Բ������
	eBB_gCMD_PLAT_SET_RECT_REGION,							  // ���þ�������
	eBB_gCMD_PLAT_DEL_RECT_REGION,							  // ɾ����������
	eBB_gCMD_PLAT_SET_POLY_REGION,							  // ���ö��������
	eBB_gCMD_PLAT_DEL_POLY_REGION,							  // ɾ�����������
	eBB_gCMD_PLAT_SET_PATH,										  // ����·��
	eBB_gCMD_PLAT_DEL_PATH,										  // ɾ��·��
	eBB_gCMD_PLAT_GET_VDR_DATA					= 0x8700,	  // ��ʻ��¼�����ݲɼ�����
	eBB_gCMD_PLAT_SET_VDR_PARA,						  		  // ��ʻ��¼�ǲ����´�����
	eBB_gCMD_PLAT_DRIVER_INFO_REQUEST,						  // �ϱ���ʻԱ�����Ϣ����
	eBB_gCMD_PLAT_MEDIA_DATA_ACK				= 0x8800,     // ��ý�������ϴ�Ӧ��
	eBB_gCMD_PLAT_TAKE_PHOTO,                            // ����ͷ������������
	eBB_gCMD_PLAT_MEDIA_DATA_SERCH,							  // �洢��ý�����ݼ���
	eBB_gCMD_PLAT_SET_MEDIA_DATA_UPLOAD,					  // �洢��ý�������ϴ�
	eBB_gCMD_PLAT_START_RECORD,								  // ��ʼ¼������
	eBB_gCMD_PALT_ONE_MEDIA_DATA_SERCH_UPLOAD,
	eBB_gCMD_PLAT_SEND_TRAN_DATA				= 0x8900,	  // ��������͸��	
	eBB_gCMD_PLAT_RSA_PUB_KEY					= 0x8A00,	  // ƽ̨RSA��Կ







	/* �Զ�������(platform)��ϢID	*/
	eBB_gCMD_PLAT_LISTEN_QUIET             = 0xA001,     //����
	eBB_gCMD_PLAT_CUT_POWER,									  //�ϵ�
	eBB_gCMD_SET_SIM_NUMBER,                             //�����豸SIM������
	eBB_gCMD_GET_SIM_NUMBER,                             //��ѯ�豸SIM������
	eBB_gCMD_SET_ADSW_FLAG                 = 0xA005,     //����ADͨ��������������/�ͼ���־
	eBB_gCMD_GET_ADSW_FLAG,                              //��ѯADͨ��������������/�ͼ���־
	eBB_gCMD_GET_VERSION_MSG,                            //��ѯ�豸�汾����Ϣ
	eBB_gCMD_I2C_DRIVER_INFO_REQUEST       = 0xA008,	  // ��ѯ�ϱ�ʹ��I2C�����ļ�ʻԱ�����Ϣ����(��ͨIC��)




   /* �Զ�������(platform)��ϢID	*/
	eBB_gCMD_TPMS_DATA_UPLOAD				   = 0x2001,		// ��̥��ѹ�����ϱ�
	eBB_gCMD_SYS_LOG_UPLOAD				   	= 0x2002,		// ϵͳ��־�����ϱ�
	eBB_gCMD_GET_SIM_NUMBER_ACK            =0x2004,        //��ѯ�豸SIM������Ӧ��
	eBB_gCMD_GET_ADSW_FLAG_ACK             =0x2006,        //��ѯADͨ��������������/�ͼ���־��Ӧ��
	eBB_gCMD_GET_VERSION_MSG_ACK			   =0x2007,			 //��ѯ�豸�汾����ϢӦ��
	eBB_gCMD_I2C_DRIVER_INFO_REPORT		   = 0x2008,	  //  �����ϱ�I2C�����ļ�ʻԱ�����Ϣ�ɼ��ϱ�	����ͨIC����

	
		
} BB_CMD_ENUM;


JT808_EXT		enum
{
	/* �ն�(termial)��ϢID */
	SET_HEART_BEAT_ID 							   = 0x0001,	     // �ն��������ͼ��
	SET_TCP_ACK_OVER_TIME_ID,										     // TCPӦ��ʱʱ��
	SET_TCP_RESEND_TIMES_ID,								           //TCP�ش�����
	SET_UDP_ACK_OVER_TIME_ID,											  //  UDPӦ��ʱʱ��
	SET_UDP_RESEND_TIMES_ID,											  // UDP�ش�����
	SET_SMS_ACK_OVER_TIME_ID,											  //  UDPӦ��ʱʱ��
	SET_SMS_RESEND_TIMES_ID,											  // UDP�ش�����
	
	SET_M_APN_ID								   	= 0x0010,		     //  ��������APN
	SET_M_USE_NAME_ID,											        // ���������û���
	SET_M_USE_SECRIERS_ID,							 			        // ����������������
	SET_M_IP_DNS_ID,											           // ��������IP��ַ������
	SET_B_APN_ID,											              //  ���ݷ�����APN
	SET_B_USE_NAME_ID,											        // ���ݷ������û���
	SET_B_USE_SECRIERS_ID,							 			        // ���ݷ�������������
   	SET_B_IP_DNS_ID,											           // ���ݷ�����IP��ַ������
	SET_SERVER_TCP_PORT_ID,							 			        // ������TCP�˿�
  	SET_SERVER_UDP_PORT_ID,											     // ������UDP�˿�				

   SET_SERVER_IC_AUTH_MAIN_IP,											// ��·����֤IC ����֤��������IP��ַ������
	SET_SERVER_IC_AUTH_TCP,											    // ��·����֤IC ����֤��������TCP�˿�
	SET_SERVER_IC_AUTH_UDP,											    // ��·����֤IC ����֤��������UDP�˿�
	SET_SERVER_IC_AUTH_BAK_IP,											// ��·����֤IC ����֤���ݷ�����IP��ַ���������˿�ͬ��������
   	

	SET_POSITION_UP_MODES_ID				   	= 0x0020,			  // �ر�����
	SET_POSITION_UP_PORJECT_ID,							 			  // λ�ûر�����
	SET_POSITION_UP_TIME_NO_DRIVER_ID,										  // ��ʻԱδ��¼�ر�ʱ����

	SET_POSITION_UP_TIME_IDEL_ID					= 0x0027,			  //����ʱ�ر�ʱ����
    SET_POSITION_UP_TIME_ALARM_ID,											  //��������ʱ��ر����
	SET_POSITION_UP_TIME_DEFAULT_ID,									  //ȱʡ��ʱ�ر����
	
	
	SET_POSITION_UP_DISTANCE_DEFAULT_ID	   	= 0x002C,			  //ȱʡ����ر����
	SET_POSITION_UP_DISTANCE_NO_DRIVER_ID,										  // ��ʻԱδ��¼�ر�������
	SET_POSITION_UP_DISTANCE_IDEL_ID,				 			  //����ʱ�ر�������
    SET_POSITION_UP_DISTANCE_ALARM_ID,							  //���������ر�������
	SET_BREAK_POINT_ANGLE_ID,										  //�սǲ����Ƕ�

	SET_ELE_RAID_RADIUS_ID						    = 0x0031,		// ����Χ���뾶	���Ƿ�λ����ֵ�� ����λΪ��	  add 2013.3

	SET_PHONE_MONITOR_CENTER_ID					= 0x0040,	  //���ƽ̨����
	SET_PHONE_RESET_ID,									              //��λ����
	SET_PHONE_RESET_FACTORY_ID,									     //�ָ��������ú���
	SET_PHONE_SMS_MONITOR_CENTER_ID,								  //���ƽ̨SMS����
	SET_PHONE_TEXT_SMS_ALARM_ID,									  //�����ı�SMS��������
	SET_PHONE_GET_MODES_ID,											  //�绰��ͨ����
	SET_PHONE_ONLINE_TIME_TIMES_LIMIT_ID,						  //ÿ���ͨ��ʱ��
	SET_PHONE_ONLINE_TIME_MONTH_LIMIT_ID,						  //���������ͨ��ʱ��
	SET_PHONE_MONITORING_ID,											  //�����绰����
	SET_PHONE_PRIVILEGE_SMS_ID,									  //��Ȩ���ź���


	SET_WORD_FOR_NO_ALARM_ID						= 0x0050,	  //����������
	SET_WORD_FOR_TEST_SMS_ALARM_ID,								  //���������ı�SMS����
	SET_WORD_FOR_TAKE_PHOTO_ALARM_ID,								  //�������տ���
	SET_WORD_FOR_TAKE_PHOTO_SAVE_ALARM_ID,						  //�������ձ��濪��
	SET_WORD_FOR_KEY_ALARM_ID,										  //�ؼ�������־
	SET_SPEED_HITH_LIMIT_ID,											  //����ٶ�
	SET_TIME_OVER_LIMIT_SPEED_ID,									  //���ٳ���ʱ��
	SET_TIME_DRIVEING_LIMIT_ID,										  //������ʻʱ������
	SET_TIME_DRIVEING_LIMIT_ONE_DAY_ID,							  //һ���ۼƼ�ʻʱ������
	SET_TIME_REST_ID,													  //��С��Ϣʱ��
	SET_TIME_STOP_CAR_ID,												  //�ͣ��ʱ��

	// add 2013.3
	SET_OVER_SPEED_PRE_ALARM_ID						= 0x005B,	 // ���ٱ���Ԥ����ֵ 
	SET_TRIAL_FATIGUE_DRIVE_PRE_ALARM_ID,							 // ƣ�ͼ�ʻԤ����ֵ
	SET_CRASH_ALARM_PARA_ID,									 // ��ײ�����������ã�bit7-bit0: ��ײʱ�䣬��λ��4ms;bit15-bit8:��ײ���٣���λ 0.1g, ���÷�Χ:0 - 79֮��,Ĭ��Ϊ10
	SET_TILT_ALARM_PARA_ID,										 // �෭�����������ã� �෭�Ƕȣ���λ1�ȣ�Ĭ��Ϊ30��

    SET_FIXED_TIME_TAKE_PHOTO_CTL_ID				 = 0x0064,	 // ��ʱ���տ���
	SET_DISTANCE_TAKE_PHOTO_CTL_ID,					             // �������տ���																										 
	// 

	SET_VIDIO_QUOLITY_ID						      = 0x0070,	  //ͼ��/��Ƭ����
	SET_VIDIO_BRIGHTNESS_ID,										     //����
	SET_VIDIO_CONTRAST_ID,										     //�Աȶ�
	SET_VIDIO_SATULATION_ID,										     //���Ͷ�
	SET_VIDIO_CHROMA_ID,											     //ɫ��


	SET_ODOMETER_VALUS_ID						   = 0x0080,	  //��̱����
	SET_PROVINCIAL_ID,													  //ʡ��
	SET_CITY_WHERE_ID,													  //����
	SET_CAR_NUBER_ID,													  //���ƺ���
	SET_CAR_COLOUR_ID,													  //������ɫ

	// add 2013.3
	SET_GNNS_POS_MODE_ID						   = 0x0090,	// GNSS ��λģʽ
	SET_GNSS_BAUD_RATE_ID,										// GNSS������
	SET_GNSS_DATA_OUTPUT_FREQUENCY_ID,							// GNSSģ����ϸ��λ�������Ƶ��
	SET_GNSS_DATA_COLLECT_FREQUENCY_ID,							// GNSSģ����ϸ��λ���ݲɼ�Ƶ��
	SET_GNSS_DATA_UPLOAD_MODE_ID,							    // GNSSģ����ϸ��λ�����ϴ���ʽ
	SET_GNSS_DATA_UPLOAD_SET_ID,							    // GNSSģ����ϸ��λ�����ϴ�����
	
	SET_CAN_CHAN_1_COLLECT_TIME_INTERVAL_ID    	= 0x0100,         // CAN����ͨ��1 �ɼ�ʱ����(ms)��0��ʾ���ɼ�
	SET_CAN_CHAN_1_UPLOAD_TIME_INTERVAL_ID,			        	// CAN����ͨ��1 �ϴ�ʱ����(ms)��0��ʾ���ɼ�
	SET_CAN_CHAN_2_COLLECT_TIME_INTERVAL_ID,         			// CAN����ͨ��2 �ɼ�ʱ����(ms)��0��ʾ���ɼ�
	SET_CAN_CHAN_2_UPLOAD_TIME_INTERVAL_ID,			        	// CAN����ͨ��2 �ϴ�ʱ����(ms)��0��ʾ���ɼ�
	SET_CAN_ID_COLLECT_SET_0_ID				  		= 0x0110,			// CAN����ID�����ɼ�����0
	SET_CAN_ID_COLLECT_SET_15_ID			  			= 0x0120,			// CAN����ID�����ɼ�����15

	SET_ACC_PARA											= 0xF00C,			//	���ٶ�
	SET_ACC_DEC_PARA										= 0xF00D,			// ���ٶ�
	// 															

		
} JT808_SET_PARA_ID;



#pragma		pack(1)
typedef		struct	_JT808_MSG_HEAD_
{	
	uint16	Cmd;
	union
	{
		uint16	B16;
		struct
		{
			uint16	DataLen:10;
			uchar		Encrypt:3;
			uchar		MultiPack:1;
			uchar		Rsv:2;
		}Bits;
	} Attr;	
	uchar		SimNum[6];
	uint16	Index;
	uint16	TotalPack;
	uint16	PackIndex;	
} JT808_MSG_HEAD,*pJT808_MSG_HEAD;




typedef		struct	_JT808_MSG_FRAME_
{	
	uchar		Head;
	uint16	Cmd;
	union	
	{
		uint16	B16;
		struct
		{
			uint16	DataLen:10;
			uchar		Encrypt:3;
			uchar		MultiPack:1;
			uchar		Rsv:2;
		}Bits;
	} Attr;	
	uchar		SimNum[6];
	uint16	Index;
	union
	{
		uchar		Buffer[1024];
		struct
		{
			uint16	TotalPack;
			uint16	PackIndex;	
		}PackNo;
	}Data;
} JT808_MSG_FRAME,*pJT808_MSG_FRAME;










#pragma		pack(1)
typedef	struct	_JT808_DEVICE_PROTOCOL_HEAD_
{
	uchar 	CheckSum;
	uint16  VersionNo;
	uint16	ManufacturerNo;
	uchar	DeviceTypeNo;
	uchar	Cmd;
	
}  JT808_DEVICE_PROTOCOL_HEAD,*pJT808_DEVICE_PROTOCOL_HEAD;

#define	INDEX_DEVICE_PROTOCOL_DATA_BLOCK	8      // ������ͨѶЭ���е����ݶ��±�			


typedef struct _JT808_COMMON_ACK_
{
	uint16 Index;
	uint16 MsgId;
	uint16 Result;
	
} JT808_COM_ACK, *pJT808_COM_ACK;

#pragma		pack(1)
typedef struct _JT808_MULTY_MEDIA_REPORT_
{
	uint32	MediaId;
	uchar		MediaType;
	uchar		MediaFormat;
	uchar		MediaEvent;
	uchar		MediaChannel;
} JT808_MULTY_MEDIA_REPORT, *pJT808_MULTY_MEDIA_REPORT;



typedef	struct _JT808_CAMERA_TAKE_PHOTO_
{
	uchar		Channel;				// ͨ��ID
	uint16		Cmd;					//	�������� 0: ֹͣ ; 0xFFFF: ¼��; �������������� 
	uint16		interal;				// ���ռ��/¼��ʱ��   ��λΪ�룬 0: ��С������ջ�һֱ¼��
	uchar		SaveFlag;			// 1�� ����; 0�� ʵʱ�ϴ�
	uchar		Res;					// �ֱ��ʣ� 0x01: 320 * 240; 0x02: 640 * 480; 0x03: 800 * 600; 0x04: 1024 * 768 ...
	uchar		Qulity;				// ͼ��/��Ƶ����  1 - 10�� 1: ������ʧ��С; 10: ѹ�������
	uchar		Bright;				// ����	  0 ~ 255
	uchar		Contrast;			// �Աȶ�  0 ~ 127
	uchar		Saturation;			// ���Ͷ�  0 ~	127
	uchar		Chroma;				// ɫ��    0 ~ 255
	
}  JT808_CAMERA_TAKE_PHOTO, *pJT808_CAMERA_TAKE_PHOTO;



JT808_EXT struct _RETRY_SEND_PIC_ 
{
	uchar		SendPicNo; 
	uchar		TotalPack; 
	union {
		uchar		SendPackNo[125];
		uchar		MediaId[125];
	} SendPic;
	uchar		Index;	

}JtSendPic;

#define		LEN_TAKE_PHOTO_ACK		10
JT808_EXT struct __TAKE_PHOTO_ACK__
{
	uchar	MediaNum;
	uchar	MediaID[LEN_TAKE_PHOTO_ACK];
} TakePhotoAck;



JT808_EXT union  	_MULTY_MEDIA_DATA_
{
	 //  �洢��ý�����ݼ���
	 struct					  
	 {
	 	uchar		MediaType;
		uchar	 	Channel;
		uchar		Event;
		uchar	   StartTime[6];	  // BCD��	 YY-MM-DD-hh-mm-ss
		uchar		EndTime[6];      // BCD��	 YY-MM-DD-hh-mm-ss
	 	
	 }SavedMediaSearch;	        

	 // �洢��ý�������ϴ�
	 struct
	 {
	 	uchar		MediaType;
		uchar	 	Channel;
		uchar		Event;
		uchar	   StartTime[6];	  // BCD��	 YY-MM-DD-hh-mm-ss
		uchar		EndTime[6];      // BCD��	 YY-MM-DD-hh-mm-ss
		uchar		DeleteFlag;
	 	
	 }SavedMediaUpload;
	

} MulMeDa;



 // ��ý����������ݸ�ʽ
#define		LEN_MEDIA_SEARCH_STRUCT		35
typedef struct  
{
	uint32	MediaId;
	uchar		MediaType;
	uchar		Channel;
	uchar		Event;
	uchar		LocationInfo[28];          // λ�û㱨��Ϣ��
} MEDIA_SEARCH_STRUCT,*pMEDIA_SEARCH_STRUCT;


JT808_EXT struct 						 // ����͸��
{
	uint16	Len;
	uchar		*ptype_and_data;		// ptype_and_data	ָ��ĵ�һ�ֽ�Ϊ͸����Ϣ���ͣ��ڶ��ֽ���Ϊ͸������		
} JT_tran_upload;
 

/*
JT808_EXT  struct
{
	uint16  pos_count;			// ����ͨ���쳣λ����Ϣ����
	uint16  alarm_count; 	   // ����ؼ�����λ����Ϣ����
	uint16  send_pos_count;		// ����ͨ���쳣λ����Ϣ����
	uint16  send_alarm_count;	// ���͹ؼ�����λ����Ϣ����
} JT_saved_pos;					// ����λ����Ϣ
*/


/*JT808_EXT  struct
{
	uchar   Length;
	uchar   Data[LEN_JT_POS_RETRY];
}JTPosAckBuf;	*/

JT808_EXT  JT808_MULTY_MEDIA_REPORT  MultiMediaReport;

JT808_EXT  uchar MediaEventFlag;


// added 2013.3
typedef union {
	uchar		AllData[104];
	struct 
	{
		uchar Stauts;   // 0x01: ��ҵ�ʸ�֤IC�����루��ʻԱ�ϰࣩ; 0x02: ��ҵ�ʸ�֤IC���γ�����ʻԱ�°ࣩ.
		uchar TimeBCD[6];     // ʱ��
		uchar ReadICResult;    //IC����ȡ���
		uchar LenOfDriverName; // ��ʻԱ��������
		uchar DriverName[20];      // ��ʻԱ����
		uchar DirverID[20];       // ��ҵ�ʸ�֤����
		uchar LenOfDepartmentName;     //  ��֤�������Ƴ���
		uchar DepartmentName[50]; //   ��֤��������
		uchar PapersValidTimeBCD[4];       // ֤����Ч��	   YYYYMMDD
		
	} DriverICInfo;
} DRIVERICINFO;	


//

// added 2013.4
#pragma		pack(1)
typedef	struct __QUERY_TER_PRO__
{
	uchar	Type;				// �ն�����		  ��JT808����˵����2013.1.18���涨1�ֽ�
	uchar	ProductorID[5];		// ������ID
	uchar	Model[20];			// �ն��ͺ�
	uchar	TerID[7];			// �ն�ID
	uchar	SimCardICCID[10];	// �ն�SIM��   BCD��
	uchar	LenHardVer;			// �ն�Ӳ���汾�ų���
	uchar	HardVer[8];		// �ն�Ӳ���汾��
	uchar	LenFirmwareVer;		// �ն˹̼��汾�ų���
	uchar	FirmwareVer[8];    // �ն˹̼��汾��
	uchar	GNSSPro;			// GNSSģ������
	uchar	GSMPro;				// ͨ��ģ������					 
	
}QUERY_TER_PRO,*pQUERY_TER_PRO;
//


// added 2013.5
#pragma		pack(1)
typedef struct  __JT_CAN_MSG__ { 		
  uint32	CAN_ID;                
  uchar  CAN_Data[8];           
} JT_CAN_MSG;

#define	LEN_JT_CAN_MSG		12

//  CAN ���������ϴ����ݸ�ʽ
typedef struct  __JT_CAN_DATA_UPLOAD__ { 		
  uint16	CanItemSum;                	   // 	 ������CAN���������������>0 
  uchar  	CanFirstRecTimeBCD[5];          //    ��1��CAN�������ݵĽ���ʱ�䣬hh-mm-ss-msms
  JT_CAN_MSG CanItem[16];							
} JT_CAN_DATA_UPLOAD,*pJT_CAN_DATA_UPLOAD;



//JT808_EXT JT_CAN_DATA_UPLOAD  JT_CanDataUpload;


 // CAN��������
JT808_EXT  struct					  
{
	uint32	Can1SampleInterval;				 // CAN����ͨ��1�ɼ�ʱ���� (ms)
	uint16 	Can1UploadInterval;				 // CAN����ͨ��1�ϴ�ʱ���� (s)
	uint32	Can2SampleInterval;				 // CAN����ͨ��2�ɼ�ʱ���� (ms)
	uint16 	Can2UploadInterval;				 // CAN����ͨ��2�ϴ�ʱ���� (s)
}JT_CanSet;



JT808_EXT enum 
{
	CONNECT_AUTH_CHECK_ONLINE			=	  0xa1,		   // ����Ƿ����ߣ�δ���ߵȴ��ﵽ��ʱ����
    CONNECT_AUTH_CONNECT_NO_WAIT  	=     0xa2,		   //  �����ӣ�������Ƿ�����
	CONNECT_AUTH_WAIT						=	  0xa3,		   // ������֤���ģ����г�ʱ����

	CHECK_CENTER_ONLINE					= 	  0xa4,   	   // ����Ƿ����ߣ�δ���ߵȴ��ﵽ��ʱ����
	CONNECT_CENTER_WAIT,							   //  �����ӣ�������Ƿ�����
	CONNECT_CENTER_NO_WAIT,							   // �������ģ����г�ʱ����
} JT_CONNECT_TYPE_ENUM;





#define		CONNECT_CREATE_USER_CENTER		'A'
#define		CONNECT_MAIN_CENTER				'B' 
#define		CONNECT_AUTH_CENTER				'C'

///////////////  ʹ��ǰ����
//#define		NEED_CREATE_USER					0xfa	

#define		NEED_CREATE_USER					OK	
	

// ʹ��ǰ���������ٿ��������ܷ�ʽ
JT808_EXT  struct
{
	uchar	Flag;
	uint32	Key;
}JTCreatUserEncry;

JT808_EXT  uchar CarUserPhoneNumber[12];				// 
JT808_EXT  uchar CreateUserFlag, Creat_User_Reging;		 //   ������־�� ��������ע���־
JT808_EXT  uint32 Creat_User_Reging_OverTime;	


JT808_EXT uchar	AuthServerOverTime;			// ��Ȩ��ʱ����

JT808_EXT  uchar	JT_RemoteReadRecodMoreDataCmd;    // Զ�̶�ȡ��¼�����ݣ������־��Ҫ���ڷ��ʹ�����


JT808_EXT  struct					 // 08H - 15H ���ݼ�����ýṹ��
{
	uint16 TotalPack;					// �ܰ�
	uint16 PackIndex;					// ����

}JT_VehRecord;	  // �����������¼�����ݽṹ



JT808_EXT		uint32  WithoutAckOfServer;	  //�������޻ظ�����
JT808_EXT uchar	RegisterWithoutTer;
JT808_EXT uint32 RestartWithoutAckOfServer;
/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */

