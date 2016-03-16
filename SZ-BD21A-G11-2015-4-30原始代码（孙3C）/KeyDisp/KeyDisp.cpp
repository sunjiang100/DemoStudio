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
#define			KEKDISP_GLOBALS			1
#include			"KeyDisp.h"
#include		   "LEDe308.h"
#include       "..\flash\DFF_25xxx.h"
#include       "..\flash\DFE_25xxx.h"
#include			"..\UART\UART0.H"
#include			"..\UART\UART2.H"
#include			"..\Common\FLASH_Addr.H"
#include			"CN_Code.h"
#include			"..\Common\GB_T19056.h"

#include			"..\GSM_GPRS\GSM_GPRS.h"
#include			"..\GSM_GPRS\JT_T808.h"
#include			"..\GPS\GPS.h"
#include			"..\ADC\ADC.h"
#include			"..\RTC\RTC.h"

#include			"..\Speed\Speed.h"
#include			"..\Print\Print.h"
#include			"..\common\Algol.h"


#include		   ".\UserSys.h"					 //usb test


#define			BSJ_LCM			0       // 0 ：不使用博思杰调度屏

#define			LEN_TO_HY_LCM_BUFFER		300

extern     	uchar   jt_dis_reg_flag;
extern 		uchar		jt_auth_ok_flag; 

uchar			DispLock1IPAddr;

uchar			DispBuffer[LEN_TO_HY_LCM_BUFFER];// 	__attribute__((at(0x7fe03800)));
char 			Create_User_Dns[LEN_IPorDNS] = {"jt1.gghypt.net\0\0\0\0\0\0"};
const   		uint32	CreateUserDnsPort = {0x000003F0};		   // 1008

extern		uchar		WaitIC_CardAuthTime,IC_CardAuthFailure,OnlyUseBD_GPS;

static uchar ConfirmCnt = 0;		 // 作为按住确认键计数，一般作按3次就打印，即大约3秒触发打印

//uchar			InputStatus;

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：TaskKeyDisp
   功能描述：
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用



__task		void			TaskKeyDisp(void)	
{
	NOP();	NOP();	NOP();	NOP();		
	KeyDisp.Init();
	DispLock1IPAddr = 0;
	NOP();	NOP();	NOP();	NOP();	
	OLED.PowerOnTiming();	
	//os_dly_wait(100);
	OLED.Init();
	Uart.puts(" OLED上电 ");
	NOP();	NOP();	NOP();	NOP();	
	while(1)
	{
		NOP();	NOP();	NOP();	NOP();
		
		KeyDisp.KeyDisp();
		TC.KeyDisp = 0;	

		NOP();	NOP();	NOP();	NOP();		
	}
}


extern		uchar		UseRecordNow;
extern		uchar		U2_ForRFID;
void			KeyDispCls::KeyDisp(void)
{	
	//uchar  TestBuffer[20];
	NOP();	NOP();
	os_dly_wait(1);
	DispCnt++;
	if ((GG_Status.Flag.Bits.UpdateRemoteSet ==1)||(U0_Iap_Flag!=0)) return;	  // 远程升级


	//if( (DispCnt%500==0)&&(PrintBusy==FALSE)&&(IDEL_For_Acc_OFF!=TRUE))	    //60秒重新初始化一次OLED
	if( (DispCnt==100)||(DispCnt%18000==0) )	// 3分钟初始化一次
   {
		OLED.Init();
		#if (WUXI_TEST != 1)
		if(U0_Iap_Flag==0)	Uart.puts(" 再次初始化OLED ");
		#endif
	   NOP();	NOP();
		if(DispCnt==100)	SystemSelfTest = TRUE;
	}	

	if(TD.MainTask==tTASK_RUN_FLAG)
	{
		if(DispCnt%100==0)		Uart.puts("KeyDisp task run\r\n");
	}


	NOP();	NOP();	NOP();	NOP();
	if ((gCnt.SystemSecond > 10))// && ((DispCnt%20)==0))				 // 10s 后检测
		Adc.eTaskADC();

	NOP();	NOP();
	
	NOP();	NOP();	NOP();	NOP();
   if(uLED.MainMenuCnt != PRINT_MENU)	
	  LED_Buzzer();
	NOP();	NOP();	NOP();	NOP();
	if(U0_Iap_Flag==0)
	   KeyProcess();
	NOP();	NOP();	NOP();	NOP();
	if((IDEL_For_Acc_OFF==TRUE)&&(gCnt.SystemSecond>60))			 //ACC省电模式
	{		 
	   //LCM_POWER_OFF;
		LCM_BL_POWER_OFF;  //应付测试，不关闭显示，只关闭OLED 背光 
	    //EXT_5V_POWER_OFF;
		JT808IOStatusFlag.VS1.Bits.SleepStatus = 1;	   // added 2013.3
	}
	else if((U0_Iap_Flag!=IAP_DOWN_LOAD)&&(GG_Status.Flag.Bits.UpdateRemoteSet!=1))
	{
	   JT808IOStatusFlag.VS1.Bits.SleepStatus = 0;	   // added 2013.3	
	   if((DispCnt%20==0)&&(PrintBusy==FALSE)&&(JT_Ctr_PowerDown==0))
		{ 	
		   LCM_POWER_ON;
			if(	DispCnt > 30)
				LCM_BL_POWER_ON;


		   if(DispCnt>100)	DispProcee();
			
		}

		NOP();	NOP();						
		if( U2PinSelDevice != U2_FOR_DISPLAY  )		return;		//  录音、RFID使用串口


		NOP();	NOP();
		
		EXT_5V_POWER_ON;
		Uart2.ReceiveProcessData();
		if((TD.Display==tDISP_TEST_FOR_EXT)&&(DispCnt%300==0))	Uart.puts(" 接收调度屏信息 ");
		
		
		NOP();	NOP();	NOP();	NOP();
		//////////////////   U0和U1透明传输时不发送数据到显示器		
		if(DispCnt%300==0)	// GPS数据	  或心跳   或GSM状态
		{
			SendGSMSignalToLCM();
			if(TD.Display==tDISP_TEST_FOR_EXT)	Uart.puts(" 调度屏心跳包 ");
			
			if(PhoneCall.RING == TRUE) 
			{
			   PhoneCall.RING = FALSE;
				//SendDataToLCM(eLCM_U_RING,strlen((char *)CallInNumber),(uchar*)CallInNumber);
				SendDataToLCM_YH(eLCM_U_RING,strlen((char *)CallInNumber),(uchar*)CallInNumber);	 // modified 2013.4
			}
		}
	}
	NOP();	NOP();	NOP();	NOP();
}






void		KeyDispCls::SystemSelfTestDisp(void)
{
	//uchar		i;
  	SetBufferValue(&OLED_Buffer[0][0], 16, 0);
	SetBufferValue(&OLED_Buffer[1][0], 16, 0);
 	strncpy((char *)&OLED_Buffer,( const char  *)&InitialingSCR[0][0],16);

	/*
	for(i=0;i<8;i++)
	{
		if(SelfTestResust&(1<<i))		memmove((char *)&OLED_Buffer[1][0],(uchar*)&SelfTestResustER_SCR[i][0],16);
		else									memmove((char *)&OLED_Buffer[1][0],(uchar*)&SelfTestResustOK_SCR[i][0],16);
		DispScreen(0);		 // 
		os_dly_wait(200);
	} */

	memmove((char *)&OLED_Buffer[1][0],(uchar*)&InitialingSCR[1][0],16);		
	DispScreen(0);		 // 
	os_dly_wait(400);
	
	strncpy((char *)&OLED_Buffer,( const char  *)&InitialingSCR[2][0],16);
	strncpy((char *)&OLED_Buffer[1][0],( const char  *)&InitialingSCR[3][0],16);
	DispScreen(0);	
	os_dly_wait(200);
}











/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：eTaskDisp
   功能描述：LCM显示程序
   入口参数：无
   出口参数：无
   全局变量: 结构LCM
   创建时间：
   修改时间：
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void		KeyDispCls::DispProcee(void)	 
{

	uchar	   c=0;
	uint16	i,j;
	uint32   t,time;
				
	union
	{
		uint32	B32[16];
		uint16	B16[32];
		uchar		Buffer[100];				
	}	Tp;	
	NOP();   NOP();            // DataSheet page:180					
	NOP();	NOP();	
	
	InitDispCNT++;
	
	//if(uLED.ExtControl)
	//return;


   //if(((DFF.ReadStatusRegister())&0x01)!=0)  return;					 	

	NOP();	NOP();
	if((++KEY_DL.LongTimeNoKey>40*5)||((uLED.MainMenuCnt==PRINT_MENU)&&(PrintBusy==FALSE)&&(PrintEvent1==0)))		 // 20秒钟没按键或打印结束，返回默认屏幕显示	
	{		
		if((uLED.MainMenuCnt==Logouting_MENU)&&(jt_dis_reg_flag==1))
		{
		 	 jt_dis_reg_flag = 0;
			 KEY_DL.LongTimeNoKey = 20*5;
			 uLED.MainMenuCnt = Logout_Fales_MENU;
		}
		else if(uLED.MainMenuCnt==PRINT_MENU)
		{
			PrintEvent1=0;
		    PrintEvent2=0;
		    Print.Init(FALSE);
			uLED.MainMenuCnt = DEFAULT_DISP;
		}
	   else if( ((UpdatePara.Enable==ENABLE_FLAG)||(U0_Iap_Flag!=0)) && (GetFlashData.TotalPage==0)  )
		{
		    uLED.MainMenuCnt = UPDATE_INDICATE;
		}
		else
		{
		   Write_File_Error = 0;
			uLED.MainMenuCnt = DEFAULT_DISP;
		   uLED.PageCnt = 0;
			ConfirmCnt = 0;
		}

	}
	//// 初始化显示内容
	if(SystemSelfTest==TRUE)
	{
		uLED.MainMenuCnt = SYS_INIT_MENU;			
		SystemSelfTest = 0;
		
	}	
	else if (uLED.ExtControl==TRUE)
	{
      uLED.MainMenuCnt = NotAllowChangeDriver_MENU;
	} 
	else 
		InitDispCNT = (3*25);		// 禁止初始化截面显示


	if((Wfile_Flag==1)||(Write_File_Error==1))
	   uLED.MainMenuCnt = DonotRemoveDiskSCR_MENU;

	NOP();	NOP();

	SetBufferValue(&OLED_Buffer[2][0],16,0);
	SetBufferValue(&OLED_Buffer[3][0],16,0);
	
	switch(uLED.MainMenuCnt)
	{
		case SYS_INIT_MENU:

				SystemSelfTestDisp();
				uLED.MainMenuCnt = DEFAULT_DISP;
			break;
		// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  主菜单1
		case MAIN_MENU:				//	
			NOP();	NOP();				
			/*if(uLED.SubMenuCnt==1)
			{
				strncpy((char *)&OLED_Buffer,(const char *)&MainMenuSCR,16);
				strncpy((char *)&OLED_Buffer[1][0],(const char *)&MainMenuSCR[1][0],16);

			}
			else
			{
				strncpy((char *)&OLED_Buffer,(const char *)&MainMenuSCR[(uLED.SubMenuCnt-1)%MAX_SUM_MENU][0],16);
				strncpy((char *)&OLED_Buffer[1][0],(const char *)&MainMenuSCR[uLED.SubMenuCnt%MAX_SUM_MENU][0],16);

			} */

			SetBufferValue(&OLED_Buffer[0][0],sizeof(OLED_Buffer),0);

			strncpy((char *)&OLED_Buffer[0][0],(const char *)&MainMenuSCR[uLED.SubMenuCnt-1][0],16);
			strncpy((char *)&OLED_Buffer[1][0],(const char *)&MainMenuSCR[uLED.SubMenuCnt][0],16);
			if((uLED.SubMenuCnt+1)<MAX_SUM_MENU)
			   strncpy((char *)&OLED_Buffer[2][0],(const char *)&MainMenuSCR[uLED.SubMenuCnt+1][0],16);
			if((uLED.SubMenuCnt+2)<MAX_SUM_MENU)
				strncpy((char *)&OLED_Buffer[3][0],(const char *)&MainMenuSCR[uLED.SubMenuCnt+2][0],16);

			if(TwinkleFlag)
			{
				TwinkleFlag = 0;
				strncpy((char *)&OLED_Buffer[0][0],"                ",16);			  //总在第一行闪烁
			}
			else
			{
				TwinkleFlag = 1;
			}
			break;

		/// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 参数设置
		case SET_PARA_MENU:

			//strncpy((char *)&OLED_Buffer,(const char *)&SetRegParaSCR[uLED.PageCnt][0],16);	
			//SetBufferValue( (uchar *)&OLED_Buffer[1][0],16,0);

			 if (uLED.PageCnt==0) 
			 {
			 	 uLED.SubMenuCnt = 0;
				 uLED.SetParaFlag = SET_PARA_INPUT_NUM_AND_DOT;

			 	 strncpy((char *)&OLED_Buffer[0][0],(const char *)&SetRegParaSCR[uLED.PageCnt][0],16);

				 if (uLED.InputStatus == INPUT_INIT_STATUS)
				 {
				 	uLED.InputPageCnt = 0;
				 	strncpy((char *)&OLED_Buffer[1][uLED.Offset],(const char *)"I", 1);
				 }

				 
				 if (uLED.InputStatus != INPUT_FINISH_ALL_CHAR)
				 {
				 	uLED.MainMenuCnt = INPUT_MENU;
					uLED.Offset = 0;
					uLED.InputStatus = INPUT_INIT_STATUS;

				 }

				 //LPC_UART0->THR = 'C'; 	  LPC_UART0->THR = '1'; 

			 }
			 else if(uLED.PageCnt<=MAX_SET_PARA_MENU)
		    {	 
			   strncpy((char *)&OLED_Buffer[0][0],(const char *)&SetRegParaSCR[uLED.SubMenuCnt+0][0],16);

			   if((uLED.SubMenuCnt+3)<MAX_SET_PARA_MENU)
			      strncpy((char *)&OLED_Buffer[3][0],(const char *)&SetRegParaSCR[uLED.SubMenuCnt+3][0],16);
			   else
				 	SetBufferValue(&OLED_Buffer[3][0], 16, 0);
			   if((uLED.SubMenuCnt+2)<MAX_SET_PARA_MENU)
				 strncpy((char *)&OLED_Buffer[2][0],(const char *)&SetRegParaSCR[uLED.SubMenuCnt+2][0],16);
		       else
				SetBufferValue(&OLED_Buffer[2][0], 16, 0);
			  if((uLED.SubMenuCnt+1)<MAX_SET_PARA_MENU)
				 strncpy((char *)&OLED_Buffer[1][0],(const char *)&SetRegParaSCR[uLED.SubMenuCnt+1][0],16);
		       else
				SetBufferValue(&OLED_Buffer[1][0], 16, 0);
			 if(uLED.SubMenuCnt == MAX_SET_PARA_MENU)
			    uLED.SubMenuCnt=1;
			/*
				 strncpy((char *)&OLED_Buffer[0][0],(const char *)&SetRegParaSCR[uLED.SubMenuCnt+0][0],16);
		    	 strncpy((char *)&OLED_Buffer[1][0],(const char *)&SetRegParaSCR[uLED.SubMenuCnt+1][0],16);
				 
				 if((uLED.SubMenuCnt+2)<MAX_SET_PARA_MENU)
				 	 strncpy((char *)&OLED_Buffer[2][0],(const char *)&SetRegParaSCR[uLED.SubMenuCnt+2][0],16);
				 else
				 	SetBufferValue(&OLED_Buffer[2][0], 16, 0);
				 
				 if((uLED.SubMenuCnt+3)<MAX_SET_PARA_MENU)
				    strncpy((char *)&OLED_Buffer[3][0],(const char *)&SetRegParaSCR[uLED.SubMenuCnt+3][0],16);
				 else
				 	SetBufferValue(&OLED_Buffer[3][0], 16, 0);
				*/ 
		    }
			 else
			 {
			    uLED.PageCnt = 0;
			 }

			
			/*if((uLED.Offset<uLED.MaxOffset) && (uLED.InputStatus != INPUT_FINISH_ALL_CHAR))
			{	
					
				if(TwinkleFlag)
				{
					TwinkleFlag = 0;
					OLED_Buffer[1][uLED.Offset] = ' ';
					
				}
				else
				{
					TwinkleFlag = 1;
				}
			}*/


			if (uLED.PasswordOK == TRUE)	 // 密码正确，说明已经输过密码，需要闪烁是子菜单 
			{

				if(TwinkleFlag)
				{
					TwinkleFlag = 0;
					strncpy((char *)&OLED_Buffer[0][0],"                ",16);			  //总在第一行闪烁
				}
				else
				{
					TwinkleFlag = 1;
				}
			}
					
						
			break;

		case SET_PARA_SUB_MENU:
			if (uLED.SubMenuCnt==eSET_PARA_CAR_TYPE)
			{
					if(uLED.SubSubMenuCnt==1)
					{
						strncpy((char *)&OLED_Buffer,(const char *)&VehicleTypeSCR,16);
						strncpy((char *)&OLED_Buffer[1][0],(const char *)&VehicleTypeSCR[1][0],16);
					}
					else
					{
						strncpy((char *)&OLED_Buffer,(const char *)&VehicleTypeSCR[uLED.SubSubMenuCnt-1][0],16);
						strncpy((char *)&OLED_Buffer[1][0],(const char *)&VehicleTypeSCR[uLED.SubSubMenuCnt][0],16);
					}
					if(TwinkleFlag)
					{
						TwinkleFlag = 0;
						strncpy((char *)&OLED_Buffer[0][0],"                ",16);			  //总在第一行闪烁
					}
					else
					{
						TwinkleFlag = 1;
					}
				
			}

			break;

		
		case	INPUT_MENU:				//输入菜单	(通过选择来输入）	用于参数设置

			 if (uLED.InputStatus == INPUT_INIT_STATUS)
			 {
			 //LPC_UART0->THR = 'C'; 	  LPC_UART0->THR = '2'; 

			 	uLED.InputPageCnt = 0;
				SetParaBuffer[17] = 0;
				i = strlen((char *)SetParaBuffer);

				if  (uLED.InputAlert == FALSE)
				{
					if (uLED.SetParaFlag != SET_PARA_INPUT_NEED_FOR_COLOR) 
					{
						if (i > uLED.Offset) 	c = SetParaBuffer[uLED.Offset];	 // 实际输入字符串的长度如果小于光标的偏移，就让光标所在的字符闪烁
						else							c = 'I';
					
			 			//strncpy((char *)&OLED_Buffer[1][uLED.Offset],(const char *)"I", 1);
						OLED_Buffer[1][uLED.Offset] = c;

					}
					else 
					{
						if (i > uLED.Offset)
						{
						   OLED_Buffer[1][uLED.Offset] = SetParaBuffer[uLED.Offset];
							OLED_Buffer[1][uLED.Offset+1] = SetParaBuffer[uLED.Offset+1];
						}
						else 
						{
							 OLED_Buffer[1][uLED.Offset] =   InputSelectColor[uLED.Offset];
							 OLED_Buffer[1][uLED.Offset+1] = InputSelectColor[uLED.Offset+1];
						}
					}
				}
			 }
			 else	if (uLED.InputStatus == INPUT_ONE_CHAR)
			 {
			 	//LPC_UART0->THR = 'C'; 	  LPC_UART0->THR = '3'; 


			 	 	//memmove((char*)&OLED_Buffer[1][uLED.Offset],(char*)&InputSelectChar[0][uLED.InputPageCnt],1);
					//OLED_Buffer[1][uLED.Offset] =  InputSelectChar[0][uLED.InputPageCnt];

					if (uLED.SetParaFlag == SET_PARA_INPUT_NUM_AND_DOT)  
					{
						OLED_Buffer[1][uLED.Offset] =  InputSelectNumDot[uLED.InputPageCnt];
					}
					else if (uLED.SetParaFlag == SET_PARA_INPUT_NEED_FOR_COLOR) 
					{
						OLED_Buffer[1][uLED.Offset] =  InputSelectColor[uLED.InputPageCnt];	
						OLED_Buffer[1][uLED.Offset+1] =  InputSelectColor[uLED.InputPageCnt+1];
					}
					else if (uLED.SetParaFlag == SET_PARA_INPUT_NEED_FOR_CAR_NUM)
					{
						if (uLED.InputPageCnt < 36 )
						{
							OLED_Buffer[1][uLED.Offset] =  InputSelectCarNum[uLED.InputPageCnt];
						}
						else
						{
							OLED_Buffer[1][uLED.Offset] 	=  InputSelectCarNum[uLED.InputPageCnt];	
							OLED_Buffer[1][uLED.Offset+1] =  InputSelectCarNum[uLED.InputPageCnt+1];
						}

					}
					 
			 }
			
			if ((uLED.Offset<uLED.MaxOffset) && (uLED.InputStatus != INPUT_FINISH_ALL_CHAR))
			{	
						
				if(TwinkleFlag)
				{
					TwinkleFlag = 0;

					if  (uLED.InputAlert == FALSE)
					{

						if (uLED.SetParaFlag == SET_PARA_INPUT_NUM_AND_DOT) 
						{
							OLED_Buffer[1][uLED.Offset] = ' ';
						}
						else if (uLED.SetParaFlag == SET_PARA_INPUT_NEED_FOR_COLOR)
						{
							OLED_Buffer[1][uLED.Offset] =  ' ';	
							OLED_Buffer[1][uLED.Offset+1] = ' ';
						}
						else  if (uLED.SetParaFlag == SET_PARA_INPUT_NEED_FOR_CAR_NUM)
						{ 
							if (uLED.InputPageCnt < 36)
							{
								  OLED_Buffer[1][uLED.Offset] = ' ';
							}
							else
							{
								OLED_Buffer[1][uLED.Offset] =  ' ';	
								OLED_Buffer[1][uLED.Offset+1] = ' ';
							}
	
						}
					}
				}
				else
				{
					TwinkleFlag = 1;
				}
			}


			

			break;


		case NotAllowChangeDriver_MENU:									//不允许更改驾驶员
			strncpy((char *)&OLED_Buffer,(const char *)&CannotChangeDriverSCR,16);	
			strncpy((char *)&OLED_Buffer[1][0],(const char *)&CannotChangeDriverSCR[1][0],16);
			break;
		/// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 车辆特征系数
		case VEHICLE_CC_MENU:		// 
			strncpy((char *)&OLED_Buffer,(const char *)&VehicleCC_SCR,16);	
			SetBufferValue( &OLED_Buffer[1][0],4,0x20);
			SetBufferValue( &OLED_Buffer[1][4],12,0);
			DFE.Read(FM_VEHICLE_PULSE_KV,4,(uchar *)&t);			
			t &= 0x0000ffff;
			Int2Str( t, (char*)&OLED_Buffer[1][4]);	
			break;
		/// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 疲劳驾驶记录
		case OVER_TIME_MENU:			

			/*if(uLED.PageCnt>40)	 	// 最大40页
				uLED.PageCnt = 0;
			if(uLED.PageCnt==0)		// 排序，最早时间在前
			{
			   SetBufferValue((uchar *)OVT_AddrTab,(20*4),0);
			   DispOVT_CNT = GetOVT_Addr(TWO_DAY_OVT,OVT_AddrTab); 
			}
			else
			{  c = 1;			
			   if(2*DispOVT_CNT<=uLED.PageCnt)
				  uLED.PageCnt = 2*DispOVT_CNT-1;
			}
			if((c==1)&&(uLED.PageCnt%2!=0)&&(DispOVT_CNT!=0))
			{
				strncpy((char *)&OLED_Buffer,(const char *)&TireTimeSCR[2][0],16);
			   strncpy((char *)&OLED_Buffer[1][0],(const char *)&TireTimeSCR[3][0],16);
				
			}
			else if(DispOVT_CNT!=0)
			{
			   strncpy((char *)&OLED_Buffer,(const char *)&TireTimeSCR[0][0],16);
			   strncpy((char *)&OLED_Buffer[1][0],(const char *)&TireTimeSCR[1][0],16);
				OLED_Buffer[0][8] = (uLED.PageCnt+2)/2/10 + '0';				   //第X条记录		按两次下翻键才显示完一条疲劳驾驶记录 按键次数为偶数只能显示条数和驾驶员代码
				OLED_Buffer[0][9] = (uLED.PageCnt+2)/2%10 + '0';	
			}
			else
			{
			   uLED.PageCnt = 0;
				strncpy((char *)&OLED_Buffer,(const char *)&NoTireTimeSCR,16);				//无疲劳驾驶记录
				strncpy((char *)&OLED_Buffer[1][0],(const char *)&NoTireTimeSCR[1][0],16);	
				break;			
			}			
			// 读取一条疲劳驾驶信息，包括驾驶证号和日期时间	
			SetBufferValue((uchar *)Tp.Buffer,30,0);		
			if((uLED.PageCnt/2<=DispOVT_CNT)&&(DispOVT_CNT!=0))				   //疲劳记录条数和按键次数小于等于疲劳记录条数认为是无疲劳驾驶记录
			{
				if(OVT_AddrTab[uLED.PageCnt/2]<8192)  // 取铁电的
				  DFE.Read(OVT_AddrTab[uLED.PageCnt/2],LEN_SAVE_FATIGUE_DRIVING_INFO,Tp.Buffer);
				else
				{
				  for(i=0;i<1000;i++)				// 等待完成 (不能用while),10s
		        {																				
			       if(((DFF.ReadStatusRegister())&0x01)!=0)					 	
			       {os_dly_wait(1);}	
		          else
			       {break;}
	           } 
				  DFF.AnyNumberRead(OVT_AddrTab[uLED.PageCnt/2],LEN_SAVE_FATIGUE_DRIVING_INFO,Tp.Buffer);
				}
				//Uart.puts((char *)Tp.Buffer);	
			}
			else
			{
				Tp.Buffer[0] = Tp.Buffer[1] = Tp.Buffer[2] = 0;				      //置驾驶证号无效
			}
			NOP();	NOP();	
            //Tp.Buffer[] = {"123456789012345678110722123800110722175500\0"};	  	  //测试数据		
			
			/////////////////////////////// 驾驶证号处理
			if(uLED.PageCnt%2==0)
			{
					strncpy((char *)&OLED_Buffer[0][12],(char *)Tp.Buffer,4);		// 驾驶证号
					strncpy((char *)&OLED_Buffer[1][0],(char *)&Tp.Buffer[4],14);
			}
			else
			/////////////////////////////// 日期时间处理			
			{
				for(c=0,i=18;i<30;i++)
				{
					//if(i!=23)
					if(1)											
					{
						Tp.Buffer[c++] = (Tp.Buffer[i]>>4) + '0';
						Tp.Buffer[c++] = (Tp.Buffer[i]&0x0f) + '0';
					}
				}
				for(c=0,i=0;i<2;i++)
				{
					for(j=0;j<20;j++)
					{
						if(OLED_Buffer[i][j] == 'x')		 //不显示秒
							OLED_Buffer[i][j] = Tp.Buffer[c++];
					}
				}
			} */

		
			if(uLED.PageCnt==0)		
			{
			   SetBufferValue((uchar *)OVT_AddrTab,(20*4),0);
			   DispOVT_CNT = GetOVT_Addr(TWO_DAY_OVT,OVT_AddrTab); 
			}

			if(uLED.PageCnt>=DispOVT_CNT)	 	// 
				uLED.PageCnt = 0;

			if((DispOVT_CNT!=0))
			{
				strncpy((char *)&OLED_Buffer,(const char *)&TireTimeSCR[0][0],16);
			   strncpy((char *)&OLED_Buffer[1][0],(const char *)&TireTimeSCR[1][0],16);
				OLED_Buffer[0][8] = (uLED.PageCnt/10) + '0';				   //第X条记录		按两次下翻键才显示完一条疲劳驾驶记录 按键次数为偶数只能显示条数和驾驶员代码
				OLED_Buffer[0][9] = ((uLED.PageCnt+1)%10) + '0';

				strncpy((char *)&OLED_Buffer[2][0],(const char *)&TireTimeSCR[2][0],16);
			   strncpy((char *)&OLED_Buffer[3][0],(const char *)&TireTimeSCR[3][0],16);
			}
			else
			{	
			   uLED.PageCnt = 0;								 
				strncpy((char *)&OLED_Buffer,(const char *)&NoTireTimeSCR,16);				//无疲劳驾驶记录
				strncpy((char *)&OLED_Buffer[1][0],(const char *)&NoTireTimeSCR[1][0],16);	
				break;			
			}			
			// 读取一条疲劳驾驶信息，包括驾驶证号和日期时间	
			SetBufferValue((uchar *)Tp.Buffer,50,0);		
			if(uLED.PageCnt<DispOVT_CNT)				   //
			{
				if(OVT_AddrTab[uLED.PageCnt]<8192)  // 取铁电的
				  DFE.Read(OVT_AddrTab[uLED.PageCnt],LEN_SAVE_FATIGUE_DRIVING_INFO,Tp.Buffer);
				else
				{
				  for(i=0;i<1000;i++)				// 等待完成 (不能用while),10s
		        {																				
			       if(((DFF.ReadStatusRegister())&0x01)!=0)					 	
			       {os_dly_wait(1);}	
		          else
			       {break;}
	           } 
				  DFF.AnyNumberRead(OVT_AddrTab[uLED.PageCnt],LEN_SAVE_FATIGUE_DRIVING_INFO,Tp.Buffer);
				}	
			}
			else
			{
				Tp.Buffer[0] = Tp.Buffer[1] = Tp.Buffer[2] = 0;				      //置驾驶证号无效
			}
			/////////////////////////////// 驾驶证号处理
		
			strncpy((char *)&OLED_Buffer[0][12],(char *)Tp.Buffer,4);		// 驾驶证号
			strncpy((char *)&OLED_Buffer[1][0],(char *)&Tp.Buffer[4],14);
		
			/////////////////////////////// 日期时间处理			
			for(c=0,i=18;i<30;i++)
			{
				Tp.Buffer[c++] = (Tp.Buffer[i]>>4) + '0';
				Tp.Buffer[c++] = (Tp.Buffer[i]&0x0f) + '0';

			}

			for(c=0,i=2;i<4;i++)
			{
				for(j=0;j<16;j++)
				{
					if(OLED_Buffer[i][j] == 'x')		 
						OLED_Buffer[i][j] = Tp.Buffer[c++];
				}
				//c += 2; // 跳过秒 //不显示秒
			}

			
			break;			
			
		/// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$                 15分钟内第分钟平均车速				 取消此项显示
		case AVG_SPEED_MENU:			//
		   SetBufferValue((uchar *)Tp.Buffer,30,0);
		   DFE.Read(FM_STOP_TIME,5,Tp.Buffer);
			c = Rtc.DateTimeValid((RTC_STR *)Tp.Buffer);	
			if((c>0)&&(c<7))
			{
			  strncpy((char *)&OLED_Buffer,(const char *)&AVG_SpeedSCR[3][0],16);
	    	  strncpy((char *)&OLED_Buffer[1][0],(const char *)&AVG_SpeedSCR[4][0],16);
			  uLED.PageCnt = 0;
			}
			else
	   	{												 
				DFE.Read(FM_STOP_LAST15_SPEED,15,&Tp.Buffer[20]);
				if(uLED.PageCnt<8)
				{
				 i = 0;
				 if(uLED.PageCnt>=1)
				  i = 2;
				 if(uLED.PageCnt==0)
		    	 {  
				    strncpy((char *)&OLED_Buffer,(const char *)&AVG_SpeedSCR[0][0],16);
					 OLED_Buffer[i][7]  = (Tp.Buffer[0]>>4) + '0';
					 OLED_Buffer[i][8]  = (Tp.Buffer[0]&0x0f) + '0';
					 OLED_Buffer[i][10] = (Tp.Buffer[1]>>4) + '0';
					 OLED_Buffer[i][11] = (Tp.Buffer[1]&0x0f) + '0';
					 OLED_Buffer[i][13] = (Tp.Buffer[2]>>4) + '0';
					 OLED_Buffer[i][14] = (Tp.Buffer[2]&0x0f) + '0';
				 }
				 else
				 {
				 	 time = Rtc.BDC_DateTimeToSecond((RTC_STR *)Tp.Buffer);
					 Rtc.SecondToDateTime_BCD(time - (uLED.PageCnt*2-1)*60, (RTC_STR *)Tp.Buffer);
					 strncpy((char *)&OLED_Buffer[0][0],(const char *)&AVG_SpeedSCR[1][0],16);
					 OLED_Buffer[0][0] = (Tp.Buffer[3]>>4) + '0';
					 OLED_Buffer[0][1] = (Tp.Buffer[3]&0x0f) + '0';
					 OLED_Buffer[0][2] = ':';
					 OLED_Buffer[0][3] = (Tp.Buffer[4]>>4) + '0';
					 OLED_Buffer[0][4] = (Tp.Buffer[4]&0x0f) + '0';
                	 c = Tp.Buffer[20+uLED.PageCnt*2-1];
					 OLED_Buffer[0][7]   = c/100 + 0x30;
			         OLED_Buffer[0][8]   = c%100/10 + 0x30;
			         OLED_Buffer[0][9]   = c%100%10 + 0x30;
					 if(OLED_Buffer[0][7]=='0')
				 	 {
						 OLED_Buffer[0][7] = ' ';
						 if(OLED_Buffer[0][8]=='0')
							 OLED_Buffer[0][8] = ' ';
					 }
				    Rtc.SecondToDateTime_BCD(time - (uLED.PageCnt*2)*60, (RTC_STR *)Tp.Buffer);	 										 
				  }
				  //if(uLED.PageCnt!=0)
			     strncpy((char *)&OLED_Buffer[1][0],(const char *)&AVG_SpeedSCR[1][0],16); 
				  OLED_Buffer[1][0] = (Tp.Buffer[3]>>4) + '0';
				  OLED_Buffer[1][1] = (Tp.Buffer[3]&0x0f) + '0';
				  OLED_Buffer[1][2] = ':';
				  OLED_Buffer[1][3] = (Tp.Buffer[4]>>4) + '0';
				  OLED_Buffer[1][4] = (Tp.Buffer[4]&0x0f) + '0';
              c = Tp.Buffer[20+uLED.PageCnt*2];
				  OLED_Buffer[1][7]   = c/100 + 0x30;
		        OLED_Buffer[1][8]   = c%100/10 + 0x30;
		        OLED_Buffer[1][9]   = c%100%10 + 0x30;
				  if(OLED_Buffer[1][7]=='0')
			 	  {
					 OLED_Buffer[1][7] = ' ';
					 if(OLED_Buffer[1][8]=='0')
						 OLED_Buffer[1][8] = ' ';
				  }										 
				 }
				 if(uLED.PageCnt==8)
				    uLED.PageCnt = 0;
				 if(uLED.PageCnt>=8)
				    uLED.PageCnt = 7;
		     }
			  
			break;
		/// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 超速记录显示					         取消此项显示
		case OVER_SPEED_MENU:

		/// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 当前驾驶员信息
		case DRIVER_INFO_MENU:		// 
				
			   if((DriverCardFlag==TRUE))																					//是否驾驶卡	 &&(StopFlag==1)
		      {
				
					if(uLED.PageCnt>1)		uLED.PageCnt = 0;
				
				               
					  if(uLED.PageCnt==0)
					  {
						  strncpy((char *)&OLED_Buffer,(const char *)&DriverInfoSCR[0][0],16);
					  	  strncpy((char *)&OLED_Buffer[1][0],(const char *)&DriverInfoSCR[1][0],16);
						  DFE.Read(EE_JT_DRIVER_NAME_LEN,1,(uchar *)&t); 	// 姓名长度
						  	if(t>10)	t = 10;
							DFE.Read(EE_JT_DRIVER_NAME,t,&OLED_Buffer[0][6]);
	
						  DFE.Read(FM_DRIVER_NUMBER,4,(uchar*)&t);
						  Int2Str( t,(char*)Tp.Buffer );
						  t = strlen((char*)Tp.Buffer);
						  if(t>10)	t = 10;
						  memmove(&OLED_Buffer[1][6],Tp.Buffer,t);
					  }
				     else if(uLED.PageCnt==1)									
					  {
					     strncpy((char *)&OLED_Buffer,(const char *)&DriverInfoSCR[2][0],16);
					  	  strncpy((char *)&OLED_Buffer[1][0],(const char *)&DriverInfoSCR[3][0],16);
						  DFE.Read(EE_JT_DRIVER_CARD_ID, 20, Tp.Buffer);
						  memmove( &OLED_Buffer[0][12],Tp.Buffer,4);
						  memmove( &OLED_Buffer[1][0],&Tp.Buffer[4],16);
					  }
					  else
					    uLED.PageCnt = 0;
				 

				 }
				 else
				 {
					 strncpy((char *)&OLED_Buffer[0][0]," 无驾驶员信息！ ",16);
					 strncpy((char *)&OLED_Buffer[1][0],"                ",16);
				 }
			  break;
			
			case  Car_MSG_MENU:				//车辆信息
				 if(uLED.PageCnt<3)
			    {
					i = 0;
				   if(uLED.PageCnt!=0)
					 i = uLED.PageCnt*2;
				   strncpy((char *)&OLED_Buffer[0][0],(const char *)&CarMSGSCR[i][0],16);
				   strncpy((char *)&OLED_Buffer[1][0],(const char *)&CarMSGSCR[i+1][0],16);
				   DFE.Read(FM_VEHICLE_CODE,LEN_VEHICLE_VCC,Tp.Buffer);				  //将地址改为该数据地址即可 
					 
				   if(uLED.PageCnt==0)
				   {
				     strncpy((char *)&OLED_Buffer[0][5],(const char *)&Tp.Buffer[0],12);					  //车牌号
					  strncpy((char *)&OLED_Buffer[1][5],(const char *)&Tp.Buffer[29],10);					  //车辆类型
				   }
				   else if(uLED.PageCnt==1)
				   {
				     //DFE.Read(FM_VEHICLE_VIN,LEN_VEHICLE_VCC,Tp.Buffer);				  //	  VIN号
				  	  strncpy((char *)&OLED_Buffer[0][6],(const char *)&Tp.Buffer[12],10);
					  strncpy((char *)&OLED_Buffer[1][0],(const char *)&Tp.Buffer[22],7);
				   }
					else if (uLED.PageCnt==2)				// 车牌颜色
					{
						DFE.Read(FM_CAR_COLOUR,1,(uchar *)&t);
						//if ((t>6)) t = 0;
						if ((t>6)) t = 1;
						Int2Str(t,(char *)&OLED_Buffer[0][13]);			// 车牌颜色代码

						//memmove(&OLED_Buffer[1][9],&InputSelectColor[(t-1)*2],2);		//  车牌颜色
						memmove(&OLED_Buffer[1][9],&InputSelectColor[t*2],2);		//  车牌颜色

					}
				   
				  }
				  else
				     uLED.PageCnt = 0;
			
			  break;
			case Mobile_PARA_MENU:  	  //无线通信参数
			   if(uLED.PageCnt<5)
			   {
					 i = 0;
					 if(uLED.PageCnt!=0)
					 	i = uLED.PageCnt*4;

					 strncpy((char *)&OLED_Buffer[0][0],(const char *)&MobileCMNCSCR[i][0],16);
			    	 strncpy((char *)&OLED_Buffer[1][0],(const char *)&MobileCMNCSCR[i+1][0],16);
					 strncpy((char *)&OLED_Buffer[2][0],(const char *)&MobileCMNCSCR[i+2][0],16);
					 strncpy((char *)&OLED_Buffer[3][0],(const char *)&MobileCMNCSCR[i+3][0],16);
			    }
				 if(uLED.PageCnt==0)
				 {
				 	DFE.Read(EE_JT_M_IP_DNS,16,Tp.Buffer);				  //将地址改为该数据地址即可	网络地址1
				  	strncpy((char *)&OLED_Buffer[1][0],(const char *)&Tp.Buffer,16);

					DFE.Read(EE_JT_B_IP_DNS,16,Tp.Buffer);				  //将地址改为该数据地址即可	网络地址2
				  	strncpy((char *)&OLED_Buffer[3][0],(const char *)&Tp.Buffer,16);
				 }
				 else if(uLED.PageCnt==1)
				 {
					DFE.Read(EE_JT_M_APN,16,Tp.Buffer);				  //将地址改为该数据地址即可	APN
				  	strncpy((char *)&OLED_Buffer[1][0],(const char *)&Tp.Buffer,16);

					DFE.Read(EE_JT_TCP_PORT,4,(uchar *)&t);				  //将地址改为该数据地址即可	TCP端口
					SetBufferValue(Tp.Buffer, 6, 0);
					Int2Str(t,(char*)&Tp.Buffer[0]);
				  	strncpy((char *)&OLED_Buffer[2][11],(const char *)&Tp.Buffer, strlen((char *)Tp.Buffer));
					DFE.Read(EE_JT_UDP_PORT,4,(uchar *)&t);				  //将地址改为该数据地址即可	UDP端口
					SetBufferValue(Tp.Buffer, 6, 0);
					Int2Str(t,(char*)&Tp.Buffer[0]);
					strncpy((char *)&OLED_Buffer[3][11],(const char *)&Tp.Buffer,strlen((char *)Tp.Buffer));
				 }
				 else if(uLED.PageCnt==2)
				 {
				  	DFE.Read(EE_SIM_NUMBER,16,Tp.Buffer);				  //将地址改为该数据地址即可	  终端SIM卡号码  本机手机号码
				  	strncpy((char *)&OLED_Buffer[1][0],(const char *)&Tp.Buffer[1],15);

					DFE.Read(EE_JT_CAR_OWNER_PHONE_NUM,16,Tp.Buffer);				  //将地址改为该数据地址即可	 车主手机号码
				   strncpy((char *)&OLED_Buffer[3][0],(const char *)&Tp.Buffer,16);
				 }
				 else if(uLED.PageCnt==3)
				 {
				    DFE.Read(EE_JT_CENTER_NUMBER,16,Tp.Buffer);				  //将地址改为该数据地址即可	  平台号码
				  	 strncpy((char *)&OLED_Buffer[1][0],(const char *)&Tp.Buffer,16);
					 DFE.Read(EE_JT_TEXT_SMS_NUMBER,16,Tp.Buffer);				  //将地址改为该数据地址即可	  SMS文本报警号码
				  	 strncpy((char *)&OLED_Buffer[3][0],(const char *)&Tp.Buffer,16);
				 }
				 else if(uLED.PageCnt==4)
				 {
				 	SetBufferValue(Tp.Buffer,16,0);		  
					if( CreateUserFlag!=OK)
					{
						strcpy((char *)Tp.Buffer, (char *)"未开户");
					}	
					else
					{
						strcpy((char *)Tp.Buffer, (char *)"已开户");
					}																		//将地址改为该数据地址即可		开户状态
				  	strncpy((char *)&OLED_Buffer[1][0],(const char *)&Tp.Buffer,16);

					if((JT808Flag.VS3.Bits.UseGPS==1)&&(JT808Flag.VS3.Bits.UseBD==1))
						memmove( &OLED_Buffer[3][0],(const uchar*)"使用GPS/北斗双模",16);
					else if(JT808Flag.VS3.Bits.UseGPS==1)
						memmove( &OLED_Buffer[3][0],(const uchar*)" 仅使用GPS定位  ",16);
					else if(JT808Flag.VS3.Bits.UseBD==1)
						memmove( &OLED_Buffer[3][0],(const uchar*)" 仅使用北斗定位 ",16);
					else
						memmove( &OLED_Buffer[3][0],(const uchar*)" 未使用任何卫星 ",16);
				  
				 }
				 else if ((uLED.PageCnt>=5) || (uLED.PageCnt <=7))
				 {
				 	if (uLED.PageCnt==5)
					{
						strncpy((char *)&OLED_Buffer[0][0],(const char *)&MobileCMNCSCR[uLED.PageCnt*4][0],16);
					 	SetBufferValue( &OLED_Buffer[1][0],16,0);
					 	Int2Str( SendInterval.DefauletTime,(char*)&OLED_Buffer[1][0]);
					 	i = strlen( (char*)&OLED_Buffer[1][0]);
					 	memmove( &OLED_Buffer[1][i],(const uchar*)"秒",2);

						DFE.Read(EE_PARA_BLOCK_STATUS,16,Tp.Buffer);
				 		SetBufferValue( &OLED_Buffer[3][0],16,0);
				 		memmove( &OLED_Buffer[2][0],(uchar*)"参数块配置1:",12);
				 		SetBufferValue( &OLED_Buffer[3][0],16,0);	

				 		if(Tp.B16[0] == PARA_BLOCK_CONFIG_FLAG)
				 			strcpy((char*)&OLED_Buffer[3][0],"已配置\0");
				 		else
				 			strcpy((char*)&OLED_Buffer[3][0],"未配置\0");
					}
					else  if(uLED.PageCnt==6)
					{
					  	DFE.Read(EE_PARA_BLOCK_STATUS,16,Tp.Buffer);
					 	SetBufferValue( &OLED_Buffer[1][0],16,0);
					 	memmove( &OLED_Buffer[0][0],(uchar*)"参数块配置2:",12);
					 	SetBufferValue( &OLED_Buffer[1][0],16,0);	
					
					 	OLED_Buffer[0][11] = '2';
					 	if(Tp.B16[1] == PARA_BLOCK_CONFIG_FLAG)
				 			strcpy((char*)&OLED_Buffer[1][0],"已配置\0");
				 		else
				 			strcpy((char*)&OLED_Buffer[1][0],"未配置\0");

						memmove( &OLED_Buffer[2][0],(uchar*)"参数块配置3:",12);
						SetBufferValue( &OLED_Buffer[3][0],16,0);
					 	if(Tp.B16[2] == PARA_BLOCK_CONFIG_FLAG)
				 			strcpy((char*)&OLED_Buffer[3][0],"已配置VDR参数\0");
				 		else
				 			strcpy((char*)&OLED_Buffer[3][0],"未配置VDR参数\0");
					}
		
					////
				 }
				 else 
				  uLED.PageCnt = 0;


			break;
	      case Schedul_MSG_MENU:		  //调度信息
				if(uLED.PageCnt<3)
			   {
					 //i = uLED.PageCnt;
			    	 strncpy((char *)&OLED_Buffer[0][0],(const char *)&SchedulMSGSCR[uLED.PageCnt][0],16);
			    	 strncpy((char *)&OLED_Buffer[1][0],(const char *)&SchedulMSGSCR[uLED.PageCnt+1][0],16);
			   }
				else
				{
				    uLED.PageCnt = 0;
				}

				 if(TwinkleFlag)
			    {
				   TwinkleFlag = 0;
					strncpy((char *)&OLED_Buffer[0][0],"                ",16);			  //总在第一行闪烁
		       }
			 	 else
				 {
					TwinkleFlag = 1;
				 }
			 
			break;

			case Set_Load_OK_Menu:																	 //设置车辆载重成功
			 strncpy((char *)&OLED_Buffer[0][0],(const char *)&SchedulMSGSCR[6][0],16);
			 strncpy((char *)&OLED_Buffer[1][0],(const char *)&SchedulMSGSCR[7][0],16);


	      break;
			case Set_Fales_Menu:																	  //设置车辆载重失败
			 strncpy((char *)&OLED_Buffer[0][0],(const char *)&SchedulMSGSCR[8][0],16);
			 strncpy((char *)&OLED_Buffer[1][0],(const char *)&SchedulMSGSCR[9][0],16);
		 	  
			
			
			break;
	      case Phone_Book_MENU:  	     //电话簿
			 
			 strncpy((char *)&OLED_Buffer,(const char *)&PhoteBookSCR[0][0],16); 
			 strncpy((char *)&OLED_Buffer[1][0],(const char *)&PhoteBookSCR[1][0],16);


			break;
	      case Milage_MENU:				  //360小时里程	及两个日历天里程


			if(uLED.PageCnt==0)			  // 脉冲统计总里程
		   {	
			 	memmove( &OLED_Buffer,(const char *)&MilageSCR[0][0],16);
				SetBufferValue(Tp.Buffer,16,0);
				Int2Str(SystemLog.VDR.SpeedSensorPulsCnt/VehiclePulseKV,(char *)Tp.Buffer);
				strcat( (char *)Tp.Buffer,".");
				Tp.Buffer[strlen((char*)Tp.Buffer)]=(((SystemLog.VDR.SpeedSensorPulsCnt%VehiclePulseKV)*10)/VehiclePulseKV)+'0';
				strcat( (char *)Tp.Buffer,"km");
				memmove(&OLED_Buffer[1][0],&Tp.Buffer,16); 
			 }
			 else if(uLED.PageCnt==1)		 		// GPS统计
		    {												
			 	memmove( &OLED_Buffer,(const char *)&MilageSCR[1][0],16);
				SetBufferValue(Tp.Buffer,16,0);
				Int2Str(SystemLog.GPS.TotalMileage/1000,(char *)Tp.Buffer);
				strcat( (char *)Tp.Buffer,".");
				Tp.Buffer[strlen((char*)Tp.Buffer)]= ((SystemLog.GPS.TotalMileage%1000)/100)+'0';
				strcat( (char *)Tp.Buffer,"km");
				strncpy((char *)&OLED_Buffer[1][0],(char *)&Tp.Buffer,16); 
			 }
			 else if(uLED.PageCnt==2)
			 {
			 	SetBufferValue(Tp.Buffer,16,0);
				Int2Str(TestMileage.Puls/VehiclePulseKV,(char *)Tp.Buffer);
				strcat( (char *)Tp.Buffer,".");
				Tp.Buffer[strlen((char*)Tp.Buffer)]=(((TestMileage.Puls%VehiclePulseKV)*10)/VehiclePulseKV)+'0';
				strcat( (char *)Tp.Buffer,"km");
				memmove(&OLED_Buffer[0][0],&Tp.Buffer,16); 

				SetBufferValue(Tp.Buffer,16,0);
				Int2Str(TestMileage.GPS/1000,(char *)Tp.Buffer);
				strcat( (char *)Tp.Buffer,".");
				Tp.Buffer[strlen((char*)Tp.Buffer)]= ((TestMileage.GPS%1000)/100)+'0';
				strcat( (char *)Tp.Buffer,"km");
				strncpy((char *)&OLED_Buffer[1][0],(char *)&Tp.Buffer,16); 
			 }
			 else if(uLED.PageCnt==3)
			 {
			 	memmove( &OLED_Buffer,(const char *)&MilageSCR[2][0],16);
				memmove( &OLED_Buffer[1][0],(const char *)&MilageSCR[3][0],16);
			 }

			break;

			case  Ctr_MENU:   //终端控制
			 if(uLED.PageCnt<14)
		    {
				 i = uLED.PageCnt;
		    	 strncpy((char *)&OLED_Buffer[0][0],(const char *)&SetParaSCR[uLED.PageCnt][0],16);
		    	 strncpy((char *)&OLED_Buffer[1][0],(const char *)&SetParaSCR[uLED.PageCnt+1][0],16);
		    }
			 else
			 {
			    uLED.PageCnt = 0;
			 }
			 
			 if(TwinkleFlag)
		    {
			   TwinkleFlag = 0;
				strncpy((char *)&OLED_Buffer[0][0],"                ",16);			  //总在第一行闪烁
	       }
		 	 else
			 {
				TwinkleFlag = 1;
			 }
	      break;


			/*case SET_INTERVAL_MENU:

				if(uLED.PageCnt>=12)
					uLED.PageCnt = 0;
				//CurrentRegInfo = uLED.PageCnt;
				strncpy((char *)&OLED_Buffer[0][0],(const char *)&IntervalSCR[0][0],16);
				strncpy((char *)&OLED_Buffer[1][0],(const char *)&IntervalSCR[uLED.PageCnt+1][0],16);
			  	if(TwinkleFlag)
			    {
				   TwinkleFlag = 0;
					strncpy((char *)&OLED_Buffer[1][0],"                ",16);			  //总在第二行闪烁
		       }
			 	 else
				 {
					TwinkleFlag = 1;
				 }

			break; */


			case  Comfir_Logout_MENU:	  //确认注销菜单
			    strncpy((char *)&OLED_Buffer[0][0],(const char *)&SetParaSCR[SET_PARA_COMFIR+0][0],16);
		    	 strncpy((char *)&OLED_Buffer[1][0],(const char *)&SetParaSCR[SET_PARA_COMFIR+1][0],16);
		   break;
	      case  Logouting_MENU: 		  //正在注销菜单
				 strncpy((char *)&OLED_Buffer[0][0],(const char *)&SetParaSCR[SET_PARA_COMFIR+2][0],16);
		    	 strncpy((char *)&OLED_Buffer[1][0],(const char *)&SetParaSCR[SET_PARA_COMFIR+3][0],16);
			break;      
	      case  Logout_OK_MENU:		  //注销成功菜单
				 strncpy((char *)&OLED_Buffer[0][0],(const char *)&SetParaSCR[SET_PARA_COMFIR+4][0],16);
		    	 strncpy((char *)&OLED_Buffer[1][0],(const char *)&SetParaSCR[SET_PARA_COMFIR+5][0],16);
				 KEY_DL.LongTimeNoKey = 20*5;
			break;   
			case  Logout_Fales_MENU:		  //注销失败菜单
				 strncpy((char *)&OLED_Buffer[0][0],(const char *)&SetParaSCR[SET_PARA_COMFIR+6][0],16);
		    	 strncpy((char *)&OLED_Buffer[1][0],(const char *)&SetParaSCR[SET_PARA_COMFIR+7][0],16);
				 KEY_DL.LongTimeNoKey = 20*5;
			break;
         case  Logout_Fales_MENU1:		  //请输入授权卡提示菜单
				 strncpy((char *)&OLED_Buffer[0][0],(const char *)&SetParaSCR[SET_PARA_COMFIR+8][0],16);
		    	 strncpy((char *)&OLED_Buffer[1][0],(const char *)&SetParaSCR[SET_PARA_COMFIR+9][0],16);
				 KEY_DL.LongTimeNoKey = 20*5;
			break;


	      case Product_MSG_MENU:		  //产品版本信息
			 if(uLED.PageCnt<3)
			 {
				 i = 0;
				 if(uLED.PageCnt!=0)
				    i = uLED.PageCnt*4;
				
				 strncpy((char *)&OLED_Buffer,(const char *)&SoleNumberSCR[i][0],16);
				 strncpy((char *)&OLED_Buffer[1][0],(const char *)&SoleNumberSCR[i+1][0],16);
				 strncpy((char *)&OLED_Buffer[2][0],(const char *)&SoleNumberSCR[i+2][0],16);
				 strncpy((char *)&OLED_Buffer[3][0],(const char *)&SoleNumberSCR[i+3][0],16);
				 Sys.GetVersion((char *)&Tp.Buffer[0]);
			 }
			 if(uLED.PageCnt==0)
			 {
			 		t = strlen( (char*)sOEM_ID);	
					if(t>16)	t = 16;
					memmove( (uchar *)&OLED_Buffer[1][0], (uchar*)sOEM_ID, t );						// 序号		
					
					strncpy((char *)&OLED_Buffer[3][0],(const char *)&Tp.Buffer[MV_POS_MODEL],LEN_MODEL); 			// 型号
					 		
			 }
			 else if(uLED.PageCnt==1)
			 {
			 	  strncpy((char *)&OLED_Buffer[1][0],(const char *)&Tp.Buffer[MV_POS_HV],5);			// 硬件版本	
				  strncpy((char *)&OLED_Buffer[3][0],(const char *)&Tp.Buffer[MV_POS_SV],5);			// 软件版本
			 }
			 else if(uLED.PageCnt==2)
		    {
				strncpy((char *)&OLED_Buffer[1][0],(const char *)&Tp.Buffer[BUILD_DATE_POS],11);		// 编译日期 
				strncpy((char *)&OLED_Buffer[3][0],(const char *)&Tp.Buffer[BUILD_TIME_POS],8);			// 编译时间
		    }
			 else 
				  uLED.PageCnt = 0;

		  break;
		  
		/// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 打印  
		case  PRINT_MENU:
			 PrintEvent2 = 1;
			 strncpy((char *)&OLED_Buffer,(const char *)&PrintSCR,16);
			 strncpy((char *)&OLED_Buffer[1][0],(const char *)&PrintSCR[1][0],16);
		  
		  break;

		case  PRINT_Comfir_MENU:
		    strncpy((char *)&OLED_Buffer,(const char *)&PrintSCR[4],16);
			 strncpy((char *)&OLED_Buffer[1][0],(const char *)&PrintSCR[5][0],16);
		  break;

		case  PRINT_NO_PAPER_MENU:
		    strncpy((char *)&OLED_Buffer,(const char *)&PrintSCR[6],16);
			 strncpy((char *)&OLED_Buffer[1][0],(const char *)&PrintSCR[7][0],16);   
		  break;
		case  DonotRemoveDiskSCR_MENU:
			if(Wfile_Flag)
			{
				strncpy((char *)&OLED_Buffer,(const char *)&DonotRemoveDiskSCR,16);
				strncpy((char *)&OLED_Buffer[1][0],(const char *)&DonotRemoveDiskSCR[1][0],16);
				KEY_DL.LongTimeNoKey = 36*5;
			}
			else
			{
				strncpy((char *)&OLED_Buffer,(const char *)&CanRemoveDiskSCR,16);
				strncpy((char *)&OLED_Buffer[1][0],(const char *)&CanRemoveDiskSCR[1][0],16);
			}
			if(Write_File_Error)
			{
			  strncpy((char *)&OLED_Buffer,(const char *)&DonotRemoveDiskSCR[2][0],16);
			  strncpy((char *)&OLED_Buffer[1][0],(const char *)&DonotRemoveDiskSCR[3][0],16);
			  //KEY_DL.LongTimeNoKey = 30*5;
			  //write_file_error_flag = 0;
			}
		  
		  break;   		
		/// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 
		case CONFIRM_MENU:			//	  现在用作快捷键，在缺省界面，按上翻键即进入此处
		  
		  /*if(uLED.PageCnt>1)
		    uLED.PageCnt = 0; 
		  strncpy((char *)&OLED_Buffer,(const char *)&ConfirmSCR[uLED.PageCnt][0],16);
		  strncpy((char *)&OLED_Buffer[1][0],(const char *)&ConfirmSCR[uLED.PageCnt+1][0],16);
		  i = uiSpeed/10;
		  if(uiSpeed%10>5)
		     i++;
		  if(GPS_Speed>5)
		     i = GPS_Speed;
		  if(uLED.PageCnt==0)
		  {
			  Rtc.GetNowDateTimeString(1,&Rtc.DT, Tp.Buffer);   //日期
			  memmove(&OLED_Buffer[0][5], Tp.Buffer, 10);
			  /////////////////////速度
			  OLED_Buffer[1][5]   = i/100 + 0x30;
			  OLED_Buffer[1][6]   = i%100/10 + 0x30;
			  OLED_Buffer[1][7]   = i%100%10 + 0x30;
		  }
		  else if(uLED.PageCnt==1)
		  {
		  	  /////////////////////速度
		
			  OLED_Buffer[0][5]   = i/100 + 0x30;
			  OLED_Buffer[0][6]   = i%100/10 + 0x30;
			  OLED_Buffer[0][7]   = i%100%10 + 0x30;
		  
		  	  /////////////////////里程
			 DFE.Read(FM_MILAGE_HEX,4,(uchar *)&t);
			 c = t%10;
			 t /= 10;
			 Int32ToDecChar(t, (char *)Tp.Buffer);	
			 strcpy((char *)&OLED_Buffer[1][7], (char *)Tp.Buffer);
			 i = strlen((char *)Tp.Buffer);
			 OLED_Buffer[1][7+i] = '.';
			 OLED_Buffer[1][7+i+1] = c + 0x30;
			 //OLED_Buffer[1][7+i+2] = 'k';
			 //OLED_Buffer[1][7+i+3] = 'm';
		  
		  } */
		  for (i=0; i<4; i++)
		  {
		   	strncpy((char *)&OLED_Buffer[i][0],(const char *)&ConfirmSCR[i][0],16);
		  }

		  // 经度 单位为度
		  t = (GPS.GYPack.Longitude/(10000000UL) );
		  Int2Str(t,(char *)Tp.Buffer);
		  strcat((char *)Tp.Buffer,".");
		  i= strlen((char *)Tp.Buffer);
		  t = ((GPS.GYPack.Longitude/100)%(100000UL) );
		  Int2Str(t,(char *)&Tp.Buffer[i]);
		  strcat((char *)Tp.Buffer,"度");
		  i= strlen((char *)Tp.Buffer);
		  memmove(&OLED_Buffer[0][5],Tp.Buffer,i);
		   // 纬度 单位为度
		  t = (GPS.GYPack.Latitude/(10000000UL) );
		  Int2Str(t,(char *)Tp.Buffer);
		  strcat((char *)Tp.Buffer,".");
		  i= strlen((char *)Tp.Buffer);
		  t = ((GPS.GYPack.Latitude/100)%(100000UL) );
		  Int2Str(t,(char *)&Tp.Buffer[i]);
		  strcat((char *)Tp.Buffer,"度");
		  i= strlen((char *)Tp.Buffer);
		  memmove(&OLED_Buffer[1][5],Tp.Buffer,i);
		  // 海拔 米
		  Int2Str(GPS_Altitude,(char *)&OLED_Buffer[2][5]);
		  strcat((char *)&OLED_Buffer[2][5],(char *)"米");

		  // 速度
		  Int2Str(uiGPS_Speed/10,(char *)&OLED_Buffer[3][5]);
		  strcat((char *)&OLED_Buffer[3][5], "km/h");

		 
		  break;   	
		
		case UPDATE_INDICATE:
		  strncpy((char *)&OLED_Buffer[0][0],(const char *)&SysUpdateSpeedSCR[0][0],16);
		  strncpy((char *)&OLED_Buffer[1][0],(const char *)&SysUpdateSpeedSCR[1][0],16);

		  break;   
		case DEFAULT_DISP:			// 
			
			strncpy((char *)&OLED_Buffer,(const char *)&DefaultScreen,16);
			 //SetBufferValue(&OLED_Buffer[2][0],16,0);
			//SetBufferValue(&OLED_Buffer[3][0],16,0);
			
			
			if( WaitIC_CardAuthTime )
			{
				memmove((char *)&OLED_Buffer[1][0],"驾驶员认证:     ",16);
				OLED_Buffer[3][12] = WaitIC_CardAuthTime/10+'0';
				OLED_Buffer[3][13] = WaitIC_CardAuthTime%10+'0';				
			}
			else if(IC_CardAuthFailure)		// 失败提示
			{
				if(IC_CardAuthFailure==1)		// 
				{
					memmove((char *)&OLED_Buffer[3][0],"认证失败，密匙错",16);
				}
				else if(IC_CardAuthFailure==2)	
				{
					memmove((char *)&OLED_Buffer[3][0],"认证失败，卡被锁",16);
				}
				else if(IC_CardAuthFailure==3)	
				{
					memmove((char *)&OLED_Buffer[3][0],"认证失败，卡拔出",16);
				}
				else if(IC_CardAuthFailure==4)	
				{
					memmove((char *)&OLED_Buffer[3][0],"认证失败，校验错",16);
				}
				else //if(IC_CardAuthFailure==3)	
				{
					memmove((char *)&OLED_Buffer[3][0],"  读卡器无应答  ",16);
				}
			}
			else if((JT808Flag.VS5.Bits.OverSpeedAlarm == 1))  /////////////////// 超速驾驶
			{
			   strncpy((char *)&OLED_Buffer[3][0],(const char *)&DefaultScreen[5][0],16);
			}
			else	if((JT808Flag.VS5.Bits.FatigueDriving == 1)&&((uiSpeed>50)||(GPS_Speed>5)))  /////////////////// 疲劳驾驶
			{
			   strncpy((char *)&OLED_Buffer[3][0],(const char *)&DefaultScreen[4][0],16);
			}
			else if ((JT808Flag.VS6.Bits.FatiguePreAlarm == 1)&&((uiSpeed>50)||(GPS_Speed>5))) // 疲劳预警
			{
				 strncpy((char *)&OLED_Buffer[3][0],(const char *)&DefaultScreen[6][0],16);
			}
			else
			{
			  if((DriverCardFlag==TRUE))																					//是否驾驶卡	 &&(StopFlag==1)
		      {
					strncpy((char *)&OLED_Buffer[3][0],(const char *)&DefaultScreen[2][0],16);
					DFE.Read(EE_JT_DRIVER_NAME_LEN,1,(uchar *)&t); 	// 姓名长度
					if(t>8)	t = 8;
					DFE.Read(EE_JT_DRIVER_NAME,t,&OLED_Buffer[3][7]);
				}
				else if ((AlertLogonTime>0)&&(AlertLogonTime<=(30*60)))	  // 提示登陆时间不为0，说明有速度，30分钟内显示
				{
					if ((gCnt.InsertIC_Card)&&(StopFlag==0))
					{
						strcpy((char *)&OLED_Buffer[3][0],(const char *)"请先停车,再插卡!");
					}
					else
					{
						strncpy((char *)&OLED_Buffer[3][0],(const char *)&DefaultScreen[3][0],16);
					}
				}
			}

		   //////////////////////////////////////////	Speed
			strncpy((char *)&OLED_Buffer[2][0],(const char *)&DefaultScreen[1][0],16);

			//速度状态
			if (SpeedStatusJudge.Status==SPEED_STATUS_UNNORMAL)
			{
				memmove((uchar *)&OLED_Buffer[2][8],(uchar *)"速度异常",8);	
			}
							// 不显示小数			
			/*i = uiSpeed/10;
			OLED_Buffer[2][0]   = i/100 + 0x30;
			OLED_Buffer[2][1]   = i%100/10 + 0x30;
			OLED_Buffer[2][2]   = i%100%10 + 0x30;
			i = GPS_Speed;
			OLED_Buffer[2][9]   = i/100 + 0x30;
			OLED_Buffer[2][10]  = i%100/10 + 0x30;
			OLED_Buffer[2][11]  = i%100%10 + 0x30;


			if(OLED_Buffer[2][0]=='0')
			{
				OLED_Buffer[2][0] = ' ';
				if(OLED_Buffer[2][1]=='0')
					OLED_Buffer[2][1] = ' ';
			}
			if(OLED_Buffer[2][9]=='0')
			{
				OLED_Buffer[2][9] = ' ';
				if(OLED_Buffer[2][10]=='0')
					OLED_Buffer[2][10] = ' ';
			}*/

			if ((uiSpeed<5)&&(SpeedStatusJudge.Status==SPEED_STATUS_UNNORMAL)&&(GPSValidFlag == GPS_VALID_FLAG))
			{	  //  速度异常，无脉冲速度，定位，屏幕要切成GPS速度
				 i = uiGPS_Speed/10;
			}
			else
			{
				i = uiSpeed/10;
			}
			
			OLED_Buffer[2][0]   = i/100 + 0x30;
			OLED_Buffer[2][1]   = i%100/10 + 0x30;
			OLED_Buffer[2][2]   = i%100%10 + 0x30;

			if(OLED_Buffer[2][0]=='0')
			{
				OLED_Buffer[2][0] = ' ';
				if(OLED_Buffer[2][1]=='0')
					OLED_Buffer[2][1] = ' ';
			}
	

		    /////////////////////////////////////////GPRS 是否在线
			if(GG_Status.Flag.Bits.Online == 1)		
	      {
			  if((PhoneCall.Busing != TRUE)&&(PhoneCall.DialFlag != TRUE)&&(PhoneCall.RingFlag != TRUE)) 
			  {
			   	if (jt_auth_ok_flag != TRUE) 
			    		OLED_Buffer[0][2] = 'g';
			    	else
			    		OLED_Buffer[0][2] = 'G';
			  }
			}
			 /////////////////////////////////////////GSM 信号强度
			if((GG_Status.Signal>2)&&(GG_Status.Signal<32))
			  OLED_Buffer[0][1]   = (GG_Status.Signal+4)/6 + 0xA1 + UseWthSimCard;					  //分5档显示信号符号

			/////////////////////////////////////////GPS 卫星个数
			if(NumberOfSatlice!=0)
		   {
		     OLED_Buffer[0][4]   = NumberOfSatlice>>8;
			  OLED_Buffer[0][5]   = NumberOfSatlice;
		    
			  if(OLED_Buffer[0][4]=='0')
		        OLED_Buffer[0][4] = ' ';
		   }

			//////////////////////////////////////		定位标志
			if(VirtaulGPS_Valid==TRUE)
				OLED_Buffer[0][3] = 'A';
			else
				OLED_Buffer[0][3] =  (GPSValidFlag == GPS_VALID_FLAG) ? 'A':'V';
			
			Rtc.GetLocalTimeString(1,&OLED_Buffer[0][8]);	  // 时间
		
			SetBufferValue(&OLED_Buffer[1][0],16,0);
			Rtc.GetNowDateTimeString(1,&Rtc.DT, Tp.Buffer);   //日期
			memmove(&OLED_Buffer[1][3], Tp.Buffer, 10);
			
			
			///////////////////////////////////////// 通讯标志
			break;
		case REG_INFO_MENU:

		  	if(uLED.PageCnt>=27)
				uLED.PageCnt = 0;
			CurrentRegInfo = uLED.PageCnt;
			strncpy((char *)&OLED_Buffer[0][0],(const char *)&RegInfoSCR[0][0],16);
			strncpy((char *)&OLED_Buffer[1][0],(const char *)&RegInfoSCR[uLED.PageCnt+1][0],16);
		  	if(TwinkleFlag)
		    {
			   TwinkleFlag = 0;
				strncpy((char *)&OLED_Buffer[1][0],"                ",16);			  //总在第二行闪烁
	       }
		 	 else
			 {
				TwinkleFlag = 1;
			 }
		break;

			default:
			break;
	}		
	NOP();	NOP();	
	DispScreen(0);		 // 
	NOP();	NOP();				
}	// End of <Disp> Function
	


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：DispScreen
   功能描述：显示一屏字符
   入口参数：无
   出口参数：无
   全局变量: 结构LCM
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			KeyDispCls::DispScreen(uchar DD)	 
{
	uchar		i,j,k,t,n,Page;
	uint32	    pCode;
	uchar		PageBuffer[130],DotBuffer[18];
	uint16   	CN_Code,xc;
	uchar		const	*ASCII_pCode;
	
	NOP();	NOP();	
	NOP();	NOP();

	pCode = pCode;

	////////////////////////////////////////////////////////////////////////////
	for(i=0,Page = 0;i<E308_TOTAL_CN_LINE;i++)					   // 2行汉字
	{
		for(j=0,n=0;j<16;j++)									         // 每行8个汉字(上半部分)
		{	
			xc = OLED_Buffer[i][j];

			if(xc==0)  xc = 0x20;
			  
			if((xc<=0x7F)&&(xc>=0x20))									   //判断是否ASCII码
			{
				ASCII_pCode = ASCII_Code + (xc - 0x20)*16;  		   //ASCII码地址
			   t = 8;												         //要取的点阵为ASCII码
			   GetASCIIDotCode(t, DotBuffer, ASCII_pCode);
			}
			else
			{
			  	CN_Code = xc<<8;
			   j++;
			   xc = OLED_Buffer[i][j];
			   CN_Code |= xc;
			    
			   pCode = CalZiMo_Addr(CN_Code);		               // 该汉字和或特殊符号在字库存储的首址	 GB2312_CODE_ADDR  从0x3C0000开始，占256Kbyte 
			   pCode += GB2312_CODE_ADDR;
				t = 16;											            
				DFF.AnyNumberRead(pCode, t, DotBuffer); 
			 }

			for(k=0;k<t;k++)								               // 每个汉字或字符的下半部分字模，即16或8字节
			{	PageBuffer[n+k] = DotBuffer[k]; 	}
			n += t;
		 }
		OLED.DataDisplaying(0,Page++,4,128,PageBuffer); 		
		/////////////////////////////////////////	
		for(j=0,n=0;j<16;j++)									         // 每行10个汉字(下半部分)
		{	
			xc = OLED_Buffer[i][j];
			if(xc==0)  xc = 0x20;
			if((xc<=0x7F)&&(xc>=0x20))									   //判断是否ASCII码
			  {
				ASCII_pCode = ASCII_Code + (xc - 0x20)*16 + 8; 		           //ASCII码地址 
			   t = 8;												         //要取的点阵为ASCII码
			   GetASCIIDotCode(t, DotBuffer, ASCII_pCode);
			  }
			else
			  {
			     CN_Code = xc<<8;
			     j++;
			     xc = OLED_Buffer[i][j];
			     CN_Code |= xc;
				  pCode =  CalZiMo_Addr(CN_Code);		             // 该汉字或特殊符号在字库存储的首址	 GB2312_CODE_ADDR  从0x0000开始，占2611688byte 
			     pCode += GB2312_CODE_ADDR + 16;
			     t = 16;	
				  DFF.AnyNumberRead(pCode, t, DotBuffer); 
				  
			   }

			for(k=0;k<t;k++)								    	          // 每个汉字或字符的下半部分字模，即16或8字节
			{	PageBuffer[n+k] = DotBuffer[k]; 	}
			n += t;
			
		  }
	    OLED.DataDisplaying(0,Page++,4,128,PageBuffer);
	}
	NOP();	NOP();
	Disp_CSDisable;				
}	// End of <Disp> Function	


																								  

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   函数名称：uint32			CalZiMo_Addr(uint16   cCode);
   功能描述：根据汉字内码，计算字模在汉字库里的起始地址
   入口参数：汉字内码
   出口参数：字模地址
   全局变量: 无
   创建时间：2011/07/22
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uint32		KeyDispCls::CalZiMo_Addr(uint16   cCode)
{
	uint32	AB,CD;
	AB = cCode>>8;			// 高字节
	CD = cCode&0x00ff;	    // 低字节
	return (((AB-0xa1)*94 + (CD-0xa1)) *32);
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   函数名称：Disp_GetGBDotCode
   功能描述：根据地址从flash取GB点阵编码
   入口参数：字符类型， 数据保存地址，数据地址
   出口参数：无
   全局变量: 无											
   创建时间：2011/07/22
   修改时间：
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void     KeyDispCls::GetGBDotCode(uchar ASorGB, uchar* DotBuffer, uint32 pCode)
{

   uint32    Dst;
   uint32    NByte;

   NByte = (uint32)ASorGB;
   Dst = pCode;
   DFE.Read(Dst, NByte,DotBuffer);
}
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   函数名称：Disp_GetASCIIDotCode
   功能描述：根据地址从flash取ASCII码点阵编码
   入口参数：字符类型， 数据保存地址，数据地址
   出口参数：无
   全局变量: 无
   创建时间：2011/07/22
   修改时间：
*/ 	
																	
void    KeyDispCls::GetASCIIDotCode(uchar t,uchar* DotBuffer, uchar const *pASCIICode)
{
   uchar  Cnt;
		for(Cnt = 0 ; t>0 ; t--)
		  {
		    
		    DotBuffer[Cnt] = *(pASCIICode+Cnt);
	  		 Cnt++;
		  }
}




          /*$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

                   以下按键部分
          $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  */


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：KeyProcess
   功能描述：按键处理
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void       	KeyDispCls::KeyProcess(void)   
{
   NOP();   NOP();
	KEY_DL.U32.KEY32 = Adc.GetKeyCode();					// 取键值，0xff为无效键，即无按键	  
	NOP();   NOP();
	//i = j>>24;
	WithoutAckOfServer++;
	if((WithoutAckOfServer>40000) || (RegisterWithoutTer >= 3)) //60000
	{
	    RegisterWithoutTer=0; //数据库中无该终端
		WithoutAckOfServer=0;  //服务器无应答
		GPRS.UnRegTerminal();	 //addBySun
	  	NOP();NOP(); 
	}
	RestartWithoutAckOfServer++;
    if(RestartWithoutAckOfServer>180000)
	{
	Uart.puts("System cool boot of 30S \r\n\0");
	os_dly_wait(100);
	DisableIRQ();
	NOP();	NOP();	NOP();	NOP();
	NVIC_SystemReset();
	}

	//
	if(KEY_DL.KeyPressTime==0)
   {
   	if((KEY_DL.U32.Psta.PreKeyCode!=0xff)&&(KEY_DL.U32.Psta.PreKeyPress!=0))
    	{

			#if(KEY_CODE_TO_PC)	 
			TestKey[0] = (j>>4) + '0';
			TestKey[1] = (j&0x0f) + '0';
			TestKey[2] = '\n';
			TestKey[3] = '\r';
			TestKey[4] = '\0';
			#endif
			 //KEY_DL.KeyPressTime = 0;
			 //j = j&0x000000ff;
			 IDEL_For_Acc_OFF = FALSE;					 //按键操作会退出休眠模式
			 IDEL_Dly_Cnt = 0;
			 KEY_DL.LongTimeNoKey = 20*5;
			 switch(KEY_DL.U32.Psta.PreKeyCode) 
			 {
		    case  4:
			  KeyMainMenu(); 
			  break;
			case  1:
			  KeyPageUp();
			  break;
			case  3:
			  KeyConfirm();
			  break;
			case  2:
			  KeyPrint();
			  break;
			case  0:			
			  KeyPageDown();
			  break;
			default:
			  break;
		   }
		 	
	    }

	}
	else
		KEY_DL.KeyPressTime--;

   NOP();   NOP();
   NOP();   NOP();
}

/*  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ */

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：KeyMainMenu
   功能描述： 功能按键
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void       	KeyDispCls::KeyMainMenu(void)			// 主菜单按键
{
   //LED   ULED;
   NOP();   NOP(); 
   if(1)
   {
   	if(KEY_DL.KeyPressTime==0)
   	{
			if(uLED.MainMenuCnt == DEFAULT_DISP)
			{
      		uLED.MainMenuCnt = MAIN_MENU;		          // 如果不是主菜单，就进入主菜单   
				uLED.SubMenuCnt = 1;
				uLED.PasswordOK = FALSE;
			}
			else if (uLED.MainMenuCnt == INPUT_MENU)
			{

				 if (uLED.SubMenuCnt==15)	  //  修改密码项
				 {
				 		if (uLED.InputModifyPwdFlag == 0)
						{
							 return;
						}
				 }

				 if (uLED.Offset>0)
				 {
				 	//OLED_Buffer[1][uLED.Offset] = SetParaBuffer[uLED.Offset];
					strcpy((char *)&OLED_Buffer[1][0], (char *)SetParaBuffer);
				 	uLED.Offset--;	
				 }
				 else
				 {
				 	uLED.Offset = strlen((char *)SetParaBuffer);//15;

				 }

				 
			}
			#if (WUXI_TEST == 1) 
			else if(uLED.MainMenuCnt == PRINT_Comfir_MENU) 	 //
			{ 
				uLED.MainMenuCnt = DEFAULT_DISP;
		   }
			else if(uLED.MainMenuCnt == PRINT_NO_PAPER_MENU) 
   		{
				uLED.MainMenuCnt = DEFAULT_DISP;
      	}
			else if(uLED.MainMenuCnt == DEFAULT_DISP) 
   		{
				uLED.MainMenuCnt = PRINT_Comfir_MENU;
      	}
			else if(uLED.MainMenuCnt == PRINT_MENU)
			{
			  uLED.MainMenuCnt = PRINT_MENU;
			}
			else if(uLED.MainMenuCnt == CONFIRM_MENU)
			{
			  uLED.MainMenuCnt = DEFAULT_DISP;
			}
			else if(uLED.MainMenuCnt==MAIN_MENU)
			{
				uLED.MainMenuCnt = DEFAULT_DISP;
			}
			else if (uLED.MainMenuCnt==SET_PARA_MENU)
			{
//				if(uLED.Offset<uLED.MaxOffset)
//					uLED.Offset++;
//				else
//					uLED.Offset = 0;
					uLED.Offset = 0;
					uLED.InputStatus = INPUT_INIT_STATUS;
					uLED.InputPageCnt = 0;
					uLED.PasswordOK = FALSE;
					uLED.InputAlert = FALSE;
					uLED.MainMenuCnt = MAIN_MENU;
					uLED.SubMenuCnt = 11;		  // 	 指向 “参数设置”

				
			}
			else if (uLED.MainMenuCnt==INPUT_MENU)
			{
				if (uLED.SubMenuCnt==0)
				{
					uLED.Offset = 0;
					uLED.InputStatus = INPUT_INIT_STATUS;
					uLED.InputPageCnt = 0;
					uLED.PasswordOK = FALSE;
					uLED.InputAlert = FALSE;
					uLED.MainMenuCnt = MAIN_MENU;
					uLED.SubMenuCnt = 11;		  // 	 指向 “参数设置”
					
				}
				else
				{
					uLED.Offset = 0;
					uLED.InputStatus = INPUT_INIT_STATUS;
					uLED.InputPageCnt = 0;
					//uLED.PasswordOK = FALSE;
					uLED.InputAlert = FALSE;
					uLED.MainMenuCnt = SET_PARA_MENU;
					uLED.InputModifyPwdFlag = 0;
				}
			 }
			 else
			 {
				uLED.MainMenuCnt = MAIN_MENU;
				//uLED.SubMenuCnt = 1;
			 }

			 #endif
			//
			 				
		}		
		KEY_DL.KeyPressTime = 20;
		BEEP(9,0,2,0,5);
		//if(DispCnt%20==0)
	   //Uart.putb(1,4,(uchar*)&KEY_DL.U32.KEY32);
		
   }
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：IsInputFinish
   功能描述：判断是否完成输入
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用

uchar      KeyDispCls::IsInputFinish(uchar ParaFlag)
{
	 

	 if (uLED.InputStatus ==  INPUT_INIT_STATUS)	  // 初始进入
	 {
	 		uLED.MainMenuCnt = INPUT_MENU;
	 		uLED.InputAlert = FALSE;
	 		uLED.Offset = 0;
	 		strncpy((char *)&OLED_Buffer[0][0],(const char *)&SetRegParaSCR[uLED.SubMenuCnt][0],16);
	 		strncpy((char *)&OLED_Buffer[1][0],"                ",16);			  // 清空显示屏第二行
	
			uLED.SetParaFlag = ParaFlag;
			if (ParaFlag == SET_PARA_INPUT_NUM_AND_DOT)
			{
				//uLED.SetParaFlag = SET_PARA_INPUT_NUM_AND_DOT;
				OLED_Buffer[1][0] = 	InputSelectNumDot[0];//InputSelectChar[0][0];
			}
			else 	if (ParaFlag == SET_PARA_INPUT_NEED_FOR_COLOR)
			{
				 OLED_Buffer[1][0] = 	InputSelectColor[0];
			}
			else if (ParaFlag == SET_PARA_INPUT_NEED_FOR_CAR_NUM)
			{
				 OLED_Buffer[1][0] = 	InputSelectCarNum[0];
			}
			

			
	  }
	  else if (uLED.InputStatus == INPUT_FINISH_ALL_CHAR)			 //  输入所有字符后确认的
	  {
	  		//Uart.puts("input:"); Uart.puts((char *)SetParaBuffer);	
			//uLED.InputStatus 	= INPUT_INIT_STATUS;
			uLED.InputStatus 	= INPUT_INIT_STATUS;
	
			return	TRUE;
	  		
	  }

	  return FALSE;
	
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：InputAlertUser
   功能描述： 提示用户
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void      KeyDispCls::InputAlertUser(char *pMsg, uchar MsgLen)
{
	Uart.puts(pMsg);

	uLED.InputAlert = TRUE;
	SetBufferValue(SetParaBuffer,16,0);
	SetBufferValue(&OLED_Buffer[1][0], 16, 0);
	strncpy((char *)&OLED_Buffer[0][0],(const char *)&SetRegParaSCR[uLED.SubMenuCnt][0],16);
	memmove((uchar*)&OLED_Buffer[1][0],(uchar*)pMsg,MsgLen);
	uLED.MainMenuCnt = INPUT_MENU;
	
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：KeyConfirm
   功能描述： 确认按键
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
extern	uchar		jt_auth_code[];
extern	uchar		GPS_CoolStart;
static	uchar InputPassword[10];    // 修改密码时临时使用

void       	KeyDispCls::KeyConfirm(void)			// 主菜单按键: 确认
{
   uint32 ID;
   union
	{
		uchar		Buffer[64];
		uint32	B32[16];
	}Tp;
	uchar	s = 0;

	

   NOP();   NOP(); 
   //if(1)
   {
   	if(KEY_DL.KeyPressTime==0)
   	{
			if(uLED.MainMenuCnt==MAIN_MENU)				//主菜单
			{
				if(uLED.SubMenuCnt==1)
					uLED.MainMenuCnt = Car_MSG_MENU;  		   //车辆信息
				else if(uLED.SubMenuCnt==2)
					uLED.MainMenuCnt = OVER_TIME_MENU;	      //疲劳驾驶
				else if(uLED.SubMenuCnt==3)
					uLED.MainMenuCnt = DRIVER_INFO_MENU;		   //当前驾驶员信息
				else if(uLED.SubMenuCnt==4)
					uLED.MainMenuCnt = Milage_MENU;	         //里程
				else if(uLED.SubMenuCnt==5)	
					uLED.MainMenuCnt = AVG_SPEED_MENU;			//15分钟平均速度
				else if(uLED.SubMenuCnt==6)
					uLED.MainMenuCnt = VEHICLE_CC_MENU;	      // 脉冲系数					
				else if(uLED.SubMenuCnt==7)
					uLED.MainMenuCnt = Mobile_PARA_MENU;	      //无线通信参数
				else if(uLED.SubMenuCnt==8)
					uLED.MainMenuCnt = Schedul_MSG_MENU;		   //调度信息
				else if(uLED.SubMenuCnt==9)
					uLED.MainMenuCnt = Phone_Book_MENU;  	   //电话簿 
				else if(uLED.SubMenuCnt==10)
					uLED.MainMenuCnt = Ctr_MENU;	            //终端控制
            else if(uLED.SubMenuCnt==11)
            {
					uLED.MainMenuCnt = SET_PARA_MENU;	   		//参数设置
					SetBufferValue( (uchar *)&OLED_Buffer[1][0],16,0);
					SetBufferValue( SetParaBuffer,16,0);
					OLED_Buffer[1][0] = '0';
					uLED.PasswordOK = 0;
					uLED.Offset = 0;
					//uLED.MaxOffset = 16;
					uLED.MaxOffset = MAX_SET_PARA_MENU;//-1;
				}
				else if(uLED.SubMenuCnt==12)
					uLED.MainMenuCnt = Product_MSG_MENU;	      //产品版本信息
				
				uLED.PageCnt = 0;
			}
			else if(uLED.MainMenuCnt==Milage_MENU)	 //里程
			{
				if(uLED.PageCnt==3)
				{
					SetBufferValue((uchar*)&TestMileage,8,0);
			 		DFE.Write(EE_TEST_MILEAGE,8,(uchar*)&TestMileage);
					uLED.PageCnt = 2;
				}
			}
			else if(uLED.MainMenuCnt==Schedul_MSG_MENU)	 //车辆载物设置
			{
				
				if(uLED.PageCnt==0)			 // 空载
				{				    
					JT808Flag.VS2.Bits.CarLoadStatus	= 0x00;
				}
			   else if(uLED.PageCnt==1)			 // 半载
				{
					JT808Flag.VS2.Bits.CarLoadStatus	 = 0x01;
				}
				else if(uLED.PageCnt==2)				// 满载
				{
					JT808Flag.VS2.Bits.CarLoadStatus	 = 0x03;
				}
				if(uLED.PageCnt<=2)
				{
				   uLED.MainMenuCnt = Set_Load_OK_Menu;
				   GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_POS_REPORT,0,NULL);	     //发送一条位置信息 
					Uart.puts("载货变化，发送位置信息 ");
				}
				else
				{
				   uLED.MainMenuCnt = Set_Fales_Menu;
				}
			}
			else if((uLED.MainMenuCnt==Set_Load_OK_Menu)||(uLED.MainMenuCnt==Set_Fales_Menu))
			{
			   uLED.MainMenuCnt = Schedul_MSG_MENU;
			}
	 //	  控制菜单，实际使用不需要
	 		#if (WUXI_TEST == 1)
			else if(uLED.MainMenuCnt==Ctr_MENU)	        //控制菜单
			{
				if(uLED.PageCnt==0)
				{
				  uLED.MainMenuCnt = Comfir_Logout_MENU;  //注销本机 
				}
				else if(uLED.PageCnt==1)
				{
					SystemSelfTest = TRUE;
				}
				else if(uLED.PageCnt==2)	 	// 调度屏
				{
					Baudrate.U2_232 = 115200;
					Uart2.Init(Baudrate.U2_232,U2_FOR_DISPLAY);
					DFE.Write(EE_BAUDRATE_U2_232,4,(uchar*)&Baudrate.U2_232);
				}
				else if(uLED.PageCnt==3)	 	// 读卡器
				{
					Baudrate.U2_232 = 9600;
					Uart2.Init(Baudrate.U2_232,U2_FOR_RFID);
				}
				else if(uLED.PageCnt==4)		// 摄像头
				{
					Baudrate.U2_485 = 57600;
					Uart2.Init(Baudrate.U2_485,U2_FOR_CAMERA);
				}
				else if(uLED.PageCnt==5)	  	// 录音器
				{
					Baudrate.U2_232 = 115200;
					Uart2.Init(Baudrate.U2_232,U2_FOR_AUDIO);
				}
			   else if(uLED.PageCnt==6)	  // 手动重启系统
				{					
					Uart.puts("手动重启系统\r\n");
					os_dly_wait(50);

					NVIC_SystemReset(); 
				}
				#if (WUXI_TEST == 1)
				else if(uLED.PageCnt==7)	  // 清除速度异常
				{
					Uart.puts("清除速度异常\r\n");
					SetBufferValue((uchar *)&SpeedStatusJudge,LEN_SPEED_STATUS_JUDGE,0);
					DFE.Write(FM_SPEED_STAUTS_JUDGE,LEN_SPEED_STATUS_JUDGE,(uchar *)&SpeedStatusJudge);
				}
				else if(uLED.PageCnt==8)	  // 使用双模定位s
				{
					//JT808Flag.VS3.Bits.UseGPS = 1;
					//JT808Flag.VS3.Bits.UseBD = 1;
					//Tp.Buffer[0] = 0x03;
					//DFE.Write(EE_JT_GNNS_POS_MODE, 1, Tp.Buffer);
					TD.GPS = tGPS_ALL_DATA;
				}
				#endif
				#if(TEST_CODE_JUST_FOR_TEST_LUOYANG_NEW==1)
				else if(uLED.PageCnt==6)	  // 只使用GPS定位s
				{
					JT808Flag.VS3.Bits.UseGPS = 1;
					Tp.Buffer[0] = 0x01;
					DFE.Write(EE_JT_GNNS_POS_MODE, 1, Tp.Buffer);
				}
				else if(uLED.PageCnt==7)	  // 只使用北斗定位
				{
					JT808Flag.VS3.Bits.UseBD = 1;
					Tp.Buffer[0] = 0x02;
					DFE.Write(EE_JT_GNNS_POS_MODE, 1, Tp.Buffer);
				}
				else if(uLED.PageCnt==8)	  // 使用双模定位s
				{
					JT808Flag.VS3.Bits.UseGPS = 1;
					JT808Flag.VS3.Bits.UseBD = 1;
					Tp.Buffer[0] = 0x03;
					DFE.Write(EE_JT_GNNS_POS_MODE, 1, Tp.Buffer);
				}
				else if(uLED.PageCnt==9)	  // 次启动模块
				{
					GPS_CoolStart = TRUE;
					TD.GPS = tGPS_USE_GPS_BD;
					OnlyUseBD_GPS = 0;
				}
				else if(uLED.PageCnt==10)	  // 次启动GPS模块
				{
					GPS_CoolStart = TRUE;
					TD.GPS = tGPS_USE_GPS;
					OnlyUseBD_GPS = 0;
				}
				#endif
				else if(uLED.PageCnt==11)	  // GPRS重新链接
				{
					GPRS.Close(); 
				}
				#if(TEST_CODE_JUST_FOR_TEST_LUOYANG_NEW==1)
				else if(uLED.PageCnt==12)	  // 使用前锁定一
				{					
					DFE.Write(EE_JT_M_IP_DNS,16,(uchar*)"jt1.gghypt.net\0\0");	
					DFE.Write(EE_JT_B_IP_DNS,16,(uchar*)"jt2.gghypt.net\0\0");	
					Tp.B32[0] = 7008;
					DFE.Write(EE_JT_TCP_PORT,4,Tp.Buffer);
					DFE.Write(EE_JT_UDP_PORT,4,Tp.Buffer);
				}
				else if(uLED.PageCnt==13)	  // 切回主中心IP
				{
					DFE.Write(EE_JT_M_IP_DNS,16,(uchar*)"202.96.42.115\0\0\0");	
					DFE.Write(EE_JT_B_IP_DNS,16,(uchar*)"202.96.42.113\0\0\0");	
					Tp.B32[0] = 9999;
					DFE.Write(EE_JT_TCP_PORT,4,Tp.Buffer);
					DFE.Write(EE_JT_UDP_PORT,4,Tp.Buffer);
					sIP.EnableFlag[eFLAG_TCP] = ENABLE_FLAG;
					Tp.Buffer[0] = ENABLE_FLAG;
					DFE.Write(EE_ENABLE_TCP,1,Tp.Buffer);
					GPRS.Close();
				}
				#endif
			}		 
			#endif
			else if(uLED.MainMenuCnt==Comfir_Logout_MENU)
         {
			   uLED.MainMenuCnt = Logouting_MENU;
				if(0)//(SetCarFlat==0)
				   uLED.MainMenuCnt = Logout_Fales_MENU1;								  //请插入设置卡   
				else  if(((GG_Status.Flag.Bits.RegGSM == 1)||(GG_Status.Flag.Bits.Roaming==1)))  
				{
				   jt_dis_reg_flag = 1;
					if(GG_Status.Flag.Bits.Online == 0)
					{
						SMS_SendNumber[0] = 0x38;
						SMS_SendNumber[1] = 0x36;
						strcat((char *)&SMS_SendNumber[2], (char *)JT808_Number.CenterSMS);
						PDU_SMS.Ack = TRUE;
					   GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DEREG,0,NULL);
					   PDU_SMS.Ack = FALSE;
					}
					else
					{
					   GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DEREG,0,NULL); 	              //发送注销请求指令    
						Uart.puts("注销请求 ");
					}
				}																																																	
				else
				{
				   uLED.MainMenuCnt = Logout_Fales_MENU;								  //无网络，无法注销
				} 
			}
			else if(uLED.MainMenuCnt == PRINT_Comfir_MENU) 
   		{
			   if((uiSpeed<30)&&(PrintEvent1==0)&&(USBTaskCreat==0))
				{					
						uLED.MainMenuCnt = PRINT_MENU;
						Print.Init(TRUE);		
						PrintEvent1 = 1;
						Print.TickCnt = 0;
						KEY_DL.LongTimeNoKey = 0; 
						//UserEvtFlag.U0print = UART0_PRINT_EVT_FLAG;
						//os_evt_set(UART0_PRINT_EVT_FLAG,TaskID.U0print);   //触发打印事件
				}
			 }
			 else if(uLED.MainMenuCnt == SET_PARA_MENU)
			 {
DISPOSE_INPUT_ALL_CHAR:
			 	 switch(uLED.SubMenuCnt)
				 {
				 	case eSET_PARA_PWD:			// 密码
				 		DFE.Read(EE_SET_PARA_PASSWORD,4,Tp.Buffer);
						if (Tp.B32[0] == 0)
						{
							Tp.B32[0] = 123;		  // 缺省密码
						}

				 		Tp.B32[1] = Str2Int((char *)SetParaBuffer);

						//Uart.puts("pwd:"); Uart.puts((char *)SetParaBuffer);
				 		if(Tp.B32[0]==Tp.B32[1])
				 		{
							uLED.PageCnt = 1;
				 			uLED.PasswordOK = TRUE;
							uLED.InputStatus = INPUT_INIT_STATUS;
							//uLED.MainMenuCnt = SET_PARA_MENU;
							uLED.InputAlert = FALSE;
							uLED.SubMenuCnt = 1;
							//uLED.SetParaFlag = FALSE;
							uLED.InputModifyPwdFlag = 0;

				 		}
				 		else
				 		{
							uLED.InputAlert = FALSE;
				 			//SetBufferValue(SetParaBuffer,16,0);
				 			memmove((uchar*)&OLED_Buffer[1][0],(uchar*)"密码错误",8);
							//uLED.InputStatus = INPUT_FINISH_ALL_CHAR;
				 		}
						SetBufferValue(SetParaBuffer,16,0);
					
					break;

					case eSET_PARA_MAIN_IP:			// 主IP
						
					//break;

					case eSET_PARA_BAK_IP:			// 副IP
						if (IsInputFinish(SET_PARA_INPUT_NUM_AND_DOT) == TRUE)
						{
							for (s = 0, Tp.Buffer[0] = 0; s < LEN_INPUT_BUFFER; s++)
							{
								if (SetParaBuffer[s] == '.')
								{
									Tp.Buffer[0]++;
								}
							}
							if (Tp.Buffer[0] != 3)			  // 非法IP
							{
									InputAlertUser("IP地址非法", 10);
							}
							else
							{
								//uLED.InputStatus 	= INPUT_INIT_STATUS;
								// 保存IP
								s = strlen((char *)SetParaBuffer);

								if (uLED.SubMenuCnt==1) // 主IP
								{
									Uart.puts("主IP");
									
									SetBufferValue(&JT808_IP_DNS.M_IPorDNS[0], LEN_IPorDNS, 0); 
									memmove((char *)&JT808_IP_DNS.M_IPorDNS[0], (char *)SetParaBuffer, s);
									DFE.Write(EE_JT_M_IP_DNS, LEN_IPorDNS, (uchar *)&JT808_IP_DNS.M_IPorDNS[0]);
									NOP();NOP();NOP();

								}
								else						// 副IP
								{
									 Uart.puts("副IP");
									 SetBufferValue(&JT808_IP_DNS.B_IPorDNS[0], LEN_IPorDNS, 0); 
									 memmove((char *)&JT808_IP_DNS.B_IPorDNS[0], (char *)SetParaBuffer, s);
									 DFE.Write(EE_JT_B_IP_DNS, LEN_IPorDNS, (uchar *)&JT808_IP_DNS.B_IPorDNS[0]);
									 NOP();NOP();NOP();
								}
					
							}
							SetBufferValue(SetParaBuffer,16,0);
						}
						
					break;

					case eSET_PARA_TCP:			// TCP端口;		
						
					//break;
					case eSET_PARA_UDP:			// UDP端口	
						if (IsInputFinish(SET_PARA_INPUT_NUM_AND_DOT) == TRUE)
						{
								//uLED.InputStatus 	= INPUT_INIT_STATUS;
								Tp.B32[1] = Str2Int((char *)SetParaBuffer);
								if (Tp.B32[1] > 65535)
								{
									InputAlertUser("端口号非法", 10);
									
								}
								else		
								{
									// 保存端口号
									if (uLED.SubMenuCnt==3) // TCP
									{
										 Uart.puts("TCP端口");

										 JT808_IP_DNS.TCP_Port = Tp.B32[1];						
				     					 DFE.Write(EE_JT_TCP_PORT, 4,(uchar *)&JT808_IP_DNS.TCP_Port);
										 NOP();NOP();NOP();
									}
									else						// UDP
									{
									 	Uart.puts("UDP端口");

										JT808_IP_DNS.UDP_Port = Tp.B32[1];
				      				DFE.Write(EE_JT_UDP_PORT, 4,(uchar *)&JT808_IP_DNS.UDP_Port);
										NOP();NOP();NOP();

									}

									SetBufferValue(SetParaBuffer,16,0);
									
								}
						}
							
						
					break;

					case eSET_PARA_PRIVINCE_ID:			// 省域ID
						if (IsInputFinish(SET_PARA_INPUT_NUM_AND_DOT) == TRUE)
						{
							  Uart.puts("省域");

							  Tp.B32[1] = Str2Int((char *)SetParaBuffer);
							  DFE.Write(EE_PROVINCE_REGION_ID, 2, (uchar *)&Tp.B32[1]);
							  NOP(); NOP(); NOP();

							  SetBufferValue(SetParaBuffer,16,0);
						}
						
					break;
					case eSET_PARA_PREFECTURE_ID:			// 市域ID
						if (IsInputFinish(SET_PARA_INPUT_NUM_AND_DOT) == TRUE)
						{
								Uart.puts("市域");

								Tp.B32[1] = Str2Int((char *)SetParaBuffer);
								DFE.Write(EE_PREFECTURE_REGION_ID, 2, (uchar *)&Tp.B32[1]);
							   NOP(); NOP(); NOP();

								SetBufferValue(SetParaBuffer,16,0);
						}
						
						
					break;
					case eSET_PARA_CAR_NUM:			// 车牌号码	
						if (IsInputFinish(SET_PARA_INPUT_NEED_FOR_CAR_NUM) == TRUE)
						{
							Uart.puts("车牌");
							SetBufferValue(Tp.Buffer,64, 0);
							memmove(Tp.Buffer,SetParaBuffer,9);
							DFE.Write(FM_VEHICLE_CODE, LEN_VEHICLE_CODE, Tp.Buffer);
							NOP(); NOP(); NOP();

							SetBufferValue(SetParaBuffer,16,0);
						}
						
						
					break;
					case eSET_PARA_CAR_COLOR:			// 车牌颜色		 蓝黄绿白黑
						if (IsInputFinish(SET_PARA_INPUT_NEED_FOR_COLOR) == TRUE)
						{
							Uart.puts("颜色:");
							Uart.puts((char *)SetParaBuffer);
							Uart.puts("\r\n");

							s  = 0;	 // 颜色代码
							if (strcmp((char *)"蓝", (char *)SetParaBuffer) == 0)
							{
								 	s = 1;
							}
							else if (strcmp((char *)"黄", (char *)SetParaBuffer) == 0)
							{
									s = 2;
							}
							else if (strcmp((char *)"绿", (char *)SetParaBuffer) == 0)
							{
									s = 3;
							}
							else if (strcmp((char *)"黑", (char *)SetParaBuffer) == 0)
							{
									s = 4;
							}
							else if (strcmp((char *)"白", (char *)SetParaBuffer) == 0)
							{
									s = 5;
							}


						   if (s)
							{
								DFE.Write(FM_CAR_COLOUR, 1, (uchar *)&s);
							}
							NOP(); NOP(); NOP();
							SetBufferValue(SetParaBuffer,16,0);
						}
						
						
					break;

					case	eSET_PARA_VIN_NUM:          // VIN 号
						if (IsInputFinish(SET_PARA_INPUT_NEED_FOR_CAR_NUM) == TRUE)
						{
							Uart.puts("VIN号");
							SetBufferValue(Tp.Buffer,64, 0);
							memmove(Tp.Buffer,SetParaBuffer,17);
							DFE.Write(FM_VEHICLE_VIN, LEN_VEHICLE_VIN, Tp.Buffer);
							NOP(); NOP(); NOP();

							SetBufferValue(SetParaBuffer,16,0);
						}

					break;


					case	eSET_PARA_CAR_TYPE: 				// 车辆类型
						uLED.MainMenuCnt = SET_PARA_SUB_MENU;
						uLED.SubSubMenuCnt = 1;
						
						
					break;

					case eSET_PARA_CAR_PHONE_NUM:			// 终端手机号码
						
					//break;
					case eSET_PARA_OWNER_PHONE_NUM:			// 车主手机号码
								 
					//break;
					case eSET_PARA_CENTER_NUM:			// 监控中心号码
						if (IsInputFinish(SET_PARA_INPUT_NUM_AND_DOT) == TRUE)
						{
								
							  	Uart.puts("手机号");

								if (strlen((char *)SetParaBuffer) < 5)
								{
										InputAlertUser("手机号非法", 10);
								}
								else
								{
									if (uLED.SubMenuCnt==eSET_PARA_CAR_PHONE_NUM) // 终端手机号
									{
										SetBufferValue(sTelNum.Sim, 16, 0);
										sTelNum.Sim[0] = 0x30;
									   memmove((uchar*)&sTelNum.Sim[1],SetParaBuffer,11);
										s = strlen((char *)&sTelNum.Sim[0]);
										DFE.Write(EE_SIM_NUMBER,16,(uchar*)&sTelNum.Sim[0]);
										NOP(); NOP(); NOP();

										//Uart.puts("phone:"); Uart.puts((char *)&sTelNum.Sim[1]);

										DecCharToPressBCD(s,(char *)sTelNum.Sim,SimNumberBCD);
										memmove(sTelNum.SimBCD_JT, SimNumberBCD, 6);	
									}
									else if (uLED.SubMenuCnt==eSET_PARA_OWNER_PHONE_NUM)	 // 车主手机号
									{
										s = strlen((char *)SetParaBuffer);
										if (s > LEN_NUMBER) 	s = LEN_NUMBER;

										DFE.Write(EE_JT_CAR_OWNER_PHONE_NUM, s, (uchar *)SetParaBuffer);
										NOP(); NOP(); NOP();		
										 
									}
									else						 // 监控中心号码
									{
										SetBufferValue(&JT808_Number.Center[0], LEN_NUMBER, 0);
										s = strlen((char *)SetParaBuffer);
					  					memmove((char *)&JT808_Number.Center[0], (char *)SetParaBuffer, s);
					  					DFE.Write(EE_JT_CENTER_NUMBER, LEN_NUMBER, (uchar *)&JT808_Number.Center[0]);
										NOP(); NOP(); NOP();

										
									}

								  	
								}
								SetBufferValue(SetParaBuffer,16,0);
						}
						
					break;

					case eSET_PARA_K_VALUE:			// K值
						if (IsInputFinish(SET_PARA_INPUT_NUM_AND_DOT) == TRUE)
						{
							VehiclePulseKV = Str2Int((char *)SetParaBuffer);
							DFE.Write( FM_VEHICLE_PULSE_KV,4,(uchar*)&VehiclePulseKV);
							NOP(); NOP(); NOP();

							SetBufferValue(SetParaBuffer,16,0);
						}

						
					break;

					case eSET_PARA_UPLOAD_INTERVAL_TIME:			// 上报时间间隔
						if (IsInputFinish(SET_PARA_INPUT_NUM_AND_DOT) == TRUE)
						{
							Tp.B32[1] = Str2Int((char *)SetParaBuffer);
							DFE.Write(EE_JT_SEND_TIME_DEFAULET, 4,(uchar *)&Tp.B32[1]);
							NOP(); NOP(); NOP();
							GSM.SetDefaultSendInterval(Tp.B32[1]);
						}

					break;

					case eSET_PARA_CREATE_USER:			//	 开户
						//Uart.puts("开户");

						//memmove((uchar*)&OLED_Buffer[1][0],(uchar*)"开户成功",8);
						InputAlertUser("开户成功", 8);

						CreateUserFlag = OK;
						DFE.Write(EE_CREATE_USER_FLAG, 1, &CreateUserFlag);
						NOP(); NOP(); NOP();
						
					break;
					
									
					case eSET_PARA_CANCER_REG:			// 注销本机
						
						GPRS.UnRegTerminal();
						
					break;
					
					case eSET_PARA_FACTORY_SET:			// 恢复出厂值
						
						Sys.FirstPowerOn(FALSE); 
						NVIC_SystemReset();  
						
					break;
					
					
					case eSET_PARA_REMOTE_UPDATE:			// 远程升级
						
						MenuSetRemoteIAP = TRUE;
						uLED.MainMenuCnt = UPDATE_INDICATE;
						
					break;			
					
					case eSET_PARA_MOD_PWD:			// 修改密码				
						if (uLED.InputModifyPwdFlag == 0)
						{
							InputAlertUser("请输入新密码", 12);
						}
						else if  (uLED.InputModifyPwdFlag == 1)
						{
							if (IsInputFinish(SET_PARA_INPUT_NUM_AND_DOT) == TRUE)
							{
								 SetBufferValue(InputPassword, 10, 0);
								 memmove((char *)InputPassword, (char *)SetParaBuffer, 10);


								 SetBufferValue(SetParaBuffer, 16,0);
								 InputAlertUser("再次输入新密码", 14);
								 uLED.InputModifyPwdFlag = 2;
								 
							}
						}
						else if (uLED.InputModifyPwdFlag == 3)
						{
							 if (IsInputFinish(SET_PARA_INPUT_NUM_AND_DOT) == TRUE)
							 {
								 if (memcmp(InputPassword, SetParaBuffer, 10) == 0)
								 {
								   Uart.puts("密码修改成功");

									InputAlertUser("密码修改成功", 12);
									Tp.B32[0] = Str2Int((char *)InputPassword);

									//Uart.puts("pwd1:");
									//Uart.puts((char *)InputPassword);

									//Uart.puts("modify pwd: ");Uart.putb(1, 4, (uchar *)&Tp.B32[0]);

									DFE.Write(EE_SET_PARA_PASSWORD,4,(uchar *)&Tp.B32[0]);
									NOP(); NOP(); NOP(); 

								 }
								 else
								 {
								 	  InputAlertUser("两次密码不同", 12);
								 }

							 }
							 uLED.InputModifyPwdFlag = 0;
							
						}			
					
						
					break;
//					case 16:			// 新密码
//						
//					break;
//					case 17:			// 再次输入新密码	
//						
//					break;
                    case eSET_gghypt:		   //addBySun
				    DFE.Write(EE_JT_M_IP_DNS,16,(uchar*)"jt1.gghypt.net\0\0");	
					DFE.Write(EE_JT_B_IP_DNS,16,(uchar*)"jt2.gghypt.net\0\0");			
					ID = 7008;
					DFE.Write(EE_JT_TCP_PORT,4,(uchar *)&ID);
					DFE.Write(EE_JT_UDP_PORT,4,(uchar *)&ID);
					GPRS.UnRegTerminal();
				   break;
					
					default:	Tp.B32[0] = 30;		break;
				 }
				 
			 }
			 else if(uLED.MainMenuCnt == DEFAULT_DISP)
			 {
			 	//uLED.MainMenuCnt	= CONFIRM_MENU;
				if (ConfirmCnt++ > 3)
				{
					ConfirmCnt = 0;
					BEEP(9,0,2,0,5);
					if((uiSpeed<30)&&(PrintEvent1==0)&&(USBTaskCreat==0))
					{					
							uLED.MainMenuCnt = PRINT_MENU;
							Print.Init(TRUE);		
							PrintEvent1 = 1;
							Print.TickCnt = 0;
							KEY_DL.LongTimeNoKey = 0; 
							//UserEvtFlag.U0print = UART0_PRINT_EVT_FLAG;
							//os_evt_set(UART0_PRINT_EVT_FLAG,TaskID.U0print);   //触发打印事件
					}
				}
				KEY_DL.KeyPressTime = 20;
				return;

		    }	
			 else if(uLED.MainMenuCnt == CONFIRM_MENU)
			 {
			 	uLED.MainMenuCnt	= CONFIRM_MENU;
		    }	
			 else if (uLED.MainMenuCnt == INPUT_MENU)
			 {
			 	  if (uLED.InputStatus == INPUT_ONE_CHAR)
				  {
				  	
					  uLED.InputStatus = INPUT_INIT_STATUS;

					  if (uLED.SetParaFlag == SET_PARA_INPUT_NUM_AND_DOT)
					  {	
					  		s = InputSelectNumDot[uLED.InputPageCnt];	

							SetParaBuffer[uLED.Offset] =  s;
												  //strncpy((char *)&OLED_Buffer[1][uLED.Offset],(const char *)&SetParaBuffer[uLED.Offset], 1);
					  		OLED_Buffer[1][uLED.Offset] = SetParaBuffer[uLED.Offset];

					  		uLED.Offset++;
					  }
					  else if (uLED.SetParaFlag == SET_PARA_INPUT_NEED_FOR_COLOR)	
					  {
					  		  	for (s = 0; s < 2; s++)				 // 汉字是两个字节
								{
							  		SetParaBuffer[uLED.Offset + s] = InputSelectColor[uLED.InputPageCnt + s];
									OLED_Buffer[1][uLED.Offset + s] = SetParaBuffer[uLED.Offset + s];
								}

								uLED.Offset += 0;
								

					  }
					  else if (uLED.SetParaFlag == SET_PARA_INPUT_NEED_FOR_CAR_NUM)
					  {
					  		if (uLED.InputPageCnt < 36)
							{
								SetParaBuffer[uLED.Offset] =  InputSelectCarNum[uLED.InputPageCnt];;
												  //strncpy((char *)&OLED_Buffer[1][uLED.Offset],(const char *)&SetParaBuffer[uLED.Offset], 1);
					  			OLED_Buffer[1][uLED.Offset] = SetParaBuffer[uLED.Offset];

					  			uLED.Offset++;
							}	
							else
							{
								for (s = 0; s < 2; s++)				 // 汉字是两个字节
								{
							  		SetParaBuffer[uLED.Offset + s] = InputSelectCarNum[uLED.InputPageCnt + s];
									OLED_Buffer[1][uLED.Offset + s] = SetParaBuffer[uLED.Offset + s];
								}

								uLED.Offset += 2;
								
							}
					  }
					 
					 
//					  SetParaBuffer[uLED.Offset] =  s;
//
//					  //strncpy((char *)&OLED_Buffer[1][uLED.Offset],(const char *)&SetParaBuffer[uLED.Offset], 1);
//					  OLED_Buffer[1][uLED.Offset] = SetParaBuffer[uLED.Offset];
//
//					  uLED.Offset++;

				  }
				  else
				  {
				  		uLED.InputStatus = INPUT_FINISH_ALL_CHAR;
						uLED.MainMenuCnt = SET_PARA_MENU;
						goto 	DISPOSE_INPUT_ALL_CHAR; // 处理完成的字符

				  }
			 }
			 else if (uLED.MainMenuCnt==SET_PARA_SUB_MENU)
			 {
			 	  if (uLED.SubMenuCnt==eSET_PARA_CAR_TYPE)
				  {
				  		SetBufferValue(Tp.Buffer, 16,0);
						strcpy((char *)Tp.Buffer, (char *)&VehicleTypeSCR[uLED.SubSubMenuCnt-1][2]);	 // 仅复制 "1.xxxx" 中的xxxx
						
						DFE.Write(FM_VEHICLE_CLASS, 12,Tp.Buffer);
						InputAlertUser("    设置成功", 12);
				  }
			 }
			 /*else if(uLED.MainMenuCnt==REG_INFO_MENU)			// 注册信息		测试时使用的
			 {
			 	 // uLED.PageCnt CurrentRegInfo
				 if(CurrentRegInfo<26)
				 {
				 	 DFE.Write(EE_PROVINCE_REGION_ID,46, (uchar *)&RegInfo25Device[CurrentRegInfo][0]);		// 注册信息
					 CreateUserFlag = OK;		  // 开户
					 DFE.Write(EE_CREATE_USER_FLAG, 1, &CreateUserFlag);
					 NeedRegServer = 0;	
					 DFE.Write(EE_NEED_REG_SERVER, 1, &NeedRegServer);
					 SetBufferValue(  (uchar*)jt_auth_code, LEN_AUTH_CODE,0);
					 if(CurrentRegInfo==25)
					 	strcpy( (char*)jt_auth_code,"13886088888\0");
					 else
					 	memmove( jt_auth_code,(uchar *)&RegInfo25Device[CurrentRegInfo][46],10);	  		// 鉴权码
					 DFE.Write(EE_AUTH_CODE, LEN_AUTH_CODE,(uchar *)&jt_auth_code[0]);
					 SetBufferValue(sTelNum.Sim, 16, 0);
					 if(CurrentRegInfo==25)
					 	strcpy ( (char*)&sTelNum.Sim[0],"013886088888\0");
					 else
					 	memmove((uchar*)&sTelNum.Sim[0],(uchar *)&RegInfo25Device[CurrentRegInfo][56],12); 		// 手机号
					 DFE.Write(EE_SIM_NUMBER,16,(uchar*)&sTelNum.Sim[0]);
					 Tp.Buffer[0] = strlen((char *)&sTelNum.Sim[0]);
					 DecCharToPressBCD(Tp.Buffer[0],(char *)sTelNum.Sim,SimNumberBCD);
					 memmove(sTelNum.SimBCD_JT, SimNumberBCD, 6);

					 ///////////////////////////////////////////////////////////////
					 SetBufferValue(Tp.Buffer,32,0);
					 strcpy( (char*)Tp.Buffer,"L0000");
					 strcat( (char*)Tp.Buffer,(char*)sTelNum.Sim);
					 DFE.Write(FM_VEHICLE_VIN,LEN_VEHICLE_VIN,Tp.Buffer);														// 车辆VIN号
					 DFE.Write(FM_VEHICLE_CLASS,12,(uchar*)"大型重装货车");

					 SetBufferValue(Tp.Buffer,32,0);
					 Tp.Buffer[0] = 0x13;		Tp.Buffer[1] = 0x05;		Tp.Buffer[2] = 0x09;
					 Tp.Buffer[3] = 0x10;		Tp.Buffer[4] = 0x20;		Tp.Buffer[5] = 0;
					 DFE.Write(FM_INSTALL_DATE,6,Tp.Buffer);											  			// 安装日期

					 DFE.Write(FM_INIT_MILAGE_BCD,4,(uchar*)&Tp.Buffer[16]);								 	// 初始里程

					 VehiclePulseKV = 3600;
					 DFE.Write( FM_VEHICLE_PULSE_KV,4,(uchar*)&VehiclePulseKV);					  			// K值

					 SetBufferValue(Tp.Buffer,32,0);							  
					 strcpy( (char*)Tp.Buffer,"0000000");
					 strcat( (char*)Tp.Buffer,(char *)&RegInfo25Device[CurrentRegInfo][9]);
					 Tp.Buffer[23] = 0x13;		Tp.Buffer[24] = 0x04;		Tp.Buffer[25] = 0x25;
					 Tp.B32[15] = Str2Int( (char*)&RegInfo25Device[CurrentRegInfo][69] );
					 Tp.Buffer[26] = Tp.Buffer[63]; 	Tp.Buffer[27] = Tp.Buffer[62];
					 Tp.Buffer[28] = Tp.Buffer[61]; 	Tp.Buffer[29] = Tp.Buffer[60];
					 DFE.Write( FM_SERIAL_NUMBER, LEN_SERIAL_NUMBER, Tp.Buffer);					  		// 唯一序列号

					 SystemLog.VDR.SpeedSensorPulsCnt = 3463*VehiclePulseKV;
					 SystemLog.VDR.SpeedSensorPulsCnt += gCnt.TickForRandData&0x0000007f;
					 DFE.Write( EE_SYS_VDR_SPEED_PULS_CNT,4,(uchar*)&SystemLog.VDR.SpeedSensorPulsCnt);

					 Uart.puts(" 操作主机界面，修改注册信息 ");
				 }				 
			 }	 */
		}		
		KEY_DL.KeyPressTime = 20;
		BEEP(9,0,2,0,5);
		//if(DispCnt%20==0)
	 //Uart.putb(1,4,(uchar*)&KEY_DL.U32.KEY32);
		
   }
}




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：KeyPageDown
   功能描述：下翻按键
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void      KeyDispCls::KeyPageDown(void)			// Key1  疲劳驾驶按键，上翻屏按键
{
	//uchar	s = 0;
   //LED  ULED;
   NOP();   NOP();
   //if(1)
   {
   	if(KEY_DL.KeyPressTime==0)
   	{

			if(uLED.MainMenuCnt == MAIN_MENU)		  	// 
			{
				if(uLED.SubMenuCnt<MAX_SUM_MENU)
					uLED.SubMenuCnt++;
				else
					uLED.SubMenuCnt = 1;
			}		
			else if(uLED.MainMenuCnt == Milage_MENU)	
			{
				if(uLED.PageCnt>3)
				 uLED.PageCnt = 0;
				else 
				 uLED.PageCnt++;
			}					
			else	if( (uLED.MainMenuCnt == Schedul_MSG_MENU))											  	// 下翻屏按键
   		{				
				if(uLED.PageCnt>=2)
				 uLED.PageCnt = 0;
				else 
				 uLED.PageCnt++;
   		}
   		else if(uLED.MainMenuCnt==SET_PARA_MENU)
   		{
   			if(uLED.PasswordOK==FALSE)
   			{
   				uLED.PageCnt = 0;

					uLED.InputStatus = INPUT_INIT_STATUS;
					uLED.MainMenuCnt = INPUT_MENU;
					uLED.Offset = 0;
					uLED.InputPageCnt = 0;
					SetBufferValue(&OLED_Buffer[1][0], 16, 0);
   			}
				else
				{
					if(uLED.SubMenuCnt<MAX_SET_PARA_MENU)
						uLED.SubMenuCnt++;
					else
						uLED.SubMenuCnt = 1;
				}
	

				/*if ((uLED.InputStatus == INPUT_FINISH_ALL_CHAR) && (uLED.PasswordOK == FALSE))
				{
					SetBufferValue(&OLED_Buffer[1][0], 16, 0);
					uLED.Offset = 0;
					uLED.InputStatus = INPUT_INIT_STATUS;
					uLED.InputPageCnt = 0;
					
				}
				else
				{

					uLED.InputStatus = INPUT_ONE_CHAR;
	
					if (uLED.InputPageCnt<16)
					{
						uLED.InputPageCnt++;
					}
					else
					{
						uLED.InputPageCnt = 0;
					}
				}*/	
   			
   		}
			else if (uLED.MainMenuCnt==INPUT_MENU)
			{
				if (uLED.SubMenuCnt==eSET_PARA_CREATE_USER) // 开户菜单
				{
					return;
				}

				if (uLED.InputAlert == TRUE)				// 如果之前有提示像“IP地址非法”，再按该功能键， 就表示要重新输入字符
				{
					
					if ((uLED.SubMenuCnt==eSET_PARA_CREATE_USER)	||	//  开户
					   (uLED.SubMenuCnt==eSET_PARA_CAR_TYPE))	
					{
						return;
					}

					if (uLED.SubMenuCnt==eSET_PARA_MOD_PWD)	  //  修改密码项
					{
						if (uLED.InputModifyPwdFlag == 0)
						{	
							uLED.InputModifyPwdFlag = 1;
							return;
						}

						if (uLED.InputModifyPwdFlag == 2)
						{
							uLED.InputModifyPwdFlag = 3;
							return;
						}
						
					}

					SetBufferValue(&OLED_Buffer[1][0], 16, 0);
					uLED.Offset = 0;
					uLED.InputStatus = INPUT_INIT_STATUS;
					uLED.InputPageCnt = 0;
					uLED.InputAlert = FALSE;

				
				}

			

				if ((uLED.InputStatus == INPUT_FINISH_ALL_CHAR))// && (uLED.PasswordOK == FALSE))
				{
					SetBufferValue(&OLED_Buffer[1][0], 16, 0);
					uLED.Offset = 0;
					uLED.InputStatus = INPUT_INIT_STATUS;
					uLED.InputPageCnt = 0;
					
				}
				else
				{

					uLED.InputStatus = INPUT_ONE_CHAR;
	
					if (uLED.SetParaFlag == SET_PARA_INPUT_NUM_AND_DOT)
					{
							if (uLED.InputPageCnt < LEN_NUM_DOT - 1)
								uLED.InputPageCnt++;
							else
								uLED.InputPageCnt = 0;
						
					}
					else if (uLED.SetParaFlag == SET_PARA_INPUT_NEED_FOR_COLOR)	 
					{
						if (uLED.InputPageCnt < LEN_SELECT_COLOR - 3)
							uLED.InputPageCnt += 2;
						else
							uLED.InputPageCnt = 0;

					}
					else if (uLED.SetParaFlag == SET_PARA_INPUT_NEED_FOR_CAR_NUM)
					{
						if (uLED.InputPageCnt < 36)  // 0 - 9 ， A - Z 36	 个单字符
						{
							uLED.InputPageCnt++;
							
						}
						else 			  // 汉字
						{
							if (uLED.InputPageCnt < LEN_SELECT_CAR_NUM - 3)
								uLED.InputPageCnt += 2;
							else
								uLED.InputPageCnt = 0;
							
							
						}

						//Uart.puts("uLED.InputPageCnt: "); 	Uart.putb(1, 1, &uLED.InputPageCnt);

						
					}

					



					/*if (uLED.InputPageCnt < s-1)
					{
						uLED.InputPageCnt++;
					}
					else
					{
						uLED.InputPageCnt = 0;
					}*/

					
				}	
			}
			else if (uLED.MainMenuCnt==SET_PARA_SUB_MENU)
			{
				if (uLED.SubMenuCnt==eSET_PARA_CAR_TYPE)
				{
					if (uLED.SubSubMenuCnt<MAX_VEHICLE_TYPE_MENU)
						uLED.SubSubMenuCnt++;
					else 
						uLED.SubSubMenuCnt = 1;
					
				}
			}
			else 
			{
				uLED.PageCnt++;
			}
		}
		KEY_DL.KeyPressTime = 20;
		BEEP(9,0,2,0,5);
		//if(DispCnt%20==0)
	 //Uart.putb(1,4,(uchar*)&KEY_DL.U32.KEY32);		
   }
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：KeyPrint
   功能描述： 打印按键
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void      KeyDispCls::KeyPrint(void)			 // Key2   平均速度显示键，下翻屏按键
{

   //LED  ULED;
   NOP();   NOP();
   //if(1)
   {
   	if(KEY_DL.KeyPressTime==0)
   	{			
			if(uLED.MainMenuCnt == PRINT_Comfir_MENU) 
			{ 
				uLED.MainMenuCnt = DEFAULT_DISP;
		   }
			else if(uLED.MainMenuCnt == PRINT_NO_PAPER_MENU) 
   		{
				uLED.MainMenuCnt = DEFAULT_DISP;
      	}
			else if(uLED.MainMenuCnt == DEFAULT_DISP) 
   		{
				uLED.MainMenuCnt = PRINT_Comfir_MENU;
      	}
			else if(uLED.MainMenuCnt == PRINT_MENU)
			{
			  uLED.MainMenuCnt = PRINT_MENU;
			}
			else if(uLED.MainMenuCnt == CONFIRM_MENU)
			{
			  uLED.MainMenuCnt = DEFAULT_DISP;
			}
			else if(uLED.MainMenuCnt==MAIN_MENU)
			{
				uLED.MainMenuCnt = DEFAULT_DISP;
			}
			else if (uLED.MainMenuCnt==SET_PARA_MENU)
			{
//				if(uLED.Offset<uLED.MaxOffset)
//					uLED.Offset++;
//				else
//					uLED.Offset = 0;
					uLED.Offset = 0;
					uLED.InputStatus = INPUT_INIT_STATUS;
					uLED.InputPageCnt = 0;
					uLED.PasswordOK = FALSE;
					uLED.InputAlert = FALSE;
					uLED.MainMenuCnt = MAIN_MENU;
					uLED.SubMenuCnt = 11;		  // 	 指向 “参数设置”

				
			}
			else if (uLED.MainMenuCnt==INPUT_MENU)
			{
				if (uLED.SubMenuCnt==0)
				{
					uLED.Offset = 0;
					uLED.InputStatus = INPUT_INIT_STATUS;
					uLED.InputPageCnt = 0;
					uLED.PasswordOK = FALSE;
					uLED.InputAlert = FALSE;
					uLED.MainMenuCnt = MAIN_MENU;
					uLED.SubMenuCnt = 11;		  // 	 指向 “参数设置”
					
				}
				else
				{
					uLED.Offset = 0;
					uLED.InputStatus = INPUT_INIT_STATUS;
					uLED.InputPageCnt = 0;
					//uLED.PasswordOK = FALSE;
					uLED.InputAlert = FALSE;
					uLED.MainMenuCnt = SET_PARA_MENU;
					uLED.InputModifyPwdFlag = 0;
				}


			}
			else
			{
				uLED.MainMenuCnt = MAIN_MENU;
				//uLED.SubMenuCnt = 1;
			}
			
      }
      KEY_DL.KeyPressTime = 20;
		BEEP(9,0,2,0,5);
		
   }
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：KeyPageUp
   功能描述： 上翻按键
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ 

// void			eTaskPrint(void);
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void       KeyDispCls::KeyPageUp(void)				    //
{
   //uchar	s  = 0;
   //LED  ULED;
   NOP();   NOP();
   if(1)
   {
   	if(KEY_DL.KeyPressTime==0)
   	{   		   		 
			if(uLED.MainMenuCnt == MAIN_MENU) 	// 从第2页主菜单 显示
			{
				if(uLED.SubMenuCnt>1)
					uLED.SubMenuCnt--;				
				else
					uLED.SubMenuCnt = MAX_SUM_MENU;
			}	
					
			else if(uLED.MainMenuCnt == Milage_MENU)
			{	
				if(uLED.PageCnt==0)
				 uLED.PageCnt = 3;
				else 
				 uLED.PageCnt--;
   		}						
			else	if(uLED.MainMenuCnt == Schedul_MSG_MENU)											  	// 上翻屏按键
   		{		     			
				
				if(uLED.PageCnt==0)
				 uLED.PageCnt = 2;
				else 
				 uLED.PageCnt--;
   		}
   		else if(uLED.MainMenuCnt==SET_PARA_MENU)
   		{
   			/*if(uLED.PasswordOK==TRUE)
   			{
   				
   			}
   			else
   			{
   				uLED.PageCnt = 0;
   			}*/


	
   			if(uLED.PasswordOK==FALSE)
   			{
   				uLED.PageCnt = 0;

   			}
				else
				{
					if(uLED.SubMenuCnt>1)
						uLED.SubMenuCnt--;				
					else
						uLED.SubMenuCnt = MAX_SET_PARA_MENU-1;
				}

				/*if ((uLED.InputStatus == INPUT_FINISH_ALL_CHAR) && (uLED.PasswordOK == FALSE))
				{
					SetBufferValue(&OLED_Buffer[1][0], 16, 0);
					uLED.Offset = 0;
					uLED.InputStatus = INPUT_INIT_STATUS;
					uLED.InputPageCnt = 0;
					
				}
				else
				{

					uLED.InputStatus = INPUT_ONE_CHAR;
	
					if (uLED.InputPageCnt>0)
					{
						uLED.InputPageCnt--;
					}
					else
					{
						uLED.InputPageCnt = 15;
					}
					
   			
   			}*/
   			
   		}
			else if (uLED.MainMenuCnt == INPUT_MENU)
			{

				if ((uLED.SubMenuCnt==eSET_PARA_CREATE_USER)	||	//  开户
					   (uLED.SubMenuCnt==eSET_PARA_CAR_TYPE))	
				{
					return;
				}

				if (uLED.InputAlert == TRUE)				// 如果之前有提示像“IP地址非法”，再按该功能键， 就表示要重新输入字符
				{
					SetBufferValue(&OLED_Buffer[1][0], 16, 0);
					uLED.Offset = 0;
					uLED.InputStatus = INPUT_INIT_STATUS;
					uLED.InputPageCnt = 0;
					uLED.InputAlert = FALSE;

					if (uLED.SubMenuCnt==eSET_PARA_MOD_PWD)	  //  修改密码项
					{
						if (uLED.InputModifyPwdFlag == 0)
						{	
							uLED.InputModifyPwdFlag = 1;		// 此时可以第一次输入密码
							return;
						}

						if (uLED.InputModifyPwdFlag == 2)
						{
							uLED.InputModifyPwdFlag = 3;		 // 此时可以再次输入密码
							return;
						}
						
					}
				}

				if ((uLED.InputStatus == INPUT_FINISH_ALL_CHAR) && (uLED.PasswordOK == FALSE))
				{
					SetBufferValue(&OLED_Buffer[1][0], 16, 0);
					uLED.Offset = 0;
					uLED.InputStatus = INPUT_INIT_STATUS;
					uLED.InputPageCnt = 0;
					
				}
				else
				{

					uLED.InputStatus = INPUT_ONE_CHAR;

					if (uLED.SetParaFlag == SET_PARA_INPUT_NUM_AND_DOT)
					{
							if (uLED.InputPageCnt>0)
								uLED.InputPageCnt--;
							else
								uLED.InputPageCnt = LEN_NUM_DOT - 1;

					}
					else if (uLED.SetParaFlag == SET_PARA_INPUT_NEED_FOR_COLOR)	 
					{

							if (uLED.InputPageCnt>1)
								uLED.InputPageCnt -= 2;
							else
								uLED.InputPageCnt = LEN_SELECT_COLOR - 2;

							
					}
					else if (uLED.SetParaFlag == SET_PARA_INPUT_NEED_FOR_CAR_NUM)
					{
						if (uLED.InputPageCnt >= 38)  // 0 - 9 ， A - Z    36个单字符
						{
							 uLED.InputPageCnt -= 2;
						}
						else 
						{
							if (uLED.InputPageCnt>0)
							{
								uLED.InputPageCnt--;
							}
							else
							{
								uLED.InputPageCnt = LEN_SELECT_CAR_NUM - 2;
							}
						}

						Uart.puts("InputPageCnt: "); Uart.putb(1, 1, &uLED.InputPageCnt);
						
					}
													 
					
   			
   			}
			} 
			else if (uLED.MainMenuCnt==SET_PARA_SUB_MENU)
			{
				if (uLED.SubMenuCnt==eSET_PARA_CAR_TYPE)
				{
					if (uLED.SubSubMenuCnt>1)
						uLED.SubSubMenuCnt--;
					else 
						uLED.SubSubMenuCnt = MAX_VEHICLE_TYPE_MENU;
					
				}
			}
			else if (uLED.MainMenuCnt==DEFAULT_DISP)
			{
				uLED.MainMenuCnt	= CONFIRM_MENU;
			}
			else   	
			{ 				
				if(uLED.PageCnt!=0)
				  uLED.PageCnt--;
			}
			
      }
      KEY_DL.KeyPressTime = 20;
		BEEP(9,0,2,0,5);
		//if(DispCnt%20==0)
	 //Uart.putb(1,4,(uchar*)&KEY_DL.U32.KEY32);		
   }
}





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：TurnOffLed
   功能描述：关闭LED
   入口参数：LED序号（必须和打开LED的序号一致）
   出口参数：无
   全局变量: 无
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			KeyDispCls::TurnOffLed(uchar i)
{
	switch(i)
	{
		case 0:	GPS_LED_OFF;		break;
		case 1:	GPRS_LED_OFF;		break;				
		default:	break;
	}
}

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：TurnOnLed
   功能描述：打开LED
   入口参数：LED序号（必须和关闭LED的序号一致）
   出口参数：无
   全局变量: 无
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			KeyDispCls::TurnOnLed(uchar i)
{
	switch(i)
	{
		case 0:	GPS_LED_ON;		break;
		case 1:	GPRS_LED_ON;	break;		
		default:	break;
	}
}	// End of <TurnOnLed> function






/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：SetLED_Status
   功能描述：设置指示灯状态
   入口参数：序号，状态
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			KeyDispCls::SetLED_Status(uchar no,uchar Data)
{
	if( (Data<20)&&(no<LED_TOTAL_NUMBER))
		Lamp.Status[no] = Data; 
}




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：LoadDispInitData
   功能描述：加载显示初始信息
   入口参数：是否显示初始化信息，0: 显示  非0：不显示
   出口参数：无
   全局变量: 本文件静态变量 InitDispCNT
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			KeyDispCls::Init(void)
{
	NOP();	NOP();
	DispCnt=0;
	LcmDataToServer = 0;
	NoDriverCnt = 0;
	Lamp.Status[0] = Lamp.Status[1] = LED_1T_PER_1SEC;

	uLED.InputStatus = INPUT_INIT_STATUS;
	uLED.InputModifyPwdFlag = 0;
}



KeyDispCls::KeyDispCls(void)
{
	
}



 /* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   函数名: TaskLED_Buzzer
   功能：  指示灯和蜂鸣器
   入口：  无
   出口：  无
*/
#if(USE_LED)
unsigned short 	const		LED_OffTimeTab[] =  { 1000,400,350,200,100,60,40,32,20,12,8,4,2,0,0};		  
unsigned short 	const		LED_OnTimeTab[] =   {    0,100, 80, 60, 40,30,25,20,12, 8,4,2,1,0,0}; 
#endif
void       KeyDispCls::LED_Buzzer(void)    //20毫秒扫描一次		
 {
 	#if(USE_LED)
	uint16		OnTime,OffTime; 
	#endif
	//uchar i;
	twinkle++; 
	NOP();   NOP();            // DataSheet page:180				
	/////////////////////////////////////////////////////////////////////	
	//return;	
#if(USE_LED)
   	
   if(IDEL_For_Acc_OFF==TRUE)
	{
	   ALL_LED_OFF;
	}
	else
	{
		for(i = 0; i<2; i++)
		{
			OnTime  = LED_OnTimeTab[Lamp.Status[i]];
			OffTime = LED_OffTimeTab[Lamp.Status[i]];
			if(Lamp.Status[i]>=LED_TURN_ON)
			{
				if(i==LED_GPRS)
				{
				  GPRS_LED_ON;
				}
				else
				{
				  GPS_LED_ON;
				}
			}
			else if((Lamp.Status[i]!=0)&&((twinkle%(OffTime+OnTime))<=OnTime))
			{
			  	if(i==LED_GPRS)
			   {
				   GPRS_LED_ON;
				}
			   else
			   {
				  GPS_LED_ON;
				}
			}
			else 
			{
			   if(i==LED_GPRS)
				{
				  GPRS_LED_OFF;
				}
				else
				{
				  GPS_LED_OFF;
				}
			}
		}
	}
#endif 
		
	//////////////////////////////////////// 蜂鸣器鸣叫程序
	if(twinkle%(2*Beep.BeepRaze)==0)							// Beep.type=Type;   Beep.times=Times; Beep.counter=Counter;  Beep.silent=Silent; Beep.BeepRaze=Raze;
	{
		if(Beep.type==SILENT)
		{
	       BUZZER_OFF;  	   
		}
		else if(Beep.type==PULL_CARD_BEEP)
		{
		   Beep.counter++;
	       if(Beep.counter>Beep.times)
		    {
		      Beep.counter=0;
		      if(BuzzerStatus)
		      {
		      	BuzzerStatus = 0;
		      	BUZZER_OFF;
		      }
		      else
		      {
		      	BuzzerStatus = 1;
		      	BUZZER_ON;
		      }	      
			  if(Beep.times>1)
			     Beep.times--;
			  else
			  {
			     Beep.type=SHORT_BEEP;
				 Beep.times=0;
				 Beep.counter=4;
				 Beep.silent=0;
			  }
		   }
		}
	    else 
		{
		   if(Beep.times<Beep.counter)
	       {
		      if(BuzzerStatus)
		      {
		      	BuzzerStatus = 0;
		      	BUZZER_OFF;
		      }
		      else
		      {
		      	BuzzerStatus = 1;
		      	BUZZER_ON;
		      }
		      Beep.times++;
		   }
		   else
		   {
		     //BUZZER_ON; 
			  BUZZER_OFF; 
			  Beep.times=255;   // 以保证 Beep.times>Beep.counter,因为Beep.counter++
			  Beep.counter++;
	    	  if(Beep.counter>Beep.silent)
			  {
			     Beep.type=SILENT;
				  BUZZER_OFF;  
			  }
		   }
		}
	 }
 }





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uchar       KeyDispCls::GetOVT_Addr(uchar HowLong, uint32 *PAddr)
{
   /*NOP();	 NOP();
	uchar   BfData[8][28],i,n, tBuffer[17];

	uchar   OverDriveTimes;
	uint32  Ntime,TowDateTime,Rtime,DFF_Page;

	Ntime = Rtc.GetRtcSecond();
	TowDateTime = Ntime - 24*3600 - Rtc.DT.Hour*3600 - Rtc.DT.Min*60 - Rtc.DT.Sec;			//日历天
	OverDriveTimes = 0;
	DFE.Read(FM_TIRE_DRIVE,224,(uchar *)BfData);				   //先搜索FRAM
	if(1)
	{
	for(i=8;i>0;i--)
	{
	  SetBufferValue(tBuffer, 17, 0);
	  memmove(tBuffer, &BfData[i-1][18], 5);
	  Rtime = Rtc.BDC_DateTimeToSecond((RTC_STR *)tBuffer);
	  if((TowDateTime<=Rtime)&&(Rtime<=Ntime))
	  {
	    *PAddr = FM_TIRE_DRIVE+(i-1)*28;
		 PAddr++; 
		 OverDriveTimes++;  
	  }
	}
	}

	DFE.Read(FM_TIRE_DRIVE_FF_Addr,4,(uchar *)&DFF_Page);				 //再搜索Flash
   if(DFF_Page!=0)					       //DFF_Page!=0
	{
	   for(n=0;n<3;n++)
		{
		    if(DFF_Page==0)
			   n = 3;
			 else
		    {
		       DFF_Page--;
		       DFF.AnyNumberRead((FF_TIRE_DRIVE_START_PAGE+DFF_Page%FF_TIRE_DRIVE_TOTAL_PAGE)*256,224,(uchar *)BfData);   //  DFF.PageRead(FF_TirDrv_Addr+DFF_Page%FF_TirDrv_Len,(uchar *)BfData);
				 for(i=8;i>0;i--)
				 {
					  SetBufferValue(tBuffer, 17, 0);
	              memmove(tBuffer, &BfData[i-1][18], 5);
					  Rtime = Rtc.BDC_DateTimeToSecond((RTC_STR *)tBuffer);
					  if((TowDateTime<=Rtime)&&(Rtime<=Ntime))
					  {
					    *PAddr = (FF_TIRE_DRIVE_START_PAGE+DFF_Page%FF_TIRE_DRIVE_TOTAL_PAGE)*256+(i-1)*28;
						 PAddr++; 
						 if(++OverDriveTimes>=20)
						 {
						    i = 0;
							 n = 3;
		             } 
						  
					  }
				 }
		     }
	    }
	}	*/
		
		uchar   OverDriveTimes;
		uint32  Ntime,TowDateTime,Rtime,Addr;	
		uint32 tl,i,ReadPage;
		pSAVE_FATIGUE_DRIVING_INFO		pSaveFatigue;
	   uchar tBuffer[256];

		Ntime = Rtc.GetRtcSecond();
		TowDateTime = Ntime - 24*3600 - Rtc.DT.Hour*3600 - Rtc.DT.Min*60 - Rtc.DT.Sec;			//日历天
		OverDriveTimes = 0;

		 Rtime = Rtc.BDC_DateTimeToSecond((RTC_STR *)tBuffer);

		DFE.Read(FM_FATIGUE_DRIVING_CNT,4,(uchar*)&tl);
		{	// 取铁电里的数据
			if (tl<5)  // 取铁电里所有疲劳驾驶数据计数
			{ 
				tl = tl;    	
			}
			else		// 取最新存在铁电里部分数据,该数据还没存到flash计数
			{	
				tl = tl % 5;	
			}

			DFE.Read(FM_SAVE_FATIGUE_DRIVING, LEN_SAVE_FATIGUE_DRIVING_INFO * tl,tBuffer);
			for (i=tl; i>0;i--)
			{
				pSaveFatigue = (pSAVE_FATIGUE_DRIVING_INFO)&tBuffer[(i-1) * LEN_SAVE_FATIGUE_DRIVING_INFO];
				Rtime = Rtc.BDC_DateTimeToSecond((RTC_STR *)pSaveFatigue->EndTime);
				if((TowDateTime<=Rtime)&&(Rtime<=Ntime))
				{
					// 找到符合的数据
					//memmove(&pSendBuffer[LEN_U0_GB_FRAME_HEAD+DataLen],&tBuffer[i * LEN_SAVE_FATIGUE_DRIVING_INFO],LEN_SAVE_STOP_DATA);
					//DataLen += LEN_SAVE_FATIGUE_DRIVING_INFO;	

					*PAddr = FM_SAVE_FATIGUE_DRIVING+(i-1) * LEN_SAVE_FATIGUE_DRIVING_INFO;
		 			PAddr++; 
		 			OverDriveTimes++; 
				}
			}
		}

		ReadPage = 	 FlashPageCnt.FatigueDrivingSave;
		while (ReadPage>0)		 // 取flash中的疲劳数据
		{
			Addr = FF_TIRE_DRIVE_START_PAGE;
			Addr += (ReadPage-1)%FF_TIRE_DRIVE_TOTAL_PAGE;
			DFF.PageRead(Addr, tBuffer);

			for (i=5; i>0;i--)
			{
				pSaveFatigue = (pSAVE_FATIGUE_DRIVING_INFO)&tBuffer[(i-1) * LEN_SAVE_FATIGUE_DRIVING_INFO];
				Rtime = Rtc.BDC_DateTimeToSecond((RTC_STR *)pSaveFatigue->StartTime);
				if((TowDateTime<=Rtime)&&(Rtime<=Ntime))
				{
					// 找到符合的数据
					//memmove(&pSendBuffer[LEN_U0_GB_FRAME_HEAD+DataLen],&tBuffer[i * LEN_SAVE_FATIGUE_DRIVING_INFO],LEN_SAVE_STOP_DATA);
					//DataLen += LEN_SAVE_FATIGUE_DRIVING_INFO;	
					
					*PAddr = Addr+(i-1) * LEN_SAVE_FATIGUE_DRIVING_INFO;
		 			PAddr++; 
		 			OverDriveTimes++;

					if(++OverDriveTimes>=20)
					{
						 return OverDriveTimes;
					}

				}
			}

		}

	return OverDriveTimes;  

 
}

#if (BSJ_LCM)
static void PDU_mobile_num_to_ASCII(uchar *src,  uchar *dest, uchar len)
{
	uchar i;

	i = LEN_SMS_NUMBER_EXT / 2;

	if (len > i)	 // pdu手机号码长度不超过 SMS_NUM_LEN / 2 个字节（包含 86）
	{
		len = i;	
	}

	for (i = 0; i < len; i++)
	{
		*dest++ = (src[i] & 0x0f) + '0';	       //   + 0x30
		*dest++ = ((src[i] >> 4) & 0x0f) + '0';
	}
	dest--;        	  // 定位到手机号（ ascii）后一位
	if (*dest ==  0x3F)   // 0x0f + '0' 	  一般少于11位的手机号码都有‘F'出现
		*dest = '\0';           // 去掉 手机号 （ ascii）后一位， 补0  
	else
		*(++dest) = '\0';    
		
	
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
	
const		uchar		AckXorError[5] = {0x24,0xff,0x02,0xd9,0x0a};
const		uchar		AckOK[5] = {0x24,0x0d,0x02,0x2b,0x0a};
//const		uchar		ReturnSuccess[5] = {0x24,0xa4,0x02,0x2b,0x0a};
//const		uchar		ReturnFailure[5] = {0x24,0xa3,0x02,0x2b,0x0a};

#define		BSJ_D1_PDU_DATA_INDEX		22
#define		BSJ_D2_PDU_DATA_INDEX      21
#define		BSJ_PDU_PARAM_INDEX        7                       // 基本参数(TP-MTI/VFP) 发送，TP-VP用相对格式
#define		BSJ_DATA_LEN_INDEX         2
#define		BSJ_DEFORE_PDU_DATA_LEN    19 							// PDU 格式之前的数据长度
#define		BSJ_DST_ADDR_COUNT_INDEX	9															//目标地址数字个数




#define		DATA_ADDR		3

void			KeyDispCls::BSJ_ReceiveProcessData(void)
{
	uchar		Cmd,s,s1,SMS_Flag;
	uint16	i,j,DataLen,Len;
	uint32	ID;
	char		tBuffer[256],*p;

	Uart2.GetRecBuffer(DispRecBuffer);
	Cmd = DispRecBuffer[1];
	DataLen = DispRecBuffer[2]-2;		// 24 01 0c 31 32 33 34 35 36 37 38 39 30 28 ff
	//Uart.putb(1,DataLen+4,(uchar*)DispRecBuffer);	  os_dly_wait(10);	// 
	s = CalXorSum((DataLen+3),DispRecBuffer);
	if( s != DispRecBuffer[(DataLen+3)] )	
	{
		Uart2.putb(1,5,(uchar*)AckXorError);
		Uart2.SendTask();	
		return;
	}
	Uart2.putb(1,5,(uchar*)AckOK);		// 先应答
	Uart2.SendTask();	
	os_dly_wait(3);
	Uart2.putb(1,5,(uchar*)AckOK);		// 先应答
	Uart2.SendTask();	
	os_dly_wait(3);
	SetBufferValue((uchar*)tBuffer,256,0);
	///////////////////////////////////////////////////
	switch(Cmd)
	{
		//////////////////////////////////////////////// 
		case	eLCM_D_MESSAGE_TO_CENTER:
			if (jt_auth_ok_flag != 1)  break;
			if( (DispRecBuffer[LCM_FRAME_DATA]=='*')&&(DispRecBuffer[LCM_FRAME_DATA+1]=='G')&&(DispRecBuffer[LCM_FRAME_DATA]=='Y'))
			{		// 采用短消息的形式，增加调度屏与主机的通讯功能
				s = CustomCmdProcess();				
			}
			else
			{
				s1 = 0;
				if(GG_Evt.GPRS.AckGSMS == 1)
				{
					if(DataLen<=2)
					{ 
					  GG_Evt.GPRS.AckGSMS = 0;
					  s1 = 0xAA;
					  tBuffer[0] = AckBuffer.Index>>8;
					  tBuffer[1] = AckBuffer.Index;
					  memmove(&tBuffer[2], (uchar *)&DispRecBuffer[LCM_FRAME_DATA], DataLen);
					  GPRS.JT_T808_NewDataPack(eBB_gCMD_TER_ASK_ACK, DataLen+2, 0, 0, (uchar *)tBuffer);	//提问应答
					  Uart.puts("提问应答11 ");	 
					}
				}
				if(s1==0)
				{
					tBuffer[0] = 1;		            //信息类型
					tBuffer[1] = DataLen>>8;			//信息长度
					tBuffer[2] = DataLen;
					memmove(&tBuffer[3], (uchar *)&DispRecBuffer[LCM_FRAME_DATA], DataLen);
					GPRS.JT_T808_NewDataPack(eBB_gCMD_PLAT_INFO_SERVICE, DataLen+3, 0, 0, (uchar *)tBuffer);		//信息服务
					Uart.puts("信息服务应答 ");	 
				}
				LcmDataToServer = TRUE;
				while(AT_Command_Busing)  { os_dly_wait(1); }
				GPRS.PopAlignmentSend(GG_Status.Signal);
				s = WaitLcmDataToServer(1000);				
			}
			if(s==OK)
				SendDataToLCM(0xa4,0,NULL);
			else
				SendDataToLCM(0xa3,0,NULL);
		break;
		//////////////////////////////////////////////// 
		case	eLCM_D_MESSAGE_TO_MOBILE:
			if( (DispRecBuffer[LCM_FRAME_DATA]=='*')&&(DispRecBuffer[LCM_FRAME_DATA+1]=='G')&&(DispRecBuffer[LCM_FRAME_DATA]=='Y'))
			{		// 采用短消息的形式，增加调度屏与主机的通讯功能
				s = CustomCmdProcess();				
			}
			else
			{

				/*s = DispRecBuffer[LCM_FRAME_DATA];
				SMS_SendNumber[0] = 0x38;
				SMS_SendNumber[1] = 0x36;
				memmove((char *)&SMS_SendNumber[2], &DispRecBuffer[LCM_FRAME_DATA+1], s);
				Uart.puts(SMS_SendNumber);  os_dly_wait(10);
				SMS.SetPduDataMessage(SMS_UCS2, (char *)SMS_SendNumber,&DispRecBuffer[LCM_FRAME_DATA + 1 + s], DataLen-1-s);*/

				SMS_SendNumber[0] = 0x38;
				SMS_SendNumber[1] = 0x36;

				SMS_Flag = FALSE;

				if ((DispRecBuffer[BSJ_PDU_PARAM_INDEX] == 0x11) &&  (DispRecBuffer[BSJ_PDU_PARAM_INDEX + 1] == 0x00))
				{	
					s = DispRecBuffer[BSJ_DST_ADDR_COUNT_INDEX];  // 目标地址长度，不包括91和‘F’
					if ((s % 2) != 0) s = (s + 1) / 2;
					else				 s = s / 2;

					 s += 1;			// 加一个字节 （ 91）

					 s1 = 3 + 7 + s + 3;	  // D1 屏用户数据长度下标
					//if (DispRecBuffer[BSJ_D1_PDU_DATA_INDEX - 1] == 
						//(DispRecBuffer[BSJ_DATA_LEN_INDEX] - BSJ_DEFORE_PDU_DATA_LEN - 2))   // BSJ D1 PDU 第22字节为pdu格式的数据长度

					if (DispRecBuffer[BSJ_DATA_LEN_INDEX] == (7 + s + 4 + (DispRecBuffer[s1]) + 2))
					{
						// \x00 \x01 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x0512345 \x0FL00001922280586 \x04萧工1392228586 \x00 \x0013886088888 \x00608888844011 \x08粤A08888枚
						//例 24 02 1B 30 32 31 00 11 00 0D 91 68 31 75 20 03 96 F2 00 08 01 06 76 84 76 84 76 84 15  FF
						//PDU_mobile_num_to_ASCII(&DispRecBuffer[LCM_PDU_SEND_NUM], (uchar *)&SMS_SendNumber[2], 6);
						//SMS.SetPduDataMessage(SMS_UCS2, (char *)SMS_SendNumber,&DispRecBuffer[BSJ_D1_PDU_DATA_INDEX], 
						//						DispRecBuffer[BSJ_DATA_LEN_INDEX ] - BSJ_DEFORE_PDU_DATA_LEN - 2);
					
						PDU_mobile_num_to_ASCII(&DispRecBuffer[LCM_PDU_SEND_NUM], (uchar *)&SMS_SendNumber[2], s - 2);

						//s = memcmp((uchar *)&SMS_SendNumber[2], "10086", 5);
						s = strlen((char *)&SMS_SendNumber[2]);
						if (s != 11)								// 不是11位手机号, 只能发送英文短信
						{
							//Uart.puts("AAA");
							//Uart.puts(&SMS_SendNumber[2]);
							//os_dly_wait(1);
							for (i = 1, j = 0; i < DispRecBuffer[s1]; i += 2)
							{
								tBuffer[j++] = DispRecBuffer[s1+1 + i];  // unicode码的英文转为ASCII码的英文   00 41 -> 41
							}
							GSM.SetSMS_Buffer(&SMS_SendNumber[2], tBuffer);
							SMS_Flag = TRUE;
							
						}
						else
						{
							SMS.SetPduDataMessage(SMS_UCS2, (char *)SMS_SendNumber,&DispRecBuffer[s1+1], DispRecBuffer[s1]); 	  
						}
						
					}
					else 		  // BSJ D2 PDU    D2 屏不存在pdu格式的数据长度	指示字节
					{
						//例  24 02 1A 30 31 35 00 11 00 0D 91 68 31 75 20 03 96 F2 00 08 01 76 84 76 84 76 84 15 FF

						//PDU_mobile_num_to_ASCII(&DispRecBuffer[LCM_PDU_SEND_NUM], (uchar *)&SMS_SendNumber[2], 6);
						PDU_mobile_num_to_ASCII(&DispRecBuffer[LCM_PDU_SEND_NUM], (uchar *)&SMS_SendNumber[2], s - 2);
						SMS.SetPduDataMessage(SMS_UCS2, (char *)SMS_SendNumber,&DispRecBuffer[BSJ_D2_PDU_DATA_INDEX], 
														DispRecBuffer[BSJ_DATA_LEN_INDEX ] - BSJ_DEFORE_PDU_DATA_LEN + 1 - 2);
					}
					
				}
				else 					  // BSJ, D2, TEXT		文本信息 	 D2可设置TEXT 和PDU , D1只有PDU
				{
					s = DispRecBuffer[LCM_FRAME_DATA];
					if (s > LEN_SMS_NUMBER_EXT / 2)   return;			     // pdu 格式的手机号不超过 SMS_NUM_LEN / 2 个字节	 
					//SMS_SendNumber[0] = 0x38;
					//SMS_SendNumber[1] = 0x36;
					memmove((char *)&SMS_SendNumber[2], &DispRecBuffer[LCM_FRAME_DATA+1], s);
					Uart.puts(SMS_SendNumber);  os_dly_wait(10);
					SMS.SetPduDataMessage(SMS_UCS2, (char *)SMS_SendNumber,&DispRecBuffer[LCM_FRAME_DATA + 1 + s], DataLen-1-s);
					
				}
				        

				//
			   //PDU_SMS.SendTimes = 0;
				//PDU_SMS.PDU_SendFlag = 0;
				//s = SMS.SendPduMessage(&PDU_SMS, PDU_SMS.TP_UD_Len);
				ID = SystemLog.GSM.SendSms;
				for(s1=0; s1<1000; s1++)
				{
					if(((SMS_Flag == FALSE) && (PDU_SMS.PDU_SendFlag == 0)) || 
						(SMS_Flag == TRUE) && (TextSMS.SendFlag == 0))
					  break;
					os_dly_wait(1);
				}
			}
			if(SystemLog.GSM.SendSms>ID)
				SendDataToLCM(0xa4,0,NULL);
			else
				SendDataToLCM(0xa3,0,NULL);
		break;

		//////////////////////////////////////////////// 
		case	eLCM_D_SET_TCP_IP_PORT:						  //TCP IP实际为JT808的主服务器TCP地址 端口号则是主/副TCP的端口号，
			// <1,”202.106.119.249”,8888>
			for(j=0,i=LCM_FRAME_DATA+4;i<32;i++)
			{
				if( (DispRecBuffer[i]=='>')||(DispRecBuffer[i]==0))
					break;
				else if(DispRecBuffer[i]!='"')
				{					
					tBuffer[j] = DispRecBuffer[i];	
					if(tBuffer[j]==',')	tBuffer[j] = ':';
					j++;
				}
			}
			s = ValidIP_Port((uchar*)tBuffer);
			if(s==OK)
			{
				tBuffer[100] = 0;
				SendDataToLCM(0xb1,1,(uchar*)&tBuffer[100]);				
				strcpy((char*)sIP.TCP,tBuffer);
				DFE.Write(EE_IP_PORT,24,(uchar*)sIP.TCP);
			
				p = MyStrChar((char *)tBuffer, ':');			    //JT808 
				*p = 0;
				DFE.Write(EE_JT_M_IP_DNS,32,(uchar *)tBuffer);		       //JT808 
				*p = ':';									  
				p = MyStrChar((char *)tBuffer, ':');			    //JT808 
				ID = My_atol(p+1);
				DFE.Write(EE_JT_TCP_PORT,4,(uchar *)&ID);

				NOP();	NOP();	NOP();	NOP();
				sIP.EnableFlag[eFLAG_UDP] = 0;
				sIP.EnableFlag[eFLAG_TCP] = 0;
				if(DispRecBuffer[LCM_FRAME_DATA+1]==0x31)					  //1,只开通TCP通道
				{
				   sIP.EnableFlag[eFLAG_TCP] = ENABLE_FLAG;
				}
				else if(DispRecBuffer[LCM_FRAME_DATA+1]==0x32)			  //2，只开通UDP通道
				{
				   sIP.EnableFlag[eFLAG_UDP] = ENABLE_FLAG;
				}
				else if(DispRecBuffer[LCM_FRAME_DATA+1]==0x33)			  //3，开通双通道，0或其他关闭主服务器TCP和UDP通道
				{
					sIP.EnableFlag[eFLAG_UDP] = ENABLE_FLAG;
				   sIP.EnableFlag[eFLAG_TCP] = ENABLE_FLAG;
				}
				DFE.Write(EE_ENABLE_TCP,1,(uchar*)&sIP.EnableFlag[eFLAG_TCP]);
				DFE.Write(EE_ENABLE_UDP,1,(uchar*)&sIP.EnableFlag[eFLAG_UDP]);
				GG_Status.Flag.Word = GG_FLAG_INIT_DATA;	
		      Uart.puts("GPRS Close at keydisp\r\n\0");
			}
			else
			{																				  
				tBuffer[100] = 1;
				SendDataToLCM(0xb1,1,(uchar*)&tBuffer[100]);
			}				
		break;

		case  eLCM_D_SET_UDP_IP_PORT: 							//UDP IP实际为JT808的备份服务器TCP地址 端口号则是主/副UDP的端口号，
			// <1,”202.106.119.249”,8888>
			for(j=0,i=LCM_FRAME_DATA+4;i<32;i++)
			{
				if( (DispRecBuffer[i]=='>')||(DispRecBuffer[i]==0))
					break;
				else if(DispRecBuffer[i]!='"')
				{					
					tBuffer[j] = DispRecBuffer[i];	
					if(tBuffer[j]==',')	tBuffer[j] = ':';
					j++;
				}
			}
			s = ValidIP_Port((uchar*)tBuffer);
			if(s==OK)
			{
				tBuffer[100] = 0;
				SendDataToLCM(0xb1,1,(uchar*)&tBuffer[100]);				
				strcpy((char*)sIP.TCP1,tBuffer);
				DFE.Write(EE_IP_PORT,24,(uchar*)sIP.TCP1);
			
				p = MyStrChar((char *)tBuffer, ':');			    //JT808 
				*p = 0;
				DFE.Write(EE_JT_B_IP_DNS,32,(uchar *)tBuffer);		       //JT808 
				*p = ':';									  
						
				p = MyStrChar((char *)tBuffer, ':');			    //JT808 
				ID = My_atol(p+1);
				DFE.Write(EE_JT_UDP_PORT,4,(uchar *)&ID);

				NOP();	NOP();	NOP();	NOP();
				sIP.EnableFlag[eFLAG_UDP1] = 0;
				sIP.EnableFlag[eFLAG_TCP1] = 0;
				if(DispRecBuffer[LCM_FRAME_DATA+1]==0x31)						  //1,只开通TCP通道
				{
				   sIP.EnableFlag[eFLAG_TCP1] = ENABLE_FLAG;
				}
				else if(DispRecBuffer[LCM_FRAME_DATA+1]==0x32)				  //2，只开通UDP通道
				{
				   sIP.EnableFlag[eFLAG_UDP1] = ENABLE_FLAG;
				}
				else if(DispRecBuffer[LCM_FRAME_DATA+1]==0x33)				  //3，开通双通道，0或其他关闭备份服务器TCP和UDP通道
				{
					sIP.EnableFlag[eFLAG_UDP1] = ENABLE_FLAG;
				   sIP.EnableFlag[eFLAG_TCP1] = ENABLE_FLAG;
				}
				DFE.Write(EE_ENABLE_TCP1,1,(uchar*)&sIP.EnableFlag[eFLAG_TCP1]);
				DFE.Write(EE_ENABLE_UDP1,1,(uchar*)&sIP.EnableFlag[eFLAG_UDP1]);
				//GG_Evt.GPRS.ReDial = TRUE;
				GG_Status.Flag.Word = GG_FLAG_INIT_DATA;	
		      Uart.puts("GPRS Close at keydisp\r\n\0");
			}
			else
			{																				  
				tBuffer[100] = 1;
				SendDataToLCM(0xb1,1,(uchar*)&tBuffer[100]);
			}				

		break;
		//////////////////////////////////////////////// 
		case	eLCM_D_SIM_CARD_NUMBER:
		   SetBufferValue(sTelNum.Sim, 16, 0);
			sTelNum.Sim[0] = 0x30;
			memmove((uchar*)&sTelNum.Sim[1],(uchar*)&DispRecBuffer[LCM_FRAME_DATA+4],11);
			DFE.Write(EE_SIM_NUMBER,16,(uchar*)&sTelNum.Sim[0]);
			DecCharToPressBCD(12,(char *)sTelNum.Sim,SimNumberBCD);
			memmove(sTelNum.SimBCD_JT, SimNumberBCD, 6);
			
		break;

		//////////////////////////////////////////////// 
		case	eLCM_D_QUE_MDT_STATUS:
			
			//strcpy(tBuffer,(char*)sTelNum.Center1);					  
			strcpy(tBuffer,(char*)JT808_Number.CenterSMS);		
			strcat(tBuffer,",\0");
			//strcpy(tBuffer,(char*)sTelNum.Center1);
	   	strcat(tBuffer,(char*)JT808_Number.CenterSMS);	  
			strcat(tBuffer,",0,0,0,0,0,\0");
			strcat(tBuffer,(char*)&sTelNum.Sim[1]);    //strcat(tBuffer,(char*)sTelNum.Sim);
			strcat(tBuffer,",\0");
			strcat(tBuffer,(char*)JT808_Number.TextSMS);	    //strcat(tBuffer,(char*)sTelNum.User); 
			strcat(tBuffer,",0,0,0,0,0,0,0,0,\0");				 //0,
			Len = strlen(tBuffer);
			Sys.GetHS_Version(&tBuffer[Len]);
			strcat(tBuffer,",\0");
						
			//strcat(tBuffer,(char*)sIP.TCP1);							// UDP IP		
			strcat(tBuffer,(char*)JT808_IP_DNS.B_IPorDNS);	
			Len = strlen(tBuffer);
			for(i=Len-7;i<Len;i++)
			{
				if(tBuffer[i]==':')	tBuffer[i] = ',';
			}
			strcat(tBuffer,",\0");
			//Len = strlen(tBuffer);
			//Int2Str(JT808_IP_DNS.TCP_Port,(char*)&tBuffer[Len]);
			//strcat(tBuffer,",\0");
			//strcat(tBuffer,(char*)sIP.TCP);							// TCP IP			
			strcat(tBuffer,(char*)JT808_IP_DNS.M_IPorDNS);	
			Len = strlen(tBuffer);
			for(i=Len-7;i<Len;i++)
			{
				if(tBuffer[i]==':')	tBuffer[i] = ',';
			}
			strcat(tBuffer,",\0");	
			//Len = strlen(tBuffer);
			//Int2Str(JT808_IP_DNS.UDP_Port,(char*)&tBuffer[Len]);
			//strcat(tBuffer,",\0");
			strcat(tBuffer,(char *)JT808_IP_DNS.M_APN);
			strcat(tBuffer,":\0");
			strcat(tBuffer,(char *)JT808_IP_DNS.B_APN);
			Len = strlen(tBuffer);					
			SendDataToLCM(0xc2,Len,(uchar*)tBuffer);
			
		break;

		//////////////////////////////////////////////// 	  <13570876758>
		case	eLCM_D_CENTER_NUMBER:
			SetBufferValue(JT808_Number.CenterSMS, 16, 0);
			memmove((uchar*)JT808_Number.CenterSMS,(uchar*)&DispRecBuffer[LCM_FRAME_DATA+1],DataLen - 2);
			DFE.Write(EE_SIM_NUMBER,16,(uchar*)JT808_Number.CenterSMS);
		break;

		case eLCM_D_LCM_VERSION:		
		//case eLCM_D_DRIVER_LOGIN_OUT:
		// 调度屏每隔几秒钟发送版本号及驾驶员号，主机难以识别是真正登签还是退签。所以登退签采用类键盘扫描去抖方法
			// 24 0A 1D 32 30 31 31 2E 30 36 2E 32 38 3B 4C 43 44 5F 54 54 53 5F 56 32 2E 30 00 00 27 11 52 FF 
			// 24 0A 1D 32 30 31 31 2E 30 36 2E 32 38 3B 4C 43 44 5F 54 54 53 5F 56 32 2E 30 00 01 27 11 53 FF
			ID = DispRecBuffer[28];
			ID <<= 8;
			ID |= DispRecBuffer[29];
			s1 = DispRecBuffer[27];			// 0登录，1退出

			if( (ID==0) || ((s1!=0)&&(s1!=1)&&(s1!=0xaa)) )		return;		// 手柄及部分调度屏不支持IC卡	

			if(  (ID==DriverID[0])||(ID==DriverID[1])||(ID==DriverID[2])||(ID==DriverID[3])\
				||(ID==DriverID[4])||(ID==DriverID[5])||(ID==DriverID[6])||(ID==DriverID[7]))
			{
				s = OK;		// 合法卡
				Driver.CardStatus = '0';
				NoDriverCnt = 0;
				//if(s1 == 0x00)		VehicleControl.Lock = LOCK_VEHICLE_DISABLE;	
			}
			else
			{
				s = ERROR; 	// 非法卡
				Driver.CardStatus = '1';				
			}
			
			// 与上次不同号就处理，并上报到中心
			if(s1==0xaa)
			{
				Driver.CardStatus = '4';
				NoDriverCnt++;
				//if( (NoDriverCnt==2)||((NoDriverCnt%10)==0))	// 不能输出提示，因为停车走人时不插卡
				//	SendDataToLCM(eLCM_U_CENTER_MSG,(4*2),(uchar*)"未插卡，即将锁车！\0");					
				//if(NoDriverCnt==5)
					//VehicleControl.Lock = LOCK_VEHICLE_ENABLE_INV;
			}
			else if( (ID != Driver.ID)||(s1 != (Driver.LogStatus-'0')) ) 
			{
				NoDriverCnt = 0;
				if(s1 == 0x00)				// 登签
				{ 	
					Driver.LogStatus = '0';
					if(s==OK)	// 有效卡		
					{
						//VehicleControl.Lock = LOCK_VEHICLE_DISABLE;
						if( (ID != Driver.ID) || (Driver.LogOutTime>TiredDrive.RestTime) )
						{
							Driver.LogInTime = 0;
						}
					}
					else
					{
						//VehicleControl.Lock = LOCK_VEHICLE_ENABLE_INV;
						SendDataToLCM(eLCM_U_CENTER_MSG,(4*2),(uchar*)"未授权卡！\0");
					}
				}
				else if(s1 == 0x01)		// 退签 (调度屏输出的退签卡号和最近的登签卡号相同）
				{
					Driver.LogStatus = '1';
					if(s==OK)				// 有效卡
					{
						//VehicleControl.Lock = LOCK_VEHICLE_ENABLE_INV;
						Driver.LogOutTime = 0;							
					}
				}		
				DFE.Write(EE_LOCK_FLAG,LEN_LOCK_VEHICLE_FLAG,(uchar *)&VehicleControl);				
				
				Driver.ID = ID;
				//////////////////////////// 上报到中心		
				strcpy(tBuffer,sNowTimeString);
				strcat(tBuffer,",1,\0");		 		// 卡动作
				if( (s==OK) )			// 合法卡
				{
					if(s1==0)	{	strcat(tBuffer,"0,\0");		}
					else			{	strcat(tBuffer,"1,\0");		}					
				}
				else 						// 非法卡
				{
					if(s1==0)	{	strcat(tBuffer,"2,\0");	}
					else			{	strcat(tBuffer,"3,\0");	}	
				}
				strcat(tBuffer,"9,\0");
				DataLen = strlen(tBuffer);
				Int32ToDecChar(Driver.ID,&tBuffer[DataLen]);
				for(i=0;i<8;i++)
				{
					strcat(tBuffer,",");
					DataLen = strlen(tBuffer);
					Int32ToDecChar(DriverID[i],&tBuffer[DataLen]);					
				}
				strcat(tBuffer,"#\0");
				DataLen = strlen(tBuffer);
				
				GPRS.TH_NewDataPack((uchar*)"I2\0",(uchar*)tBuffer);			
				
				DFE.Write(EE_DRIVER_INFO,LEN_DRIVER_INFO,(uchar*)&Driver);		// 当前司机卡号

			}
			if(TD.Display==tHANDLE_DISPLAY_TEST)
			{
				if(s==OK)	tBuffer[0] = 'A';
				else			tBuffer[0] = 'V';
				tBuffer[1] = ' ';
				if(s1==0)	{strcat(tBuffer,"Login  \0");}
				else			{strcat(tBuffer,"Logout \0");}
				DataLen = strlen(tBuffer);
				Int16ToDecChar(ID,&tBuffer[DataLen]);
				strcat(tBuffer,"\r\n\0");
				Uart.puts(tBuffer);
			}

		break;


		case	eLCM_D_DIAL_NUMBER:		  	// 	
			
			PhoneCall.DialCnt = 0;
			PhoneCall.Length = DataLen;
			memmove(PhoneCall.Buffer,&DispRecBuffer[LCM_FRAME_DATA],PhoneCall.Length);			
			PhoneCall.DialFlag = TRUE;
		break;

		case	eLCM_D_RING_OFF:
			PhoneCall.KeyHang = TRUE;	 	// 手柄主动挂机

		break;

		case 	eLCM_D_ANSWER_CALL:			// 被叫，手机摘机
			PhoneCall.CalledAnswer = TRUE;

		break;

		default:
		   break;

		/*
		eLCM_D_MESSAGE_TO_CENTER 		= 0x01,				//  
		eLCM_D_MESSAGE_TO_MOBILE		= 0x02,				// 
  		eLCM_D_SET_UDP_IP_PORT			= 0x03,				// 
		eLCM_D_SIM_CARD_NUMBER			= 0x04,				// 
		eLCM_D_CENTER_NUMBER				= 0x05,			   // 
		eLCM_D_NO_SCAN_ALARM_TIME		= 0x06,				// 
		eLCM_D_DIAL_NUMBER				= 0x07,				// 
		eLCM_D_RING_OFF					= 0x08,				// 
		eLCM_D_SEND_DTMF					= 0x09,				// 
		eLCM_D_LCM_VERSION				= 0x0a,				// 
		eLCM_D_SET_APN						= 0x0b,				//
		eLCM_D_SELF_CHECK					= 0x0c,				// 
		eLCM_D_ANSWER_CALL				= 0x0e,				// 
		eLCM_D_HAND_OFF					= 0x0f,				//
		eLCM_D_SET_USE_ALARM_NUMBER	= 0x10,				//
		eLCM_D_QUE_MDT_STATUS			= 0x11,				// 
		eLCM_D_SET_WATCH_CAR				= 0x12,				//
		eLCM_D_CANCEL_WATCH_CAR			= 0x13,				// 
		eLCM_D_SELECT_APN_OR_GPRS		= 0x14,				// 
		eLCM_D_SELECT_UDP_OR_TCP		= 0x15,				/// 
		eLCM_D_ADJ_VOL						= 0x16,				// 
		eLCM_D_ADJ_MIC						= 0x17,				// 

		eLCM_D_DOOR_LEVEL					= 0x21,				// 
		eLCM_D_CAR_SPK_LEVEL				= 0x22,				// 
		eLCM_D_SET_RADIO_STATUS			= 0x23,				// 
		eLCM_D_SET_HIDE_MODE				= 0x24,				// 
		eLCM_D_ER_PASSWORD				= 0x25,				// 
		eLCM_D_SET_CAR_NUMBER			= 0x26,		// 
		eLCM_D_SET_CENTER_NUMBER1		= 0x27,		// 
		eLCM_D_SET_SHAKE_VALUE			= 0x28,		// 
		eLCM_D_SET_TEMPERATE_VALUE		= 0x29,		// 

		eLCM_D_DRIVER_LOGIN_OUT			= 0x30,		// 
		eLCM_D_SIM_CARD_NUMBER1			= 0x31,		// 
		eLCM_D_SET_ROB_ALARM_NUMBER	= 0x32,		// 
		eLCM_D_QUE_MDT_STATUS2			= 0x33,		// 
		eLCM_D_SET_CALL_MODE				= 0x34,		// 
		eLCM_D_SET_TCP_IP_PORT			= 0x35,		// 

		eLCM_D_RESTORE_FACTORY_PARA	= 0x37,		// _RestoreFactorySetting
		eLCM_D_SET_SERVER_LOGIN_NAME	= 0x38,		// 
		eLCM_D_SET_SERVER_LOGIN_PSD	= 0x39,		// 
		*/
	}
}
#endif 

//#define		CONNECT_CREATE_USER_CENTER		'A'
//#define		CONNECT_MAIN_CENTER				'B'

//const	char Create_User_Dns[LEN_IPorDNS] = {"58.83.210.131\0\0\0\0\0\0"};

		

uchar 	Connect_Create_USer_Center_Or_Main_Center(uchar Type, uchar Wait)
{
	uchar		IP_Port[LEN_IP_PORT] = { 0 };	
	uchar 		t,i;
	uint32		tmp;

	if (Type == CONNECT_CREATE_USER_CENTER)
	{

		//CreateUserFlag = ENABALE_CREATE_USER;
		Uart1.ClearTransmitFlag();

		// 保存主中心IP和端口到临时区
		DFE.Read(EE_JT_M_IP_DNS, LEN_IPorDNS, &IP_Port[0]);
		NOP(); NOP(); NOP();
	
		DFE.Write(EE_JT_TMP_TCP_IP, LEN_IPorDNS, &IP_Port[0]);
		NOP(); NOP(); NOP();
	
		DFE.Read(EE_JT_TCP_PORT,4,(uchar *)&tmp);
		NOP(); 
		DFE.Write(EE_JT_TMP_TCP_PORT, 4, (uchar *)&tmp);
		NOP(); NOP(); NOP();
	
		// 修改TCP使用的IP和端口
		SetBufferValue(IP_Port, LEN_IPorDNS, 0);
		strcpy((char *)IP_Port, Create_User_Dns);

		DFE.Write(EE_JT_M_IP_DNS, LEN_IPorDNS, &IP_Port[0]);
		NOP(); NOP(); NOP();NOP(); NOP(); NOP();
		tmp = CreateUserDnsPort;
		DFE.Write(EE_JT_TCP_PORT,4,(uchar *)&tmp);
		NOP(); NOP(); NOP();

	}
	else if (Type == CONNECT_MAIN_CENTER)
	{
		DFE.Read(EE_JT_TMP_TCP_IP, LEN_IPorDNS, &IP_Port[0]);
		NOP(); 
		
		DFE.Read(EE_JT_TMP_TCP_PORT, 4, (uchar *)&tmp);
		NOP();
		
		DFE.Write(EE_JT_TCP_PORT,4,(uchar *)&tmp);
		NOP(); NOP(); NOP(); 

		DFE.Write(EE_JT_M_IP_DNS, LEN_IPorDNS, &IP_Port[0]);
		NOP(); NOP(); NOP();
		
	}
	sIP.EnableFlag[eFLAG_TCP] = ENABLE_FLAG;
	t = ENABLE_FLAG;
	DFE.Write(EE_ENABLE_TCP,1,(uchar*)&t);
	NOP(); NOP(); NOP();
	sIP.Online[eFLAG_TCP] = 0;
	GG_Status.Flag.Word = GG_FLAG_INIT_DATA;	
	Uart.puts("GRRS Close\r\n\0");
	os_dly_wait(100);

	if (!Wait) return OK;

	i = 0;
	while (sIP.Online[eFLAG_TCP]!= ENABLE_FLAG)	   // 等待上线
	{
		i++;	
		if (i > 300)	return ERROR;			 // 30秒超时

		os_dly_wait(10);	
	}

	//Uart.puts("EEE22");
    //os_dly_wait(1);
		
	return OK;
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：	
   功能描述：使用锁定解锁
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
0F 51 80 55 
00 01 00 00 00 00 00 
00 00 00 00 
05 31 32 33 34 35 
0F 4C 30 30 30 30 31 39 32 32 32 38 30 35 38 36 
04 CF F4 B9 A4 
31 33 39 32 32 32 38 30 35 38 36 00 
31 33 38 38 36 30 38 38 38 38 38 00 
36 30 38 38 38 38 38 
34 34 30 31 31 08 D4 C1 41 30 38 38 38 38 99 13 37 F0 

*/ //////////////////////////////////////////////////////////////////////////////////

void 			JT808_UnLock_Terminal(uchar *pData, uint16 Len)
{
	uchar 	i, s;
	uint16 	Addr, CurLen;

  	uchar		tBuffer[80];

	Addr = 11;
	for(i=0;i<3;i++)		// 3段变长数据
	{
		CurLen = *(pData+Addr);
		Addr += (CurLen+1);		
	}

	SetBufferValue(tBuffer,80,0);

	memmove( &tBuffer[5],(pData+Addr),12);		

	CurLen = GPRS.GetRegInfo(&tBuffer[17]);
   CurLen += 17;

	tBuffer[17] = 0;
	tBuffer[17] = 41;		// 省域ID

	tBuffer[19] = 0x01;
	tBuffer[20] = 0x2f;	// 市域ID

	tBuffer[53] = 2;		// 车牌颜色，黄
	memmove( &tBuffer[54],"检G",3);


	// 向货运中心注册
	for (i = 0; i < 2; i++)
	{
		if (OK == Connect_Create_USer_Center_Or_Main_Center(CONNECT_CREATE_USER_CENTER, 1))
		{
			break;
		}
	}
	if (i >= 2)
	{
		Connect_Create_USer_Center_Or_Main_Center(CONNECT_MAIN_CENTER,0);	
		// 终端不在线
		//KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(6*2+1),(uchar*)"\x43\x7E\xC8\x7A\xEF\x00\x20\x4E\x0D\x57\x28\x7E\xBF");
		s = 0x04;  // 其他错误
		KeyDisp.SendDataToLCM_YH(eLCM_D_CAR_REG_RESULT, 1, &s); 
		os_dly_wait(5);

		return;
	}
	Creat_User_Reging = TRUE;
	Creat_User_Reging_OverTime = 0;
	GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_CREATE_USER,CurLen,tBuffer);
	os_dly_wait(50);
	Uart.puts("使用前解锁 ");
    
	
}

// added 2013.4

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 	
/*
0F 03 30 42 31 33 38 32 38 37 39 34 31 36 31 4D F0 FF
	03^30^42^31^33^38^32^38^37^39^34^31^36^31=4D
*/
uint16			CallForSetPara;
extern		uchar			OnlyUsePulseSpeed,OnlyUseGPS_Speed;
extern		uchar		GPS_CoolStart;
//extern		uint32	GPSValidCnt;
//uchar		const		TestCmdData[] = {0x0F,0xA4,0x33,0x44,0x4E,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x30,0x43,0x00,0x2A,0x00,0x47,0x00,0x59,0x00,0x2C,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x2C,0x00,0x52,0x00,0x38,0x00,0x31,0x00,0x2C,0x00,0x31,0x00,0x32,0x00,0x33,0x00,0x34,0x00,0x35,0x00,0x36,0x00,0x23,0xAE,0x27,0xF0,0xFF};
 extern char		DefaultCenterNumber1[];
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			KeyDispCls::YH_ReceiveProcessData(void)
{
	uchar		t,tCmd,Cmd = 0,s,s1,SMS_Flag,*pT;
	union
	{
		uchar		Buffer[256];
		uint16	B16[128];
		uint32	B32[64];
	}Tp;
	uint16		i,j,DataLen,Len;
	uint32		ID;
	
	//memmove(U0_RecBuffer,TestCmdData,sizeof(TestCmdData));
	
	//Cmd = DispRecBuffer[1];
	Uart2.GetRecBuffer(DispRecBuffer);

	//Uart.putb(1, 128, DispRecBuffer);
	//os_dly_wait(1);

	// 注：因为最大长度256不能满足有些指令的要求，所以对长度做了扩展，下面是扩展说明：
	// 如果第一个字节的最高位为1，则表示扩展长度。在扩展长度状态下，其他15个字节通过16进制大端模式来保存长度。
	// 比如：0x80 0x12表示长度为0x0012，0x81 0x12表示长度为0x0112。
	if(DispRecBuffer[2] & 0x80)	// 为扩展长度
	{
	  DataLen = (DispRecBuffer[2] & 0x7F) << 8;
	  DataLen |= DispRecBuffer[3];
	  
	  //Uart.puts("AAA111");Uart.putb(1, 2, (uchar *)&DataLen);os_dly_wait(1); 
	}
	else   		 // 普通长度
	{
		// ASCII码表示（0~9，A~F，最大长度为256）(例如长为11个，十六进制是0B，则两个字节就写0x30 0x42)
		DataLen = HexCharToByte((char*)&DispRecBuffer[2]);		// 

	}

	Cmd = DispRecBuffer[1];
	if( (Cmd!=0x56) )				// 可恶，这些指令无校验
	{

		s = CalXorSum((DataLen+3),&DispRecBuffer[1]);

		if( s != DispRecBuffer[(DataLen+4)] )	
		{
			SendDataToLCM_YH(0xfe,0,Tp.Buffer);  	// 检验错误
			return;
		}
	}
	

	//Uart.putb(1, DataLen+6, DispRecBuffer);os_dly_wait(1);
	
	SetBufferValue((uchar*)Tp.Buffer,256,0);
	///////////////////////////////////////////////////
	switch(Cmd)
	{
		//////////////////////////////////////////////// 
		case 0x01:	 // 单字节指令
		{
			Cmd = DispRecBuffer[4];
			switch(Cmd)
			{
				case 0x01:	break;
				case 0x02: 	
					if (PhoneCall.Busing!=TRUE)
						PhoneCall.CalledAnswer = TRUE; 		
					break;

				case 0x03:	
				
					if( CallForSetPara==0)		PhoneCall.KeyHang = TRUE;
				break;

				case 0x06:	// 呼叫中心
					//PhoneCall.DialCnt = 0;
					//PhoneCall.Length = DataLen;
					//memmove(PhoneCall.Buffer,DefaultCenterNumber1,strlen(DefaultCenterNumber1));		  // 	 先注释
					//PhoneCall.DialFlag = TRUE;
				break;
					
				case 0xB0:
					Tp.Buffer[0] = 0x36;
					SendDataToLCM_YH(eLCM_U_PHONE_VOL,1,Tp.Buffer);
					
				break;
				default:		break;
			}
			DataLen = 0;
		}
		break;

		//////////////////////////////////////////////// 
		case 0x02:			//  拨号

			if( (DispRecBuffer[LCM_FRAME_DATA_YH]=='0')&&(DispRecBuffer[LCM_FRAME_DATA_YH+1]=='*')  )
				goto SetParaToGPS;
			PhoneCall.DialCnt = 0;
			PhoneCall.Length = DataLen;
			memmove(PhoneCall.Buffer,&DispRecBuffer[LCM_FRAME_DATA_YH],DataLen);			
			PhoneCall.DialFlag = TRUE;
			DataLen = 0;			
		
		break;

		//////////////////////////////////////////////// 
		case 0x03:			// 
			DataLen = 0;
			
				
		break;


		/////////////////////////////////////////////////
		case 0x0c:
SetParaToGPS:
			//Uart.puts((char*)&DispRecBuffer[LCM_FRAME_DATA_YH]);	// 测试语句
			if( (DispRecBuffer[LCM_FRAME_DATA_YH]=='0')&&(DispRecBuffer[LCM_FRAME_DATA_YH+1]=='*')  )
			{
				CallForSetPara = (10*100UL);		// 10秒

				tCmd = (DispRecBuffer[LCM_FRAME_DATA_YH+2]-'0')<<4;
				tCmd += (DispRecBuffer[LCM_FRAME_DATA_YH+3]-'0');
				t = (DispRecBuffer[LCM_FRAME_DATA_YH+4]-'0')<<4;
				t += (DispRecBuffer[LCM_FRAME_DATA_YH+5]-'0');
				switch(tCmd)	 	// 用参数设置功能，临时实现调度屏未实现的功能
				{
					case 0x00:		// 自检
						
						SystemSelfTest = TRUE;

					break;
					/////////////////////////////////////////////////////// 只有命令字
					case 0x01:		// 手动注册 (应付检测使用）
						Uart.puts("手动注册 ");
						GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_REG,2,Tp.Buffer);
						
					break;

					case 0x02:		// 手动鉴权（应付检测使用）
						Uart.puts("手动鉴权 ");
						GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_AUTH,2,Tp.Buffer);
						
					break;

					case 0x03:		// 电子运单（应付检测使用）			
						Uart.puts("手动电子运单 ");
						strcpy((char*)Tp.Buffer,"这是测试的电子运单\r\n");
						GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_ELE_WAYBILL_UPLOAD,10,Tp.Buffer);
						
					break;

					case 0x04:		// 手动触发多媒体事件上报（应付检测使用）
						Uart.puts("手动触发多媒体 ");
						GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_MEDIA_EVENT_UPLOAD,2,Tp.Buffer);
						
					break;

					case 0x05:		// 手动触发驾驶身份上报信息
						GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DRIVER_INFO_REPORT,2,Tp.Buffer);
						Uart.puts("手动输入驾驶员身份信息 ");
					break;

					case 0x06:		// 校时
						GPS.GPSValidCnt = 0;
						Uart.puts("手动校时");
					break;

					case 0x08:			// 查看注册信息及手机号

						SetBufferValue(Tp.Buffer,256,0);

						strcpy( (char*)Tp.Buffer,"\x47手机号码: ");
						Len = strlen( (char*)Tp.Buffer );
						DFE.Read(EE_SIM_NUMBER, 16, (uchar *)&Tp.Buffer[Len]);
						
						strcat( (char*)Tp.Buffer,"; 省域ID: ");
						Len = strlen( (char*)Tp.Buffer );
						DFE.Read(EE_PROVINCE_REGION_ID, 2, (uchar *)&DataLen);//Tp.Buffer[Len]);
						Int2Str(DataLen, (char*)&Tp.Buffer[Len]);
						
						strcat( (char*)Tp.Buffer,"; 市域ID: ");
						Len = strlen( (char*)Tp.Buffer);
						DFE.Read(EE_PREFECTURE_REGION_ID, 2, (uchar *)&DataLen);//Tp.Buffer[Len]);
						Int2Str(DataLen, (char*)&Tp.Buffer[Len]);
						
						strcat( (char*)Tp.Buffer,"; 制造商ID: ");
						Len = strlen( (char*)Tp.Buffer);
						DFE.Read(EE_VENDOR_ID, 5, (uchar *)&Tp.Buffer[Len]);

						
						strcat( (char*)Tp.Buffer,"; 终端型号: ");
						Len = strlen( (char*)Tp.Buffer);
						DFE.Read(EE_TERMINAL_TYPE, 20, (uchar *)&Tp.Buffer[Len]);
						

						strcat( (char*)Tp.Buffer,"; 终端ID: ");
						Len = strlen( (char*)Tp.Buffer);
						DFE.Read(EE_TERMINAL_ID, 7, (uchar *)&Tp.Buffer[Len]);
						

						strcat( (char*)Tp.Buffer,"; 车牌颜色: ");
						Len = strlen( (char*)Tp.Buffer);
						DFE.Read(FM_CAR_COLOUR, 1, (uchar *)&Tp.Buffer[Len]);
						t = Tp.Buffer[Len];
						
						if(t)
						{
							strcat( (char*)Tp.Buffer,"; 车牌号码: ");
							Len = strlen( (char*)Tp.Buffer);
							DFE.Read(FM_VEHICLE_CODE, 9, (uchar *)&Tp.Buffer[Len]);							
						}
						else
						{
							strcat( (char*)Tp.Buffer,"; 车辆VIN号: ");
							Len = strlen( (char*)Tp.Buffer);
							DFE.Read(FM_VEHICLE_VIN, LEN_VEHICLE_VIN, (uchar *)&Tp.Buffer[Len]);							
						}
						Len = strlen( (char*)Tp.Buffer);
						//SendDataToLCM_YH(eLCM_U_CENTER_MSG,Len,Tp.Buffer);
						//SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,Len,Tp.Buffer);
						CenterSendDataToLCM_YH(Len,Tp.Buffer);
						return;

					break;

					case 0x09:
						Uart.puts("手动向平台注销 ");
						GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DEREG,0,NULL);

					break;

					/////////////////////////////////////////////////////// 1字节命令+1字节数据
					case 0x11:		// 事件报告
						Uart.puts("手动事件报告 ");
						Tp.Buffer[0] = t;
						GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_EVENT_REPORT,1,Tp.Buffer);
						
					break;

					case 0x12:		// 信息点播/取消	 注：这里无需输入信息ID，在发送函数处理
						Uart.puts("手动信息点播取消 ");
						Tp.Buffer[0] = t;	
						GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_GET_INFO,1,Tp.Buffer);
						
					break;

					case 0x16:   // 手动向平台发注销
						Uart.puts("手动向平台发注销");
						GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DEREG,0,NULL);

					break;


					case 0x20:
						Uart.puts("手动清除存储器页计数器 ");
						SetBufferValue( (uchar*)&FlashPageCnt,LEN_FLASH_PAGE_CNT,0);
			   		DFE.Write( FM_FLASH_PAGE_CNT,LEN_FLASH_PAGE_CNT,(uchar*)&FlashPageCnt);	
					break;
					
					case 0x21:
						
						if(t==0)
						{
							SetBufferValue((uchar*)&RoundFenceAttr,LEN_EE_FENCE_ATTR,0);
							DFE.Write(EE_ROUND_FENCE_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RoundFenceAttr);
							SetBufferValue((uchar*)&RectFenceAttr,LEN_EE_FENCE_ATTR,0);
							DFE.Write(EE_RECTANGLE_FENCE_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RectFenceAttr);
							SetBufferValue((uchar*)&PolygonFenceAttr,LEN_EE_FENCE_ATTR,0);
							DFE.Write(EE_POLYGON_FENCE_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&PolygonFenceAttr);
							SetBufferValue((uchar*)&RoadAttr,LEN_EE_FENCE_ATTR,0);
							DFE.Write(EE_ROAD_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RoadAttr);	
							Uart.puts(" 手动删除全部围栏和路线 ");
						}
						else if(t==1)
						{
							SetBufferValue((uchar*)&RoundFenceAttr,LEN_EE_FENCE_ATTR,0);
							DFE.Write(EE_ROUND_FENCE_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RoundFenceAttr);
							Uart.puts(" 手动删除圆形围栏 ");
						}
						else if(t==2)
						{
							SetBufferValue((uchar*)&RectFenceAttr,LEN_EE_FENCE_ATTR,0);
							DFE.Write(EE_RECTANGLE_FENCE_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RectFenceAttr);
							Uart.puts(" 手动删除矩形围栏 ");
						}
						else if(t==3)
						{
							SetBufferValue((uchar*)&PolygonFenceAttr,LEN_EE_FENCE_ATTR,0);
							DFE.Write(EE_POLYGON_FENCE_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&PolygonFenceAttr);
							Uart.puts(" 手动删除多边形围栏 ");
						}
						else if(t==4)
						{
							SetBufferValue((uchar*)&RoadAttr,LEN_EE_FENCE_ATTR,0);
							DFE.Write(EE_ROAD_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RoadAttr);
							Uart.puts(" 手动删除路线 ");
						}
						
					break;
					
					case 0x22:
						
						if(t==0x01)
						{
							DFE.Write(EE_JT_M_IP_DNS,16,(uchar*)"jt1.gghypt.net\0\0");	
							DFE.Write(EE_JT_B_IP_DNS,16,(uchar*)"jt2.gghypt.net\0\0");	
							ID = 1008;
							DFE.Write(EE_JT_TCP_PORT,4,(uchar *)&ID);
							DFE.Write(EE_JT_UDP_PORT,4,(uchar *)&ID);
						}
						else if(t==0x02)
						{
							DFE.Write(EE_JT_M_IP_DNS,16,(uchar*)"202.96.42.115\0\0\0");	
							DFE.Write(EE_JT_B_IP_DNS,16,(uchar*)"202.96.42.113\0\0\0");	
							ID = 9999;
							DFE.Write(EE_JT_TCP_PORT,4,(uchar *)&ID);
							DFE.Write(EE_JT_UDP_PORT,4,(uchar *)&ID);
							sIP.EnableFlag[eFLAG_TCP] = ENABLE_FLAG;
							t = ENABLE_FLAG;
							DFE.Write(EE_ENABLE_TCP,1,(uchar*)&t);
							GPRS.Close();
						}
						else if(t==0)
						{
							DFE.Write(EE_JT_M_IP_DNS,16,(uchar*)"bdgps.guoyee.com\0\0");	
							DFE.Write(EE_JT_B_IP_DNS,16,(uchar*)"bdgps.guoyee.com\0\0");	
							ID = 8021;
							DFE.Write(EE_JT_TCP_PORT,4,(uchar *)&ID);
							DFE.Write(EE_JT_UDP_PORT,4,(uchar *)&ID);
							sIP.EnableFlag[eFLAG_TCP] = ENABLE_FLAG;
							t = ENABLE_FLAG;
							DFE.Write(EE_ENABLE_TCP,1,(uchar*)&t);
							GPRS.Close();
						}
						
					break;

					case 0x23:

						if(t<0x10)
						{
							if(t&0x01) 	JT808Flag.VS3.Bits.UseGPS = 1;
							else			JT808Flag.VS3.Bits.UseGPS = 0;					
							if(t&0x02) 	JT808Flag.VS3.Bits.UseBD = 1;
							else			JT808Flag.VS3.Bits.UseBD = 0;
							DFE.Write(EE_JT_GNNS_POS_MODE, 1, (uchar*)&t);
						}
						else
						{
							SetBufferValue(Tp.Buffer,64,0);
							if(JT808Flag.VS3.Bits.UseGPS == 1 )		strcat((char*)Tp.Buffer,"使用GPS定位，");
							if(JT808Flag.VS3.Bits.UseBD == 1)		strcat((char*)Tp.Buffer,"使用北斗定位");
							CenterSendDataToLCM_YH(strlen((char*)Tp.Buffer),Tp.Buffer);
						}

					break;


					case 0x24:	
						while(AT_Command_Busing)  { os_dly_wait(1); }
						//JT808_HANDLE_AUDIO_ON_CONTROL;
							Uart1.puts("AT+QAUDCH=2\r\n\0"); 	os_dly_wait(50);
						SPEAKER_POWER_ON
						
					   if(t==0x01)
						{
							Uart.puts(" TTS 测试1");
							Uart1.puts("AT+QTTS=2,\"你好\",1\r\n\0");
						}
						else
						{
							Uart.puts(" TTS 测试2");
							Uart1.puts("AT+QTTS=2,\"你好\",2\r\n\0");	  // TEST OK
						}
						os_dly_wait(100);
					break;

					
					//////////////////////////////////// 设备脉冲系数
					case 0x40:
						
						Tp.B32[0] = Str2Int( (char*)&DispRecBuffer[LCM_FRAME_DATA_YH+6] );
						SpeedC.SetGetPara(3,4,Tp.Buffer);
						Uart.puts("手动设备脉冲系数 ");						
						
					break;
					
					// uchar			OnlyUsePulseSpeed,OnlyUseGPS_Speed;
					case 0x90:		// 优先使用脉冲，再使用GPS速度
						OnlyUsePulseSpeed = FALSE;
						OnlyUseGPS_Speed = FALSE;
						Uart.puts("手动设置，优先使用脉冲，再使用卫星速度 ");
						break;
						
					case 0x91:		//使用脉冲速度
						OnlyUsePulseSpeed = TRUE ;
						OnlyUseGPS_Speed = FALSE;
						Uart.puts("只使用脉冲速度 ");
						break;
						
					case 0x92:		// 使用GPS速度
						OnlyUsePulseSpeed = FALSE;
						OnlyUseGPS_Speed = TRUE;
						Uart.puts("只使用卫星速度 ");
						break;

					case 0x93:

						Tp.Buffer[0] = t;
						SpeedC.SetGetPara(eSPEED_CMD_DISABLE_CNT,1,Tp.Buffer);
						
						break;

					case 0x97:
						GPS_CoolStart = TRUE;
						Uart.puts("手动发送卫星模块冷启动指令 ");
					break;
					
					////////////////////////////////////  出厂设备
					case 0x98:
						
						if(t==0x98)
							Sys.FirstPowerOn(TRUE);
						else if(t==90)
							Sys.FirstPowerOn(0);
						Uart.puts("恢复出厂设置 ");
					break;

					/////////////////////////////////// 重启
					case 0x99:		// 重启终端						
						Uart.puts("手动重启系统");
						os_dly_wait(100);
						DisableIRQ();
						while(1);						
					break;

				
					default:	break;
				}
			}
			DataLen = 0;

		break;

		//////////////////////////////////////////////// 
		case 0xe0:			//   参数查询，返回也是e0指令
			DataLen = 0;
			
		break;

		//////////////////////////////////////////////// 
		case 0xa4:			//   收到信息，信息中包含命令，可直接执行命令，用于查询、设置参数
			// 0F A4 33 63 4E 31 32 33 34 35 36 37 38 39 30 31 32 33 34 35 36 37 38 39 30 
			// 43 00 2A 00 47 00 59 00 2C 00 30 00 30 00 30 00 2C 00 52 00 38 00 31 00 2C 00 31 00 32 00 33 00 34 00 35 00 36 00 23 AE F0 
			// U2 Rec Test Data: 0F A4 33 32 4E 34 30 30 36 30 32 30 38 36 33 FF FF FF FF FF FF FF FF FF FF 43 00 4A 00 54 00 31 00 2E 00 47 00 47 00 48 00 59 00 50 00 54 00 2E 00 4E 00 45 00 54 C0 F0 
			
						
			if (jt_auth_ok_flag != 1)  break;	   
			if (DispRecBuffer[LCM_FRAME_DATA_YH] == 0x4E)
			{
				// 如果号码出现"###"说明是要发到中心平台的信息
				if ((DispRecBuffer[LCM_FRAME_DATA_YH+1] == '#') && (DispRecBuffer[LCM_FRAME_DATA_YH+2] == '#') &&
					(DispRecBuffer[LCM_FRAME_DATA_YH+3] == '#'))	       
				{
					s1 = 0;
					Len = DataLen - (1+LCM_PHONE_NUM_LEN_YH+1); // 发送中心的实际数据段长度

					if(GG_Evt.GPRS.AckGSMS == 1)
					{
						//if(Len<=2)
						{ 
						  GG_Evt.GPRS.AckGSMS = 0;
						  s1 = 0xAA;
						  Tp.Buffer[0] = Plat_Serial_No >> 8;//AckBuffer.Index>>8;
						  Tp.Buffer[1] = Plat_Serial_No & 0xff; //AckBuffer.Index;
						  //memmove(&Tp.Buffer[2], (uchar *)&DispRecBuffer[LCM_FRAME_DATA_YH+1+LCM_PHONE_NUM_LEN_YH+1], Len);
						  /*if (Len == 2) s = 1; 	// 回复一般是数字，为ascii码  	 宏地平台 最多答案ID为‘0’ - ‘6’	 调度屏回复数字是 unicode码
						  else			s = 0;
		
						  memmove(&Tp.Buffer[2], (uchar *)&DispRecBuffer[LCM_FRAME_DATA_YH+1+LCM_PHONE_NUM_LEN_YH+1+s], Len-s);	 // 忽略00，比如00 31， 只发送31就可以了
						  //Len -= s;  

						  GPRS.JT_T808_NewDataPack(eBB_gCMD_TER_ASK_ACK, Len+2, 0, 0, (uchar *)Tp.Buffer);	//提问应答 */
						  // 
						  // 0F A4 31 61 4E 23 23 23 23 FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF 43 00 27 00 31 EF F0 
						  s =  DispRecBuffer[LCM_FRAME_DATA_YH+1+LCM_PHONE_NUM_LEN_YH+1+1]; // 数字的unicode码第一个字节为0,有效是第二字节
						  if ((s > '0') && (s <= '9')) 	 
						  {
						  	Tp.Buffer[2] = s - 0x30;	//0x01;
						  }
						  else
						  {
						  	GG_Evt.GPRS.AckGSMS = 1;
							SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(9*2+1),(uchar*)"\x43\x8B\xF7\x56\xDE\x59\x0D\x6B\x63\x78\x6E\x76\x84\x65\x70\x5B\x57\x00\x21"); // 请回复正确的数字!
							DataLen = 0;
							break;
						  }	
						  Uart.puts("提问应答 ");	 
						  GPRS.JT808_DataPackPushAlignment(0,eBB_gCMD_TER_ASK_ACK, 3, 0, (uchar *)Tp.Buffer);		//提问应答
						}
					}
					/*if(s1==0)
					{
						Tp.Buffer[0] = 1;		            //信息类型
						Tp.Buffer[1] = DataLen>>8;			//信息长度
						Tp.Buffer[2] = DataLen;
						memmove(&Tp.Buffer[3], (uchar *)&DispRecBuffer[LCM_FRAME_DATA_YH+1+LCM_PHONE_NUM_LEN_YH+1], Len);
						GPRS.JT_T808_NewDataPack(eBB_gCMD_PLAT_INFO_SERVICE, Len+3, 0, 0, (uchar *)Tp.Buffer);		//信息服务
						//GPRS.PushAlignment(DataLen,&DispRecBuffer[LCM_FRAME_DATA]);
					}*/
					LcmDataToServer = TRUE;
					while(AT_Command_Busing)  { os_dly_wait(1); }
					GPRS.PopAlignmentSend(GG_Status.Signal);
					s = WaitLcmDataToServer(1000);	
					if(s==OK) 	s = 0x17;
					else		s = 0x18;

					SendDataToLCM_YH(0x01,1,&s);			
					
				}  // to center
				else  // 发到手机上的短信息		
				{
				// 0F A4 31 61 4E 31 33 35 37 30 32 33 30 36 39 32 FF FF FF FF FF FF FF FF FF 43 4F 60 59 7D 31 F0 
					SetBufferValue((uchar *)SMS_SendNumber, LEN_SMS_NUMBER_EXT, 0);
					SMS_SendNumber[0] = 0x38;
					SMS_SendNumber[1] = 0x36;
	
					SMS_Flag = FALSE;
					s = 0xff;
					pT = SearchArray(20,(uchar *)&DispRecBuffer[LCM_FRAME_DATA_YH+1], 1, &s);
					if (pT == NULL)	s = 20;	                               
					else s = pT - &DispRecBuffer[LCM_FRAME_DATA_YH+1]; // 电话号码长度

					memmove(&SMS_SendNumber[2], &DispRecBuffer[LCM_FRAME_DATA_YH+1], s);	
	
					s1 = LCM_FRAME_DATA_YH+1+LCM_PHONE_NUM_LEN_YH+1;	 // 短信内容下标
					Len = DataLen - (1+LCM_PHONE_NUM_LEN_YH+1); 			// 短信内容长度

					if (s != 11)								// 不是11位手机号, 只能发送英文短信	  如：飞信号码
					{
						for (i = 1, j = 0; i < Len; i += 2)
						{
							Tp.Buffer[j++] = DispRecBuffer[s1+i+1];  // unicode码的英文转为ASCII码的英文   00 41 -> 41
						}
						GSM.SetSMS_Buffer(&SMS_SendNumber[2], (char *)Tp.Buffer);
						SMS_Flag = TRUE;
						
					}
					else
					{
						SMS.SetPduDataMessage(SMS_UCS2, (char *)SMS_SendNumber,&DispRecBuffer[s1], Len); 	  
					}
					

					ID = SystemLog.GSM.SendSms;
					for(s1=0; s1<1000; s1++)
					{
						if(((SMS_Flag == FALSE) && (PDU_SMS.PDU_SendFlag == 0)) || 
							(SMS_Flag == TRUE) && (TextSMS.SendFlag == 0))
						  break;
						os_dly_wait(1);
					}

					if(SystemLog.GSM.SendSms>ID)	s = 0x17;
					else		  					s = 0x18;

					SendDataToLCM_YH(0x01,1,&s);
				
						
						
				} // to mobile
			}
			DataLen = 0;		
		break;

		//////////////////////////////////////////////// 
		case 0xac:			// 导航屏发给车机透传数据
			
			//GPRS.PushAlignment(DataLen+5,DispRecBuffer);	  // 不做处理，直接发送到监控中心
			LcmDataToServer = TRUE;
			s = WaitLcmDataToServer(500);	
			if(s==OK)
				Tp.Buffer[0] = 0x17;
			else
				Tp.Buffer[1] = 0x18;
			SendDataToLCM_YH(1,1,Tp.Buffer);
			DataLen = 0;
		break;   


		case 0xc0:		// 登录
			//tl = Str2Int((char*)&DispRecBuffer[LCM_FRAME_DATA_YH]);	 //
			//s = DriverLoginLogout(0,tl);
			if(s==OK)		Tp.Buffer[0] = 0x30;	
			else				Tp.Buffer[1] = 0x31;
			SendDataToLCM_YH(0x21,1,Tp.Buffer);
			DataLen = 0;
		break;

		case 0xc1:		// 退出
			//tl = Str2Int((char*)&DispRecBuffer[LCM_FRAME_DATA_YH]);	
		//	s = DriverLoginLogout(1,tl);    // 先注释 xzb
			if(s==OK)		Tp.Buffer[0] = 0x30;	
			else				Tp.Buffer[1] = 0x31;
			SendDataToLCM_YH(0x22,1,Tp.Buffer);
			DataLen = 0;
		break;

		case 0xd0:		// 紧急按钮
			
			VehicleStatus.VS6.Bits.CallHelpAlarm = 1;
			DataLen = 0;
		break;

		// added 2013.5
		case 0x51:			// 车辆注册
			JT808_UnLock_Terminal(&DispRecBuffer[LCM_FRAME_DATA_YH], DataLen);	 // 使用前锁定 解锁	
			DataLen = 0;
		break;
		//

		case 0x56:			// 装载状态

			if( DispRecBuffer[LCM_FRAME_DATA_YH]==0)		// 空载
			{
				JT808Flag.VS2.Bits.CarLoadStatus = 0;
				strcpy((char*)Tp.Buffer,"手动输入状态：空载  \0");
			}
			else if( DispRecBuffer[LCM_FRAME_DATA_YH]==1)	// 半载
			{
				JT808Flag.VS2.Bits.CarLoadStatus = 1;		
				strcpy((char*)Tp.Buffer,"手动输入状态：半载  \0");
			}
			else if( DispRecBuffer[LCM_FRAME_DATA_YH]==2)	// 满载
			{
				JT808Flag.VS2.Bits.CarLoadStatus = 3;
				strcpy((char*)Tp.Buffer,"手动输入状态：满载  \0");
			}
		  	JT808_GPRS_TempPack = TRUE;
			Uart.puts( (char*)Tp.Buffer);

		break;


		default:	DataLen = 0;		break;
	}

	if(DataLen)
	{
		SendDataToLCM_YH(Cmd,DataLen,Tp.Buffer);
	}
}

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			KeyDispCls::SendDataToLCM_YH(uchar Cmd,uint16 Len,uchar *pData)
{
	#if (1)//#if (WUXI_TEST==1)
	static uchar Busing = FALSE;
	#endif
	uchar		Xor;
	uint32	i=0;

	#if (1)//#if (WUXI_TEST==1)
	if (Cmd==eLCM_U_CENTER_TMP_MSG)
	{
		while (Busing==TRUE) {os_dly_wait(10); }
		Busing = TRUE;
		//  SendDataToLCM_YH的pData[0] 是 编码标志，所以跳过。
		if (ERROR==GSM.TTS_Opreation(Len-1,pData+1))
		{
			Uart.puts("TTS数据发送失败\r\n");
		}
	
		NOP();
		Busing = FALSE;
	}
	else			 
	#endif
	{	
		while(SendDataToLCM_Busing)	
		{ 
			os_dly_wait(1);	
			i++;
			if((i%50)==0)	Uart.puts("发送显示数据超时 ");
			if(i>300)
			{
				Uart2.Init(Baudrate.U2_232,U2_FOR_DISPLAY);	// 串口
				os_dly_wait(100);
				break;
			}
		}
		SendDataToLCM_Busing = TRUE;
	
		if(Len>(LEN_TO_HY_LCM_BUFFER-10))		Len = (LEN_TO_HY_LCM_BUFFER-10);			// DispBuffer最大长度300
	
		DispBuffer[0] = 0x0f;
		DispBuffer[1] = Cmd;
		DispBuffer[2] = HalfByteToHexChar(Len>>4);
		DispBuffer[3] = HalfByteToHexChar(Len&0x0f);
		if(Len)	
			memmove(&DispBuffer[4],pData,Len);
		Len += 3;
		Xor = CalXorSum(Len,&DispBuffer[1]);
		Len ++;
		DispBuffer[Len] = Xor;	Len++;
		DispBuffer[Len] = 0xf0;	Len++;
		DispBuffer[Len] = 0xff;	Len++;
	
		Uart2.putb(1,Len,DispBuffer);
		Uart2.SendTask();
		SendDataToLCM_Busing = 0;
	}
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			KeyDispCls::CenterSendDataToLCM_YH(uint16 Len,uchar *pData)
{
	uchar tBuffer[256];
	uchar i,j;  //tmp;


	if(U2PinSelDevice!=U2_FOR_DISPLAY)	return;		// 录音模块使用U2
	
	j = 0;
	tBuffer[j++] = 0x4E; //  号码
	// 20位中心号码
	memmove(tBuffer+j, "####", 4);
	j += 4;
	for (i=0; i<16; i++)	
	{
		tBuffer[j+i] = 0xFF;		
	}
	j += 16;
	//
	tBuffer[j++] = 0x54;   // 时间
	//
	Rtc.GetNowDateTimeString(0,&Rtc.DT, tBuffer+j);
	memmove(tBuffer+j, tBuffer+j+2, 12); // 去掉年份的“20” 如：2013，处理就变成：13
	for(i=0; i<12; i+=2)	  // 符合YH调度屏的时间顺序
	{
		tBuffer[j+i] ^= tBuffer[j+i+1];
		tBuffer[j+i+1] ^= tBuffer[j+i];
		tBuffer[j+i] ^= tBuffer[j+i+1];

	}
	
	j += 12;
	tBuffer[j++] = 0x47; // GB //0x43;  // 表示中文信息
	memmove(tBuffer+j, pData, Len);  // 信息内容
	Len += j;
	SendDataToLCM_YH(eLCM_U_CENTER_MSG, Len, tBuffer);


		
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
// 0F 0D 30 31 00 0C F0 FF
void			KeyDispCls::SendHeartDataToLCM_YH(void)
{
	uchar		tBuffer[16];
	//0F 0D 30 31 00 0C F0 FF
	tBuffer[0] = 0x0f;		tBuffer[1] = 0x0d;	tBuffer[2] = 0x30;
	tBuffer[3] = 0x31;		tBuffer[4] = 0x00;	tBuffer[5] = 0x0c;
	tBuffer[6] = 0xf0;		tBuffer[7] = 0xff;
	Uart2.putb(1,8,tBuffer);
	os_dly_wait(1);
	Uart2.SendTask();

	
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			KeyDispCls::SendGSMSignalToLCM(void)		  
{
	uchar tBuffer[24];

	// 网络商：中国移动编码
	memmove(tBuffer,(uchar *)"\x43\x54\x2D\x47\x53\x4D\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 20); 
	tBuffer[20] = 0x2C;  // 逗号分隔符	   下面tBuffer[21] 表示信号值及上线状态
	// 0x30 无信息,	 0x31 1格信息，... 0x36 6格信号，0x37 打开GPRS,上线
	if ((GG_Status.Signal < 5) || (GG_Status.Signal == 99)) tBuffer[21] = 0x30;  
	else if (GG_Status.Signal <= 8)	 tBuffer[21] = 0x31;
	else if (GG_Status.Signal <= 12)	 tBuffer[21] = 0x32;
	else if (GG_Status.Signal <= 15)	 tBuffer[21] = 0x33;
	else if (GG_Status.Signal <= 20)	 tBuffer[21] = 0x34;
	else if (GG_Status.Signal <= 25)	 tBuffer[21] = 0x35;
	else if (GG_Status.Signal <= 30)	 tBuffer[21] = 0x36;

	if (GG_Status.Flag.Bits.Online == 1)  tBuffer[21] = 0x37;

	SendDataToLCM_YH(eLCM_U_GMS_SIGNAL, 22, tBuffer);			
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
#if (0)
void			KeyDispCls::SendGpsDataToLCM(void)
{
	GPS_DATA_TO_LCM	Lcm;
	uchar		GpsData[32];	

	SetBufferValue((uchar*)&Lcm,LEN_GPS_DATA_TO_LCM,0);

	Lcm.Head = 0x24;
	Lcm.Cmd = eLCM_U_GPS_INFO;
	Lcm.Len	= 29;
	GPS.LCM_BCD_Pack(GpsData);
	memmove(Lcm.Time,GpsData+3,16);
	memmove(Lcm.Date,GpsData,3);

	
	if(SystemLog.Flag.Online == TRUE)
		Lcm.TypeStatus = 0x11;
	Lcm.CSQ = GG_Status.Signal;
	Lcm.Xor = CalXorSum(30,(uchar*)&Lcm);
	Lcm.End = 0x0a;

	Uart2.putb(1,32,(uchar*)&Lcm);
	os_dly_wait(1);
	Uart2.SendTask();	

	//Uart.putb(1,32,(uchar*)&Lcm);
		
}
#endif


#if (BSJ_LCM)
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：SendDataToLCM 
   功能描述： 发送数据给手柄
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			KeyDispCls::SendDataToLCM(uchar Cmd,uint16 Len,uchar *pData)
{
	uchar		Xor;
	static uchar			DBusing  = 0;

	if (U2PinSel !=U2_PIN_FOR_RS232)			 
	{
		return;
	}
	
	while(DBusing == TRUE) 	
	{ 
		Uart.puts("DBusing\n");
		Uart.putb(1, 1, &DBusing);

		os_dly_wait(1);	
	}
	//Uart.puts("BBB2");
	//os_dly_wait(1);

//	if (PhoneCall.RING == TRUE)
//	{
//		
//		Uart.puts("AAAAA");
//		os_dly_wait(1);
//	}

	DBusing = TRUE;

	DispBuffer[0] = 0x24;
	DispBuffer[1] = Cmd;
	DispBuffer[2] = Len+2;
	if(Len)	
		memmove(&DispBuffer[3],pData,Len);
	Len += 3;
	Xor = CalXorSum(Len,DispBuffer);
	DispBuffer[Len] = Xor;	Len++;
	DispBuffer[Len] = 0x0a;	Len++;

	Uart2.putb(1,Len,DispBuffer);	
	//os_dly_wait(1);
	Uart2.SendTask();	
	
	DBusing = 0;
}
#endif


uchar			KeyDispCls::WaitLcmDataToServer(uint16 WaitTick)
{
	uint16	i;
	for(i=0;i<WaitTick;i++)
	{
		os_dly_wait(1);
		if(LcmDataToServer==0)	return OK;
	}
	return ERROR;
}

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：CustomCmdProcess
   功能描述：使用调度屏的短信功能，增加自定义的指令，修改或读取主机参数
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
	(DispRecBuffer[LCM_FRAME_DATA]=='*')
	
	此类指令格式使用 *GY,Gxx,x1,x2,...#
	*GY,          固定字符串
	Gxx           指令码，“10” ~ "99"
	x1            参数1
	x2            参数2
	#				  结束符

*/


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uchar			KeyDispCls::CustomCmdProcess(void)
{
	uchar		s,Cmd;
	uchar		tBuffer[32];
	char		*pR;
	NOP();	NOP();
	s = ERROR;		// 先认为是错误的指令
	pR = MyStrStr((char*)&DispRecBuffer[LCM_FRAME_DATA],"*GY,G\0");
	if(pR != NULL)
	{
		SetBufferValue(tBuffer,32,0);
		Cmd = My_atol((char*)&DispRecBuffer[LCM_FRAME_DATA+5]);
		if((Cmd>9)&&(Cmd<100))   // 2位数命令字
		{
			switch(Cmd)
			{
				case 10:		  // 修改协议头，两个任意大字英文字母 *GY,G10,TH#
					tBuffer[0] = tBuffer[4] = '*';
					tBuffer[1] = tBuffer[5] = DispRecBuffer[LCM_FRAME_DATA+8];
					tBuffer[2] = tBuffer[6] = DispRecBuffer[LCM_FRAME_DATA+9];
					tBuffer[3] = tBuffer[7] = 0;

					if( (tBuffer[1]>='A')&&(tBuffer[1]<='Z')  &&  (tBuffer[2]>='A')&&(tBuffer[2]<='Z') )
					{
						s = OK;												
						DFE.Write(EE_PACK_HEAD,8,(uchar*)tBuffer);	
						sPackHead[0] = '*';
						sPackHead[1] = tBuffer[1];
						sPackHead[2] = tBuffer[2];
						sPackHead[3] = 0;					
					}
					
				break;	

				case 11:			// 测试指令 *GY,G11,U,2,A,102#
					
					TD.MainTask 	= 0;
					TD.GPS      	= 0;
					TD.GPRS     	= 0;
					TD.CAN		 	= 0;
					TD.SW       	= 0;
					TD.ADC      	= 0;
					TD.Display  	= 0;
					TD.Camera		= 0;
					TD.Value			= 0;	
					TD.TestMode		= 0;
					if(DispRecBuffer[LCM_FRAME_DATA+8]=='U')
					{
						Cmd = DispRecBuffer[LCM_FRAME_DATA+10];
						TD.Value = ( (0x55<<8)+DispRecBuffer[LCM_FRAME_DATA+12] );
						s = OK;
						switch(Cmd)
						{
							case 1:	TD.MainTask 	= TD.Value; 	break;							
							case 2:	TD.GPS      	= TD.Value;    break;
							case 3:	TD.GPRS     	= TD.Value;		break;	// 				
							case 4:	TD.CAN		 	= TD.Value; 	break;	// 
							case 5:	TD.SW       	= TD.Value;    break;
							case 6:	TD.ADC      	= TD.Value;		break;	
							case 7:	TD.Display  	= TD.Value;    break;
							case 8:	TD.Camera		= TD.Value;    break;
							default: 
								s = ERROR;
							break;
						}		
						TD.Value = My_atol( (char*)&DispRecBuffer[LCM_FRAME_DATA+14]  );
					}
				break;

				
				default:	break;
			}
		}
	}
	return s;
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：	 TPMS_ReceiveProcessData
   功能描述：	 轮胎气压数据处理
   入口参数：
   出口参数：
   全局变量: 
   创建时间：2014.04.16
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用	  
// 55 AA 08 00 00 3C 00 CB 55 AA 08 01 00 3C 08 C2 55 AA 08 10 00 3A 08 D5 55 AA 08 11 00 3B 00 DD 
void			KeyDispCls::TPMS_ReceiveProcessData(uint16 RecLen,uchar *pRecData)
{
	static uchar 	Index = 0, tCnt = 0;
	uchar	tBuffer[128+16], Num, i, Len;
	
	
//	if (TD.Display==tDISP_TEST_FOR_EXT)
//	{
//		Uart.puts("串口2接收轮胎数据 Test Data:");
//		Uart.putb(1,RecLen, pRecData);
//		Uart.puts("\r\n");
//	}

	tCnt++;

	if (tCnt==15)	// 1分钟发一次
	{

		tCnt = 0;
		
		Num = RecLen>>3; 		  // RecLen / 8	  每条"55 aa" 开始的数据为8个字节,即每个轮胎数据为8个字节
		Len = 0;
		tBuffer[Len++] = Num;				// 车轮个数
		tBuffer[Len++] = Index++;		   // 流水号
		// 车辆类型 	 小车，每条数据8字节，	// 0:小车， 1：大车
		if ((RecLen%8) == 0)			  //  
		{
			tBuffer[Len++] = 0; 	
		}
		else
		{
			tBuffer[Len++] = 1; 
		}
	
	   tBuffer[Len++] = 0; // 保留
	
		for (i=0; i<Num;i++)
		{
			memmove(&tBuffer[Len+i*4],&pRecData[i*8+3],4);
		}
		Len += Num * 4;
	
		GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TPMS_DATA_UPLOAD,Len,tBuffer);
	
		}	
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			KeyDispCls::ReceiveProcessData(void)
{
	
}






/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */  
