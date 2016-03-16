#ifndef      __USER_GLOBASL_H__
#define      __USER_GLOBASL_H__

#ifdef      USER_GLOBALS
#define     USER_EXT
#else
#define     USER_EXT    extern
#endif

#include			"..\Config\VariableType.h"


USER_EXT void 	Reset_Watch_Dog(void);


#ifndef	__noinit__
#define 	__noinit__ 		__attribute__((zero_init))//��������ʼ��Ϊ0
#endif

#pragma		pack(1)
USER_EXT    __noinit__  unsigned int SystemResetFlag;

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   �������ݷ��͵�����0
*/
#define		TEST_MODE							0xaa55
#define		TEST_MODE_AUTO						0x7AAE

#define		TEST_DATA_OFF						0x0000						// 
#define		tRTC_CHANGE_CLOCK					((0x55<<8)+'R')
#define		tTASK_RUN_FLAG						((0x55<<8)+0xfe)

#define		tMAIN_SYS_SEC_CNT					((0x55<<8)+'+')
#define		tMAIN_WRITE_DFE					((0x55<<8)+'W')
#define		tMAIN_IIC							((0x55<<8)+'I')
//#define		tMAIN_TEST_HC595					((0x55<<8)+'H')
#define		tMAIN_UART							((0x55<<8)+'U')
#define		tMAIN_PWM							((0x55<<8)+'P')
#define		tMAIN_ACCEL							((0X55<<8)+'A')
#define		tMAIN_TMP_TEST						((0X55<<8)+'T')
#define		tMAIN_IC_CARD_DATA				((0X55<<8)+'C')

#define		tMAIN_RECORD_START					((0x55<<8)+'S')			// ��ʼ¼��
#define		tMAIN_RECORD_STOP					((0x55<<8)+'p')			// ֹͣ¼��
#define		tMAIN_RECORD_INIT					((0x55<<8)+'i')			// ��ʼ�� ¼������
#define		tMAIN_RECORD_UPLOAD					((0x55<<8)+'u')			// ����¼��
#define		tMAIN_RECORD_PLAY					((0x55<<8)+'y')			// ����¼��

#define		tGPS_ORIGINAL_DATA				((0x55<<8)+'0')			// ԭʼ����
#define		tGPS_ROUND_FENCE					((0x55<<8)+'1')			// ����Χ�����
#define		tGPS_RECT_FENCE					((0x55<<8)+'2')			// ����Χ�����
#define		tGPS_POLYGON_FENCE				((0x55<<8)+'3')			// ����Χ�����
#define		tGPS_ROAD_LINE_FENCE				((0x55<<8)+'4')			// ����Χ�����
#define		tGPS_CHAR_PACK						((0x55<<8)+'5')			// GPS�ַ������ݰ�
#define		tGPS_HEX_PACK						((0x55<<8)+'6')			// GPSʮ���������ݰ�
#define		tGPS_2D_DISTANCE					((0x55<<8)+'7')			// GPS�������
#define		tGPS_MMM								((0x55<<8)+'8')			// MMM��ʽ�����16����
#define		tGPS_ELE_FENCE_HEX_DATA			((0x55<<8)+'9')			// ����Χ��16��������(MMM)
#define		tGPS_ALL_DATA						((0x55<<8)+'A')			// GPSģ���������������
#define		tGPS_GET_DATA_INTERVAL			((0x55<<8)+'B')			// 
#define		tGPS_REC_OK_EVT_FLAG				((0x55<<8)+'C')			// 
#define		tGPS_PACK_FLAG						((0x55<<8)+'D')			// �����־
#define		tGPS_SET_ELE_RAIL					((0x55<<8)+'E')			// ���õ���Χ��
#define		tGPS_FORCE_DATA_PACK				((0x55<<8)+'F')			// 
#define		tGPS_ORIGINAL_DATA_HEX			((0x55<<8)+'G')			// ��PutBuffer
#define		tGPS_TASK_RUN						((0x55<<8)+'H')			// 
#define		tGPS_ANT_STATUS					((0x55<<8)+'I')			// 
#define		tGPS_CM_PACK						((0x55<<8)+'J')			// 
#define		tGPS_CONST_DATA					((0x55<<8)+'K')			//
#define		tGPS_MESSAGE_SAVE 				((0x55<<8)+'L')			//	GPS��Ϣ�洢����
#define		tGPS_BLIND_MESSAGE_SAVE 		((0x55<<8)+'M')			//	GPSä������
#define		tGPS_ANT_OPEN 						((0x55<<8)+'N')			//	GPS���߿�·
#define		tGPS_TEMP_PACK						((0x55<<8)+'O')			// ��ʱ���
#define		tGPS_TEST							((0x55<<8)+'P')			//
#define		tGPS_HISTORY_DATA					((0x55<<8)+'Q')			//
#define		tGPS_TIME_TO_PACK					((0x55<<8)+'R')			//
#define		tGPS_EXT_PACK						((0x55<<8)+'S')			//	��չ��
#define		tGPS_USE_GPS						((0x55<<8)+'T')			//	
#define		tGPS_USE_BD							((0x55<<8)+'U')			//	
#define		tGPS_USE_GPS_BD					((0x55<<8)+'V')			//	
#define		tGPS_COOL_START					((0x55<<8)+'W')			//	
#define		tGPS_VIRTUAL_GPS_VALID			((0x55<<8)+'X')			//	
#define		tGPS_UPDATE							((0x55<<8)+'u')			// ��������

///////////////////////////////////////
#define		tGPRS_AT_ACK_CHECK				((0x55<<8)+'0')			// 
#define		tGPRS_DIAL_STATUS					((0x55<<8)+'1')			// 
#define		tGPRS_SEND_DATA_INFO				((0x55<<8)+'2')			// ����������Ϣ
#define		tGPRS_GET_CMD_DATA				((0x55<<8)+'3')			// 
#define		tGPRS_MOBILE_INIT					((0x55<<8)+'4')			// 
#define		tGPRS_SAVE_DATA_CNT				((0x55<<8)+'5')			// 
#define		tGPRS_GSM_SMS						((0x55<<8)+'6')			// SMS
#define		tTEST_SEND_SMS1					((0x55<<8)+'8')			// ���Զ���1
#define		tTEST_SEND_SMS2					((0x55<<8)+'9')			// ���Զ���2
#define		tGPRS_GSM_ALL_DATA				((0x55<<8)+'A')			// ģ�鷵�ص�ȫ������
#define		tGSM_PHONE_STATUS					((0x55<<8)+'B')		  // ��ʱ����
#define		tGPRS_DOWNLOAD_CMD_TEST			((0x55<<8)+'C')
#define		tGSM_DIAL_NUMBER					((0x55<<8)+'D')			// ����
#define		tAT_CMD_ERROR						((0x55<<8)+'E')
#define		tGPRS_FIFO_DATA					((0x55<<8)+'F')			// 
#define		tGPRS_GETFILE    				   ((0x55<<8)+'G')			// �ļ���ȡ
#define		tGPRS_U0_GPRS						((0x55<<8)+'H')
#define		tGSM_UPGRADE_DATA1				((0x55<<8)+'I')			//	Զ���������ݲ���
#define		tTEST_SMS_CMD						((0x55<<8)+'M')			// ����������� 
#define		tGPRS_NO_LINK						((0x55<<8)+'N')
#define		tGPRS_SEND_OK						((0x55<<8)+'O')			// 
#define		tGPRS_PACK_DATA					((0x55<<8)+'P')			// �������
#define		tGSM_REC_SMS						((0x55<<8)+'S')			// ���ն��Ŵ���
#define		tGSM_TASK_RUN						((0x55<<8)+'T')			// ��������״̬
#define		tGSM_UPGRADE_DATA					((0x55<<8)+'U')			//	Զ���������ݲ���
#define		tGPRS_CONST_SIGNAL				((0x55<<8)+'Z')			// 
#define		tGPRS_UPDATE						((0x55<<8)+'u')

#define		tGPRS_ALL_PARAMETER				((0x55<<8)+'a')			// ȫ������


#define		tGPRS_TMP_TEST						((0x55<<8)+'t')		  // ��ʱ����



///////////////////////////////////////
#define		tGPRS_AUTO_SEND					((0x55<<8)+'0')			// 
///////////////////////////////////////
#define		tSW_32BIT_TO_UART0				((0x55<<8)+'0')			// ��ȡ����������
#define		tSW_OUT_PUT_HIGH					((0x55<<8)+'1')			// �������Ϊ��
#define		tSW_OUT_PUT_LOW					((0x55<<8)+'2')			// �������Ϊ��
#define		tSW_MOTO_PULS_CNT					((0x55<<8)+'3')			// ������ת�ټ�����
#define		tSW_TEST_DELAY						((0x55<<8)+'T')			// ���Լ̵���
#define		tSW_PULS_ENABLE					((0x55<<8)+'P')			// �������
#define		tCONTROL_OUTPUT					((0x55<<8)+'C')			// ����������
#define		tSW_RADIO_OUT						((0x55<<8)+'R')			// ����ң��
#define		tSW_CHECK_ALARM					((0x55<<8)+'A')			// ����������
#define		tSW_LOCK_DOOR_SIGNAL				((0x55<<8)+'L')			// �����ź�
//#define		tHC595_OUT1							((0x55<<8)+'5')			// 
//#define		tHC595_OUT2							((0x55<<8)+'6')			// 
#define		tSPEED_DRIVE_PARA					((0x55<<8)+'S')			// 
#define		tSPEED_OVER_SPEED_PARA			((0x55<<8)+'O')			// 
#define		tSPEED_SIMU_SPEED					((0x55<<8)+'M')			// ģ���ٶȲ���
#define		tSPEED_RECORD						((0x55<<8)+'R')			// ��¼�ǲɼ�

///////////////////////////////////////
#define		tADC_CH0								((0x55<<8)+'0')			// ADC0��������ʾ
#define		tADC_CH1								((0x55<<8)+'1')			// ADC0��������ʾ
#define		tADC_CH2								((0x55<<8)+'2')			// ADC0��������ʾ
#define		tADC_CH3								((0x55<<8)+'3')			// ADC0��������ʾ
#define		tADC_ALL								((0x55<<8)+'A')			// ����ͨ��
#define		tADC_HEX								((0x55<<8)+'H')			// Mega16ȫ��ģ����
#define		tADC_MAIN_POWER					((0x55<<8)+'M')			// Mega16ȫ��ģ����
#define		tADC_TEST							((0x55<<8)+'T')			// ADC����
#define		tADC_KEY_BOARD						((0x55<<8)+'K')			// AD����
///////////////////////////////////////
#define		tDISP_NOT_FOR_SALE				((0x55<<8)+'0')			
#define		tDISP_ALL_ON						((0x55<<8)+'A')
//#define		tDISP_IP								((0x55<<8)+'I')
#define		tDISP_PRINT_INIT					((0x55<<8)+'I')
#define		tDISP_PRINT_TEST					((0x55<<8)+'P')
#define		tDISP_PRINT_MOTOR_FORWARD		((0x55<<8)+'F')
#define		tDISP_PRINT_MOTOR_BACK			((0x55<<8)+'B')
#define		tDISP_PRINT_POWER_OFF			((0x55<<8)+'C')
#define		tDISP_PRINT_LATCH					((0x55<<8)+'L')

#define		tDISP_OLD_RECODE				   ((0x55<<8)+'N')		  //���ԭʼ��¼����
#define		tDISP_NEW_RECODE		    	   ((0x55<<8)+'M')		  //������²����ļ�¼����
#define		tDISP_USB_RECODE		    	   ((0x55<<8)+'U')		  //���USB�ɼ�����
#define		tDISP_TEST_FOR_EXT				((0x55<<8)+'E')			// �ⲿ��ʾ

///////////////////////////////////////
#define		tMAIN_TEST							((0x55<<8)+'0')			

///////////////////////////////////////
#define		tCAMERA_TAKE_PHOTO				((0x55<<8)+'1')
#define		tCAMERA_READ_DATA					((0x55<<8)+'2')
#define		tCAMERA_RUN_TEST					((0x55<<8)+'3')			// ��������ͷ�����Ƿ�������
#define		tCAMERA_INIT						((0x55<<8)+'I')			// ��������ͷ�����Ƿ�������
#define		tCAMERA_ALL_DATA					((0x55<<8)+'A')			// ��������ͷ�����Ƿ�������
#define		tCAMERA_PACK_LEN					((0x55<<8)+'L')		
#define		tCAMERA_V_DATA						((0x55<<8)+'V')  			// ��������
#define		tCAMERA_CMD_ERROR					((0x55<<8)+'E')  			// ��������
#define		tCAMERA_CHCEK						((0x55<<8)+'C')			// �������ͷ

#define		tHANDLE_DISPLAY_TEST				((0x55<<8)+'H')			// �ֱ�����ʾ������
#define		tTIRED_TEST							((0x55<<8)+'T')			// ƣ�ͼ�ʻ����

#define		tCAN_ALL_DATA						((0x55<<8)+'A')
#define		tCAN_SEND_DATA						((0x55<<8)+'S')
#define		tCAN_REC_DATA						((0x55<<8)+'R')
#define		tCAN_TEST_DATA						((0x55<<8)+'T')

#define     USE_IN_SIM                    6
#define     USE_OUT_SIM                    0


#pragma		pack(1)
USER_EXT		struct
{
	uint16		MainTask;
	uint16		GPS;
	uint16		GPRS;
	uint16		CAN;	
	uint16		SW;
	uint16		ADC;	
	uint16		Display;
	uint16		Camera;
	uint16		Speed;
	uint16		Value;
	uint16		TestMode;
} volatile	 TD;		// TestData;


#pragma		pack(1)
USER_EXT		struct
{
	uint32		MainTask;
	uint32		GPS;
	uint32		GSM;
	uint32		GG_Rec;
	uint32		GG_Send;
	uint32		Camera;	
	uint32		GPRS_LongTimeNoSend;
	uint32		CANSend;
	uint32		CANRecv;
	uint32		KeyDisp;
	uint32		Speed;
	uint32		USB;
	uint32		Print;
	uint32		GPRS_LongTimeNoAck;
} volatile	TC,TickCnt;		// Task stop counter

#define			IDLE_MODE_FLAG			0x5a


//////////////////////// modified 2013.3 //////////////////////////////////////////////
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   JT808,����״̬�������ݽṹ��
   �ɼ�ʱ1��Ч��0��Ч���������ʱ�踺�߼��������޸ġ�
*/

#pragma		pack(1)
typedef		struct
{	
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		ACC:1;						// 
			uchar		GPS:1;						// 
			uchar		Lat:1;		  				// 
			uchar		Lon:1;						// 
			uchar		Operations:1;		   	// 
			uchar		LonLatEncrypt:1;	  		// 
			uchar		Rsv6:1;		// 
			uchar		Rsv7:1;      // 
		} Bits;
	} VS1;	
	////////////////////////////////
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		CarLoadStatus:2;			 // 00 �ճ��� 01 ���� �� 10 ���� �� 11 ����
			uchar		OffOil:1;		  	// 
			uchar		OffE:1;				// 
			uchar		LockDoor:1;			// 
			uchar 	Door1:1;				// 1���ſ���0�� �Źأ� �����������ͬ��ǰ��
			uchar		Door2:1;				// ����
			uchar		Door3:1;      			// ����
		} Bits;
	} VS2;	 
	////////////////////////////////
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		Door4:1;	// ��ʻ��	
			uchar		Door5:1;	// �Զ���
			uchar		UseGPS:1;	// 0��δʹ��GPS��λ	 1: ʹ��GPS��λ
			uchar		UseBD:1;	// 0��δʹ�ñ�����λ	 1: ʹ�ñ�����λ
			uchar		UseGLONASS:1;	// 0��δʹ��GLONASS��λ	 1: ʹ��GLONASS��λ
			uchar		UseGalileo:1;	// 0��δʹ��Galileo��λ	 1: ʹ��Galileo��λ
			uchar		RTurnLight:1;	//��ת���
			uchar		HighLight:1;	//Զ��� 
		} Bits;
	} VS3;	
	////////////////////////////////
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		Rsv0:1;		//
			uchar		Rsv1:1;		//
			uchar		Rsv2:1;		//
			uchar		Rsv3:1;		//
			uchar		Rsv4:1;		//
			uchar		Rsv5:1;		//
			uchar		Rsv6:1;		//
			uchar		Rsv7:1;		//
		} Bits;
	} VS4;		
	/////////////////////////////////////////////////////// ����Ϊ������־λ
	union
	{			
		uchar		Byte;					
		struct	
		{
			uchar		EmergencyAlarm:1;		//
			uchar		OverSpeedAlarm:1;		//
			uchar		FatigueDriving:1;		//
			uchar		Warning:1;		//
			uchar		ANT_Error:1;		//	GNSSģ�鷢������
			uchar		AntOpen:1;		//
			uchar		AntShort:1;		//
			uchar		MainPowrLow:1;		//
		} Bits;
	} VS5;		

	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		MainPowerOff:1;		//
			uchar		DispError:1;		//
			uchar		TTS_Error:1;		//
			uchar		CameraError:1;		//
			uchar		IC_Card_ModelError:1;		// ��·����֤IC��
			uchar		OverSpeedPreAlarm:1;			// ����Ԥ��
			uchar		FatiguePreAlarm:1;			//  ƣ�ͼ�ʻԤ��
			uchar		Rsv7:1;		//
		} Bits;
	} VS6;		


	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		Rsv0:1;		//
			uchar		Rsv1:1;		//
			uchar		DrivingOverTime:1;		//
			uchar		StopOverTime:1;		//
			uchar		InOutArea:1;			//
			uchar		InOutLine:1;			//
			uchar		TravelTime:1;			//
			uchar		LineDeviate:1;			//
		} Bits;
	} VS7;	
	
	
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		VSS_Error:1;			//
			uchar		AbnormalOil:1;			//
			uchar		StealAlarm:1;			//
			uchar		IllegalPowerOn:1;		//
			uchar		IllegalMove:1;			//
			uchar		CrashPreAlarm:1;		//	��ײԤ��
			uchar		TiltPreAlarm:1;			//	�෭Ԥ��
			uchar		IllegalOpenDoor:1;		//	�Ƿ����ű������ն�δ��������ʱ�����жϷǷ����ţ�
		} Bits;
	} VS8;			


} JT808_STRU_VSDR,*pJT808_STRU_VSDR;					// Vehicle Status Data Recorder				
			

USER_EXT		volatile		JT808_STRU_VSDR		JT808Flag;


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   JT808  ��չ�źų���
*/
#pragma		pack(1)
typedef		struct
{	
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		DippedHeadLight:1;				// 	  ������ź�
			uchar		HighLight:1;						// 	 Զ����ź�
			uchar		RTurnLight:1;		  				// 	 ��ת���
			uchar		LTurnLight:1;						// 	 ��ת���
			uchar		Brake:1;		   					// 	 �ƶ��ź�
			uchar		ReversGear:1;	  		         // 	 �����ź�
			uchar		FogLight:1;		 				   //   ����ź�
			uchar		ClearanceLamp:1;              //  ʾ����
		} Bits;
	} VS1;	
	////////////////////////////////
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		Speaker:1;			// �����ź�	
			uchar		AirCondition:1;	// �յ�״̬
			uchar		Neutral:1;			// �յ�״̬
			uchar		Retarder:1;			// ����������
			uchar		ABSWork:1;			//  ABS����
			uchar		Heater:1;			// ����������
			uchar		Clutch:1;			// �����״̬
			uchar		Rev:1;	    		// ���� 
		} Bits;
	} VS2;	
	////////////////////////////////
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar Rev:8;
		} Bits;
	} VS3;	 
	////////////////////////////////
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar Rev:8;
		} Bits;
	} VS4;
	union
	{
		uchar		Byte;	
	}VS5;
	union
	{
		uchar		Byte;	
	}VS6;
	union
	{
		uchar		Byte;
		struct
		{
			uchar		AccelAdd:1;
			uchar		AccelDec:1;
			uchar		Rsv6:6;
		}Bits;
	}VS7;
	union
	{
		uchar		Byte;
	}VS8;	
} JT808_EXTEN_STRU_VSDR,*pJT808_EXTEN_STRU_VSDR;

USER_EXT		volatile		JT808_EXTEN_STRU_VSDR		JT808ExtenVsFlag;	


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   JT808  IO״̬
*/
#pragma		pack(1)
typedef		struct
{	
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		DeepSleepStatus:1;					// 	  �������״̬
			uchar		SleepStatus:1;						// 	  ����״̬
			uchar		Rev:6;                            // ����
			
		} Bits;
	} VS1;	
	////////////////////////////////
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar	Rev:8;                   //  ����
		} Bits;
	} VS2;	
	////////////////////////////////
} JT808_IO_STRU_VSDR,*pJT808_IO_STRU_VSDR;

USER_EXT		volatile		JT808_IO_STRU_VSDR		JT808IOStatusFlag;



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   GB19056-2011 ״̬λ����
*/
#pragma		pack(1)
typedef		struct
{	
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		CallHelpAlarm:1;		   //	��������
			uchar		CollisionAlarm:1;			// ��ײ����
			uchar		OverSpeedAlarm:1;		  	// ���ٱ���
			uchar		FTdrivingAlarm:1;		  	// ƣ�ͼ�ʻ����
			uchar		OutAreaAlarm:1;			   // ��ֹʻ��Խ�籨��
			uchar		InAreaAlarm:1;				// ��ֹʻ��Խ�籨��
			uchar		DeviateRouteAlarm:1;		// ƫ��·�߱���
			uchar		Rsv:1;      				// 
		} Bits;
	} VS1;	
	////////////////////////////////
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		MainPowerLost:1;			// ��ƿ�����
			uchar		BakPowerLow:1;				// ���õ�ص�ѹ��					
			uchar		Rsv2:1;						// 
			uchar		Rsv3:1;						// 
			uchar		Rsv4:1;						// 
			uchar		Rsv5:1;						// 
			uchar		Rsv6:1;						// 
			uchar		Rsv7:1;						// 
		} Bits;
	} VS2;	 
	////////////////////////////////
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		Rsv0:1;						// 
			uchar		Rsv1:1;						// 
			uchar		Rsv2:1;						// 
			uchar		Rsv3:1;						// 
			uchar		Rsv4:1;						// 
			uchar		Rsv5:1;						// 
			uchar		Rsv6:1;						// 
			uchar		SpeedSensor:1;				// 
		} Bits;
	} VS3;	
	////////////////////////////////
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		ACC:1;		  				// ACC
			uchar		DoorOpen:1;					// ���ſ�
			uchar		Rsv2:1;						// 
			uchar		Rsv3:1;						// 
			uchar		Rsv4:1;						// 
			uchar		Rsv5:1;						// 
			uchar		Rsv6:1;						// 
			uchar		Brake:1;						// ɲ��
		} Bits;
	} VS4;		

	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		Rsv0:1;		  				// 
			uchar		Rsv1:1;						// 
			uchar		Rsv2:1;						// 
			uchar		Rsv3:1;						// 
			uchar		Rsv4:1;						// 
			uchar		Rsv5:1;						// 
			uchar		Rsv6:1;						// 
			uchar		Rsv7:1;						// 
		} Bits;
	} VS5;		

	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		Rsv0:1;		  				// 
			uchar		Rsv1:1;						// 
			uchar		Rsv2:1;						// 
			uchar		Rsv3:1;						// 
			uchar		Rsv4:1;						// 
			uchar		Rsv5:1;						// 
			uchar		Rsv6:1;						// 
			uchar		Rsv7:1;						// 			
		} Bits;
	} VS6;		

	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		Rsv0:1;		  				// 
			uchar		Rsv1:1;						// 
			uchar		Rsv2:1;						// 
			uchar		Rsv3:1;						// 
			uchar		Rsv4:1;						// 
			uchar		Rsv5:1;						// 
			uchar		Rsv6:1;						// 
			uchar		Rsv7:1;						// 
		} Bits;
	} VS7;	
	
	union
	{			
		uchar		Byte;					
		struct		
		{
		  	uchar		Rsv0:1;		  				// 
			uchar		Rsv1:1;						// 
			uchar		Rsv2:1;						// 
			uchar		Rsv3:1;						// 
			uchar		Rsv4:1;						// 
			uchar		Rsv5:1;						// 
			uchar		Rsv6:1;						// 
			uchar		Rsv7:1;						//  			
		} Bits;
	} VS8;			
} GB19056_STRU_VSDR,*pGB19056_STRU_VSDR;					// Vehicle Status Data Recorder				

USER_EXT		volatile		GB19056_STRU_VSDR		GB19056_VehicleStatus;



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   ʹ�ܱ�־ 
*/
typedef		struct
{	
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		ParkOverTimeAlarm:1;					// ����ʹ��ͣ����ʱ�������
			uchar		SpeedLimitAlarm:1;					// ����ʹ�ó������ٱ������
			uchar		StealAlarm:1;							// ����ʹ�õ������
			uchar		RobAlarm:1;								// ����ʹ�ýپ����         
			uchar		NoSetScanAlarm:1;						// ����ʹ��δ����������   
			uchar		TrailCarAlarm:1;						// ����ʹ���ϳ��������     
			uchar		HelpAlarm:1;							// ����ʹ�������������     
			uchar		TireDriveAlarm:1;						// ����ʹ��ƣ�ͼ�ʻ���     
		} Bits;
	} EN1;
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		IllegalOpenDoor:1;					// ����ʹ�÷Ƿ����ż��        
			uchar		IllegalMakeFire:1;					// ����ʹ�÷Ƿ������        
			uchar		CheckShake:1;							// ����ʹ���𶯼��            
			uchar		AutoLockDoor:1;						// ����ʹ������                
			uchar		LowVoltage:1;							// ����ʹ�õ�ѹ���            
			uchar		RemoteMonitor:1;						// ����Զ�̼�����־  
			uchar		UseHandleFlag:1;						// ����ʹ���ֱ���־  
			uchar		HandleWatchCar:1;						// �Ƿ������ֱ�����
		} Bits;
	} EN2;	
	union
	{			
		uchar		Byte;					
		struct		
		{
		  	uchar		Rsv0:1;		  				// 
			uchar		Rsv1:1;						// 
			uchar		Rsv2:1;						// 
			uchar		Rsv3:1;						// 
			uchar		Rsv4:1;						// 
			uchar		Rsv5:1;						// 
			uchar		Rsv6:1;						// 
			uchar		Rsv7:1;						//  			
		} Bits;
	} EN3;	
	union
	{			
		uchar		Byte;					
		struct		
		{
		  	uchar		Rsv0:1;		  				// 
			uchar		Rsv1:1;						// 
			uchar		Rsv2:1;						// 
			uchar		Rsv3:1;						// 
			uchar		Rsv4:1;						// 
			uchar		Rsv5:1;						// 
			uchar		Rsv6:1;						// 
			uchar		Rsv7:1;						//  			
		} Bits;
	} EN4;			 	
	////////////////////////////////
} STRUCT_ENABLE_FLAG,*pENABLE_FLAG;
USER_EXT		STRUCT_ENABLE_FLAG		EnableFlag;



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   ����Χ���������ݣ�����Χ�����ݴ洢��Flash����Բ�Ρ����Ρ�����Ρ�·�ߣ���4�࣬ÿ��������Ϊ128
   ÿ�����Դ洢������ĳ���Ϊ32B
   ǰ16B���ܸ�������ǰ����Χ���š�
   ��16B��128Bits��ÿ��Bits����Χ������Ч�ԣ�0����Ч���������Χ����1����Ч������
   ����洢λ�ã�EE_ELE_FENCE_ATTR_BASE_ADDR
*/
#define		MAX_FENCE_NUM					128
#define		LEN_EE_FENCE_ATTR				64
#pragma		pack(1)
typedef		struct
{	   
	uint32	RemoteID;		// Զ������Χ����ID�ţ�������ʹ�ã�
	uint32	InTimeCnt;		// ��Χ��ʱ��
	uint32	OutTimeCnt;		// ��Χ��ʱ��
	uchar		Total;
	uchar		CurrentNo;		// ��ǰΧ��/·�ߣ�ȡֵ1~128
	uchar		PreNo;			// �ϴ�Χ��/·�ߣ�ȡֵ1~128
	uchar		Width;				// 
		
	uchar		Enable[16];		// 128Bits��ʹ��128��Χ��
	
	int32		Lon1;				// ��ǰΧ��/·�߾�γ�ȣ�����ʱ�������Խǵ����꣬·��ʱ��·�ε������յ�����
	int32		Lat1;
	int32		Lon2;
	int32		Lat2;	
	
	uint32	SecNo;			// ·�κ�
	uint32	PreSecNo;		// �ϸ�·�κ�
	uint16   MaxDriveTime;	//·����ʻ������ֵ	��λs
	uint16   MinDriveTime;	//·����ʻ���㷧ֵ	��λs	
	uchar  	LimitSpeed;		//����ٶ� (ע������ʱΪ˫�ֽڣ�
	uchar    ContinueTime;	//�����������ʱ��		��λs	
	uchar		RsvB[2];			// ����
} FENCE_ATTR_STRUCT,*pFENCE_ATTR_STRUCT;
USER_EXT		FENCE_ATTR_STRUCT		RoundFenceAttr,RectFenceAttr,PolygonFenceAttr,RoadAttr;


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   595 �������  
*/
/* #pragma		pack(1)
typedef		struct
{	
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		PowerSeldLock:1;			// ����Դ��������
			uchar		PowerLCM:1;					// 
			uchar		PowerPrint:1;		  		// 
			uchar		PowerGPS:1;					// 
			uchar		PowerGSM:1;		   		// 
			uchar		PowerCamera:1;	  			// 
			uchar		PowerExt5V:1;				// 
			uchar		SwitchSIM:1;      		// 
		} Bits;
	} Out1;
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		DST1:1;						// ������ӡ�����ȿ���1
			uchar		DST2:1;						// 
			uchar		DST3:1;		  				// 
			uchar		DST4:1;						// 
			uchar		DST5:1;		   			// 
			uchar		DST6:1;	  					// 
			uchar		PowerMotor:1;				// ������ӡ����������Դ����
			uchar		PowerLCM_BL:1;      		// LCM�������
		} Bits;
	} Out2;	 	
	////////////////////////////////
} HC595_OUT,*pHC595_OUT;
USER_EXT		volatile			HC595_OUT		HC595;	*/

//#define			INVALID_CARD_LOCK_VECHCLE		0x7e5a
#define			LOCK_VEHICLE_ENABLE				0x55aa
#define			LOCK_VEHICLE_ENABLE_INV			0xaa55
#define			LOCK_VEHICLE_DISABLE				0x0000
#define			LEN_LOCK_VEHICLE_FLAG			2
////////// ������ƽṹ /////////////////////////
#define		MAX_PLUS_PERIOD				(9999)
#pragma		pack(2)
typedef		struct	_OUTPUT_CONTROL_
{
	uint16	Lock,ForceLock;
	uint16	ControlOut1,ControlOut2,ControlOut3,ControlOut4,ControlOut5;
	uint16	Rsv;
} OUTPUT_CONTROL,*pOUTPUT_CONTROL;
USER_EXT    volatile		OUTPUT_CONTROL			VehicleControl;


#pragma		pack(1)
typedef		struct		_IDLE_STR_
{
	uchar		EnableInterIdleMode;						// �������ʡ��ģʽ
	uchar		IdleModeFlag;								// ʡ��ģʽ��־
	uint16	InterIdleModeTime;						// ����ʱ������ʡ��ģʽ
	uint16	WakeUpTime;								   // ����ʱ�����
	uint16	WakeUpLongTime;							// ���ѳ���ʱ��
	uint32	SecondCnt;
}	IDLE_STR,*pIDLE_STR;
USER_EXT   volatile		IDLE_STR		Idle;
							
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	ϵͳ��־��GSM/GPRS��ؼ��������塣����ʧ�ԣ�ÿ���Ӵ洢һ�Σ���ͨ��Զ�̶�ȡ
*/	
#define		SYSTME_LOG_GROUP		8
#define		NUMBER_PER_GROUP		8
#define		SYSTEM_LOG_NUMBER		(SYSTME_LOG_GROUP*NUMBER_PER_GROUP)		// ÿ��8��
#define		LEN_SYSTEM_LOG			(SYSTEM_LOG_NUMBER*4)	// ÿ������4�ֽ�
#define		SYSTEM_LOG_EE_PAGE	(LEN_SYSTEM_LOG/EE_PAGE_LEN)
#pragma		pack(4)
typedef		struct
{	
	struct
	{
		uint32	DialCnt;					// GPRS���Ŵ���������
		uint32	DialOK;					// GPRS���ųɹ�����						
		uint32	OnLineTime;				// GPRS����ʱ�䣬��λ����
		uint32	RecTotalPack;			// GPRS�����ܰ���
		uint32	RecTotalBytes;			// GPRS�������ֽ���
		uint32	PushTotalBytes;		// GPRS�������ֽ���������Ӳ����ڷ����򱾵ض�����
		uint32	SendTotalBytes;		// GPRS�����ֽڼ�����
		uint32	SendTotalPack;			// GPRS���Ͱ���������		
	}GPRS;
	struct
	{
		uint32	CoolBoot;				// GSMģ��������������
		uint32	SimErrorTimes;			// SIM����ʱ��������������������ɵ�����ԭ�����Ҳ�������
		uint32	UnRegTime;				// GSM�Ҳ���������ʱ������������й��ƶ�����ͨ�����ţ�
		uint32	EbbSignalTime;			// GSM���ź�ʱ��
		uint32	RecSms;					// GSM���ն��ż�����
		uint32	SendSms;					// GSM���Ͷ��ż�����
		uint32	RegServer;				// ע�����������������			  G12 δ��
		uint32	RegServerOK;			// ע��������ɹ�����������	  G12 δ��
	}GSM;	
	struct  
	{
		uint32	CallerTimes;			// ���д���				//G12 δ��
		uint32	CallerTime;				// ����ͨ��ʱ��	   //G12 δ��
		uint32	CalledTimes;			// ���д���				//G12 δ��
		uint32	CalledTime;				// ����ͨ��ʱ��		//G12 δ��
		uint32	GpsDataSavePosIndex; // �洢��·�Ͽ�ʱ��λ����Ϣ����	  (���Զ��壬��洢���)  //G12 δ��
		uint32	GpsDataSendPosIndex;	//	������·�Ͽ�ʱ��λ����Ϣ����	  (���Զ��壬��洢���)  //G12 δ��
		uint32	GpsDataSaveAlarmIndex;		// �洢������λ����Ϣ����	  (���Զ��壬��洢���)  //G12 δ��
		uint32	GpsDataSendAlarmIndex;	   //	���ͺ�����λ����Ϣ����    (���Զ��壬��洢���)  //G12 δ��
	}GG;
	struct
	{	
		unsigned long long  	SpeedCaculus; // �ٶȻ���ֵ
		uint32	TotalMileage;			// �ۼ�����̣���
		uint32	PackTotalNumber;		// �������
		uint32	PackValidNumber;		// ����Ķ�λ����
		uint32	RestartCnt;				// GPSģ����������
		uint32	ValidTime;				// ��Ч��λ��ʱ��,��
		uint32	DataErrorCnt;			// RMC������������
	}GPS;	
	struct
	{
		uint32	HighSpeedPulsCnt;		// ����1����������������ۼ�ֵ��      	 //G12 δ��
		uint32	SpeedSensorPulsCnt;	// ����2�����ٴ���������������ۼ�ֵ��	 //G12 δ��
		uint32	MotorSpeedPulsCnt;	// ����3��������ת������������ۼ�ֵ��	 //G12 δ��
		uint32	MotoRunTimeCnt;		// �������ۼƹ���ʱ��						 //G12 δ��
		uint32	SystemWorkTime;		// ϵͳ�ۼƹ���ʱ��  
		uint32	SystemLastRunTime;	// ϵͳ�����ʱ��							 //G12 δ��
		uint32	Power1_2;				// ����Դ���ⲿ��ص�ѹ(��vbat1)����λ: mV		  
		uint32	Power3_4;				// ��2�ֽ�Ϊ���ϵ��(��vbat3)�����˱�����أ���ѹֵ����λ: mV	  
	}VDR;
	struct
	{
		uint32	SystemRestartCnt;		// ϵͳ����������
		uint32	Update;					// ����������������
		uint32	GPRS;						// GPRS������������
		uint32	TaskOverTime;			// ����ʱ��������
		uint32	PowerOn;					// �ϵ���������
		uint32	WatchDog;				//	���Ź���������
		uint32	WakeUp;				   // ���ߺ�����������	 
		uint32	ResetPin;				//	 ����
	} ResetCnt;
	struct
	{
		int32		Longitude;				// 4B�����ȣ��Ŵ���10e7������Ϊ��������Ϊ����
		int32		Latitude;				// 4B��γ�ȣ��Ŵ���10e7������Ϊ��γ����Ϊ��?
		uint32	RtcSecond;				// 4B��ʱ�䣬ͳһΪ�룬2000��1��1��00:00:00Ϊ��0��
		uint32	Speed_Info;				// 1�ֽ�GPS�ٶȣ�1�ֽڸ߶ȣ�1�ֽ�GPSˮƽ�������ӣ�1�ֽ����Ǹ���
		uint32	GsmMmcMnc;				// ��12BitΪMMC���м�12BitΪMNC����8Bit����
		uint32	GsmLacCi;				// 2�ֽڻ�վ��ϢLAC��2�ֽڻ�վ��ϢCID
		uint32	Gsm_Info;				// 1�ֽڻ�վ��ϢBSIC, 1�ֽ�GSM��Ϣǿ�ȣ�1�ֽڻ����¶ȣ�1�ֽ�����Դ��ѹ
		uint32	VehicleStatus1;		// ����״̬��־λ��ǰ32λ		
	}GG1;
	struct
	{
		uint32	VehicleStatus2;		// ����״̬��־λ����32λ	
		uint32	Rsv;
		uint32	UpLoadIndex;			// �����ϴ���ˮ��
		uint32	AccChangeCnt;
		uint32	GsmLbsCnt;
		uint32	RegIsDenied;			// GSMע�ᱻ�ܾ�
		uint32	Index;
		uint32	CheckSum;
	} Other;
	struct
	{
		uint32		Online;
		uint32		UnReg;
		uint32		WakeUp;
	} Flag;
}SYSTEM_LOG,*pSYSTEM_LOG;	
USER_EXT		volatile	SYSTEM_LOG		SystemLog;


#pragma		pack(4)
typedef		struct
{
	uint32		GPS;
	uint32		Puls;
}TEST_MILEAGE,*pTEST_MILEAGE;
USER_EXT		volatile			TEST_MILEAGE		TestMileage;

	
	


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������洢��Flash��ҳ��ַ��������64������(256B)
*/
#define		FLASH_PAGE_CNT_NUM		42
#define		LEN_FLASH_PAGE_CNT		(FLASH_PAGE_CNT_NUM*4UL)
#pragma		pack(4)
typedef		struct
{
	uint32	Rsv01;
	uint32	Rsv02;
	uint32	Rsv03;
	uint32	Rsv04;
	uint32	Rsv05;
	uint32	Rsv06;
	uint32	Rsv07;
	uint32	RoundEleRail;           // Բ�ε���Χ��
	uint32	SquqreEleRail;          // ���ε���Χ��
	uint32	RoadLine;               // ·�߼ƻ�
	uint32	SytemLog;               // ϵͳ��־�洢
	uint32	Numbers;						// Ӧ�����Ȳ��ԣ���������ҳ��
	uint32	BlindSave;              // ä���洢
	uint32	BlindRead;              // ä����ȡ
	uint32	HistorySave;            // ��ʷ���ݴ洢
	uint32	HistoryRead;            // ��ʷ���ݶ�ȡ
	uint32	PicSave;                // ͼƬ�洢
	uint32	PicRead;                // ͼƬ��ȡ	
	uint32	CanBusDataSave;			// 
	uint32	CanBusDataRead;			// 
	uint32	AlarmMsgSave;				// ��������λ����Ϣ
	uint32	AlarmMsgRead;				// 
	uint32	NormalMsgSave;				// ��ͨλ����Ϣ
	uint32	NormalMsgRead;				// 
	uint32	GnssDataSave;				// 
	uint32	GnssDataRead;				//
	uint32	Speed48HSave;				// 48Сʱ�ٶȼ���������Ϣ
	uint32	Speed48HRead;
	uint32	Location360HSave;			// 360Сʱλ����Ϣ
	uint32	Location360HRead;
	uint32	ParkingDataSave;			// ͣ������
	uint32	ParkingDataRead;
	uint32	FatigueDrivingSave;		// ƣ�ͼ�ʻ
	uint32	FatigueDrivingRead;
	uint32	DriverLogInOutSave;     // ��ǩ����ǩ
	uint32	DriverLogInOutRead;	
	uint32	PowerOnOffSave;         // �ϵ�ϵ��¼
	uint32	PowerOnOffRead; 
	uint32	ParaModifySave;         // �����޸�
	uint32	ParaModifyRead;
	uint32	SpeedStatusSave;
	uint32	SpeedStatusRead;
}FF_PAGE_CNT,*pFF_PAGE_CNT;
USER_EXT		FF_PAGE_CNT		FlashPageCnt;


typedef		struct
{
	uint32	StartPage;	
	uint32	TotalPage;	
}GET_FLASH_DATA,*pGET_FLASH_DATA;
USER_EXT		GET_FLASH_DATA		GetFlashData;



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   ����״̬�������ݽṹ��
   �ɼ�ʱ1��Ч��0��Ч���������ʱ�踺�߼��������޸ġ�
*/
#define		LEN_VEHICLE_STATUS_PACK		10
#define		BIT_TEMPERATURE				2
#define		BIT_SIGNAL_QUALITY			5
#pragma		pack(1)
typedef		struct
{	
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		TemperatureAlarm:1;		// �¶ȱ���					 
			uchar		PasswordErrorAlarm:1;	// ������󱨾�			 
			uchar		GPRS_Delay:1;		  		// GPRS��������		G11ʹ��	 
			uchar		GasAndPowerOffStatus:1;	// �������ڶ��Ͷϵ�״̬		 G11ʹ��
			uchar		MainPowerLost:1;		   // ��ƿ�����		  G11ʹ��
			uchar		HighLevelSensor1:1;	   // �ߵ�ƽ������1
			uchar		HighLevelSensor2:1;		// �ߵ�ƽ������2
			uchar		LowLevelSensor1:1;      // �͵�ƽ������1  IdleMode:1;					// ʡ���־������ʡ��ǰ���͸ñ�־��
		} Bits;
	} VS1;	
	////////////////////////////////
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		GPS_ModuleError:1;		// GPSģ��������		  G11ʹ��
			uchar		Temperature1:1;			// �¶�1
			uchar		Temperature2:1;		  	// �¶�2
			uchar		BackupBatterySupply:1;	// �����ɺ󱸵�Դ����
			uchar		MainPowerLost:1;			// ��ƿ�����
			uchar		GPS_ANT_Open:1;	   	// GPS���߿�·    	   				// 
			uchar		GPS_ANT_Short:1;			// GPS���߶�·   
			uchar		LowLevelSensor2:1;      // �͵�ƽ������2     LowPowerAlarm:1;			// ��ѹ������־
		} Bits;
	} VS2;	 
	////////////////////////////////
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		DoorOpen:1;					// ���ſ�					G11ʹ��
			uchar		EnableAlarm:1;				// �������״̬
			uchar		ACC:1;		  				// ACC					   G11ʹ��
			uchar		Pressure1:1;	         // ѹ��1����		      	 
			uchar		Pressure2:1;		      // ѹ��2����
			uchar		Moto:1;	   				// ������					G11ʹ��
			uchar		OwnDefineAlarm:1;			// �Զ��屨��
			uchar		OverSpeed:1;				// ��������					G11ʹ��
		} Bits;
	} VS3;	
	////////////////////////////////
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		StealAlarm:1;				// ���Ա���				G11ʹ��
			uchar		RobAlarm:1;					// ��ٱ���				G11ʹ��
			uchar		OverSpeed:1;		  		// ���ٱ���				
			uchar		LawlessPowerOn:1;			// �Ƿ����				G11ʹ��
			uchar		InAreaAlarm:1;		   	// ��ֹʻ��Խ�籨��		G11ʹ��
			uchar		GPS_ANT_Open:1;	   	// GPS���߿�·����		G11ʹ��
			uchar		GPS_ANT_Short:1;			// GPS���߶�·����		G11ʹ��
			uchar		OutAreaAlarm:1;			// ��ֹʻ��Խ�籨��		G11ʹ��
		} Bits;
	} VS4;		

	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		GSMmodelStatus:1;			      	// �ֻ�ģ������			G11ʹ��
			uchar		SIMcardStatus:1;			      	// �ֻ�������				G11ʹ��
			uchar		MobilSignalStatus:1;		  	     	// �ҵ�GSM�ź�				G11ʹ��
			uchar		GPRSonlineStatus:1;		        	// GPRS����					G11ʹ��
			uchar		CameraOK:1;	            			// ����ͷ����			   G11ʹ��
			uchar    EMTstatus:1;						   //�����¶�����
			uchar    OilTankStatus:1;				      //�����¶�����
			uchar    SystemStart:1;					   	//ϵͳ����			 G11ʹ��
		} Bits;
	} VS5;		

	union
	{			
		uchar		Byte;					
		struct		
		{

		   uchar		UndefendAlarm:1;	    	// δ�������
			uchar		FTdrivingAlarm:1;		  	// ƣ�ͼ�ʻ����		 G11ʹ��
			uchar		VibrationAlarm:1;		  	// �𶯱���				 G11ʹ��
			uchar		LowPowerAlarm:1;	     	// ����Դ��ѹ�ͱ���	  G112ʹ��
			uchar		StopCarOverTime:1;	  	//ͣ����ʱ  			  G11ʹ��
			uchar		CarBeTrail:1;	         //�ϳ����� 				  G11ʹ��
			uchar		UnlawOpenDoor:1;	     	//�Ƿ�����
			uchar		CallHelpAlarm:1;		   //��������
			
		} Bits;
	} VS6;		


	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		TemperatureAlarm3:1;	   	// �¶�3����
			uchar		TemperatureAlarm4:1;	   	// �¶�4����
			uchar		Pressure3:1;		         // ѹ��3����
			uchar		Pressure4:1;			      // ѹ��4����
			uchar		HighLevelSensor3:1;	          	// �ߵ�ƽ������3
		   uchar		HighLevelSensor4:1;		         // �ߵ�ƽ������4
		   uchar		LowLevelSensor3:1;					//	�͵�ƽ������3
		   uchar		LowLevelSensor4:1;		         // �͵�ƽ������4
		} Bits;
	} VS7;	
	
	
	union
	{			
		uchar		Byte;					
		struct		
		{
		  uchar		EnablePowerSave:1;	       	//����ʡ��
		  uchar		IdleMode:1;				      	//ʡ���־������ʡ��ǰ���͸ñ�־��	 
		  uchar		BackDoorOpen:1;		       	//��β���  
		  uchar		DoorUnlockAlarm:1;	        	//
		  uchar		PhoneCalling:1;					//ͨ����־		  	   G11ʹ��	  
		  uchar		CtrlLineAlarm:1;		         //�����߶Ͽ�����
		  uchar		RemovalAlarm:1;			      //	�������				 
		  uchar		CanBusLockFlag:1;		         // CAN����������־
		} Bits;
	} VS8;			


} STRU_VSDR,*pSTRU_VSDR;					// Vehicle Status Data Recorder
				

USER_EXT		STRU_VSDR		VehicleStatus;


// ��ҵͳһͨѶЭ��֡��ʽ///////////////////////// Э��֡��ʽ
//#define		GY_PROTOCOL_CHECK_START_ADDR			6
//#define		DATA_OFFSET_M_PACK						18
//#define		DATA_OFFSET_1_PACK						14


#define		LEN_GY_HEAD_TO_CHECK	6		  // Э��ͷ��У������ܳ�
#define		LEN_GY_CMD_TO_ATTR		8         // GY Э���е����������������Եĳ���
#define		LEN_GY_PACK_INFO	    4		  //  �зְ�ʱ���ְ������Ͱ����ܳ�
//#define		LEN_GY_NO_PACK_HEAD		14        //  �޷ְ���Э��ͷ���ܳ�	

/*typedef		struct	_GY_COMM_PROTOCOL_
{
	uint32	Head;					// 4B��*GY:	// 0x3A59472A
	uint16	Check;				// 2B��У��
	uint16	Cmd;					// 2B��������
	uint32	Index;				// 4B����ˮ��
	union
	{
		uint16	Len;
		struct
		{
			uchar		Len:8;			// 11Bit�����ݶγ���
			uchar		LenH:3;			// 
			uchar		Encry:2;			// 2Bit
			uchar		Pack:1;			// 1Bit
			uchar		Check:2;			// 2Bit
		}Bits;
	}Attr;
	union
	{
		uchar		Buffer[128];		// ʵ�ʳ������2048�ֽڣ���ʹ�ýṹָ��ָ��󻺳�
		uint16	B16[64];				// 
		uint32	B32[32];				// 
		struct
		{
			uint16	Total;
			uint16	Index;
		}Pack;
	}Data;
}GY_COMM_PROTOCOL,*pGY_COMM_PROTOCOL;	*/



/// ϵͳ��־��7�������Ӧ�Ľṹ�����£�����ʹ�����ڿ�����ʽд��ϵͳ��־��
/// ��λ��������������ṹ�����ϵͳ��־��7�����
/*typedef		struct
{
	int32		Longitude;			// 4B�����ȣ��Ŵ���10e7������Ϊ��������Ϊ����
	int32		Latitude;			// 4B��γ�ȣ��Ŵ���10e7������Ϊ��γ����Ϊ��?
	uint32	RtcSecond;			// 4B��ʱ�䣬ͳһΪ�룬2000��1��1��00:00:00Ϊ��0��
	uchar		Speed;				// 1B, �ٶȣ�����/Сʱ
	uchar		Altitude;			// 1B�����Σ���λ��50�ף����50*255��
	uchar		Level;				// 1B��GPSˮƽ��������
	uchar		SatelNumber;		// 1B��GPS���Ǹ���	
	uint16	LAC;					// 2B��GSM��վ��Ϣ1
	uint16	CI;					// 2B��GSM��վ��Ϣ2
	uchar		BSIC;					// 1B��GSM��վ��Ϣ3
	uchar		GsmSignal;			// 1B��GSM��Ϣ����ֵ
	uchar		eTepm;				// 1B�������¶ȣ�-127 ~ +127
	uchar		Rsv;					// 1B������
	uint32	VehicleStatus1;	// ����״̬��־λ��ǰ32λ
	uint32	VehicleStatus2;	// ����״̬��־λ����32λ	
} GG_INFO,*pGG_INFO;	  */


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   ������
*/
typedef	struct	
{	
	uint32	U0_232;
	uint32	U2_232;
	uint32	U2_485;
	uint32	CanBus;
	uint32	U1_GSM;	
	uint32	U3_GPS;
		
} BAUDRATE,*pBAUDRATE;
USER_EXT    volatile		BAUDRATE		Baudrate;





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   ��ʻԱ�����Ϣ��EEPROM�洢�ռ�Ϊ�����洢���������
*/
#define		LEN_ALL_DRIVER_INFO			(32+16*4+8)
#define		LEN_DRIVER_INFO				16
#pragma		pack(1)
typedef	struct	
{	
	uint32	ID;					
	uint32	LogInTime;
	uint32	LogOutTime;
	uchar		LogStatus;		// 2��״̬����¼���˳�
	uchar		CardStatus;		// 2��״̬���Ϸ������Ƿ���
	uchar		Rsv1,Rsv2;
} DRIVER_INFO,*pDRIVER_INFO;
USER_EXT    volatile		DRIVER_INFO		Driver,Driver1,Driver2,Driver3;	// ��ǰ�ź����4����
USER_EXT    uint32		DriverID[8];	// ���ͬʱ8��˾������ͨ�����������޸�


//#define		LEN_TIRED_DRIVE			8
//typedef	struct
//{
//	uint16	WorkTime;	// ���������ʱ��
//	uint16	RestTime;	// �����Ϣʱ��
//	uchar		OnTime;		// ����Ϣת�������Ĺ���ʱ��
//	uchar		OffTime;		// �ӹ���ת����Ϣ�Ĺ���ʱ��
//	uchar		Speed;		// ��ֹƯ�ƣ��ٶȴﵽNֵ��ʼ�㹤��ʱ��
//	uchar		Para;			// 
//} TIRED_DRIVE,*pTIRED_DRIVE;
//
//USER_EXT    volatile		TIRED_DRIVE		TiredDrive;


/*typedef	struct
{
	uint32	WorkTime;	      //���������ʱ��
	uint32   WorkTimeOneDay;   //�����ۼƼ�ʻʱ������ 
	uint32	RestTime;	      //�����Ϣʱ��
	uint32   StopTime;         //�ͣ��ʱ��
   uchar		OnTime;		      //����Ϣת�������Ĺ���ʱ��
	uchar		OffTime;		      //�ӹ���ת����Ϣ�Ĺ���ʱ��
	uint32	Para;			   
} JT_TIRED_DRIVE,*pJT_TIRED_DRIVE;	

USER_EXT    volatile		JT_TIRED_DRIVE		JT808TiredDrive;	  */




#define		LEN_PHONE_CALL_STRUCT		54
#pragma		pack(1)
typedef		struct
{
	uint32	TimeCnt;
	uint32   RingingCnt;

	uchar		DialFlag;
	uchar		RingFlag;
	uchar		Length;
	uchar		Busing;	

	uchar		KeyHang;		// �����һ�����
	uchar		GsmHang;		// GSMģ��ָ��ָ��һ�
	uchar		CalledAnswer;	// ���У��ֱ�Ӧ��
	uchar		CallerAnswer;	// ���У��Է�Ӧ��

	uchar		DialCnt;
	uchar		ListeningFlag;
	uchar    Answer;
	uchar    RingCnt;

	uchar    Dialing;
	uchar		CenterCall;		// ���ĺ���
	uchar    RING;
	char		Buffer[32];
} DIAL_PHONE,*pDIAL_PHONE;
USER_EXT   DIAL_PHONE		PhoneCall;


/////////// add by xzb  ///////////////
USER_EXT		enum
{
	eTMR_TMP_SERVER	= 0,
	eTMR_ROAD_LINE		= 1,
	eTMR_CAN_BUS		= 2,
}ENUM_TIMER;
#define		TIMER_NUMBER	4
#define		CNT_NUMBER		12
typedef		struct
{
	uint32	InsertIC_Card;
	uint32	InsertTF_Card;
	uint32	SystemSecond;
	uint32	GPS_RtcSecond;
	uint32	RtcSecond;
	uint32	R8025_Second;
	uint32	GetDataSecond;
	uint32	AlarmDelay;
	uint32	PulsControl;			// �����������
	uint32	TickForRandData;		// ʹ���ظ��жϼ���Ϊ�����
	uint32	UpDateFrame1;			// 
	uint32	UpDateFrame2;			// 	
	uint16	Timer[TIMER_NUMBER];
} COUNTER,*pCOUNTER;
USER_EXT   COUNTER			gCnt;



USER_EXT    volatile     uint32      SpeedBuff[10];


USER_EXT	uchar		TxdTBuffer[1024+16];
USER_EXT	uchar    U0_TxdBuffer[512];//  				__attribute__((at(0x7fe00400)));	// 512B
USER_EXT	uchar		U0_RecBuffer[1024+256];//				__attribute__((at(0x7fe00600)));	// 512B

#define		GPS_VALID_FLAG				'A'
#define		GPS_INVALID_FLAG			'V'
USER_EXT    volatile		uchar			GPSValidFlag,VirtaulGPS_Valid;

USER_EXT   volatile		uint32		GPS_RtcSecond;

USER_EXT   volatile		uint32		CanAutoSendInterval;


USER_EXT    volatile		uchar       OverSpeedAlarmType;   //���ٱ�������
USER_EXT    volatile		uchar       InOutAreaType;        //�������򱨾�����
USER_EXT    volatile		uchar       InOrOutArea;          //����������ʶ

USER_EXT    volatile		uint32      RoundOverSpeedAlarmTime;  //Բ��Χ�����ٱ���ʱ��



// add by xzb
USER_EXT	 uchar	ManulAlarmEventID;			  // �˹������¼�ID

USER_EXT  	 uint32	OverSpeedAreaRoadID;
USER_EXT  	 uint32 	FenceOrRoadAlarmID;       // ·��ID
USER_EXT  	 uint16  RoadSectionDriveTime;    // ·����ʻʱ��
USER_EXT  	 uchar  	RoadSectionDriveResult;  // ���


// added 2013.3
USER_EXT	volatile 	uint16   JT808Analog1, JT808Analog2;           // JT808Э���и�����Ϣ�е���·ģ����





USER_EXT   	volatile		uint32      UpdataCounter;
USER_EXT   	volatile		uint32      UpdataCounterP;
USER_EXT   	volatile		uchar			 FTPdowningflag;

USER_EXT   	volatile		uint32      FtpFileBytes;

#define     LPC1756_FlashSize         0x40000			  // 		256K

#define		BD_Firware_Size			(0x20000+0x34)			// 128K 

//USER_EXT  volatile		uchar			AlarmValue;
USER_EXT   	volatile		uint32		AlarmDelayCnt;



#define		SEND_SMS_GY_FLAG		0x55

USER_EXT    volatile		uint32		GPRS_WaitReturn,SendSMS_GY_Flag;


USER_EXT    volatile		uchar			TcpPicture;
USER_EXT    volatile		uchar			MotoRunFlag;
USER_EXT    volatile		uchar			GPRS_TempPack;
USER_EXT    volatile		uchar			WakeUpType,PowerSelfLock;

USER_EXT    volatile		uchar			JT808_GPRS_TempPack,JT808_QuicklySendPack;

USER_EXT   uchar Jt_photo_upload;					      // ͼƬ�ϴ�ƽ̨��־
USER_EXT   uchar AT_Command_Busing,GSM_TaskBusing;
extern uchar MediaEventFlag;

USER_EXT	   enum		          // ��ý���¼������
{ 	
	eBB_MEDIA_EVENT_PLAT,			  	// ƽ̨�·�ָ��
	eBB_MEDIA_EVENT_ON_TIME,		  	// ��ʱ����
	eBB_MEDIA_EVENT_ROB,			     	// ���ٱ�������
	eBB_MEDIA_EVENT_CRASH,			  	// ��ײ�෭��������
	eBB_MEDIA_EVENT_OPEN_DOOR,			// ��������	
	eBB_MEDIA_EVENT_CLOSE_DOOR,		// ��������	
	eBB_MEDIA_EVENT_SPEED_OVER20,		// �ٶȴӵ���20��������20	
	eBB_MEDIA_EVENT_DISTANCE,			// ����������
	eBB_MEDIA_EVENT_OTHER,          	// 	
	
} BB_MUL_MEDIA_EVENT_ENUM;



///////////////////////////////////////////////// �ߵ͵�ƽ��Ч��־
USER_EXT    enum
{
	HL_DOOR = 0,
	HL_ACC,
	HL_AUTO,
	HL_MAIN_POWER,
	
} enHIGH_LOW_LEVEL;		// �ߵ͵�ƽָ�ⲿ�ӿڵģ���CPU���Ŵ�

#define		NUMBER_OF_HL_IO			5
USER_EXT    volatile		uint32		HighLowLevel;		// 
USER_EXT    volatile		uint32		MainPowerValue,LowPowerAlarmValue;

USER_EXT    enum
{
	WAKE_TYPE_NOTHING = 0,
	WAKE_TYPE_ACC,
	WAKE_TYPE_MOTO,
	WAKE_TYPE_HELP,
	WAKE_TYPE_DOOR,
	WAKE_TYPE_TIME,
	WAKE_TYPE_RADIO_UNLOCK,
}enWAKE_UP_TYPE;







/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   IP���
*/
USER_EXT		enum						  // �����˳�����޸ģ�����
{
	eFLAG_TCP = 0,				  //IP��ַ��DNS		  JT  ��������
	eFLAG_UDP = 1,				  //IP��ַ��DNS
	eFLAG_TCP1 =2,				  //IP��ַ��DNS		  JT  ���ݷ�����
	eFLAG_UDP1 =3,				  //IP��ַ��DNS
	eFLAG_TCP2  =4,			  //IP��ַ��DNS		  JT  ��ʱ���ӻ�����������
	eFLAG_UDP2  =5,			  //IP��ַ��DNS

	eFLAG_IC_AUTH_TCP = 6,			  // IC ����֤TCP
	eFLAG_IC_AUTH_UDP = 7,			  // IC ����֤UDP
	eFLAG_IC_AUTH_B_TCP = 8,		  // IC ����֤����TCP
	eFLAG_IC_AUTH_B_UDP = 9,		  // IC ����֤����UDP

	eFLAG_APN,
	eFLAG_NEED_REG,
//	eFLAG_M_DOMAIN_TCP = 8,
//	eFLAG_M_DOMAIN_UDP = 9,
//	eFLAG_B_DOMAIN_TCP = 10,
//	eFLAG_B_DOMAIN_UDP = 11,
//	eFLAG_C_DOMAIN_TCP = 12,
//	eFLAG_C_DOMAIN_UDP = 13,
	

	
	
} ENUM_IP_INDEX;
#define		TOTAL_IP_NUMBER		6
#define		MAX_SOCKET_NUMBER		15
#define		LEN_IP_PORT				32
#define		LEN_OEM_ID				16

USER_EXT		struct
{
	uchar		TCP[LEN_IP_PORT];			//����TCP��
	uchar		UDP[LEN_IP_PORT];			//����UDP��
	uchar		TCP1[LEN_IP_PORT];
	uchar		UDP1[LEN_IP_PORT];
	uchar		TCP2[LEN_IP_PORT];
	uchar		UDP2[LEN_IP_PORT];
	uchar		APN[16+1];				// APN
	uchar		Socket[12];				// ����ENUM_IP_INDEX 10��IP��ַ�����ӵ�Socketͨ����
	uchar		Online[12];				// ����ENUM_IP_INDEX 10��IP��ַ����״̬
	uchar		EnableFlag[16];			// ʹ�ñ�־ =ENABLE_FLAGʹ�ܣ�����ֵ����
}sIP;

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   �绰�������
*/
#define		LEN_SMS_NUMBER			(16)
USER_EXT		struct
{
	uchar		Center[LEN_SMS_NUMBER+1];
	uchar		Center1[LEN_SMS_NUMBER+1];
	uchar		User[LEN_SMS_NUMBER+1];
	//uchar		User1[LEN_SMS_NUMBER+1];
	//uchar		Help[LEN_SMS_NUMBER+1];
	uchar		SmsCenter[LEN_SMS_NUMBER+1];	// �ƶ���˾SMS���ĺ��� JT808Ҳ���������
	uchar		Sim[LEN_SMS_NUMBER+1];			// �������� JT808Ҳ���������
	uchar		SimBCD_GB[8];
	uchar		SimBCD_JT[8];
}sTelNum;



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   JT808 ������
*/
#define  LEN_IPorDNS		 32
#define  LEN_IP_NAME		 16
#define  LEN_IP_SECRIER	 8

/* ���÷��ͳ�ʱ���Լ��ش����ʱ��
\x0f\x00\x00\x00  \x03\x00\x00\x00  \x0f\x00\x00\x00  \x03\x00\x00\x00  \x0f\x00\x00\x00  \x03\x00\x00\x00  
*/
#define		LEN_SEND_OVT_STRU			24
USER_EXT struct _JT808_Mobile_Send_Para
{	
	uint32 TCP_ACK_OverTime;
	uint32 TCP_ResendTimes;
	uint32 UDP_ACK_OverTime;
	uint32 UDP_ResendTimes;
	uint32 SMS_ACK_OverTime;
	uint32 SMS_ResendTimes;
} SendOVT, *pSendOVT;

// 
#define		LEN_SEND_INTERVAL_STRU			 	(12*4)		// �������פ���ڴ棬���洢������
USER_EXT struct _JT808_DATA_UP
{	
	uint32 HeartbeatTime;
	uint32 NoDriverTime;						          //��ʻԱδ��¼ʱʱ��ر����	  s
	uint32 IDEL_ModeTime;                         //����ʱʱ��ر����				s
	uint32 AlarmModeTime;						       //��������ʱʱ��ر����			s
	uint32 DefauletTime;						          //ȱʡʱ��ر����     s
	uint32 DefauletDistance;						    //ȱʡ����ر����     s
	uint32 NoDriverDistance;						    //��ʻԱδ��¼ʱ����ر����	  s
	uint32 IDEL_ModeDistance;                     //����ʱ����ر����				s
	uint32 AlarmModeDistance;						    //��������ʱ����ر����			s
	union
	{
		uint32	B32;
		struct
		{
			uchar  Mode;                                  //λ�ûر����� 0����ʱ�ر�  1������ر�  2����ʱ�Ͷ���
			uchar  Condition;								       //λ�ûر�����0 ����ACC״̬ 1�����ݵ�¼״̬��ACC״̬�����жϵ�¼״̬������¼�ٸ���ACC״̬
			uchar  Angle;                           	    //�սǲ����Ƕ�
			uchar		Rsv;
		}Byte;
	}MCA;
	uint32	T_Second;	  				// ��ʱ����ʱ����
	uint32	T_ContinuedTime;			// ��ʱ����ʱ��
	uint32	Second;					  	// ��רʱ����
	uint32	Distance;					// �ϴ�������	
} SendInterval, *pSendInterval;



#define   LEN_JT808_IP_DNS	        (4*LEN_IPorDNS+3*LEN_IP_NAME+2*4)

#pragma		pack(1)
USER_EXT struct _JT808_sIP
{
	uchar  M_APN[LEN_IPorDNS];
	uchar  M_IP_Name[LEN_IP_NAME];
	uchar  M_IP_Secrier[LEN_IP_SECRIER];
	uchar  M_IPorDNS[LEN_IPorDNS];
	uchar  B_APN[LEN_IPorDNS];
	uchar  B_IP_Name[LEN_IP_NAME];
	uchar  B_IP_Secrier[LEN_IP_SECRIER];
	uchar  B_IPorDNS[LEN_IPorDNS];
	uint32 TCP_Port;
	uint32 UDP_Port;
	uchar  IC_Auth_M_IPorDNS[LEN_IPorDNS];
	uchar  IC_Auth_B_IPorDNS[LEN_IPorDNS];
	uint32 IC_Auth_TCP_Port;
	uint32 IC_Auth_UDP_Port;
} JT808_IP_DNS, *pJT808_IP_DNS;


#define    LEN_NUMBER   	  16
#pragma		pack(1)
USER_EXT struct _JT808_Number
{
	uchar  Center[LEN_NUMBER];							  //���ƽ̨����
	uchar  Reset[LEN_NUMBER];							  //��λ����
	uchar  ResetFactory[LEN_NUMBER];					  //�ָ��������ú���
	uchar  CenterSMS[LEN_NUMBER];						  //���ƽ̨���ź���
	uchar  TextSMS[LEN_NUMBER];						  //�����ı���������
	uchar  GetCallMode;									  //�������緽ʽ0�� �Զ�����  1�� ACC onʱ�Զ����� Offʱ�ֶ�����
	uint32 CallTime;										  //ÿ������ͨ��ʱ��
	uint32 CallTimeMonth;						        //ÿ������ͨ��ʱ��
	uchar  JT[LEN_NUMBER];								  //��������
	uchar  Privilege[LEN_NUMBER];						  //��ȨSMS����
} JT808_Number, *pJT808_Number;

#pragma		pack(4)
USER_EXT struct _JT808_ALARM
{
	uint32 Shield;                                 //���������֣�������־��ӦλΪ1����Ӧ����������
	uint32 SendTextSMS;								     //������־��ӦλΪ1 ������ʱ�����ı�����
	uint32 TakePhoto;						              //������־��ӦλΪ1 ������ʱ����	 
	uint32 SavePhoto;                              //������־��ӦλΪ1 ������ʱ�洢��Ƭ��	�����ϴ���Ƭ
	uint32 KeyAlarm;						              //������־��ӦλΪ1 ���ñ���Ϊ�ؼ�����			

} JT808_Alarm_Word, *pJT808_Alarm_Word;




#pragma		pack(1)
USER_EXT struct _JT808_Camera_Para_
{
	uchar  Quality;                                 //ͼ��/ͼƬ���� 1~10�� 1�����
	uchar  Light;								            //����
	uchar  Contrast;						               //�Աȶ�	 
	uchar  Satulation;                              //���Ͷ�
	uchar  Colour;						                  //ɫ��			

} JT808_Camera_Para, *pCamera_Para;


#pragma		pack(1)
USER_EXT struct _JT808_Ctr_sIP				        //����ָ���ķ�����������������
{
	uchar  Ctr;												  //���ӿ��Ʒ�0���л���ָ��ƽ̨��1���л���ԭ�����ƽ̨
	uchar  C_ANP[LEN_IPorDNS];
	uchar  C_IP_Name[LEN_IP_NAME];
	uchar  C_IP_Secrier[LEN_IP_SECRIER];
	uchar  C_IPorDNS[LEN_IPorDNS];
	uint32 C_TCP_Port;
	uint32 C_UDP_Port;
	uchar  CP_ID[5];
	uchar  Akey[LEN_IP_NAME];										  //��ʱ���ӵ�ƽ̨��Ȩ��
	uchar  HVS[LEN_IP_NAME];
	uchar  HWVS[LEN_IP_NAME];
	uchar  URL[LEN_IPorDNS];
	uint16 Net_Time;
} JT808_Ctr_IP_DNS, *pJT808_Ctr_IP_DNS;

#define   LEN_JT_CTR_IP_DNS       (1+3*LEN_IPorDNS+4*LEN_IP_NAME+LEN_IP_SECRIER+3*2+5)






#pragma		pack(1)
USER_EXT struct _JT808_GET_GB_DATA_			//Զ�̶�ȡ��¼����
{
	uint16  Index;					   			//�·��������ˮ��    
	uchar   GetDataCmd;                  //�·�GB������
	uchar   Rvs;        
	uint16  LenPack;						      //���ݰ���С
	uint16  SendPackNummber;					//Ŀǰ���䵽�ڼ�������
	uint16  PackNummber;                  //�ּ�������		
	uint32  LenAllData;						   //δ�����ݳ���
	uint16  PageAddr;                      //��ǰ��������flashҳ
	uint16  LenReadFromFF;                 //��flash���������ݳ���
	
}JT_GET_GB_DATA,  *pJT_GET_GB_DATA;


////////////////////////////////////////////////////////
#pragma		pack(1)
typedef		struct
{
	uchar		Head;
	uchar		Check;
	uint16	Ver;
	uint16	OemID;
	uchar		DeviceType;
	uchar		Cmd;
	union
	{
		uchar		Buffer[128];
		uint32	B32[128/4];
	}Data;
}JT808_EXT_DEVICE_DATA,*pJT808_EXT_DEVICE_DATA;


USER_EXT    		uchar		sPIN_Code[4+2];
USER_EXT    		uchar		sOEM_ID[16+2];
USER_EXT    		uchar		sPackHead[5];


USER_EXT    		uchar		SimNumberBCD[8];							// �����ֻ�����BCD

USER_EXT    		char		sNowDateString[7];
USER_EXT    		char		sNowTimeString[7];

#define				USE_GPRS_PROTOCOL			'U'						// GB19506Э��Ĭ��Ϊ�̶�ʹ�ã��ɼ�����4��
#define		      GY_PROTOCOL			0
#define		      GB_PROTOCOL			1
#define		      JT_PROTOCOL			2

USER_EXT    struct
{
	uchar		GPRS[4];
	uchar		Disp[4];
} Protocol;

#define		IC_AT24C_CARD		'A'
#define		IC_4442_CARD		'4'
USER_EXT    volatile		uchar		IC_CardType;		// 0,'A','4'������״̬��0�޿�

///////////// end        //////////////////

typedef		struct  
{
	unsigned long  Key; 
	unsigned long	A;
	unsigned long	B;
	unsigned long	C;
} ENCRY_KEY,*pENCRY_KEY;	
USER_EXT		ENCRY_KEY	gKey;	


USER_EXT    volatile		uchar		CenterNumFlag,CenterNum1Flag,CenterNum2Flag;

///////////// ADD BY HYC /////////////////////////////////////
USER_EXT  	volatile		char		EnableParkOverTimeAlarm;				// ����ʹ��ͣ����ʱ�������
USER_EXT    volatile		uint16	ParkOverTimeAlarmTime;					// ͣ����ʱ����ʱ��(��)


USER_EXT    volatile		uchar		AlarmStatusFlag;

USER_EXT    volatile		uint16	EnableAlarmFlag;							// �ܾ������־(bit0:Ϊ1��������;bit1:�پ�;bit2:δ�������;bit3:�ϳ�����)

USER_EXT    volatile		char		EnableStealAlarm;							// ����ʹ�õ������
USER_EXT    volatile		char		EnableRobAlarm;							// ����ʹ�ýپ����
USER_EXT    volatile		char		EnableNoSetScanAlarm;					// ����ʹ��δ����������
USER_EXT    volatile		char		EnableTrailCarAlarm;						// ����ʹ���ϳ��������
USER_EXT    volatile		char		EnableHelpAlarm;							// ����ʹ�������������
USER_EXT    volatile		char		EnableTireDriveAlarm;					// ����ʹ��ƣ�ͼ�ʻ���
USER_EXT    volatile		char		EnableIllegalOpenDoor;					// ����ʹ�÷Ƿ����ż��
USER_EXT    volatile		char		EnableIllegalMakeFire;					// ����ʹ�÷Ƿ������
USER_EXT    volatile		char		EnableCheckShake;							// ����ʹ���𶯼��
USER_EXT    volatile		char		EnableAutoLockDoor;						// ����ʹ������
USER_EXT    volatile		char		EnableLowVoltage;							// ����ʹ�õ�ѹ���

USER_EXT    volatile		char		EnableDialOrAnswerFlag;					// ����ͨ�����ܱ�־(bit0:��1������;bit1:��1�������)
USER_EXT    volatile		char		EnableRemoteMonitor;						// ����Զ�̼�����־


USER_EXT    volatile		char		SetScanAlarmFlag;							// �������״̬��־

USER_EXT    volatile		char		PulsControlCnt;							// ����ʱ�ķ�������

USER_EXT    volatile		char		LockDoorFlag;								// ���ű�־		
USER_EXT    volatile		char		SPK_Flag;	  								// ���ȱ�־		 	  	
USER_EXT    volatile		char		AlarmLight_Flag;							// ���Ʊ�־
USER_EXT    volatile		char		DoorStatus;									// �Ŵ�״̬
USER_EXT    volatile		char		LockDoorSignal;							// �����ź�������

USER_EXT    volatile		char		HandleRecvDataFlag;


USER_EXT    volatile		uchar		CheckLinkModeEnable;	
USER_EXT    volatile		uchar		CameraWorking,SendPicData,SendAudioData;

USER_EXT		volatile     uchar	TempGPSValidTime;

USER_EXT		volatile     uint32	InvalidGPSValidTime;		  // ����λʱ��


	
/*
////////////////////////////////////////////////////////////////////////////////////////
USER_EXT    volatile		uchar		DisplayLcdType;							// ��������
USER_EXT    volatile		uchar		MakePhoneAndReceiveFlag;
USER_EXT    volatile		uint16	DisplayLcdOverTime;
USER_EXT    volatile		uchar		SendHandleSignalFlag;	*/
USER_EXT    volatile		uchar		HandleHoldFlag;	  


USER_EXT    struct
{
	uchar		Enable;
	uchar		Brake;
	uchar		Acc;
	uchar		Motor;
}SimuSpeed;



USER_EXT    volatile		char		HandleWatchCarPassword[6];			   	// �ֱ���������
USER_EXT    volatile		uchar		HandleCmdtype;
USER_EXT    volatile		uchar		LockCarFlag;                  	      // ������־
USER_EXT    volatile		uchar		LoginFlag;								   	// ��½��Ա��־
USER_EXT    volatile		uchar		HandleLoginSuccess;					   	// �ֱ������¼�ɹ���־

USER_EXT    volatile		uchar		LcmDataToServer;

USER_EXT    volatile		uint16		Plat_Serial_No;						  // ��¼�ض���ƽ̨�ظ���ˮ��
																	
USER_EXT    	uchar    	FactoryPassword[8];									// �ֱ��ĳ�����������
USER_EXT    	uchar    	EnableHandleWatchCar;								// �Ƿ������ֱ�����
USER_EXT    	uchar    	HandleWatchCarMode;									// �ֱ�������ʽ


USER_EXT    	uchar    	GpsTempSaveFlag;										// GPS��Ϣ��ʱ�洢��־

USER_EXT    	uint16		Send_Msg_Record_Start;
USER_EXT    	uint16		Send_Msg_Record_Count;
USER_EXT    	uint16		UpDataFrameCnt1,UpDataFrameCnt2;					// 



USER_EXT		uchar			NeedRegServer,TempNeedRegServer,UseFlowControl,Uart0CommandFlag;

USER_EXT		uchar			IC_Card_Auth_Flag,IC_Card_Auth_GPRS;
USER_EXT		uchar			Rfid_Power_On;

USER_EXT		uchar		IC_Card_Auth_Buffer[32];




USER_EXT		uchar			GPRS_SendAligFull,DTU_Busing,RTU_Busing,GSM_Busing;

USER_EXT		uchar			GPRS_Transparent;

#define		U2_FOR_CAMERA			'C'		// RS485
#define		U2_FOR_AUDIO			'A'
#define		U2_FOR_RFID				'R'
#define		U2_FOR_DISPLAY			'D'
USER_EXT		uchar			U2PinSelDevice;


//USER_EXT uchar	JT_read_direction;
//USER_EXT uchar JT_read_motion_event;
USER_EXT	uchar	accel_interrupt_flag;
USER_EXT	uchar	I2C_Busy_Now_Flag;
USER_EXT	uchar	Current_Use_4442_Flag;


USER_EXT enum 
{
	///// �¼�
	ACCEL_NO_EVENT,
	ACCEL_TILT,
	ACCEL_SINGLE_TAP,
	ACCEL_DOUBLE_TAP,

	///// ����
	ACCEL_NO_CHANGE,
	ACCEL_FACE_UP,
	ACCEL_FACE_DOWN,
	ACCEL_UP,
	ACCEL_DOWN,
	ACCEL_RIGHT,
	ACCEL_LEFT,

	ACCEL_TAP,
	
} ACCEL_DIRECTION_ENUM;




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�ж����������(С�����ȼ��ߣ�
*/
USER_EXT 	enum 
{
	CAN_INT_PRIORITY		=1,
	RIT_INT_PRIORITY     ,		   //WRC
	MCPWM_INT_PRIORITY	,
	RTC_INT_PRIORITY		,
	TIMER3_INT_PRIORITY	,		   //WRC
	TIMER2_INT_PRIORITY  ,	
	TIMER0_INT_PRIORITY  ,
	TIMER1_INT_PRIORITY	,
	UART0_INT_PRIORITY	,
	UART1_INT_PRIORITY	,
	UART2_INT_PRIORITY	,
	UART3_INT_PRIORITY	,
	SSP0_INT_PRIORITY		,
	SSP1_INT_PRIORITY		,
	I2C0_INT_PRIORITY    ,	
	GPIO_EINT3_PRIORITY  ,
}ENUM_INT_PRIORITY;



USER_EXT 	enum 
{
	eST_ER_FLASH 	= 0x01,
	eST_ER_FRAM		= 0x02,
	eST_ER_GSM		= 0x04,
	eST_ER_GPS		= 0x08,
	eST_ER_ANT_OPEN	= 0x10,
	eST_ER_ANT_SHORT	= 0x20,
	eST_ER_CAMERA		= 0x40,
	eST_ER_RFID			= 0x80,
}ENUM_SELF_TEST_ER_CODE;



//USER_EXT    	uchar    	UDP_CommunicateFlag;												// 

USER_EXT    	uchar    	SendMessageFlag;
USER_EXT    	uchar    	WhichMessage;
//USER_EXT    	uchar    	SendMsgTest;

//USER_EXT		uchar			PackBuffer[544];			                     //	 GPRS_DATA_PACK_LEN
USER_EXT    	uint32    	UpgradeFlag;											// ������־
USER_EXT    	uchar    	SPI0_Busy,SSP0_Busy,PrintBusy,PrintEvent1,PrintEvent2;

USER_EXT    	uchar    	osInitFlag;


//#define		POWER_SAVE_MODE_FLAG			0x55aa7ffe
//USER_EXT    __noinit__ 		uint32	PowerSaveModeFlag;					

USER_EXT    	uint32    	DialElapsedTime;


/////////// add by wrc  ///////////////
USER_EXT    uint32   VehiclePulseKV;

USER_EXT  volatile	uint16	uiSpeed,uiGPS_Speed,GPS_Speed;

USER_EXT  volatile	uchar    PowerOn,ResetSource,SysReset;

USER_EXT  volatile	uint32	SpeedTackCnt;

USER_EXT  volatile	uchar		ucSpeedLimit;				//���ٷ�ֵ

USER_EXT  volatile	uchar		SpeedModeSelect;			//�ٶ�ȡGPS�ٶȻ��������ٶȱ�־	1 GPS  0 ����

USER_EXT  volatile	uint32	PulsPerMin;

USER_EXT  volatile	uchar 	SystemSecondCntLast;

USER_EXT    volatile		uint16       LenDataForUSB;

USER_EXT    volatile		uchar        USBUseFlag;

USER_EXT    volatile		uchar        Write_File_Error;

USER_EXT    volatile    uchar        Wfile_Flag;              // ��ʶ��һ�β���U��

USER_EXT    volatile		uchar        USBCnt;

USER_EXT    volatile		uchar        USBTaskCreat;		//USB�����������־

USER_EXT    volatile		uchar        PrintTaskCreat;		//��ӡ�����������־

USER_EXT    volatile		uchar        CamererTaskNow;		//��ִ�����������־

USER_EXT    volatile		uint16       NumberOfSatlice;			//�ɼ����Ǹ���	

//USER_EXT    volatile		uchar        GSM_Signal;              //gsm�ź�ǿ�� 

USER_EXT    volatile		uchar        SelfTestResust;   		 //�Լ���

USER_EXT    volatile		uchar        JT_Ctr_PowerDown;          //�ػ�

USER_EXT    volatile		uchar        JT_ShutCommunication;		 //�ر�ͨ�ű�־

USER_EXT    volatile		uint32       JT_ShutMobileCnt;          //�ر���������ͨ��

USER_EXT    volatile		uchar        GetGB_Data_Flag;          //����ͨ��ȡ��¼���ݱ�ʶ 0���������ã�  1��ȡ���ݳ��ȣ�2��ȡ���ݣ�

USER_EXT    		uchar              DriverCardFlag,CurrentRegInfo;            //��ݿ���ʶ1���п���0 �޿�



USER_EXT    volatile		uint32       InitMileage;    	//��ʼ���

USER_EXT    volatile		uchar        LastAzimuth;      //��һ��GPS���ݵĽǶ�

USER_EXT    volatile		uchar        LastAzimuthCnt;	 //�սǼ�����

USER_EXT    volatile		uchar        UseWthSimCard;    //SIM��ѡ�� 6 ����SIM��  0 ����SIM��

USER_EXT    volatile		uchar        SetCarFlat;		// ���ÿ�/��Ȩ�������־

USER_EXT    volatile		uchar        IDEL_For_Acc_OFF;		// ACC�ؽ���ʡ��ģʽ��־

USER_EXT    volatile		uint32        IDEL_Dly_Cnt,CanOverTimeCnt;		// ����ʡ��ģʽ��ʱ��

USER_EXT		volatile		uchar			  SaveLogFlag;			// ������־��־
USER_EXT		volatile		uchar			  InRtcIntFlag;
USER_EXT				uchar			  OverTimeDriveUpateFlag; // 0: ���³�ʱ��ʻ�������	  1:������


//////////////////////////////////////////////

extern		const   unsigned long  	PeriClock[];
extern		const		char		constAPN[];


#pragma		pack(1)
USER_EXT    struct
{
	uint16	MDT_Num0;
	uint32	MDT_Num;
} volatile	hOEM_ID;


#pragma		pack(1)
USER_EXT    union
{
	uchar		Buffer[4];
	uint32	B32;
} volatile	RS485;

USER_EXT		struct
{
	uchar		Start[5];
	uchar		End[5];
	uchar		Value[5];
}AccelPara[2];


// add 2013.3

USER_EXT		uint16		GPS_Azimuth,GPS_Altitude;			//

USER_EXT		uint16		JtCrashAlarmSet;					  // ��ײ������������	 b7-b0: ��ײʱ�䣬��λ��4ms, b15-b8: ��ײ���ٶȣ�
															  // ��λ��0.1g, ���÷�Χ��:0 - 79֮�䣬Ĭ��Ϊ10.
USER_EXT		uint16		JtTiltAlarmSet;						  // �෭�����������ã��෭�Ƕȣ���λ1�ȣ�Ĭ��30��

USER_EXT		uchar       IC_Card_Insert;						   // IC��״̬

USER_EXT		uchar			DialOK_NeedAuthFlag,MenuSetRemoteIAP,UseGPS_Mil,EnableSysSleep,EnableVDR_Upload;

USER_EXT	uchar  CloseSocketStatus, DisConectSomeSocket;						   // �ر�socketͨ��״̬�� �ر�ָ��ͨ����־ 

#define		PARA_BLOCK_CONFIG_FLAG		0xA57E
#define		QUICKLY_SAVING_DATA			0xA5
USER_EXT	uchar  SamlePeriod1S,TestAccuracy,TestBlind,TestCanBus,ReadyTestBlind,TestRectFence,TestRoadLine,GetGnssDataFlag;
USER_EXT	uchar  TestMultiCenter,SystemSelfTest;


USER_EXT	uchar ICCardBusing;			// TRUE: IC���ڲ���,æµ		FALSE��IC������

#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
