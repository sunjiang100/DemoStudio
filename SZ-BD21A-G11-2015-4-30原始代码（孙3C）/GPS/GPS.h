/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
	�ļ���: GPS.h
	����������GPS��ض����ͷ�ļ�
	���뻷����Keil for ARM 2.2
	Ŀ��CPU: LPC213x
	���ߣ�	Ф����
	����ʱ�䣺
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
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


#define     __CONSTANT_COORDINATES_DEBUG__			0	  				// ��������(���ڲ���)

#define		USE_BD_UPDATE				0   // 0�� ��ʹ�ñ�����������		>0: ʹ�ñ����������� 



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   �����һЩԤ����
$GPRMC,075105.618,A,2306.8053,N,11320.4763,E,000.00,110.19,140605,,*0B//   
$GPGGA,065715.802,2307.7638,N,11320.4766,E,1,03,20.6,0.0,M,-6.4,M,0.0,0000*5E
*/	 


#define		RECORDER_JOURNEY			200		// �������¼����λ����

#define		GPS_HEX_LENGTH				16			// ��16����16�ֽڴ洢һ����Ϣ�ĳ���
#define		GPS_BCD_LENGTH				32			// ��BCD�� 32�ֽڴ洢һ����Ϣ�ĳ���


#define		TOTAL_ELE_RAIL_NUM		64



//        ��λ�ſ���ǰ���������ܣ�����


/////////// GPS���ݴ洢�Ŀ�ʼ�����źͽ���������
#define		GPS_SAVE_START_SECTOR		
#define		GPS_SAVE_END_SECTOR			


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   ����������������ڵڼ������ź�(�����λ�ã���0��ʼ)
$GPRMC,021657.00,A,2251.01634,N,10816.19148,E,0.017,264.52,120406,,,A*66		// U blox	
$GPRMC,075105.618,A,2306.8053,N,11320.4763,E,000.00,110.19,140605,,*0B			//	Led
$GPRMC,061922.921,A,2251.0488,N,10816.1671,E,2.66,329.71,020806,,*09
*/	
#define		PTIME			0 			//	ʱ��
#define		PVALID		1			// ��Ч����Ч��־
#define		PLAT			2			// γ��
#define		PLATD			3			// �ϱ�γ��־
#define		PLON			4			// ����
#define		PLOND			5			// ��������־  Position lontitude direction
#define		PSPEED		6			// �ٶ�
#define		PDIR			7			// ����
#define		PDATE			8			// ����




// GPS����״̬
#define		GPS_ANT_OK		0
#define		GPS_ANT_OPEN	1
#define		GPS_ANT_SHORT	2


#define		LEN_GPS		80





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   GPS��������
*/	 
#define		LEN_SEND_CFG		16
#pragma		pack(1)
typedef		union
{
	uchar		Buffer[LEN_SEND_CFG];
	struct
	{
	uint32	Times;			// ���ʹ�������������ֵΪ0xffffffff��ζ�Ų��޴���һֱ����	
	uint16	Interval;		// ������ʻʱ�ķ���ʱ����
	uint16	StopInterval;	// ����ֹͣʱ�ķ���ʱ����
	uint16	Distance;		// ���;�����
	uint16	T_Interval;		// ��ʱ����ʱ����
	uint16	T_Time;			// ��ʱ���͵ĳ���ʱ��
	uchar		Type;				// �������ͣ��߼��水ʱ�䣬�߼��ٰ�����
	uchar		DelayAlarmTime;// ����
	}Str;

}GPS_SEND,*pGPS_SEND;



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   GPS�ȷ���
*/	 
#pragma		pack(1)
typedef		struct
{
	uint32	D;	 	// ��
	uint32	M;		// ��
	uint32	S;		// �ֵ�С�����֣�X60��Ϊ�� 
	int32	MMM;	// ȫ����Ϊ��(�洢)(FFF.FFFF���Ŵ�10^4�����з��ţ���С��), ����*60 + ��(FF.FFFF) 
	double	dbD;	// ʵ����ʽ��113.232450
}	GPS_DMS,*pGPS_DMS;



///////// GY��ʽGPS���ݰ� ////////////////////
#define		LEN_GY_GPS_PACK			22
#pragma		pack(1)
typedef		struct	
{
	int32	Longitude;			// 4B�����ȣ��Ŵ���10e7������Ϊ��������Ϊ����	 
	int32	Latitude;			// 4B��γ�ȣ��Ŵ���10e7������Ϊ��γ����Ϊ��γ
	uint32	RtcSecond;			// 4B��ʱ�䣬ͳһΪ�룬2000��1��1��00:00:00Ϊ��0��
	uint16	Altitude;			// 2B�����Σ���λ��1��	
	uint16	Speed;				// 2B���ٶȣ���λ��0.1km/h
	uint16	Azimuth;				// 2B����λ��,��λ��1��	
}	GY_GPS_PACK,*pGY_GPS_PACK;


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   JT808 GRS���ݸ�ʽ
*/
#define		PACK_NUM_PER_PAGE				9
#define		LEN_JT808_GPS_PACK			28
typedef		struct	
{
	uint32	Alarm;
	uint32	Status;
	uint32	Latitude;			// 4B��γ�ȣ��Ŵ���10e7������Ϊ��γ����Ϊ��?
	uint32	Longitude;			// 4B�����ȣ��Ŵ���10e7������Ϊ��������Ϊ����	
	uint16	Altitude;			// 2B�����Σ���λ����	
	uint16	Speed;				// 2B���ٶȣ�0.1km/h
	uint16	Azimuth;				// 2B����λ��,��λ����	
	uchar		Date[3];
	uchar		Time[3];
} JT808_GPS_PACK,*pJT808_GPS_PACK;	


///////// TH BCD��ʽGPS���ݰ� ////////////////////
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




///////// TH BCD��ʽGPS���ݰ� ////////////////////
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
   GB19055 GRS���ݸ�ʽ
*/
#define		LEN_GB19056_GPS_PACK			10
#pragma		pack(1)
typedef		struct	
{
	int32	Lon;			// 4B�����ȣ��ַŴ���10e4������Ϊ��������Ϊ����	����λΪ0.0001��ÿ���أ� 
	int32	Lat;			// 4B��γ�ȣ��ַŴ���10e4������Ϊ��γ����Ϊ��γ	 ����λΪ0.0001��ÿ���أ�
	int16	Altitude;			// 2B�����Σ���λ��1��	  ��Ч��ֵ��ΧΪ-32767�ס�32767��;

} GB19056_GPS_PACK,*pGB19056_GPS_PACK;	








/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   GPS�ȷ���
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
   ����Χ�����ݽṹ��Բ�κ;��Σ�
   ID�������ն���˵��ȡ���ֽڼ��ɡ�����GIS��������ͳһ��������IDȡ4�ֽ����θ��á�
*/
#define		LEN_ELE_FENCE_STRUCT		32
#pragma		pack(1)
typedef		struct
{
	uint32	ID;					  			//Χ��ID����ͬ���͵�Χ��ID���ظ�	
	uint32	Lat1;	     						// ���ϵ�γ��
	uint32	Lon1;								// ���ϵ㾭��
	uint32	Lat2;	     						// ���ϵ�γ�ȣ�Բ��ʱΪ0
	uint32	Lon2;								// ���ϵ㾭�ȣ�Բ��ʱΪ0
	uint32	StartTime;						// 
	uint32	EndTime;							// 	
	union                            	
	{										 		//Χ������
	  uint16  Word;
	  struct
	  {
	    uchar	Time:1;					 	//1����ʱ��
	  	 uchar	SpeedLimit:1;			 	//1����
		 uchar	In_AlarmToDriver:1;	 	//1��Χ����������ʻԱ
	  	 uchar	In_AlarmToCenter:1;	 	//1��Χ������������
		 uchar	Out_AlarmToDriver:1;	 	//1��Χ����������ʻԱ
	  	 uchar	Out_AlarmToCenter:1;	 	//1��Χ������������
		 uchar	Latitude:1;				 	//0��γ��1��γ
		 uchar	Longitude:1;			 	//0������1����
		 uchar	CanOpenDoor:1;			 	//����
		 uchar	Rsv:5;
		 uchar	TurnOffGsm:1;
		 uchar	GetGNSS:1;
	  }Bits;
	}Attr;
	uchar		LimitSpeed;						//����ٶ�
	uchar    ContinueTime;					//���ٿɳ���ʱ��	
} ELE_FENCE_STRUCT,*pELE_FENCE_STRUCT;



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   ·�����ݽṹ�������������ٴ洢1ҳ
   ID�������ն���˵��ȡ���ֽڼ��ɡ�����GIS��������ͳһ��������IDȡ4�ֽ����θ��á�
*/
#define		MAX_ROAD_DOT_NUM				20
#define		LEN_ROAD_DOT_STRUCT			24
#pragma		pack(1)
typedef		struct
{
	uint32	ID;					  			//�յ�ID
	uint32	SecID;				  			//·��ID	
	uint32	Lat;	              			//�յ�γ�� HEX, ��λ��0.0001��
	uint32	Lon;			       			//�յ㾭��
	uint16   MaxDriveTime;				//·����ʻ������ֵ	��λs
	uint16   MinDriveTime;				//·����ʻ���㷧ֵ	��λs	
	uchar  	LimitSpeed;			   	//����ٶ� (ע������ʱΪ˫�ֽڣ�
	uchar    ContinueTime;				//�����������ʱ��		��λs
	uchar    Width;			          	//·��  ��λm
	union
	{										    	//Χ������
	  uchar    Byte;
	  struct
	  {
	    uchar  DriveTime:1;				 	//1��ʻʱ��
	  	 uchar  LimitSpeed:1;			 	//1����
		 uchar  Lat:1;				 			//0��γ��1��γ
		 uchar  Lon:1;				 			//0������1����
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
	uint32	ID;					    		//��·ID
	uint32	StartTime;
   uint32	EndTime;
	uchar		DotNumber;				  		//��·�յ���
	uchar		Rsv;
	union
	{										    //��·����
	  uint16  Word;
	  struct
	  {
	    uchar  Time:1;					 //1����ʱ��
	  	 uchar  Rvs1:1;			       //����
		 uchar  In_AlarmToDriver:1;	 //1��Χ����������ʻԱ
	  	 uchar  In_AlarmToCenter:1;	 //1��Χ������������
		 uchar  Out_AlarmToDriver:1;	 //1��Χ����������ʻԱ
	  	 uchar  Out_AlarmToCenter:1;	 //1��Χ������������
		 uchar  Rvs6:1;				    //����
		 uchar  Rvs7:1;				    //����
		 uchar  Rvs8:8;					 //����
	  }Bits;
	}Attr;   
   ROAD_DOT_STRUCT	Dot[MAX_ROAD_DOT_NUM];
} ROAD_LINE_STRUCT,*pROAD_LINE_STRUCT;



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   ��������ݽṹ Polygon
   ID�������ն���˵��ȡ���ֽڼ��ɡ�����GIS��������ͳһ��������IDȡ4�ֽ����θ��á�
*/
#define		MAX_POLY_DOT_NUMBER			29
#define		LEN_POLYGON_STRUCT			256
#pragma		pack(1)
typedef		struct
{
	uint32	ID;					    		//�����״ID
	uint32	StartTime;
   uint32	EndTime;
   uint16	LimitSpeed;						// 
   uchar		ContinueTime;					// 
	uchar		DotNumber;				  		//������
	union                            	
	{										 		//Χ������
	  uint16  Word;
	  struct
	  {
	    uchar	Time:1;					 	//1����ʱ��
	  	 uchar	SpeedLimit:1;			 	//1����
		 uchar	In_AlarmToDriver:1;	 	//1��Χ����������ʻԱ
	  	 uchar	In_AlarmToCenter:1;	 	//1��Χ������������
		 uchar	Out_AlarmToDriver:1;	 	//1��Χ����������ʻԱ
	  	 uchar	Out_AlarmToCenter:1;	 	//1��Χ������������
		 uchar	Latitude:1;				 	//0��γ��1��γ
		 uchar	Longitude:1;			 	//0������1����
		 uchar	CanOpenDoor:1;			 	//����
		 uchar	Rsv:5;
		 uchar	TurnOffGsm:1;
		 uchar	GetGNSS:1;
	  }Bits;
	}Attr;
   struct
   {
   	uint32	Lat;
   	uint32	Lon;
   }Dot[MAX_POLY_DOT_NUMBER];					// ����Σ������30�����㡣
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
   GPS��  
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
	/////////////////////////////////////////////////////////////////////////////////// GPS������
	uint32		PreLon,PreLat,PreSecond;
	GPS_DMS			LonDms,LatDms;							// �ȡ��֡��루�ֵ�С����
	signed char		LonDir,LatDir;							// �����ϱ���������������������γ������γ��
	RTC_STR			gRtc,gtlRtc;	 						// ����ʱ��	 gtl:  GPS to Local
	
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

