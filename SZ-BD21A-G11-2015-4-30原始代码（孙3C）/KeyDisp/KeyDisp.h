/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述                              
	文件名: Key.h
	日期：	2005-3-13	
   矩阵键盘键值扫描，功能处理函数
   本程序处理AD键盘，适用于AVR系列单片机。
   键值读取函数即：GetKeyCode();
   在键盘初始化程序InitKeyboard中把ucKey1和ucKey2赋值0xff
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
#ifndef      __KEYDISP_H__
#define      __KEYDISP_H__


#ifdef      KEKDISP_GLOBALS
#define     KEKDISP_EXT
#else
#define     KEKDISP_EXT    extern
#endif

#include		"..\Config\RTOS.h"
//#include		 "..\Config\VariableType.h"

 ////////// 指示灯的状态定义/////////////////

#define		LED_TURN_OFF				0			// 关闭
#define		LED_1T_PER_4SEC			1			// 1次/4秒
#define		LED_1T_PER_3SEC			2			// 1次/3秒
#define		LED_1T_PER_2SEC			3			// 1次/2秒
#define		LED_1T_PER_1SEC			4			// 1次/秒
#define		LED_2T_PER_1SEC			5			// 2次/秒
#define		LED_3T_PER_1SEC			6			// 3次/秒
#define		LED_4T_PER_1SEC			7			// 4次/秒
#define		LED_5T_PER_1SEC			8			// 5次/秒
#define		LED_6T_PER_1SEC			9			// 6次/秒
#define		LED_7T_PER_1SEC			10			// 7次/秒
#define		LED_8T_PER_1SEC			11			// 8次/秒
#define		LED_TURN_ON					12			// 长亮(实为快速扫描)

////////// GPRS指示灯的状态定义////////////////////
#define		LED_MOBILE_ERROR			0			// 手机模块不工作
#define		LED_MOBILE_NO_SIM			6			// 检测不到SIM卡
#define		LED_MOBILE_UNREG			4			// 未找到营运商网络
#define		LED_MOBILE_REG				1			// 找到营运商网络
#define		LED_GPRS_OFF_LINE			1			// 找到营运商网络，模块工作，但不在线，1次/秒
#define		LED_GPRS_SEND_DATA		11			// GPRS正在发送数据
#define		LED_GPRS_ON_LINE			12			// GPRS在线，长亮



#define		LED_TOTAL_NUMBER			4			// 指示灯数量
#define		LED_GPS						0			// 第0个为GPS指示灯
#define		LED_GPRS						1			// 第1个为GPRS指示灯
#define		LED_EXT_Singal				2			// 第2个为外部信号指示灯
#define		LED_LOW_IDLE				3			// 第3个为低功耗指示灯

// 如果Times>Counter，则单声响，否则多声响

#define			LCM_FRAME_HEAD			0
#define			LCM_FRAME_CMD			1
#define			LCM_FRAME_LENGTH		2
#define			LCM_FRAME_DATA			3
#define			LCM_FRAME_XOR			(U0_RecBuffer[LCM_FRAME_LENGTH]+(3-2))
#define			LCM_PDU_SEND_NUM		12

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	D: Download, LCM -> GPS
	U: Upload, GPS -> LCM
*/
KEKDISP_EXT		enum			 // 调度屏发命令到主机
{
	eLCM_D_MESSAGE_TO_CENTER 		= 0x01,				//  
	eLCM_D_MESSAGE_TO_MOBILE		= 0x02,				// 
  	eLCM_D_SET_UDP_IP_PORT			= 0x03,				// 
	eLCM_D_SIM_CARD_NUMBER			= 0x04,				// 
	eLCM_D_CENTER_NUMBER			= 0x05,			   // 
	eLCM_D_NO_SCAN_ALARM_TIME		= 0x06,				// 
	eLCM_D_DIAL_NUMBER				= 0x07,				// 
	eLCM_D_RING_OFF					= 0x08,				// 
	eLCM_D_SEND_DTMF				= 0x09,				// 
	eLCM_D_LCM_VERSION				= 0x0a,				// 
	eLCM_D_SET_APN					= 0x0b,				//
	eLCM_D_SELF_CHECK				= 0x0c,				// 
	eLCM_D_ANSWER_CALL				= 0x0e,				// 
	eLCM_D_HAND_OFF					= 0x0f,				//
	eLCM_D_SET_USE_ALARM_NUMBER		= 0x10,				//
	eLCM_D_QUE_MDT_STATUS			= 0x11,				// 
	eLCM_D_SET_WATCH_CAR			= 0x12,				//
	eLCM_D_CANCEL_WATCH_CAR			= 0x13,				// 
	eLCM_D_SELECT_APN_OR_GPRS		= 0x14,				// 
	eLCM_D_SELECT_UDP_OR_TCP		= 0x15,				/// 
	eLCM_D_ADJ_VOL					= 0x16,				// 
	eLCM_D_ADJ_MIC					= 0x17,				// 

	eLCM_D_DOOR_LEVEL				= 0x21,				// 
	eLCM_D_CAR_SPK_LEVEL			= 0x22,				// 
	eLCM_D_SET_RADIO_STATUS			= 0x23,				// 
	eLCM_D_SET_HIDE_MODE			= 0x24,				// 
	eLCM_D_ER_PASSWORD				= 0x25,				// 
	eLCM_D_SET_CAR_NUMBER			= 0x26,		// 
	eLCM_D_SET_CENTER_NUMBER1		= 0x27,		// 
	eLCM_D_SET_SHAKE_VALUE			= 0x28,		// 
	eLCM_D_SET_TEMPERATE_VALUE		= 0x29,		// 

	eLCM_D_DRIVER_LOGIN_OUT			= 0x30,		// 
	eLCM_D_SIM_CARD_NUMBER1			= 0x31,		// 
	eLCM_D_SET_ROB_ALARM_NUMBER		= 0x32,		// 
	eLCM_D_QUE_MDT_STATUS2			= 0x33,		// 
	eLCM_D_SET_CALL_MODE			= 0x34,		// 
	eLCM_D_SET_TCP_IP_PORT			= 0x35,		// 

	eLCM_D_RESTORE_FACTORY_PARA		= 0x37,		// _RestoreFactorySetting
	eLCM_D_SET_SERVER_LOGIN_NAME	= 0x38,		// 
	eLCM_D_SET_SERVER_LOGIN_PSD		= 0x39,		// 
	eLCM_D_CAR_REG_INFO				= 0x51,		// 	车辆注册信息
	eLCM_D_CAR_REG_RESULT			= 0x52,		// 注册返回结果

}  ENUM_LCM_CMD_D,*pENUM_LCM_CMD_D;


KEKDISP_EXT		enum			   // 主机发命令到调度屏
{


	//////////////////////////////////////////////////////////////////////////
	eLCM_U_RING							= 0x03,		// 	 modified 2013.4
	eLCM_U_GMS_SIGNAL					= 0x05,		// 网络状态
	eLCM_U_PHONE_VOL					= 0x0a,			// 返回音量值
	//eLCM_U_GPS_INFO						= 0xa0,		// 
	eLCM_U_CENTER_MSG					= 0xa6,		// 	 modified 2013.4
	eLCM_U_CENTER_TMP_MSG				= 0xa8,     // 临时信息，语音播报

	//eLCM_U_HOLD_OFF						= 0xa6,		// 
	eLCM_U_ANSWER_CALL					= 0x13,		// 
	eLCM_U_UPDATE_CONST_INFO			= 0xac,		// 
	eLCM_U_CLEAR_CONST_INFO				= 0xad,		// 
	eLCM_U_REC_PDU_SMS					= 0xa6,		// 	  modified 2013.4
	eLCM_U_REC_TXT_SMS					= 0xb0,		// 
	eLCM_U_REMOTE_SET_CENTER_NUMBER		= 0xb2,		// 
	eLCM_U_REMOTE_RESET_WATCH_PSD		= 0xb3,		// 
	eLCM_U_UPDATE_GROUP_NUMBER			= 0xb4,		// 
	eLCM_U_CLEAR_GROUP_NUMBER			= 0xb5,		// 
	eLCM_U_REMOTE_CHANGE_HANDLE_PSD		= 0xb6,		// 
	eLCM_U_PLEASE_INPUT_WATCH_PSD		= 0xb7,		// 

	eLCM_U_MDT_STATUS_INFO				= 0xc2,		// 
  	eLCM_U_ELE_CALL_CAR					= 0xe0,		// 

	eLCM_U_RING_OFF						= 0x12,		 // added 2013.4

	
} ENUM_LCM_CMD_U,*pENUM_LCM_CMD_U;



KEKDISP_EXT		enum				 // 与 SetRegParaSCR数组相对应
{
	eSET_PARA_PWD,
	eSET_PARA_MAIN_IP,
	eSET_PARA_BAK_IP,
	eSET_PARA_TCP,
	eSET_PARA_UDP,
	eSET_PARA_PRIVINCE_ID,
	eSET_PARA_PREFECTURE_ID,
	eSET_PARA_CAR_NUM,
	eSET_PARA_CAR_COLOR,
	eSET_PARA_VIN_NUM,
	eSET_PARA_CAR_TYPE,
	eSET_PARA_CAR_PHONE_NUM,
	eSET_PARA_OWNER_PHONE_NUM,
	eSET_PARA_CENTER_NUM,
	eSET_PARA_K_VALUE,
	eSET_PARA_UPLOAD_INTERVAL_TIME,
	eSET_PARA_CREATE_USER,
	eSET_PARA_CANCER_REG,
	eSET_PARA_FACTORY_SET,
	eSET_PARA_REMOTE_UPDATE,
	eSET_PARA_MOD_PWD,
	eSET_gghypt,
}	ENUM_SET_PARA;

#define		LEN_GPS_DATA_TO_LCM			32

#define		LEN_CMD_DATA_TO_LCM		   252



#define			LCM_FRAME_HEAD_YH			0
#define			LCM_FRAME_CMD_YH			1
#define			LCM_FRAME_LENGTH_YH		    2
#define			LCM_FRAME_DATA_YH			4
#define			LCM_PHONE_NUM_LEN_YH		20


#pragma		pack(1)
typedef		struct
{
	uchar		Head;
	uchar		Cmd;
	uchar		Len;
	uchar		Time[3];
	uchar		Lat[4];
	uchar		Lon[4];
	uchar		Speed[2];
	uchar		Dir[2];
	uchar		GpsStatus;
	uchar		TypeStatus;
	uchar		OverSpeed;
	uchar		Rsv[2];
	uchar		CSQ;
	uchar		ACC;
	uchar		WatchStatus;
	uchar		Date[3];
	uchar		TcpOrUdp;
	//uchar		M2M;	
	uchar		Xor;
	uchar		End;
} GPS_DATA_TO_LCM,*pGPS_DATA_TO_LCM;


// 如果Times>Counter，则单声响，否则多声响
#define    	SILENT                0
#define    	PULL_CARD_BEEP        6
#define    	DRIVER_CARD_BEEP      5
#define    	SHORT_BEEP            5
#define	   BAD_CARD_BEEP         4
#define    	INIT_CARD_BEEP        3       // 插入初始卡时的响声
#define    	OVER_SPEED_BEEP       2
#define    	RICH_CARD_BEEP        1
#define    	MOVE_DATA_FINISH      1
#define    	BEEP(Type,Times,Counter,Silent,Raze)   {  Beep.type=Type;   Beep.times=Times; Beep.counter=Counter;  Beep.silent=Silent; Beep.BeepRaze=Raze;}

#pragma		pack(1)
 
typedef	 struct   
{
	unsigned char	  Rsv;
	unsigned short    AllOff;
   unsigned short    Status[LED_TOTAL_NUMBER];
  
} LAMP,*pLAMP;


////////////// 定义菜单序号 ///////////////////////////////////
typedef		enum
{
	SYS_INIT_MENU = 0,	
	///////////////////// 第1主菜单
	MAIN_MENU,	
	VEHICLE_CC_MENU,		  // 脉冲系数
	OVER_SPEED_MENU,		  //超速
	PARA_SETUP_MENU,		  //参数设置
	DRIVER_INFO_MENU,		  //驾驶员信息				  
	Car_MSG_MENU,	   	  //车辆信息
   Milage_MENU,			  //里程
	///////////////////// 
	AVG_SPEED_MENU,		  //平均速度	 15分钟
	PRINT_MENU,				  //打印
	OVER_TIME_MENU,		  //疲劳驾驶
	Mobile_PARA_MENU,  	  //无线通信参数
	Schedul_MSG_MENU,		  //调度信息
	Phone_Book_MENU,  	  //电话簿
	Ctr_MENU,              //终端控制菜单
	SET_PARA_MENU,				// 参数设置
	Product_MSG_MENU,		  //产品版本信息
	REG_INFO_MENU,				// 
	NotAllowChangeDriver_MENU,      //不允许更换驾驶员提示
	DonotRemoveDiskSCR_MENU,   //U盘正在采集数据提示 16
	/////////////////////  	
	SYELE_MENU		 =	18,
	/////////////////////
	DEFAULT_DISP    = 20,

	Set_Load_OK_Menu,
	Set_Fales_Menu,
	Comfir_Logout_MENU,
	Logouting_MENU,       
	Logout_OK_MENU,
	Logout_Fales_MENU,
	PRINT_Comfir_MENU,
	CONFIRM_MENU,
	Logout_Fales_MENU1,								  //
	PRINT_NO_PAPER_MENU,
	UPDATE_INDICATE,                     //升级指示
	INPUT_MENU,
	SET_PARA_SUB_MENU,			// 参数二级菜单
	/////////////////
} MENU,*pMENU;




typedef   struct
{
	uchar			PasswordOK;
	uchar			ExtControl;
	uchar			MainMenuCnt;
	uchar			SubMenuCnt;
	uchar			PageCnt;		
	uchar			SetParaFlag;
	uchar			MaxLine;
	uchar       Offset;
	uchar			MaxOffset;
	uchar			PreOffset;
	uchar			InputPageCnt;
	uchar			InputStatus;
	uchar			InputAlert;		//  需要在输入菜单中，显示提示的标志
	uchar			InputModifyPwdFlag;
	uchar			SubSubMenuCnt;
} LED;

KEKDISP_EXT 	LED   uLED;			


KEKDISP_EXT		enum	
{
	INPUT_INIT_STATUS,
	INPUT_ONE_CHAR,
	INPUT_FINISH_ALL_CHAR,

	
}InputEnum;



KEKDISP_EXT		enum	
{
	//SET_PARA_INPUT_NUM,				   // 仅有0 - 9
	SET_PARA_INPUT_NUM_AND_DOT,		// 0 - 9  和 点号 "."
	//SET_PARA_INPUT_NUM_AND_LETTER,	// 0 - 9 和 A - Z
	SET_PARA_INPUT_NEED_FOR_COLOR,	// 车辆颜色所需的字符：	蓝黄绿白黑
	SET_PARA_INPUT_NEED_FOR_CAR_NUM, // 车牌所需的字符：0 - 9, A - Z,  京沪津渝浙皖闽赣港鲁澳豫蒙鄂新湘宁粤藏琼桂川冀贵晋云辽陕吉甘黑青苏台

	
}SetparaInputEnum;

typedef		struct
{
	union 
	{
	 uint32    KEY32;
	 struct
	 {
	 uchar   PreKeyDown;
    uchar	PreKeyPress;
	 uchar   PreKeyUp;
	 uchar   PreKeyCode;
	 }Psta;
	}U32;
uint16   LongTimeNoKey,KeyPressTime;

}KEY_RM,pKEY_RM;

KEKDISP_EXT 	KEY_RM   sKEY_RM;


typedef	 struct
{
   uchar    type;
   uchar    times;
	uchar    counter;
	uchar    silent;
	uchar    BeepRaze;
} Beepz;

KEKDISP_EXT   		Beepz        Beep;



#define    	KEY_TOTAL_ROW         2       // 总行数
#define    	KEY_TOTAL_COLUMN      5       // 总列数


#define		LEN_INPUT_BUFFER	 	18

static	uint16	uiSpeedCNT;


static	uint32	twinkle, uiSpeedSUM;
static	uchar		TwinkleFlag;
static	uchar		InitDispCNT;
KEKDISP_EXT 	uint32   OVT_AddrTab[20];
static	uchar		DispOVT_CNT;
static   uchar	   OLED_Buffer[4][17];	// 定义32字节(一个画面)OLED缓冲 
static	uchar    BuzzerStatus;

KEKDISP_EXT uchar  PrintPaperStu;  	  // 0: 缺纸		1：有纸

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	KeyDisp类
*/
class			KeyDispCls
{
public:
	KeyDispCls(void);
	void		Init(void);
	void     InitKeyboard(void);
	void		KeyDisp(void);		
	void		SetLED_Status(uchar no,uchar Data);
  	void		ReceiveProcessData(void);	
	uchar		WaitLcmDataToServer(uint16);
	void		DispProcee(void);
	void		DispScreen(uchar DD);
	uchar    	GetOVT_Addr(uchar HowLong, uint32 *PAddr);
	//void		SendDataToLCM(uchar Cmd,uint16 Len,uchar *pData);
	void		SendDataToLCM_YH(uchar Cmd,uint16 Len,uchar *pData);
	void		SendHeartDataToLCM_YH(void);
	void		CenterSendDataToLCM_YH(uint16 Len,uchar *pData);
	void		TPMS_ReceiveProcessData(uint16 RecLen,uchar *pRecData);
	//void		SendGpsDataToLCM(void);
	void		SendGSMSignalToLCM(void);
	//void		BSJ_ReceiveProcessData(void);
  	void		YH_ReceiveProcessData(void);
	void     	LED_Buzzer(void);
	uchar		CustomCmdProcess(void);
	void		SystemSelfTestDisp(void);
	uint32		CalZiMo_Addr(uint16   cCode);
	KEY_RM   KEY_DL; 
private:
	
	uint32	DispCnt;
	uint16	NoDriverCnt;
	LAMP   	Lamp;
	uchar		DriverLogStatus;
	uchar		DispRecBuffer[256];
	uchar 	SendDataToLCM_Busing;
	uchar		SetParaBuffer[LEN_INPUT_BUFFER];
	////////////////////////////
	//uint32		CalZiMo_Addr(uint16   cCode);
	void        GetGBDotCode(uchar ASorGB, uchar* DotBuffer, uint32 pCode);
	void        GetASCIIDotCode(uchar t,uchar* DotBuffer, uchar const *pASCIICode);
	void       	KeyProcess(void);
	void       	KeyMainMenu(void);
	void	   	KeyPageDown(void);
   void       	KeyPageUp(void);
   void       	KeyConfirm(void);
   void       	KeyPrint(void);
//	void		eTaskLED(void);
	void		TurnOffLed(uchar);
	void		TurnOnLed(uchar);	
	uchar 	IsInputFinish(uchar ParaFlag);
	void     InputAlertUser(char *pMsg, uchar MsgLen);
	
};


KEKDISP_EXT		__noinit__		KeyDispCls		KeyDisp;

#endif

