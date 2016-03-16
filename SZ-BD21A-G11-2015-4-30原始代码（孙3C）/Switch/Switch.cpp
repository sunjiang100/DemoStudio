/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 
$								  		All rights reserved.	  
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

#define			SWITCH_GLOBALS			1
#include   		"Switch.h"
#include			<string.h>
#include			"..\UserSys.h"
#include			"..\ADC\ADC.h"
#include			"..\UART\UART0.h"
#include			"..\UART\UART1.h"
#include			"..\UART\UART2.h"
#include			"..\UART\UART3.h"
#include			"..\Timer\Timer2.h"

#include			"..\GSM_GPRS\GSM_GPRS.h"

#include			"..\KeyDisp\KeyDisp.h"
#include			"..\Speed\Pulse.h"
#include			"..\FLASH\DFE_25xxx.H"
#include			"..\FLASH\DFF_25xxx.H"
#include			"..\Config\RTOS.h"
#include			"..\SPI\SSP.h"
#include		   "..\PWM\PWM.h"
#include			"..\GPS\GPS.h"
#include 		"..\Accelerometer\kionix_kxtf9_common.h"	
#include			"..\Common\GB_T19056.h"
#include			"..\Common\FLASH_ADDR.H"

static		uchar		volatile		EnableCheckLockDoorSignal;

//static		uint16	volatile		SwCnt[18];	

/*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 
$$ 函数名称：eTask()                                 
$$	函数功能：开关量检测与输出控制
$$	输入变量：无
$$	输出变量	 无
$$	全局变量：
$$	修改时间：
$$	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
void			SwitchCls::eTask(void)										// 10ms调用一次
{
	char		SW32Bit[128];
	uint16	i,SwStatus;
	
	NOP();  NOP();  
	NOP();  NOP();
	TickCnt++;

	if (GG_Status.Flag.Bits.UpdateRemoteSet ==1) return;			 // 远程升级
			
	if(TickCnt>1500)														// 100ms毫秒检测一次输入引脚状态	
	{																				
		SwStatus = ReadIOState();	
		NOP();	NOP();		
		CheckSwitchAlarm(SwStatus);									   // 开关量报警检测 
		NOP();	NOP();
	}
	NOP();	NOP();

	TH_VehicleControlOutput();				

	NOP();	NOP();									  					
	

	if(TickCnt%5==0)															
	{																				
		CheckMotoStop();														// 50ms检测是否有发动机转动信号		
	}

	
	
	Check_Collision();			// 碰撞或侧翻检测	 
	
	if(TickCnt%10==0)															// 每100ms检测一次
	{																				
		
		
		if (!(JT808_Alarm_Word.Shield & 0x01))		      CheckRobAlarm();			// 劫警检测
		//if (!(JT808_Alarm_Word.Shield & (0x01 << 26)))	CheckStealAlarm();		// 盗警检测				//检测外部防盗器输入
		
		//if (!(JT808_Alarm_Word.Shield & (0x01 << 28)))	CheckTrailCarAlarm();	// 拖车检测				非法位移
		//CheckNoSetScanAlarm();												// 未设防检测
		//CheckHelpAlarm();														// 求助报警检测
		//CheckTireDriveAlarm();												// 疲劳驾驶检测
		//CheckIllegalOpenDoor();												// 非法开门检测
		//if (!(JT808_Alarm_Word.Shield & (0x01 << 27)))  CheckIllegalMakeFire();						// 非法点火检测
		//CheckMachineSelfTest();												// 终端自检(24小时重启一次)
		//CheckShakeAlarm();													   // 震动检测
		//CheckLoginAlarm();													   // 登陆密码错误报警检测
		if (!(JT808_Alarm_Word.Shield & (0x01 << 8)))	CheckMainPower();			// 主电源掉电检测	
		
		//CheckUnlockAlarm();													// 门未锁好报警
		//CheckHandleWatchCar();												// 手柄看车检测
		//AutoLockDoor();														// 自动锁门
	}

	
	
	if(TickCnt%100==0)														// 1秒钟检测一次发动机脉冲数
	{
		CheckMotoRunOrNot();										
	}	
	
	if(TD.TestMode==TEST_MODE_AUTO)
	{
		if( TickCnt%(2*100) ==0 )
		{
			SetBufferValue((uchar*)SW32Bit,32,0);
			strcpy((char*)SW32Bit,"GY-TD-SW");
			memmove(&SW32Bit[8],(uchar*)&VehicleStatus,8);
			for(i=8;i<16;i++)	
			{	SW32Bit[i] = ~SW32Bit[i];	}
			SW32Bit[16] = '\r';
			SW32Bit[17] = '\n';
			Uart.putb(1,18,(uchar*)SW32Bit);
		}

	}
	else if((TD.TestMode==TEST_MODE)&&(TickCnt%300==0))
	{
		/*if(SimuSpeed.Enable == ENABLE_FLAG);
		{
         //SimuSpeed.Enable = ENABLE_FLAG;
			Sys.PowerControl(PCONP_PWM1,1);	
			Pwm.SetTargetSpeed(100,12); 

		} */
		SetBufferValue((uchar*)SW32Bit,128,0);
		Uart.puts("\r\n\r\n/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& Switch Test &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/\r\n\0");
		IntToHexChar(HighLowLevel,8,(char*)SW32Bit);
		strcat(SW32Bit,"  ");
		Uart.puts(SW32Bit);
		SetBufferValue((uchar*)SW32Bit,128,0);
		strcpy(SW32Bit,"1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1  \r\n\0");
		for(i=0;i<32;i++)
		{
			if((HighLowLevel&(1UL<<(i)))==0)
			{
				SW32Bit[i<<1] = '0';
			}
		}
		Uart.puts(SW32Bit);
		SetBufferValue((uchar*)SW32Bit,128,0);
		Uart.puts     ("ADSW1  ADSW2   SW3    SW4    SW5    SW6    SW7  ACC  AUTO  BUTTON  BRAKE \r\n\0");
		strcpy(SW32Bit,"  L      L      L      L      L      L      L    OFF  OFF    OFF     OFF  \r\n\0");
		if(DOOR_Status)											{SW32Bit[2]='H';	}
		if(JT808ExtenVsFlag.VS1.Bits.ReversGear)			{SW32Bit[9]='H';	}
		if(Button_Status)										{SW32Bit[16]='H';	}
		if(JT808ExtenVsFlag.VS1.Bits.DippedHeadLight)					{SW32Bit[23]='H'; }	
		if(JT808ExtenVsFlag.VS1.Bits.HighLight)		  {SW32Bit[30]='H';	}
		if(JT808ExtenVsFlag.VS1.Bits.LTurnLight)		  {SW32Bit[37]='H';	}
		if(JT808ExtenVsFlag.VS1.Bits.RTurnLight)			   {SW32Bit[44]='H';	}
		if(ACC_Status)											{SW32Bit[50]='N';	SW32Bit[51]=' ';	}
		if(MOTO_Status)										{SW32Bit[55]='N';	SW32Bit[56]=' ';	}
		if(Button_Status)										{SW32Bit[62]='N';	SW32Bit[63]=' ';	}
		if(JT808ExtenVsFlag.VS1.Bits.Brake)			   {SW32Bit[70]='N';	SW32Bit[71]=' ';	}	
		Uart.puts(SW32Bit);	
	
		os_dly_wait(1);	
	}

			
	#if(0)	//COMPILE_SW_TEST_DATA_CODE)
	if(TD.SW == tSW_32BIT_TO_UART0)
	{
		if( TickCnt%100 ==0 )
		{
			SetBufferValue((uchar*)SW32Bit,128,'0');
			i = 0;
			if(0)																SW32Bit[i] = '1';	 	i++;
			if(0)																SW32Bit[i] = '1';  	i++;
			if(!(SwStatus&0x0100))										SW32Bit[i] = '1';  	i++;
			if(!MainPower_Status)										SW32Bit[i] = '1';  	i++;
			if(Button_Status)												SW32Bit[i] = '1';  	i++;
			if(DOOR_Status)												SW32Bit[i] = '1';  	i++;
			if(MOTO_Status)												SW32Bit[i] = '1';  	i++;
			if(ACC_Status)													SW32Bit[i] = '1';  	i++;
			SW32Bit[i++] = ' ';
			if(SwStatus&0x0080)											SW32Bit[i] = '1';	 	i++;
			if(SwStatus&0x0040)											SW32Bit[i] = '1';  	i++;
			if(SwStatus&0x0020)											SW32Bit[i] = '1';  	i++;
			if(SwStatus&0x0010)											SW32Bit[i] = '1';  	i++;
			if(SwStatus&0x0008)											SW32Bit[i] = '1';  	i++;
			if(SwStatus&0x0004)											SW32Bit[i] = '1';  	i++;
			if(SwStatus&0x0002)											SW32Bit[i] = '1';  	i++;
			if(SwStatus&0x0001)											SW32Bit[i] = '1';  	i++;
			SW32Bit[i++] = ' ';
			/////////////////////////////////////////////////////////////
			if(VehicleStatus.VS8.Bits.IdleMode) 					SW32Bit[i] = '1';    i++;
			if(VehicleStatus.VS6.Bits.VibrationAlarm) 			SW32Bit[i] = '1';    i++;
			if(VehicleStatus.VS1.Bits.Reset) 						SW32Bit[i] = '1';    i++;
			if(VehicleStatus.VS1.Bits.MainPowerLost) 				SW32Bit[i] = '1';    i++;
			if(VehicleStatus.VS1.Bits.GasAndPowerOffStatus) 	SW32Bit[i] = '1';    i++;
			if(VehicleStatus.VS1.Bits.GPRS_Delay) 					SW32Bit[i] = '1';    i++;	
			if(VehicleStatus.VS1.Bits.PasswordErrorAlarm)		SW32Bit[i] = '1';    i++;
			if(VehicleStatus.VS1.Bits.DoorState) 					SW32Bit[i] = '1';		i++;			
			SW32Bit[i++] = ' ';
			/////////////////////////////////////////////////////////////
			if(VehicleStatus.VS2.Bits.LowVoltageAlarm) 			SW32Bit[i] = '1';    i++;
			if(VehicleStatus.VS2.Bits.UseIdleFlag) 				SW32Bit[i] = '1';    i++;
			if(VehicleStatus.VS2.Bits.ParkOverTime) 				SW32Bit[i] = '1';    i++;
			if(VehicleStatus.VS2.Bits.NoScanAlarm) 				SW32Bit[i] = '1';    i++;
			if(VehicleStatus.VS2.Bits.Rsv) 							SW32Bit[i] = '1';    i++;
			if(VehicleStatus.VS2.Bits.TrailCarAlarm) 				SW32Bit[i] = '1';    i++;
			if(VehicleStatus.VS2.Bits.Door2Open) 					SW32Bit[i] = '1';    i++;
			if(VehicleStatus.VS2.Bits.GPS_ModuleError) 			SW32Bit[i] = '1';		i++;
			SW32Bit[i++] = ' ';
			/////////////////////////////////////////////////////////////
			if(VehicleStatus.VS3.Bits.OverSpeed) 					SW32Bit[i] = '1';    i++;
			if(VehicleStatus.VS3.Bits.TireDrive) 					SW32Bit[i] = '1';    i++;
			if(VehicleStatus.VS3.Bits.Moto) 							SW32Bit[i] = '1';    i++;
			if(VehicleStatus.VS3.Bits.HelpAlarm) 					SW32Bit[i] = '1';    i++;
			if(VehicleStatus.VS3.Bits.IlleOpenDoorAlarm) 		SW32Bit[i] = '1';    i++;
			if(VehicleStatus.VS3.Bits.ACC_Off) 						SW32Bit[i] = '1';    i++;
			if(VehicleStatus.VS3.Bits.SetScan) 						SW32Bit[i] = '1';    i++;
			if(VehicleStatus.VS3.Bits.DoorUnlockAlarm) 			SW32Bit[i] = '1';		i++;
			SW32Bit[i++] = ' ';
			/////////////////////////////////////////////////////////////
			if(VehicleStatus.VS4.Bits.OutAreaAlarm) 				SW32Bit[i] = '1';    i++;
			if(VehicleStatus.VS4.Bits.GPS_ANT_Short) 				SW32Bit[i] = '1';    i++;
			if(VehicleStatus.VS4.Bits.GPS_ANT_Open) 				SW32Bit[i] = '1';    i++;
			if(VehicleStatus.VS4.Bits.InAreaAlarm) 				SW32Bit[i] = '1';    i++;
			if(VehicleStatus.VS4.Bits.LawlessPowerOn) 			SW32Bit[i] = '1';    i++;
			if(VehicleStatus.VS4.Bits.OverSpeedAlarm) 			SW32Bit[i] = '1';    i++;
			if(VehicleStatus.VS4.Bits.RobAlarm) 					SW32Bit[i] = '1';    i++;
			if(VehicleStatus.VS4.Bits.StealAlarm) 					SW32Bit[i] = '1';		i++;
			SW32Bit[i++] = '\r';
			SW32Bit[i++] = '\n';
			SW32Bit[i++] = '\0';
			Uart.puts((char*)SW32Bit);
			os_dly_wait(1);
		}		
	}
	#endif
	NOP();	NOP();
		
}



/****************************************************************************************************
** 函数名称：Init()
** 功能描述：开关量初始化
** 入口参数：无
** 出口参数：无
** 全局变量: 无
** 修改时间：2009.08.21   
*****************************************************************************************************/
void		SwitchCls::Init(void)
{
		
	NOP();	NOP();	NOP();	NOP();	
	//DFE.Read(EE_LOCK_FLAG,LEN_LOCK_VEHICLE_FLAG,(uchar *)&VehicleControl);	
	GetVehicleStatus();
	NOP();	NOP();	NOP();	NOP();
	
	TH_VehicleControlOutput();
	
	VehicleStatus.VS3.Bits.ACC = 1;
	NOP();	NOP();	NOP();	NOP();	

	TrailRadius = 0;
	DFE.Read(EE_JT_ELE_RAID_RADIUS,2,(uchar *)&TrailRadius);


}



/***************************************************************************************
**	函数名称：ReadIOState()
** 功能描述：读取输入IO引脚的状态
** 入口参数：无
** 出口参数：成功返回0
** 全局变量: SP_State,SG1_State
** 创建时间：2009.08.21
** 修改时间；
** 说明：SP_State,SG1_State的位与外部引脚相对应。其中,SP组IO口引脚为高电平,SP_State相对应
** 的位为1,否则为0;SG1组IO口引脚为低电平,SG1_State相对应的位为1,否则为0;
****************************************************************************************/
uint32		MainPowerLowCnt,MainPowerHighCnt;

uint16		SwitchCls::ReadIOState()				// 100ms									  		
{
	uint32	i;
	uint16	th,tl,AdcSW[16]={0};
	union
	{
		uint32	B[4];
		uint16	B16[8];
		uchar		B8[16];	
	}Tp;
	uchar		Index;
	
	/////////// 读取I/O引脚 ////////////////////////////
	GET_IO_PIN_INIT;
	ACC_Status		= !GET_ACC_STATUS;								  // 点火时,标志为1		SW1
	MOTO_Status 	= !GET_MOTO_STATUS;							  // 发动时,标志为1		 SW3
	Button_Status 	= !GET_BUTTON_STATUS;		//EMButton_Statu;      //!GET_BUTTON_STATUS;		紧急按钮状态，>2s才起作用					  
	
	Adc.GetAllResult(AdcSW);

   	// added 2013.3
	JT808Analog1 = AdcSW[3];    //  
	JT808Analog2 = AdcSW[2];    // 主电源

	//

	if(AdcSW[1]<1100)		  //缺纸判断
	{
	   PrintPaperStu = 0;		// 缺纸
	}
	else
	{
		PrintPaperStu = 1;
	}

	if (AdcSW[2] < 6000) //  掉电， JT808
	{
		MainPower_Status = 0;
		DOOR_Status = 1;  // 主电源掉电，门磁一直认为关门
	}
	else	
	{
		MainPower_Status = 1;
	}
	
	if(MainPowerValue>180)			LowPowerAlarmValue = 180;			// 主电源大于16V，认为是24V的车
	else if(MainPowerValue<170)	LowPowerAlarmValue = 110;			// jt808要求， 24V车，默认17V低压保护 
		
	if( (TD.ADC == tADC_MAIN_POWER ) && ((TickCnt%200)==0) )
	{
		Uart.puts("Main power: ");
		Int2Str( MainPowerValue, (char*)Tp.B8 );
		Uart.puts((char*)Tp.B8);
		Uart.puts("   Power low Alarm value: ");
		Int2Str( LowPowerAlarmValue,(char*)Tp.B8 );
		Uart.puts((char*)Tp.B8);
	}

	if( ((MainPowerValue<110)&&(MainPowerValue>80)) || ((MainPowerValue<210)&&(MainPowerValue>170)) )
	{		
		MainPowerLowCnt++;
		if(MainPowerLowCnt>(15*100L))		  	// 10mS调用一次
		{
			if (!(JT808_Alarm_Word.Shield & (0x01 << 7)))
			{
				if(JT808Flag.VS5.Bits.MainPowrLow == 0)
				{
					//GPRS_TempPack = TRUE;
					JT808_GPRS_TempPack = TRUE;
					#if (WUXI_TEST !=1)
					Uart.puts("TP. Main power low");
					#endif
					JT808Flag.VS5.Bits.MainPowrLow = 1;
					VehicleStatus.VS6.Bits.LowPowerAlarm = 1;
					Index = 7;
			
					if (JT808_Alarm_Word.SendTextSMS & (0x01 << Index))		     								// 发送短信
					{
						GSM.SetSMS_Buffer((char *)&JT808_Number.TextSMS[0],"Main Power Low Alarm!!!");		
					}
				
				   if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))
				   {
			  			if (!(JT808_Alarm_Word.SavePhoto & (0x01 << Index)))	Jt_photo_upload = 1;		  // 上传到平台
						MediaEventFlag =eBB_MEDIA_EVENT_OTHER;
			  			Camera.SetTakeNumber(2,TRUE);		  // 拍照 
		  		   }
					
				}
			}

			PowerSelfLock = 0;								
		}
	}
	else
	{
		if(MainPowerLowCnt)	MainPowerLowCnt--;
	}

	if(MainPowerValue>(LowPowerAlarmValue+2))
	{
		MainPowerHighCnt++;

	  	if(MainPowerHighCnt>(15*100L))		  	// 10mS调用一次
		{
			//if(VehicleStatus.VS6.Bits.LowPowerAlarm == 1)
			if(JT808Flag.VS5.Bits.MainPowrLow == 1)
			{
				//GPRS_TempPack = TRUE;
				JT808_GPRS_TempPack = TRUE;
				#if (WUXI_TEST !=1)
				Uart.puts("TP. Main power normal");
				#endif
			}
			VehicleStatus.VS6.Bits.LowPowerAlarm = 0;
			JT808Flag.VS5.Bits.MainPowrLow = 0;
			PowerSelfLock = TRUE;								
		}
	}
	else
	{
		if(MainPowerHighCnt)	MainPowerHighCnt--;
	}

	Tp.B16[0] = 0;
	//////////////////////////////////
	if(AdcSW[2]<18000) 		// 12V车
	{
		th = 8000;
		tl = 4000;
	}
	else						// 24V车
	{
		th = 18000;
		tl = 8000;
	}
	for(i=0;i<2;i++)		// 两路模拟量
	{				
		Tp.B8[0] >>= 1;
		if((HighLowLevel&(1UL<<i))!=0)			// 高电平有效
		{
			if(AdcSW[i+3]>th)
				Tp.B8[0] |= 0x80;
		}
		else												// 低电平有效
		{
			if(AdcSW[i+3]<tl)
				Tp.B8[0] |= 0x80;
		}
		
	}

	
	/////////////////////////////////////////////////////////// 正负触电发输入
	//	 模拟量
	
	if(Tp.B16[0]&0x0001)			// Adc04		门						   //
	{   
		 DOOR_Status = 1;
		 if (JT808Flag.VS2.Bits.Door1 == 0);
		 {
		 	JT808Flag.VS2.Bits.Door1 = 1;
		 	//Uart.puts("开门\r\n\0");//os_dly_wait(1);
		 }
		 VehicleStatus.VS3.Bits.DoorOpen = 1;
	}
	else
	{
		DOOR_Status = 0;
		if (JT808Flag.VS2.Bits.Door1 == 1);
		{
		 	JT808Flag.VS2.Bits.Door1 = 0;
		 	//Uart.puts("关门\r\n\0");os_dly_wait(1);
		}
		VehicleStatus.VS3.Bits.DoorOpen = 0;
	}
	
	if(Tp.B16[0]&0x0002)				// 	Adc05 	倒档	
	{
		JT808ExtenVsFlag.VS1.Bits.ReversGear = 1;
	}
	else
	{
		JT808ExtenVsFlag.VS1.Bits.ReversGear = 0;
	}

	/*if(Tp.B16[0]&0x0004)									 //喇叭		  硬件没有对应的接口，所以这里屏蔽
		J808ExtenVsFlag.VS2.Bits.Speaker = 1;
	else
	   J808ExtenVsFlag.VS2.Bits.Speaker = 0;	*/
   // 

	/////// 开关量 ///////////////////////////////////
	if(!GET_BRAKE_STATUS)			// SW2				 // 制动	 
		JT808ExtenVsFlag.VS1.Bits.Brake = 1;
	else						
		JT808ExtenVsFlag.VS1.Bits.Brake = 0;

	if(!GET_DIPPED_BEAM)			 // 	 SW4	 // 近光灯	  
		JT808ExtenVsFlag.VS1.Bits.DippedHeadLight = 1;
	else
		JT808ExtenVsFlag.VS1.Bits.DippedHeadLight = 0;		 

	// modified 2015.1.8
	if(!GET_HIGH_BEAM)			//		SW5	 //  远光灯
	   JT808ExtenVsFlag.VS1.Bits.HighLight = 1;
	else
	  JT808ExtenVsFlag.VS1.Bits.HighLight = 0;

	if(!GET_LEFT_LIGHT)		// SW6							 //左转向灯 
	   JT808ExtenVsFlag.VS1.Bits.LTurnLight = 1;
	else
	   JT808ExtenVsFlag.VS1.Bits.LTurnLight = 0;

	if(!GET_RIGHT_LIGHT)					// SW7				 //右转向灯	
	   JT808ExtenVsFlag.VS1.Bits.RTurnLight = 1;
	else
	   JT808ExtenVsFlag.VS1.Bits.RTurnLight = 0;

		
	return Tp.B16[0];
}




/****************************************************************************************************
** 函数名称：CheckMotoStop()
** 功能描述：发动机停止检测
** 入口参数：无
** 出口参数：无
** 全局变量: 无
** 修改时间：2009.08.21   
*****************************************************************************************************/ 
void		SwitchCls::CheckMotoStop(void)								  //50ms
{
	if(MOTO_Status)	MotoRotateSpeedCnt++;	
}



/****************************************************************************************************
**	函数名称：CheckMotoRunOrNot
** 功能描述：检测发动机是否在工作
** 入口参数：无
** 出口参数：无
** 全局变量: 无
** 修改时间：
****************************************************************************************************/ 	
void			SwitchCls::CheckMotoRunOrNot(void)								// 1秒种调用一次
{
	
	char		Puls[16];
	
	static	volatile	 uint32	MotoRunSecondCnt;
	NOP();	NOP();	NOP();
		
	if((MotoRotateSpeedCnt>10))		// 10个脉冲以上或维持高电平2S以上		 //||MOTO_Statu)
	{
		MotoRotateSpeedCnt = 0;
		VehicleStatus.VS3.Bits.Moto = 1;
		if ( JT808Flag.VS1.Bits.Operations == 0)
		{
			JT808_GPRS_TempPack = TRUE;
			Uart.puts("TP. Moto off");
		}

		JT808Flag.VS1.Bits.Operations = 1;		  //运营状态
		MotoRunFlag = 1;
		MotoRunSecondCnt++;

		if(MotoRunSecondCnt>=60)
		{
			MotoRunSecondCnt = 0;						
			SystemLog.VDR.MotoRunTimeCnt += 60;	// 此处不存储,每隔30秒统一存储
		}
	}		
	else
	{
		MotoRunFlag = 0;
		VehicleStatus.VS3.Bits.Moto = 0;
		if (JT808Flag.VS1.Bits.Operations == 1)
		{
			JT808_GPRS_TempPack = TRUE;
			Uart.puts("TP. Moto off");
		}

		JT808Flag.VS1.Bits.Operations = 0;		 //停运状态
	}

	///////////////////////////////////////////////////////////////////////
	if(TD.SW == tSW_MOTO_PULS_CNT)
	{
		if(MotoRotateSpeedCnt)
		{
			Int32ToDecChar(MotoRotateSpeedCnt,Puls);
			strcat(Puls,"\r\n\0");	
		}
		else
		{
			strcpy(Puls,"Moto Stop\r\n\0");
		}
		Uart.puts(Puls);
		os_dly_wait(2);
	}
}


/********************************************************************************************
**	函数名称：CheckSwitchAlarm()
** 功能描述：开关量报警检测
** 入口参数：IO引脚状态SwStatus
** 出口参数：无
** 全局变量: 无
** 修改时间：2009.09.04  
*********************************************************************************************/ 
void		SwitchCls::CheckSwitchAlarm(uint16  SwStatus)
{
	static	volatile	 	uchar   AccPreStatus;
	static	volatile	 	uint32	AccChangeCnt,StartCnt=0;


	NOP();	NOP();	NOP();	NOP();
	StartCnt++;
	///////////// ACC /////////////////////
	if(ACC_Status==0)
	{
		AccChangeCnt = 0;
		// 系统启动11秒以后，ACC由开到关，检测锁门信号
		if((AlarmCnt.ACC==100)&&(AccPreStatus==1)&&(StartCnt>(ALARM_DELAY_TIME+100)))	
		{
			if(TD.GPS==tGPS_TEMP_PACK)
			{
				Uart.puts("GTP ACC Off\r\n\0");
			}
		}
		VehicleStatus.VS3.Bits.ACC = 1;
		JT808Flag.VS1.Bits.ACC = 0;
		if(++AlarmCnt.ACC==ALARM_DELAY_TIME)
		{
			if(AccPreStatus==1)		// 有变化，报警	 ACC 不需要报警
			{
				JT808_GPRS_TempPack = TRUE;
				Uart.puts("TP. ACC on ot off");
				//GPRS_TempPack = TRUE;
				AccPreStatus = 0;
				//if(StartCnt>(5*10*10))		Camera.SetTakeNumber(2,TRUE);
				if(TD.GPS==tGPS_TEMP_PACK)
				{
					Uart.puts("GTP ACC OFF\r\n\0");
				}
			}			
		}							 //ACC关且无按键操作时，60秒后进入休眠关掉除通信模块外其他外设电源，包括GPS模块	来电会唤醒休眠，通话结束继续进入休眠
		if((AlarmCnt.ACC>6*ALARM_DELAY_TIME)&&(IDEL_Dly_Cnt++>5*ALARM_DELAY_TIME) && (PhoneCall.Busing != TRUE))		
		{
		   IDEL_For_Acc_OFF = TRUE;
		}
	}
	else
	{
		AlarmCnt.ACC = 0;
		VehicleStatus.VS3.Bits.ACC = 0;
		JT808Flag.VS1.Bits.ACC = 1;
		if(++AccChangeCnt==ALARM_DELAY_TIME)
		{
			
			if(AccPreStatus==0)		// 有变化，报警
			{
				JT808_GPRS_TempPack = TRUE;
				Uart.puts("TP. ACC off to on");
				//GPRS_TempPack = TRUE;
				//if(StartCnt>(50*10*10))		Camera.SetTakeNumber(2,TRUE);
				if(TD.GPS==tGPS_TEMP_PACK)
				{
					Uart.puts("GTP ACC ON\r\n\0");
				}
				AccPreStatus = 1;
			}
 		   IDEL_For_Acc_OFF = FALSE;

			if (JT_Ctr_PowerDown == 1)
			{
				JT_Ctr_PowerDown == 0;             
				NVIC_SystemReset();					  // 重启
			}
			
		}
		

		
	}

	if(SetScanAlarmFlag==1)					
		VehicleStatus.VS6.Bits.UndefendAlarm = 1;
	else
		VehicleStatus.VS6.Bits.UndefendAlarm = 0;


	/////////////////////////////////////////////////////////////////
	

}


/**************************************************************************************************
** 函数名称: TH_VehicleControlOutput()
** 函数功能: 开关量输出控制
** 输入参数: 无
** 输出参数: 无
** 全局变量: 无
** 修改时间: 2009.08.31
***************************************************************************************************/
void			SwitchCls::TH_VehicleControlOutput(void)						// 100ms
{
	static	uchar			PulsStatus;
	static	volatile		uint32 	LastTime,TestDelayCnt,LastTime2;	 


	if(TD.SW == tSW_OUT_PUT_HIGH)
	{
		ALL_OUTPUT_HIGH;
		return;
	}
	else if(TD.SW == tSW_OUT_PUT_LOW)
	{
		ALL_OUTPUT_LOW;
		return;
	}
	///////////////////////////////////////////////////////	
	if((TD.SW == tSW_TEST_DELAY)||(TD.TestMode==TEST_MODE)||(TD.TestMode==TEST_MODE_AUTO))
	{
		

		TestDelayCnt++;
		if(TestDelayCnt%200==0)
		{
			STOP_OIL_CONTROL;
			//CONTROL_OUT3;              //ALL_OUTPUT_HIGH;
			CONTROL_OUT2;
			CONTROL_OUT1;
		}
		else if(TestDelayCnt%100==0)
		{
		    REGAIN_OIL_CONTROL;
			 //NULL_CONTROL_OUT3;	   //ALL_OUTPUT_LOW;
			 NULL_CONTROL_OUT2;
		    NULL_CONTROL_OUT1;
		}
		return;
	}	
	


	if( MotoRunFlag )		// 如需发动机工作时不锁车，请设置VehicleControl.Lock = LOCK_VEHICLE_ENABLE_INV;
	{
		if(VehicleControl.Lock==LOCK_VEHICLE_ENABLE)	
		{
			//if(VehicleStatus.VS1.Bits.GasAndPowerOffStatus == 0)
			  if(JT808Flag.VS2.Bits.OffE==0)
			  {
				//	GPRS_TempPack = TRUE;
				   JT808_GPRS_TempPack = TRUE;
					Uart.puts("TP. Moto on.Gas off= 1");
				}
			VehicleStatus.VS1.Bits.GasAndPowerOffStatus = 1;
			JT808Flag.VS2.Bits.OffE = 1;
			JT808Flag.VS2.Bits.OffOil = 1;
		}
		else 
		{
			if(VehicleControl.Lock == LOCK_VEHICLE_DISABLE)
			{
				//if(VehicleStatus.VS1.Bits.GasAndPowerOffStatus == 1)
				   if(JT808Flag.VS2.Bits.OffE==1)
					{
						//GPRS_TempPack = TRUE;
					  JT808_GPRS_TempPack = TRUE;
					  Uart.puts("TP. Gas off = 1.1");
					}
				  VehicleStatus.VS1.Bits.GasAndPowerOffStatus = 0;
				  JT808Flag.VS2.Bits.OffE = 0;
				  JT808Flag.VS2.Bits.OffOil = 0;
			}	 
		}
	}	
	else		// 发动机停止
	{
		if((VehicleControl.Lock==LOCK_VEHICLE_ENABLE)||\
		   (VehicleControl.Lock==LOCK_VEHICLE_ENABLE_INV) )
		{	
			//if(VehicleStatus.VS1.Bits.GasAndPowerOffStatus == 0)
					//GPRS_TempPack = TRUE;
			VehicleStatus.VS1.Bits.GasAndPowerOffStatus = 1;
			if(JT808Flag.VS2.Bits.OffE==0)
			{
				//JT808_GPRS_TempPack = TRUE;
				  JT808_GPRS_TempPack = TRUE;
				  Uart.puts("TP. Gas off=1");
			}
			JT808Flag.VS2.Bits.OffE = 1;
			JT808Flag.VS2.Bits.OffOil = 1;
		}
		else
		{
		   if(VehicleControl.Lock == LOCK_VEHICLE_DISABLE)
			{
				//if(VehicleStatus.VS1.Bits.GasAndPowerOffStatus == 1)
					//GPRS_TempPack = TRUE;
				VehicleStatus.VS1.Bits.GasAndPowerOffStatus = 0;

				if(JT808Flag.VS2.Bits.OffE==1)
				{
				//JT808_GPRS_TempPack = TRUE;
				  JT808_GPRS_TempPack = TRUE;
				  Uart.puts("TP. Gas off = 0");
				}
			   JT808Flag.VS2.Bits.OffE = 0;
				JT808Flag.VS2.Bits.OffOil = 0;
			}
		}
	}
	
	
	//if(VehicleStatus.VS1.Bits.GasAndPowerOffStatus == 1)
	
	/*if(SimuSpeed.Enable==ENABLE_FLAG)							   //当模拟行车时，这路输出用作刹车输出
	{	  		
		if(SimuSpeed.Brake)	{	SIMU_BRAKE_OUT_ON;	}
		else						{	SIMU_BRAKE_OUT_OFF;	}
	}
	else */
	if(JT808Flag.VS2.Bits.OffOil==1)
	{	STOP_OIL_CONTROL;	}
	else
	{	REGAIN_OIL_CONTROL;	}
	
	
	
	///////////////////// 其它５路输出，直接使用管理中心命令，输出电平或脉冲//////////////////////////////
	//////////////////////////////////////////  控制输出１
	if(VehicleControl.ControlOut1==0)			// 输出有效
	{
		PulsStatus = 1;
	}
	else if(VehicleControl.ControlOut1>MAX_PLUS_PERIOD)	// 输出无效
	{
		PulsStatus = 0;
	}
	else			// 方波输出
	{
		if(TickCnt%VehicleControl.ControlOut1==0)		// PulsOutEnable是周期，单位10ms
		{
			PulsStatus = !PulsStatus;		// 取反
		}
	}
	
		
	
//	if(SimuSpeed.Enable!=ENABLE_FLAG)			  //当模拟行车时，这路输出用作速度脉冲输出
//	{	 
//		if(PulsStatus)
//		{
//			CONTROL_OUT1;
//		}
//		else
//		{
//			NULL_CONTROL_OUT1;
//		}
//	}
	//////////////////////////////////////////  控制输出１
	if(VehicleControl.ControlOut2==0)			// 输出有效
	{
		PulsStatus = 1;
	}
	else if(VehicleControl.ControlOut2>MAX_PLUS_PERIOD) 	// 输出无效
	{
		PulsStatus = 0;
	}
	else			// 方波输出
	{
		if(TickCnt%VehicleControl.ControlOut2==0)		// PulsOutEnable是周期，单位10ms
		{
			PulsStatus = !PulsStatus;		// 取反
		}
	}	
	
		
//	if(PulsStatus)
//	{
//		CONTROL_OUT2;
//	}
//	else
//	{
//		NULL_CONTROL_OUT2;
//	}

	//////////////////////////////////////////  控制输出3
	if(VehicleControl.ControlOut3==0)			// 输出有效
	{
		PulsStatus = 1;
	}
	else if(VehicleControl.ControlOut3>MAX_PLUS_PERIOD)	// 输出无效
	{
		PulsStatus = 0;
	}
	else			// 方波输出
	{
		if(TickCnt%VehicleControl.ControlOut3==0)		// PulsOutEnable是周期，单位10ms
		{
			PulsStatus = !PulsStatus;		// 取反
		}
	}				
//	if(PulsStatus)
//	{
//		CONTROL_OUT3;
//	}
//	else
//	{
//		NULL_CONTROL_OUT3;
//	}

		
		
}


//////////////////////////// 以下是报警检测部分代码 //////////////////////////////////////////////


/*************************************************************************************************
** 函数名称: CheckRobAlarm()
** 函数功能: 劫警检测
** 输入参数: 无
** 输出参数: 无
** 全局变量: 无
** 创建时间：2009.09.02
** 修改时间: 
************************************************************************************************/
void		SwitchCls::CheckRobAlarm()
{
	static	uchar		Pressflag=0,Alarmflag=0;
	static	uint32	LastTime;
	uchar		Index = 0;   // 对应设置报警字的第几位


	//if(Enable.EN1.Bits.RobAlarm==1)																			// 如果允许使用劫警		JT808 始终允许
	{
		//if(VehicleStatus.VS4.Bits.RobAlarm==0)													// 未发生劫警
		if(JT808Flag.VS5.Bits.EmergencyAlarm==0)
		{
			
			if((MainPower_Status !=0) && (Button_Status==1)&&(Pressflag==0))		// 如果主电源开，并且按下紧急按钮, 
			{						
				Pressflag = 1;
				LastTime = gCnt.SystemSecond;
	
				if(Alarmflag==0)
				{
					Alarmflag=1;
					
					VehicleStatus.VS4.Bits.RobAlarm = 1;
					JT808Flag.VS5.Bits.EmergencyAlarm=1;							
					//GPRS_TempPack = TRUE;
					JT808_GPRS_TempPack = TRUE;
					Uart.puts("TP. Energency alarm");
					#if (  TEST_CODE_JUST_FOR_TEST_LUOYANG != 1 )
					//sdfsaf
					if (JT808_Alarm_Word.SendTextSMS & (0x01 << Index))		     								// 发送短信
					{
						GSM.SetSMS_Buffer((char *)&JT808_Number.TextSMS[0],"Emergency	Alarm!!!");		
					}
					//if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))	Camera.SetTakeNumber(2,TRUE);		  // 拍照
					if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))
				   {
			  			if (!(JT808_Alarm_Word.SavePhoto & (0x01 << Index)))	Jt_photo_upload = 1;		  // 上传到平台
						MediaEventFlag = eBB_MEDIA_EVENT_ROB;
			  			Camera.SetTakeNumber(2,TRUE);		  // 拍照 
		  		   } 
					#endif	

					Uart.puts("GTP rob  Alarm\r\n\0");				  																																								// 劫警发生时拨号标志
				 }
			 }
		}
		else
		{
		 	Alarmflag=0;
			if((Button_Status==1)&&(Pressflag==0))									// 如果按下紧急按钮
			{
				Pressflag = 1;
				LastTime  = gCnt.SystemSecond;
				
				//VehicleStatus.VS4.Bits.RobAlarm=0;
				
				JT808Flag.VS5.Bits.EmergencyAlarm=0;		
				//$$$$$$$$$$$$$ Test $$$$$$$$$$$$$$$$$$$$$$$$
				#if		__SWITCH_DEBUG__
					if(TD.SW==tSW_CHECK_ALARM)
						Uart.puts("Cancel Rob Alarm!\r\n");
				#endif
				//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
			}
		}
	}
	

	if(Pressflag==1)
	{
	 	if((gCnt.SystemSecond-LastTime)>10)											// 10秒后,才允许再按下按钮
		{
			Pressflag=0;
		}
	}

}

/*************************************************************************************************
** 函数名称: CheckStealAlarm()
** 函数功能: 盗警检测
** 输入参数: 无
** 输出参数: 无
** 全局变量: 无
** 创建时间：2009.09.04
** 修改时间: 2009.09.10
************************************************************************************************/
void		SwitchCls::CheckStealAlarm()
{	
	
	#if ( TEST_CODE_JUST_FOR_TEST_LUOYANG == 1)
	NOP();	NOP();
	#else
	//fdsa
	uchar		Index = 26;
							 
	
	 if(StealAlarmStatus)
	 {
	 	if(JT808Flag.VS8.Bits.StealAlarm==0)
	 	{  
		  JT808Flag.VS8.Bits.StealAlarm = 1;
		  VehicleStatus.VS4.Bits.StealAlarm = 1;
		  JT808_GPRS_TempPack = TRUE;
		  Uart.puts("TP. Steal alarm");
		  //if (JT808_Alarm_Word.SendTextSMS & (1 << Index))	
		  #if ( TEST_CODE_JUST_FOR_TEST_LUOYANG==1)	  // 应付测试使用
			NOP();	NOP();
			#else
		//	jflsja f
		  if ((JT808_Alarm_Word.SendTextSMS >> Index) & 0x01 )	     								// 发送短信
		  {
			 GSM.SetSMS_Buffer((char *)&JT808_Number.TextSMS[0],"Steal	Alarm!!!");		
		  }
		  //if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))	Camera.SetTakeNumber(2,TRUE);		  // 拍照 
		  if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))
		  {
	  			if (!(JT808_Alarm_Word.SavePhoto & (0x01 << Index)))	Jt_photo_upload = 1;		  // 上传到平台
				MediaEventFlag =eBB_MEDIA_EVENT_OTHER;
	  			Camera.SetTakeNumber(2,TRUE);		  // 拍照 
  		  }

		  if(TD.GPS==tGPS_TEMP_PACK)
		  {
		  	 Uart.puts("GTP steal  Alarm\r\n\0");
		  }
		  RESET_WATCH_DOG;
		  os_dly_wait(20);
		  RESET_WATCH_DOG;
		  os_dly_wait(20);
		  WhichMessage = 1;
		  SendMessageFlag = 1;
		  //$$$$$$$$$$$$$ Test $$$$$$$$$$$$$$$$$$$$$$$$
			#if		__SWITCH_DEBUG__
				if(TD.SW==tSW_CHECK_ALARM)
					Uart.puts("Steal Alarm!\r\n");
			#endif
			#endif
		  //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

	 	 }
	 }
	 else
	 {
	   if(JT808Flag.VS8.Bits.StealAlarm == 1)
		{
		  JT808_GPRS_TempPack = TRUE;
		  Uart.puts("TP. Cancel steal alarm");
		}

	 	JT808Flag.VS8.Bits.StealAlarm = 0;
		VehicleStatus.VS4.Bits.StealAlarm = 0;
    }
	//if(Enable.EN1.Bits.StealAlarm==1)																// 如果允许盗警检测,且是设防状态	  JT808 默认检测外部报警信号
   if(0)
	{
		if(SetScanAlarmFlag==1)
		{	if(ACC_Status==1)												   			// 如果出现点火
			{
				if(VehicleStatus.VS4.Bits.StealAlarm==0)
				{
					VehicleStatus.VS4.Bits.StealAlarm = 1;
					GPRS_TempPack = TRUE;
					if(TD.GPS==tGPS_TEMP_PACK)
					{
						Uart.puts("GTP steal  Alarm\r\n\0");
					}
					RESET_WATCH_DOG;
					os_dly_wait(20);
					RESET_WATCH_DOG;
					os_dly_wait(20);
					WhichMessage = 1;
					SendMessageFlag = 1;
					//$$$$$$$$$$$$$ Test $$$$$$$$$$$$$$$$$$$$$$$$
					#if		__SWITCH_DEBUG__
						if(TD.SW==tSW_CHECK_ALARM)
							Uart.puts("Steal Alarm!\r\n");
					#endif
					//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
				}																
			}			
		}
		else
		{
			VehicleStatus.VS4.Bits.StealAlarm = 0;
																			// 撤防后,关闭报警喇叭
		}
	}
	#endif

}


/*************************************************************************************************
** 函数名称: CheckTrailCarAlarm()
** 函数功能: 拖车检测							
** 输入参数: 无
** 输出参数: 无
** 全局变量: 无
** 创建时间：2009.09.04
** 修改时间: 2009.09.10
注：JT808技术要求ACC关GPS模块进入关机状态，这样就不能检测拖车报警了，所以，这里等于摆设
************************************************************************************************/
uint16	SpeedCnt;
void		SwitchCls::CheckTrailCarAlarm()
{
	#if ( TEST_CODE_JUST_FOR_TEST_LUOYANG == 1)
	NOP();	NOP();
	#else

	uchar			Index = 28;
	uint16		speed;
	static uint32		Pre_Mileage  = 0xfffffffe;
	uint32 Tmp;
	//fdsfsaf
	//if(Enable.EN1.Bits.TrailCarAlarm==1)					// 如果允许拖车检测		 JT808 默认检测拖车
	{
		//if(VehicleStatus.VS6.Bits.CarBeTrail==0
		if(JT808Flag.VS8.Bits.IllegalMove==0)										  		
		{
			if((GPSValidFlag==GPS_VALID_FLAG)&&(ACC_Status==0))			// 定位有效,且ACC熄火
			{
				/*speed = GPS.GetSpeedX100();
				if(speed>500)										  // 如果出现车辆位置移动
				{
					SpeedCnt++;
				}
				else
					SpeedCnt = 0;
				if( (SpeedCnt>200))//&&(speed!=200)&&(speed!=300) )	// 拖车检测.(海里/小时)		
				{*/
				
				Tmp = GPS.GetMileage();
				if (Pre_Mileage < Tmp + TrailRadius)	
				{
					//VehicleStatus.VS6.Bits.CarBeTrail = 1;
					JT808Flag.VS8.Bits.IllegalMove = 1;	
					VehicleStatus.VS6.Bits.CarBeTrail = 1;
					WhichMessage = 3;
					SendMessageFlag = 1;
					//GPRS_TempPack = TRUE;
					JT808_GPRS_TempPack = TRUE;
					Uart.puts("TP. trail car");
					#if ( TEST_CODE_JUST_FOR_TEST_LUOYANG==1)	  // 应付测试使用
					NOP();	NOP();
					#else
					//jflsja f
					if (JT808_Alarm_Word.SendTextSMS & (0x01 << Index))		     								// 发送短信
					{
						GSM.SetSMS_Buffer((char *)&JT808_Number.TextSMS[0],"Trail Car Alarm!!!");		
					}
					//if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))	Camera.SetTakeNumber(2,TRUE);		  // 拍照
					if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))
				   {
			  			if (!(JT808_Alarm_Word.SavePhoto & (0x01 << Index)))	Jt_photo_upload = 1;		  // 上传到平台
						MediaEventFlag =eBB_MEDIA_EVENT_OTHER;
			  			Camera.SetTakeNumber(2,TRUE);		  // 拍照 
		  		   } 

					if(TD.GPS==tGPS_TEMP_PACK)
					{
						Uart.puts("GTP trail car  Alarm\r\n\0");
					}
					//$$$$$$$$$$$$$ Test $$$$$$$$$$$$$$$$$$$$$$$$
					#if		__SWITCH_DEBUG__
						if(TD.SW==tSW_CHECK_ALARM)
							Uart.puts("Trail Car Alarm!\r\n");
					#endif
					#endif
					//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

				
				}
				
				Pre_Mileage = Tmp;
			}
		}
	}
	#endif
	
}


/*************************************************************************************************
** 函数名称: CheckNoSetScanAlarm()
** 函数功能: 未设防检测
** 输入参数: 无
** 输出参数: 无
** 全局变量: 无
** 创建时间：2009.09.04
** 修改时间: 
************************************************************************************************/
void		SwitchCls::CheckNoSetScanAlarm()					 							// 50ms
{
	#if ( TEST_CODE_JUST_FOR_TEST_LUOYANG == 1)
	NOP();	NOP();
	#else
	static	uchar		flag=0;
	static	uint32	LastTime;
	
	if(EnableFlag.EN1.Bits.NoSetScanAlarm==1)																// 如果允许未设防检测
	{
		if((SetScanAlarmFlag==0)&&(VehicleStatus.VS3.Bits.ACC==1))					// 停车未设防
		{
			if(VehicleStatus.VS6.Bits.UndefendAlarm==0)										// 如果没有发生未设防报警
			{
				if((LockDoorSignal==1)&&(flag==0))
				{
					LastTime = gCnt.SystemSecond;
					flag = 1;
					//$$$$$$$$$$$$$ Test $$$$$$$$$$$$$$$$$$$$$$$$
					#if		__SWITCH_DEBUG__
						if(TD.SW==tSW_CHECK_ALARM)
							Uart.puts("Inter Check NoSet Scan...\r\n");
					#endif
					//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
				}
		
				if(((gCnt.SystemSecond-LastTime)>15)&&(flag==1))											// 15秒
				{		
					VehicleStatus.VS6.Bits.UndefendAlarm = 1;								// 发生报警
					GPRS_TempPack = TRUE;
					if(TD.GPS==tGPS_TEMP_PACK)
					{
						Uart.puts("GTP No Set Scan  Alarm\r\n\0");
					}
					//$$$$$$$$$$$$$ Test $$$$$$$$$$$$$$$$$$$$$$$$
					#if		__SWITCH_DEBUG__
						if(TD.SW==tSW_CHECK_ALARM)
							Uart.puts("No Set Scan Alarm!\r\n");
					#endif
					//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$	 																		// 发生报警，打开报警喇叭
				}
			}
		}
		else
		{
			flag = 0;
			VehicleStatus.VS6.Bits.UndefendAlarm = 0;
		}
	}
	#endif
}


/*************************************************************************************************
** 函数名称: CheckHelpAlarm()
** 函数功能: 求助报警检测
** 输入参数: 无
** 输出参数: 无
** 全局变量: 无
** 创建时间：2009.09.04
** 修改时间: 
************************************************************************************************/
void		SwitchCls::CheckHelpAlarm()					 							// 50ms
{
	uchar		i;
	static	uchar		PressFlag=0,AlarmFlag=0;
	static	uint32	LastTime;
	
	if(EnableFlag.EN1.Bits.HelpAlarm==1)																// 如果允许求助报警检测
	{
		if(GG_Status.Registration!=TRUE)	return ;
		
		if(((Button_Status==1)||(WakeUpType==WAKE_TYPE_HELP))&&(PressFlag==0))							// 如果按下紧急按钮
		{						
			PressFlag = 1;
			LastTime = gCnt.SystemSecond;

			if(AlarmFlag==0)
			{
				AlarmFlag=1;
				VehicleStatus.VS6.Bits.CallHelpAlarm = 1;								// 求助报警
				VehicleStatus.VS4.Bits.RobAlarm = 1;
				GPRS_TempPack = TRUE;
				if(TD.GPS==tGPS_TEMP_PACK)
				{
					Uart.puts("GTP help Alarm\r\n\0");
				}
				////////////////////////////////////////////////////
				for(i=0;i<10;i++)
				{
					RESET_WATCH_DOG;
					os_dly_wait(50);
				}
				//Uart.puts("准备救助拨号...\r\n\0");	Uart.PopAlignmentSend();
				GPRS.DialHelpPhone();													// 拨打预设好的求助电话
			}
			else
			{
			 	AlarmFlag=0;
				VehicleStatus.VS6.Bits.CallHelpAlarm = 0;								// 求助报警
				VehicleStatus.VS4.Bits.RobAlarm = 0;
				for(uchar i=0;i<8;i++)									 				// 必要时加入挂机
				{																		 
				 	Uart1.puts("ath\r\n");
					os_dly_wait(10);
				}
			}
			for(uchar i=0;i<10;i++)
			{
				RESET_WATCH_DOG;
				os_dly_wait(30);
			}
			if(WakeUpType==WAKE_TYPE_HELP)
			{
				WakeUpType= WAKE_TYPE_NOTHING;
				DFE.Write(EE_WAKE_UP_TYPE,1,(uchar *)&WakeUpType);	
				#if(DFE_DEBUG==TRUE)
				if(TD.MainTask==tMAIN_WRITE_DFE)	Uart.puts("DFE16\r\n\0");
				#endif
			}
		}
		
		if(PressFlag==1)
		{
		 	if((gCnt.SystemSecond-LastTime)>10)											// 10秒后,才允许再按下按钮
			{
				PressFlag=0;
			}
		}
	}
	
}




/*************************************************************************************************
** 函数名称: CheckTireDriveAlarm()
** 函数功能: 疲劳驾驶报警
** 输入参数: 无
** 输出参数: 无
** 全局变量: 无
** 创建时间：2009.09.08
** 修改时间: 
************************************************************************************************/
void		SwitchCls::CheckTireDriveAlarm()
{
	static	uchar		flag=0;
	static	uint32	LastTime;

	if(EnableFlag.EN1.Bits.TireDriveAlarm==1)														// 如果允许疲劳驾驶报警
	{
		if(VehicleStatus.VS6.Bits.FTdrivingAlarm==0)
		{
			if(VehicleStatus.VS3.Bits.Moto==1)
			{
				if(flag==0)														
			 	{
			  		LastTime = gCnt.SystemSecond;
			   	flag=1;
			 	}	 
	
			 	if((gCnt.SystemSecond-LastTime)>3600*4)								// 4小时到
			 	{
					VehicleStatus.VS6.Bits.FTdrivingAlarm = 1;									// 报警
					GPRS_TempPack = TRUE;
					if(TD.GPS==tGPS_TEMP_PACK)
					{
						Uart.puts("GTP tire drive Alarm\r\n\0");
					}
					flag=0;
					//$$$$$$$$$$$$$ Test $$$$$$$$$$$$$$$$$$$$$$$$
					#if		__SWITCH_DEBUG__
						if(TD.SW==tSW_CHECK_ALARM)
							Uart.puts("Tire Drive Alarm!\r\n");
					#endif
					//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
				}
			}
			else
			{
				flag=0;
			}				
	   }	
	}
}



/*************************************************************************************************
** 函数名称: CheckIllegalOpenDoor()
** 函数功能: 非法开门检测
** 输入参数: 无
** 输出参数: 无
** 全局变量: 无
** 创建时间：2009.09.08
** 修改时间: 
************************************************************************************************/
void		SwitchCls::CheckIllegalOpenDoor()
{
	static	uint16 Cnt=0;
	
	if((EnableFlag.EN2.Bits.IllegalOpenDoor==1)&&(DOOR_Status==0))				   	// 如果允许非法开门报警，且门被打开
	{ 
		if(SetScanAlarmFlag==1)											// 如果是设防
		{
			if( (VehicleStatus.VS3.Bits.DoorOpen==0)|| (WakeUpType==WAKE_TYPE_DOOR) )		// 设防情况下,发生车门打开
			{
				Cnt++;
				if(Cnt>10)
				{
					Cnt = 0;
					VehicleStatus.VS3.Bits.DoorOpen = 1;		// 产生非法开门报警
					GPRS_TempPack = TRUE;
					if(TD.GPS==tGPS_TEMP_PACK)
					{
						Uart.puts("GTP invalid open door\r\n\0");
					}
					//$$$$$$$$$$$$$ Test $$$$$$$$$$$$$$$$$$$$$$$$
					#if		__SWITCH_DEBUG__
						if(TD.SW==tSW_CHECK_ALARM)
							Uart.puts("Illegal Open Door Alarm!\r\n");
					#endif
					//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
					if(WakeUpType==WAKE_TYPE_DOOR)
					{
						WakeUpType= WAKE_TYPE_NOTHING;
					   DFE.Write(EE_WAKE_UP_TYPE,1,(uchar *)&WakeUpType);	
						#if(DFE_DEBUG==TRUE)
						if(TD.MainTask==tMAIN_WRITE_DFE)	Uart.puts("DFE15\r\n\0");
						#endif
					}
				}
			}				
		}
		else
		{
			Cnt = 0;
			VehicleStatus.VS3.Bits.DoorOpen=0;
		}	
	}
	else
		Cnt = 0;	
			  	
}


/*************************************************************************************************
** 函数名称: CheckIllegalMakeFire()
** 函数功能: 非法点火检测
** 输入参数: 无
** 输出参数: 无
** 全局变量: 无
** 创建时间：2009.09.08
** 修改时间: 
************************************************************************************************/
void		SwitchCls::CheckIllegalMakeFire()
{
	uchar Index = 27;
	//if(Enable.EN2.Bits.IllegalMakeFire==1)																		// 如果允许非法点火报警
	{ 
		if(SetScanAlarmFlag==1)																			// 如果设防情况
		{
			//if( ((VehicleStatus.VS4.Bits.LawlessPowerOn==0)&&(ACC_Status==1))\
			if( ((JT808Flag.VS8.Bits.IllegalPowerOn==0)&&(ACC_Status==1))\
			     || (WakeUpType==WAKE_TYPE_ACC) )				// 如果发生点火
			{
				JT808_GPRS_TempPack = TRUE;
				Uart.puts("TP. ACC on");
				#if ( TEST_CODE_JUST_FOR_TEST_LUOYANG==1)	  // 应付测试使用
				NOP();	NOP();
				#else
				//jflsja f
				if (JT808_Alarm_Word.SendTextSMS & (0x01 << Index))		     								// 发送短信
				{
					GSM.SetSMS_Buffer((char *)&JT808_Number.TextSMS[0],"Trail Car Alarm!!!");		
				}
				//if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))	Camera.SetTakeNumber(2,TRUE);		  // 拍照
				if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))
			   {
		  			if (!(JT808_Alarm_Word.SavePhoto & (0x01 << Index)))	Jt_photo_upload = 1;		  // 上传到平台
					MediaEventFlag =eBB_MEDIA_EVENT_OTHER;
		  			Camera.SetTakeNumber(2,TRUE);		  // 拍照 
	  		   } 
				#endif
				JT808Flag.VS8.Bits.IllegalPowerOn = 1;
				VehicleStatus.VS4.Bits.LawlessPowerOn = 1;										// 非法点火报警
				//GPRS_TempPack = TRUE;
				if(TD.GPS==tGPS_TEMP_PACK)
				{
					Uart.puts("GTP invalid fire Alarm\r\n\0");
				}
				//$$$$$$$$$$$$$ Test $$$$$$$$$$$$$$$$$$$$$$$$
				#if		__SWITCH_DEBUG__
					if(TD.SW==tSW_CHECK_ALARM)
						Uart.puts("Illegal Make Fire Alarm!\r\n");
				#endif
				//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
				if(WakeUpType==WAKE_TYPE_ACC)
				{
					WakeUpType= WAKE_TYPE_NOTHING;
				   DFE.Write(EE_WAKE_UP_TYPE,1,(uchar *)&WakeUpType);	
					#if(DFE_DEBUG==TRUE)
					if(TD.MainTask==tMAIN_WRITE_DFE)	Uart.puts("DFE14\r\n\0");
					#endif
				}
			}	
		}
		else
		{
			//if(WakeUpType!=WAKE_TYPE_ACC)
		 		//VehicleStatus.VS4.Bits.LawlessPowerOn=0;
		}	
	}
}



/*************************************************************************************************
** 函数名称: CheckMachineSlfe()
** 函数功能: 终端自检
** 输入参数: 无
** 输出参数: 无
** 全局变量: 无
** 创建时间：2009.09.08
** 修改时间: 
************************************************************************************************/
void		SwitchCls::CheckMachineSelfTest()
{
	uchar	i;
	static	uchar		flag;
	static	uint32	volatile		SelfTestSecondCnt;
	 
	if(flag==0)
	{
	  	SelfTestSecondCnt = gCnt.SystemSecond;
	   flag=1;
		//$$$$$$$$$$$$$ Test $$$$$$$$$$$$$$$$$$$$$$$$
		#if		__SWITCH_DEBUG__
		if(TD.SW==tSW_CHECK_ALARM)
			Uart.puts("Inter Machine Slfe...\r\n");
		#endif
		//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$																						// 获取开机时的系统时间
	 }

	if(MOTO_Status)		return;		// 发动机工作，不重启
	 
	if( (gCnt.SystemSecond-SelfTestSecondCnt)>3600*24)											
	{
		GPRS_TempPack = TRUE;
		if(TD.GPS==tGPS_TEMP_PACK)
		{
			Uart.puts("GTP self test Alarm\r\n\0");
		}
		flag=0;
		SelfTestSecondCnt = gCnt.SystemSecond;	   
		//$$$$$$$$$$$$$ Test $$$$$$$$$$$$$$$$$$$$$$$$
		#if		__SWITCH_DEBUG__
		if(TD.SW==tSW_CHECK_ALARM)
			Uart.puts("Machine Self! System is restart...\r\n");
			Uart.PopAlignmentSend();
		#endif
		//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$			 
		for(i=0;i<100;i++)													  				// 等待30秒,上传位置信息完毕
		{
		 	RESET_WATCH_DOG;
			os_dly_wait(30);
		}
			
		//if((SetScanAlarmFlag==0)&&(ACC_Status==1))									// 正常情况,且车钥匙开 (需求中的,测试时间的准确性时可以不开启)
		//{

			Uart.puts("self test while1\r\n\0");
			Uart.PopAlignmentSend();
			os_dly_wait(100);
		 	NOP();	NOP();	NOP();	NOP();
			Switch.SaveVehicleStatus();			// 
			DisableIRQ();
			NOP();	NOP();	NOP();	NOP();
			for(;;)																					// 重启一次
			{
				NOP();	NOP();	NOP();	NOP();	
				NOP();	NOP();	NOP();	NOP();	
				while(1)
				{
					NOP();	NOP();	NOP();	NOP();		
					NOP();	NOP();	NOP();	NOP();
				}	
			}
		//}		
	 }
}


/*************************************************************************************************
** 函数名称: CheckShakeAlarm()
** 函数功能: 震动检测
** 输入参数: 无
** 输出参数: 无
** 全局变量: 无
** 创建时间：2009.09.08
** 修改时间: 
************************************************************************************************/
static	uint32	ShakeCnt;
void		SwitchCls::CheckShakeAlarm()
{
	if(EnableFlag.EN2.Bits.CheckShake==1)																				// 如果允许震动检测	
	{ 
		if(SetScanAlarmFlag==1)																			// 如果设防
		{
			if((VehicleStatus.VS6.Bits.VibrationAlarm==0)&&((IO_Status&swSHAKE)!=0))			// 发生震动
			{
				if(++ShakeCnt>50)
				{
					VehicleStatus.VS6.Bits.VibrationAlarm = 1;												// 震动报警
					GPRS_TempPack = TRUE;
					if(TD.GPS==tGPS_TEMP_PACK)
					{
						Uart.puts("GTP shake Alarm\r\n\0");
					}
				}
			}	
			else
				ShakeCnt = 0;
		}
		else
		{
		 	VehicleStatus.VS6.Bits.VibrationAlarm=0;
			//CloseSPK();
		}	
	}
	else
	{
		ShakeCnt = 0;
		VehicleStatus.VS6.Bits.VibrationAlarm=0;
	}
}



/*************************************************************************************************
** 函数名称: CheckLoginAlarm()
** 函数功能: 登陆密码错误报警检测
** 输入参数: 无
** 输出参数: 无
** 全局变量: 无
** 创建时间：2009.09.08
** 修改时间: 
************************************************************************************************/
void		SwitchCls::CheckLoginAlarm()
{
	// 在手柄处理模块中实现


}

/*************************************************************************************************
** 函数名称: CheckMainPower()
** 函数功能: 主电源掉电检测
** 输入参数: 无
** 输出参数: 无
** 全局变量: 无
** 创建时间：2009.09.08
** 修改时间: 
************************************************************************************************/
void		SwitchCls::CheckMainPower()
{
	static uchar  flag=0;
	static uint32 LastTime;	
	uchar	 Index = 8;	
	EXT_POWER_STATUS	  PowerStatus;
	uint32 tl, Addr;
	uchar tBuffer[256];

   PowerStatus.Status = 0;

	if(MainPower_Status==0)
	{	
		//if(VehicleStatus.VS1.Bits.MainPowerLost==0)
		if(JT808Flag.VS6.Bits.MainPowerOff==0)
		{
			if(flag==0)
			{
				LastTime = gCnt.SystemSecond;
				flag = 1;
				//$$$$$$$$$$$$$ Test $$$$$$$$$$$$$$$$$$$$$$$$
				#if	__SWITCH_DEBUG__
				if(TD.SW==tSW_CHECK_ALARM)
					Uart.puts("Inter Main Power Check...\r\n");
				#endif
				//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
			}	
			#if (WUXI_TEST == 1)	
			if (1) 		  // 掉电后报警
			#else
			if((gCnt.SystemSecond-LastTime)>0)		  // 掉电2秒后报警
			#endif
			{													 
				VehicleStatus.VS1.Bits.MainPowerLost = 1;
				JT808Flag.VS6.Bits.MainPowerOff = 1;						
				LastTime = gCnt.SystemSecond;
				Uart.puts("TP.Main power off");
				JT808_GPRS_TempPack = TRUE;
				
				PowerStatus.Status = 2;  // 断电

				#if ( TEST_CODE_JUST_FOR_TEST_LUOYANG==1)
				NOP();	NOP();
				#else
				//fdsfs 
				if (JT808_Alarm_Word.SendTextSMS & (0x01 << Index))		     								// 发送短信
					{
						GSM.SetSMS_Buffer((char *)&JT808_Number.TextSMS[0],"Main Power Off Alarm!!!");		
					}
				//if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))	Camera.SetTakeNumber(2,TRUE);		  // 拍照 
				if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))
			   {
		  			if (!(JT808_Alarm_Word.SavePhoto & (0x01 << Index)))	Jt_photo_upload = 1;		  // 上传到平台
					MediaEventFlag =eBB_MEDIA_EVENT_OTHER;
		  			Camera.SetTakeNumber(2,TRUE);		  // 拍照 
	  		   }
				if(TD.GPS==tGPS_TEMP_PACK)
				{
					Uart.puts("GTP main power Alarm\r\n\0");
				}
				//$$$$$$$$$$$$$ Test $$$$$$$$$$$$$$$$$$$$$$$$
				#if	__SWITCH_DEBUG__
				if(TD.SW==tSW_CHECK_ALARM)
					Uart.puts("Main Power Alarm!\r\n");
				#endif
				#endif
				//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
			}
		}
		else
		{
			flag=0;
		}
	} 
	else
	{
		flag=0;
		VehicleStatus.VS1.Bits.MainPowerLost = 0;
		if (JT808Flag.VS6.Bits.MainPowerOff == 1)
		{
			JT808_GPRS_TempPack = TRUE;
			PowerStatus.Status = 1;  // 通电
		 	Uart.puts("TP.Main power Restoration");
		}
			
		JT808Flag.VS6.Bits.MainPowerOff = 0;				
	}

	// 2015.1.22	 // 保存外部供电状态
	if (PowerStatus.Status)
	{
		//if ((TickCnt<1000)&&(PowerStatus.Status==2)) return;	// 开机10s，不保存有掉电记录

		Rtc.GetRTC_BCD((RTC_STR *)tBuffer);
		memmove(PowerStatus.Time,tBuffer, 6);
		DFE.Read(FM_EXT_POWER_STATUS_CNT,4,(uchar*)&tl);
		Addr = tl%36;							  // 	 36条
		Addr *= LEN_EXT_POWER_STATUS;
		Addr += FM_EXT_POWER_STATUS;		  	
		DFE.Write( Addr, LEN_EXT_POWER_STATUS,(uchar *)&PowerStatus);
		tl++;
		DFE.Write(FM_EXT_POWER_STATUS_CNT,4,(uchar*)&tl);	 	// 驾驶员登入登出计数器加1后存储到铁电	
		//
		Uart.puts("更新外部供电记录到铁电 "); 
		if ((tl)&&(tl%36)==0)
		{
			// 铁电充满近一页的Flash的空间，则放到Flash上
			if( (FlashPageCnt.PowerOnOffSave%FF_PAGE_PER_SECTOR)==0)
			{
				Addr = FF_POWER_ON_OFF_START_PAGE;
				Addr += FlashPageCnt.PowerOnOffSave%FF_POWER_ON_OFF_TOTAL_PAGE;
				
				DFF.SectorEraseWaitOK(Addr/FF_PAGE_PER_SECTOR,10);
			}				
			///////////////////////////////////////////存储到Flash  这样1页flash存有36条外部供电记录
			DFE.Read(FM_EXT_POWER_STATUS,LEN_EXT_POWER_STATUS*36,tBuffer); // 
	
			Addr = FF_POWER_ON_OFF_START_PAGE;
			Addr += FlashPageCnt.PowerOnOffSave%FF_POWER_ON_OFF_TOTAL_PAGE;
			DFF.PageProgramWaitOK(Addr,tBuffer,3);
			FlashPageCnt.PowerOnOffSave++;

			DFE.Write( FM_FLASH_PAGE_CNT, LEN_FLASH_PAGE_CNT, (uchar*)&FlashPageCnt);

	
	  		Uart.puts("保存外部供电记录到Flash"); 
			Uart.puts("Addr Test Data:"); Uart.putb(1,4, (uchar *)&Addr); Uart.puts("\r\n");
		
		}
		
	}
}



/*************************************************************************************************
** 函数名称: CheckUnlockAlarm()
** 函数功能: 检测未锁门报警
** 输入参数: 无
** 输出参数: 无
** 全局变量: 无
** 创建时间：2009.09.08
** 修改时间: 
************************************************************************************************/
void		SwitchCls::CheckUnlockAlarm()
{
	static	uchar		flag=0;
	static	uint32	LastTime;
	
	if(EnableCheckLockDoorSignal==TRUE)
	{
		if(flag==0)
		{
			LastTime = gCnt.SystemSecond;
			flag = 1;
		}			
		if(((gCnt.SystemSecond-LastTime)>15)&&(flag==1))											// 15秒
		{		
			LastTime = gCnt.SystemSecond;
			if(LockDoorSignal!=1)											 // 
			{
				if(VehicleStatus.VS8.Bits.DoorUnlockAlarm==0)
					GPRS_TempPack = TRUE;
				VehicleStatus.VS8.Bits.DoorUnlockAlarm = 1;								// 发生报警				
				if(TD.GPS==tGPS_TEMP_PACK)
				{
					Uart.puts("GTP unlock DooR Alarm\r\n\0");
				}		
			}	 																	
			else
				VehicleStatus.VS8.Bits.DoorUnlockAlarm = 0;
		}		 	
	}
	else	
		flag = 0;
}

/*************************************************************************************************
** 函数名称: CheckHandleWatchCar()
** 函数功能: 手柄看车检测
** 输入参数: 无
** 输出参数: 无
** 全局变量: 无
** 创建时间：2009.10.21
** 修改时间: 
************************************************************************************************/
void		SwitchCls::CheckHandleWatchCar()
{
	static	uchar		flag=0;
	static	uint32	LastTime,HoldPhoneTime;
	
	if(EnableFlag.EN2.Bits.HandleWatchCar==1)									  				// 如果允许手柄看车
	{
		 if(HandleWatchCarMode==1)									  				// 自动看车
		 {
			 if(ACC_Status==0)							  
			 {
				  if(flag==0)
				  {
						LastTime = gCnt.SystemSecond;
						flag = 1;
				  }
				  if(((gCnt.SystemSecond-LastTime)>60)&&(HandleLoginSuccess!=0))
				  {
						SetScanAlarmFlag = 1;										// 自动设防
						DFE.Write(EE_SET_SCAN_FLAG,1,(uchar*)&SetScanAlarmFlag);
						HandleLoginSuccess = 0; 
						#if(DFE_DEBUG==TRUE)
						if(TD.MainTask==tMAIN_WRITE_DFE)	Uart.puts("DFE13\r\n\0");
						#endif
				  }
			 }
			 else
			 {
			  	flag = 0;
				//////// 处理 ///////////////////////////
				if(HandleLoginSuccess!=1)						  				// 非正常登录点火
				{
					if(HandleHoldFlag==1)
					{
						HoldPhoneTime = gCnt.SystemSecond;
						HandleHoldFlag = 0;
					}
					if((gCnt.SystemSecond-HoldPhoneTime)>15)						// 15s
					{
						//DisplayLcd.SendDataToLcd(0xa5,0,NULL);					// 手柄振铃
						HoldPhoneTime = gCnt.SystemSecond;
					}
				}
			 }
		 }
		 else																 	 			// 手动看车
		 {
			 if((ACC_Status==1)&&(HandleLoginSuccess!=1))							  
			 {
				if(HandleHoldFlag==1)
				{
					HoldPhoneTime = gCnt.SystemSecond;
					HandleHoldFlag = 0;
				}
				if((gCnt.SystemSecond-HoldPhoneTime)>15)										// 15s
				{
					//DisplayLcd.SendDataToLcd(0xa5,0,NULL);					// 手柄振铃
					HoldPhoneTime = gCnt.SystemSecond;
				}
			 }
		 }	
	}
}



/*************************************************************************************************
** 函数名称: PowerSaveMode()
** 函数功能: 省电模式处理
** 输入参数: 无
** 输出参数: 无
** 全局变量: 无
** 创建时间：2009.09.08
** 修改时间:
	ACC_Status		= !GET_ACC_STATUS;							  	  // 点火时,标志为1
	MOTO_Status 	= !GET_MOTO_STATUS;								  // 发动时,标志为1
	DOOR_Status 	= !GET_DOOR_STATUS;								  // 关门时,标志为1
	Button_Status 	= !GET_BUTTON_STATUS;	 
************************************************************************************************/
void		SwitchCls::PowerSaveMode()					// 每秒执行1次
{
	
	uint32	Time;			
		
	////////////////////////////////////////////////////////
	if((Idle.EnableInterIdleMode==1))
		VehicleStatus.VS8.Bits.EnablePowerSave = 1;
	else
	{
		Idle.SecondCnt=0;
		VehicleStatus.VS8.Bits.EnablePowerSave = 0;
		return;
	}
	
	if( (ACC_Status)||(MOTO_Status)||(!DOOR_Status)||(Button_Status)||(WakeUpType==WAKE_TYPE_TIME) )
	{
		Idle.SecondCnt = 0;				
		VehicleStatus.VS8.Bits.IdleMode = 0;
		if(WakeUpType == WAKE_TYPE_TIME)
		{
			WakeUpType = WAKE_TYPE_NOTHING;
			DFE.Write(EE_WAKE_UP_TYPE,1,(uchar *)&WakeUpType);
			#if(DFE_DEBUG==TRUE)
			if(TD.MainTask==tMAIN_WRITE_DFE)	Uart.puts("DFE12\r\n\0");
		#endif	
		}
		return;
	}	
	
	
	Idle.SecondCnt++;			
	Time = Idle.InterIdleModeTime;	
	
	if((Idle.SecondCnt%((Time-2)*60)==0))		// 进入省电模式前，发一条省电标志的信息
	{
		VehicleStatus.VS8.Bits.IdleMode = 1;
		GPRS_TempPack = TRUE;
		if(TD.GPS==tGPS_TEMP_PACK)
		{
			Uart.puts("GTP Ready Idle\r\n\0");
		}
	}
	
	if(Idle.SecondCnt>Time*60)
	{	
		if(Idle.EnableInterIdleMode==1)
			Idle.IdleModeFlag = IDLE_MODE_FLAG;							
		//else
		//	Idle.IdleModeFlag = !IDLE_MODE_FLAG;
	}	
	
	if(Idle.IdleModeFlag == IDLE_MODE_FLAG)
	{			
		DFE.Write(EE_IDLE_FLAG,1,(uchar *)&Idle.IdleModeFlag);
		VehicleStatus.VS8.Bits.IdleMode = 0;
		os_dly_wait(1);
		//$$$$$$$$$$$$$ Test $$$$$$$$$$$$$$$$$$$$$$$$
		Uart.puts("Inter PowerSaveMode!\r\n");
		Uart.PopAlignmentSend();		
		//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
		os_dly_wait(100);
		Switch.SaveVehicleStatus();			// 
		DisableIRQ();			
		while(1);	
	}	
}



/////////////// 省电模式部分 //////////////////////////
void			SwitchCls::WakeUp(void)
{
	static	uint32	IdleTaskCnt=0;
	uchar		t,type;
	
	IdleTaskCnt++;
	//if(IdleTaskCnt%20==0)
	gCnt.SystemSecond++;
										  
	/////////// 读取I/O引脚 ////////////////////////////
	GET_IO_PIN_INIT;
	////////////////////////// 唤醒
	if( ((GET_ACC_STATUS)==0) || ((GET_MOTO_STATUS)==0)||( gCnt.SystemSecond>(Idle.WakeUpTime*60))     )
	{																		
		type = WAKE_TYPE_NOTHING;
		if((GET_ACC_STATUS)==0)			type = WAKE_TYPE_ACC;							// 记录唤醒的原因
		if((GET_MOTO_STATUS)==0)		type = WAKE_TYPE_MOTO;
		//if((GET_BUTTON_STATUS)==0)		type = WAKE_TYPE_HELP;
		//if(GET_DOOR_STATUS==1)			type = WAKE_TYPE_DOOR;
		if(gCnt.SystemSecond>(Idle.WakeUpTime*60))	type = WAKE_TYPE_TIME;
				


		//os_dly_wait(100);		
	 	t = 0;	// 下次启动不进入省电模式（除非在满足条件的地方修改）	                                
		DFE.Write(EE_IDLE_FLAG,1,(uchar *)&t);			
		//os_dly_wait(2);
		DFE.Write(EE_WAKE_UP_TYPE,1,(uchar *)&type);				
		//os_dly_wait(2);
		Switch.SaveVehicleStatus();			// 
		
		Uart.puts("wake up while1\r\n\0");
		Uart.PopAlignmentSend();
		os_dly_wait(100);
		DisableIRQ();
		while(1);
		//return TRUE;
	}
	
		
	//////////////////////////////////////// 省电模式的灯闪
//	if(IdleTaskCnt%30==0)
//	{
//		IDLE_LED_ON_CONTROL;
//	}
//	else
//	{
//		IDLE_LED_OFF_CONTROL;	
//	}
//
//	#if(1)
//	if(IdleTaskCnt%10==0)	
//	{
//		GPS_LED_OFF;
//		GPRS_LED_OFF;
//	}
//	#endif
	///return	FALSE;
}










/*************************************************************************************************
** 函数名称: AutoLockDoor()
** 函数功能: 自动锁门
** 输入参数: 无
** 输出参数: 无
** 全局变量: 无
** 创建时间：2009.09.08
** 修改时间: 
************************************************************************************************/
void		SwitchCls::AutoLockDoor()
{
	static	uchar		flag=0;
	uchar 	speed;

	if(EnableFlag.EN2.Bits.AutoLockDoor==1)																	// 如果允许自动锁门
	{					  
		if(flag==0)
		{
			// 落锁方式1
			if(IO_Status&swBRAKE)																	// 检测到刹车信号
			{
				os_dly_wait(1);																		// 延时10MS去除干扰
				if(IO_Status&swBRAKE)																// 再次检测刹车信号
				{
					if((ACC_Status==1)&&(LockDoorSignal==2))		 					// ACC开且没有锁门
					{	
						LockDoorFlag=1;																// 锁门		
						flag = 1;  
						//$$$$$$$$$$$$$ Test $$$$$$$$$$$$$$$$$$$$$$$$
						#if		__SWITCH_DEBUG__
						if(TD.SW==tSW_CHECK_ALARM)
							Uart.puts("Auto Lock Door1!\r\n");
						#endif
						//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
					}
				}
			}		
			// 落锁方式2 	
			if(GPSValidFlag==GPS_VALID_FLAG)
			{
				speed = GPS.GetSpeed();																
				if((speed>16)&&(LockDoorSignal==2))
				{																							// 如果门是开的
					LockDoorFlag = 1;																	// 锁门
					flag = 1;
					//$$$$$$$$$$$$$ Test $$$$$$$$$$$$$$$$$$$$$$$$
					#if		__SWITCH_DEBUG__
					if(TD.SW==tSW_CHECK_ALARM)
						Uart.puts("Auto Lock Door2!\r\n");
					#endif
					//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
				}	
			}
		}

		if(flag==1)
		{
		  //if((LockDoorSignal==1)&&((IO_Status&swACC)==0))						// 检测起锁，且车钥匙打开
		  //{
		  //		flag = 0;	
		  //}
		  if(LockDoorFlag!=1)														// 检测起锁		
		  {
				flag = 0;	
		  }
		}
	}
}


extern	"C"		void       Delay_mS(unsigned char  c);
/*************************************************************************************************
** 函数名称: CheckRadioControl()
** 函数功能: 无线遥控接收
** 输入参数: 无
** 输出参数: 无
** 全局变量: 无
** 创建时间：2009.09.02
** 修改时间: 
************************************************************************************************/
//static		uint32		KeyCnt1=0,KeyCnt2=0,NoKeyCnt=0;
void			SwitchCls::CheckRadioControl(uchar	sleep)										// 50ms
{
	//uchar		tmp,Data;	
   


}






/*************************************************************************************************
** 函数名称: SaveVehicleStatus()
** 函数功能: 写车辆状态标志入铁电
** 输入参数: 无
** 输出参数: 无
** 全局变量: 无
** 修改时间: 2009.12.19
************************************************************************************************/
void		SwitchCls::SaveVehicleStatus()
{
	uchar		buf[4];

	buf[0]= VehicleStatus.VS1.Byte;
	buf[1]= VehicleStatus.VS2.Byte;
	buf[2]= VehicleStatus.VS3.Byte;
	buf[3]= VehicleStatus.VS4.Byte;

	DFE.Write(EE_VEHICLE_STATUS,4,buf);

	#if(DFE_DEBUG==TRUE)
	if(TD.MainTask==tMAIN_WRITE_DFE)	Uart.puts("DFE10\r\n\0");
	#endif
}


/*************************************************************************************************
** 函数名称: GetVehicleStatus()
** 函数功能: 读取车辆状态
** 输入参数: 无
** 输出参数: 无
** 全局变量: 无
** 修改时间: 2009.12.19
************************************************************************************************/
void		SwitchCls::GetVehicleStatus()
{
	uchar		buf[4]={0};
	//char		buf1[16]={0},buf2[128]={0};

	DFE.Read(EE_VEHICLE_STATUS1,4,buf);
	VehicleStatus.VS1.Byte = buf[0];
	VehicleStatus.VS2.Byte = buf[1];
	VehicleStatus.VS3.Byte = buf[2];
	VehicleStatus.VS4.Byte = buf[3];

	/*
	strcpy(buf2,"*XY,");
	strcat(buf2,(char *)sOEM_ID);
	strcat(buf2,",V1,0,V,0,0,0,0,0,0,0,");
	SetBufferValue((uchar *)buf1,8,0);
	ByteToHexChar(~buf[0],buf1);
	strcat(buf2,buf1);
	SetBufferValue((uchar *)buf1,8,0);
	ByteToHexChar(~buf[1],buf1);
	strcat(buf2,buf1);
	SetBufferValue((uchar *)buf1,8,0);
	ByteToHexChar(~buf[2],buf1);
	strcat(buf2,buf1);
	SetBufferValue((uchar *)buf1,8,0);
	ByteToHexChar(~buf[3],buf1);
	strcat(buf2,buf1);
	strcat(buf2,"#<0,0,00,0,0,0,00>System restart...");
	GPRS.PushAlignment(1,strlen(buf2),(uchar *)buf2);					// 上一次状态入队
	VehicleStatus.VS1.Bits.IdleMode = 0;
	//Uart.putb(1,4,buf);
	//Uart.putb(1,strlen(buf2),(uchar *)buf2);	 */
}

SwitchCls::SwitchCls(void)
{
	
}


/********************************  End of File *************************************************/
