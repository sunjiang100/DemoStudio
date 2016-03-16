/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$									
$									
$                       
$								  
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
#ifndef		__SPEED_H__
#define		__SPEED_H__


#ifdef      SPEEDRD_GLOBALS
#define     SPEED_EXT
#else
#define     SPEED_EXT    extern
#endif


#include		"..\Config\RTOS.h"
#include		"Pulse.h"


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>RUN_TIME_LIMIT: ƣ�ͼ�ʻ����ʱ�� <1-300>
//   	<i> Default: 240����
*/
#define		RUN_TIME_LIMIT			240*60			// ƣ�ͼ�ʻ����ʱ��

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>FREE_TIME_LIMIT: ��Ϣʱ������ <1-60>
//   	<i> Default: 20����
*/
#define		FREE_TIME_LIMIT		20*60			// ��Ϣʱ������

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    ������ر�������
*/


#define		LEN_SAVE_STOP_DATA			234
#pragma		pack(1)
typedef		struct
{
	uchar		DateTime[6];
	uchar		DriverID[18];
	union
	{
		uchar		Buffer[10*20];		// 20�룬ÿ��10�ֽ�
		struct
		{
			uchar	Speed_0S;	  			// 
			uchar	Sw_0S;
			uchar	Speed_D2S;
			uchar	Sw_D2S;
			uchar	Speed_D4S;
			uchar	Sw_D4S;
			uchar	Speed_D6S;
			uchar	Sw_D6S;
			uchar	Speed_D8S;
			uchar	Sw_D8S;			
		}Sec[20];	// 20������
	}Data;
	struct
	{
		uint32	Lon;
		uint32	Lat;
		uint16	High;
	}Pos;
} SAVE_STOP_DATA,*pSAVE_STOP_DATA;


typedef		struct
{
	uchar	Speed_0S;	  			// 
	uchar	Sw_0S;
	uchar	Speed_D2S;
	uchar	Sw_D2S;
	uchar	Speed_D4S;
	uchar	Sw_D4S;
	uchar	Speed_D6S;
	uchar	Sw_D6S;
	uchar	Speed_D8S;
	uchar	Sw_D8S;			
}STOP_DATA,*pSTOP_DATA;	// 30������



typedef		struct
{
	uchar		StartTime[6];
	SP_SW		SpeedSw[60];
} SP_SW_PER_MIN,*pSP_SW_PER_MIN;


#define		LEN_CURRENT_DRIVER_INFO		42
typedef		struct
{
	uchar		ID[18];				//��ʻ֤����
	uchar		Name[20];			//��ʻԱ����
	uint32	Number;				//��ʻԱ����
}CURRENT_DRIVER_INFO,*pCURRENT_DRIVER_INFO;



#define		LEN_LOCATOIN_INFO_PER_MIN	11
#define		LEN_LOCATION_INFO				10
typedef		struct
{
	uint32	Lon;
	uint32	Lat;
	uint16	Altitude;
}LOCATION_INFO,*pLOCATION_INFO;

/////////////////////////////////// ����ʻԱ�ļ�ʻʱ����Ϣ���洢���6λ��ʻԱ������ǰ��ʻԱ��
//////// ����6λʱ�����������λ���忨ʱ��������λ��
//////// ��ǰ��ʻԱ�ļ�ʻʱ�䳬��ͣ��ʱ�䣬������ʻԱ�ļ�ʻʱ��ȫ�����㡣
/*#define		LEN_DRIVER_INFO_TIME		48
typedef		struct
{	
	uint16	Number;					// ��ʻԱ�Ա�ţ��ڲ�ʹ��
	uchar		ID[18];			
	uint32	StartTimeSec;			// ��ʻ��ʼʱ�䣬�루2000��1��1��00:00:00Ϊ��0�룩
	uint32	EndTimeSec;				// ��ʻ����ʱ�䣬��
	uint16	TravelTimeSec;			// ��ʻʱ�䣬�룬������ʱ�����ֵ65535ʱ���洢һ��ƣ�ͼ�ʻ��¼
	uint16	RestTimeSec;			// ��Ϣʱ�䣬��
	uint16	OneDayTravelTimeSec;	// 1���ܼ�ʻʱ�䣬�룬������ʱ�����ֵ65535ʱ���洢һ��ƣ�ͼ�ʻ��¼	
	uint16	ParkingTimeSec;		// ͣ��ʱ��
	LOCATION_INFO	StartLocatoin;			// ��ʼλ����Ϣ	
	uint16	OffTime;	               // ����λʱ��
}DRIVER_INFO_TIME,*pDRIVER_INFO_TIME;	*/


#define		LEN_DRIVER_INFO_TIME		56
typedef		struct
{	
	uint16	Number;					// ��ʻԱ�Ա�ţ��ڲ�ʹ��
	uchar		ID[18];			
	uint32	StartTimeSec;			// ��ʻ��ʼʱ�䣬�루2000��1��1��00:00:00Ϊ��0�룩
	uint32	EndTimeSec;				// ��ʻ����ʱ�䣬��
	uint32	TravelTimeSec;			// ��ʻʱ�䣬�룬������ʱ�����ֵ65535ʱ���洢һ��ƣ�ͼ�ʻ��¼
	uint32	RestTimeSec;			// ��Ϣʱ�䣬��
	uint32	OneDayTravelTimeSec;	// 1���ܼ�ʻʱ�䣬�룬������ʱ�����ֵ65535ʱ���洢һ��ƣ�ͼ�ʻ��¼	
	uint32	ParkingTimeSec;		// ͣ��ʱ��
	LOCATION_INFO	StartLocatoin;			// ��ʼλ����Ϣ	
	uint16	OffTime;	               // ����λʱ��
}DRIVER_INFO_TIME,*pDRIVER_INFO_TIME;			



#define		LEN_DRIVING_TIME_LIMIT		20
typedef		struct
{
	uint32	TravelTimeSec;			// ��ʻʱ�䣬�룬������ʱ�����ֵ65535ʱ���洢һ��ƣ�ͼ�ʻ��¼
	uint32	RestTimeSec;			// ��Ϣʱ�䣬��
	uint32	OneDayTravelTimeSec;	// 1��������ۼƼ�ʻʱ��
	uint32	ParkingTimeSec;		// ������ͣ��ʱ��
	uint16	PreAlarmTime;			// Ԥ��ʱ��
	uchar		EnableCheckFatigue;	// ʹ�ܼ��ƣ�ͼ�ʻ
	uchar		EnableCheckParking;
}DRIVING_TIME_LIMIT,*PDRIVING_TIME_LIMIT;


#define		LEN_SAVE_FATIGUE_DRIVING_INFO		50
typedef		struct
{
	uchar		ID[18];
	uchar		StartTime[6];
	uchar		EndTime[6];
	LOCATION_INFO	StartLocation;
	LOCATION_INFO	EndLocation;
}SAVE_FATIGUE_DRIVING_INFO,*pSAVE_FATIGUE_DRIVING_INFO;


SPEED_EXT		enum
{
	eSPEED_CMD_GET_DRIVING_TIME 	= 0x01,
	eSPEED_CMD_GET_OVER_SPEED		= 0x02,
	eSPEED_CMD_DISABLE_CNT			= 0x03,

  	eSPEED_CMD_SET_DRIVING_TIME 	= (0x01+0x80),
	eSPEED_CMD_SET_OVER_SPEED		= (0x02+0x80),

}ENUM_SPEED_PARA;


#define		LEN_OVER_SPEED_PARA		8
typedef		struct
{
	uchar		Enable;
	uchar		HighSpeedLimit;
	uchar		LowSpeedLimit;
	uchar		PreAlarmDifference;
	uint16	EnableTime;
	uint16	SecondCnt;	
} OVER_SPEED_PARA,*pOVER_SPEED_PARA;

// ��ʻ����ݼ�¼ (��ʻԱ�ǽ��ǳ�״̬��
#define		LEN_DRIVER_IN_OUT_STATUS		25
typedef		struct
{
	uchar Time[6];  // �¼�����ʱ��
	uchar ID[18];	 // ��������ʻ֤����
	uchar	EvenType;	// �¼����� �¼����ͣ�01H����¼��02H���˳�������Ԥ��		
} DRIVER_IN_OUT_STATUS,*pDRIVER_IN_OUT_STATUS;

// �ٶ�״̬�ж� 
#define		SPEED_STATUS_NORMAL					1
#define		SPEED_STATUS_UNNORMAL				2
  
#define		LEN_SPEED_STATUS_JUDGE		8
typedef		struct
{
	uchar	Flag;  // 0: δ�ж��� 1�����ж�
	uchar	Status;	// 1���ٶ�������2���ٶ��쳣
	uchar LastTime[6];  // �ж�����ʱ�� BCD��
} SPEED_STATUS_JUDGE,*pSPEED_STATUS_JUDGE;

#define		LEN_SPEED_STATUS_LOG		133
typedef		struct
{
	uchar	Status;	// 1���ٶ�������2���ٶ��쳣	��¼�ǵ��ٶ�״̬
	uchar StartTime[6];  // �ж�����ʱ�� BCD��
	uchar EndTime[6];  // �ж�����ʱ�� BCD��
	struct 
	{
		uchar RecordSpeed;
		uchar RefSpeed;
	}Speed[60];

} SPEED_STATUS_LOG,*pSPEED_STATUS_LOG;

SPEED_EXT SPEED_STATUS_JUDGE	  SpeedStatusJudge;

SPEED_EXT SPEED_STATUS_LOG SpeedStatusLog;

SPEED_EXT uint16	AlertLogonTime;    // ��ʾ�忨��½��ʱ

SPEED_EXT	uchar		StopFlag;

#define	DRIVER_NUMBER_MIN			10000			 //  ��ʻԱ�Ա����Сֵ��С�����ֵ����Ϊ��Ч





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	UART��
*/
class			SpeedCls
{
public:
	SpeedCls(void);
	uint16     	DrivingDataRecording(uint16 tSpeed);
	void	   	TwoDayMilage(uint32  PulsMin,uint16 VCC);
	void	   	DrivingDataInit(uchar );
	uchar			CheckNewDriver(void);
	uchar			CheckDriverCard(void);
	void     	FatigueDrivingFun(uint16	Speed);
	void	   	OverSpeedFun(uint16   Speed);
	void			GetCurrentDriver(uchar *pDriver);
	void			SetGetPara(uchar Cmd,uchar Len,uchar *pData);
	void			ReceiveRFID(uint16 Length,uchar *pData);
	uchar			SendDataToRFID(uchar Cmd, uchar *pData, uint16 Len);	
	uchar			GetDataFromRFID(uchar Cmd, uchar *pData, uint16 Len);
	void			U2_ForRFID_Fun(void);		//
	uchar			U2_SendDataToRFID(uchar Cmd, uchar *pData, uint16 Len);
	uchar			RFID_ReaderSeltTest(void);
	uint16		SpeedStatusLogFun(void);
	uchar			InsertCardFlag;
		
private:
	
	void     RecodeInOut(uchar InOrOut);
	void     Save360HDataToFlash(uchar *pBuffer);
	void		Save360HDataToFlashAndClearFMPosData(uchar *p256ByteBuffer, uchar *p64ByteBuffer);
	//SAVE_STOP_DATA		SaveStopData;
	STOP_DATA	StopData[30];
	uint16		Last5Speed[5];	
	SP_SW_PER_MIN	SpeedSwPerMin[2];
	
	uchar			SpeedSwMinFlag;			// ÿҳ��2���ӣ�ǰһ���ӱ�־0����һ���ӱ�־Ϊ1
	
	CURRENT_DRIVER_INFO		CurrentDriverInfo;							 
	DRIVER_INFO_TIME			CurrentDriverInfoTime;
	DRIVER_INFO_TIME			RecentDriverInfoTime[2];	  // 

	LOCATION_INFO				LocationInfo,ValidLocationInfo;
	
	DRIVING_TIME_LIMIT		LimitTime;

public:
	OVER_SPEED_PARA			OverSpeed;	

};

SPEED_EXT	__noinit__		SpeedCls		SpeedC;		// 




/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
#endif

