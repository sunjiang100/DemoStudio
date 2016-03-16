/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: GPS.h
	功能描述：GPS相关定义的头文件
	编译环境：Keil for ARM 2.2
	目标CPU: LPC213x
	作者：	肖建辉
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  

#ifndef		_GPS_H_
#define		_GPS_H_

#include		"..\Config\RTOS.h"
#include		"..\RTC\RTC.h"	 


#ifdef      GPS_GLOBALS
#define     GPS_EXT
#else
#define     GPS_EXT    extern
#endif


#define     __CONSTANT_COORDINATES_DEBUG__			0	  				// 常量坐标(用于测试)

#define		USE_BD_UPDATE				0   // 0： 不使用北斗升级代码		>0: 使用北斗升级代码 



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   所需的一些预定义
$GPRMC,075105.618,A,2306.8053,N,11320.4763,E,000.00,110.19,140605,,*0B//   
$GPGGA,065715.802,2307.7638,N,11320.4766,E,1,03,20.6,0.0,M,-6.4,M,0.0,0000*5E
*/	 


#define		RECORDER_JOURNEY			200		// 按距离记录，单位：米

#define		GPS_HEX_LENGTH				16			// 以16进制16字节存储一条信息的长度
#define		GPS_BCD_LENGTH				32			// 以BCD码 32字节存储一条信息的长度


#define		TOTAL_ELE_RAIL_NUM		64



//        复位脚控制前端有三极管，反向


/////////// GPS数据存储的开始扇区号和结束扇区号
#define		GPS_SAVE_START_SECTOR		
#define		GPS_SAVE_END_SECTOR			


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   定义各个参数紧挨在第几个逗号后(是相对位置，从0开始)
$GPRMC,021657.00,A,2251.01634,N,10816.19148,E,0.017,264.52,120406,,,A*66		// U blox	
$GPRMC,075105.618,A,2306.8053,N,11320.4763,E,000.00,110.19,140605,,*0B			//	Led
$GPRMC,061922.921,A,2251.0488,N,10816.1671,E,2.66,329.71,020806,,*09
*/	
#define		PTIME			0 			//	时间
#define		PVALID		1			// 有效或无效标志
#define		PLAT			2			// 纬度
#define		PLATD			3			// 南北纬标志
#define		PLON			4			// 经度
#define		PLOND			5			// 东西经标志  Position lontitude direction
#define		PSPEED		6			// 速度
#define		PDIR			7			// 方向
#define		PDATE			8			// 日期




// GPS天线状态
#define		GPS_ANT_OK		0
#define		GPS_ANT_OPEN	1
#define		GPS_ANT_SHORT	2


#define		LEN_GPS		80





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   GPS发送类型
*/	 
#define		LEN_SEND_CFG		16
#pragma		pack(1)
typedef		union
{
	uchar		Buffer[LEN_SEND_CFG];
	struct
	{
	uint32	Times;			// 发送次数计数器，初值为0xffffffff意味着不限次数一直发送	
	uint16	Interval;		// 车辆行驶时的发送时间间隔
	uint16	StopInterval;	// 车辆停止时的发送时间间隔
	uint16	Distance;		// 发送距离间隔
	uint16	T_Interval;		// 临时发送时间间隔
	uint16	T_Time;			// 临时发送的持续时长
	uchar		Type;				// 发送类型，逻辑真按时间，逻辑假按距离
	uchar		DelayAlarmTime;// 保留
	}Str;

}GPS_SEND,*pGPS_SEND;



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   GPS度分秒
*/	 
#pragma		pack(1)
typedef		struct
{
	uint32	D;	 	// 度
	uint32	M;		// 分
	uint32	S;		// 分的小数部分，X60即为秒 
	int32	MMM;	// 全部化为分(存储)(FFF.FFFF，放大10^4倍，有符号，无小数), 即度*60 + 分(FF.FFFF) 
	double	dbD;	// 实数格式：113.232450
}	GPS_DMS,*pGPS_DMS;



///////// GY格式GPS数据包 ////////////////////
#define		LEN_GY_GPS_PACK			22
#pragma		pack(1)
typedef		struct	
{
	int32	Longitude;			// 4B，经度，放大了10e7倍，正为东经，负为西经	 
	int32	Latitude;			// 4B，纬度，放大了10e7倍，正为北纬，负为南纬
	uint32	RtcSecond;			// 4B，时间，统一为秒，2000年1月1日00:00:00为第0秒
	uint16	Altitude;			// 2B，海拔，单位：1米	
	uint16	Speed;				// 2B，速度，单位：0.1km/h
	uint16	Azimuth;				// 2B，方位角,单位：1度	
}	GY_GPS_PACK,*pGY_GPS_PACK;


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   JT808 GRS数据格式
*/
#define		PACK_NUM_PER_PAGE				9
#define		LEN_JT808_GPS_PACK			28
typedef		struct	
{
	uint32	Alarm;
	uint32	Status;
	uint32	Latitude;			// 4B，纬度，放大了10e7倍，正为北纬，负为南?
	uint32	Longitude;			// 4B，经度，放大了10e7倍，正为东经，负为西经	
	uint16	Altitude;			// 2B，海拔，单位：米	
	uint16	Speed;				// 2B，速度，0.1km/h
	uint16	Azimuth;				// 2B，方位角,单位：度	
	uchar		Date[3];
	uchar		Time[3];
} JT808_GPS_PACK,*pJT808_GPS_PACK;	


///////// TH BCD格式GPS数据包 ////////////////////
#define		LEN_TH_BCD_GPS_PACK			32
#pragma		pack(1)
typedef		union	
{
	uchar		Buffer[32];
	struct
	{
		uchar		Head;
		uchar		ID[5];
		uchar		Time[3];
		uchar		Date[3];
		uchar		Lat[4];
		uchar		Rsv_10H;
		uchar		Lon[5];		
		uchar		SpeedDir[3];
		uchar		StatusFlag[4];
		uchar		AlarmFlag;
		uchar		Rsv_1EH;
		uchar		Index;
	}Str;
}TH_BCD_GPS_PACK,*pTH_BCD_GY_GPS_PACK;




///////// TH BCD格式GPS数据包 ////////////////////
#define		LEN_LCM_BCD_GPS_PACK			19
#pragma		pack(1)
typedef		union	
{
	uchar		Buffer[LEN_LCM_BCD_GPS_PACK];
	struct
	{		
		uchar		Date[3];
		uchar		Time[3];
		uchar		Lat[4];
		uchar		Lon[4];
		uchar		Speed[2];
		uchar		Dir[2];
		uchar		GpsStatus;
	}Str;
}LCM_BCD_GPS_PACK,*pLCM_BCD_GPS_PACK;


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   GB19055 GRS数据格式
*/
#define		LEN_GB19056_GPS_PACK			10
#pragma		pack(1)
typedef		struct	
{
	int32	Lon;			// 4B，经度，分放大了10e4倍，正为东经，负为西经	（单位为0.0001分每比特） 
	int32	Lat;			// 4B，纬度，分放大了10e4倍，正为北纬，负为南纬	 （单位为0.0001分每比特）
	int16	Altitude;			// 2B，海拔，单位：1米	  有效数值范围为-32767米～32767米;

} GB19056_GPS_PACK,*pGB19056_GPS_PACK;	








/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   GPS度分秒
*/
#define		LEN_SP_GPS_STRUCT		60
typedef		struct	
{
	char		Date[6+1];
	char		Time[6+1];
	char		LatDir;
	char		Lat[9+1];
	char		LonDir;
	char		Lon[10+1];
	char		SpeedDir[6+1];
	char		Speed[3+1];
	char		Azimuth[3+1];
	char		Altitude[8];
	char		End;	
}GPS_STRING_PARA,*pGPS_STRING_PARA;





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   电子围栏数据结构（圆形和矩形）
   ID：对于终端来说，取单字节即可。对于GIS管理中心统一部署，所有ID取4字节整形更好。
*/
#define		LEN_ELE_FENCE_STRUCT		32
#pragma		pack(1)
typedef		struct
{
	uint32	ID;					  			//围栏ID。不同类型的围栏ID可重复	
	uint32	Lat1;	     						// 左上点纬度
	uint32	Lon1;								// 左上点经度
	uint32	Lat2;	     						// 左上点纬度，圆形时为0
	uint32	Lon2;								// 左上点经度，圆形时为0
	uint32	StartTime;						// 
	uint32	EndTime;							// 	
	union                            	
	{										 		//围栏属性
	  uint16  Word;
	  struct
	  {
	    uchar	Time:1;					 	//1根据时间
	  	 uchar	SpeedLimit:1;			 	//1限速
		 uchar	In_AlarmToDriver:1;	 	//1进围栏报警给驾驶员
	  	 uchar	In_AlarmToCenter:1;	 	//1进围栏报警给中心
		 uchar	Out_AlarmToDriver:1;	 	//1出围栏报警给驾驶员
	  	 uchar	Out_AlarmToCenter:1;	 	//1出围栏报警给中心
		 uchar	Latitude:1;				 	//0北纬，1南纬
		 uchar	Longitude:1;			 	//0东经，1西经
		 uchar	CanOpenDoor:1;			 	//保留
		 uchar	Rsv:5;
		 uchar	TurnOffGsm:1;
		 uchar	GetGNSS:1;
	  }Bits;
	}Attr;
	uchar		LimitSpeed;						//最高速度
	uchar    ContinueTime;					//超速可持续时间	
} ELE_FENCE_STRUCT,*pELE_FENCE_STRUCT;



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   路线数据结构，不定长，最少存储1页
   ID：对于终端来说，取单字节即可。对于GIS管理中心统一部署，所有ID取4字节整形更好。
*/
#define		MAX_ROAD_DOT_NUM				20
#define		LEN_ROAD_DOT_STRUCT			24
#pragma		pack(1)
typedef		struct
{
	uint32	ID;					  			//拐点ID
	uint32	SecID;				  			//路段ID	
	uint32	Lat;	              			//拐点纬度 HEX, 单位：0.0001分
	uint32	Lon;			       			//拐点经度
	uint16   MaxDriveTime;				//路段行驶过长阀值	单位s
	uint16   MinDriveTime;				//路段行驶不足阀值	单位s	
	uchar  	LimitSpeed;			   	//最高速度 (注：设置时为双字节）
	uchar    ContinueTime;				//超速允许持续时间		单位s
	uchar    Width;			          	//路宽  单位m
	union
	{										    	//围栏属性
	  uchar    Byte;
	  struct
	  {
	    uchar  DriveTime:1;				 	//1行驶时间
	  	 uchar  LimitSpeed:1;			 	//1限速
		 uchar  Lat:1;				 			//0北纬，1南纬
		 uchar  Lon:1;				 			//0东经，1西经
		 uchar  Rvs4:1;
		 uchar  Rvs5:1;
		 uchar  Rvs6:1;
		 uchar  Rvs7:1;
	  }Bits;
	}Attr;	
} ROAD_DOT_STRUCT,*pROAD_DOT_STRUCT;


#define		LEN_ROAD_LINE_STRUCT		16
#pragma		pack(1)
typedef		struct
{
	uint32	ID;					    		//线路ID
	uint32	StartTime;
   uint32	EndTime;
	uchar		DotNumber;				  		//线路拐点数
	uchar		Rsv;
	union
	{										    //线路属性
	  uint16  Word;
	  struct
	  {
	    uchar  Time:1;					 //1根据时间
	  	 uchar  Rvs1:1;			       //保留
		 uchar  In_AlarmToDriver:1;	 //1进围栏报警给驾驶员
	  	 uchar  In_AlarmToCenter:1;	 //1进围栏报警给中心
		 uchar  Out_AlarmToDriver:1;	 //1出围栏报警给驾驶员
	  	 uchar  Out_AlarmToCenter:1;	 //1出围栏报警给中心
		 uchar  Rvs6:1;				    //保留
		 uchar  Rvs7:1;				    //保留
		 uchar  Rvs8:8;					 //保留
	  }Bits;
	}Attr;   
   ROAD_DOT_STRUCT	Dot[MAX_ROAD_DOT_NUM];
} ROAD_LINE_STRUCT,*pROAD_LINE_STRUCT;



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   多边形数据结构 Polygon
   ID：对于终端来说，取单字节即可。对于GIS管理中心统一部署，所有ID取4字节整形更好。
*/
#define		MAX_POLY_DOT_NUMBER			29
#define		LEN_POLYGON_STRUCT			256
#pragma		pack(1)
typedef		struct
{
	uint32	ID;					    		//多边形状ID
	uint32	StartTime;
   uint32	EndTime;
   uint16	LimitSpeed;						// 
   uchar		ContinueTime;					// 
	uchar		DotNumber;				  		//顶点数
	union                            	
	{										 		//围栏属性
	  uint16  Word;
	  struct
	  {
	    uchar	Time:1;					 	//1根据时间
	  	 uchar	SpeedLimit:1;			 	//1限速
		 uchar	In_AlarmToDriver:1;	 	//1进围栏报警给驾驶员
	  	 uchar	In_AlarmToCenter:1;	 	//1进围栏报警给中心
		 uchar	Out_AlarmToDriver:1;	 	//1出围栏报警给驾驶员
	  	 uchar	Out_AlarmToCenter:1;	 	//1出围栏报警给中心
		 uchar	Latitude:1;				 	//0北纬，1南纬
		 uchar	Longitude:1;			 	//0东经，1西经
		 uchar	CanOpenDoor:1;			 	//保留
		 uchar	Rsv:5;
		 uchar	TurnOffGsm:1;
		 uchar	GetGNSS:1;
	  }Bits;
	}Attr;
   struct
   {
   	uint32	Lat;
   	uint32	Lon;
   }Dot[MAX_POLY_DOT_NUMBER];					// 多边形，最多少30个顶点。
} POLYGON_STRUCT,*pPOLYGON_STRUCT;


/*
*XY,9080010001,V1,054103,A,2813.64812,N,11254.16622,E,015.234,149.123,041209,FFFFF9FF#<0,0,25,0,0,0,10>
*GY,1108100001,V1,052525,A,2308.3527,N,11322.0975,E,0,0,260211,FEFFFBFF#<
*/


#define		LEN_GPS_CHAR_PACK			160
#define		LEN_TH_GPS_PACK			160
#define		LEN_GPS_STRING				160
#define		LEN_GPS_ANT_STRING		100

#define		GPRS_PACK_FLAG				0x55aa7ffe



#pragma		pack(1)

#define		LEN_SAVE_DATA			(256+4)
	


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   GPS类  
*/
class			GPSCls
{
public:
	GPSCls(void);
	void			Init(uchar);	
	void       		eTask(void);
	//void			GetSendConfig(GPS_SEND *pSend);
	//void		 	SetSendConfig(GPS_SEND *pSend,uchar Save);
//	void			GetGPS_Pack(uchar *pGY,char *pCM,uchar *pZL);
	uint32			GPRMC_LonLatToDu32(int32 Flag,char *ptGPS, GPS_DMS *ptDms);	
	void			GY_MMM_ToRMC(uint32 MMM,uchar *pRmc);	
	void			TestGPS(void);	
	uchar			DataCompleteAnalys(char	*pGPS);
	uchar			GetSpeed();
	uint16			GetSpeedX100();
	uint32			GetMileage(void);
	void			SetGPSPower(uchar on);
	void			CheckGpsAntAsLine();	
	void			SaveBlindData(uchar *pData);
	void			SaveHistoryData(uchar *pData);
	void			SecondHoppingProcess(uchar Blind);	
	//void			PowerOff(void);
	void			RtcPlusPlus(void);
	//void			ExtData(char *p);
	uint16		Get1MinAvgSpeed(void);
	void			GB19056_DataPack(uchar *pDst);
	void			GY_DataPack(uchar *pDst); 
	void			TH_DataPack(char *pDst);
	//void			TH_BCD_Pack(uchar	*pDst);
	//void			LCM_BCD_Pack(uchar	*pDst);
	void			JT808_DataPack(uchar *pDst);
	uint32			Calculate2DotSphericalDistance(
					double	Lon1,double Lat1,double	Lon2,double Lat2);					
	uint32		Calculate2DotDistance(int32 Lon1,int32 Lat1,int32 Lon2,int32 Lat2);
	void			CheckRoundEleFence(int32 iLon,int32 iLat);
	void			CheckRoadLineFence(int32 iLon,int32 iLat);
	void			InvalidToValid(char *pGPS);
	void			SearchRoadSection(uchar Cur,int32 iLon,int32 iLat);
	uchar 		CheckIsOutRoad(uint32 LonS, uint32 LatS, uint32 LonE, uint32 LatE,uint32 LonQ, uint32 LatQ, uchar RoadWidth);
	
	void			GNSSUpdateProccess(void);
	uchar       QueryGNSSVersion(char *pVer);
	void			LocationInfoPushAlignment(void);
public:	
	//uchar			volatile	GPS_PowerOn;
	uchar			volatile	ClearAlarmFlag;
//	uchar			volatile	GpsSaveFlag;
//private:	
	char			GPS_String[LEN_GPS_STRING];
	char			GPS_AntString[LEN_GPS_ANT_STRING];
	
	char			TH_GPS_Pack[LEN_TH_GPS_PACK];	
	char			GPS_SatelStatus[LEN_GPS_STRING];	
	uchar			GPS_SatelNum,GPS_Working,BD_SatelNum;
	/////////////////////////////////////////////////////////////////////////////////// GPS各参数
	uint32		PreLon,PreLat,PreSecond;
	GPS_DMS			LonDms,LatDms;							// 度、分、秒（分的小数）
	signed char		LonDir,LatDir;							// 东西南北，东经正，西经负，北纬正，南纬负
	RTC_STR			gRtc,gtlRtc;	 						// 日期时间	 gtl:  GPS to Local
	
	ELE_FENCE_STRUCT			TestOneRectFence;
	//uchar				volatile	GPS_AntStatus,GPS_PreAntStatus;
	uint32			GPS_InvalidCnt,GPSValidCnt;
	uint32			TaskTickCnt;
	//char			CMPack[LEN_CM_GPS_PACK];
	
	char			GPS_CharPack[LEN_GPS_CHAR_PACK],Last_GPS_CharPack[LEN_GPS_CHAR_PACK]; 
	
	GPS_SEND					Send;	
	GY_GPS_PACK				GYPack,LastGYPack;
	TH_BCD_GPS_PACK		THPack;
	LCM_BCD_GPS_PACK		LCMPack;
	GB19056_GPS_PACK		GBPack,LastGBPack;
	JT808_GPS_PACK			JTPack, LastJTPack;
	
	GPS_STRING_PARA		spGPS;
	uchar			NoUse2;
	uchar			DataUpDate;	
	double		dbPreLon,dbPreLat;
	
	uint16		NowEleRailNum;
	uint16		InOutEleRailTime;
	uint32		AntShortTimeCnt,GPS_Distance;
	
	uint16      InOutRoundEleRailTime;	
	uchar			GPS_PowerOn;	
	uchar			DriftingFlag;
	uchar       BD_UpDateFlag;
	
private:	
	void			GPS_AntCheck(char *pAnt);	
	uchar			DataPackErrorCheck(void);
	void			GPS_LocalTimeString(void);
	//void			LocationInfoPushAlignment(void);
	uchar			BCD_PackBusing,GY_PackBusing,TH_PackBusing,LCM_PackBusing,GB_PackBusing,JT_PackBusing;
	//#endif
	uint32		GetSatelliteStatus(char *pSatel);	// 

	void			CheckRectangleFence(int32 Lon,int32 Lat);
	uchar 		ChekTimeRand(uint32 St,uint32 Et);
	
	void			PrintRoadLineLonLat(uint32,uint32,uint32,uint32,uint32,uint32,uchar);
	
	//uint32		LastSendTime,LastSendMileage,LastRegServerTime,LastSaveTime,LastSendTimeB,LastSendTimeH;
	uint32		VirtaulGPS_Second,SumOfSpeed1Min,AvgSpeed1Min;
	
	uchar			PreEleRailNo;
	uint32		DataErrorCnt;
	uchar			PS[32];
	uint32		GPRS_PackFlag;	
	uint32		PackCnt;
	uchar    	GpsSaveCnt,GpsSaveBuffer[LEN_SAVE_DATA];
 	//uchar    	GpsReadSaveBuffer[LEN_SAVE_DATA];
	uchar    	GpsBlindCnt,GpsBlindBuffer[LEN_SAVE_DATA];
	
	
};

GPS_EXT	__noinit__		GPSCls		GPS;	// 

#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */

