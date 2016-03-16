/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$									
$									
$                       
$								  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 
	功能描述：
	编译环境：
	目标CPU: 
	作者：
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
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
//   	<o>RUN_TIME_LIMIT: 疲劳驾驶上限时间 <1-300>
//   	<i> Default: 240分钟
*/
#define		RUN_TIME_LIMIT			240*60			// 疲劳驾驶上限时间

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>FREE_TIME_LIMIT: 休息时间下限 <1-60>
//   	<i> Default: 20分钟
*/
#define		FREE_TIME_LIMIT		20*60			// 休息时间下限

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    接收相关变量定义
*/


#define		LEN_SAVE_STOP_DATA			234
#pragma		pack(1)
typedef		struct
{
	uchar		DateTime[6];
	uchar		DriverID[18];
	union
	{
		uchar		Buffer[10*20];		// 20秒，每秒10字节
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
		}Sec[20];	// 20秒数据
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
}STOP_DATA,*pSTOP_DATA;	// 30秒数据



typedef		struct
{
	uchar		StartTime[6];
	SP_SW		SpeedSw[60];
} SP_SW_PER_MIN,*pSP_SW_PER_MIN;


#define		LEN_CURRENT_DRIVER_INFO		42
typedef		struct
{
	uchar		ID[18];				//驾驶证号码
	uchar		Name[20];			//驾驶员姓名
	uint32	Number;				//驾驶员代码
}CURRENT_DRIVER_INFO,*pCURRENT_DRIVER_INFO;



#define		LEN_LOCATOIN_INFO_PER_MIN	11
#define		LEN_LOCATION_INFO				10
typedef		struct
{
	uint32	Lon;
	uint32	Lat;
	uint16	Altitude;
}LOCATION_INFO,*pLOCATION_INFO;

/////////////////////////////////// 各驾驶员的驾驶时间信息，存储最近6位驾驶员（含当前驾驶员）
//////// 超过6位时，覆盖最旧那位（插卡时间最早那位）
//////// 当前驾驶员的驾驶时间超过停车时间，其它驾驶员的驾驶时间全部清零。
/*#define		LEN_DRIVER_INFO_TIME		48
typedef		struct
{	
	uint16	Number;					// 驾驶员自编号，内部使用
	uchar		ID[18];			
	uint32	StartTimeSec;			// 驾驶开始时间，秒（2000年1月1日00:00:00为第0秒）
	uint32	EndTimeSec;				// 驾驶结束时间，秒
	uint16	TravelTimeSec;			// 驾驶时间，秒，等于限时或最大值65535时，存储一条疲劳驾驶记录
	uint16	RestTimeSec;			// 休息时间，秒
	uint16	OneDayTravelTimeSec;	// 1天总驾驶时间，秒，等于限时或最大值65535时，存储一条疲劳驾驶记录	
	uint16	ParkingTimeSec;		// 停车时间
	LOCATION_INFO	StartLocatoin;			// 起始位置信息	
	uint16	OffTime;	               // 不在位时间
}DRIVER_INFO_TIME,*pDRIVER_INFO_TIME;	*/


#define		LEN_DRIVER_INFO_TIME		56
typedef		struct
{	
	uint16	Number;					// 驾驶员自编号，内部使用
	uchar		ID[18];			
	uint32	StartTimeSec;			// 驾驶开始时间，秒（2000年1月1日00:00:00为第0秒）
	uint32	EndTimeSec;				// 驾驶结束时间，秒
	uint32	TravelTimeSec;			// 驾驶时间，秒，等于限时或最大值65535时，存储一条疲劳驾驶记录
	uint32	RestTimeSec;			// 休息时间，秒
	uint32	OneDayTravelTimeSec;	// 1天总驾驶时间，秒，等于限时或最大值65535时，存储一条疲劳驾驶记录	
	uint32	ParkingTimeSec;		// 停车时间
	LOCATION_INFO	StartLocatoin;			// 起始位置信息	
	uint16	OffTime;	               // 不在位时间
}DRIVER_INFO_TIME,*pDRIVER_INFO_TIME;			



#define		LEN_DRIVING_TIME_LIMIT		20
typedef		struct
{
	uint32	TravelTimeSec;			// 驾驶时间，秒，等于限时或最大值65535时，存储一条疲劳驾驶记录
	uint32	RestTimeSec;			// 休息时间，秒
	uint32	OneDayTravelTimeSec;	// 1天允许的累计驾驶时间
	uint32	ParkingTimeSec;		// 允许的最长停车时间
	uint16	PreAlarmTime;			// 预警时间
	uchar		EnableCheckFatigue;	// 使能检测疲劳驾驶
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

// 驾驶人身份记录 (驾驶员登进登出状态）
#define		LEN_DRIVER_IN_OUT_STATUS		25
typedef		struct
{
	uchar Time[6];  // 事件发生时间
	uchar ID[18];	 // 机动车驾驶证号码
	uchar	EvenType;	// 事件类型 事件类型：01H：登录，02H：退出，其他预留		
} DRIVER_IN_OUT_STATUS,*pDRIVER_IN_OUT_STATUS;

// 速度状态判定 
#define		SPEED_STATUS_NORMAL					1
#define		SPEED_STATUS_UNNORMAL				2
  
#define		LEN_SPEED_STATUS_JUDGE		8
typedef		struct
{
	uchar	Flag;  // 0: 未判定， 1：已判定
	uchar	Status;	// 1：速度正常，2：速度异常
	uchar LastTime[6];  // 判定发生时间 BCD码
} SPEED_STATUS_JUDGE,*pSPEED_STATUS_JUDGE;

#define		LEN_SPEED_STATUS_LOG		133
typedef		struct
{
	uchar	Status;	// 1：速度正常，2：速度异常	记录仪的速度状态
	uchar StartTime[6];  // 判定发生时间 BCD码
	uchar EndTime[6];  // 判定结束时间 BCD码
	struct 
	{
		uchar RecordSpeed;
		uchar RefSpeed;
	}Speed[60];

} SPEED_STATUS_LOG,*pSPEED_STATUS_LOG;

SPEED_EXT SPEED_STATUS_JUDGE	  SpeedStatusJudge;

SPEED_EXT SPEED_STATUS_LOG SpeedStatusLog;

SPEED_EXT uint16	AlertLogonTime;    // 提示插卡登陆计时

SPEED_EXT	uchar		StopFlag;

#define	DRIVER_NUMBER_MIN			10000			 //  驾驶员自编号最小值，小于这个值，视为无效





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	UART类
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
	
	uchar			SpeedSwMinFlag;			// 每页存2分钟，前一分钟标志0，后一分钟标志为1
	
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

