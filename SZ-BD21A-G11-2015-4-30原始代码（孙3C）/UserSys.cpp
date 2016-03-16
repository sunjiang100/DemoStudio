/*----------------------------------------------------------------------------
 *      R T L   K e r n e l   E x a m p l e
 *----------------------------------------------------------------------------
 *      Name:    BLINKY.C
 *      Purpose: RTX example program
 *      Rev.:    V3.03a 
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c)  KEIL - An ARM Company. All rights reserved. 
 *---------------------------------------------------------------------------*/
#define      CONST_GLOBALS   1	 			   
// Program Size: Code=173840 RO-data=3828 RW-data=1052 ZI-data=36556  
// Program Size: Code=169116 RO-data=3824 RW-data=1048 ZI-data=36552  
//#include 		"config.h"
#include		<string.h>			   
//#include		"CAN\can.h"		 
#include		"UserSys.h"			 
#include			<stdlib.h>						  
#include			<string.h>

#include		"Common\UserGlobals.h"
#include		"Config\RTOS.h"
#include		"Config\UserConfig.h"
#include		"Switch\switch.h"
#include		"Common\CppGlobals.h"
#include 	"Common\flash_addr.h"
#include		"UART\UART0.h"
#include		"UART\UART1.h"		   
#include		"UART\UART2.h"
#include		"UART\UART3.h"
#include		"ADC\ADC.h"
#include		"RTC\RTC.h"
//#include	"IIC\IIC0.h" 
#include		"SPI\SPI.h"
#include		"CAN\CAN.h"
#include		"FLASH\DFE_25xxx.H"
#include		"FLASH\DFF_25xxx.H"
#include		"SPI\SSP.h"
#include		".\KeyDisp\LEDe308.h"
#include		".\Speed\Pulse.h"
#include		".\Speed\Speed.h"
#include		".\IIC\iic0.h"
#include		".\Common\GB_T19056.h"
#include		".\Print\Print.h"

#include		"PWM\PWM.h"
#include		"Switch\switch.h"
//#include	"CAN\TaskCAN.h"
//#include		"camera\camera.h"

//#include		"GPS\GPS.h"	 
#include		"Timer\Timer2.h"
#include		"Common\EEPROM_Addr.h"
//#include		"GSM_GPRS\CmdPrototype.h"
#include		"GSM_GPRS\GSM_GPRS.h"
#include		"KeyDisp\KeyDisp.h"
#include		"Common\Algol.h"
#include		"GPS\GPS.h"
//#include		<string.h>	

#include 	".\Accelerometer\kionix_kxtf9_common.h"	
 
#include		".\Common\ConstCode.h"
#include		".\GSM_GPRS\JT_T808.h"

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	系统类定义及任务函数原型声明
*/ 	
SystemCls		Sys;


#define		ENABLE_IDLE_MODE_PROG			0

#define		USE_FIRST_POWER_ON				1				// 逻辑真: 初次上电设置初始值，逻辑假：不设置

// 注意： 各家公司产品，统一在ConstCode.h中定义不同公司要编译的



#define        constOEM_ID  ((uchar *)(0x2000+0x00a0))




		 									
// Fcclk定义在UserConfig.h文件里
const   unsigned long  	PeriClock[4] = {Fcclk/4L,Fcclk,Fcclk/2L,Fcclk/8L};

// 	"CMNET\0"		"Gzjtyy.gd\0"	 "gzjtxx03.gd\0"  
const		char		constAPN[20]	= {"CMNET\0"};//{"1,\"gzjtxx03.gd\"\r\n\0"};


// 中国石化销售有限公司广东广州石油分公司

  

//const uchar    FirstVericc[] = {0x00, 0x00, 0x0E, 0x10};															//车辆特征系数
const uchar    FirstDriver[] = {"\x01\x00\x00\x00XXXXXXXXXXXXXXXXXX\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"};	//驾驶员信息
const	char		GB_Version[] __attribute__((at(0x00004400)))	= {"\x12\x00\0\0"};	            //执行的标准版本号和修改单号
const	char		JT_Version[] __attribute__((at(0x00004410)))	= {"JT/T808-2013"};	            //执行的标准版本号和修改单号


//const uchar    FirstCarMSG[] = {"GUOYEGPS201134567检A.168888\0\0大型客车\0\0\0\0\x01\0"};				//车辆信息，车辆颜色  // 检测使用
const uchar    FirstCarMSG[] = {"检A.168888\0\0X0000000000000000大型客车\0\0\0\0\x01\0"};	  // 车牌 12，VIN 17, 车辆分类 12






#if (PRODUCT_TYPE==SHANGHAI_SIZENG_PRT)
const	char		JT_ProductorID[]	__attribute__((at(0x00004570)))	 ={"70517\0"};	
const	char		DefaultSIMNumber[] __attribute__((at(0x00004540)))	= {"015921411030\0\0\0"};	 
const	char		DefaultCenterNumber1[] __attribute__((at(0x00004530)))	= {"15921411030\0\0\0\0"};
#elif (PRODUCT_TYPE==GUOYEE_PRT)
const	char		JT_ProductorID[]	__attribute__((at(0x00004570)))	 ={"70803\0"};			 
const	char		DefaultSIMNumber[] __attribute__((at(0x00004540)))	= {"018026276826\0\0\0"};
const	char		DefaultCenterNumber1[] __attribute__((at(0x00004530)))	= {"18026276826\0\0\0\0"};
#elif (PRODUCT_TYPE==GENERAL_PRT)
const	char		JT_ProductorID[]	__attribute__((at(0x00004570)))	 ={"00000\0"};
const	char		DefaultSIMNumber[] __attribute__((at(0x00004540)))	= {"013800200500\0\0\0"};
const	char		DefaultCenterNumber1[] __attribute__((at(0x00004530)))	= {"13800200500\0\0\0\0"};
#endif






/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：MainTask
   功能描述: 主任务函数
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：				   
*/ 
#define		LEN_TASK_GSM_STACK			2500	 //2500	216286 32946 1224 62096
#define		LEN_TASK_GPS_STACK			2500
#define		LEN_TASK_CAMERA_STACK      1024
#define		LEN_TASK_GPRS_SEND_STACK	2048


uchar			TaskGSM_Stack[LEN_TASK_GSM_STACK] __attribute__((at(0x10002000))) ;		
uchar			TaskGPS_Stack[LEN_TASK_GPS_STACK]; //__attribute__((at(0x10003000))) ;
uchar      	TaskPrintUSB_stack[2048];
uchar      	TaskKeyDisp_stack[1548];												
uchar      	TaskSpeed_stack[1024];
uchar		  	TaskCamera_stack[LEN_TASK_CAMERA_STACK];
uchar			TaskGPRS_SendStack[LEN_TASK_GPRS_SEND_STACK];


void		IdleTask(void);
__task	void		TaskGPS(void);
__task	void		TaskGSM_GPRS_Receive(void);
__task	void		TaskSpeed(void);
__task	void		TaskGPRS_SendPhoneCall(void);
__task	void		TaskCamera(void);
__task	void		task_send_CAN(void);
__task	void		task_rece_CAN(void);
__task	void		TaskKeyDisp(void);
__task	void        TaskPrint(void);
extern	"C"  __task	 void		tTaskUSB(void);
extern	uchar 	Connect_Create_USer_Center_Or_Main_Center(uchar Type, uchar Wait);

extern	uint32	CanBusRecTickCnt;

void Reset_Watch_Dog(void)
{
	DisableIRQ();LPC_WDT->WDFEED = 0xAA; LPC_WDT->WDFEED = 0x55;EnableIRQ();\
	LPC_GPIO2->FIODIR |= (1UL<<10); 	NOP();   NOP();\
	LPC_GPIO2->FIOCLR |= (1UL<<10); 	NOP();	NOP();\
	LPC_GPIO2->FIOSET |= (1UL<<10); 	NOP();	NOP();\
	LPC_GPIO2->FIOCLR |= (1UL<<10); 	NOP();	NOP();\
	LPC_GPIO2->FIOSET |= (1UL<<10); 	NOP();	NOP();
                                
}


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
__task		void 		MainTask(void) 	
{
	static   uchar SleepFlag = FALSE;			// FALSE : 不在睡眠
	uchar		tBuffer[64];
	uchar		*pD;
	uint32	i,tl;
	static	uint32	volatile		flag,MainTaskCnt=0,LastTime=0;  
	

	NOP();	NOP();	NOP();	NOP();
	
	//RFID_POWER_OFF;
	//#endif
	PRINT_POWER_OFF;
	//HC595_MR2_HIGH;
	NOP();NOP();NOP();
	SetBufferValue((uchar *)&SystemLog, LEN_SYSTEM_LOG, 0);
	Sys.CheckResetSource();
	//BUZZER_ON;
	for(i=0;i<1;i++)
	{
		RESET_WATCH_DOG;
		os_dly_wait(50);
	}
	BUZZER_OFF;	
	
	RFID_POWER_OFF;
	for(i=0;i<3;i++)
	{
		RESET_WATCH_DOG;
		os_dly_wait(50);
	}
	
	// 初始化监控任务计数器
	TC.MainTask = 0;
	TC.GPS = 0;	
	TC.GSM = 0;		     
	TC.GG_Rec = 0;
	TC.GG_Send = 0;
	TC.Camera = 0;
	TC.CANSend = 0;
	TC.CANRecv = 0;
	TC.KeyDisp = 0;
	TC.Speed = 0;
	TC.USB = 0;
	TC.Print = 0;	
	IC_Card_Auth_GPRS = 0;
	NOP();	NOP();	NOP();	NOP();
	TaskID.MainTask  = os_tsk_self ();  
	NOP();	NOP();	NOP();	NOP();
	#if(SYSTEM_BOOT_MESSAGE)
	Uart.puts("Main task start......\r\n\0");		Uart.PopAlignmentSend();
	os_dly_wait(5);
	#endif

	REGAIN_OIL_CONTROL;	  	// 启动后不自动锁车
	VehicleControl.Lock = LOCK_VEHICLE_DISABLE;	
	Sys.TestFunction();   

	RESET_WATCH_DOG;
	#if(SYSTEM_BOOT_MESSAGE)
	os_dly_wait(5);	
	#endif

	SSP0.Init();
	os_dly_wait(2);

	//HC595.Out1.Byte = 0xFF;          //0xE7;
	MOBILE_POWER_ON;
	//HC595.Out1.Bits.SwitchSIM = 0;   // 使用外部SIM卡		added 2013.4
	//HC595_OUTPUT;

	//HC595.Out2.Byte = 0xc0;
	//HC595_OUTPUT2;

	//RFID_POWER_ON;

	NOP();	NOP();  
	CAMERA_POWER_ON;
	#if(SYSTEM_BOOT_MESSAGE)
	Uart.puts("Init driver\r\n\0");		Uart.PopAlignmentSend();
	os_dly_wait(5);
	#endif
	NOP();	NOP();
	Sys.Init();
	NOP();	NOP();	NOP();	NOP();

	RFID_POWER_ON;
	

	#if(SYSTEM_BOOT_MESSAGE)
	Uart.puts("Load init para\r\n\0");		Uart.PopAlignmentSend();
	os_dly_wait(5);
	#endif
	Sys.LoadInitValue();
 	Uart.PopAlignmentSend();
	RESET_WATCH_DOG;
	#if(SYSTEM_BOOT_MESSAGE)
	Uart.puts("Read ID IP\r\n\0");		Uart.PopAlignmentSend();				  
	os_dly_wait(5);
	#endif
	RESET_WATCH_DOG;	
	Sys.ReadID_IP();	
	RESET_WATCH_DOG;
	
	

	NOP();	NOP();		
	Uart.PopAlignmentSend();		
	os_dly_wait(10);
	//Uart.puts("Init Counter Test Data:");
	Uart.puts("\r\n");
	Uart.putb(1,16,(uchar*)&gKey);
	Uart.puts("\r\n");
	//Uart.puts("\r\n");
	Uart.PopAlignmentSend();
	os_dly_wait(5);
	NOP();	NOP();
	RESET_WATCH_DOG;
	USBUseFlag = 0;
	USBTaskCreat = 0;
	PrintTaskCreat = 0;
	JT_ShutCommunication = 0;
	JT_ShutMobileCnt = 0;
	JT_Ctr_PowerDown = 0;
	GetGB_Data_Flag = 0;
	PrintBusy=FALSE;

	Creat_User_Reging_OverTime= 0;
	Creat_User_Reging = FALSE;

	#if(DEFAULT_POWER_ON_RFID)	   
	Rfid_Power_On = TRUE; // 	  由于RFID的电不开，会影响I2C总线，所以要一直开着电
	#else
    Rfid_Power_On = FALSE;
	#endif

	RESET_WATCH_DOG;

	#if(SIMU)
	if(SimuSpeed.Enable==ENABLE_FLAG)
	{
	  	 //LPC_UART0->THR = 'B';
	    Pwm.SetTargetSpeed(60,60);
		Sys.PowerControl(PCONP_PWM1,1);
	}	 
	#endif
	//LPC_UART0->THR = 'C';
	Sys.OemTest();
	//LPC_UART0->THR = 'D';

	//GSM.GetModelIMEIForPrint('R');		  // 输出IMEI号和SIM卡串号		
	//RESET_WATCH_DOG;

	////////////////////////////////////////////////////////		
	NOP();	NOP();	
	RESET_WATCH_DOG;
	TaskID.GPS = os_tsk_create_user(TaskGPS,TASK_GPS,&TaskGPS_Stack,LEN_TASK_GPS_STACK);
	#if(SYSTEM_BOOT_MESSAGE)
	Uart.puts("Create 'GPS' task\r\n\0");		Uart.PopAlignmentSend();
	os_dly_wait(5);
	#endif
	////////////////////////////////////////////////////////		
	NOP();	NOP();
	TaskID.GSM = os_tsk_create_user(TaskGSM_GPRS_Receive,TASK_GSM_RECEIVE,&TaskGSM_Stack,LEN_TASK_GSM_STACK);
	#if(SYSTEM_BOOT_MESSAGE)
	Uart.puts("Create 'GSM' task\r\n\0");		Uart.PopAlignmentSend();
	os_dly_wait(5);
	#endif
	////////////////////////////////////////////////////////		
	NOP();	NOP();
	RESET_WATCH_DOG;
	TaskID.Camera = os_tsk_create_user(TaskCamera,TASK_CAMERA,&TaskCamera_stack, LEN_TASK_CAMERA_STACK);
	#if(SYSTEM_BOOT_MESSAGE)
	Uart.puts("Create 'Camera' task\r\n\0");		Uart.PopAlignmentSend();
	os_dly_wait(5);
	#endif
	////////////////////////////////////////////////////////		
	NOP();	NOP();
	TaskID.PhoneCall = os_tsk_create_user(TaskGPRS_SendPhoneCall,TASK_GPRS_SEND_PHONE,&TaskGPRS_SendStack,LEN_TASK_GPRS_SEND_STACK);
	#if(SYSTEM_BOOT_MESSAGE)
	Uart.puts("Create 'PhoneCall' task\r\n\0");		Uart.PopAlignmentSend();
	os_dly_wait(5);
	#endif
	////////////////////////////////////////////////////////		
	NOP();	NOP();
	RESET_WATCH_DOG;	
	TaskID.Send_CAN = os_tsk_create (task_send_CAN, TASK_CAN_SEND);  			 /* Start          transmit task        */
	#if(SYSTEM_BOOT_MESSAGE)
	Uart.puts("Create 'CanSend' task\r\n\0");		Uart.PopAlignmentSend();
	os_dly_wait(5);
	#endif
	RESET_WATCH_DOG;
  	//os_tsk_create_user (task_rece_CAN, TASK_CAN_REC, TaskCrec_stack, sizeof(TaskCrec_stack));  			 /* Start           receive task        */
	TaskID.Rece_CAN = os_tsk_create (task_rece_CAN, TASK_CAN_REC);  	
    #if(SYSTEM_BOOT_MESSAGE)
	Uart.puts("Create 'CanRec' task\r\n\0");		Uart.PopAlignmentSend();
	os_dly_wait(5);			
	#endif		
	////////////////////////////////////////////////////////		
	NOP();	NOP();	 
	RESET_WATCH_DOG;
	TaskID.KeyDisp = os_tsk_create_user (TaskKeyDisp,TASK_KEY_DISP,TaskKeyDisp_stack,sizeof(TaskKeyDisp_stack));
    #if(SYSTEM_BOOT_MESSAGE)
	Uart.puts("Create 'KeyDisp' task\r\n\0");		Uart.PopAlignmentSend();				
	os_dly_wait(5);
	#endif
	RESET_WATCH_DOG;
 /////////// add by wrc  ///////////////
	TaskID.Speed = os_tsk_create_user (TaskSpeed,TASK_SPEED,TaskSpeed_stack,sizeof(TaskSpeed_stack));	        //,TackSpeed_stack,sizeof(TackSpeed_stack)
	#if(SYSTEM_BOOT_MESSAGE)
	Uart.puts("Create 'Speed' task\r\n\0");		Uart.PopAlignmentSend();
	os_dly_wait(5);
	#endif
	RESET_WATCH_DOG;
	
	#if(SYSTEM_BOOT_MESSAGE)
	Uart.puts("Accelerometer init......\r\n\0");		Uart.PopAlignmentSend();	RESET_WATCH_DOG;		os_dly_wait(5);
	#endif
	Accelerometer_init(0);
	U0_Iap_Flag = 0;
	GG_Status.Flag.Bits.UpdateRemoteSet = 0;
	GB_LargeData.SearchFlag = 0;

	
	NOP();	NOP();				
	RESET_WATCH_DOG;
	CAMERA_POWER_ON;
	VehicleStatus.VS5.Bits.SystemStart = 1;
	IC_CARD_RESET_PIN_LOW;	// IC卡（4442）高电平复位
	Uart.puts("Main task running......\r\n\0");		Uart.PopAlignmentSend();
	NOP();	NOP();
//	LCM_POWER_ON;
	//BEEP(9,0,2,0,10);
	while(1)
	{
		
		NOP();	NOP();	
		osInitFlag = TRUE;
		MainTaskCnt++;
		TC.MainTask = 0;
		if(MainTaskCnt==1)
		{
			PRINT_HOT_OFF;// 关步进电机电源和LED背光
		}
		
		RESET_WATCH_DOG;
		os_dly_wait(1);
		NOP();	NOP();	NOP();	NOP();
		Uart.ReceiveProcessData();	
	   //if((MainTaskCnt%50)==0)		  //    if((MainTaskCnt%20)==0)
		if((MainTaskCnt%20)==0)
		{
			Uart.SendVDR_Data_08H_To_15H(0,0);	//

//			if ((Clear_GB_LargeData_TotalPack_OverTime)&&(Clear_GB_LargeData_TotalPack_OverTime--))
//			{
//				if (Clear_GB_LargeData_TotalPack_OverTime==0)
//				{ 
//					GB_LargeData.TotalPack = 0;
//				}
//
//			}
		}
		NOP();	NOP();	NOP();	NOP();
		Uart.PopAlignmentSend();
		NOP();	NOP();	NOP();	NOP();

		if(U0_Iap_Flag!=0)
		{
			U0_Iap_cnt--;
			if(U0_Iap_cnt==0) 
			{
			   U0_Iap_Flag = 0;
			   Uart.Init(UART0_BPS);
				os_dly_wait(1);
				#if(SYSTEM_BOOT_MESSAGE)
				Uart.puts("Set  UART0_BPS  38400\r\n\0");	
		      #endif
			}
			U0_IapOverTime++;
			if( U0_IapOverTime > (20*100) )
			{
				NVIC_SystemReset();	 //				//超时
			}
	   }

		if ( (GG_Status.Flag.Bits.UpdateRemoteSet == 1)||(U0_Iap_Flag!=0))   continue; // 远程升级

		if((CamererTaskNow==0)&&(JT_ShutCommunication==0))			 
		{
		   if((USBTaskCreat==0)&&(PrintTaskCreat==0)&&((GET_USB_STATUS&0x01)==1))
			{
				if(1)
				//if((USB_CONNECT1&0x01))
			   {

				IDEL_Dly_Cnt = 0;
		      IDEL_For_Acc_OFF = FALSE;   //插入USB会唤醒休眠
				BEEP(9,0,2,0,4);
		      os_dly_wait(4);			  	 //这里延时太久会出现U盘初始化不成功
				NOP(); NOP();  NOP();
				TaskID.USB = os_tsk_create_user (tTaskUSB,TASK_USB,TaskPrintUSB_stack,sizeof(TaskPrintUSB_stack));
	         USBTaskCreat = 1;
				//#if(SYSTEM_BOOT_MESSAGE)
				Uart.puts("Create 'TaskUSB' task\r\n\0");		Uart.PopAlignmentSend();	  os_dly_wait(2);
		      //#endif
			   }
			} else if(((PrintTaskCreat==0)&&((GET_USB_STATUS&0x01)==0)&&(PrintEvent2==1))||	   								//PrintEvent2==1
			   ((TD.Display==tDISP_PRINT_TEST)||(TD.Display==tDISP_PRINT_MOTOR_FORWARD)||	
				(TD.Display==tDISP_PRINT_MOTOR_BACK)||(TD.Display==tDISP_PRINT_POWER_OFF)||	
				(TD.Display==tDISP_PRINT_LATCH)))
			{
				if(1)
				{
					NOP(); NOP();  NOP();
					TaskID.Print = os_tsk_create_user (TaskPrint,TASK_PRINT,TaskPrintUSB_stack,sizeof(TaskPrintUSB_stack));
					PrintTaskCreat=1;
					//#if(SYSTEM_BOOT_MESSAGE)
					Uart.puts("Create 'TaskPrint' task\r\n\0");		Uart.PopAlignmentSend();	  os_dly_wait(2);
			      //	#endif
				}
			}
		}

		//Sys.TestFunction();  
//		if(gCnt.SystemSecond<(3*60))
//		{
//			POWER_SELF_LOCK_ON;	
//		}
//		else
//		{
//			if(PowerSelfLock)
//			{	POWER_SELF_LOCK_ON;	}
//			else
//			{	POWER_SELF_LOCK_OFF; }
//		}
			
	   if(GET_CTS)	
		{		  		 	
			GSM_Busing = TRUE;
		}
		else
		{
			GSM_Busing = FALSE;
		}

	  	#if(SIMU)		
		if((MainTaskCnt%50==0)&&(SimuSpeed.Enable==ENABLE_FLAG))	 
		{
		    Pwm.SimuSpeedProcess();					  //模拟车辆行驶
		    NOP();	NOP();
			if((Rtc.DT.Sec==0))
			{
				if(Rtc.DT.Min==0)
				   Pwm.SetTargetSpeed(60,60);   
				if(Rtc.DT.Min==5)
				   Pwm.SetTargetSpeed(0,60);
				if(Rtc.DT.Min==8)
				   Pwm.SetTargetSpeed(60,60);	   		 		 
			   if(Rtc.DT.Min==10)
				   Pwm.SetTargetSpeed(120,60);
				if(Rtc.DT.Min==12)
				   Pwm.SetTargetSpeed(0,60);
				if(Rtc.DT.Min==15)
				   Pwm.SetTargetSpeed(150,60);	    //超速一会
				if(Rtc.DT.Min==17)
				   Pwm.SetTargetSpeed(0,2);	       //撞车了
				if(Rtc.DT.Min==20)
				   Pwm.SetTargetSpeed(60,60);
				if(Rtc.DT.Min==25)
				   Pwm.SetTargetSpeed(0,60);
				if(Rtc.DT.Min==30)
				   Pwm.SetTargetSpeed(60,20);
				if(Rtc.DT.Min==35)
				   Pwm.SetTargetSpeed(0,20);	
				if(Rtc.DT.Min==37)
				   Pwm.SetTargetSpeed(60,20);			
				if((Rtc.DT.Min==40)&&((Rtc.DT.Hour==3)||(Rtc.DT.Hour==8)||(Rtc.DT.Hour==13)||(Rtc.DT.Hour==18)||(Rtc.DT.Hour==23)))					//一天产生4个疲劳驾驶
				   Pwm.SetTargetSpeed(0,20);					  
			}
			RESET_WATCH_DOG;
		}
	   #endif
	  
		// added 2013.4
		if (MainTaskCnt % 100 == 0)			// RFID模块电源控制	
		{
			if (Rfid_Power_On == TRUE) 	
			{ RFID_POWER_ON; }	
			else
			{ RFID_POWER_OFF;	}

			
		}
		
		/////////////////////////////////// Flash页地址，每分钟存储1次在铁电。系统有内部电池，无需每秒更新。
		if( (MainTaskCnt % (60*100))==0)			
		{
			DFE.Write(FM_FLASH_PAGE_CNT,LEN_FLASH_PAGE_CNT,(uchar*)&FlashPageCnt);
		}

		if (CreateUserFlag==NEED_CREATE_USER)		 // 对快速开户时注册超时处理
		{
			if ((Creat_User_Reging==TRUE))
			{
				Creat_User_Reging_OverTime++;
				if (Creat_User_Reging_OverTime>(100*15))
				{
					Creat_User_Reging_OverTime= 0;
					Creat_User_Reging = FALSE;
					tBuffer[0] = 0x01;   // 注册失败
					KeyDisp.SendDataToLCM_YH(eLCM_D_CAR_REG_RESULT, 1, &tBuffer[0]); 
					os_dly_wait(2);
					Connect_Create_USer_Center_Or_Main_Center(CONNECT_MAIN_CENTER, 0);	 // 切换回主中心
				}

			}
		}

		CanBusRecTickCnt++; // CAN总线计数器，只能放这里，不能放CAN接收任务，否则计时相差很大
	
		

		
		NOP();	NOP();	NOP();	NOP();
		
		NOP();	NOP();	NOP();	NOP();
		Switch.eTask();  
		NOP();	NOP();	NOP();	NOP();
		
		if(MainTaskCnt%100==0)
		{
			if (SleepFlag == TRUE)				 // 如果之前状态是休眠
			{
				if (IDEL_For_Acc_OFF == FALSE)
				{
					SystemLog.ResetCnt.WakeUp++;
				}	
			}
			SleepFlag =	IDEL_For_Acc_OFF;  // 记住是否休眠标志
			Rtc.CheckWorkStatus();		   //监控任务
			if(TD.MainTask==tMAIN_SYS_SEC_CNT)
			{
				if(InRtcIntFlag==TRUE)
				{
					InRtcIntFlag = 0;
					strcpy((char*)tBuffer,"In Rtc: ");
					Int2Str(gCnt.SystemSecond,(char*)&tBuffer[8]);
				}
				else
				{
					strcpy((char*)tBuffer,"Rtc: ");
					Int2Str(gCnt.SystemSecond,(char*)&tBuffer[5]);
				}
				Uart.puts((char*)tBuffer);
			}
			
			
			if(JT_Ctr_PowerDown)					  //远程关机，关掉所有外设
			{
			  PRINT_HOT_OFF;
			  ALL_LED_OFF;			  
			
			 }
		
			 if( PrintEvent2==0)
			 {				 
					STEP_MOTOR_STOP;	
			 }
			 
			if ((GB_LargeData.OverTimeSearch)&&(GB_LargeData.OverTimeSearch--))
			{				 
				 if (GB_LargeData.OverTimeSearch==0)
				 {			  
				 	 GB_LargeData.SearchFlag = 0;
				 }
			}			
		}	
		
		if((MainTaskCnt%(30*100L)==0) || (SaveLogFlag == TRUE) && (GG_Status.Flag.Bits.UpdateRemoteSet ==0))		// 因为用铁电，30秒存一次，或者有需要时也存，如果用EEPROM应该30分钟存一次
		{
			if (SaveLogFlag == TRUE)	SaveLogFlag = FALSE;
			pD = (uchar *)&SystemLog;
			tl = EE_SYSTEM_LOG_BASE;
			for(i=0;i<SYSTEM_LOG_EE_PAGE;i++)
			{	
				NOP();NOP();NOP();						
				DFE.Write(tl,EE_PAGE_LEN,pD);
				tl += EE_PAGE_LEN;
				pD += EE_PAGE_LEN;
			}
			
				DFE.Write(EE_TEST_MILEAGE,8,(uchar*)&TestMileage);
		}


		if((TD.MainTask==tMAIN_TMP_TEST)&& (MainTaskCnt%(10*100L)==0))
		{
			Uart.puts("TTS TEST\r\n");
			GSM.TTS_Opreation(11,(uchar *)"TTS测试成功");
		} 
		
		//////////////////////////////////////

//		if(MainTaskCnt%100==0)
//		{	
//			if(TD.MainTask==tTASK_RUN_FLAG)
//			{
//				SetBufferValue((uchar*)tBuffer,64,0);
//				strcpy((char*)tBuffer,"Main Task.  1 2 3 4 5 6 7 8 9 A B C D E F\r\n\0");
//				tBuffer[12] = TaskID.MainTask+'0';
//				tBuffer[14] = TaskID.GPS+'0';
//				tBuffer[16] = TaskID.GSM+'0';
//				tBuffer[18] = TaskID.PhoneCall+'0';
//				tBuffer[20] = TaskID.Send_CAN+'0';
//				tBuffer[22] = TaskID.Rece_CAN+'0';
//				tBuffer[24] = TaskID.KeyDisp+'0';
//				tBuffer[26] = TaskID.Speed+'0';
//				tBuffer[28] = TaskID.USB+'0';
//				tBuffer[30] = TaskID.Print+'0';
//				
//				Uart.puts((char*)tBuffer);				
//			}
//		}
	}//End of while(1)
}//End of <MainTask> function


//void		GetVIC_IRQ_Number(void);


void			SystemCls::IdleMode(void)
{
	NOP();	NOP();
	//#if(SYSTEM_BOOT_MESSAGE)
	Uart.Init(UART0_BPS);
	Delay_mS(100);
	//CheckResetSource();
	//#endif
	#if(ENABLE_IDLE_MODE_PROG)
	uchar		i,t,*p;
	char 		TBuffer[16];
	///////////////////////////////////////
	NOP();	NOP();
	RESET_WATCH_DOG;
	DFE.Read(EE_IDLE_STR,LEN_EE_IDLE,(uchar*)&Idle);	
	
	Cnt.SystemSecond = 0;
	SystemSecondCntLast = 0;
	t = 0;	// 下次启动不进入省电模式（除非在满足条件的地方修改）
	DFE.Write(EE_IDLE_FLAG,1,(uchar *)&t);
		
	os_dly_wait(10);
	RESET_WATCH_DOG;
	Uart.Init(UART0_BPS);
	#if(SYSTEM_BOOT_MESSAGE)
	Delay_mS(100);
	#endif


	NOP();	NOP();
	CheckResetSource();
	#if(SYSTEM_BOOT_MESSAGE)
	Delay_mS(100);
	#endif
	

	//os_dly_wait(10);
	p = (uchar*)&Idle;
	TBuffer[0] = 'I';		TBuffer[1] = 'D';		TBuffer[2] = 'L';		TBuffer[3] = 'E';
	for(i=4;i<12;i++)
	{
		TBuffer[i] = *p;	p++;
	}
	TBuffer[12] = '\r';	TBuffer[13] = '\n';
	for(i=0;i<14;i++)
	{
		LPC_UART0->THR = TBuffer[i];
	}	
	
	#if(SYSTEM_BOOT_MESSAGE)
	RESET_WATCH_DOG;
	Delay_mS(100);
	RESET_WATCH_DOG;	
	Delay_mS(100);
	#endif
	if(Idle.IdleModeFlag==IDLE_MODE_FLAG)
	{
		strcpy((char*)TBuffer,"Idle mode...\r\n\0");
		for(i=0;i<14;i++)
		{
			LPC_UART0->THR = TBuffer[i];
		}
		RESET_WATCH_DOG;	
		Delay_mS(100);
		RESET_WATCH_DOG;	
		Delay_mS(100);
		RESET_WATCH_DOG;	
		Delay_mS(100);
		//os_dly_wait(5);
		
		NOP();	NOP();	
/*		PLLCON &= ~0x02;
		PLLFEED = 0xaa;
		PLLFEED = 0x55;		
		PLLCON &= ~0x01;
		PLLFEED = 0xaa;
		PLLFEED = 0x55;
		CLKSRCSEL = 0;
		PLLCFG  = 0;
		PLLFEED = 0xaa;
		PLLFEED = 0x55;
		PCLKSEL0 = 0x00;
		PCLKSEL1 = 0x00;		
		CCLKCFG = 0;				 	
		Sys.InitWatchDog(6000000);
		RESET_WATCH_DOG;			  		*/
		
		
		
		strcpy((char*)TBuffer,"Idle mode1..\r\n\0");
		for(i=0;i<14;i++)
		{
			LPC_UART0->THR = TBuffer[i];
		}
		RESET_WATCH_DOG;	
		Delay_mS(100);
		RESET_WATCH_DOG;	
		Delay_mS(100);
		RESET_WATCH_DOG;	
		Delay_mS(100);				
		
				
		LPC_SC->PCONP =((1UL<<2)|(1UL<<PCONP_UART0));		
		LPC_GPIO0->FIODIR &= (~((1UL<<15)|(1UL<<17)|(1UL<<18)|(1UL<<19)|(1UL<<20)|(1UL<<21)));	  
		Timer2.Init(2000000-60000);

		NOP();	NOP();
		while(1)
		{
			//os_dly_wait(5);
			NOP();	NOP();
			LPC_SC->PCON = 0x01;
			POWER_SELF_LOCK_ON;			
			RESET_WATCH_DOG;
			Switch.WakeUp();
			//U0THR = 'W';
			//Switch.CheckRadioControl(1);			
		}
	}
	NOP();	NOP();
	#endif
	NOP();	NOP();
}

















											


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：INit
   功能描述: 系统初始化
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：	
*/ 	
__inline		void		SystemCls::Init(void)
{
	NOP();	NOP();	NOP();	NOP();

	
	REGAIN_OIL_CONTROL;	  	// 启动后不自动锁车
	VehicleControl.Lock = LOCK_VEHICLE_DISABLE;  			
	
	NOP();	NOP();
	RESET_WATCH_DOG;	
	NOP();	NOP();		  
	#if(SYSTEM_BOOT_MESSAGE)
	Uart.puts("I2C0 init......\r\n\0");		Uart.PopAlignmentSend();	RESET_WATCH_DOG;		os_dly_wait(5);
    #endif
	I2C0.Init();
	NOP();	NOP();
	RESET_WATCH_DOG;
	NOP();	NOP();
	#if(SYSTEM_BOOT_MESSAGE)
	Uart.puts("Rtc init......\r\n\0");		Uart.PopAlignmentSend();	RESET_WATCH_DOG;		os_dly_wait(5);
	#endif
	Rtc.Init(0);	
	
   
   #if(SYSTEM_BOOT_MESSAGE)
	Uart.puts("Time2 init......\r\n\0");		Uart.PopAlignmentSend();	RESET_WATCH_DOG;		os_dly_wait(5);
   #endif
	Timer2.Init(16000000);						
	NOP();	NOP();
	#if(SYSTEM_BOOT_MESSAGE)
	Uart.puts("TimeTIM0 init......\r\n\0");		Uart.PopAlignmentSend();	RESET_WATCH_DOG;		os_dly_wait(5);
	#endif
	Pulse.Timer0Init();
	NOP();	NOP();
	#if(SYSTEM_BOOT_MESSAGE)
	Uart.puts("TimeTIM1 init......\r\n\0");		Uart.PopAlignmentSend();	RESET_WATCH_DOG;		os_dly_wait(5);
	#endif
	Pulse.Timer1Init();
	NOP();	NOP();
	
	#if(SYSTEM_BOOT_MESSAGE)
	Uart.puts("TimeTIM3 init......\r\n\0");		Uart.PopAlignmentSend();	RESET_WATCH_DOG;		os_dly_wait(5);
	#endif
	Pulse.Timer3Init();
	NOP();	NOP();
    #if(SYSTEM_BOOT_MESSAGE)
	Uart.puts("TimeRIT init......\r\n\0");		Uart.PopAlignmentSend();	RESET_WATCH_DOG;		os_dly_wait(5);
	#endif
	Pulse.RitInit(100);
	NOP();	NOP();
	
	#if(SYSTEM_BOOT_MESSAGE)
	Uart.puts("SPI init......\r\n\0");		Uart.PopAlignmentSend();	RESET_WATCH_DOG;		os_dly_wait(5);
	#endif
	SPI0.Init();
	NOP();	NOP();
	SSP0.Init();
	NOP();	NOP();
	DFF.Init();
	NOP();	NOP();
	DFE.Init();
	NOP();	NOP();
	os_dly_wait(10);
	RESET_WATCH_DOG;
	
	NOP();	NOP();
	#if(SYSTEM_BOOT_MESSAGE)
	Uart.puts("Read baudrate......\r\n\0");		Uart.PopAlignmentSend();	RESET_WATCH_DOG;		os_dly_wait(5);	
	#endif
	DFE.Read(EE_BAUDRATE_U0_232,16,(uchar*)&Baudrate);
		
	Baudrate.U1_GSM = UART1_BPS;		// U1不可修改	
	//Baudrate.U3_GPS = UART3_BPS;		// U3不可修改
	DFE.Read(EE_BAUDRATE_U3_232,4,(uchar*)&Baudrate.U3_GPS);
	//Baudrate.CanBus = CAN_BPS;
	
	if( (Baudrate.U0_232!=9600L)&&(Baudrate.U0_232!=19200UL)&&(Baudrate.U0_232!=38400UL)&&\
		 (Baudrate.U0_232!=57600UL)&&(Baudrate.U0_232!=115200UL))
		 Baudrate.U0_232 = 115200UL;

	if( (Baudrate.U2_232!=9600UL)&&(Baudrate.U2_232!=19200UL)&&(Baudrate.U2_232!=38400UL)&&\
		 (Baudrate.U2_232!=57600UL)&&(Baudrate.U2_232!=115200UL))
		 Baudrate.U2_232 = 115200UL;
	
	if( (Baudrate.U2_485!=9600UL)&&(Baudrate.U2_485!=19200UL)&&(Baudrate.U2_485!=38400UL)&&\
		 (Baudrate.U2_485!=57600UL)&&(Baudrate.U2_485!=115200UL))
		 Baudrate.U2_485 = 57600UL;
		 
	if( (Baudrate.CanBus!=100000UL)&&(Baudrate.CanBus!=250000UL)&&(Baudrate.CanBus!=500000UL)&&\
		 (Baudrate.CanBus!=1000000UL) )
		 Baudrate.CanBus = 250000UL;
	// added 2013.5
	if((Baudrate.U3_GPS!=9600UL)&&(Baudrate.U3_GPS!=19200UL)&&(Baudrate.U3_GPS!=38400UL)&&\
	 (Baudrate.U3_GPS!=57600UL)&&(Baudrate.U3_GPS!=115200UL))
	 	Baudrate.U3_GPS = 9600;//115200UL;
	//
	

	#if(SYSTEM_BOOT_MESSAGE)
	Uart.puts("Uart0 init......\r\n\0");		Uart.PopAlignmentSend();	RESET_WATCH_DOG;		os_dly_wait(5);
	#endif
	Uart.Init(UART0_BPS);	//	Baudrate.U0_232);
	RESET_WATCH_DOG;
	NOP();	NOP();
	#if(SYSTEM_BOOT_MESSAGE)
	Uart.puts("Uart1 init......\r\n\0");		Uart.PopAlignmentSend();	RESET_WATCH_DOG;		os_dly_wait(5);
	#endif
	Uart1.Init(Baudrate.U1_GSM);
	RESET_WATCH_DOG;
	NOP();	NOP();
	//Uart.puts("Uart2 init......\r\n\0");		Uart.PopAlignmentSend();	RESET_WATCH_DOG;		os_dly_wait(5);
	//Uart2.Init(Baudrate.U2_485);
	NOP();	NOP();
	//os_dly_wait(5);
	RESET_WATCH_DOG;
	#if(SYSTEM_BOOT_MESSAGE)
	Uart.puts("Uart3 init......\r\n\0");		Uart.PopAlignmentSend();	RESET_WATCH_DOG;		os_dly_wait(5);
	#endif
	Uart3.Init(Baudrate.U3_GPS);
	NOP();	NOP();
	RESET_WATCH_DOG;
	#if(SYSTEM_BOOT_MESSAGE)
	Uart.puts("Can bus init......\r\n\0");		Uart.PopAlignmentSend();	RESET_WATCH_DOG;		os_dly_wait(5);
	#endif
	CanBus.Init(1,Baudrate.CanBus);
	NOP();	NOP();
	#if(SYSTEM_BOOT_MESSAGE)
	Uart.puts("Adc init......\r\n\0");			Uart.PopAlignmentSend();	RESET_WATCH_DOG;		os_dly_wait(5);
	#endif
	strncpy((char*)sOEM_ID,(char*)constOEM_ID, 16);		  // 终端ID 7字节

	Adc.Init();
	NOP();	NOP();
	/*#if(SYSTEM_BOOT_MESSAGE)
	Uart.puts("Accelerometer init......\r\n\0");		Uart.PopAlignmentSend();	RESET_WATCH_DOG;		os_dly_wait(5);
	#endif
	Accelerometer_init(0);
	NOP();	NOP();
	NOP();	NOP(); */

} 	// End of <SystemCls::Init> function




void			SystemCls::GetGYFlagToUart(void)
{
	NOP();  NOP();
	//Uart.SendGYFlag((uchar*)constGYFlag);
}


#define			tID_IP			0

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：ReadID_IP
   功能描述：
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
extern			char Create_User_Dns[];
void			SystemCls::ReadID_IP(void)
{	
	//uchar		Len;
	union
	{
		uint32	B32[256/4];
		uint16	B16[256/2];
		uchar		Buffer[256];
	} Tp;
 	
	SetBufferValue((uchar*)&sIP,(LEN_IP_PORT*4),0);
	//SetBufferValue((uchar*)sOEM_ID,LEN_OEM_ID,0);

	SetBufferValue((uchar*)&JT808_IP_DNS, LEN_JT808_IP_DNS, 0);
	SetBufferValue((uchar*)Tp.Buffer, 256, 0);
	DFE.Read(EE_ENABLE_IP,16,(uchar *)sIP.EnableFlag);								    //使能连接控制
	sIP.EnableFlag[eFLAG_TCP1] = 0; sIP.EnableFlag[eFLAG_UDP1] = 0;				 //开机默认先连接主服务器 ，主服务器连接不上才连接备份服务器

	//Uart.putb(1, 16, (uchar*)sIP.EnableFlag);		Uart.PopAlignmentSend();	 os_dly_wait(5);
	//sIP.EnableFlag[eFLAG_TCP]=ENABLE_FLAG; 

	// 读IP地址
	//DFE.Read(EE_IP_PORT,(LEN_IP_PORT*4),(uchar*)&sIP);
	//os_dly_wait(1);
	DFE.Read(EE_JT_MAIN_SERVER, LEN_JT808_IP_DNS, (uchar*)&JT808_IP_DNS);				 //加载主、备份服务器信息
	os_dly_wait(1);
	DFE.Read(EE_JT_NET_CTR, LEN_JT_CTR_IP_DNS, (uchar*)&JT808_Ctr_IP_DNS);				 //加载临时连接或升级服务器信息
	os_dly_wait(1);

   	// 如果主中心和货运中心(快速开户中心）一致，就需要加载临时IP到主中心,修正主中心和货运中心共用0通道的问题
	if (memcmp(Create_User_Dns, JT808_IP_DNS.M_IPorDNS, 10) == 0)	 
	{
		DFE.Read(EE_JT_TMP_TCP_IP, LEN_IPorDNS, JT808_IP_DNS.M_IPorDNS);
		NOP();NOP();NOP();
		DFE.Read(EE_JT_TMP_TCP_PORT, 4, (uchar *)&JT808_IP_DNS.TCP_Port);
		NOP(); 
		DFE.Write(EE_JT_M_IP_DNS, LEN_IPorDNS, JT808_IP_DNS.M_IPorDNS);
		NOP(); NOP(); NOP();
		DFE.Write(EE_JT_TCP_PORT, 4, (uchar *)&JT808_IP_DNS.TCP_Port);
		NOP(); NOP(); NOP();
	}
	//

	//sIP.TCP[31] = sIP.UDP[31] = sIP.TCP1[31] = sIP.UDP1[31] = 0;
	
	JT808_IP_DNS.M_IPorDNS[31] = JT808_IP_DNS.B_IPorDNS[31] = JT808_Ctr_IP_DNS.C_IPorDNS[31] = 0;	
	//////////////////////////////////////////// IP地址			
	//if(ValidIP_Port((uchar*)sIP.TCP)!=OK)		sIP.EnableFlag[eFLAG_TCP] = 0;
	//if(ValidIP_Port((uchar*)sIP.UDP)!=OK)		sIP.EnableFlag[eFLAG_UDP] = 0;
	//if(ValidIP_Port((uchar*)sIP.TCP1)!=OK)		sIP.EnableFlag[eFLAG_TCP1] = 0;
	//if(ValidIP_Port((uchar*)sIP.UDP1)!=OK)		sIP.EnableFlag[eFLAG_UDP1] = 0;
	
	Int2Str(JT808_IP_DNS.TCP_Port,(char*)&Tp.Buffer[0]);
	strcat((char *)JT808_IP_DNS.M_IPorDNS, ":");					 
    strcat((char *)JT808_IP_DNS.M_IPorDNS, (char *)&Tp.Buffer[0]);
	SetBufferValue((uchar*)Tp.Buffer, 10, 0);
	Int2Str(JT808_IP_DNS.UDP_Port,(char*)&Tp.Buffer[0]);
	strcat((char *)JT808_IP_DNS.B_IPorDNS, ":");	
	strcat((char *)JT808_IP_DNS.B_IPorDNS, (char *)&Tp.Buffer[0]);
	Int2Str(JT808_Ctr_IP_DNS.C_TCP_Port,(char*)&Tp.Buffer[0]);
	strcat((char *)JT808_Ctr_IP_DNS.C_IPorDNS, ":");
	strcat((char *)JT808_Ctr_IP_DNS.C_IPorDNS, (char *)&Tp.Buffer[0]);

	if(ValidIP_Port((uchar*)JT808_IP_DNS.M_IPorDNS)!=OK)		 {	sIP.EnableFlag[eFLAG_TCP] = 0; sIP.EnableFlag[eFLAG_UDP] = 0; }		  //JT808 IP地址或DNS
	if(ValidIP_Port((uchar*)JT808_IP_DNS.B_IPorDNS)!=OK)	    {	sIP.EnableFlag[eFLAG_TCP1] = 0; sIP.EnableFlag[eFLAG_UDP1] = 0; }
	if(ValidIP_Port((uchar*)JT808_Ctr_IP_DNS.C_IPorDNS)!=OK)	 {	sIP.EnableFlag[eFLAG_TCP2] = 0; sIP.EnableFlag[eFLAG_UDP2] = 0; }
 	if(JT808_IP_DNS.TCP_Port<1024)			                   { sIP.EnableFlag[eFLAG_TCP] = 0; sIP.EnableFlag[eFLAG_TCP1]= 0; }
	if(JT808_IP_DNS.UDP_Port<1024)			                   { sIP.EnableFlag[eFLAG_UDP] = 0; sIP.EnableFlag[eFLAG_UDP1] = 0; }

	DFE.Read(EE_JT_M_APN, LEN_IPorDNS, (uchar *)&JT808_IP_DNS.M_APN[0]);
	if( (JT808_IP_DNS.M_APN[0]==0)||(JT808_IP_DNS.M_APN[1]==0) )
	{
		strcpy((char *)&JT808_IP_DNS.M_APN[0], constAPN);
	}
	DFE.Read(EE_JT_B_APN, LEN_IPorDNS, (uchar *)&JT808_IP_DNS.B_APN[0]);
	if( (JT808_IP_DNS.B_APN[0]==0)||(JT808_IP_DNS.B_APN[1]==0) )
	{
		strcpy((char *)&JT808_IP_DNS.B_APN[0], constAPN);
	}
	
	sIP.Socket[0] = sIP.Socket[1] = sIP.Socket[2] = sIP.Socket[3] = sIP.Socket[4] = sIP.Socket[5] = 0xff;
	SPI0_Busy = 0;
	
	DFE.Read(EE_NEED_REG_SERVER,1,&NeedRegServer);	
	 


	DFE.Read(EE_CREATE_USER_FLAG, 1, &CreateUserFlag);					//
	NOP(); NOP();NOP();

	DFE.Read(EE_APN,16,sIP.APN);
	sIP.APN[16] = 0;


	/////JT808 IP相关信息


	////////////////////////////////////////////////////////////////// OEM_ID
	//strcpy((char*)sOEM_ID,(char*)constOEM_ID);	
	//strncpy((char*)sOEM_ID,(char*)constOEM_ID, 16);
	///////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////
	// 启动输出信息
	RESET_WATCH_DOG;	
	SetBufferValue(Tp.Buffer,128,0);
	strcpy((char *)Tp.Buffer,"\r\n\0");
	strcat((char *)Tp.Buffer,(char *)constModelVersion);
	strcat((char *)Tp.Buffer,"\r\n\0");
	strcat((char *)Tp.Buffer,(char *)CompileDate);
	strcat((char *)Tp.Buffer," ");
	strcat((char *)Tp.Buffer,(char *)CompileTime);
	strcat((char *)Tp.Buffer,"\r\n\0");
	Uart.puts((char*)Tp.Buffer);		
//	Uart2.putb(1,strlen((char*)Tp.Buffer),Tp.Buffer);
	os_dly_wait(10);
	RESET_WATCH_DOG;

	SetBufferValue(Tp.Buffer,128,0);
	strcpy((char *)Tp.Buffer,"Main IP: ");
	strcat((char *)Tp.Buffer,(char *)JT808_IP_DNS.M_IPorDNS);
	strcat((char *)Tp.Buffer,"\r\n\0");
	//Len = strlen((char*)Tp.Buffer);
	//DFE.Read(EE_DOMAIN_ADDR,32,(uchar*)&Tp.Buffer[Len]);
	strcat((char *)Tp.Buffer,"Back IP: ");
	strcat((char *)Tp.Buffer,(char *)JT808_IP_DNS.B_IPorDNS);
	//strcat((char *)Tp.Buffer,(char *)JT808_IP_DNS.M_IPorDNS);
	strcat((char *)Tp.Buffer,"\r\n\0");

	strcat((char *)Tp.Buffer,"M_APN: ");
	strcat((char *)Tp.Buffer,(char *)JT808_IP_DNS.M_APN);
	//strcat((char *)Tp.Buffer,(char *)JT808_IP_DNS.M_IPorDNS);
	strcat((char *)Tp.Buffer,"\r\n\0");

	strcat((char *)Tp.Buffer,"B_APN: ");
	strcat((char *)Tp.Buffer,(char *)JT808_IP_DNS.B_APN);
	//strcat((char *)Tp.Buffer,(char *)JT808_IP_DNS.M_IPorDNS);
	strcat((char *)Tp.Buffer,"\r\n\0");

	Uart.puts((char*)Tp.Buffer);		
//	Uart2.putb(1,strlen((char*)Tp.Buffer),Tp.Buffer);
	os_dly_wait(10);
	RESET_WATCH_DOG;

	 // 在读到GSM的序列号里有打印Device ID
	SetBufferValue(Tp.Buffer,128,0);
	strcpy((char *)Tp.Buffer,"Device ID: \0");
	strcat((char *)Tp.Buffer,(char*)sOEM_ID);
	strcat((char *)Tp.Buffer,"\r\n\0");
	Uart.puts((char*)Tp.Buffer);
	RESET_WATCH_DOG;
//	Uart2.putb(1,strlen((char*)Tp.Buffer),Tp.Buffer);
	
}





void			SystemCls::GetVersion(char *p)
{
	uchar    Len;
	uint32   CompTime;
	if ((U0_Iap_Flag != 0))
	{
		if (U0_Iap_Flag==9) // 特定查询
		{
			strncpy(p,(const char *)&constModelVersion[MV_POS_MODEL],LEN_MODEL); 
			strcat((char *)p , ",\0");
			Len = strlen(p);
			strncpy(p+Len,(const char *)&constModelVersion[MV_POS_HV-8],27);  // 硬件，软件版本 字符串
			strcat((char *)p , ",\0");
			strcat((char *)p , (char *)CompileDate);
			strcat((char *)p , " ");
			strcat((char *)p , (char *)CompileTime);
		}
		else
		{
			strcpy((char *)p , (char *)Model);	   // 型号
			strcat((char *)p , ",\0");
		   Len = strlen(p);
			Int32ToDecChar(HardVersion, p+Len);				// 硬件版本
			strcat((char *)p , ",\0");
			Len = strlen(p);
			Int32ToDecChar(SoftVersion, p+Len);				// 软件版本
			strcat((char *)p , ",\0");
			//Len = strlen(p);
			//Int32ToDecChar(CompilerDT, p+Len);				// 编译时间
			Len = strlen(p);
			CompTime = Rtc.KeilDateTimeToSecond((char *)CompileDate, (char *)CompileTime);
			Int32ToDecChar(CompTime, p+Len);			// 编译时间
			strcat((char *)p , "\0");	
		}
	
	}
	else 
	{														 
		strcpy(p,(char*)constModelVersion);
		strcat(p,",");
		strcat(p,(char*)CompileDate);
		strcat(p,",");
		strcat(p,(char*)CompileTime);
	}
}



void			SystemCls::GetHS_Version(char *p, uchar Type)
{
	uchar 	i;

	if (Type == 0)
	{
		strcat(p,CompileDate);
		strcat(p,",");
		strcpy(p,(char*)constHSVersion);
	}
	else	   // JT808
	{
		// 硬件版本号长度	   8
		i = 0;
		p[i++] = 5+3; 
		p[i++] =  ((HardVersion >> 8) & 0x0f) + '0';
		p[i++] = '.';
		p[i++] =  ((HardVersion >> 4) & 0x0f) + '0';
		p[i++] = '.';
		p[i++] =  ((HardVersion >> 0) & 0x0f) + '0';
		i += 3;
		
		// 软件版本
		p[i++] = 5+3;
		p[i++] =  ((SoftVersion >> 8) & 0x0f) + '0';
		p[i++] = '.';
		p[i++] =  ((SoftVersion >> 4) & 0x0f) + '0';
		p[i++] = '.';
		p[i++] =  ((SoftVersion >> 0) & 0x0f) + '0';
	}
}

void			SystemCls::GetGB_Version(char *p)
{
	strcpy(p,(char*)GB_Version);
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：ReadID
   功能描述：
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
/*
void			SystemCls::PowerControl(void)
{

	NOP();   NOP();	
	NOP();	NOP();
	//AD0CR &= ~(1UL<<PDN);		  rst = 0;

	// 外设功率控制
	LPC_SC->PCONP = 0;				
	#if(PCONP_TIMER0)		
		LPC_SC->PCONP |= (1UL<<PCONP_TIMER0);	
	#endif
	#if(PCONP_TIMER1)		
		LPC_SC->PCONP |= (1UL<<PCONP_TIMER1);	
	#endif
	#if(PCONP_UART0)		
		LPC_SC->PCONP |= (1UL<<PCONP_UART0);		
	#endif
	#if(PCONP_UART1)		
		LPC_SC->PCONP |= (1UL<<PCONP_UART1);		
	#endif
	#if(PCONP_PWM0)		
		LPC_SC->PCONP |= (1UL<<PCONP_PWM0);		
	#endif
	#if(PCONP_IIC0)		
		LPC_SC->PCONP |= (1UL<<PCONP_IIC0);		
	#endif
	#if(PCONP_IIC1)		
		LPC_SC->PCONP |= (1UL<<PCONP_IIC1);		
	#endif
	#if(PCONP_SPI0)		
		LPC_SC->PCONP |= (1UL<<PCONP_SPI0);		
	#endif
	#if(PCONP_RTC)			
		LPC_SC->PCONP |= (1UL<<PCONP_RTC);		
	#endif
	#if(PCONP_SSP)			
		LPC_SC->PCONP |= (1UL<<PCONP_SSP);		
	#endif
	#if(PCONP_ADC0)		
		LPC_SC->PCONP |= (1UL<<PCONP_ADC0);		
	#endif
	#if(PCONP_ADC1)		
		LPC_SC->PCONP |= (1UL<<PCONP_ADC1);		
	#endif
	#if(PCONP_UART2)		
		LPC_SC->PCONP |= (1UL<<PCONP_UART2);		
	#endif
	#if(PCONP_UART3)		
		LPC_SC->PCONP |= (1UL<<PCONP_UART3);		
	#endif

	#if(PCONP_CAN1)		
		LPC_SC->PCONP |= (1UL<<PCONP_CAN1);
	#endif

	#if(PCONP_RIT)		
		LPC_SC->PCONP |= (1UL<<PCONP_RIT);	
		
				
	#endif


}




											*/

/*
 * standard interrupt function saves only R0 - R12 and returns with 
 */
extern	"C"	void DefaultIRQ (void) __irq 
{
  	//VICVectAddr = 0;
}


/*************************************************************************************************
** 函数名称：LoadInitValue()
** 功能描述：装载系统初始值
** 入口参数：无
** 出口参数：无
** 全局变量: 无
** 修改时间：2009.09.02

****************************************************************************************************/ 	

const		char	FirstRunFlag[] = {"www.guoyee.com\r\0"};

__inline		void		SystemCls::LoadInitValue(void)
{
	uchar 	i,c,tmp,tBuffer[128];
	union
	{
		uchar		B8[8];
		uint16	B16[4];
		uint32	B32[2];
		unsigned  long long  B64;
	}Tp;

	//////////////////////////////////////////////////////////
	NOP();	NOP();	NOP();	NOP();
	#if( USE_FIRST_POWER_ON != 0)
	DFE.Read(EE_SYS_FIRST_RUN,16,(uchar*)tBuffer);
	os_dly_wait(10);
	c = 0;
	for(i=0;i<16;i++)
	{
		if(MyStrChar((char*)tBuffer,FirstRunFlag[i]))
			c++;
	}
	NOP();	NOP();	
	if(c<10)	  	// 认为是第一次上电运行
	{
		Uart.puts("System first run flag,Initializing,please waiting...\r\n\0");
		os_dly_wait(5);
		//FirstPowerOn(FALSE);					//
		FirstPowerOn(TRUE);	
		strcpy((char*)tBuffer,"www.guoyee.com\r\0");
		DFE.Write(EE_SYS_FIRST_RUN,16,(uchar*)tBuffer);
	}
	#else
	Uart.puts("No need first run falg.\r\n\0");
	#endif
	NOP();	NOP();
	MenuSetRemoteIAP = 0;
	GPS.Init(0);
	//DFE.Write(FM_SERIAL_NUMBER,16,(uchar *)constOEM_ID); 


	os_dly_wait(1);
	KeyDisp.Init();
	os_dly_wait(1);
	Switch.Init();
	os_dly_wait(1);	
	TD.MainTask		= 0;
	TD.GPS         = 0;
	TD.GPRS        = 0;
	TD.CAN		   = 0;
	TD.SW          = 0;
	TD.ADC         = 0;
	TD.Display     = 0;
	TD.Camera		= 0;
	gCnt.SystemSecond = 0;
	SystemSecondCntLast = 0;
	gCnt.InsertIC_Card = 0;
	gCnt.InsertTF_Card = 0;

	//HC595.Out2.Byte = 0xc0;			// HC595第二片输出控制全部关闭
	
	SamlePeriod1S = TestAccuracy = TestBlind = TestCanBus = ReadyTestBlind = TestRoadLine = 0;
	
	UpgradeFlag = 0;
	SPI0_Busy = 0;		
	
	InOrOutArea = 0xff;  
	
//	UDP_CommunicateFlag = 0;
	
	GpsTempSaveFlag = TRUE;
	HandleLoginSuccess = 1;

	Send_Msg_Record_Start = 0;
	Send_Msg_Record_Count = 0;
	
	
	DFE.Read(EE_ROUND_FENCE_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RoundFenceAttr);
	DFE.Read(EE_RECTANGLE_FENCE_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RectFenceAttr);
	DFE.Read(EE_POLYGON_FENCE_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&PolygonFenceAttr);
	DFE.Read(EE_ROAD_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RoadAttr);


	DFE.Read(FM_ICCardInOrOut_FLAG,1,&DriverCardFlag);	       //驾驶员身份卡标识
	
	DFE.Read(EE_ACCEL_ADD_DEC_PARA,LEN_EE_ACCEL_ADD_DEC_PARA,(uchar*)&AccelPara);
	Uart.puts("Accel Load Init Test Data:");
	Uart.putb(1,30,(uchar*)&AccelPara[0]);
	Uart.puts("\r\n\0");
		
	DFE.Read(EE_JT_RESEND_MACHINING,LEN_SEND_OVT_STRU,(uchar *)&SendOVT);			  //加载重传参数

	//Uart.puts("pSendInterval...\r\n\0");	Uart.PopAlignmentSend();	 os_dly_wait(5);

	DFE.Read(EE_SEND_INTERVAL_BASE,LEN_SEND_INTERVAL_STRU,(uchar *)&SendInterval);  //加载位置回报参数
	
	#if( TEST_CODE_JUST_FOR_TEST_LUOYANG == 1)
	SendInterval.IDEL_ModeTime = (5*60);
	#else
	NOP();	NOP();	//fsf saf
	#endif
	
	DFE.Read( FM_USE_GPS_MIL, 1, (uchar*)&UseGPS_Mil );
	if( UseGPS_Mil==0 )		Uart.puts(" 使用GPS累计里程 ");
	else				  			Uart.puts(" 使用脉冲累计里程 ");

	DFE.Read(EE_ENABLE_SIMU_SPEED,1,(uchar*)&tmp);									  //加载是否模拟车辆行驶标志
	SimuSpeed.Enable  = tmp;													           //
	/////////// ADD BY HYC /////////////////
//	DFE.Read(EE_SPEED_CALCULUS,8,&Tp.B8[0]);	 									  // 已经在GPS.CPP中 
//	DFE.Read(EE_TOTAL_MILEAGE,4,&Tp.B8[0]);
//	GPS_SpeedCaculus = Tp.B64;
//	GPS_TotalMileage = Tp.B32[0];
	
	DFE.Read(EE_USE_PARK_OVER_TIME,1,&tmp);					 					  // 读取停车超时报警
	EnableParkOverTimeAlarm = tmp;	 													
	
	DFE.Read(EE_SIM_NUMBER,16,(uchar*)sTelNum.Sim);								 //加载SIM卡号码
	tmp = strlen((char*)sTelNum.Sim);
	DecCharToPressBCD(tmp,(char*)sTelNum.Sim,(uchar*)&sTelNum.SimBCD_GB);
	//for(i=0;i<8;i++)	{ if(sTelNum.SimBCD_GB[i]==0)	sTelNum.SimBCD_GB[i] = 0x0f;	}
	memmove(sTelNum.SimBCD_JT, sTelNum.SimBCD_GB, 8);

	DFE.Read(EE_SMS_CENTER_NUMBER, 16, (uchar *)sTelNum.SmsCenter);			//加载移动公司SMS中心号码
	
	
	DFE.Read(EE_JT_CENTER_NUMBER,9+7*16,(uchar *)&JT808_Number);           //加载各种号码

	DFE.Read(EE_CENTER_NUMBER,16,(uchar*)sTelNum.Center);
	DFE.Read(EE_CENTER_NUMBER1,16,(uchar*)sTelNum.Center1);

	DFE.Read(EE_JT_SHIELD_WORD, 20,(uchar *)&JT808_Alarm_Word);						      // 加载报警字
	//DFE.Read(EE_JT_SHIELD_WORD, 16,(uchar *)&JT808_Alarm_Word.Shield);						// 读取报警屏蔽字
	//DFE.Read(EE_JT_TEXT_SMS_WORD, 4,(uchar *)&JT808_Alarm_Word.SendTextSMS);			// 读取报警发送文本SMS开关
	//DFE.Read(EE_JT_TAKE_PHOTO_WORD, 4,(uchar *)&JT808_Alarm_Word.TakePhoto);			// 读取报警拍照开关
	//DFE.Read(EE_JT_KEY_WORD, 4,(uchar *)&JT808_Alarm_Word.KeyAlarm);						// 读取关键标志
	Uart.puts("Alarm word Test Data:");	
	Uart.putb(1,4,(uchar *)&JT808_Alarm_Word);
	Uart.puts("\r\n\0");

	DFE.Read(EE_MANUL_ALARM_EVENT_ID, 2, (uchar *)&ManulAlarmEventID);         	   // 读取人工报警事件ID

	DFE.Read(EE_CALL_TIME_MONTH, 4, (uchar *)&GSM.CallTimeMonth);		  //加载月通话总时间

	DFE.Read(EE_TIME_MONTH, 4, (uchar *)&GSM.OneMonthSec);		        //加载月周期时间或月份

	DFE.Read(EE_PARK_OVER_TIME,2,&Tp.B8[0]);					 
	if(Tp.B16[0]==0)
	{
		ParkOverTimeAlarmTime = 10;
	}
	else
	{
		ParkOverTimeAlarmTime = Tp.B16[0];	
	}
	 
	//	使能标志位
	DFE.Read(EE_ENABLE_FLAG,4,(uchar*)&EnableFlag);									  //读取各种标志使能
	   	
	DFE.Read(EE_JT_CRASH_ALARM_PARA, 2, (uchar *)&JtCrashAlarmSet);
	NOP(); 

	DFE.Read(EE_JT_TILT_ALARM_PARA, 2, (uchar *)&JtTiltAlarmSet);
	NOP(); 

	DFE.Read(EE_JT_GNNS_POS_MODE, 1, &Tp.B8[0]);
	NOP();	NOP();	NOP();	NOP();	
	if(Tp.B8[0]&0x01) 	JT808Flag.VS3.Bits.UseGPS = 1;
	else						JT808Flag.VS3.Bits.UseGPS = 0;					
	if(Tp.B8[0]&0x02) 	JT808Flag.VS3.Bits.UseBD = 1;
	else						JT808Flag.VS3.Bits.UseBD = 0;
		 
	/////////////////////////////

	DFE.Read(FM_SpeeMode_FLAG, 2,(uchar*)tBuffer);									//速度获取途径     传感器或GPS
	SpeedModeSelect = tBuffer[0];	


/*	DFE.Read(EE_HANDLE_WATCH_CAR_PASSWORD,6,tBuffer);					 			  // 读取看车手柄密码		无手柄看车
	for(i=0;i<6;i++)
		HandleWatchCarPassword[i] = tBuffer[i];										*/	  

	DFE.Read(EE_DIAL_ANSWER_ENABLE_FLAG,1,&tmp);									  // 读取允许通话功能标志位
	EnableDialOrAnswerFlag = tmp;	
	/*
	DFE.Read(EE_ENABLE_ALARM_FLAG,2,&Tp.B8[0]);									  	// 读取允许开启警情标志位
	EnableAlarmFlag = Tp.B16[0];	
	if(EnableAlarmFlag&0x0001)		EnableStealAlarm = 1;							// 允许使用盗警检测
	if(EnableAlarmFlag&0x0002)		EnableRobAlarm = 1;								// 允许使用劫警检测
	if(EnableAlarmFlag&0x0004)		EnableNoSetScanAlarm = 1;						// 允许使用未设防报警检测
	if(EnableAlarmFlag&0x0008)		EnableTrailCarAlarm = 1;						// 允许使用拖车报警检测
	if(EnableAlarmFlag&0x0010)		EnableHelpAlarm = 1;								// 允许使用求助报警检测
	if(EnableAlarmFlag&0x0020)		EnableTireDriveAlarm = 1;						// 允许使用疲劳驾驶检测
	if(EnableAlarmFlag&0x0040)		EnableIllegalOpenDoor = 1;						// 允许使用非法开门检测
	if(EnableAlarmFlag&0x0080)		EnableIllegalMakeFire = 1;						// 允许使用非法点火检测
	if(EnableAlarmFlag&0x0100)		EnableCheckShake = 1;							// 允许使用震动检测
	if(EnableAlarmFlag&0x0200)		EnableAutoLockDoor = 1;							// 允许使用锁门
	if(EnableAlarmFlag&0x0400)		EnableLowVoltage = 1;							// 允许使用低压检测
	*/
	//DFE.Read(EE_HELP_NUMBER,16,(uchar*)sUserHelpNumber);		  		  	      // 读取求助号码



	DFE.Read(EE_SELECT_SIM_CARD,1,tBuffer);									      //sim卡选择，内置，外置
	if(tBuffer[0]=='I')
	{
		//HC595.Out1.Bits.SwitchSIM = 1;
	    UseWthSimCard = USE_IN_SIM;
	}
	else
	{
		//HC595.Out1.Bits.SwitchSIM = 0;
		UseWthSimCard = USE_OUT_SIM;
   }
												   

	///////////////////////////////////////////////////// 读取省电模式相关值
	DFE.Read(EE_IDLE_STR,LEN_EE_IDLE,(uchar*)&Idle);

	if( (Idle.InterIdleModeTime<3)||(Idle.InterIdleModeTime>21600) )					// 多少时间后进入省电模式
		Idle.InterIdleModeTime = 30;
	if( (Idle.WakeUpTime<5)||(Idle.WakeUpTime>21600) )										// 多少时间后唤醒
		Idle.WakeUpTime = 30;
	if( (Idle.WakeUpLongTime<5)||(Idle.WakeUpLongTime>21600) )							// 唤醒持续时间
		Idle.WakeUpLongTime = 30;
	Idle.SecondCnt=0;	
	////////////////////////////////////////////////////// 加载完省电模式相关参数
	

	DFE.Read(EE_HANDLE_FACTORY_PASSWORD,8,tBuffer);
	for(i=0;i<8;i++)
		FactoryPassword[i] = tBuffer[i];		
	
/*	DFE.Read(EE_ENABLE_HANDLE_WATCH_CAR,1,&tmp);                           //JT808无手柄看车
	EnableHandleWatchCar = tmp;	
	DFE.Read(EE_HANDLE_WATCH_CAR_MODE,1,&tmp);
	HandleWatchCarMode = tmp;											  */
	
	
	DFE.Read(EE_SET_SCAN_FLAG,1,&tmp);
	SetScanAlarmFlag = tmp;	
	DFE.Read(EE_WAKE_UP_TYPE,1,(uchar*)&WakeUpType);

	VehicleStatus.VS8.Bits.IdleMode = 0;
	tmp = 0;
	DFE.Write(EE_WAKE_UP_TYPE,1,&tmp);
	
	
	DFE.Read(EE_LOCK_FLAG,LEN_LOCK_CONTROL_OUT,(uchar *)&VehicleControl);	 	// 

	if (SystemLog.ResetCnt.PowerOn != 0) // 前面有计数
	{
	 	DFE.Read(EE_SYS_RESET_POWER_ON_CNT,4,(uchar*)&SystemLog.ResetCnt.PowerOn);
	 	SystemLog.ResetCnt.PowerOn++;
	  	DFE.Write(EE_SYS_RESET_POWER_ON_CNT,4,(uchar*)&SystemLog.ResetCnt.PowerOn);
	}
	if (SystemLog.ResetCnt.WatchDog != 0) // 前面有计数
	{
		DFE.Read(EE_SYS_RESET_WATCHDOG_CNT,4,(uchar*)&SystemLog.ResetCnt.WatchDog);
	  	SystemLog.ResetCnt.WatchDog++;
	  	DFE.Write(EE_SYS_RESET_WATCHDOG_CNT,4,(uchar*)&SystemLog.ResetCnt.WatchDog);	
	}

	if (SystemLog.ResetCnt.ResetPin != 0) // 前面有计数
	{
		DFE.Read(EE_SYS_RESET_PIN_RESET_CNT,4,(uchar*)&SystemLog.ResetCnt.ResetPin);
	  	SystemLog.ResetCnt.ResetPin++;
	  	DFE.Write(EE_SYS_RESET_PIN_RESET_CNT,4,(uchar*)&SystemLog.ResetCnt.ResetPin);	
		NOP(); NOP();NOP();NOP();NOP();NOP();NOP();
	}

	DFE.Read(EE_TEST_MILEAGE,8,(uchar*)&TestMileage);
			   
	DFE.Read(EE_SYS_RESET_SYSTEM_CNT,4,(uchar*)&SystemLog.ResetCnt.SystemRestartCnt);
	SystemLog.ResetCnt.SystemRestartCnt++;
	DFE.Write(EE_SYS_RESET_SYSTEM_CNT,4,(uchar*)&SystemLog.ResetCnt.SystemRestartCnt);	
	
	DFE.Read(EE_HIGH_LOW_LEVEL,8,Tp.B8);
	HighLowLevel = Tp.B32[0];	 
	LowPowerAlarmValue = Tp.B32[1];

	/////////////////////////////////////////
	DFE.Read(EE_SYSTEM_LOG_BASE,LEN_SYSTEM_LOG,(uchar *)&SystemLog);

	///////////////////////////////////////////////////////// Flash页计数器初始化
	DFE.Read(FM_FLASH_PAGE_CNT_INIT_FLAG,16,tBuffer);
	for(c=0,i=0;i<16;i++)
	{
		if(MyStrChar((char*)tBuffer,FirstRunFlag[i]))
			c++;
	}
	if(c<10)
	{
		SetBufferValue( (uchar*)&FlashPageCnt,LEN_FLASH_PAGE_CNT,0);
		DFE.Write( FM_FLASH_PAGE_CNT,LEN_FLASH_PAGE_CNT,(uchar*)&FlashPageCnt);			
		DFE.Write( FM_FLASH_PAGE_CNT_INIT_FLAG,16,(uchar*)FirstRunFlag );
		Uart.puts("首次初始化，Flash存储器页计数器全部清零 ");
	}

	DFE.Read(FM_FLASH_PAGE_CNT,LEN_FLASH_PAGE_CNT,(uchar*)&FlashPageCnt);


	DFE.Read(EE_GPRS_DISP_PROTOCOL,8,(uchar*)&Protocol);
	
	DFE.Read(EE_TERMINAL_TYPE,20,tBuffer);

	if (memcmp(tBuffer,"SZ-BD21",7)==0)
	{
	//	tBuffer[7] = 'A';
    	tBuffer[7] = 0;
		tBuffer[8] = 0;
		DFE.Write(EE_TERMINAL_TYPE,20,tBuffer);
	}


	//////////////////////////////////////////////////////////
	DFE.Read(EE_PACK_HEAD,8,(uchar*)tBuffer);							//无线通信协议头
	c = 0;
	if( (tBuffer[0]==tBuffer[4])&&(tBuffer[1]==tBuffer[5])&&(tBuffer[2]==tBuffer[6])&&\
	    (tBuffer[3]==tBuffer[7])&&(tBuffer[3]==0) )
	{
		if(tBuffer[0]=='*')
		{
			if( (tBuffer[1]>='A')&&(tBuffer[1]<='Z')  &&  (tBuffer[2]>='A')&&(tBuffer[2]<='Z'))
				c = TRUE;						
		}		
	}
	if(c==TRUE)
		strcpy((char*)sPackHead,(char*)tBuffer);	
	else
		strcpy((char*)sPackHead,"*GY\0");	
		
	NOP();	NOP();		
}

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：SystemCls::RestorFactorySet(void)
   功能描述：系统初始化函数
   入口参数：无
   出口参数：无
   全局变量: 无
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
#include "./Speed/Speed.h"
void		SystemCls::FirstPowerOn(uchar Format)
{
	uint32	Length;
	//pGPS_SEND	pSd;
	union
	{	
	   uint32   	U32[64];
	   uint16   	U16[128];
		uchar		 	tBuffer[256];
		struct
      {
	     uint32		RunTime;	         // 允许连续驾驶时间
		  uint32		RestTime;
	     uint32		RunTimeOneDay;	   //一天的累计驾驶时间;	 
	     uint32    MaxStopTime;	   // 最长停车时间
		  uint32		PreAlarmTime;
   	}bf;	
	}Tf;		//
		
	if(Format==TRUE)
	{		
		//Uart.EreadFRAM_RecodeMem(FM_TOTAL_SPEED_PULS, 3800);			//擦除记录空间所有数据和标志
		Uart.EreadFRAM_RecodeMem(0, 8192);									   //擦除整片Fram
		////////////////////////////////////////// 清除所有Flash页计数器
		SetBufferValue((uchar*)&FlashPageCnt,LEN_FLASH_PAGE_CNT,0);
		DFE.Write( FM_FLASH_PAGE_CNT, LEN_FLASH_PAGE_CNT, (uchar*)&FlashPageCnt);
	}
	else
	{
		SetBufferValue(Tf.tBuffer, 256, 0);
		DFE.Write(FM_FACTORY_SET_BASE, 0x1ff-0x140, (uchar*)Tf.tBuffer); 	 	//清除总里程，360小时里程，两个日历天里程，停车前15分钟平均速度，5秒种每秒的脉冲数
		DFE.Write(FM_LAST_SECOND, 0x42-0x30, (uchar*)Tf.tBuffer);				   //清除驾驶时间信息，设备掉电时刻	
		//清除驾驶信息和历史驾驶员信息
		DFE.Write(FM_CURRENT_DRIVER_INFO_TIME, 56*3, (uchar*)Tf.tBuffer);					   
		NOP();NOP();
		// 清除位置信息时间
		DFE.Write(FM_360H_LOCATION_TIME,6,(uchar*)Tf.tBuffer);			    
		  
		/////删除圆形围栏、矩形、方形围栏、路线围栏
		SetBufferValue( Tf.tBuffer,64,0);
		DFE.Write(EE_ELE_FENCE_ATTR_BASE_ADDR,64,Tf.tBuffer);
		
		DFE.Write(FM_INIT_MILAGE_BCD, 4, (uchar*)Tf.tBuffer); 
		NOP(); NOP(); NOP();
		
		SetBufferValue((uchar*)&RoundFenceAttr,LEN_EE_FENCE_ATTR,0);
		DFE.Write(EE_ROUND_FENCE_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RoundFenceAttr);
		SetBufferValue((uchar*)&RectFenceAttr,LEN_EE_FENCE_ATTR,0);
		DFE.Write(EE_RECTANGLE_FENCE_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RectFenceAttr);
		SetBufferValue((uchar*)&PolygonFenceAttr,LEN_EE_FENCE_ATTR,0);
		DFE.Write(EE_POLYGON_FENCE_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&PolygonFenceAttr);
		SetBufferValue((uchar*)&RoadAttr,LEN_EE_FENCE_ATTR,0);
		DFE.Write(EE_ROAD_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RoadAttr);	

		FlashPageCnt.NormalMsgSave = 0;	FlashPageCnt.NormalMsgRead = 0;
		FlashPageCnt.AlarmMsgSave = 0;	FlashPageCnt.AlarmMsgRead = 0;
	
		
		FlashPageCnt.HistoryRead = FlashPageCnt.HistorySave = 0;
		FlashPageCnt.Speed48HRead = FlashPageCnt.Speed48HSave = 0;
		FlashPageCnt.Location360HRead = FlashPageCnt.Location360HSave = 0;
		FlashPageCnt.ParkingDataRead = FlashPageCnt.ParkingDataSave = 0;
		FlashPageCnt.FatigueDrivingRead = FlashPageCnt.FatigueDrivingSave = 0;
		FlashPageCnt.DriverLogInOutRead = FlashPageCnt.DriverLogInOutSave = 0;
		FlashPageCnt.PowerOnOffRead = FlashPageCnt.PowerOnOffSave = 0;
		FlashPageCnt.ParaModifyRead = FlashPageCnt.ParaModifySave = 0;
		FlashPageCnt.SpeedStatusRead = FlashPageCnt.SpeedStatusSave = 0;
        FlashPageCnt.BlindSave = FlashPageCnt.BlindRead = 0;
	    DFE.Write(FM_FLASH_PAGE_CNT,LEN_FLASH_PAGE_CNT,(uchar*)&FlashPageCnt);
		Uart.puts("\r\nFlash存储页计数器部分清零 ");

		 //#if (WUXI_TEST == 1)
		 memmove(&Tf.tBuffer[0],"\x15\x01\x15\x10\x25\x00",6);		  
		 DFE.Write(FM_INSTALL_DATE,6,&Tf.tBuffer[0]);		 
		 //#endif
	
	}



//	pSd = (pGPS_SEND)&Tf.tBuffer[0];
//	pSd->Str.Times = 0xffffffff;
//	pSd->Str.Interval = 60;
//	pSd->Str.StopInterval = 60;
//	pSd->Str.Distance = 1000;
//	pSd->Str.T_Interval = 38;
//	pSd->Str.T_Time = 0;
//	pSd->Str.Type = 0x55;
//	pSd->Str.DelayAlarmTime = 10;
//	GPS.SetSendConfig(pSd,1);
	
	RESET_WATCH_DOG; 
	DFE.Write(EE_IP_PORT,32,(uchar*)DefaultIP);	
	os_dly_wait(10);


	AccelPara[0].Start[0] = 0;		  AccelPara[0].End[0] = 20;		AccelPara[0].Value[0] = 11;			
	AccelPara[0].Start[1] = 20;	  AccelPara[0].End[1] = 60;		AccelPara[0].Value[1] = 11;			
	AccelPara[0].Start[2] = 60;	  AccelPara[0].End[2] = 100;		AccelPara[0].Value[2] = 8;			
	AccelPara[0].Start[3] = 100;	  AccelPara[0].End[3] = 120;		AccelPara[0].Value[3] = 4;			
	AccelPara[0].Start[4] = 120;	  AccelPara[0].End[4] = 200;		AccelPara[0].Value[4] = 2;
	
	AccelPara[1].Start[0] = 0;		  AccelPara[1].End[0] = 20;		AccelPara[1].Value[0] = 20;			
	AccelPara[1].Start[1] = 20;	  AccelPara[1].End[1] = 60;		AccelPara[1].Value[1] = 10;			
	AccelPara[1].Start[2] = 60;	  AccelPara[1].End[2] = 100;		AccelPara[1].Value[2] = 6;			
	AccelPara[1].Start[3] = 100;	  AccelPara[1].End[3] = 120;		AccelPara[1].Value[3] = 4;			
	AccelPara[1].Start[4] = 120;	  AccelPara[1].End[4] = 200;		AccelPara[1].Value[4] = 3;			
	DFE.Write(EE_ACCEL_ADD_DEC_PARA,LEN_EE_ACCEL_ADD_DEC_PARA,(uchar*)&AccelPara);
	Uart.puts("Accel Set Init Test Data:");
	Uart.putb(1,30,(uchar*)&AccelPara[0]);
	Uart.puts("\r\n\0");		

	

	RESET_WATCH_DOG; 

	DFE.Write(EE_CENTER_NUMBER,16,(uchar*)DefaultCenterNumber);	
	os_dly_wait(10);

	DFE.Write(EE_CENTER_NUMBER1,16,(uchar*)DefaultCenterNumber1);	
	os_dly_wait(10);

	DFE.Write(EE_DOMAIN_ADDR,32,(uchar*)GY_DomainName);	
	os_dly_wait(10);


	SetBufferValue((uchar*)&SystemLog,LEN_SYSTEM_LOG,0);

	RESET_WATCH_DOG; 

	#if (1)
	Tf.bf.RunTime = 4*60*60;															 //疲劳驾驶，和超时驾驶，超时停车默认参数			JT808		  4
	Tf.bf.RunTimeOneDay = 20*60*60;
	Tf.bf.RestTime = 20*60;
	Tf.bf.PreAlarmTime = 30*60;	// 30分钟 预警
	#else // test 
	Tf.bf.RunTime = 5*60;															 //疲劳驾驶，和超时驾驶，超时停车默认参数			JT808		  4
	Tf.bf.RunTimeOneDay = 20*60*60;
	Tf.bf.RestTime = 2*60;
	Tf.bf.PreAlarmTime = 2*60;	// 2分钟 预警
	#endif																																 //20
	Tf.bf.MaxStopTime = (1*60*60);

	DFE.Write(FM_DRIVING_PARA,20,(uchar *)&Tf.tBuffer[0]);

	Tf.U16[0] = 0;
	DFE.Write(FM_FATIGUE_DRIVING_CNT,4,(uchar*)&Tf.U16[0]);
	OverTimeDriveUpateFlag = 0;
	DFE.Write(EE_OVERTIME_DRIVE_UPDATE_FLAG,1,&OverTimeDriveUpateFlag);

	// 超速报警值
	Tf.tBuffer[0] = TRUE;
	Tf.tBuffer[1] = 130;
	Tf.tBuffer[2] = 1;
	Tf.tBuffer[3] = 90;				 // 	  超速值	90km/h
	Tf.U16[2] = 10;
	Tf.U16[3] = 10;
	SpeedC.SetGetPara( eSPEED_CMD_SET_OVER_SPEED,LEN_OVER_SPEED_PARA,Tf.tBuffer);

	

	NOP();	NOP();
	VehiclePulseKV = 3600UL;  //450;
	DFE.Write(FM_VEHICLE_PULSE_KV,4,(uchar *)&VehiclePulseKV);		 //出厂特征系数
	
	NOP();	NOP();
	DFE.Write(FM_DRIVER_NUMBER,22,(uchar *)FirstDriver); 				 //出厂驾驶员
	NOP();	NOP(); 
	//DFE.Write(FM_VEHICLE_VIN,LEN_VEHICLE_VCC,(uchar *)FirstCarMSG); 					//出厂车辆信息 
	DFE.Write(FM_VEHICLE_CODE,LEN_VEHICLE_VCC,(uchar *)FirstCarMSG);
	NOP();	NOP();  
	DFE.Write(FM_GPS_Standard,15,(uchar *)GB_Version); 
	NOP();	NOP();
	Tf.tBuffer[0] = 0;
	//Tf.tBuffer[1] = 120;
	Tf.tBuffer[1] = 0; //	 1:GPS
	DFE.Write(FM_SpeeMode_FLAG, 2,(uchar*)&Tf.tBuffer[0]); 			  //速度来源，0：传感器，1:GPS
	os_dly_wait(2);


	//DFE.Write(EE_PROVINCE_REGION_ID, 29,(uchar *)JT_RegMsg);  		  //省域ID,市县域ID,制造商ID,终端型号
	//os_dly_wait(2);

	////////////省域ID,市县域ID,制造商ID,终端型号 终端ID	共29+7字节
	 //if(Format==TRUE)
	 {
		SetBufferValue(Tf.tBuffer, 128, 0);
		/*Tf.tBuffer[0] = PROVINCE_REGION_ID >> 8;
		Tf.tBuffer[1] = PROVINCE_REGION_ID & 0xff;
		Tf.tBuffer[2] = PREFECTURE_REGION_ID >> 8;
		Tf.tBuffer[3] = PREFECTURE_REGION_ID & 0xff;*/

 
		Tf.U16[0] = PROVINCE_REGION_ID;
		Tf.U16[1] = PREFECTURE_REGION_ID;
	
	
		memmove(&Tf.tBuffer[4],(uchar *)JT_ProductorID, 5);	  // 制造商ID
	
		//Len = strlen(Model);
		//if (Len > 20)  Len = 20;
		//strncpy((char *)&Tf.tBuffer[9], Model, Len);// 终端型号
	
		#if (PRODUCT_TYPE==GUOYEE_PRT)
		strncpy((char *)&Tf.tBuffer[9], "GY-G11A", 7);// 终端型号
		#elif (PRODUCT_TYPE==SHANGHAI_SIZENG_PRT)
		//strncpy((char *)&Tf.tBuffer[9], "SZ-BD21A", 8);// 终端型号 	
		strncpy((char *)&Tf.tBuffer[9], "SZ-BD21", 7);// 终端型号  
		#elif  (PRODUCT_TYPE==GENERAL_PRT) 
		strncpy((char *)&Tf.tBuffer[9], "BD-GPS", 6);// 终端型号 
		#endif  
	
		//strncpy((char*)sOEM_ID,(char*)constOEM_ID, 16);		  // 终端ID 7字节
		//Len = strlen((char *)sOEM_ID);
		//SetBufferValue(Tf.tBuffer, 20, 0);
		//DecCharToPressBCD(Len, (char *)sOEM_ID,(uchar*)&Tf.tBuffer[29]);
		memmove((uchar*)&Tf.tBuffer[29], &sOEM_ID[3], 7);
		
		DFE.Write(EE_PROVINCE_REGION_ID, 29+7, Tf.tBuffer);
		os_dly_wait(2);
		//
	}
	
	SimuSpeed.Enable = 0;											  //默认关闭模拟行车
	DFE.Write(EE_ENABLE_SIMU_SPEED,1,(uchar*)&SimuSpeed.Enable);	
	///////重传参数
	
	SendOVT.TCP_ACK_OverTime = 10;
	SendOVT.TCP_ResendTimes = 3;
	SendOVT.UDP_ACK_OverTime = 10;
	SendOVT.UDP_ResendTimes = 3;
	SendOVT. SMS_ACK_OverTime = 30;
	SendOVT.SMS_ResendTimes = 3;
    DFE.Write(EE_JT_RESEND_MACHINING,LEN_SEND_OVT_STRU,(uchar *)&SendOVT);
	NOP();	NOP(); NOP();NOP();NOP();NOP();NOP();
	////////主服务器参数
	if(Format==TRUE)
	{
		strcpy((char *)&JT808_IP_DNS.M_APN[0], constAPN);
		
		strcpy((char *)&JT808_IP_DNS.M_IP_Name[0], "BDGPS\0");
		strcpy((char *)&JT808_IP_DNS.M_IP_Secrier[0], "BDGPS\0");
		#if (PRODUCT_TYPE==SHANGHAI_SIZENG_PRT)
		strcpy((char *)&JT808_IP_DNS.M_IPorDNS[0], "jt1.gghypt.net\0");
		#elif ( PRODUCT_TYPE==GUOYEE_PRT )
		strcpy((char *)&JT808_IP_DNS.M_IPorDNS[0], "113.108.150.103");
		#else
		strcpy((char *)&JT808_IP_DNS.M_IPorDNS[0], "bdgps.guoyee.com\0");
		#endif
		/////////副服务器参数	 
		
		strcpy((char *)&JT808_IP_DNS.B_APN[0], constAPN);	// "gzjtxx03.gd\0");		
		strcpy((char *)&JT808_IP_DNS.B_IP_Name[0], "BDGPS\0");
		strcpy((char *)&JT808_IP_DNS.B_IP_Secrier[0], "BDGPS\0");
		#if (PRODUCT_TYPE==SHANGHAI_SIZENG_PRT)
		strcpy((char *)&JT808_IP_DNS.B_IPorDNS[0], "jt2.gghypt.net\0");
		#elif (PRODUCT_TYPE==GUOYEE_PRT)
		strcpy((char *)&JT808_IP_DNS.B_IPorDNS[0], "bdgps.guoyee.com\0");
		#else
		strcpy((char *)&JT808_IP_DNS.B_IPorDNS[0], "bdgps.guoyee.com\0");
		#endif


		// added 2013.4
		//////// 认证服务器
		strcpy((char *)&JT808_IP_DNS.IC_Auth_M_IPorDNS[0], "202.96.42.113");
		JT808_IP_DNS.IC_Auth_TCP_Port = 11001;
		DFE.Write(EE_JT_IC_AUTH_M_IP_DNS, LEN_IPorDNS, &JT808_IP_DNS.IC_Auth_M_IPorDNS[0]);
		NOP();	NOP();NOP();NOP();NOP();NOP();
		DFE.Write(EE_JT_IC_AUTH_TCP_PORT, 4,  (uchar *)&JT808_IP_DNS.IC_Auth_TCP_Port);
		NOP();	NOP();NOP();NOP();NOP();NOP();
		//
	  	//DFE.Write(EE_JT_BACK_SERVER,4*32,(uchar *)&JT808_IP_DNS.B_ANP[0]);
		/////////端口号

		#if (PRODUCT_TYPE==SHANGHAI_SIZENG_PRT)
		JT808_IP_DNS.TCP_Port = 7008;
		JT808_IP_DNS.UDP_Port = 7008;	
		#else
		JT808_IP_DNS.TCP_Port = 8009;
		JT808_IP_DNS.UDP_Port = 8011;
		#endif


		//DFE.Write(EE_JT_MAIN_SERVER,4*32,(uchar *)&JT808_IP_DNS);
		DFE.Write(EE_JT_MAIN_SERVER,LEN_JT808_IP_DNS,(uchar *)&JT808_IP_DNS);
		NOP();	NOP();NOP();NOP();NOP();NOP();
	}
	////////连接控制
	JT808_Ctr_IP_DNS.Ctr = 0x01;
	DFE.Write(EE_JT_NET_CTR,1,(uchar *)&JT808_Ctr_IP_DNS.Ctr);		              //连接控制默认连接主服务器

	NeedRegServer = ENABLE_FLAG;
	DFE.Write(EE_NEED_REG_SERVER,1,(uchar *)&NeedRegServer);	           			  //默认需要注册

	CreateUserFlag = NEED_CREATE_USER;		  // 开户
	DFE.Write(EE_CREATE_USER_FLAG, 1, &CreateUserFlag);

	sIP.EnableFlag[eFLAG_TCP] = ENABLE_FLAG;								              //默认打开主服务器TCP,TCP1连接
	sIP.EnableFlag[eFLAG_TCP1] = ENABLE_FLAG;	
	DFE.Write(EE_ENABLE_TCP,1,(uchar*)&sIP.EnableFlag[eFLAG_TCP]);
	DFE.Write(EE_ENABLE_TCP1,1,(uchar*)&sIP.EnableFlag[eFLAG_TCP1]);

	/////信息汇报参数
	SendInterval.HeartbeatTime = 5*60;									   
 	SendInterval.NoDriverTime = 8*60;						       //驾驶员未登录时时间回报间隔	  s
	SendInterval.IDEL_ModeTime = 5*60; //30*60;                       //休眠时时间回报间隔				s
	SendInterval.AlarmModeTime = 10;						          //紧急报警时时间回报间隔			s
	SendInterval.DefauletTime = 30;						       //缺省时间回报间隔     s
	SendInterval.DefauletDistance = 1000;					    //缺省距离回报间隔     m
	SendInterval.NoDriverDistance = 10*1000;					    //驾驶员未登录时距离回报间隔	  m
	SendInterval.IDEL_ModeDistance = 30*1000;                 //休眠时距离回报间隔				m
	SendInterval.AlarmModeDistance = 500;						    //紧急报警时距离回报间隔			m
	SendInterval.MCA.Byte.Mode = 0;					//位置回报策略 0：定时回报  1：定距回报  2：定时和定距		  1B
	SendInterval.MCA.Byte.Condition = 0;			//位置回报方案0 根据ACC状态 1：根据登录状态和ACC状态，先判断登录状态，若登录再根据ACC状态  1B	
	SendInterval.MCA.Byte.Angle = 160;                           	    //拐角补传角度							1B
	SendInterval.MCA.Byte.Rsv = 0; 					
	SendInterval.T_Second = 60;
	SendInterval.T_ContinuedTime = 0;
    DFE.Write(EE_SEND_INTERVAL_BASE,LEN_SEND_INTERVAL_STRU,(uchar *)&SendInterval);
	///////号码
	if(Format==TRUE)
	{
		#if (PRODUCT_TYPE==GUOYEE_PRT)
		strcpy((char *)&JT808_Number.Center[0], "13570230692\0");						  //监控平台号码
		strcpy((char *)&JT808_Number.CenterSMS[0], "13570230692\0");					  //监控平台短信号码
	   strcpy((char *)&JT808_Number.TextSMS[0], "13570230692\0");						  //接收文本报警号码
		#elif (PRODUCT_TYPE==SHANGHAI_SIZENG_PRT)
		strcpy((char *)&JT808_Number.Center[0], "00000000000\0");						  //监控平台号码
		strcpy((char *)&JT808_Number.CenterSMS[0], "15921411030\0");					  //监控平台短信号码
		strcpy((char *)&JT808_Number.TextSMS[0], "00000000000\0");						  //接收文本报警号码
		#elif	(PRODUCT_TYPE==GENERAL_PRT)
		strcpy((char *)&JT808_Number.Center[0], "00000000000\0");
		strcpy((char *)&JT808_Number.CenterSMS[0], "00000000000\0");					  //监控平台短信号码
		strcpy((char *)&JT808_Number.TextSMS[0], "00000000000\0");						  //接收文本报警号码
		#endif
		
		strcpy((char *)&JT808_Number.Reset[0], "13800000000\0");							  //复位号码
		strcpy((char *)&JT808_Number.ResetFactory[0], "13800000000\0");				  //恢复出厂设置号码

		
		JT808_Number.GetCallMode = 1;									                       //接听来电方式0： 自动接听  1： ACC on时自动接听 Off时手动接听		  1B
		JT808_Number.CallTime = 30*60;										                 //每次允许通话时长				  4B
		JT808_Number.CallTimeMonth = 5*60*60;						                       //每月允许通话时长				  4B
		strcpy((char *)&JT808_Number.JT[0], "13570230692\0");								  //监听号码
		strcpy((char *)&JT808_Number.Privilege[0], "13570230692\0");					  //特权SMS号码
	 	DFE.Write(EE_JT_CENTER_NUMBER,9+7*16,(uchar *)&JT808_Number);
		NOP(); NOP(); NOP();
		DFE.Write(EE_SIM_NUMBER, 16,(uchar *)DefaultSIMNumber);   //SIM卡号码
		NOP(); NOP(); NOP();
	}
	//////报警关联		  // 报警屏蔽字 1不报警，0报
	// 默认：欠压报警、掉电报警、碰撞、侧翻；
	//JT808_Alarm_Word.Shield = 0xffffffff;                         //报警屏蔽字，报警标志相应位为1，相应报警被屏蔽
	//JT808_Alarm_Word.Shield &= (~((1UL<<0)|(1UL<<1)|(1UL<<20)|(1UL<<21)|(1UL<<23)));	// 紧急报警、超速报警
	//JT808_Alarm_Word.Shield &= (~(1UL<<7));	// 主电欠压
	//JT808_Alarm_Word.Shield &= (~(1UL<<8));	// 主电掉电
	//JT808_Alarm_Word.Shield &= (~(1UL<<29));	// 碰撞报警
	//JT808_Alarm_Word.Shield &= (~(1UL<<30));	// 欠压报警
	JT808_Alarm_Word.Shield = (	(1UL<<11)|(1UL<<12)|(1UL<<29)	);


	JT808_Alarm_Word.SendTextSMS = 0;								     //报警标志相应位为1 ，报警时发送文本短信
	JT808_Alarm_Word.TakePhoto = 0;						              //报警标志相应位为1 ，报警时拍照	 
	JT808_Alarm_Word.SavePhoto = 0;                               //报警标志相应位为1 ，报警时存储照片，	否则上传照片
	JT808_Alarm_Word.KeyAlarm = 0;						              //报警标志相应位为1 ，该报警为关键报警			
	DFE.Write(EE_JT_SHIELD_WORD,5*4,(uchar *)&JT808_Alarm_Word);
	NOP(); NOP(); NOP();
	Uart.puts("Default Alarm Test Data:");
	Uart.putb(1,4,(uchar*)&JT808_Alarm_Word.Shield);
	Uart.puts("\r\n\0");
	// added 2013.3
	NOP(); NOP(); NOP(); 

	JtCrashAlarmSet = 50 | (10 << 8);  // 4 * 5	= 200ms, 加速度为10 * 0.1g = 1g
	DFE.Write(EE_JT_CRASH_ALARM_PARA, 2, (uchar *)&JtCrashAlarmSet);
	NOP(); NOP(); NOP();
	JtTiltAlarmSet	= 30;  // 30 度
	DFE.Write(EE_JT_TILT_ALARM_PARA, 2, (uchar *)&JtTiltAlarmSet);
	NOP(); NOP(); NOP();

	Tf.tBuffer[0] = (1 << 1); // 启用北斗定位
	Tf.tBuffer[0] |= (1 << 0); // 启用GPS定位
	DFE.Write(EE_JT_GNNS_POS_MODE, 1, Tf.tBuffer);
	NOP(); NOP(); NOP();
	Tf.tBuffer[0] = 0x01; // 默认波特率为9600
	DFE.Write(EE_JT_GNSS_BAUD_RATE, 1, Tf.tBuffer);
	NOP(); NOP(); NOP(); 
	Tf.tBuffer[0] = 0x01; // 默认1000ms输出从模块输出数据
	DFE.Write(EE_JT_GNSS_DATA_OUTPUT_FREQUENCY, 1, Tf.tBuffer);
	NOP(); NOP(); NOP(); 
	Tf.tBuffer[0] = 1; // 默认 1秒
	DFE.Write(EE_JT_GNSS_DATA_COLLECT_FREQUENCY, 1, Tf.tBuffer);
	NOP(); NOP(); NOP(); 
	//
	SpeedC.OverSpeed.Enable = TRUE;
	SpeedC.OverSpeed.HighSpeedLimit= 120;	 // 120km/h
	SpeedC.OverSpeed.PreAlarmDifference = 100;
	SpeedC.OverSpeed.EnableTime = 5;
	DFE.Write( FM_OVER_SPEED_PARA,LEN_OVER_SPEED_PARA,(uchar*)&SpeedC.OverSpeed );

	// added 2013.5
	// CAN总线设置相关
	JT_CanSet.Can1SampleInterval = 2000;  // 默认CAN1采集为2S
	JT_CanSet.Can1UploadInterval = 120;   // 默认CAN1上传数据为120s
	JT_CanSet.Can2SampleInterval = 2000;  // 默认CAN2采集为2S
	JT_CanSet.Can2UploadInterval = 120;   // 默认CAN2上传数据为120s

	DFE.Write(EE_JT_CAN_CHAN_1_COLLECT_TIME_INTERVAL, 12, (uchar *)&JT_CanSet);
	NOP(); NOP(); NOP(); 

	SetBufferValue(Tf.tBuffer, 16, 0);
	Tf.tBuffer[0] = Tf.tBuffer[1] = 0;
	Tf.tBuffer[2] = 0x03;  Tf.tBuffer[3] = 0xE8;    // 设置为1000ms 采集间隔	//  方便测试
	Tf.tBuffer[4] = Tf.tBuffer[5] =  0;				 // CAN1, 标准帧，原始数据
	Tf.tBuffer[6] = 0x01; Tf.tBuffer[7] = 0xfd;			  // 这里用G10的CAN数据的ID : 0x1fd
	DFE.Write(EE_JT_CAN_ID_COLLECT_SET_BASE, 8, Tf.tBuffer);		  // 默认设置对应JT808的 CAN: 0x0110 
	NOP(); NOP(); NOP();

	DFE.Write(FM_SIGNAL_CONFIG,80,(uchar *)"未定义\x00\x00\x00\x00未定义\x00\x00\x00\x00未定义\x00\x00\x00\x00近光\x00\x00\x00\x00\x00\x00远光\x00\x00\x00\x00\x00\x00右转向\x00\x00\x00\x00左转向\x00\x00\x00\x00制动\x00\x00\x00\x00\x00\x00");
	NOP();NOP();NOP();

	SetBufferValue((uchar *)&SpeedStatusJudge,LEN_SPEED_STATUS_JUDGE,0);
	DFE.Write(FM_SPEED_STAUTS_JUDGE,LEN_SPEED_STATUS_JUDGE,(uchar *)&SpeedStatusJudge);

   Tf.U32[0] = 0xFFFFFFFE;													  //开关量高低检测配置
	Tf.U32[1] = 85; 															  //8.5v 12v车低压报警阀值
	DFE.Write(EE_HIGH_LOW_LEVEL,8,Tf.tBuffer);

	SetBufferValue(&Tf.tBuffer[0],32,0);	



#if (PRODUCT_TYPE==SHANGHAI_SIZENG_PRT)
	strcpy((char*)sPackHead,"*SZ\0");
	strcpy((char*)&Tf.tBuffer[0],"*SZ\0");
	strcpy((char*)&Tf.tBuffer[4],"*SZ\0");	

	 // 国标唯一编码
	SetBufferValue(Tf.tBuffer,35,0);
	memmove((uchar*)Tf.tBuffer,"C000928",7);   // 生产厂CCC认证代码
	Length = 7;
	DFE.Read(EE_TERMINAL_TYPE,16, (uchar*)Tf.tBuffer+Length);      			//  认证产品型号	 后面的型号使用这个
	Length += 16;
	memmove((uchar*)Tf.tBuffer+Length,"\x14\x11\x01",3); 				// BCD码 生产日期年月日
	Length += 3;
	
	Tf.U32[36] = SwapINT32(Str2Int((char *)sOEM_ID));					  // 编号
	memmove((uchar*)Tf.tBuffer+Length,(uchar*)&Tf.U32[36],4); 
	Length += 4+5;		//  加上5个字节的备用													  			
	
	DFE.Write(FM_SERIAL_NUMBER,35,Tf.tBuffer);
	NOP();NOP();
	 	

#elif (PRODUCT_TYPE==GUOYEE_PRT)
	strcpy((char*)sPackHead,"*GY\0");
	strcpy((char*)&Tf.tBuffer[0],"*GY\0");
	strcpy((char*)&Tf.tBuffer[4],"*GY\0");
	
	// 国标唯一编码
	SetBufferValue(Tf.tBuffer,35,0);
	memmove((uchar*)Tf.tBuffer,"0000000",7);   // 生产厂CCC认证代码
	Length = 7;
	DFE.Read(EE_TERMINAL_TYPE,16, (uchar*)Tf.tBuffer+Length);      			//  认证产品型号	 后面的型号使用这个
	Length += 16;
	memmove((uchar*)Tf.tBuffer+Length,"\x14\x11\x01",3); 				// BCD码 生产日期年月日
	Length += 3;
	
	Tf.U32[36] = SwapINT32(Str2Int((char *)sOEM_ID));					  // 编号
	memmove((uchar*)Tf.tBuffer+Length,(uchar*)&Tf.U32[36],4); 
	Length += 4+5;		//  加上5个字节的备用													  			
	
	DFE.Write(FM_SERIAL_NUMBER,35,Tf.tBuffer);
	NOP();NOP();
		
#elif (PRODUCT_TYPE==GENERAL_PRT)
	strcpy((char*)sPackHead,"*TH\0");
	strcpy((char*)&Tf.tBuffer[0],"*TH\0");
	strcpy((char*)&Tf.tBuffer[4],"*TH\0");	

	 // 国标唯一编码
	SetBufferValue(Tf.tBuffer,35,0);
	memmove((uchar*)Tf.tBuffer,"0000000",7);   // 生产厂CCC认证代码
	Length = 7;
	DFE.Read(EE_TERMINAL_TYPE,16, (uchar*)Tf.tBuffer+Length);      			//  认证产品型号	 后面的型号使用这个
	Length += 16;
	memmove((uchar*)Tf.tBuffer+Length,"\x14\x11\x01",3); 				// BCD码 生产日期年月日
	Length += 3;
	
	Tf.U32[36] = SwapINT32(Str2Int((char *)sOEM_ID));					  // 编号
	memmove((uchar*)Tf.tBuffer+Length,(uchar*)&Tf.U32[36],4); 
	Length += 4+5;		//  加上5个字节的备用													  			
	
	DFE.Write(FM_SERIAL_NUMBER,35,Tf.tBuffer);
	NOP();NOP();

#endif
	// 包头
	DFE.Write(EE_PACK_HEAD,8,&Tf.tBuffer[0]);	
	os_dly_wait(10);

	// add by xzb
	NeedRegServer = ENABLE_FLAG;
	DFE.Write(EE_NEED_REG_SERVER, 1, &NeedRegServer);

	
	accel_default_para();  // 把加速度传感器参数写入铁电

	if(Format==TRUE)			  //第一次上电初始化日期
	{
	    LPC_RTC->YEAR  = 2015;
	    LPC_RTC->MONTH = 1;
	    LPC_RTC->DOM   = 15;
	    LPC_RTC->HOUR	= 10;
	    LPC_RTC->MIN	= 25;
	    LPC_RTC->SEC	= 0;

		 //#if (WUXI_TEST == 1)
		 memmove(&Tf.tBuffer[0],"\x15\x01\x15\x10\x25\x00",6);		  
		 DFE.Write(FM_INSTALL_DATE,6,&Tf.tBuffer[0]);		 
		 //#endif
	}
	
	
			

}



/////////// change by wrc  ///////////////
void       SystemCls::CheckResetSource(void)
{		
	uchar		RstSrc;
	char		tBuffer[32];	
	NOP();	NOP();
	RstSrc = LPC_SC->RSID;	  
	switch(RstSrc&0x0f)
	{
		case 0x01:
			strcpy(tBuffer,"\r\nPower On Reset\r\n\0");						
			//ResetSource = POWER_ON_RESET;
			PowerOn = 1;
			SystemLog.ResetCnt.PowerOn++;

			break;
		case 0x02:
			strcpy(tBuffer,"\r\nReset Pin Reset\r\n\0");
			//ResetSource = RESET_PIN_RESET;
			SystemLog.ResetCnt.ResetPin++;
			break;
		case 0x03:
			strcpy(tBuffer,"\r\nPowerOn ResetPin\r\n\0");
			PowerOn = 1;
			SystemLog.ResetCnt.PowerOn++;
			break;
		case 0x04:
			strcpy(tBuffer,"\r\nWatchdog Reset\r\n\0");
			//ResetSource = WATCHDOG_RESET;
			SystemLog.ResetCnt.WatchDog++;
			break;
		case 0x08:		  
			strcpy(tBuffer,"\r\nLow Power Reset\r\n\0");
			//ResetSource = LOW_POWER_RESET;
			break;		
		default: 
			strcpy(tBuffer,"\r\nDon't know reason Reset\r\n\0");			
			//ResetSource = DO_NOT_KONW_REASON_RESET;
			PowerOn = 1;
			break;
	}
	SysReset = 1;
	LPC_SC->RSID = 0xff;	
	Uart.puts(tBuffer);
	Uart.PopAlignmentSend();
	NOP();	NOP();
}









void	SystemCls::ResetWatchDog(void)
{
	RESET_WATCH_DOG;
} 	// End of <ResetWatchDog> function


#define		WATCH_DOG_PERIOD		1			// 秒
void			SystemCls::InitWatchDog(uint32 Clock)
{  
	NOP();	NOP();
	#if(OUTSIDE_WATCH_DOG)
	NOP();	NOP();
	#else
	DisableIRQ();
	LPC_WDT->WDTC = Clock;	//(Fwdt*WATCH_DOG_PERIOD);		// 设置WDTC，喂狗重装值
   	LPC_WDT->WDMOD = 0x03;								// 设置并启动WDT
   	LPC_WDT->WDFEED = 0xAA;
   	LPC_WDT->WDFEED = 0x55;
	EnableIRQ(); 
	NOP();	NOP();
	#endif
	ResetWatchDog();
}	


/////////////////////////////////////  系统类构造函数，空
SystemCls::SystemCls(void)
{
	
	


}	// End of <SystemCls::SystemCls> function


/*
	TaskID.GSM_GPRS_Transmit = os_tsk_create(TaskGSM_GPRS_Transmit,TASK_GSM_GPRS_TRANSMIT);
	TaskID.GPS = os_tsk_create(TaskGPS,TASK_GPS);
	TaskID.Camera = os_tsk_create(TaskCameraTrans,TASK_CAMERA);
*/

//static	char		Monitor_tBuf[64];
void			SystemCls::TaskMonitor(void)
{
	uchar 		s;
	static		uchar	ResetFlag = 0;
	
	if ((JT_Ctr_PowerDown == 1) || (FTPdowningflag == TRUE))		return;

	s = 0;
	if(++TC.MainTask > (3*60UL))				s = '1';
	if(++TC.GPS > (3*60L + 10))				s = '2';
	//#if (WUXI_TEST != 1)
	if(++TC.GSM > (10*60L + 5))				s = '3';

				  
	if(++TC.GG_Rec > (10*60L + 10))			s = '4';
	if(++TC.GG_Send > (10*60L + 15))			s = '5';
	
	if(++TC.Camera > (6*60L))					s = '6';
	if(++TC.GPRS_LongTimeNoSend > (1*60*60L))	s = '7';
	//#endif
	if(++TC.CANSend >(5*60L + 25))  			s = '9';
	if(++TC.CANRecv >(5*60L + 30))  			s = 'A';
	if(++TC.KeyDisp >(3*60L))  				s = 'B';
	if(++TC.Speed >(4*60L))    				s = 'C';
	if(USBTaskCreat==1)
	{ 
		if(++TC.USB >(5*60L + 35))  			s = 'D';
	}
	if (PrintTaskCreat==1)
	{
		if(++TC.Print >(5*60L + 40))  		s = 'E';
	}

	if (s)	ResetFlag++;
	if (ResetFlag == 1)	SaveLogFlag = TRUE;			 // 任务超时，设置保存日志标志

	if(ResetFlag > 5)					 // 任务超时5秒后重启
	{
		LPC_UART0->THR = 'T';	LPC_UART0->THR = 'C';	LPC_UART0->THR = 'O';	
		LPC_UART0->THR = 'V';	LPC_UART0->THR = 'T';	LPC_UART0->THR = ':';	
		LPC_UART0->THR	= s;
		LPC_UART0->THR = '\r';	LPC_UART0->THR = '\n';
		Sys.Delay_mS(100);
		
		DisableIRQ();
		Sys.Delay_mS(10);
		SystemLog.ResetCnt.TaskOverTime++;
		DFE.Write( EE_SYSTEM_LOG_BASE,LEN_SYSTEM_LOG,(uchar*)&SystemLog);
		NVIC_SystemReset(); 		// 重启
		//while(1);			
	}		

	
}













void           SystemCls::Delay_uS(uint16 t)		// 主频11.0592M * 5
{
   uchar	   c;
   uint16   i;
   for(i=0;i<t;i++) 
   { 
		NOP(); NOP();
	   for(c=0;c<6;c++)
      { NOP(); NOP(); NOP(); NOP(); NOP(); NOP();NOP(); NOP(); }
   }
}	// End of <Delay_uS> function


void       SystemCls::Delay_mS(unsigned short  t)  		// 主频11.0592M * 5
{
  
   while(t--)
	{
		Delay_uS(1000);
	}
	 
}	// End of <Delay_mS> function



/*
	if(TD.GPS==tGPS_TEMP_PACK)
	{
		Uart.puts("GTP \r\n\0");
	}
*/


void     SystemCls::OemTest(void)							  //自检Fram Flash
{

	uchar DataBuffer[256], Dbuffer[20],  i, c = 0;
	
	SelfTestResust = 0;
	SetBufferValue(Dbuffer, 10, 0xAA);
	SetBufferValue(&Dbuffer[10], 10, 0x55);
	SetBufferValue(DataBuffer, 10, 0xAA);
	SetBufferValue(&DataBuffer[10], 10, 0x55);
	
	RESET_WATCH_DOG;
	DFF.BlockErase(FF_COMMON_START_PAGE/FF_PAGE_PER_BLOCK); 
	os_dly_wait(101);
	RESET_WATCH_DOG;
	DFF.PageProgramWaitOK(FF_COMMON_START_PAGE, DataBuffer,3);
	RESET_WATCH_DOG;
	SetBufferValue(DataBuffer, 256, 0);
	os_dly_wait(1);
	DFF.PageRead(FF_COMMON_START_PAGE, DataBuffer);
	RESET_WATCH_DOG;
	if(SearchArray(20,DataBuffer,20,Dbuffer)==0)					//认为Flash错误
	  SelfTestResust |= eST_ER_FLASH;
	RESET_WATCH_DOG;
	DFF.BlockErase(FF_COMMON_START_PAGE/FF_PAGE_PER_BLOCK); 
	RESET_WATCH_DOG;
	os_dly_wait(100);
	DFE.Read(EE_SYS_FIRST_RUN,16,(uchar*)DataBuffer);
	RESET_WATCH_DOG;
	os_dly_wait(1);
	for(i=0;i<16;i++)
	{
		if(MyStrChar((char*)DataBuffer,FirstRunFlag[i]))
			c++;
	}
	NOP();	NOP();
	if(c<10)	  	                     // 认为Fram错误
	  SelfTestResust |= eST_ER_FRAM;
	  
	  
	 /////////////////////////////////////////// 检测外置RFID
	 

   //SelfTestResust = 0x0F;
}
/*************************************************************************************************
** 函数名称: TestFunction()
** 函数功能: 系统测试
** 输入参数: 无
** 输出参数: 无
** 全局变量: 无
** 修改时间: 2009.09.02
洛阳经纬度
$GPGGA,074053.000,3441.1310,N,11224.5050,E,1,08,3.8,175.6,M,0.0,M,,*6A
$GPGLL,3441.1310,N,11224.5050,E,074053.000,A*34
$GNGSA,A,3,20,07,08,09,04,,,,,,,,4.7,3.8,2.8*2E
$GNGSA,A,3,214,209,207,,,,,,,,,,4.7,3.8,2.8*17
$GPGSV,3,1,10,01,61,050,,04,06,220,33,07,23,189,28,08,46,218,29*7D
$GPGSV,3,2,10,09,42,227,32,11,39,050,,19,08,071,,20,30,133,23*78
$GPGSV,3,3,10,28,67,334,,32,26,097,23*7D
$BDGSV,1,1,04,202,00,000,24,207,15,186,16,209,10,181,25,214,21,143,25*63
$GPRMC,074053.000,A,3441.1310,N,11224.5050,E,0.00,0.00,030813,,*0A
$GPVTG,0.00,T,,M,0.00,N,0.00,K*50
$GPZDA,074053.000,03,08,2013,00,00*58
************************************************************************************************/
#define		TEST_FUN			0

#if(TEST_FUN)
char		tsBuffer[1024];
uchar		tdsBuffer[1024+16];
#endif

uint32 	cout_test = 0x2;
void		SystemCls::TestFunction(void)
{
		
	#if(TEST_FUN)
	NOP();	NOP();
	strcpy( (char*)tsBuffer,"*GY,000,S52,153621,IAP1,P,113.108.150.103,21,gye,gye0201,G11BD.gyup,..#\0\0");
	GPRS.ProcessFrameData(0,strlen((char*)tsBuffer),(uchar*)tsBuffer);
	NOP();	NOP();
	#endif
	NOP();	NOP();	
}	    


void		SystemCls::PowerControl(uchar Index,uchar Power)
{
	if((Index==PCONP_ADC)&&(Power==0) )
		LPC_ADC->ADCR &= (1UL<<21);
	if(Power)	LPC_SC->PCONP |= (1UL<<Index);
	else			LPC_SC->PCONP &= (~(1UL<<Index));
}

									  

			
/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */


