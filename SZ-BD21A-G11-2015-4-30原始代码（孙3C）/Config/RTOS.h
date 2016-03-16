				  
#ifndef      __RTOS_H__
#define      __RTOS_H__

#ifdef      USER_OS_GLOBALS
#define     USER_OS_EXT
#else
#define     USER_OS_EXT    extern
#endif


/////////////////////////////////////////
#include    	"CPU.h"

#include			"UserConfig.h"

#include			"..\Common\UserGlobals.h"

#include			"..\Common\EEPROM_Addr.h"
//#include			"..\Common\Algol.h"





#define			OS_TICK_MS				10


USER_OS_EXT		enum
{	
	RESERVE		= 0,	
	TASK_MAIN ,			 		
	TASK_GPS,
	TASK_GSM_RECEIVE,	
	TASK_GPRS_SEND_PHONE,
	TASK_CAMERA,
	TASK_CAN_SEND,
	TASK_CAN_REC,
	TASK_SPEED,
	TASK_PRINT,
	TASK_USB,
	TASK_KEY_DISP,
	MAX_PRIORITY,				
}	TASK_Pri,*pTASK_Pri;



USER_OS_EXT		enum
{
	//OVER_TIME	= (1<<0),
	GPS_REC_OK_EVT_FLAG = (1<<1),
	RTC_ONE_SECOND_EVT_FLAG = 	(1<<2),
		
} MAIN_TASK_EVT_FLAG;



USER_OS_EXT		enum
{
	//OVER_TIME	= (1<<0),
	GPRS_REC_OK_EVT_FLAG = (1<<1),
	MOBILE_RET_EVT_FLAG  = (1<<2),
	UART1_RECEIVE_CMD_OK_EVT_FLAG = (1<<3),

		
} GSM_GPRS_EVT_FLAG;

USER_OS_EXT		enum
{
	//OVER_TIME	= (1<<0),
	UART0_RECEIVE_OK_EVT_FLAG = (1<<1),
	UART0_TRANSMIT_OK_EVT_FLAG = (1<<2),
	UART0_PRINT_EVT_FLAG = (1<<3),
		
} UART0_EVT_FLAG;

typedef		struct	__TASK_ID
{
	uint32	OS_Init;
	uint32	MainTask;		
	uint32	GPS;		
	uint32	GSM;
	uint32	PhoneCall;	
	uint32	Camera;
	uint32	Print;
	uint32	USB;
	uint32	KeyDisp;
	uint32	Speed;
	uint32   Send_CAN;
	uint32  	Rece_CAN;
	uint32	MAX_ID;
}	TASK_ID,*pTASK_ID;

USER_OS_EXT		TASK_ID	TaskID;


typedef		struct	__USER_EVT_FLAG
{
	uchar		OS_Init;
	uchar		MainTask;
	uchar		KeyDisp;
	uchar		UART0;		
	uchar		IIC;
	uchar		GPS;
	uchar		GSM_GPRS_Transmit;
	uchar		GSM_GPRS_Receive;
	uchar		Test;
	uchar		MAX_ID;
}	USER_EVT_FLAG,*pUSER_EVT_FLAG;

USER_OS_EXT		USER_EVT_FLAG		UserEvtFlag;
USER_OS_EXT		USER_EVT_FLAG		TaskStop;


#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
