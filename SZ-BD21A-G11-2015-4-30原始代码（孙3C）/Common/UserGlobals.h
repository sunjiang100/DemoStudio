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
#define 	__noinit__ 		__attribute__((zero_init))//变量不初始化为0
#endif

#pragma		pack(1)
USER_EXT    __noinit__  unsigned int SystemResetFlag;

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   测试数据发送到串口0
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

#define		tMAIN_RECORD_START					((0x55<<8)+'S')			// 开始录音
#define		tMAIN_RECORD_STOP					((0x55<<8)+'p')			// 停止录音
#define		tMAIN_RECORD_INIT					((0x55<<8)+'i')			// 初始化 录音控制
#define		tMAIN_RECORD_UPLOAD					((0x55<<8)+'u')			// 播放录音
#define		tMAIN_RECORD_PLAY					((0x55<<8)+'y')			// 播放录音

#define		tGPS_ORIGINAL_DATA				((0x55<<8)+'0')			// 原始数据
#define		tGPS_ROUND_FENCE					((0x55<<8)+'1')			// 电子围栏相关
#define		tGPS_RECT_FENCE					((0x55<<8)+'2')			// 电子围栏相关
#define		tGPS_POLYGON_FENCE				((0x55<<8)+'3')			// 电子围栏相关
#define		tGPS_ROAD_LINE_FENCE				((0x55<<8)+'4')			// 电子围栏相关
#define		tGPS_CHAR_PACK						((0x55<<8)+'5')			// GPS字符串数据包
#define		tGPS_HEX_PACK						((0x55<<8)+'6')			// GPS十六进制数据包
#define		tGPS_2D_DISTANCE					((0x55<<8)+'7')			// GPS两点距离
#define		tGPS_MMM								((0x55<<8)+'8')			// MMM格式输出，16进制
#define		tGPS_ELE_FENCE_HEX_DATA			((0x55<<8)+'9')			// 电子围栏16进制数据(MMM)
#define		tGPS_ALL_DATA						((0x55<<8)+'A')			// GPS模块输出的所有数据
#define		tGPS_GET_DATA_INTERVAL			((0x55<<8)+'B')			// 
#define		tGPS_REC_OK_EVT_FLAG				((0x55<<8)+'C')			// 
#define		tGPS_PACK_FLAG						((0x55<<8)+'D')			// 打包标志
#define		tGPS_SET_ELE_RAIL					((0x55<<8)+'E')			// 设置电子围栏
#define		tGPS_FORCE_DATA_PACK				((0x55<<8)+'F')			// 
#define		tGPS_ORIGINAL_DATA_HEX			((0x55<<8)+'G')			// 用PutBuffer
#define		tGPS_TASK_RUN						((0x55<<8)+'H')			// 
#define		tGPS_ANT_STATUS					((0x55<<8)+'I')			// 
#define		tGPS_CM_PACK						((0x55<<8)+'J')			// 
#define		tGPS_CONST_DATA					((0x55<<8)+'K')			//
#define		tGPS_MESSAGE_SAVE 				((0x55<<8)+'L')			//	GPS信息存储测试
#define		tGPS_BLIND_MESSAGE_SAVE 		((0x55<<8)+'M')			//	GPS盲区补报
#define		tGPS_ANT_OPEN 						((0x55<<8)+'N')			//	GPS天线开路
#define		tGPS_TEMP_PACK						((0x55<<8)+'O')			// 临时打包
#define		tGPS_TEST							((0x55<<8)+'P')			//
#define		tGPS_HISTORY_DATA					((0x55<<8)+'Q')			//
#define		tGPS_TIME_TO_PACK					((0x55<<8)+'R')			//
#define		tGPS_EXT_PACK						((0x55<<8)+'S')			//	扩展包
#define		tGPS_USE_GPS						((0x55<<8)+'T')			//	
#define		tGPS_USE_BD							((0x55<<8)+'U')			//	
#define		tGPS_USE_GPS_BD					((0x55<<8)+'V')			//	
#define		tGPS_COOL_START					((0x55<<8)+'W')			//	
#define		tGPS_VIRTUAL_GPS_VALID			((0x55<<8)+'X')			//	
#define		tGPS_UPDATE							((0x55<<8)+'u')			// 北斗升级

///////////////////////////////////////
#define		tGPRS_AT_ACK_CHECK				((0x55<<8)+'0')			// 
#define		tGPRS_DIAL_STATUS					((0x55<<8)+'1')			// 
#define		tGPRS_SEND_DATA_INFO				((0x55<<8)+'2')			// 发送数据信息
#define		tGPRS_GET_CMD_DATA				((0x55<<8)+'3')			// 
#define		tGPRS_MOBILE_INIT					((0x55<<8)+'4')			// 
#define		tGPRS_SAVE_DATA_CNT				((0x55<<8)+'5')			// 
#define		tGPRS_GSM_SMS						((0x55<<8)+'6')			// SMS
#define		tTEST_SEND_SMS1					((0x55<<8)+'8')			// 测试短信1
#define		tTEST_SEND_SMS2					((0x55<<8)+'9')			// 测试短信2
#define		tGPRS_GSM_ALL_DATA				((0x55<<8)+'A')			// 模块返回的全部数据
#define		tGSM_PHONE_STATUS					((0x55<<8)+'B')		  // 临时测试
#define		tGPRS_DOWNLOAD_CMD_TEST			((0x55<<8)+'C')
#define		tGSM_DIAL_NUMBER					((0x55<<8)+'D')			// 拨号
#define		tAT_CMD_ERROR						((0x55<<8)+'E')
#define		tGPRS_FIFO_DATA					((0x55<<8)+'F')			// 
#define		tGPRS_GETFILE    				   ((0x55<<8)+'G')			// 文件读取
#define		tGPRS_U0_GPRS						((0x55<<8)+'H')
#define		tGSM_UPGRADE_DATA1				((0x55<<8)+'I')			//	远程升级数据测试
#define		tTEST_SMS_CMD						((0x55<<8)+'M')			// 短信命令测试 
#define		tGPRS_NO_LINK						((0x55<<8)+'N')
#define		tGPRS_SEND_OK						((0x55<<8)+'O')			// 
#define		tGPRS_PACK_DATA					((0x55<<8)+'P')			// 打包数据
#define		tGSM_REC_SMS						((0x55<<8)+'S')			// 接收短信处理
#define		tGSM_TASK_RUN						((0x55<<8)+'T')			// 任务运行状态
#define		tGSM_UPGRADE_DATA					((0x55<<8)+'U')			//	远程升级数据测试
#define		tGPRS_CONST_SIGNAL				((0x55<<8)+'Z')			// 
#define		tGPRS_UPDATE						((0x55<<8)+'u')

#define		tGPRS_ALL_PARAMETER				((0x55<<8)+'a')			// 全部参数


#define		tGPRS_TMP_TEST						((0x55<<8)+'t')		  // 临时测试



///////////////////////////////////////
#define		tGPRS_AUTO_SEND					((0x55<<8)+'0')			// 
///////////////////////////////////////
#define		tSW_32BIT_TO_UART0				((0x55<<8)+'0')			// 读取开关量输入
#define		tSW_OUT_PUT_HIGH					((0x55<<8)+'1')			// 输出控制为高
#define		tSW_OUT_PUT_LOW					((0x55<<8)+'2')			// 输出控制为低
#define		tSW_MOTO_PULS_CNT					((0x55<<8)+'3')			// 发动机转速计数器
#define		tSW_TEST_DELAY						((0x55<<8)+'T')			// 测试继电器
#define		tSW_PULS_ENABLE					((0x55<<8)+'P')			// 脉冲输出
#define		tCONTROL_OUTPUT					((0x55<<8)+'C')			// 控制输出相关
#define		tSW_RADIO_OUT						((0x55<<8)+'R')			// 无线遥控
#define		tSW_CHECK_ALARM					((0x55<<8)+'A')			// 报警检测测试
#define		tSW_LOCK_DOOR_SIGNAL				((0x55<<8)+'L')			// 锁门信号
//#define		tHC595_OUT1							((0x55<<8)+'5')			// 
//#define		tHC595_OUT2							((0x55<<8)+'6')			// 
#define		tSPEED_DRIVE_PARA					((0x55<<8)+'S')			// 
#define		tSPEED_OVER_SPEED_PARA			((0x55<<8)+'O')			// 
#define		tSPEED_SIMU_SPEED					((0x55<<8)+'M')			// 模拟速度测试
#define		tSPEED_RECORD						((0x55<<8)+'R')			// 记录仪采集

///////////////////////////////////////
#define		tADC_CH0								((0x55<<8)+'0')			// ADC0结果输出显示
#define		tADC_CH1								((0x55<<8)+'1')			// ADC0结果输出显示
#define		tADC_CH2								((0x55<<8)+'2')			// ADC0结果输出显示
#define		tADC_CH3								((0x55<<8)+'3')			// ADC0结果输出显示
#define		tADC_ALL								((0x55<<8)+'A')			// 所有通道
#define		tADC_HEX								((0x55<<8)+'H')			// Mega16全部模拟量
#define		tADC_MAIN_POWER					((0x55<<8)+'M')			// Mega16全部模拟量
#define		tADC_TEST							((0x55<<8)+'T')			// ADC测试
#define		tADC_KEY_BOARD						((0x55<<8)+'K')			// AD键盘
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

#define		tDISP_OLD_RECODE				   ((0x55<<8)+'N')		  //输出原始记录数据
#define		tDISP_NEW_RECODE		    	   ((0x55<<8)+'M')		  //输出最新产生的记录数据
#define		tDISP_USB_RECODE		    	   ((0x55<<8)+'U')		  //输出USB采集数据
#define		tDISP_TEST_FOR_EXT				((0x55<<8)+'E')			// 外部显示

///////////////////////////////////////
#define		tMAIN_TEST							((0x55<<8)+'0')			

///////////////////////////////////////
#define		tCAMERA_TAKE_PHOTO				((0x55<<8)+'1')
#define		tCAMERA_READ_DATA					((0x55<<8)+'2')
#define		tCAMERA_RUN_TEST					((0x55<<8)+'3')			// 测试摄像头任务是否在运行
#define		tCAMERA_INIT						((0x55<<8)+'I')			// 测试摄像头任务是否在运行
#define		tCAMERA_ALL_DATA					((0x55<<8)+'A')			// 测试摄像头任务是否在运行
#define		tCAMERA_PACK_LEN					((0x55<<8)+'L')		
#define		tCAMERA_V_DATA						((0x55<<8)+'V')  			// 虚拟数据
#define		tCAMERA_CMD_ERROR					((0x55<<8)+'E')  			// 虚拟数据
#define		tCAMERA_CHCEK						((0x55<<8)+'C')			// 检查摄像头

#define		tHANDLE_DISPLAY_TEST				((0x55<<8)+'H')			// 手柄、显示屏测试
#define		tTIRED_TEST							((0x55<<8)+'T')			// 疲劳驾驶测试

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
   JT808,车辆状态基本数据结构。
   采集时1有效，0无效。如果发送时需负逻辑，则需修改。
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
			uchar		CarLoadStatus:2;			 // 00 空车； 01 半载 ； 10 保留 ； 11 满载
			uchar		OffOil:1;		  	// 
			uchar		OffE:1;				// 
			uchar		LockDoor:1;			// 
			uchar 	Door1:1;				// 1：门开，0： 门关， （下面的门类同）前门
			uchar		Door2:1;				// 中门
			uchar		Door3:1;      			// 后门
		} Bits;
	} VS2;	 
	////////////////////////////////
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		Door4:1;	// 驾驶门	
			uchar		Door5:1;	// 自定义
			uchar		UseGPS:1;	// 0：未使用GPS定位	 1: 使用GPS定位
			uchar		UseBD:1;	// 0：未使用北斗定位	 1: 使用北斗定位
			uchar		UseGLONASS:1;	// 0：未使用GLONASS定位	 1: 使用GLONASS定位
			uchar		UseGalileo:1;	// 0：未使用Galileo定位	 1: 使用Galileo定位
			uchar		RTurnLight:1;	//右转向灯
			uchar		HighLight:1;	//远光灯 
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
	/////////////////////////////////////////////////////// 以下为报警标志位
	union
	{			
		uchar		Byte;					
		struct	
		{
			uchar		EmergencyAlarm:1;		//
			uchar		OverSpeedAlarm:1;		//
			uchar		FatigueDriving:1;		//
			uchar		Warning:1;		//
			uchar		ANT_Error:1;		//	GNSS模块发生故障
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
			uchar		IC_Card_ModelError:1;		// 道路运输证IC卡
			uchar		OverSpeedPreAlarm:1;			// 超速预警
			uchar		FatiguePreAlarm:1;			//  疲劳驾驶预警
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
			uchar		CrashPreAlarm:1;		//	碰撞预警
			uchar		TiltPreAlarm:1;			//	侧翻预警
			uchar		IllegalOpenDoor:1;		//	非法开门报警（终端未设置区域时，不判断非法开门）
		} Bits;
	} VS8;			


} JT808_STRU_VSDR,*pJT808_STRU_VSDR;					// Vehicle Status Data Recorder				
			

USER_EXT		volatile		JT808_STRU_VSDR		JT808Flag;


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   JT808  扩展信号车辆
*/
#pragma		pack(1)
typedef		struct
{	
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		DippedHeadLight:1;				// 	  近光灯信号
			uchar		HighLight:1;						// 	 远光灯信号
			uchar		RTurnLight:1;		  				// 	 右转向灯
			uchar		LTurnLight:1;						// 	 左转向灯
			uchar		Brake:1;		   					// 	 制动信号
			uchar		ReversGear:1;	  		         // 	 倒档信号
			uchar		FogLight:1;		 				   //   雾灯信号
			uchar		ClearanceLamp:1;              //  示廓灯
		} Bits;
	} VS1;	
	////////////////////////////////
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		Speaker:1;			// 喇叭信号	
			uchar		AirCondition:1;	// 空调状态
			uchar		Neutral:1;			// 空档状态
			uchar		Retarder:1;			// 缓冲器工作
			uchar		ABSWork:1;			//  ABS工作
			uchar		Heater:1;			// 加热器工作
			uchar		Clutch:1;			// 离合器状态
			uchar		Rev:1;	    		// 保留 
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
   JT808  IO状态
*/
#pragma		pack(1)
typedef		struct
{	
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		DeepSleepStatus:1;					// 	  深度休眼状态
			uchar		SleepStatus:1;						// 	  休眼状态
			uchar		Rev:6;                            // 保留
			
		} Bits;
	} VS1;	
	////////////////////////////////
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar	Rev:8;                   //  保留
		} Bits;
	} VS2;	
	////////////////////////////////
} JT808_IO_STRU_VSDR,*pJT808_IO_STRU_VSDR;

USER_EXT		volatile		JT808_IO_STRU_VSDR		JT808IOStatusFlag;



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   GB19056-2011 状态位定义
*/
#pragma		pack(1)
typedef		struct
{	
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		CallHelpAlarm:1;		   //	求助报警
			uchar		CollisionAlarm:1;			// 碰撞报警
			uchar		OverSpeedAlarm:1;		  	// 超速报警
			uchar		FTdrivingAlarm:1;		  	// 疲劳驾驶报警
			uchar		OutAreaAlarm:1;			   // 禁止驶出越界报警
			uchar		InAreaAlarm:1;				// 禁止驶入越界报警
			uchar		DeviateRouteAlarm:1;		// 偏离路线报警
			uchar		Rsv:1;      				// 
		} Bits;
	} VS1;	
	////////////////////////////////
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		MainPowerLost:1;			// 电瓶被拆除
			uchar		BakPowerLow:1;				// 备用电池电压低					
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
			uchar		DoorOpen:1;					// 车门开
			uchar		Rsv2:1;						// 
			uchar		Rsv3:1;						// 
			uchar		Rsv4:1;						// 
			uchar		Rsv5:1;						// 
			uchar		Rsv6:1;						// 
			uchar		Brake:1;						// 刹车
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
   使能标志 
*/
typedef		struct
{	
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		ParkOverTimeAlarm:1;					// 允许使用停车超时报警检测
			uchar		SpeedLimitAlarm:1;					// 允许使用车辆超速报警检测
			uchar		StealAlarm:1;							// 允许使用盗警检测
			uchar		RobAlarm:1;								// 允许使用劫警检测         
			uchar		NoSetScanAlarm:1;						// 允许使用未设防报警检测   
			uchar		TrailCarAlarm:1;						// 允许使用拖车报警检测     
			uchar		HelpAlarm:1;							// 允许使用求助报警检测     
			uchar		TireDriveAlarm:1;						// 允许使用疲劳驾驶检测     
		} Bits;
	} EN1;
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		IllegalOpenDoor:1;					// 允许使用非法开门检测        
			uchar		IllegalMakeFire:1;					// 允许使用非法点火检测        
			uchar		CheckShake:1;							// 允许使用震动检测            
			uchar		AutoLockDoor:1;						// 允许使用锁门                
			uchar		LowVoltage:1;							// 允许使用低压检测            
			uchar		RemoteMonitor:1;						// 允许远程监听标志  
			uchar		UseHandleFlag:1;						// 允许使用手柄标志  
			uchar		HandleWatchCar:1;						// 是否允许手柄看车
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
   电子围栏属性数据（电子围栏数据存储在Flash）。圆形、矩形、多边形、路线，共4类，每类最多个数为128
   每类属性存储在铁电的长度为32B
   前16B：总个数、当前所在围栏号、
   后16B：128Bits，每个Bits代表围栏的有效性，0：无效，无需检测该围栏，1：有效，需检测
   铁电存储位置：EE_ELE_FENCE_ATTR_BASE_ADDR
*/
#define		MAX_FENCE_NUM					128
#define		LEN_EE_FENCE_ATTR				64
#pragma		pack(1)
typedef		struct
{	   
	uint32	RemoteID;		// 远程设置围栏的ID号（报警需使用）
	uint32	InTimeCnt;		// 进围栏时间
	uint32	OutTimeCnt;		// 出围栏时间
	uchar		Total;
	uchar		CurrentNo;		// 当前围栏/路线，取值1~128
	uchar		PreNo;			// 上次围栏/路线，取值1~128
	uchar		Width;				// 
		
	uchar		Enable[16];		// 128Bits，使能128个围栏
	
	int32		Lon1;				// 当前围栏/路线经纬度，矩形时，两个对角点坐标，路线时，路段的起点和终点坐标
	int32		Lat1;
	int32		Lon2;
	int32		Lat2;	
	
	uint32	SecNo;			// 路段号
	uint32	PreSecNo;		// 上个路段号
	uint16   MaxDriveTime;	//路段行驶过长阀值	单位s
	uint16   MinDriveTime;	//路段行驶不足阀值	单位s	
	uchar  	LimitSpeed;		//最高速度 (注：设置时为双字节）
	uchar    ContinueTime;	//超速允许持续时间		单位s	
	uchar		RsvB[2];			// 保留
} FENCE_ATTR_STRUCT,*pFENCE_ATTR_STRUCT;
USER_EXT		FENCE_ATTR_STRUCT		RoundFenceAttr,RectFenceAttr,PolygonFenceAttr,RoadAttr;


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   595 输出控制  
*/
/* #pragma		pack(1)
typedef		struct
{	
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		PowerSeldLock:1;			// 主电源自锁控制
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
			uchar		DST1:1;						// 热敏打印机加热控制1
			uchar		DST2:1;						// 
			uchar		DST3:1;		  				// 
			uchar		DST4:1;						// 
			uchar		DST5:1;		   			// 
			uchar		DST6:1;	  					// 
			uchar		PowerMotor:1;				// 热铭打印机步进马达电源控制
			uchar		PowerLCM_BL:1;      		// LCM背光控制
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
////////// 输出控制结构 /////////////////////////
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
	uchar		EnableInterIdleMode;						// 允许进入省电模式
	uchar		IdleModeFlag;								// 省电模式标志
	uint16	InterIdleModeTime;						// 多少时间后进入省电模式
	uint16	WakeUpTime;								   // 多少时间后唤醒
	uint16	WakeUpLongTime;							// 唤醒持续时间
	uint32	SecondCnt;
}	IDLE_STR,*pIDLE_STR;
USER_EXT   volatile		IDLE_STR		Idle;
							
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	系统日志。GSM/GPRS相关计数器定义。非易失性，每分钟存储一次，可通过远程读取
*/	
#define		SYSTME_LOG_GROUP		8
#define		NUMBER_PER_GROUP		8
#define		SYSTEM_LOG_NUMBER		(SYSTME_LOG_GROUP*NUMBER_PER_GROUP)		// 每组8个
#define		LEN_SYSTEM_LOG			(SYSTEM_LOG_NUMBER*4)	// 每个参数4字节
#define		SYSTEM_LOG_EE_PAGE	(LEN_SYSTEM_LOG/EE_PAGE_LEN)
#pragma		pack(4)
typedef		struct
{	
	struct
	{
		uint32	DialCnt;					// GPRS拨号次数计数器
		uint32	DialOK;					// GPRS拨号成功次数						
		uint32	OnLineTime;				// GPRS在线时间，单位：秒
		uint32	RecTotalPack;			// GPRS接收总包数
		uint32	RecTotalBytes;			// GPRS接收总字节数
		uint32	PushTotalBytes;		// GPRS进队总字节数（如进队不等于发送则本地丢包）
		uint32	SendTotalBytes;		// GPRS发送字节计数器
		uint32	SendTotalPack;			// GPRS发送包数计数器		
	}GPRS;
	struct
	{
		uint32	CoolBoot;				// GSM模块冷启动计数器
		uint32	SimErrorTimes;			// SIM卡坏时间计数器（含卡坏、卡松等其它原因导致找不到卡）
		uint32	UnRegTime;				// GSM找不到网络商时间计数器（如中国移动、联通、电信）
		uint32	EbbSignalTime;			// GSM弱信号时间
		uint32	RecSms;					// GSM接收短信计数器
		uint32	SendSms;					// GSM发送短信计数器
		uint32	RegServer;				// 注册服务器次数计数器			  G12 未用
		uint32	RegServerOK;			// 注册服务器成功次数计数器	  G12 未用
	}GSM;	
	struct  
	{
		uint32	CallerTimes;			// 主叫次数				//G12 未用
		uint32	CallerTime;				// 主叫通话时间	   //G12 未用
		uint32	CalledTimes;			// 被叫次数				//G12 未用
		uint32	CalledTime;				// 被叫通话时间		//G12 未用
		uint32	GpsDataSavePosIndex; // 存储链路断开时的位置信息条数	  (可自定义，与存储相关)  //G12 未用
		uint32	GpsDataSendPosIndex;	//	发送链路断开时的位置信息条数	  (可自定义，与存储相关)  //G12 未用
		uint32	GpsDataSaveAlarmIndex;		// 存储含报警位置信息条数	  (可自定义，与存储相关)  //G12 未用
		uint32	GpsDataSendAlarmIndex;	   //	发送含报警位置信息条数    (可自定义，与存储相关)  //G12 未用
	}GG;
	struct
	{	
		unsigned long long  	SpeedCaculus; // 速度积分值
		uint32	TotalMileage;			// 累计总里程，米
		uint32	PackTotalNumber;		// 打包个数
		uint32	PackValidNumber;		// 打包的定位个数
		uint32	RestartCnt;				// GPS模块重启次数
		uint32	ValidTime;				// 有效定位总时间,秒
		uint32	DataErrorCnt;			// RMC错误数据条数
	}GPS;	
	struct
	{
		uint32	HighSpeedPulsCnt;		// 脉冲1（高速脉冲计数器累计值）      	 //G12 未用
		uint32	SpeedSensorPulsCnt;	// 脉冲2（车速传感器脉冲计数器累计值）	 //G12 未用
		uint32	MotorSpeedPulsCnt;	// 脉冲3（发动机转速脉冲计数器累计值）	 //G12 未用
		uint32	MotoRunTimeCnt;		// 发动机累计工作时长						 //G12 未用
		uint32	SystemWorkTime;		// 系统累计工作时长  
		uint32	SystemLastRunTime;	// 系统最后工作时刻							 //G12 未用
		uint32	Power1_2;				// 主电源、外部电池电压(即vbat1)，单位: mV		  
		uint32	Power3_4;				// 低2字节为板上电池(即vbat3)（除了报警电池）电压值，单位: mV	  
	}VDR;
	struct
	{
		uint32	SystemRestartCnt;		// 系统重启计数器
		uint32	Update;					// 程序升级重启次数
		uint32	GPRS;						// GPRS故障重启次数
		uint32	TaskOverTime;			// 任务超时重启次数
		uint32	PowerOn;					// 上电重启次数
		uint32	WatchDog;				//	看门狗重启次数
		uint32	WakeUp;				   // 休眠后唤醒重启次数	 
		uint32	ResetPin;				//	 保留
	} ResetCnt;
	struct
	{
		int32		Longitude;				// 4B，经度，放大了10e7倍，正为东经，负为西经
		int32		Latitude;				// 4B，纬度，放大了10e7倍，正为北纬，负为南?
		uint32	RtcSecond;				// 4B，时间，统一为秒，2000年1月1日00:00:00为第0秒
		uint32	Speed_Info;				// 1字节GPS速度，1字节高度，1字节GPS水平精度因子，1字节卫星个数
		uint32	GsmMmcMnc;				// 低12Bit为MMC，中间12Bit为MNC，高8Bit保留
		uint32	GsmLacCi;				// 2字节基站信息LAC，2字节基站信息CID
		uint32	Gsm_Info;				// 1字节基站信息BSIC, 1字节GSM信息强度，1字节环境温度，1字节主电源电压
		uint32	VehicleStatus1;		// 车辆状态标志位，前32位		
	}GG1;
	struct
	{
		uint32	VehicleStatus2;		// 车辆状态标志位，后32位	
		uint32	Rsv;
		uint32	UpLoadIndex;			// 数据上传流水号
		uint32	AccChangeCnt;
		uint32	GsmLbsCnt;
		uint32	RegIsDenied;			// GSM注册被拒绝
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
	各参数存储在Flash的页地址计数器，64个参数(256B)
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
	uint32	RoundEleRail;           // 圆形电子围栏
	uint32	SquqreEleRail;          // 方形电子围栏
	uint32	RoadLine;               // 路线计划
	uint32	SytemLog;               // 系统日志存储
	uint32	Numbers;						// 应付精度测试，条数，非页数
	uint32	BlindSave;              // 盲区存储
	uint32	BlindRead;              // 盲区读取
	uint32	HistorySave;            // 历史数据存储
	uint32	HistoryRead;            // 历史数据读取
	uint32	PicSave;                // 图片存储
	uint32	PicRead;                // 图片读取	
	uint32	CanBusDataSave;			// 
	uint32	CanBusDataRead;			// 
	uint32	AlarmMsgSave;				// 含报警的位置信息
	uint32	AlarmMsgRead;				// 
	uint32	NormalMsgSave;				// 普通位置信息
	uint32	NormalMsgRead;				// 
	uint32	GnssDataSave;				// 
	uint32	GnssDataRead;				//
	uint32	Speed48HSave;				// 48小时速度及开关量信息
	uint32	Speed48HRead;
	uint32	Location360HSave;			// 360小时位置信息
	uint32	Location360HRead;
	uint32	ParkingDataSave;			// 停车数据
	uint32	ParkingDataRead;
	uint32	FatigueDrivingSave;		// 疲劳驾驶
	uint32	FatigueDrivingRead;
	uint32	DriverLogInOutSave;     // 登签、退签
	uint32	DriverLogInOutRead;	
	uint32	PowerOnOffSave;         // 上电断电记录
	uint32	PowerOnOffRead; 
	uint32	ParaModifySave;         // 参数修改
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
   车辆状态基本数据结构。
   采集时1有效，0无效。如果发送时需负逻辑，则需修改。
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
			uchar		TemperatureAlarm:1;		// 温度报警					 
			uchar		PasswordErrorAlarm:1;	// 密码错误报警			 
			uchar		GPRS_Delay:1;		  		// GPRS阻塞报警		G11使用	 
			uchar		GasAndPowerOffStatus:1;	// 车辆处于断油断电状态		 G11使用
			uchar		MainPowerLost:1;		   // 电瓶被拆除		  G11使用
			uchar		HighLevelSensor1:1;	   // 高电平传感器1
			uchar		HighLevelSensor2:1;		// 高电平传感器2
			uchar		LowLevelSensor1:1;      // 低电平传感器1  IdleMode:1;					// 省电标志（进入省电前发送该标志）
		} Bits;
	} VS1;	
	////////////////////////////////
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		GPS_ModuleError:1;		// GPS模块有问题		  G11使用
			uchar		Temperature1:1;			// 温度1
			uchar		Temperature2:1;		  	// 温度2
			uchar		BackupBatterySupply:1;	// 主机由后备电源供电
			uchar		MainPowerLost:1;			// 电瓶被拆除
			uchar		GPS_ANT_Open:1;	   	// GPS天线开路    	   				// 
			uchar		GPS_ANT_Short:1;			// GPS天线短路   
			uchar		LowLevelSensor2:1;      // 低电平传感器2     LowPowerAlarm:1;			// 低压报警标志
		} Bits;
	} VS2;	 
	////////////////////////////////
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		DoorOpen:1;					// 车门开					G11使用
			uchar		EnableAlarm:1;				// 车辆设防状态
			uchar		ACC:1;		  				// ACC					   G11使用
			uchar		Pressure1:1;	         // 压力1报警		      	 
			uchar		Pressure2:1;		      // 压力2报警
			uchar		Moto:1;	   				// 发动机					G11使用
			uchar		OwnDefineAlarm:1;			// 自定义报警
			uchar		OverSpeed:1;				// 车辆超速					G11使用
		} Bits;
	} VS3;	
	////////////////////////////////
	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		StealAlarm:1;				// 盗窃报警				G11使用
			uchar		RobAlarm:1;					// 打劫报警				G11使用
			uchar		OverSpeed:1;		  		// 超速报警				
			uchar		LawlessPowerOn:1;			// 非法点火				G11使用
			uchar		InAreaAlarm:1;		   	// 禁止驶入越界报警		G11使用
			uchar		GPS_ANT_Open:1;	   	// GPS天线开路报警		G11使用
			uchar		GPS_ANT_Short:1;			// GPS天线短路报警		G11使用
			uchar		OutAreaAlarm:1;			// 禁止驶出越界报警		G11使用
		} Bits;
	} VS4;		

	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		GSMmodelStatus:1;			      	// 手机模块良好			G11使用
			uchar		SIMcardStatus:1;			      	// 手机卡良好				G11使用
			uchar		MobilSignalStatus:1;		  	     	// 找到GSM信号				G11使用
			uchar		GPRSonlineStatus:1;		        	// GPRS在线					G11使用
			uchar		CameraOK:1;	            			// 摄像头良好			   G11使用
			uchar    EMTstatus:1;						   //环境温度正负
			uchar    OilTankStatus:1;				      //油箱温度正负
			uchar    SystemStart:1;					   	//系统启动			 G11使用
		} Bits;
	} VS5;		

	union
	{			
		uchar		Byte;					
		struct		
		{

		   uchar		UndefendAlarm:1;	    	// 未设防报警
			uchar		FTdrivingAlarm:1;		  	// 疲劳驾驶报警		 G11使用
			uchar		VibrationAlarm:1;		  	// 震动报警				 G11使用
			uchar		LowPowerAlarm:1;	     	// 主电源电压低报警	  G112使用
			uchar		StopCarOverTime:1;	  	//停车超时  			  G11使用
			uchar		CarBeTrail:1;	         //拖车报警 				  G11使用
			uchar		UnlawOpenDoor:1;	     	//非法开门
			uchar		CallHelpAlarm:1;		   //求助报警
			
		} Bits;
	} VS6;		


	union
	{			
		uchar		Byte;					
		struct		
		{
			uchar		TemperatureAlarm3:1;	   	// 温度3报警
			uchar		TemperatureAlarm4:1;	   	// 温度4报警
			uchar		Pressure3:1;		         // 压力3报警
			uchar		Pressure4:1;			      // 压力4报警
			uchar		HighLevelSensor3:1;	          	// 高电平传感器3
		   uchar		HighLevelSensor4:1;		         // 高电平传感器4
		   uchar		LowLevelSensor3:1;					//	低电平传感器3
		   uchar		LowLevelSensor4:1;		         // 低电平传感器4
		} Bits;
	} VS7;	
	
	
	union
	{			
		uchar		Byte;					
		struct		
		{
		  uchar		EnablePowerSave:1;	       	//允许省电
		  uchar		IdleMode:1;				      	//省电标志（进入省电前发送该标志）	 
		  uchar		BackDoorOpen:1;		       	//后尾箱打开  
		  uchar		DoorUnlockAlarm:1;	        	//
		  uchar		PhoneCalling:1;					//通话标志		  	   G11使用	  
		  uchar		CtrlLineAlarm:1;		         //控制线断开报警
		  uchar		RemovalAlarm:1;			      //	拆除报警				 
		  uchar		CanBusLockFlag:1;		         // CAN总线锁车标志
		} Bits;
	} VS8;			


} STRU_VSDR,*pSTRU_VSDR;					// Vehicle Status Data Recorder
				

USER_EXT		STRU_VSDR		VehicleStatus;


// 国业统一通讯协议帧格式///////////////////////// 协议帧格式
//#define		GY_PROTOCOL_CHECK_START_ADDR			6
//#define		DATA_OFFSET_M_PACK						18
//#define		DATA_OFFSET_1_PACK						14


#define		LEN_GY_HEAD_TO_CHECK	6		  // 协议头到校验码的总长
#define		LEN_GY_CMD_TO_ATTR		8         // GY 协议中的命令字至长度属性的长度
#define		LEN_GY_PACK_INFO	    4		  //  有分包时，分包总数和包序总长
//#define		LEN_GY_NO_PACK_HEAD		14        //  无分包的协议头部总长	

/*typedef		struct	_GY_COMM_PROTOCOL_
{
	uint32	Head;					// 4B，*GY:	// 0x3A59472A
	uint16	Check;				// 2B，校验
	uint16	Cmd;					// 2B，命令字
	uint32	Index;				// 4B，流水号
	union
	{
		uint16	Len;
		struct
		{
			uchar		Len:8;			// 11Bit，数据段长度
			uchar		LenH:3;			// 
			uchar		Encry:2;			// 2Bit
			uchar		Pack:1;			// 1Bit
			uchar		Check:2;			// 2Bit
		}Bits;
	}Attr;
	union
	{
		uchar		Buffer[128];		// 实际长度最大2048字节，请使用结构指针指向大缓冲
		uint16	B16[64];				// 
		uint32	B32[32];				// 
		struct
		{
			uint16	Total;
			uint16	Index;
		}Pack;
	}Data;
}GY_COMM_PROTOCOL,*pGY_COMM_PROTOCOL;	*/



/// 系统日志第7组参数对应的结构体如下，可以使用内在拷贝方式写入系统日志。
/// 上位机管理软件按本结构体分析系统日志第7组参数
/*typedef		struct
{
	int32		Longitude;			// 4B，经度，放大了10e7倍，正为东经，负为西经
	int32		Latitude;			// 4B，纬度，放大了10e7倍，正为北纬，负为南?
	uint32	RtcSecond;			// 4B，时间，统一为秒，2000年1月1日00:00:00为第0秒
	uchar		Speed;				// 1B, 速度，海里/小时
	uchar		Altitude;			// 1B，海拔，单位：50米，最高50*255米
	uchar		Level;				// 1B，GPS水平精度因子
	uchar		SatelNumber;		// 1B，GPS卫星个数	
	uint16	LAC;					// 2B，GSM基站信息1
	uint16	CI;					// 2B，GSM基站信息2
	uchar		BSIC;					// 1B，GSM基站信息3
	uchar		GsmSignal;			// 1B，GSM信息质量值
	uchar		eTepm;				// 1B，环境温度，-127 ~ +127
	uchar		Rsv;					// 1B，保留
	uint32	VehicleStatus1;	// 车辆状态标志位，前32位
	uint32	VehicleStatus2;	// 车辆状态标志位，后32位	
} GG_INFO,*pGG_INFO;	  */


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   波特率
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
   驾驶员相关信息，EEPROM存储空间为连续存储，否则出错
*/
#define		LEN_ALL_DRIVER_INFO			(32+16*4+8)
#define		LEN_DRIVER_INFO				16
#pragma		pack(1)
typedef	struct	
{	
	uint32	ID;					
	uint32	LogInTime;
	uint32	LogOutTime;
	uchar		LogStatus;		// 2个状态，登录或退出
	uchar		CardStatus;		// 2个状态，合法卡、非法卡
	uchar		Rsv1,Rsv2;
} DRIVER_INFO,*pDRIVER_INFO;
USER_EXT    volatile		DRIVER_INFO		Driver,Driver1,Driver2,Driver3;	// 当前号和最近4个号
USER_EXT    uint32		DriverID[8];	// 最多同时8个司机，可通过管理中心修改


//#define		LEN_TIRED_DRIVE			8
//typedef	struct
//{
//	uint16	WorkTime;	// 允许最长工作时间
//	uint16	RestTime;	// 最短休息时间
//	uchar		OnTime;		// 从休息转到工作的过度时间
//	uchar		OffTime;		// 从工作转到休息的过度时间
//	uchar		Speed;		// 防止漂移，速度达到N值后开始算工作时间
//	uchar		Para;			// 
//} TIRED_DRIVE,*pTIRED_DRIVE;
//
//USER_EXT    volatile		TIRED_DRIVE		TiredDrive;


/*typedef	struct
{
	uint32	WorkTime;	      //允许最长工作时间
	uint32   WorkTimeOneDay;   //当天累计驾驶时间门限 
	uint32	RestTime;	      //最短休息时间
	uint32   StopTime;         //最长停车时间
   uchar		OnTime;		      //从休息转到工作的过度时间
	uchar		OffTime;		      //从工作转到休息的过度时间
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

	uchar		KeyHang;		// 按键挂机命令
	uchar		GsmHang;		// GSM模块指令指令挂机
	uchar		CalledAnswer;	// 被叫，手柄应答
	uchar		CallerAnswer;	// 主叫，对方应答

	uchar		DialCnt;
	uchar		ListeningFlag;
	uchar    Answer;
	uchar    RingCnt;

	uchar    Dialing;
	uchar		CenterCall;		// 中心呼叫
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
	uint32	PulsControl;			// 锁车脉冲控制
	uint32	TickForRandData;		// 使用重复中断计数为随机数
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


USER_EXT    volatile		uchar       OverSpeedAlarmType;   //超速报警类型
USER_EXT    volatile		uchar       InOutAreaType;        //进出区域报警类型
USER_EXT    volatile		uchar       InOrOutArea;          //进或出区域标识

USER_EXT    volatile		uint32      RoundOverSpeedAlarmTime;  //圆形围栏超速报警时限



// add by xzb
USER_EXT	 uchar	ManulAlarmEventID;			  // 人工报警事件ID

USER_EXT  	 uint32	OverSpeedAreaRoadID;
USER_EXT  	 uint32 	FenceOrRoadAlarmID;       // 路段ID
USER_EXT  	 uint16  RoadSectionDriveTime;    // 路段行驶时间
USER_EXT  	 uchar  	RoadSectionDriveResult;  // 结果


// added 2013.3
USER_EXT	volatile 	uint16   JT808Analog1, JT808Analog2;           // JT808协议中附加信息中的两路模拟量





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

USER_EXT   uchar Jt_photo_upload;					      // 图片上传平台标志
USER_EXT   uchar AT_Command_Busing,GSM_TaskBusing;
extern uchar MediaEventFlag;

USER_EXT	   enum		          // 多媒体事件项编码
{ 	
	eBB_MEDIA_EVENT_PLAT,			  	// 平台下发指令
	eBB_MEDIA_EVENT_ON_TIME,		  	// 定时动作
	eBB_MEDIA_EVENT_ROB,			     	// 抢劫报警触发
	eBB_MEDIA_EVENT_CRASH,			  	// 碰撞侧翻报警触发
	eBB_MEDIA_EVENT_OPEN_DOOR,			// 开门拍照	
	eBB_MEDIA_EVENT_CLOSE_DOOR,		// 关门拍照	
	eBB_MEDIA_EVENT_SPEED_OVER20,		// 速度从低于20，到超过20	
	eBB_MEDIA_EVENT_DISTANCE,			// 定距离拍照
	eBB_MEDIA_EVENT_OTHER,          	// 	
	
} BB_MUL_MEDIA_EVENT_ENUM;



///////////////////////////////////////////////// 高低电平有效标志
USER_EXT    enum
{
	HL_DOOR = 0,
	HL_ACC,
	HL_AUTO,
	HL_MAIN_POWER,
	
} enHIGH_LOW_LEVEL;		// 高低电平指外部接口的，非CPU引脚处

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
   IP相关
*/
USER_EXT		enum						  // 这里的顺序不能修改！！！
{
	eFLAG_TCP = 0,				  //IP地址或DNS		  JT  主服务器
	eFLAG_UDP = 1,				  //IP地址或DNS
	eFLAG_TCP1 =2,				  //IP地址或DNS		  JT  备份服务器
	eFLAG_UDP1 =3,				  //IP地址或DNS
	eFLAG_TCP2  =4,			  //IP地址或DNS		  JT  临时连接或升级服务器
	eFLAG_UDP2  =5,			  //IP地址或DNS

	eFLAG_IC_AUTH_TCP = 6,			  // IC 卡认证TCP
	eFLAG_IC_AUTH_UDP = 7,			  // IC 卡认证UDP
	eFLAG_IC_AUTH_B_TCP = 8,		  // IC 卡认证备份TCP
	eFLAG_IC_AUTH_B_UDP = 9,		  // IC 卡认证备份UDP

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
	uchar		TCP[LEN_IP_PORT];			//公用TCP池
	uchar		UDP[LEN_IP_PORT];			//公用UDP池
	uchar		TCP1[LEN_IP_PORT];
	uchar		UDP1[LEN_IP_PORT];
	uchar		TCP2[LEN_IP_PORT];
	uchar		UDP2[LEN_IP_PORT];
	uchar		APN[16+1];				// APN
	uchar		Socket[12];				// 上面ENUM_IP_INDEX 10组IP地址的链接的Socket通道号
	uchar		Online[12];				// 上面ENUM_IP_INDEX 10组IP地址在线状态
	uchar		EnableFlag[16];			// 使用标志 =ENABLE_FLAG使能，其它值禁能
}sIP;

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   电话号码相关
*/
#define		LEN_SMS_NUMBER			(16)
USER_EXT		struct
{
	uchar		Center[LEN_SMS_NUMBER+1];
	uchar		Center1[LEN_SMS_NUMBER+1];
	uchar		User[LEN_SMS_NUMBER+1];
	//uchar		User1[LEN_SMS_NUMBER+1];
	//uchar		Help[LEN_SMS_NUMBER+1];
	uchar		SmsCenter[LEN_SMS_NUMBER+1];	// 移动公司SMS中心号码 JT808也用这个号码
	uchar		Sim[LEN_SMS_NUMBER+1];			// 本机号码 JT808也用这个号码
	uchar		SimBCD_GB[8];
	uchar		SimBCD_JT[8];
}sTelNum;



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   JT808 设置项
*/
#define  LEN_IPorDNS		 32
#define  LEN_IP_NAME		 16
#define  LEN_IP_SECRIER	 8

/* 设置发送超时，以及回传相关时间
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
#define		LEN_SEND_INTERVAL_STRU			 	(12*4)		// 最后两个驻留内存，不存储在铁电
USER_EXT struct _JT808_DATA_UP
{	
	uint32 HeartbeatTime;
	uint32 NoDriverTime;						          //驾驶员未登录时时间回报间隔	  s
	uint32 IDEL_ModeTime;                         //休眠时时间回报间隔				s
	uint32 AlarmModeTime;						       //紧急报警时时间回报间隔			s
	uint32 DefauletTime;						          //缺省时间回报间隔     s
	uint32 DefauletDistance;						    //缺省距离回报间隔     s
	uint32 NoDriverDistance;						    //驾驶员未登录时距离回报间隔	  s
	uint32 IDEL_ModeDistance;                     //休眠时距离回报间隔				s
	uint32 AlarmModeDistance;						    //紧急报警时距离回报间隔			s
	union
	{
		uint32	B32;
		struct
		{
			uchar  Mode;                                  //位置回报策略 0：定时回报  1：定距回报  2：定时和定距
			uchar  Condition;								       //位置回报方案0 根据ACC状态 1：根据登录状态和ACC状态，先判断登录状态，若登录再根据ACC状态
			uchar  Angle;                           	    //拐角补传角度
			uchar		Rsv;
		}Byte;
	}MCA;
	uint32	T_Second;	  				// 临时跟踪时间间隔
	uint32	T_ContinuedTime;			// 临时跟踪时长
	uint32	Second;					  	// 上专时间间隔
	uint32	Distance;					// 上传距离间隔	
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
	uchar  Center[LEN_NUMBER];							  //监控平台号码
	uchar  Reset[LEN_NUMBER];							  //复位号码
	uchar  ResetFactory[LEN_NUMBER];					  //恢复出厂设置号码
	uchar  CenterSMS[LEN_NUMBER];						  //监控平台短信号码
	uchar  TextSMS[LEN_NUMBER];						  //接收文本报警号码
	uchar  GetCallMode;									  //接听来电方式0： 自动接听  1： ACC on时自动接听 Off时手动接听
	uint32 CallTime;										  //每次允许通话时长
	uint32 CallTimeMonth;						        //每月允许通话时长
	uchar  JT[LEN_NUMBER];								  //监听号码
	uchar  Privilege[LEN_NUMBER];						  //特权SMS号码
} JT808_Number, *pJT808_Number;

#pragma		pack(4)
USER_EXT struct _JT808_ALARM
{
	uint32 Shield;                                 //报警屏蔽字，报警标志相应位为1，相应报警被屏蔽
	uint32 SendTextSMS;								     //报警标志相应位为1 ，报警时发生文本短信
	uint32 TakePhoto;						              //报警标志相应位为1 ，报警时拍照	 
	uint32 SavePhoto;                              //报警标志相应位为1 ，报警时存储照片，	否则上传照片
	uint32 KeyAlarm;						              //报警标志相应位为1 ，该报警为关键报警			

} JT808_Alarm_Word, *pJT808_Alarm_Word;




#pragma		pack(1)
USER_EXT struct _JT808_Camera_Para_
{
	uchar  Quality;                                 //图像/图片质量 1~10， 1，最好
	uchar  Light;								            //亮度
	uchar  Contrast;						               //对比度	 
	uchar  Satulation;                              //饱和度
	uchar  Colour;						                  //色度			

} JT808_Camera_Para, *pCamera_Para;


#pragma		pack(1)
USER_EXT struct _JT808_Ctr_sIP				        //连接指定的服务器或升级服务器
{
	uchar  Ctr;												  //连接控制符0：切换到指定平台，1：切换回原来监控平台
	uchar  C_ANP[LEN_IPorDNS];
	uchar  C_IP_Name[LEN_IP_NAME];
	uchar  C_IP_Secrier[LEN_IP_SECRIER];
	uchar  C_IPorDNS[LEN_IPorDNS];
	uint32 C_TCP_Port;
	uint32 C_UDP_Port;
	uchar  CP_ID[5];
	uchar  Akey[LEN_IP_NAME];										  //临时连接的平台鉴权码
	uchar  HVS[LEN_IP_NAME];
	uchar  HWVS[LEN_IP_NAME];
	uchar  URL[LEN_IPorDNS];
	uint16 Net_Time;
} JT808_Ctr_IP_DNS, *pJT808_Ctr_IP_DNS;

#define   LEN_JT_CTR_IP_DNS       (1+3*LEN_IPorDNS+4*LEN_IP_NAME+LEN_IP_SECRIER+3*2+5)






#pragma		pack(1)
USER_EXT struct _JT808_GET_GB_DATA_			//远程读取记录数据
{
	uint16  Index;					   			//下发命令的流水号    
	uchar   GetDataCmd;                  //下发GB命令字
	uchar   Rvs;        
	uint16  LenPack;						      //数据包大小
	uint16  SendPackNummber;					//目前传输到第几包数据
	uint16  PackNummber;                  //分几包传输		
	uint32  LenAllData;						   //未传数据长度
	uint16  PageAddr;                      //当前搜索到的flash页
	uint16  LenReadFromFF;                 //从flash读出的数据长度
	
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


USER_EXT    		uchar		SimNumberBCD[8];							// 本机手机号码BCD

USER_EXT    		char		sNowDateString[7];
USER_EXT    		char		sNowTimeString[7];

#define				USE_GPRS_PROTOCOL			'U'						// GB19506协议默认为固定使用，可加其它4种
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
USER_EXT    volatile		uchar		IC_CardType;		// 0,'A','4'，三个状态，0无卡

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
USER_EXT  	volatile		char		EnableParkOverTimeAlarm;				// 允许使用停车超时报警检测
USER_EXT    volatile		uint16	ParkOverTimeAlarmTime;					// 停车超时报警时间(分)


USER_EXT    volatile		uchar		AlarmStatusFlag;

USER_EXT    volatile		uint16	EnableAlarmFlag;							// 总警情检测标志(bit0:为1开启盗警;bit1:劫警;bit2:未设防报警;bit3:拖车报警)

USER_EXT    volatile		char		EnableStealAlarm;							// 允许使用盗警检测
USER_EXT    volatile		char		EnableRobAlarm;							// 允许使用劫警检测
USER_EXT    volatile		char		EnableNoSetScanAlarm;					// 允许使用未设防报警检测
USER_EXT    volatile		char		EnableTrailCarAlarm;						// 允许使用拖车报警检测
USER_EXT    volatile		char		EnableHelpAlarm;							// 允许使用求助报警检测
USER_EXT    volatile		char		EnableTireDriveAlarm;					// 允许使用疲劳驾驶检测
USER_EXT    volatile		char		EnableIllegalOpenDoor;					// 允许使用非法开门检测
USER_EXT    volatile		char		EnableIllegalMakeFire;					// 允许使用非法点火检测
USER_EXT    volatile		char		EnableCheckShake;							// 允许使用震动检测
USER_EXT    volatile		char		EnableAutoLockDoor;						// 允许使用锁门
USER_EXT    volatile		char		EnableLowVoltage;							// 允许使用低压检测

USER_EXT    volatile		char		EnableDialOrAnswerFlag;					// 允许通话功能标志(bit0:置1允许打出;bit1:置1允许接听)
USER_EXT    volatile		char		EnableRemoteMonitor;						// 允许远程监听标志


USER_EXT    volatile		char		SetScanAlarmFlag;							// 车辆设防状态标志

USER_EXT    volatile		char		PulsControlCnt;							// 断油时的方波计数

USER_EXT    volatile		char		LockDoorFlag;								// 锁门标志		
USER_EXT    volatile		char		SPK_Flag;	  								// 喇叭标志		 	  	
USER_EXT    volatile		char		AlarmLight_Flag;							// 警灯标志
USER_EXT    volatile		char		DoorStatus;									// 门磁状态
USER_EXT    volatile		char		LockDoorSignal;							// 锁车信号输入检测

USER_EXT    volatile		char		HandleRecvDataFlag;


USER_EXT    volatile		uchar		CheckLinkModeEnable;	
USER_EXT    volatile		uchar		CameraWorking,SendPicData,SendAudioData;

USER_EXT		volatile     uchar	TempGPSValidTime;

USER_EXT		volatile     uint32	InvalidGPSValidTime;		  // 不定位时间


	
/*
////////////////////////////////////////////////////////////////////////////////////////
USER_EXT    volatile		uchar		DisplayLcdType;							// 命令类型
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



USER_EXT    volatile		char		HandleWatchCarPassword[6];			   	// 手柄看车密码
USER_EXT    volatile		uchar		HandleCmdtype;
USER_EXT    volatile		uchar		LockCarFlag;                  	      // 看车标志
USER_EXT    volatile		uchar		LoginFlag;								   	// 登陆成员标志
USER_EXT    volatile		uchar		HandleLoginSuccess;					   	// 手柄密码登录成功标志

USER_EXT    volatile		uchar		LcmDataToServer;

USER_EXT    volatile		uint16		Plat_Serial_No;						  // 记录特定的平台回复流水号
																	
USER_EXT    	uchar    	FactoryPassword[8];									// 手柄的出厂设置密码
USER_EXT    	uchar    	EnableHandleWatchCar;								// 是否允许手柄看车
USER_EXT    	uchar    	HandleWatchCarMode;									// 手柄看车方式


USER_EXT    	uchar    	GpsTempSaveFlag;										// GPS信息临时存储标志

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
	///// 事件
	ACCEL_NO_EVENT,
	ACCEL_TILT,
	ACCEL_SINGLE_TAP,
	ACCEL_DOUBLE_TAP,

	///// 方向
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
	中断优先组分配(小的优先级高）
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
USER_EXT    	uint32    	UpgradeFlag;											// 升级标志
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

USER_EXT  volatile	uchar		ucSpeedLimit;				//超速阀值

USER_EXT  volatile	uchar		SpeedModeSelect;			//速度取GPS速度还是脉冲速度标志	1 GPS  0 脉冲

USER_EXT  volatile	uint32	PulsPerMin;

USER_EXT  volatile	uchar 	SystemSecondCntLast;

USER_EXT    volatile		uint16       LenDataForUSB;

USER_EXT    volatile		uchar        USBUseFlag;

USER_EXT    volatile		uchar        Write_File_Error;

USER_EXT    volatile    uchar        Wfile_Flag;              // 标识第一次插入U盘

USER_EXT    volatile		uchar        USBCnt;

USER_EXT    volatile		uchar        USBTaskCreat;		//USB创建了任务标志

USER_EXT    volatile		uchar        PrintTaskCreat;		//打印创建了任务标志

USER_EXT    volatile		uchar        CamererTaskNow;		//在执行拍照任务标志

USER_EXT    volatile		uint16       NumberOfSatlice;			//可见卫星个数	

//USER_EXT    volatile		uchar        GSM_Signal;              //gsm信号强度 

USER_EXT    volatile		uchar        SelfTestResust;   		 //自检结果

USER_EXT    volatile		uchar        JT_Ctr_PowerDown;          //关机

USER_EXT    volatile		uchar        JT_ShutCommunication;		 //关闭通信标志

USER_EXT    volatile		uint32       JT_ShutMobileCnt;          //关闭所有无线通信

USER_EXT    volatile		uchar        GetGB_Data_Flag;          //无线通道取记录数据标识 0，不起作用；  1，取数据长度；2，取数据；

USER_EXT    		uchar              DriverCardFlag,CurrentRegInfo;            //身份卡标识1，有卡，0 无卡



USER_EXT    volatile		uint32       InitMileage;    	//初始里程

USER_EXT    volatile		uchar        LastAzimuth;      //上一次GPS数据的角度

USER_EXT    volatile		uchar        LastAzimuthCnt;	 //拐角计数器

USER_EXT    volatile		uchar        UseWthSimCard;    //SIM卡选择 6 内置SIM卡  0 外置SIM卡

USER_EXT    volatile		uchar        SetCarFlat;		// 设置卡/授权卡插入标志

USER_EXT    volatile		uchar        IDEL_For_Acc_OFF;		// ACC关进入省电模式标志

USER_EXT    volatile		uint32        IDEL_Dly_Cnt,CanOverTimeCnt;		// 进入省电模式计时器

USER_EXT		volatile		uchar			  SaveLogFlag;			// 保存日志标志
USER_EXT		volatile		uchar			  InRtcIntFlag;
USER_EXT				uchar			  OverTimeDriveUpateFlag; // 0: 更新超时驾驶铁电计数	  1:不更新


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

USER_EXT		uint16		JtCrashAlarmSet;					  // 碰撞报警参数设置	 b7-b0: 碰撞时间，单位：4ms, b15-b8: 碰撞加速度，
															  // 单位：0.1g, 设置范围在:0 - 79之间，默认为10.
USER_EXT		uint16		JtTiltAlarmSet;						  // 侧翻报警参数设置：侧翻角度，单位1度，默认30度

USER_EXT		uchar       IC_Card_Insert;						   // IC卡状态

USER_EXT		uchar			DialOK_NeedAuthFlag,MenuSetRemoteIAP,UseGPS_Mil,EnableSysSleep,EnableVDR_Upload;

USER_EXT	uchar  CloseSocketStatus, DisConectSomeSocket;						   // 关闭socket通道状态， 关闭指定通道标志 

#define		PARA_BLOCK_CONFIG_FLAG		0xA57E
#define		QUICKLY_SAVING_DATA			0xA5
USER_EXT	uchar  SamlePeriod1S,TestAccuracy,TestBlind,TestCanBus,ReadyTestBlind,TestRectFence,TestRoadLine,GetGnssDataFlag;
USER_EXT	uchar  TestMultiCenter,SystemSelfTest;


USER_EXT	uchar ICCardBusing;			// TRUE: IC卡在操作,忙碌		FALSE：IC卡空闲

#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
