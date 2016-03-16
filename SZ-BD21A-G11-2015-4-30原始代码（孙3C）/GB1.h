/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												Фң���ӣ����ݣ��з�����
$										Shawyoo Electronics (GuangZhou) R&D Center
$                       	http://www.shawyoo.com		 Email: autoele@qq.com
$								 QQȺ��9069471/9069433/9524253		All rights reserved.	  
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

#ifndef      __GB_H__
#define      __GB_H__

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
#define		GET_SPECIAL_NF_DATA			0x3c	// �ɼ�ָ��NAND Flash��ַ�ͳ��ȵ�����
#define		GET_SPECIAL_FM_DATA			0x3e	// �ɼ�ָ�������ַ�ͳ��ȵ�����
#define		GET_ALL_DATA					0x3f	// �ɼ���������



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
#define		SET_SPECIAL_FM_DATA			0xae	// ָ����ַ�ͳ��ȣ�д������
#define		SET_INIT_SYS					0xaf	// ��ʼ���ն˴洢�������ã���

////////////   �������� ////////////////////////////////////////////////////////////
#define		SET_KEY_CODE					0xb0	// ģ����̲���

		


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	  			�����������¼�������еĴ洢��ַ
				OPR -- OPeration Recorder
*/
/////////	����������	//////////////////////////////////////////////////////////
#define		OPR_GET_DRIVER_CODE			0x100	// *��ʻԱ���뼰���Ӧ�Ļ�������ʻ֤��
#define		OPR_GET_REAL_TIME				0x108	// *
#define		OPR_GET_MILAGE_360H			0x110	// *���360h�ڵ��ۼ���ʻ�������
#define		OPR_GET_VEHICLE_CC			0x118	// *Vehicle Characteristic Coefficients
#define		OPR_GET_SPEED_360H			0x120	// *���360h�ڵ���ʻ�ٶ����ݣ�ÿ����һ����
#define		OPR_GET_VEHICLE_VIN			0x128	// *����VIN�ţ����ƺ��룬��������
#define		OPR_GET_STOP_DATA				0x130	// *�¹��ɵ����ݣ�ͣ��ǰ���ݣ�
#define		OPR_GET_MILAGE_2DAY			0x138	// ���2���������ڵ��ۼ���ʻ���
#define		OPR_GET_SPEED_2DAY			0x140	// ���2���������ڵ���ʻ�ٶ�(ÿ����һ��)
#define		OPR_GET_OVER_TIME_3H			0x148	// ƣ�ͼ�ʻ����
///////// 	���У�����涨������ 0x01~0x07��7�������������������ʾ����ʾ
/////////	���ظ�������ǹ��꣩ ///////////////////////////////////////////////
#define		OPR_GET_SENSOR_SPEED			0x150	// �ɼ�������ÿ�������������ٶ����ޡ��Ӽ��ٶ�����
#define		OPR_GET_USER_VEHICLE_NUM	0x158	// �û�������ţ�4�ֽ�10�����ַ�
#define		OPR_GET_INTERVAL				0x160	// �ɼ���¼ʱ�����򱨾�ʱ������ֵ
#define		OPR_GET_SOLE_NUMBER			0x168	// ��ƷΨһ���к�
#define		OPR_GET_OVER_SPEED			0x170	// ��������
#define		OPR_GET_OP_RECORD				0x178	// �������������¼
#define		OPR_GET_USER_FLAG				0x180	// �û���ʶ
#define		OPR_GET_ALL_DATA				0x188	// �ɼ���������
////////////	����������	//////////////////////////////////////////////////////////
#define		OPR_SET_DRIVER_CODE			0x190	// ���ü�ʻԱ���룬��ʻ֤����
#define		OPR_SET_VEHICLE_VIN			0x198	// ���ó���VIN�ţ����ƺţ���������
#define		OPR_SET_REAL_TIME				0x1a0	// ����ʵʱʱ��
#define		OPR_SET_VEHICLE_CC			0x1a8	// ���ó�������ϵ��
////////////  ���ظ�������ǹ��꣩ ////////////////////////////////////////////////
#define		OPR_SET_SENSOR_SPEED			0x1b0	// ���ô�����ÿ�������������ٶ����ޡ��Ӽ��ٶ�����
#define		OPR_SET_USER_VEHICLE_NUM	0x1b8	// �����û�������ţ�4�ֽ�10�����ַ�
#define		OPR_SET_INTERVAL				0x1c0	// ���ü�¼ʱ�����򱨾�ʱ������ֵ
#define		OPR_SET_SOLE_NUMBER			0x1c8	// ���ò�ƷΨһ���к�
#define		OPR_SET_INIT_SYS				0x1d0	// ��ʼ���ն˴洢ϵͳ�����ã���
#define		OPR_SET_USER_FLAG				0x1d8	// �����û���ʶ

#define		OPR_SET_KEY_CODE				0x1e0	// ģ����̲���


////////////	������	////////////////////////////////////////////////////////////
#define		RESERVER_WORD				0x00


#define		TIMES_STOP_DATA			10		// ��¼ͣ��ǰ���ݵĴ���


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	  			����������洢���е���ʼ��ַ(�͵�ַ��ǰ)
*/
////////////	����������洢���е���ʼ��ַ  ///////////////////////////////
#define		FM_VEHICLE_VIN				0x03		// ����VIN��
#define		FM_VEHICLE_CODE			0x14		// ���ƺ���
#define		FM_VEHICLE_CLASS			0x20		// ��������
#define		FM_USER_VEHICLE_NUM		0x2c		// �û��������
#define		FM_FILE_TYPE1				0x30		// �ļ����ʹ洢��ʼ��ַ1
#define		FM_FILE_TYPE2				0x38		// �ļ����ʹ洢��ַ2
#define		FM_VEHICLE_CC				0x40		// Vehicle Characteristic Coefficients
#define		FM_SENSOR_KV				0x46		// ������ÿ����������(Kֵ)

#define		FM_DRIVER_CODE				0x50		// ��ʻԱ���뼰���Ӧ�Ļ�������ʻ֤��
#define		FM_RUN_TIME_START			0x65		// ��ƣ�ͼ�ʻ��ʻ�Ŀ�ʼʱ��
#define		FM_TIME_SPEED				0x70		// ��¼ʱ�����򱨾�ʱ������ֵ
#define		FM_SPEED_LIMIT				0x74		// ����ֵ
#define		FM_ACC_LIMIT				0x75		// �޼��ٶ�

#define		FM_INSTALL_DATE			0x78		// ���ΰ�װ����(������ʱ����)
#define		FM_LAST_SECOND				0x7e		// ��¼������״̬�����һ����(������ʱ����)
#define		FM_SYSTEM_START_CNT		0x88		// ϵͳ�����ܴ��������һ������ʱ��
//#define		FM_SS_LAST_ONE				0x8a		// ϵͳ���һ����������ʱ��
#define		FM_USER_FLAG   			0xb0		// �û���ʶ�洢��ʼ��ַ
#define		FM_SOLE_NUMBER				0xd0		// ��ƷΨһ���к�
#define		FM_OEM_FLAG					0xe0		// �����̱�־

#define		FM_OP_RECORD				0x100		// �����������¼
#define		FM_MILAGE_360H_BCD		0x200		// ���360h�ڵ��ۼ���ʻ�������(BCD��ʱ��)
#define		FM_MILAGE_360H_HEX		0x208		// �պõ�360h���ۼ���ʻ������ݣ�3�ֽ�Hex��
#define		FM_TOTAL_SPEED_PULS		0x20c		// �Ӱ�װ���ٶȴ�������������������

#define		FM_MILAGE_2DAY				0x210		// ���2���������ڵ��ۼ���ʻ���
#define		FM_PULS_YESTODAY			0x218		// ������ٶȴ�����������
#define		FM_PULS_TODAY				0x21c		// ������ٶȴ�����������

#define		FM_RUN_STATUS_CNT			0x220		// ��ʻʱ��(����)������(2�ֽ�)
#define		FM_STOP_TIME_CNT			0x222		// ͣ��ʱ��(����)������(2�ֽ�)
#define		FM_TIRE_DRIVE_FLAG		0x224		// ����ƣ�ͼ�ʻ��־��ƣ��Ϊ��0��
#define		FM_OVER_TIME_CNT			0x226		// ��ʱ�ܴ���������
#define		FM_OVER_TIME_REC			0x228		// ��ʱ��¼������ʼ��ַ

#define		FM_OVER_SPEED_CNT			0x4ae		// ���ٴ���������
#define		FM_OVER_SPEED_REC			0x4b0		// ���ټ�¼


#define		FM_STOP_TIMS_CNT			0x730		// ���ڵڼ���ͣ����2�ֽ�
#define		FM_20S_DP					0x732		// 20�����ݵĵ�ǰ�洢��ַָ��
#define		FM_20S_CNT					0x734		// ������20���еĵڼ���
#define		FM_LAST_5S_PULS			0x736		// ���5����ÿ���������
#define		FM_LAST_23S_DATA			0x740		// ���23�����ݴ洢��ַ�������Ƿ�ͣ����
#define		FM_STOP_DATA				0x82e		// �¹��ɵ����ݣ�ͣ��ǰ���ݣ�
#define		FM_20S_DATA_START_ADDR	0x82e		// 20S���ݿ�ʼ�洢��ַ
#define		FM_20S_DATA_DRIVER		0x103a	// 20S���ݶ�Ӧ�ļ�ʻ֤����
												
#define		FM_KV_OP_REC_CNT			0x10fe	// ��������ϵ�����ش���������
#define		FM_KV_OP_REC				0x1100	// ��������ϵ�����ؼ�¼(100����1200�ֽ�)

#define		FM_ID_CARD_REC_CNT		0x142e	// ������ʻԱ������
#define		FM_ID_CARD_REC				0x1430	// ��ʻԱֵ���¼(��ʻ֤��+ˢ��ʱ��)

// ���5����ʻԱ��ʻ֤�ţ�ˢ��ʱ�䣬��ʻʱ���ͣ��ʱ��
#define		FM_LAST_5_DRIVERS			0x2280	// 	

#define		FM_LAST_15MIN_DRIVER		0x23a0	// ���15���Ӷ�Ӧ�ļ�ʻԱ����
#define		FM_STOP_15MIN_DRIVER		0x23cd	// ͣ��ǰ15���Ӷ�Ӧ�ļ�ʻԱ����
#define		FM_LAST_15MIN_DRIVER_CNT 0x23fa	// ���15���Ӽ�����

#define		FM_15MIN_AVG_SPEED		0x2400	// ͣ��ǰ15������ÿ���ӵ�ƽ������
#define		FM_LAST_STOP_RTC			0x240f	// ���һ��ͣ��ʱ�̵�����
#define		FM_STOP_DRIVER_CODE		0x2415	// ͣ��ʱ�̼�ʻԱ����(�޼�ʻ֤��)
#define		FM_1H_AVG_START			0x2418	// ���1Сʱ����ʼ���ں�ʱ��������ʱ��
#define		FM_1H_AVG_SPEED_MIN		0x241d	// ���1Сʱ��ÿ���ӵ�ƽ���ٶ�
#define		FM_360H_CNT					0x2460	// ������360Сʱ�ĵڼ���Сʱ
#define		FM_360H_MIN_CNT			0x2464	// 360Сʱ�ڵķ��Ӽ�����(4�ֽ�)
#define		FM_LAST_15MIN_AVG_SPEED	0x2468	// ���15����ÿ���ӵ�ƽ���ٶ�

#define		FM_SPEED_360H				0x2488	// ���360Сʱ��ÿ���ӵ�ƽ���ٶ�
															// ~ 0x7ff0 23400�ֽ�Ϊ360Сʱ�ٶȿ�����





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	  			���ֲ����ĳ����ֽ���
*/
#define		LEN_DRIVER_CODE			21		// ��ʻԱ���뼰���Ӧ�Ļ�������ʻ֤��
#define		LEN_MILAGE_360H			3		// ���360h�ڵ��ۼ���ʻ�������
#define		LEN_VEHICLE_CC				3		// Vehicle Characteristic Coefficients
#define		LEN_VEHICLE_VCC			41		// ����VIN�ţ����ƺ��룬��������
#define		LEN_VEHICLE_VIN			17		// ����VIN��
#define		LEN_VEHICLE_CODE			12		// ���ƺ���
#define		LEN_VEHICLE_CLASS			12		// ��������
#define		LEN_RTC						6		// RTC

#define		LEN_SENSOR_KV				2		// �ɼ�������ÿ����������
#define		LEN_USER_VEHICLE_NUM		4		// �û�������ţ�4�ֽ�10�����ַ�
#define		LEN_ABOUT_TIME				4		// ʱ�����ֵ
#define		LEN_TIME_SPEED				6		// ʱ�����ֵ���ٶ����ޡ��Ӽ��ٶ�����
#define		LEN_SOLE_NUMBER			16		// ��ƷΨһ���к�
#define		LEN_OVER_SPEED				645	// ��������
#define		LEN_OP_RECORD				256	// �������������¼
#define		LEN_USER_FLAG				32		// �û���ʶ
#define		LEN_ALL_DATA				0x7000// �ɼ���������

#define		LEN_OVER_TIME_1REC		28		// ƣ�ͼ�ʻ��¼ÿ����Ϣ�����ֽ�
#define		LEN_OVER_SPEED_1REC		32		// ���ټ�¼ÿ����Ϣ�����ֽ�
#define		LEN_ID_CARD_1REC			24		// ��ʻԱ�����֤�洢�ĳ���(֤��+�忨ʱ��)
#define		LEN_RUN_STATUS_CNT		6		// ������ʻֹͣ��ƣ�ͼ�ʻ��־λ�ĳ���

#define		LEN_LAST_5_DRIVERS_1REC	32		// ���5����ʻԱ���˴Σ���ʻ��Ϣÿ�ڵ��ֽڳ���

#define		LEN_KV_OP_1REC				8		// ��������ϵ�����ز�����¼һ���ĳ���
#define		LEN_KV_OP_REC				800	// ��������ϵ�����ز�����¼�Ĵ洢����

#define		LEN_360H_1REC				65		// ÿСʱ�ٶȿ������ֽڳ���

#define		LEN_OVER_TIME				645	// ��ʱ���ݿ鳤��

#define		LEN_INSTALL_DATE			6		// ���ΰ�װ���ں�ʱ��

#define		LEN_20S_DATA				206	// һ��20S���ݳ���
#define		LEN_STOP_DATA				(TIMES_STOP_DATA*LEN_20S_DATA)	// �¹��ɵ����ݳ���
#define		LEN_20S_DATA_DRIVER	   180	// 10���ɵ����ݶ�Ӧ��10����ʻ֤�ŵ��ܳ���
#define		LEN_MILAGE_2DAY			3		// ���2���������ڵ��ۼ���ʻ���
#define		LEN_360H_DATA				(65*360)	// 360Сʱ���ٶ�����



#define		FM_CAPACITY					32768		// �����������ֽڣ�

////////////	�����ⲿ����	/////////////////////////////////////////////////////

 
 

#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */