/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 
$								  		All rights reserved.	  
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


#define			BSJ_LCM			0       // 0 ����ʹ�ò�˼�ܵ�����

#define			LEN_TO_HY_LCM_BUFFER		300

extern     	uchar   jt_dis_reg_flag;
extern 		uchar		jt_auth_ok_flag; 

uchar			DispLock1IPAddr;

uchar			DispBuffer[LEN_TO_HY_LCM_BUFFER];// 	__attribute__((at(0x7fe03800)));
char 			Create_User_Dns[LEN_IPorDNS] = {"jt1.gghypt.net\0\0\0\0\0\0"};
const   		uint32	CreateUserDnsPort = {0x000003F0};		   // 1008

extern		uchar		WaitIC_CardAuthTime,IC_CardAuthFailure,OnlyUseBD_GPS;

static uchar ConfirmCnt = 0;		 // ��Ϊ��סȷ�ϼ�������һ������3�ξʹ�ӡ������Լ3�봥����ӡ

//uchar			InputStatus;

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�TaskKeyDisp
   ����������
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������



__task		void			TaskKeyDisp(void)	
{
	NOP();	NOP();	NOP();	NOP();		
	KeyDisp.Init();
	DispLock1IPAddr = 0;
	NOP();	NOP();	NOP();	NOP();	
	OLED.PowerOnTiming();	
	//os_dly_wait(100);
	OLED.Init();
	Uart.puts(" OLED�ϵ� ");
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
	if ((GG_Status.Flag.Bits.UpdateRemoteSet ==1)||(U0_Iap_Flag!=0)) return;	  // Զ������


	//if( (DispCnt%500==0)&&(PrintBusy==FALSE)&&(IDEL_For_Acc_OFF!=TRUE))	    //60�����³�ʼ��һ��OLED
	if( (DispCnt==100)||(DispCnt%18000==0) )	// 3���ӳ�ʼ��һ��
   {
		OLED.Init();
		#if (WUXI_TEST != 1)
		if(U0_Iap_Flag==0)	Uart.puts(" �ٴγ�ʼ��OLED ");
		#endif
	   NOP();	NOP();
		if(DispCnt==100)	SystemSelfTest = TRUE;
	}	

	if(TD.MainTask==tTASK_RUN_FLAG)
	{
		if(DispCnt%100==0)		Uart.puts("KeyDisp task run\r\n");
	}


	NOP();	NOP();	NOP();	NOP();
	if ((gCnt.SystemSecond > 10))// && ((DispCnt%20)==0))				 // 10s ����
		Adc.eTaskADC();

	NOP();	NOP();
	
	NOP();	NOP();	NOP();	NOP();
   if(uLED.MainMenuCnt != PRINT_MENU)	
	  LED_Buzzer();
	NOP();	NOP();	NOP();	NOP();
	if(U0_Iap_Flag==0)
	   KeyProcess();
	NOP();	NOP();	NOP();	NOP();
	if((IDEL_For_Acc_OFF==TRUE)&&(gCnt.SystemSecond>60))			 //ACCʡ��ģʽ
	{		 
	   //LCM_POWER_OFF;
		LCM_BL_POWER_OFF;  //Ӧ�����ԣ����ر���ʾ��ֻ�ر�OLED ���� 
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
		if( U2PinSelDevice != U2_FOR_DISPLAY  )		return;		//  ¼����RFIDʹ�ô���


		NOP();	NOP();
		
		EXT_5V_POWER_ON;
		Uart2.ReceiveProcessData();
		if((TD.Display==tDISP_TEST_FOR_EXT)&&(DispCnt%300==0))	Uart.puts(" ���յ�������Ϣ ");
		
		
		NOP();	NOP();	NOP();	NOP();
		//////////////////   U0��U1͸������ʱ���������ݵ���ʾ��		
		if(DispCnt%300==0)	// GPS����	  ������   ��GSM״̬
		{
			SendGSMSignalToLCM();
			if(TD.Display==tDISP_TEST_FOR_EXT)	Uart.puts(" ������������ ");
			
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
	�������ƣ�eTaskDisp
   ����������LCM��ʾ����
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: �ṹLCM
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
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
	if((++KEY_DL.LongTimeNoKey>40*5)||((uLED.MainMenuCnt==PRINT_MENU)&&(PrintBusy==FALSE)&&(PrintEvent1==0)))		 // 20����û�������ӡ����������Ĭ����Ļ��ʾ	
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
	//// ��ʼ����ʾ����
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
		InitDispCNT = (3*25);		// ��ֹ��ʼ��������ʾ


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
		// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  ���˵�1
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
				strncpy((char *)&OLED_Buffer[0][0],"                ",16);			  //���ڵ�һ����˸
			}
			else
			{
				TwinkleFlag = 1;
			}
			break;

		/// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ��������
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


			if (uLED.PasswordOK == TRUE)	 // ������ȷ��˵���Ѿ�������룬��Ҫ��˸���Ӳ˵� 
			{

				if(TwinkleFlag)
				{
					TwinkleFlag = 0;
					strncpy((char *)&OLED_Buffer[0][0],"                ",16);			  //���ڵ�һ����˸
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
						strncpy((char *)&OLED_Buffer[0][0],"                ",16);			  //���ڵ�һ����˸
					}
					else
					{
						TwinkleFlag = 1;
					}
				
			}

			break;

		
		case	INPUT_MENU:				//����˵�	(ͨ��ѡ�������룩	���ڲ�������

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
						if (i > uLED.Offset) 	c = SetParaBuffer[uLED.Offset];	 // ʵ�������ַ����ĳ������С�ڹ���ƫ�ƣ����ù�����ڵ��ַ���˸
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


		case NotAllowChangeDriver_MENU:									//��������ļ�ʻԱ
			strncpy((char *)&OLED_Buffer,(const char *)&CannotChangeDriverSCR,16);	
			strncpy((char *)&OLED_Buffer[1][0],(const char *)&CannotChangeDriverSCR[1][0],16);
			break;
		/// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ��������ϵ��
		case VEHICLE_CC_MENU:		// 
			strncpy((char *)&OLED_Buffer,(const char *)&VehicleCC_SCR,16);	
			SetBufferValue( &OLED_Buffer[1][0],4,0x20);
			SetBufferValue( &OLED_Buffer[1][4],12,0);
			DFE.Read(FM_VEHICLE_PULSE_KV,4,(uchar *)&t);			
			t &= 0x0000ffff;
			Int2Str( t, (char*)&OLED_Buffer[1][4]);	
			break;
		/// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ƣ�ͼ�ʻ��¼
		case OVER_TIME_MENU:			

			/*if(uLED.PageCnt>40)	 	// ���40ҳ
				uLED.PageCnt = 0;
			if(uLED.PageCnt==0)		// ��������ʱ����ǰ
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
				OLED_Buffer[0][8] = (uLED.PageCnt+2)/2/10 + '0';				   //��X����¼		�������·�������ʾ��һ��ƣ�ͼ�ʻ��¼ ��������Ϊż��ֻ����ʾ�����ͼ�ʻԱ����
				OLED_Buffer[0][9] = (uLED.PageCnt+2)/2%10 + '0';	
			}
			else
			{
			   uLED.PageCnt = 0;
				strncpy((char *)&OLED_Buffer,(const char *)&NoTireTimeSCR,16);				//��ƣ�ͼ�ʻ��¼
				strncpy((char *)&OLED_Buffer[1][0],(const char *)&NoTireTimeSCR[1][0],16);	
				break;			
			}			
			// ��ȡһ��ƣ�ͼ�ʻ��Ϣ��������ʻ֤�ź�����ʱ��	
			SetBufferValue((uchar *)Tp.Buffer,30,0);		
			if((uLED.PageCnt/2<=DispOVT_CNT)&&(DispOVT_CNT!=0))				   //ƣ�ͼ�¼�����Ͱ�������С�ڵ���ƣ�ͼ�¼������Ϊ����ƣ�ͼ�ʻ��¼
			{
				if(OVT_AddrTab[uLED.PageCnt/2]<8192)  // ȡ�����
				  DFE.Read(OVT_AddrTab[uLED.PageCnt/2],LEN_SAVE_FATIGUE_DRIVING_INFO,Tp.Buffer);
				else
				{
				  for(i=0;i<1000;i++)				// �ȴ���� (������while),10s
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
				Tp.Buffer[0] = Tp.Buffer[1] = Tp.Buffer[2] = 0;				      //�ü�ʻ֤����Ч
			}
			NOP();	NOP();	
            //Tp.Buffer[] = {"123456789012345678110722123800110722175500\0"};	  	  //��������		
			
			/////////////////////////////// ��ʻ֤�Ŵ���
			if(uLED.PageCnt%2==0)
			{
					strncpy((char *)&OLED_Buffer[0][12],(char *)Tp.Buffer,4);		// ��ʻ֤��
					strncpy((char *)&OLED_Buffer[1][0],(char *)&Tp.Buffer[4],14);
			}
			else
			/////////////////////////////// ����ʱ�䴦��			
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
						if(OLED_Buffer[i][j] == 'x')		 //����ʾ��
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
				OLED_Buffer[0][8] = (uLED.PageCnt/10) + '0';				   //��X����¼		�������·�������ʾ��һ��ƣ�ͼ�ʻ��¼ ��������Ϊż��ֻ����ʾ�����ͼ�ʻԱ����
				OLED_Buffer[0][9] = ((uLED.PageCnt+1)%10) + '0';

				strncpy((char *)&OLED_Buffer[2][0],(const char *)&TireTimeSCR[2][0],16);
			   strncpy((char *)&OLED_Buffer[3][0],(const char *)&TireTimeSCR[3][0],16);
			}
			else
			{	
			   uLED.PageCnt = 0;								 
				strncpy((char *)&OLED_Buffer,(const char *)&NoTireTimeSCR,16);				//��ƣ�ͼ�ʻ��¼
				strncpy((char *)&OLED_Buffer[1][0],(const char *)&NoTireTimeSCR[1][0],16);	
				break;			
			}			
			// ��ȡһ��ƣ�ͼ�ʻ��Ϣ��������ʻ֤�ź�����ʱ��	
			SetBufferValue((uchar *)Tp.Buffer,50,0);		
			if(uLED.PageCnt<DispOVT_CNT)				   //
			{
				if(OVT_AddrTab[uLED.PageCnt]<8192)  // ȡ�����
				  DFE.Read(OVT_AddrTab[uLED.PageCnt],LEN_SAVE_FATIGUE_DRIVING_INFO,Tp.Buffer);
				else
				{
				  for(i=0;i<1000;i++)				// �ȴ���� (������while),10s
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
				Tp.Buffer[0] = Tp.Buffer[1] = Tp.Buffer[2] = 0;				      //�ü�ʻ֤����Ч
			}
			/////////////////////////////// ��ʻ֤�Ŵ���
		
			strncpy((char *)&OLED_Buffer[0][12],(char *)Tp.Buffer,4);		// ��ʻ֤��
			strncpy((char *)&OLED_Buffer[1][0],(char *)&Tp.Buffer[4],14);
		
			/////////////////////////////// ����ʱ�䴦��			
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
				//c += 2; // ������ //����ʾ��
			}

			
			break;			
			
		/// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$                 15�����ڵڷ���ƽ������				 ȡ��������ʾ
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
		/// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ���ټ�¼��ʾ					         ȡ��������ʾ
		case OVER_SPEED_MENU:

		/// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ��ǰ��ʻԱ��Ϣ
		case DRIVER_INFO_MENU:		// 
				
			   if((DriverCardFlag==TRUE))																					//�Ƿ��ʻ��	 &&(StopFlag==1)
		      {
				
					if(uLED.PageCnt>1)		uLED.PageCnt = 0;
				
				               
					  if(uLED.PageCnt==0)
					  {
						  strncpy((char *)&OLED_Buffer,(const char *)&DriverInfoSCR[0][0],16);
					  	  strncpy((char *)&OLED_Buffer[1][0],(const char *)&DriverInfoSCR[1][0],16);
						  DFE.Read(EE_JT_DRIVER_NAME_LEN,1,(uchar *)&t); 	// ��������
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
					 strncpy((char *)&OLED_Buffer[0][0]," �޼�ʻԱ��Ϣ�� ",16);
					 strncpy((char *)&OLED_Buffer[1][0],"                ",16);
				 }
			  break;
			
			case  Car_MSG_MENU:				//������Ϣ
				 if(uLED.PageCnt<3)
			    {
					i = 0;
				   if(uLED.PageCnt!=0)
					 i = uLED.PageCnt*2;
				   strncpy((char *)&OLED_Buffer[0][0],(const char *)&CarMSGSCR[i][0],16);
				   strncpy((char *)&OLED_Buffer[1][0],(const char *)&CarMSGSCR[i+1][0],16);
				   DFE.Read(FM_VEHICLE_CODE,LEN_VEHICLE_VCC,Tp.Buffer);				  //����ַ��Ϊ�����ݵ�ַ���� 
					 
				   if(uLED.PageCnt==0)
				   {
				     strncpy((char *)&OLED_Buffer[0][5],(const char *)&Tp.Buffer[0],12);					  //���ƺ�
					  strncpy((char *)&OLED_Buffer[1][5],(const char *)&Tp.Buffer[29],10);					  //��������
				   }
				   else if(uLED.PageCnt==1)
				   {
				     //DFE.Read(FM_VEHICLE_VIN,LEN_VEHICLE_VCC,Tp.Buffer);				  //	  VIN��
				  	  strncpy((char *)&OLED_Buffer[0][6],(const char *)&Tp.Buffer[12],10);
					  strncpy((char *)&OLED_Buffer[1][0],(const char *)&Tp.Buffer[22],7);
				   }
					else if (uLED.PageCnt==2)				// ������ɫ
					{
						DFE.Read(FM_CAR_COLOUR,1,(uchar *)&t);
						//if ((t>6)) t = 0;
						if ((t>6)) t = 1;
						Int2Str(t,(char *)&OLED_Buffer[0][13]);			// ������ɫ����

						//memmove(&OLED_Buffer[1][9],&InputSelectColor[(t-1)*2],2);		//  ������ɫ
						memmove(&OLED_Buffer[1][9],&InputSelectColor[t*2],2);		//  ������ɫ

					}
				   
				  }
				  else
				     uLED.PageCnt = 0;
			
			  break;
			case Mobile_PARA_MENU:  	  //����ͨ�Ų���
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
				 	DFE.Read(EE_JT_M_IP_DNS,16,Tp.Buffer);				  //����ַ��Ϊ�����ݵ�ַ����	�����ַ1
				  	strncpy((char *)&OLED_Buffer[1][0],(const char *)&Tp.Buffer,16);

					DFE.Read(EE_JT_B_IP_DNS,16,Tp.Buffer);				  //����ַ��Ϊ�����ݵ�ַ����	�����ַ2
				  	strncpy((char *)&OLED_Buffer[3][0],(const char *)&Tp.Buffer,16);
				 }
				 else if(uLED.PageCnt==1)
				 {
					DFE.Read(EE_JT_M_APN,16,Tp.Buffer);				  //����ַ��Ϊ�����ݵ�ַ����	APN
				  	strncpy((char *)&OLED_Buffer[1][0],(const char *)&Tp.Buffer,16);

					DFE.Read(EE_JT_TCP_PORT,4,(uchar *)&t);				  //����ַ��Ϊ�����ݵ�ַ����	TCP�˿�
					SetBufferValue(Tp.Buffer, 6, 0);
					Int2Str(t,(char*)&Tp.Buffer[0]);
				  	strncpy((char *)&OLED_Buffer[2][11],(const char *)&Tp.Buffer, strlen((char *)Tp.Buffer));
					DFE.Read(EE_JT_UDP_PORT,4,(uchar *)&t);				  //����ַ��Ϊ�����ݵ�ַ����	UDP�˿�
					SetBufferValue(Tp.Buffer, 6, 0);
					Int2Str(t,(char*)&Tp.Buffer[0]);
					strncpy((char *)&OLED_Buffer[3][11],(const char *)&Tp.Buffer,strlen((char *)Tp.Buffer));
				 }
				 else if(uLED.PageCnt==2)
				 {
				  	DFE.Read(EE_SIM_NUMBER,16,Tp.Buffer);				  //����ַ��Ϊ�����ݵ�ַ����	  �ն�SIM������  �����ֻ�����
				  	strncpy((char *)&OLED_Buffer[1][0],(const char *)&Tp.Buffer[1],15);

					DFE.Read(EE_JT_CAR_OWNER_PHONE_NUM,16,Tp.Buffer);				  //����ַ��Ϊ�����ݵ�ַ����	 �����ֻ�����
				   strncpy((char *)&OLED_Buffer[3][0],(const char *)&Tp.Buffer,16);
				 }
				 else if(uLED.PageCnt==3)
				 {
				    DFE.Read(EE_JT_CENTER_NUMBER,16,Tp.Buffer);				  //����ַ��Ϊ�����ݵ�ַ����	  ƽ̨����
				  	 strncpy((char *)&OLED_Buffer[1][0],(const char *)&Tp.Buffer,16);
					 DFE.Read(EE_JT_TEXT_SMS_NUMBER,16,Tp.Buffer);				  //����ַ��Ϊ�����ݵ�ַ����	  SMS�ı���������
				  	 strncpy((char *)&OLED_Buffer[3][0],(const char *)&Tp.Buffer,16);
				 }
				 else if(uLED.PageCnt==4)
				 {
				 	SetBufferValue(Tp.Buffer,16,0);		  
					if( CreateUserFlag!=OK)
					{
						strcpy((char *)Tp.Buffer, (char *)"δ����");
					}	
					else
					{
						strcpy((char *)Tp.Buffer, (char *)"�ѿ���");
					}																		//����ַ��Ϊ�����ݵ�ַ����		����״̬
				  	strncpy((char *)&OLED_Buffer[1][0],(const char *)&Tp.Buffer,16);

					if((JT808Flag.VS3.Bits.UseGPS==1)&&(JT808Flag.VS3.Bits.UseBD==1))
						memmove( &OLED_Buffer[3][0],(const uchar*)"ʹ��GPS/����˫ģ",16);
					else if(JT808Flag.VS3.Bits.UseGPS==1)
						memmove( &OLED_Buffer[3][0],(const uchar*)" ��ʹ��GPS��λ  ",16);
					else if(JT808Flag.VS3.Bits.UseBD==1)
						memmove( &OLED_Buffer[3][0],(const uchar*)" ��ʹ�ñ�����λ ",16);
					else
						memmove( &OLED_Buffer[3][0],(const uchar*)" δʹ���κ����� ",16);
				  
				 }
				 else if ((uLED.PageCnt>=5) || (uLED.PageCnt <=7))
				 {
				 	if (uLED.PageCnt==5)
					{
						strncpy((char *)&OLED_Buffer[0][0],(const char *)&MobileCMNCSCR[uLED.PageCnt*4][0],16);
					 	SetBufferValue( &OLED_Buffer[1][0],16,0);
					 	Int2Str( SendInterval.DefauletTime,(char*)&OLED_Buffer[1][0]);
					 	i = strlen( (char*)&OLED_Buffer[1][0]);
					 	memmove( &OLED_Buffer[1][i],(const uchar*)"��",2);

						DFE.Read(EE_PARA_BLOCK_STATUS,16,Tp.Buffer);
				 		SetBufferValue( &OLED_Buffer[3][0],16,0);
				 		memmove( &OLED_Buffer[2][0],(uchar*)"����������1:",12);
				 		SetBufferValue( &OLED_Buffer[3][0],16,0);	

				 		if(Tp.B16[0] == PARA_BLOCK_CONFIG_FLAG)
				 			strcpy((char*)&OLED_Buffer[3][0],"������\0");
				 		else
				 			strcpy((char*)&OLED_Buffer[3][0],"δ����\0");
					}
					else  if(uLED.PageCnt==6)
					{
					  	DFE.Read(EE_PARA_BLOCK_STATUS,16,Tp.Buffer);
					 	SetBufferValue( &OLED_Buffer[1][0],16,0);
					 	memmove( &OLED_Buffer[0][0],(uchar*)"����������2:",12);
					 	SetBufferValue( &OLED_Buffer[1][0],16,0);	
					
					 	OLED_Buffer[0][11] = '2';
					 	if(Tp.B16[1] == PARA_BLOCK_CONFIG_FLAG)
				 			strcpy((char*)&OLED_Buffer[1][0],"������\0");
				 		else
				 			strcpy((char*)&OLED_Buffer[1][0],"δ����\0");

						memmove( &OLED_Buffer[2][0],(uchar*)"����������3:",12);
						SetBufferValue( &OLED_Buffer[3][0],16,0);
					 	if(Tp.B16[2] == PARA_BLOCK_CONFIG_FLAG)
				 			strcpy((char*)&OLED_Buffer[3][0],"������VDR����\0");
				 		else
				 			strcpy((char*)&OLED_Buffer[3][0],"δ����VDR����\0");
					}
		
					////
				 }
				 else 
				  uLED.PageCnt = 0;


			break;
	      case Schedul_MSG_MENU:		  //������Ϣ
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
					strncpy((char *)&OLED_Buffer[0][0],"                ",16);			  //���ڵ�һ����˸
		       }
			 	 else
				 {
					TwinkleFlag = 1;
				 }
			 
			break;

			case Set_Load_OK_Menu:																	 //���ó������سɹ�
			 strncpy((char *)&OLED_Buffer[0][0],(const char *)&SchedulMSGSCR[6][0],16);
			 strncpy((char *)&OLED_Buffer[1][0],(const char *)&SchedulMSGSCR[7][0],16);


	      break;
			case Set_Fales_Menu:																	  //���ó�������ʧ��
			 strncpy((char *)&OLED_Buffer[0][0],(const char *)&SchedulMSGSCR[8][0],16);
			 strncpy((char *)&OLED_Buffer[1][0],(const char *)&SchedulMSGSCR[9][0],16);
		 	  
			
			
			break;
	      case Phone_Book_MENU:  	     //�绰��
			 
			 strncpy((char *)&OLED_Buffer,(const char *)&PhoteBookSCR[0][0],16); 
			 strncpy((char *)&OLED_Buffer[1][0],(const char *)&PhoteBookSCR[1][0],16);


			break;
	      case Milage_MENU:				  //360Сʱ���	���������������


			if(uLED.PageCnt==0)			  // ����ͳ�������
		   {	
			 	memmove( &OLED_Buffer,(const char *)&MilageSCR[0][0],16);
				SetBufferValue(Tp.Buffer,16,0);
				Int2Str(SystemLog.VDR.SpeedSensorPulsCnt/VehiclePulseKV,(char *)Tp.Buffer);
				strcat( (char *)Tp.Buffer,".");
				Tp.Buffer[strlen((char*)Tp.Buffer)]=(((SystemLog.VDR.SpeedSensorPulsCnt%VehiclePulseKV)*10)/VehiclePulseKV)+'0';
				strcat( (char *)Tp.Buffer,"km");
				memmove(&OLED_Buffer[1][0],&Tp.Buffer,16); 
			 }
			 else if(uLED.PageCnt==1)		 		// GPSͳ��
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

			case  Ctr_MENU:   //�ն˿���
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
				strncpy((char *)&OLED_Buffer[0][0],"                ",16);			  //���ڵ�һ����˸
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
					strncpy((char *)&OLED_Buffer[1][0],"                ",16);			  //���ڵڶ�����˸
		       }
			 	 else
				 {
					TwinkleFlag = 1;
				 }

			break; */


			case  Comfir_Logout_MENU:	  //ȷ��ע���˵�
			    strncpy((char *)&OLED_Buffer[0][0],(const char *)&SetParaSCR[SET_PARA_COMFIR+0][0],16);
		    	 strncpy((char *)&OLED_Buffer[1][0],(const char *)&SetParaSCR[SET_PARA_COMFIR+1][0],16);
		   break;
	      case  Logouting_MENU: 		  //����ע���˵�
				 strncpy((char *)&OLED_Buffer[0][0],(const char *)&SetParaSCR[SET_PARA_COMFIR+2][0],16);
		    	 strncpy((char *)&OLED_Buffer[1][0],(const char *)&SetParaSCR[SET_PARA_COMFIR+3][0],16);
			break;      
	      case  Logout_OK_MENU:		  //ע���ɹ��˵�
				 strncpy((char *)&OLED_Buffer[0][0],(const char *)&SetParaSCR[SET_PARA_COMFIR+4][0],16);
		    	 strncpy((char *)&OLED_Buffer[1][0],(const char *)&SetParaSCR[SET_PARA_COMFIR+5][0],16);
				 KEY_DL.LongTimeNoKey = 20*5;
			break;   
			case  Logout_Fales_MENU:		  //ע��ʧ�ܲ˵�
				 strncpy((char *)&OLED_Buffer[0][0],(const char *)&SetParaSCR[SET_PARA_COMFIR+6][0],16);
		    	 strncpy((char *)&OLED_Buffer[1][0],(const char *)&SetParaSCR[SET_PARA_COMFIR+7][0],16);
				 KEY_DL.LongTimeNoKey = 20*5;
			break;
         case  Logout_Fales_MENU1:		  //��������Ȩ����ʾ�˵�
				 strncpy((char *)&OLED_Buffer[0][0],(const char *)&SetParaSCR[SET_PARA_COMFIR+8][0],16);
		    	 strncpy((char *)&OLED_Buffer[1][0],(const char *)&SetParaSCR[SET_PARA_COMFIR+9][0],16);
				 KEY_DL.LongTimeNoKey = 20*5;
			break;


	      case Product_MSG_MENU:		  //��Ʒ�汾��Ϣ
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
					memmove( (uchar *)&OLED_Buffer[1][0], (uchar*)sOEM_ID, t );						// ���		
					
					strncpy((char *)&OLED_Buffer[3][0],(const char *)&Tp.Buffer[MV_POS_MODEL],LEN_MODEL); 			// �ͺ�
					 		
			 }
			 else if(uLED.PageCnt==1)
			 {
			 	  strncpy((char *)&OLED_Buffer[1][0],(const char *)&Tp.Buffer[MV_POS_HV],5);			// Ӳ���汾	
				  strncpy((char *)&OLED_Buffer[3][0],(const char *)&Tp.Buffer[MV_POS_SV],5);			// ����汾
			 }
			 else if(uLED.PageCnt==2)
		    {
				strncpy((char *)&OLED_Buffer[1][0],(const char *)&Tp.Buffer[BUILD_DATE_POS],11);		// �������� 
				strncpy((char *)&OLED_Buffer[3][0],(const char *)&Tp.Buffer[BUILD_TIME_POS],8);			// ����ʱ��
		    }
			 else 
				  uLED.PageCnt = 0;

		  break;
		  
		/// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ��ӡ  
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
		case CONFIRM_MENU:			//	  ����������ݼ�����ȱʡ���棬���Ϸ���������˴�
		  
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
			  Rtc.GetNowDateTimeString(1,&Rtc.DT, Tp.Buffer);   //����
			  memmove(&OLED_Buffer[0][5], Tp.Buffer, 10);
			  /////////////////////�ٶ�
			  OLED_Buffer[1][5]   = i/100 + 0x30;
			  OLED_Buffer[1][6]   = i%100/10 + 0x30;
			  OLED_Buffer[1][7]   = i%100%10 + 0x30;
		  }
		  else if(uLED.PageCnt==1)
		  {
		  	  /////////////////////�ٶ�
		
			  OLED_Buffer[0][5]   = i/100 + 0x30;
			  OLED_Buffer[0][6]   = i%100/10 + 0x30;
			  OLED_Buffer[0][7]   = i%100%10 + 0x30;
		  
		  	  /////////////////////���
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

		  // ���� ��λΪ��
		  t = (GPS.GYPack.Longitude/(10000000UL) );
		  Int2Str(t,(char *)Tp.Buffer);
		  strcat((char *)Tp.Buffer,".");
		  i= strlen((char *)Tp.Buffer);
		  t = ((GPS.GYPack.Longitude/100)%(100000UL) );
		  Int2Str(t,(char *)&Tp.Buffer[i]);
		  strcat((char *)Tp.Buffer,"��");
		  i= strlen((char *)Tp.Buffer);
		  memmove(&OLED_Buffer[0][5],Tp.Buffer,i);
		   // γ�� ��λΪ��
		  t = (GPS.GYPack.Latitude/(10000000UL) );
		  Int2Str(t,(char *)Tp.Buffer);
		  strcat((char *)Tp.Buffer,".");
		  i= strlen((char *)Tp.Buffer);
		  t = ((GPS.GYPack.Latitude/100)%(100000UL) );
		  Int2Str(t,(char *)&Tp.Buffer[i]);
		  strcat((char *)Tp.Buffer,"��");
		  i= strlen((char *)Tp.Buffer);
		  memmove(&OLED_Buffer[1][5],Tp.Buffer,i);
		  // ���� ��
		  Int2Str(GPS_Altitude,(char *)&OLED_Buffer[2][5]);
		  strcat((char *)&OLED_Buffer[2][5],(char *)"��");

		  // �ٶ�
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
				memmove((char *)&OLED_Buffer[1][0],"��ʻԱ��֤:     ",16);
				OLED_Buffer[3][12] = WaitIC_CardAuthTime/10+'0';
				OLED_Buffer[3][13] = WaitIC_CardAuthTime%10+'0';				
			}
			else if(IC_CardAuthFailure)		// ʧ����ʾ
			{
				if(IC_CardAuthFailure==1)		// 
				{
					memmove((char *)&OLED_Buffer[3][0],"��֤ʧ�ܣ��ܳ״�",16);
				}
				else if(IC_CardAuthFailure==2)	
				{
					memmove((char *)&OLED_Buffer[3][0],"��֤ʧ�ܣ�������",16);
				}
				else if(IC_CardAuthFailure==3)	
				{
					memmove((char *)&OLED_Buffer[3][0],"��֤ʧ�ܣ����γ�",16);
				}
				else if(IC_CardAuthFailure==4)	
				{
					memmove((char *)&OLED_Buffer[3][0],"��֤ʧ�ܣ�У���",16);
				}
				else //if(IC_CardAuthFailure==3)	
				{
					memmove((char *)&OLED_Buffer[3][0],"  ��������Ӧ��  ",16);
				}
			}
			else if((JT808Flag.VS5.Bits.OverSpeedAlarm == 1))  /////////////////// ���ټ�ʻ
			{
			   strncpy((char *)&OLED_Buffer[3][0],(const char *)&DefaultScreen[5][0],16);
			}
			else	if((JT808Flag.VS5.Bits.FatigueDriving == 1)&&((uiSpeed>50)||(GPS_Speed>5)))  /////////////////// ƣ�ͼ�ʻ
			{
			   strncpy((char *)&OLED_Buffer[3][0],(const char *)&DefaultScreen[4][0],16);
			}
			else if ((JT808Flag.VS6.Bits.FatiguePreAlarm == 1)&&((uiSpeed>50)||(GPS_Speed>5))) // ƣ��Ԥ��
			{
				 strncpy((char *)&OLED_Buffer[3][0],(const char *)&DefaultScreen[6][0],16);
			}
			else
			{
			  if((DriverCardFlag==TRUE))																					//�Ƿ��ʻ��	 &&(StopFlag==1)
		      {
					strncpy((char *)&OLED_Buffer[3][0],(const char *)&DefaultScreen[2][0],16);
					DFE.Read(EE_JT_DRIVER_NAME_LEN,1,(uchar *)&t); 	// ��������
					if(t>8)	t = 8;
					DFE.Read(EE_JT_DRIVER_NAME,t,&OLED_Buffer[3][7]);
				}
				else if ((AlertLogonTime>0)&&(AlertLogonTime<=(30*60)))	  // ��ʾ��½ʱ�䲻Ϊ0��˵�����ٶȣ�30��������ʾ
				{
					if ((gCnt.InsertIC_Card)&&(StopFlag==0))
					{
						strcpy((char *)&OLED_Buffer[3][0],(const char *)"����ͣ��,�ٲ忨!");
					}
					else
					{
						strncpy((char *)&OLED_Buffer[3][0],(const char *)&DefaultScreen[3][0],16);
					}
				}
			}

		   //////////////////////////////////////////	Speed
			strncpy((char *)&OLED_Buffer[2][0],(const char *)&DefaultScreen[1][0],16);

			//�ٶ�״̬
			if (SpeedStatusJudge.Status==SPEED_STATUS_UNNORMAL)
			{
				memmove((uchar *)&OLED_Buffer[2][8],(uchar *)"�ٶ��쳣",8);	
			}
							// ����ʾС��			
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
			{	  //  �ٶ��쳣���������ٶȣ���λ����ĻҪ�г�GPS�ٶ�
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
	

		    /////////////////////////////////////////GPRS �Ƿ�����
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
			 /////////////////////////////////////////GSM �ź�ǿ��
			if((GG_Status.Signal>2)&&(GG_Status.Signal<32))
			  OLED_Buffer[0][1]   = (GG_Status.Signal+4)/6 + 0xA1 + UseWthSimCard;					  //��5����ʾ�źŷ���

			/////////////////////////////////////////GPS ���Ǹ���
			if(NumberOfSatlice!=0)
		   {
		     OLED_Buffer[0][4]   = NumberOfSatlice>>8;
			  OLED_Buffer[0][5]   = NumberOfSatlice;
		    
			  if(OLED_Buffer[0][4]=='0')
		        OLED_Buffer[0][4] = ' ';
		   }

			//////////////////////////////////////		��λ��־
			if(VirtaulGPS_Valid==TRUE)
				OLED_Buffer[0][3] = 'A';
			else
				OLED_Buffer[0][3] =  (GPSValidFlag == GPS_VALID_FLAG) ? 'A':'V';
			
			Rtc.GetLocalTimeString(1,&OLED_Buffer[0][8]);	  // ʱ��
		
			SetBufferValue(&OLED_Buffer[1][0],16,0);
			Rtc.GetNowDateTimeString(1,&Rtc.DT, Tp.Buffer);   //����
			memmove(&OLED_Buffer[1][3], Tp.Buffer, 10);
			
			
			///////////////////////////////////////// ͨѶ��־
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
				strncpy((char *)&OLED_Buffer[1][0],"                ",16);			  //���ڵڶ�����˸
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
	�������ƣ�DispScreen
   ������������ʾһ���ַ�
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: �ṹLCM
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
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
	for(i=0,Page = 0;i<E308_TOTAL_CN_LINE;i++)					   // 2�к���
	{
		for(j=0,n=0;j<16;j++)									         // ÿ��8������(�ϰ벿��)
		{	
			xc = OLED_Buffer[i][j];

			if(xc==0)  xc = 0x20;
			  
			if((xc<=0x7F)&&(xc>=0x20))									   //�ж��Ƿ�ASCII��
			{
				ASCII_pCode = ASCII_Code + (xc - 0x20)*16;  		   //ASCII���ַ
			   t = 8;												         //Ҫȡ�ĵ���ΪASCII��
			   GetASCIIDotCode(t, DotBuffer, ASCII_pCode);
			}
			else
			{
			  	CN_Code = xc<<8;
			   j++;
			   xc = OLED_Buffer[i][j];
			   CN_Code |= xc;
			    
			   pCode = CalZiMo_Addr(CN_Code);		               // �ú��ֺͻ�����������ֿ�洢����ַ	 GB2312_CODE_ADDR  ��0x3C0000��ʼ��ռ256Kbyte 
			   pCode += GB2312_CODE_ADDR;
				t = 16;											            
				DFF.AnyNumberRead(pCode, t, DotBuffer); 
			 }

			for(k=0;k<t;k++)								               // ÿ�����ֻ��ַ����°벿����ģ����16��8�ֽ�
			{	PageBuffer[n+k] = DotBuffer[k]; 	}
			n += t;
		 }
		OLED.DataDisplaying(0,Page++,4,128,PageBuffer); 		
		/////////////////////////////////////////	
		for(j=0,n=0;j<16;j++)									         // ÿ��10������(�°벿��)
		{	
			xc = OLED_Buffer[i][j];
			if(xc==0)  xc = 0x20;
			if((xc<=0x7F)&&(xc>=0x20))									   //�ж��Ƿ�ASCII��
			  {
				ASCII_pCode = ASCII_Code + (xc - 0x20)*16 + 8; 		           //ASCII���ַ 
			   t = 8;												         //Ҫȡ�ĵ���ΪASCII��
			   GetASCIIDotCode(t, DotBuffer, ASCII_pCode);
			  }
			else
			  {
			     CN_Code = xc<<8;
			     j++;
			     xc = OLED_Buffer[i][j];
			     CN_Code |= xc;
				  pCode =  CalZiMo_Addr(CN_Code);		             // �ú��ֻ�����������ֿ�洢����ַ	 GB2312_CODE_ADDR  ��0x0000��ʼ��ռ2611688byte 
			     pCode += GB2312_CODE_ADDR + 16;
			     t = 16;	
				  DFF.AnyNumberRead(pCode, t, DotBuffer); 
				  
			   }

			for(k=0;k<t;k++)								    	          // ÿ�����ֻ��ַ����°벿����ģ����16��8�ֽ�
			{	PageBuffer[n+k] = DotBuffer[k]; 	}
			n += t;
			
		  }
	    OLED.DataDisplaying(0,Page++,4,128,PageBuffer);
	}
	NOP();	NOP();
	Disp_CSDisable;				
}	// End of <Disp> Function	


																								  

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   �������ƣ�uint32			CalZiMo_Addr(uint16   cCode);
   �������������ݺ������룬������ģ�ں��ֿ������ʼ��ַ
   ��ڲ�������������
   ���ڲ�������ģ��ַ
   ȫ�ֱ���: ��
   ����ʱ�䣺2011/07/22
   �޸�ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uint32		KeyDispCls::CalZiMo_Addr(uint16   cCode)
{
	uint32	AB,CD;
	AB = cCode>>8;			// ���ֽ�
	CD = cCode&0x00ff;	    // ���ֽ�
	return (((AB-0xa1)*94 + (CD-0xa1)) *32);
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   �������ƣ�Disp_GetGBDotCode
   �������������ݵ�ַ��flashȡGB�������
   ��ڲ������ַ����ͣ� ���ݱ����ַ�����ݵ�ַ
   ���ڲ�������
   ȫ�ֱ���: ��											
   ����ʱ�䣺2011/07/22
   �޸�ʱ�䣺
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void     KeyDispCls::GetGBDotCode(uchar ASorGB, uchar* DotBuffer, uint32 pCode)
{

   uint32    Dst;
   uint32    NByte;

   NByte = (uint32)ASorGB;
   Dst = pCode;
   DFE.Read(Dst, NByte,DotBuffer);
}
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   �������ƣ�Disp_GetASCIIDotCode
   �������������ݵ�ַ��flashȡASCII��������
   ��ڲ������ַ����ͣ� ���ݱ����ַ�����ݵ�ַ
   ���ڲ�������
   ȫ�ֱ���: ��
   ����ʱ�䣺2011/07/22
   �޸�ʱ�䣺
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

                   ���°�������
          $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  */


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�KeyProcess
   ������������������
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void       	KeyDispCls::KeyProcess(void)   
{
   NOP();   NOP();
	KEY_DL.U32.KEY32 = Adc.GetKeyCode();					// ȡ��ֵ��0xffΪ��Ч�������ް���	  
	NOP();   NOP();
	//i = j>>24;
	WithoutAckOfServer++;
	if((WithoutAckOfServer>40000) || (RegisterWithoutTer >= 3)) //60000
	{
	    RegisterWithoutTer=0; //���ݿ����޸��ն�
		WithoutAckOfServer=0;  //��������Ӧ��
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
			 IDEL_For_Acc_OFF = FALSE;					 //�����������˳�����ģʽ
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
	�������ƣ�KeyMainMenu
   ���������� ���ܰ���
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void       	KeyDispCls::KeyMainMenu(void)			// ���˵�����
{
   //LED   ULED;
   NOP();   NOP(); 
   if(1)
   {
   	if(KEY_DL.KeyPressTime==0)
   	{
			if(uLED.MainMenuCnt == DEFAULT_DISP)
			{
      		uLED.MainMenuCnt = MAIN_MENU;		          // ����������˵����ͽ������˵�   
				uLED.SubMenuCnt = 1;
				uLED.PasswordOK = FALSE;
			}
			else if (uLED.MainMenuCnt == INPUT_MENU)
			{

				 if (uLED.SubMenuCnt==15)	  //  �޸�������
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
					uLED.SubMenuCnt = 11;		  // 	 ָ�� ���������á�

				
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
					uLED.SubMenuCnt = 11;		  // 	 ָ�� ���������á�
					
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
	�������ƣ�IsInputFinish
   �����������ж��Ƿ��������
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������

uchar      KeyDispCls::IsInputFinish(uchar ParaFlag)
{
	 

	 if (uLED.InputStatus ==  INPUT_INIT_STATUS)	  // ��ʼ����
	 {
	 		uLED.MainMenuCnt = INPUT_MENU;
	 		uLED.InputAlert = FALSE;
	 		uLED.Offset = 0;
	 		strncpy((char *)&OLED_Buffer[0][0],(const char *)&SetRegParaSCR[uLED.SubMenuCnt][0],16);
	 		strncpy((char *)&OLED_Buffer[1][0],"                ",16);			  // �����ʾ���ڶ���
	
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
	  else if (uLED.InputStatus == INPUT_FINISH_ALL_CHAR)			 //  ���������ַ���ȷ�ϵ�
	  {
	  		//Uart.puts("input:"); Uart.puts((char *)SetParaBuffer);	
			//uLED.InputStatus 	= INPUT_INIT_STATUS;
			uLED.InputStatus 	= INPUT_INIT_STATUS;
	
			return	TRUE;
	  		
	  }

	  return FALSE;
	
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�InputAlertUser
   ���������� ��ʾ�û�
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
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
	�������ƣ�KeyConfirm
   ���������� ȷ�ϰ���
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
extern	uchar		jt_auth_code[];
extern	uchar		GPS_CoolStart;
static	uchar InputPassword[10];    // �޸�����ʱ��ʱʹ��

void       	KeyDispCls::KeyConfirm(void)			// ���˵�����: ȷ��
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
			if(uLED.MainMenuCnt==MAIN_MENU)				//���˵�
			{
				if(uLED.SubMenuCnt==1)
					uLED.MainMenuCnt = Car_MSG_MENU;  		   //������Ϣ
				else if(uLED.SubMenuCnt==2)
					uLED.MainMenuCnt = OVER_TIME_MENU;	      //ƣ�ͼ�ʻ
				else if(uLED.SubMenuCnt==3)
					uLED.MainMenuCnt = DRIVER_INFO_MENU;		   //��ǰ��ʻԱ��Ϣ
				else if(uLED.SubMenuCnt==4)
					uLED.MainMenuCnt = Milage_MENU;	         //���
				else if(uLED.SubMenuCnt==5)	
					uLED.MainMenuCnt = AVG_SPEED_MENU;			//15����ƽ���ٶ�
				else if(uLED.SubMenuCnt==6)
					uLED.MainMenuCnt = VEHICLE_CC_MENU;	      // ����ϵ��					
				else if(uLED.SubMenuCnt==7)
					uLED.MainMenuCnt = Mobile_PARA_MENU;	      //����ͨ�Ų���
				else if(uLED.SubMenuCnt==8)
					uLED.MainMenuCnt = Schedul_MSG_MENU;		   //������Ϣ
				else if(uLED.SubMenuCnt==9)
					uLED.MainMenuCnt = Phone_Book_MENU;  	   //�绰�� 
				else if(uLED.SubMenuCnt==10)
					uLED.MainMenuCnt = Ctr_MENU;	            //�ն˿���
            else if(uLED.SubMenuCnt==11)
            {
					uLED.MainMenuCnt = SET_PARA_MENU;	   		//��������
					SetBufferValue( (uchar *)&OLED_Buffer[1][0],16,0);
					SetBufferValue( SetParaBuffer,16,0);
					OLED_Buffer[1][0] = '0';
					uLED.PasswordOK = 0;
					uLED.Offset = 0;
					//uLED.MaxOffset = 16;
					uLED.MaxOffset = MAX_SET_PARA_MENU;//-1;
				}
				else if(uLED.SubMenuCnt==12)
					uLED.MainMenuCnt = Product_MSG_MENU;	      //��Ʒ�汾��Ϣ
				
				uLED.PageCnt = 0;
			}
			else if(uLED.MainMenuCnt==Milage_MENU)	 //���
			{
				if(uLED.PageCnt==3)
				{
					SetBufferValue((uchar*)&TestMileage,8,0);
			 		DFE.Write(EE_TEST_MILEAGE,8,(uchar*)&TestMileage);
					uLED.PageCnt = 2;
				}
			}
			else if(uLED.MainMenuCnt==Schedul_MSG_MENU)	 //������������
			{
				
				if(uLED.PageCnt==0)			 // ����
				{				    
					JT808Flag.VS2.Bits.CarLoadStatus	= 0x00;
				}
			   else if(uLED.PageCnt==1)			 // ����
				{
					JT808Flag.VS2.Bits.CarLoadStatus	 = 0x01;
				}
				else if(uLED.PageCnt==2)				// ����
				{
					JT808Flag.VS2.Bits.CarLoadStatus	 = 0x03;
				}
				if(uLED.PageCnt<=2)
				{
				   uLED.MainMenuCnt = Set_Load_OK_Menu;
				   GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_POS_REPORT,0,NULL);	     //����һ��λ����Ϣ 
					Uart.puts("�ػ��仯������λ����Ϣ ");
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
	 //	  ���Ʋ˵���ʵ��ʹ�ò���Ҫ
	 		#if (WUXI_TEST == 1)
			else if(uLED.MainMenuCnt==Ctr_MENU)	        //���Ʋ˵�
			{
				if(uLED.PageCnt==0)
				{
				  uLED.MainMenuCnt = Comfir_Logout_MENU;  //ע������ 
				}
				else if(uLED.PageCnt==1)
				{
					SystemSelfTest = TRUE;
				}
				else if(uLED.PageCnt==2)	 	// ������
				{
					Baudrate.U2_232 = 115200;
					Uart2.Init(Baudrate.U2_232,U2_FOR_DISPLAY);
					DFE.Write(EE_BAUDRATE_U2_232,4,(uchar*)&Baudrate.U2_232);
				}
				else if(uLED.PageCnt==3)	 	// ������
				{
					Baudrate.U2_232 = 9600;
					Uart2.Init(Baudrate.U2_232,U2_FOR_RFID);
				}
				else if(uLED.PageCnt==4)		// ����ͷ
				{
					Baudrate.U2_485 = 57600;
					Uart2.Init(Baudrate.U2_485,U2_FOR_CAMERA);
				}
				else if(uLED.PageCnt==5)	  	// ¼����
				{
					Baudrate.U2_232 = 115200;
					Uart2.Init(Baudrate.U2_232,U2_FOR_AUDIO);
				}
			   else if(uLED.PageCnt==6)	  // �ֶ�����ϵͳ
				{					
					Uart.puts("�ֶ�����ϵͳ\r\n");
					os_dly_wait(50);

					NVIC_SystemReset(); 
				}
				#if (WUXI_TEST == 1)
				else if(uLED.PageCnt==7)	  // ����ٶ��쳣
				{
					Uart.puts("����ٶ��쳣\r\n");
					SetBufferValue((uchar *)&SpeedStatusJudge,LEN_SPEED_STATUS_JUDGE,0);
					DFE.Write(FM_SPEED_STAUTS_JUDGE,LEN_SPEED_STATUS_JUDGE,(uchar *)&SpeedStatusJudge);
				}
				else if(uLED.PageCnt==8)	  // ʹ��˫ģ��λs
				{
					//JT808Flag.VS3.Bits.UseGPS = 1;
					//JT808Flag.VS3.Bits.UseBD = 1;
					//Tp.Buffer[0] = 0x03;
					//DFE.Write(EE_JT_GNNS_POS_MODE, 1, Tp.Buffer);
					TD.GPS = tGPS_ALL_DATA;
				}
				#endif
				#if(TEST_CODE_JUST_FOR_TEST_LUOYANG_NEW==1)
				else if(uLED.PageCnt==6)	  // ֻʹ��GPS��λs
				{
					JT808Flag.VS3.Bits.UseGPS = 1;
					Tp.Buffer[0] = 0x01;
					DFE.Write(EE_JT_GNNS_POS_MODE, 1, Tp.Buffer);
				}
				else if(uLED.PageCnt==7)	  // ֻʹ�ñ�����λ
				{
					JT808Flag.VS3.Bits.UseBD = 1;
					Tp.Buffer[0] = 0x02;
					DFE.Write(EE_JT_GNNS_POS_MODE, 1, Tp.Buffer);
				}
				else if(uLED.PageCnt==8)	  // ʹ��˫ģ��λs
				{
					JT808Flag.VS3.Bits.UseGPS = 1;
					JT808Flag.VS3.Bits.UseBD = 1;
					Tp.Buffer[0] = 0x03;
					DFE.Write(EE_JT_GNNS_POS_MODE, 1, Tp.Buffer);
				}
				else if(uLED.PageCnt==9)	  // ������ģ��
				{
					GPS_CoolStart = TRUE;
					TD.GPS = tGPS_USE_GPS_BD;
					OnlyUseBD_GPS = 0;
				}
				else if(uLED.PageCnt==10)	  // ������GPSģ��
				{
					GPS_CoolStart = TRUE;
					TD.GPS = tGPS_USE_GPS;
					OnlyUseBD_GPS = 0;
				}
				#endif
				else if(uLED.PageCnt==11)	  // GPRS��������
				{
					GPRS.Close(); 
				}
				#if(TEST_CODE_JUST_FOR_TEST_LUOYANG_NEW==1)
				else if(uLED.PageCnt==12)	  // ʹ��ǰ����һ
				{					
					DFE.Write(EE_JT_M_IP_DNS,16,(uchar*)"jt1.gghypt.net\0\0");	
					DFE.Write(EE_JT_B_IP_DNS,16,(uchar*)"jt2.gghypt.net\0\0");	
					Tp.B32[0] = 7008;
					DFE.Write(EE_JT_TCP_PORT,4,Tp.Buffer);
					DFE.Write(EE_JT_UDP_PORT,4,Tp.Buffer);
				}
				else if(uLED.PageCnt==13)	  // �л�������IP
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
				   uLED.MainMenuCnt = Logout_Fales_MENU1;								  //��������ÿ�   
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
					   GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DEREG,0,NULL); 	              //����ע������ָ��    
						Uart.puts("ע������ ");
					}
				}																																																	
				else
				{
				   uLED.MainMenuCnt = Logout_Fales_MENU;								  //�����磬�޷�ע��
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
						//os_evt_set(UART0_PRINT_EVT_FLAG,TaskID.U0print);   //������ӡ�¼�
				}
			 }
			 else if(uLED.MainMenuCnt == SET_PARA_MENU)
			 {
DISPOSE_INPUT_ALL_CHAR:
			 	 switch(uLED.SubMenuCnt)
				 {
				 	case eSET_PARA_PWD:			// ����
				 		DFE.Read(EE_SET_PARA_PASSWORD,4,Tp.Buffer);
						if (Tp.B32[0] == 0)
						{
							Tp.B32[0] = 123;		  // ȱʡ����
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
				 			memmove((uchar*)&OLED_Buffer[1][0],(uchar*)"�������",8);
							//uLED.InputStatus = INPUT_FINISH_ALL_CHAR;
				 		}
						SetBufferValue(SetParaBuffer,16,0);
					
					break;

					case eSET_PARA_MAIN_IP:			// ��IP
						
					//break;

					case eSET_PARA_BAK_IP:			// ��IP
						if (IsInputFinish(SET_PARA_INPUT_NUM_AND_DOT) == TRUE)
						{
							for (s = 0, Tp.Buffer[0] = 0; s < LEN_INPUT_BUFFER; s++)
							{
								if (SetParaBuffer[s] == '.')
								{
									Tp.Buffer[0]++;
								}
							}
							if (Tp.Buffer[0] != 3)			  // �Ƿ�IP
							{
									InputAlertUser("IP��ַ�Ƿ�", 10);
							}
							else
							{
								//uLED.InputStatus 	= INPUT_INIT_STATUS;
								// ����IP
								s = strlen((char *)SetParaBuffer);

								if (uLED.SubMenuCnt==1) // ��IP
								{
									Uart.puts("��IP");
									
									SetBufferValue(&JT808_IP_DNS.M_IPorDNS[0], LEN_IPorDNS, 0); 
									memmove((char *)&JT808_IP_DNS.M_IPorDNS[0], (char *)SetParaBuffer, s);
									DFE.Write(EE_JT_M_IP_DNS, LEN_IPorDNS, (uchar *)&JT808_IP_DNS.M_IPorDNS[0]);
									NOP();NOP();NOP();

								}
								else						// ��IP
								{
									 Uart.puts("��IP");
									 SetBufferValue(&JT808_IP_DNS.B_IPorDNS[0], LEN_IPorDNS, 0); 
									 memmove((char *)&JT808_IP_DNS.B_IPorDNS[0], (char *)SetParaBuffer, s);
									 DFE.Write(EE_JT_B_IP_DNS, LEN_IPorDNS, (uchar *)&JT808_IP_DNS.B_IPorDNS[0]);
									 NOP();NOP();NOP();
								}
					
							}
							SetBufferValue(SetParaBuffer,16,0);
						}
						
					break;

					case eSET_PARA_TCP:			// TCP�˿�;		
						
					//break;
					case eSET_PARA_UDP:			// UDP�˿�	
						if (IsInputFinish(SET_PARA_INPUT_NUM_AND_DOT) == TRUE)
						{
								//uLED.InputStatus 	= INPUT_INIT_STATUS;
								Tp.B32[1] = Str2Int((char *)SetParaBuffer);
								if (Tp.B32[1] > 65535)
								{
									InputAlertUser("�˿ںŷǷ�", 10);
									
								}
								else		
								{
									// ����˿ں�
									if (uLED.SubMenuCnt==3) // TCP
									{
										 Uart.puts("TCP�˿�");

										 JT808_IP_DNS.TCP_Port = Tp.B32[1];						
				     					 DFE.Write(EE_JT_TCP_PORT, 4,(uchar *)&JT808_IP_DNS.TCP_Port);
										 NOP();NOP();NOP();
									}
									else						// UDP
									{
									 	Uart.puts("UDP�˿�");

										JT808_IP_DNS.UDP_Port = Tp.B32[1];
				      				DFE.Write(EE_JT_UDP_PORT, 4,(uchar *)&JT808_IP_DNS.UDP_Port);
										NOP();NOP();NOP();

									}

									SetBufferValue(SetParaBuffer,16,0);
									
								}
						}
							
						
					break;

					case eSET_PARA_PRIVINCE_ID:			// ʡ��ID
						if (IsInputFinish(SET_PARA_INPUT_NUM_AND_DOT) == TRUE)
						{
							  Uart.puts("ʡ��");

							  Tp.B32[1] = Str2Int((char *)SetParaBuffer);
							  DFE.Write(EE_PROVINCE_REGION_ID, 2, (uchar *)&Tp.B32[1]);
							  NOP(); NOP(); NOP();

							  SetBufferValue(SetParaBuffer,16,0);
						}
						
					break;
					case eSET_PARA_PREFECTURE_ID:			// ����ID
						if (IsInputFinish(SET_PARA_INPUT_NUM_AND_DOT) == TRUE)
						{
								Uart.puts("����");

								Tp.B32[1] = Str2Int((char *)SetParaBuffer);
								DFE.Write(EE_PREFECTURE_REGION_ID, 2, (uchar *)&Tp.B32[1]);
							   NOP(); NOP(); NOP();

								SetBufferValue(SetParaBuffer,16,0);
						}
						
						
					break;
					case eSET_PARA_CAR_NUM:			// ���ƺ���	
						if (IsInputFinish(SET_PARA_INPUT_NEED_FOR_CAR_NUM) == TRUE)
						{
							Uart.puts("����");
							SetBufferValue(Tp.Buffer,64, 0);
							memmove(Tp.Buffer,SetParaBuffer,9);
							DFE.Write(FM_VEHICLE_CODE, LEN_VEHICLE_CODE, Tp.Buffer);
							NOP(); NOP(); NOP();

							SetBufferValue(SetParaBuffer,16,0);
						}
						
						
					break;
					case eSET_PARA_CAR_COLOR:			// ������ɫ		 �����̰׺�
						if (IsInputFinish(SET_PARA_INPUT_NEED_FOR_COLOR) == TRUE)
						{
							Uart.puts("��ɫ:");
							Uart.puts((char *)SetParaBuffer);
							Uart.puts("\r\n");

							s  = 0;	 // ��ɫ����
							if (strcmp((char *)"��", (char *)SetParaBuffer) == 0)
							{
								 	s = 1;
							}
							else if (strcmp((char *)"��", (char *)SetParaBuffer) == 0)
							{
									s = 2;
							}
							else if (strcmp((char *)"��", (char *)SetParaBuffer) == 0)
							{
									s = 3;
							}
							else if (strcmp((char *)"��", (char *)SetParaBuffer) == 0)
							{
									s = 4;
							}
							else if (strcmp((char *)"��", (char *)SetParaBuffer) == 0)
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

					case	eSET_PARA_VIN_NUM:          // VIN ��
						if (IsInputFinish(SET_PARA_INPUT_NEED_FOR_CAR_NUM) == TRUE)
						{
							Uart.puts("VIN��");
							SetBufferValue(Tp.Buffer,64, 0);
							memmove(Tp.Buffer,SetParaBuffer,17);
							DFE.Write(FM_VEHICLE_VIN, LEN_VEHICLE_VIN, Tp.Buffer);
							NOP(); NOP(); NOP();

							SetBufferValue(SetParaBuffer,16,0);
						}

					break;


					case	eSET_PARA_CAR_TYPE: 				// ��������
						uLED.MainMenuCnt = SET_PARA_SUB_MENU;
						uLED.SubSubMenuCnt = 1;
						
						
					break;

					case eSET_PARA_CAR_PHONE_NUM:			// �ն��ֻ�����
						
					//break;
					case eSET_PARA_OWNER_PHONE_NUM:			// �����ֻ�����
								 
					//break;
					case eSET_PARA_CENTER_NUM:			// ������ĺ���
						if (IsInputFinish(SET_PARA_INPUT_NUM_AND_DOT) == TRUE)
						{
								
							  	Uart.puts("�ֻ���");

								if (strlen((char *)SetParaBuffer) < 5)
								{
										InputAlertUser("�ֻ��ŷǷ�", 10);
								}
								else
								{
									if (uLED.SubMenuCnt==eSET_PARA_CAR_PHONE_NUM) // �ն��ֻ���
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
									else if (uLED.SubMenuCnt==eSET_PARA_OWNER_PHONE_NUM)	 // �����ֻ���
									{
										s = strlen((char *)SetParaBuffer);
										if (s > LEN_NUMBER) 	s = LEN_NUMBER;

										DFE.Write(EE_JT_CAR_OWNER_PHONE_NUM, s, (uchar *)SetParaBuffer);
										NOP(); NOP(); NOP();		
										 
									}
									else						 // ������ĺ���
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

					case eSET_PARA_K_VALUE:			// Kֵ
						if (IsInputFinish(SET_PARA_INPUT_NUM_AND_DOT) == TRUE)
						{
							VehiclePulseKV = Str2Int((char *)SetParaBuffer);
							DFE.Write( FM_VEHICLE_PULSE_KV,4,(uchar*)&VehiclePulseKV);
							NOP(); NOP(); NOP();

							SetBufferValue(SetParaBuffer,16,0);
						}

						
					break;

					case eSET_PARA_UPLOAD_INTERVAL_TIME:			// �ϱ�ʱ����
						if (IsInputFinish(SET_PARA_INPUT_NUM_AND_DOT) == TRUE)
						{
							Tp.B32[1] = Str2Int((char *)SetParaBuffer);
							DFE.Write(EE_JT_SEND_TIME_DEFAULET, 4,(uchar *)&Tp.B32[1]);
							NOP(); NOP(); NOP();
							GSM.SetDefaultSendInterval(Tp.B32[1]);
						}

					break;

					case eSET_PARA_CREATE_USER:			//	 ����
						//Uart.puts("����");

						//memmove((uchar*)&OLED_Buffer[1][0],(uchar*)"�����ɹ�",8);
						InputAlertUser("�����ɹ�", 8);

						CreateUserFlag = OK;
						DFE.Write(EE_CREATE_USER_FLAG, 1, &CreateUserFlag);
						NOP(); NOP(); NOP();
						
					break;
					
									
					case eSET_PARA_CANCER_REG:			// ע������
						
						GPRS.UnRegTerminal();
						
					break;
					
					case eSET_PARA_FACTORY_SET:			// �ָ�����ֵ
						
						Sys.FirstPowerOn(FALSE); 
						NVIC_SystemReset();  
						
					break;
					
					
					case eSET_PARA_REMOTE_UPDATE:			// Զ������
						
						MenuSetRemoteIAP = TRUE;
						uLED.MainMenuCnt = UPDATE_INDICATE;
						
					break;			
					
					case eSET_PARA_MOD_PWD:			// �޸�����				
						if (uLED.InputModifyPwdFlag == 0)
						{
							InputAlertUser("������������", 12);
						}
						else if  (uLED.InputModifyPwdFlag == 1)
						{
							if (IsInputFinish(SET_PARA_INPUT_NUM_AND_DOT) == TRUE)
							{
								 SetBufferValue(InputPassword, 10, 0);
								 memmove((char *)InputPassword, (char *)SetParaBuffer, 10);


								 SetBufferValue(SetParaBuffer, 16,0);
								 InputAlertUser("�ٴ�����������", 14);
								 uLED.InputModifyPwdFlag = 2;
								 
							}
						}
						else if (uLED.InputModifyPwdFlag == 3)
						{
							 if (IsInputFinish(SET_PARA_INPUT_NUM_AND_DOT) == TRUE)
							 {
								 if (memcmp(InputPassword, SetParaBuffer, 10) == 0)
								 {
								   Uart.puts("�����޸ĳɹ�");

									InputAlertUser("�����޸ĳɹ�", 12);
									Tp.B32[0] = Str2Int((char *)InputPassword);

									//Uart.puts("pwd1:");
									//Uart.puts((char *)InputPassword);

									//Uart.puts("modify pwd: ");Uart.putb(1, 4, (uchar *)&Tp.B32[0]);

									DFE.Write(EE_SET_PARA_PASSWORD,4,(uchar *)&Tp.B32[0]);
									NOP(); NOP(); NOP(); 

								 }
								 else
								 {
								 	  InputAlertUser("�������벻ͬ", 12);
								 }

							 }
							 uLED.InputModifyPwdFlag = 0;
							
						}			
					
						
					break;
//					case 16:			// ������
//						
//					break;
//					case 17:			// �ٴ�����������	
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
							//os_evt_set(UART0_PRINT_EVT_FLAG,TaskID.U0print);   //������ӡ�¼�
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
					  		  	for (s = 0; s < 2; s++)				 // �����������ֽ�
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
								for (s = 0; s < 2; s++)				 // �����������ֽ�
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
						goto 	DISPOSE_INPUT_ALL_CHAR; // ������ɵ��ַ�

				  }
			 }
			 else if (uLED.MainMenuCnt==SET_PARA_SUB_MENU)
			 {
			 	  if (uLED.SubMenuCnt==eSET_PARA_CAR_TYPE)
				  {
				  		SetBufferValue(Tp.Buffer, 16,0);
						strcpy((char *)Tp.Buffer, (char *)&VehicleTypeSCR[uLED.SubSubMenuCnt-1][2]);	 // ������ "1.xxxx" �е�xxxx
						
						DFE.Write(FM_VEHICLE_CLASS, 12,Tp.Buffer);
						InputAlertUser("    ���óɹ�", 12);
				  }
			 }
			 /*else if(uLED.MainMenuCnt==REG_INFO_MENU)			// ע����Ϣ		����ʱʹ�õ�
			 {
			 	 // uLED.PageCnt CurrentRegInfo
				 if(CurrentRegInfo<26)
				 {
				 	 DFE.Write(EE_PROVINCE_REGION_ID,46, (uchar *)&RegInfo25Device[CurrentRegInfo][0]);		// ע����Ϣ
					 CreateUserFlag = OK;		  // ����
					 DFE.Write(EE_CREATE_USER_FLAG, 1, &CreateUserFlag);
					 NeedRegServer = 0;	
					 DFE.Write(EE_NEED_REG_SERVER, 1, &NeedRegServer);
					 SetBufferValue(  (uchar*)jt_auth_code, LEN_AUTH_CODE,0);
					 if(CurrentRegInfo==25)
					 	strcpy( (char*)jt_auth_code,"13886088888\0");
					 else
					 	memmove( jt_auth_code,(uchar *)&RegInfo25Device[CurrentRegInfo][46],10);	  		// ��Ȩ��
					 DFE.Write(EE_AUTH_CODE, LEN_AUTH_CODE,(uchar *)&jt_auth_code[0]);
					 SetBufferValue(sTelNum.Sim, 16, 0);
					 if(CurrentRegInfo==25)
					 	strcpy ( (char*)&sTelNum.Sim[0],"013886088888\0");
					 else
					 	memmove((uchar*)&sTelNum.Sim[0],(uchar *)&RegInfo25Device[CurrentRegInfo][56],12); 		// �ֻ���
					 DFE.Write(EE_SIM_NUMBER,16,(uchar*)&sTelNum.Sim[0]);
					 Tp.Buffer[0] = strlen((char *)&sTelNum.Sim[0]);
					 DecCharToPressBCD(Tp.Buffer[0],(char *)sTelNum.Sim,SimNumberBCD);
					 memmove(sTelNum.SimBCD_JT, SimNumberBCD, 6);

					 ///////////////////////////////////////////////////////////////
					 SetBufferValue(Tp.Buffer,32,0);
					 strcpy( (char*)Tp.Buffer,"L0000");
					 strcat( (char*)Tp.Buffer,(char*)sTelNum.Sim);
					 DFE.Write(FM_VEHICLE_VIN,LEN_VEHICLE_VIN,Tp.Buffer);														// ����VIN��
					 DFE.Write(FM_VEHICLE_CLASS,12,(uchar*)"������װ����");

					 SetBufferValue(Tp.Buffer,32,0);
					 Tp.Buffer[0] = 0x13;		Tp.Buffer[1] = 0x05;		Tp.Buffer[2] = 0x09;
					 Tp.Buffer[3] = 0x10;		Tp.Buffer[4] = 0x20;		Tp.Buffer[5] = 0;
					 DFE.Write(FM_INSTALL_DATE,6,Tp.Buffer);											  			// ��װ����

					 DFE.Write(FM_INIT_MILAGE_BCD,4,(uchar*)&Tp.Buffer[16]);								 	// ��ʼ���

					 VehiclePulseKV = 3600;
					 DFE.Write( FM_VEHICLE_PULSE_KV,4,(uchar*)&VehiclePulseKV);					  			// Kֵ

					 SetBufferValue(Tp.Buffer,32,0);							  
					 strcpy( (char*)Tp.Buffer,"0000000");
					 strcat( (char*)Tp.Buffer,(char *)&RegInfo25Device[CurrentRegInfo][9]);
					 Tp.Buffer[23] = 0x13;		Tp.Buffer[24] = 0x04;		Tp.Buffer[25] = 0x25;
					 Tp.B32[15] = Str2Int( (char*)&RegInfo25Device[CurrentRegInfo][69] );
					 Tp.Buffer[26] = Tp.Buffer[63]; 	Tp.Buffer[27] = Tp.Buffer[62];
					 Tp.Buffer[28] = Tp.Buffer[61]; 	Tp.Buffer[29] = Tp.Buffer[60];
					 DFE.Write( FM_SERIAL_NUMBER, LEN_SERIAL_NUMBER, Tp.Buffer);					  		// Ψһ���к�

					 SystemLog.VDR.SpeedSensorPulsCnt = 3463*VehiclePulseKV;
					 SystemLog.VDR.SpeedSensorPulsCnt += gCnt.TickForRandData&0x0000007f;
					 DFE.Write( EE_SYS_VDR_SPEED_PULS_CNT,4,(uchar*)&SystemLog.VDR.SpeedSensorPulsCnt);

					 Uart.puts(" �����������棬�޸�ע����Ϣ ");
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
	�������ƣ�KeyPageDown
   �����������·�����
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void      KeyDispCls::KeyPageDown(void)			// Key1  ƣ�ͼ�ʻ�������Ϸ�������
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
			else	if( (uLED.MainMenuCnt == Schedul_MSG_MENU))											  	// �·�������
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
				if (uLED.SubMenuCnt==eSET_PARA_CREATE_USER) // �����˵�
				{
					return;
				}

				if (uLED.InputAlert == TRUE)				// ���֮ǰ����ʾ��IP��ַ�Ƿ������ٰ��ù��ܼ��� �ͱ�ʾҪ���������ַ�
				{
					
					if ((uLED.SubMenuCnt==eSET_PARA_CREATE_USER)	||	//  ����
					   (uLED.SubMenuCnt==eSET_PARA_CAR_TYPE))	
					{
						return;
					}

					if (uLED.SubMenuCnt==eSET_PARA_MOD_PWD)	  //  �޸�������
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
						if (uLED.InputPageCnt < 36)  // 0 - 9 �� A - Z 36	 �����ַ�
						{
							uLED.InputPageCnt++;
							
						}
						else 			  // ����
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
	�������ƣ�KeyPrint
   ���������� ��ӡ����
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void      KeyDispCls::KeyPrint(void)			 // Key2   ƽ���ٶ���ʾ�����·�������
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
					uLED.SubMenuCnt = 11;		  // 	 ָ�� ���������á�

				
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
					uLED.SubMenuCnt = 11;		  // 	 ָ�� ���������á�
					
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
	�������ƣ�KeyPageUp
   ���������� �Ϸ�����
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 

// void			eTaskPrint(void);
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void       KeyDispCls::KeyPageUp(void)				    //
{
   //uchar	s  = 0;
   //LED  ULED;
   NOP();   NOP();
   if(1)
   {
   	if(KEY_DL.KeyPressTime==0)
   	{   		   		 
			if(uLED.MainMenuCnt == MAIN_MENU) 	// �ӵ�2ҳ���˵� ��ʾ
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
			else	if(uLED.MainMenuCnt == Schedul_MSG_MENU)											  	// �Ϸ�������
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

				if ((uLED.SubMenuCnt==eSET_PARA_CREATE_USER)	||	//  ����
					   (uLED.SubMenuCnt==eSET_PARA_CAR_TYPE))	
				{
					return;
				}

				if (uLED.InputAlert == TRUE)				// ���֮ǰ����ʾ��IP��ַ�Ƿ������ٰ��ù��ܼ��� �ͱ�ʾҪ���������ַ�
				{
					SetBufferValue(&OLED_Buffer[1][0], 16, 0);
					uLED.Offset = 0;
					uLED.InputStatus = INPUT_INIT_STATUS;
					uLED.InputPageCnt = 0;
					uLED.InputAlert = FALSE;

					if (uLED.SubMenuCnt==eSET_PARA_MOD_PWD)	  //  �޸�������
					{
						if (uLED.InputModifyPwdFlag == 0)
						{	
							uLED.InputModifyPwdFlag = 1;		// ��ʱ���Ե�һ����������
							return;
						}

						if (uLED.InputModifyPwdFlag == 2)
						{
							uLED.InputModifyPwdFlag = 3;		 // ��ʱ�����ٴ���������
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
						if (uLED.InputPageCnt >= 38)  // 0 - 9 �� A - Z    36�����ַ�
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
	�������ƣ�TurnOffLed
   �����������ر�LED
   ��ڲ�����LED��ţ�����ʹ�LED�����һ�£�
   ���ڲ�������
   ȫ�ֱ���: ��
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
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
	�������ƣ�TurnOnLed
   ������������LED
   ��ڲ�����LED��ţ�����͹ر�LED�����һ�£�
   ���ڲ�������
   ȫ�ֱ���: ��
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
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
	�������ƣ�SetLED_Status
   ��������������ָʾ��״̬
   ��ڲ�������ţ�״̬
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			KeyDispCls::SetLED_Status(uchar no,uchar Data)
{
	if( (Data<20)&&(no<LED_TOTAL_NUMBER))
		Lamp.Status[no] = Data; 
}




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�LoadDispInitData
   ����������������ʾ��ʼ��Ϣ
   ��ڲ������Ƿ���ʾ��ʼ����Ϣ��0: ��ʾ  ��0������ʾ
   ���ڲ�������
   ȫ�ֱ���: ���ļ���̬���� InitDispCNT
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
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
   ������: TaskLED_Buzzer
   ���ܣ�  ָʾ�ƺͷ�����
   ��ڣ�  ��
   ���ڣ�  ��
*/
#if(USE_LED)
unsigned short 	const		LED_OffTimeTab[] =  { 1000,400,350,200,100,60,40,32,20,12,8,4,2,0,0};		  
unsigned short 	const		LED_OnTimeTab[] =   {    0,100, 80, 60, 40,30,25,20,12, 8,4,2,1,0,0}; 
#endif
void       KeyDispCls::LED_Buzzer(void)    //20����ɨ��һ��		
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
		
	//////////////////////////////////////// ���������г���
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
			  Beep.times=255;   // �Ա�֤ Beep.times>Beep.counter,��ΪBeep.counter++
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
	�������ƣ�
   ����������
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uchar       KeyDispCls::GetOVT_Addr(uchar HowLong, uint32 *PAddr)
{
   /*NOP();	 NOP();
	uchar   BfData[8][28],i,n, tBuffer[17];

	uchar   OverDriveTimes;
	uint32  Ntime,TowDateTime,Rtime,DFF_Page;

	Ntime = Rtc.GetRtcSecond();
	TowDateTime = Ntime - 24*3600 - Rtc.DT.Hour*3600 - Rtc.DT.Min*60 - Rtc.DT.Sec;			//������
	OverDriveTimes = 0;
	DFE.Read(FM_TIRE_DRIVE,224,(uchar *)BfData);				   //������FRAM
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

	DFE.Read(FM_TIRE_DRIVE_FF_Addr,4,(uchar *)&DFF_Page);				 //������Flash
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
		TowDateTime = Ntime - 24*3600 - Rtc.DT.Hour*3600 - Rtc.DT.Min*60 - Rtc.DT.Sec;			//������
		OverDriveTimes = 0;

		 Rtime = Rtc.BDC_DateTimeToSecond((RTC_STR *)tBuffer);

		DFE.Read(FM_FATIGUE_DRIVING_CNT,4,(uchar*)&tl);
		{	// ȡ�����������
			if (tl<5)  // ȡ����������ƣ�ͼ�ʻ���ݼ���
			{ 
				tl = tl;    	
			}
			else		// ȡ���´��������ﲿ������,�����ݻ�û�浽flash����
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
					// �ҵ����ϵ�����
					//memmove(&pSendBuffer[LEN_U0_GB_FRAME_HEAD+DataLen],&tBuffer[i * LEN_SAVE_FATIGUE_DRIVING_INFO],LEN_SAVE_STOP_DATA);
					//DataLen += LEN_SAVE_FATIGUE_DRIVING_INFO;	

					*PAddr = FM_SAVE_FATIGUE_DRIVING+(i-1) * LEN_SAVE_FATIGUE_DRIVING_INFO;
		 			PAddr++; 
		 			OverDriveTimes++; 
				}
			}
		}

		ReadPage = 	 FlashPageCnt.FatigueDrivingSave;
		while (ReadPage>0)		 // ȡflash�е�ƣ������
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
					// �ҵ����ϵ�����
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

	if (len > i)	 // pdu�ֻ����볤�Ȳ����� SMS_NUM_LEN / 2 ���ֽڣ����� 86��
	{
		len = i;	
	}

	for (i = 0; i < len; i++)
	{
		*dest++ = (src[i] & 0x0f) + '0';	       //   + 0x30
		*dest++ = ((src[i] >> 4) & 0x0f) + '0';
	}
	dest--;        	  // ��λ���ֻ��ţ� ascii����һλ
	if (*dest ==  0x3F)   // 0x0f + '0' 	  һ������11λ���ֻ����붼�С�F'����
		*dest = '\0';           // ȥ�� �ֻ��� �� ascii����һλ�� ��0  
	else
		*(++dest) = '\0';    
		
	
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
	
const		uchar		AckXorError[5] = {0x24,0xff,0x02,0xd9,0x0a};
const		uchar		AckOK[5] = {0x24,0x0d,0x02,0x2b,0x0a};
//const		uchar		ReturnSuccess[5] = {0x24,0xa4,0x02,0x2b,0x0a};
//const		uchar		ReturnFailure[5] = {0x24,0xa3,0x02,0x2b,0x0a};

#define		BSJ_D1_PDU_DATA_INDEX		22
#define		BSJ_D2_PDU_DATA_INDEX      21
#define		BSJ_PDU_PARAM_INDEX        7                       // ��������(TP-MTI/VFP) ���ͣ�TP-VP����Ը�ʽ
#define		BSJ_DATA_LEN_INDEX         2
#define		BSJ_DEFORE_PDU_DATA_LEN    19 							// PDU ��ʽ֮ǰ�����ݳ���
#define		BSJ_DST_ADDR_COUNT_INDEX	9															//Ŀ���ַ���ָ���




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
	Uart2.putb(1,5,(uchar*)AckOK);		// ��Ӧ��
	Uart2.SendTask();	
	os_dly_wait(3);
	Uart2.putb(1,5,(uchar*)AckOK);		// ��Ӧ��
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
			{		// ���ö���Ϣ����ʽ�����ӵ�������������ͨѶ����
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
					  GPRS.JT_T808_NewDataPack(eBB_gCMD_TER_ASK_ACK, DataLen+2, 0, 0, (uchar *)tBuffer);	//����Ӧ��
					  Uart.puts("����Ӧ��11 ");	 
					}
				}
				if(s1==0)
				{
					tBuffer[0] = 1;		            //��Ϣ����
					tBuffer[1] = DataLen>>8;			//��Ϣ����
					tBuffer[2] = DataLen;
					memmove(&tBuffer[3], (uchar *)&DispRecBuffer[LCM_FRAME_DATA], DataLen);
					GPRS.JT_T808_NewDataPack(eBB_gCMD_PLAT_INFO_SERVICE, DataLen+3, 0, 0, (uchar *)tBuffer);		//��Ϣ����
					Uart.puts("��Ϣ����Ӧ�� ");	 
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
			{		// ���ö���Ϣ����ʽ�����ӵ�������������ͨѶ����
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
					s = DispRecBuffer[BSJ_DST_ADDR_COUNT_INDEX];  // Ŀ���ַ���ȣ�������91�͡�F��
					if ((s % 2) != 0) s = (s + 1) / 2;
					else				 s = s / 2;

					 s += 1;			// ��һ���ֽ� �� 91��

					 s1 = 3 + 7 + s + 3;	  // D1 ���û����ݳ����±�
					//if (DispRecBuffer[BSJ_D1_PDU_DATA_INDEX - 1] == 
						//(DispRecBuffer[BSJ_DATA_LEN_INDEX] - BSJ_DEFORE_PDU_DATA_LEN - 2))   // BSJ D1 PDU ��22�ֽ�Ϊpdu��ʽ�����ݳ���

					if (DispRecBuffer[BSJ_DATA_LEN_INDEX] == (7 + s + 4 + (DispRecBuffer[s1]) + 2))
					{
						// \x00 \x01 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x00 \x0512345 \x0FL00001922280586 \x04����1392228586 \x00 \x0013886088888 \x00608888844011 \x08��A08888ö
						//�� 24 02 1B 30 32 31 00 11 00 0D 91 68 31 75 20 03 96 F2 00 08 01 06 76 84 76 84 76 84 15  FF
						//PDU_mobile_num_to_ASCII(&DispRecBuffer[LCM_PDU_SEND_NUM], (uchar *)&SMS_SendNumber[2], 6);
						//SMS.SetPduDataMessage(SMS_UCS2, (char *)SMS_SendNumber,&DispRecBuffer[BSJ_D1_PDU_DATA_INDEX], 
						//						DispRecBuffer[BSJ_DATA_LEN_INDEX ] - BSJ_DEFORE_PDU_DATA_LEN - 2);
					
						PDU_mobile_num_to_ASCII(&DispRecBuffer[LCM_PDU_SEND_NUM], (uchar *)&SMS_SendNumber[2], s - 2);

						//s = memcmp((uchar *)&SMS_SendNumber[2], "10086", 5);
						s = strlen((char *)&SMS_SendNumber[2]);
						if (s != 11)								// ����11λ�ֻ���, ֻ�ܷ���Ӣ�Ķ���
						{
							//Uart.puts("AAA");
							//Uart.puts(&SMS_SendNumber[2]);
							//os_dly_wait(1);
							for (i = 1, j = 0; i < DispRecBuffer[s1]; i += 2)
							{
								tBuffer[j++] = DispRecBuffer[s1+1 + i];  // unicode���Ӣ��תΪASCII���Ӣ��   00 41 -> 41
							}
							GSM.SetSMS_Buffer(&SMS_SendNumber[2], tBuffer);
							SMS_Flag = TRUE;
							
						}
						else
						{
							SMS.SetPduDataMessage(SMS_UCS2, (char *)SMS_SendNumber,&DispRecBuffer[s1+1], DispRecBuffer[s1]); 	  
						}
						
					}
					else 		  // BSJ D2 PDU    D2 ��������pdu��ʽ�����ݳ���	ָʾ�ֽ�
					{
						//��  24 02 1A 30 31 35 00 11 00 0D 91 68 31 75 20 03 96 F2 00 08 01 76 84 76 84 76 84 15 FF

						//PDU_mobile_num_to_ASCII(&DispRecBuffer[LCM_PDU_SEND_NUM], (uchar *)&SMS_SendNumber[2], 6);
						PDU_mobile_num_to_ASCII(&DispRecBuffer[LCM_PDU_SEND_NUM], (uchar *)&SMS_SendNumber[2], s - 2);
						SMS.SetPduDataMessage(SMS_UCS2, (char *)SMS_SendNumber,&DispRecBuffer[BSJ_D2_PDU_DATA_INDEX], 
														DispRecBuffer[BSJ_DATA_LEN_INDEX ] - BSJ_DEFORE_PDU_DATA_LEN + 1 - 2);
					}
					
				}
				else 					  // BSJ, D2, TEXT		�ı���Ϣ 	 D2������TEXT ��PDU , D1ֻ��PDU
				{
					s = DispRecBuffer[LCM_FRAME_DATA];
					if (s > LEN_SMS_NUMBER_EXT / 2)   return;			     // pdu ��ʽ���ֻ��Ų����� SMS_NUM_LEN / 2 ���ֽ�	 
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
		case	eLCM_D_SET_TCP_IP_PORT:						  //TCP IPʵ��ΪJT808����������TCP��ַ �˿ں�������/��TCP�Ķ˿ںţ�
			// <1,��202.106.119.249��,8888>
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
				if(DispRecBuffer[LCM_FRAME_DATA+1]==0x31)					  //1,ֻ��ͨTCPͨ��
				{
				   sIP.EnableFlag[eFLAG_TCP] = ENABLE_FLAG;
				}
				else if(DispRecBuffer[LCM_FRAME_DATA+1]==0x32)			  //2��ֻ��ͨUDPͨ��
				{
				   sIP.EnableFlag[eFLAG_UDP] = ENABLE_FLAG;
				}
				else if(DispRecBuffer[LCM_FRAME_DATA+1]==0x33)			  //3����ͨ˫ͨ����0�������ر���������TCP��UDPͨ��
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

		case  eLCM_D_SET_UDP_IP_PORT: 							//UDP IPʵ��ΪJT808�ı��ݷ�����TCP��ַ �˿ں�������/��UDP�Ķ˿ںţ�
			// <1,��202.106.119.249��,8888>
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
				if(DispRecBuffer[LCM_FRAME_DATA+1]==0x31)						  //1,ֻ��ͨTCPͨ��
				{
				   sIP.EnableFlag[eFLAG_TCP1] = ENABLE_FLAG;
				}
				else if(DispRecBuffer[LCM_FRAME_DATA+1]==0x32)				  //2��ֻ��ͨUDPͨ��
				{
				   sIP.EnableFlag[eFLAG_UDP1] = ENABLE_FLAG;
				}
				else if(DispRecBuffer[LCM_FRAME_DATA+1]==0x33)				  //3����ͨ˫ͨ����0�������رձ��ݷ�����TCP��UDPͨ��
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
		// ������ÿ�������ӷ��Ͱ汾�ż���ʻԱ�ţ���������ʶ����������ǩ������ǩ�����Ե���ǩ���������ɨ��ȥ������
			// 24 0A 1D 32 30 31 31 2E 30 36 2E 32 38 3B 4C 43 44 5F 54 54 53 5F 56 32 2E 30 00 00 27 11 52 FF 
			// 24 0A 1D 32 30 31 31 2E 30 36 2E 32 38 3B 4C 43 44 5F 54 54 53 5F 56 32 2E 30 00 01 27 11 53 FF
			ID = DispRecBuffer[28];
			ID <<= 8;
			ID |= DispRecBuffer[29];
			s1 = DispRecBuffer[27];			// 0��¼��1�˳�

			if( (ID==0) || ((s1!=0)&&(s1!=1)&&(s1!=0xaa)) )		return;		// �ֱ������ֵ�������֧��IC��	

			if(  (ID==DriverID[0])||(ID==DriverID[1])||(ID==DriverID[2])||(ID==DriverID[3])\
				||(ID==DriverID[4])||(ID==DriverID[5])||(ID==DriverID[6])||(ID==DriverID[7]))
			{
				s = OK;		// �Ϸ���
				Driver.CardStatus = '0';
				NoDriverCnt = 0;
				//if(s1 == 0x00)		VehicleControl.Lock = LOCK_VEHICLE_DISABLE;	
			}
			else
			{
				s = ERROR; 	// �Ƿ���
				Driver.CardStatus = '1';				
			}
			
			// ���ϴβ�ͬ�žʹ������ϱ�������
			if(s1==0xaa)
			{
				Driver.CardStatus = '4';
				NoDriverCnt++;
				//if( (NoDriverCnt==2)||((NoDriverCnt%10)==0))	// ���������ʾ����Ϊͣ������ʱ���忨
				//	SendDataToLCM(eLCM_U_CENTER_MSG,(4*2),(uchar*)"δ�忨������������\0");					
				//if(NoDriverCnt==5)
					//VehicleControl.Lock = LOCK_VEHICLE_ENABLE_INV;
			}
			else if( (ID != Driver.ID)||(s1 != (Driver.LogStatus-'0')) ) 
			{
				NoDriverCnt = 0;
				if(s1 == 0x00)				// ��ǩ
				{ 	
					Driver.LogStatus = '0';
					if(s==OK)	// ��Ч��		
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
						SendDataToLCM(eLCM_U_CENTER_MSG,(4*2),(uchar*)"δ��Ȩ����\0");
					}
				}
				else if(s1 == 0x01)		// ��ǩ (�������������ǩ���ź�����ĵ�ǩ������ͬ��
				{
					Driver.LogStatus = '1';
					if(s==OK)				// ��Ч��
					{
						//VehicleControl.Lock = LOCK_VEHICLE_ENABLE_INV;
						Driver.LogOutTime = 0;							
					}
				}		
				DFE.Write(EE_LOCK_FLAG,LEN_LOCK_VEHICLE_FLAG,(uchar *)&VehicleControl);				
				
				Driver.ID = ID;
				//////////////////////////// �ϱ�������		
				strcpy(tBuffer,sNowTimeString);
				strcat(tBuffer,",1,\0");		 		// ������
				if( (s==OK) )			// �Ϸ���
				{
					if(s1==0)	{	strcat(tBuffer,"0,\0");		}
					else			{	strcat(tBuffer,"1,\0");		}					
				}
				else 						// �Ƿ���
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
				
				DFE.Write(EE_DRIVER_INFO,LEN_DRIVER_INFO,(uchar*)&Driver);		// ��ǰ˾������

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
			PhoneCall.KeyHang = TRUE;	 	// �ֱ������һ�

		break;

		case 	eLCM_D_ANSWER_CALL:			// ���У��ֻ�ժ��
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

		// ����������IP�Ͷ˿ڵ���ʱ��
		DFE.Read(EE_JT_M_IP_DNS, LEN_IPorDNS, &IP_Port[0]);
		NOP(); NOP(); NOP();
	
		DFE.Write(EE_JT_TMP_TCP_IP, LEN_IPorDNS, &IP_Port[0]);
		NOP(); NOP(); NOP();
	
		DFE.Read(EE_JT_TCP_PORT,4,(uchar *)&tmp);
		NOP(); 
		DFE.Write(EE_JT_TMP_TCP_PORT, 4, (uchar *)&tmp);
		NOP(); NOP(); NOP();
	
		// �޸�TCPʹ�õ�IP�Ͷ˿�
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
	while (sIP.Online[eFLAG_TCP]!= ENABLE_FLAG)	   // �ȴ�����
	{
		i++;	
		if (i > 300)	return ERROR;			 // 30�볬ʱ

		os_dly_wait(10);	
	}

	//Uart.puts("EEE22");
    //os_dly_wait(1);
		
	return OK;
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�	
   ����������ʹ����������
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
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
	for(i=0;i<3;i++)		// 3�α䳤����
	{
		CurLen = *(pData+Addr);
		Addr += (CurLen+1);		
	}

	SetBufferValue(tBuffer,80,0);

	memmove( &tBuffer[5],(pData+Addr),12);		

	CurLen = GPRS.GetRegInfo(&tBuffer[17]);
   CurLen += 17;

	tBuffer[17] = 0;
	tBuffer[17] = 41;		// ʡ��ID

	tBuffer[19] = 0x01;
	tBuffer[20] = 0x2f;	// ����ID

	tBuffer[53] = 2;		// ������ɫ����
	memmove( &tBuffer[54],"��G",3);


	// ���������ע��
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
		// �ն˲�����
		//KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(6*2+1),(uchar*)"\x43\x7E\xC8\x7A\xEF\x00\x20\x4E\x0D\x57\x28\x7E\xBF");
		s = 0x04;  // ��������
		KeyDisp.SendDataToLCM_YH(eLCM_D_CAR_REG_RESULT, 1, &s); 
		os_dly_wait(5);

		return;
	}
	Creat_User_Reging = TRUE;
	Creat_User_Reging_OverTime = 0;
	GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_CREATE_USER,CurLen,tBuffer);
	os_dly_wait(50);
	Uart.puts("ʹ��ǰ���� ");
    
	
}

// added 2013.4

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
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
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
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

	// ע����Ϊ��󳤶�256����������Щָ���Ҫ�����ԶԳ���������չ����������չ˵����
	// �����һ���ֽڵ����λΪ1�����ʾ��չ���ȡ�����չ����״̬�£�����15���ֽ�ͨ��16���ƴ��ģʽ�����泤�ȡ�
	// ���磺0x80 0x12��ʾ����Ϊ0x0012��0x81 0x12��ʾ����Ϊ0x0112��
	if(DispRecBuffer[2] & 0x80)	// Ϊ��չ����
	{
	  DataLen = (DispRecBuffer[2] & 0x7F) << 8;
	  DataLen |= DispRecBuffer[3];
	  
	  //Uart.puts("AAA111");Uart.putb(1, 2, (uchar *)&DataLen);os_dly_wait(1); 
	}
	else   		 // ��ͨ����
	{
		// ASCII���ʾ��0~9��A~F����󳤶�Ϊ256��(���糤Ϊ11����ʮ��������0B���������ֽھ�д0x30 0x42)
		DataLen = HexCharToByte((char*)&DispRecBuffer[2]);		// 

	}

	Cmd = DispRecBuffer[1];
	if( (Cmd!=0x56) )				// �ɶ���Щָ����У��
	{

		s = CalXorSum((DataLen+3),&DispRecBuffer[1]);

		if( s != DispRecBuffer[(DataLen+4)] )	
		{
			SendDataToLCM_YH(0xfe,0,Tp.Buffer);  	// �������
			return;
		}
	}
	

	//Uart.putb(1, DataLen+6, DispRecBuffer);os_dly_wait(1);
	
	SetBufferValue((uchar*)Tp.Buffer,256,0);
	///////////////////////////////////////////////////
	switch(Cmd)
	{
		//////////////////////////////////////////////// 
		case 0x01:	 // ���ֽ�ָ��
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

				case 0x06:	// ��������
					//PhoneCall.DialCnt = 0;
					//PhoneCall.Length = DataLen;
					//memmove(PhoneCall.Buffer,DefaultCenterNumber1,strlen(DefaultCenterNumber1));		  // 	 ��ע��
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
		case 0x02:			//  ����

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
			//Uart.puts((char*)&DispRecBuffer[LCM_FRAME_DATA_YH]);	// �������
			if( (DispRecBuffer[LCM_FRAME_DATA_YH]=='0')&&(DispRecBuffer[LCM_FRAME_DATA_YH+1]=='*')  )
			{
				CallForSetPara = (10*100UL);		// 10��

				tCmd = (DispRecBuffer[LCM_FRAME_DATA_YH+2]-'0')<<4;
				tCmd += (DispRecBuffer[LCM_FRAME_DATA_YH+3]-'0');
				t = (DispRecBuffer[LCM_FRAME_DATA_YH+4]-'0')<<4;
				t += (DispRecBuffer[LCM_FRAME_DATA_YH+5]-'0');
				switch(tCmd)	 	// �ò������ù��ܣ���ʱʵ�ֵ�����δʵ�ֵĹ���
				{
					case 0x00:		// �Լ�
						
						SystemSelfTest = TRUE;

					break;
					/////////////////////////////////////////////////////// ֻ��������
					case 0x01:		// �ֶ�ע�� (Ӧ�����ʹ�ã�
						Uart.puts("�ֶ�ע�� ");
						GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_REG,2,Tp.Buffer);
						
					break;

					case 0x02:		// �ֶ���Ȩ��Ӧ�����ʹ�ã�
						Uart.puts("�ֶ���Ȩ ");
						GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_AUTH,2,Tp.Buffer);
						
					break;

					case 0x03:		// �����˵���Ӧ�����ʹ�ã�			
						Uart.puts("�ֶ������˵� ");
						strcpy((char*)Tp.Buffer,"���ǲ��Եĵ����˵�\r\n");
						GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_ELE_WAYBILL_UPLOAD,10,Tp.Buffer);
						
					break;

					case 0x04:		// �ֶ�������ý���¼��ϱ���Ӧ�����ʹ�ã�
						Uart.puts("�ֶ�������ý�� ");
						GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_MEDIA_EVENT_UPLOAD,2,Tp.Buffer);
						
					break;

					case 0x05:		// �ֶ�������ʻ����ϱ���Ϣ
						GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DRIVER_INFO_REPORT,2,Tp.Buffer);
						Uart.puts("�ֶ������ʻԱ�����Ϣ ");
					break;

					case 0x06:		// Уʱ
						GPS.GPSValidCnt = 0;
						Uart.puts("�ֶ�Уʱ");
					break;

					case 0x08:			// �鿴ע����Ϣ���ֻ���

						SetBufferValue(Tp.Buffer,256,0);

						strcpy( (char*)Tp.Buffer,"\x47�ֻ�����: ");
						Len = strlen( (char*)Tp.Buffer );
						DFE.Read(EE_SIM_NUMBER, 16, (uchar *)&Tp.Buffer[Len]);
						
						strcat( (char*)Tp.Buffer,"; ʡ��ID: ");
						Len = strlen( (char*)Tp.Buffer );
						DFE.Read(EE_PROVINCE_REGION_ID, 2, (uchar *)&DataLen);//Tp.Buffer[Len]);
						Int2Str(DataLen, (char*)&Tp.Buffer[Len]);
						
						strcat( (char*)Tp.Buffer,"; ����ID: ");
						Len = strlen( (char*)Tp.Buffer);
						DFE.Read(EE_PREFECTURE_REGION_ID, 2, (uchar *)&DataLen);//Tp.Buffer[Len]);
						Int2Str(DataLen, (char*)&Tp.Buffer[Len]);
						
						strcat( (char*)Tp.Buffer,"; ������ID: ");
						Len = strlen( (char*)Tp.Buffer);
						DFE.Read(EE_VENDOR_ID, 5, (uchar *)&Tp.Buffer[Len]);

						
						strcat( (char*)Tp.Buffer,"; �ն��ͺ�: ");
						Len = strlen( (char*)Tp.Buffer);
						DFE.Read(EE_TERMINAL_TYPE, 20, (uchar *)&Tp.Buffer[Len]);
						

						strcat( (char*)Tp.Buffer,"; �ն�ID: ");
						Len = strlen( (char*)Tp.Buffer);
						DFE.Read(EE_TERMINAL_ID, 7, (uchar *)&Tp.Buffer[Len]);
						

						strcat( (char*)Tp.Buffer,"; ������ɫ: ");
						Len = strlen( (char*)Tp.Buffer);
						DFE.Read(FM_CAR_COLOUR, 1, (uchar *)&Tp.Buffer[Len]);
						t = Tp.Buffer[Len];
						
						if(t)
						{
							strcat( (char*)Tp.Buffer,"; ���ƺ���: ");
							Len = strlen( (char*)Tp.Buffer);
							DFE.Read(FM_VEHICLE_CODE, 9, (uchar *)&Tp.Buffer[Len]);							
						}
						else
						{
							strcat( (char*)Tp.Buffer,"; ����VIN��: ");
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
						Uart.puts("�ֶ���ƽ̨ע�� ");
						GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DEREG,0,NULL);

					break;

					/////////////////////////////////////////////////////// 1�ֽ�����+1�ֽ�����
					case 0x11:		// �¼�����
						Uart.puts("�ֶ��¼����� ");
						Tp.Buffer[0] = t;
						GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_EVENT_REPORT,1,Tp.Buffer);
						
					break;

					case 0x12:		// ��Ϣ�㲥/ȡ��	 ע����������������ϢID���ڷ��ͺ�������
						Uart.puts("�ֶ���Ϣ�㲥ȡ�� ");
						Tp.Buffer[0] = t;	
						GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_GET_INFO,1,Tp.Buffer);
						
					break;

					case 0x16:   // �ֶ���ƽ̨��ע��
						Uart.puts("�ֶ���ƽ̨��ע��");
						GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DEREG,0,NULL);

					break;


					case 0x20:
						Uart.puts("�ֶ�����洢��ҳ������ ");
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
							Uart.puts(" �ֶ�ɾ��ȫ��Χ����·�� ");
						}
						else if(t==1)
						{
							SetBufferValue((uchar*)&RoundFenceAttr,LEN_EE_FENCE_ATTR,0);
							DFE.Write(EE_ROUND_FENCE_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RoundFenceAttr);
							Uart.puts(" �ֶ�ɾ��Բ��Χ�� ");
						}
						else if(t==2)
						{
							SetBufferValue((uchar*)&RectFenceAttr,LEN_EE_FENCE_ATTR,0);
							DFE.Write(EE_RECTANGLE_FENCE_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RectFenceAttr);
							Uart.puts(" �ֶ�ɾ������Χ�� ");
						}
						else if(t==3)
						{
							SetBufferValue((uchar*)&PolygonFenceAttr,LEN_EE_FENCE_ATTR,0);
							DFE.Write(EE_POLYGON_FENCE_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&PolygonFenceAttr);
							Uart.puts(" �ֶ�ɾ�������Χ�� ");
						}
						else if(t==4)
						{
							SetBufferValue((uchar*)&RoadAttr,LEN_EE_FENCE_ATTR,0);
							DFE.Write(EE_ROAD_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RoadAttr);
							Uart.puts(" �ֶ�ɾ��·�� ");
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
							if(JT808Flag.VS3.Bits.UseGPS == 1 )		strcat((char*)Tp.Buffer,"ʹ��GPS��λ��");
							if(JT808Flag.VS3.Bits.UseBD == 1)		strcat((char*)Tp.Buffer,"ʹ�ñ�����λ");
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
							Uart.puts(" TTS ����1");
							Uart1.puts("AT+QTTS=2,\"���\",1\r\n\0");
						}
						else
						{
							Uart.puts(" TTS ����2");
							Uart1.puts("AT+QTTS=2,\"���\",2\r\n\0");	  // TEST OK
						}
						os_dly_wait(100);
					break;

					
					//////////////////////////////////// �豸����ϵ��
					case 0x40:
						
						Tp.B32[0] = Str2Int( (char*)&DispRecBuffer[LCM_FRAME_DATA_YH+6] );
						SpeedC.SetGetPara(3,4,Tp.Buffer);
						Uart.puts("�ֶ��豸����ϵ�� ");						
						
					break;
					
					// uchar			OnlyUsePulseSpeed,OnlyUseGPS_Speed;
					case 0x90:		// ����ʹ�����壬��ʹ��GPS�ٶ�
						OnlyUsePulseSpeed = FALSE;
						OnlyUseGPS_Speed = FALSE;
						Uart.puts("�ֶ����ã�����ʹ�����壬��ʹ�������ٶ� ");
						break;
						
					case 0x91:		//ʹ�������ٶ�
						OnlyUsePulseSpeed = TRUE ;
						OnlyUseGPS_Speed = FALSE;
						Uart.puts("ֻʹ�������ٶ� ");
						break;
						
					case 0x92:		// ʹ��GPS�ٶ�
						OnlyUsePulseSpeed = FALSE;
						OnlyUseGPS_Speed = TRUE;
						Uart.puts("ֻʹ�������ٶ� ");
						break;

					case 0x93:

						Tp.Buffer[0] = t;
						SpeedC.SetGetPara(eSPEED_CMD_DISABLE_CNT,1,Tp.Buffer);
						
						break;

					case 0x97:
						GPS_CoolStart = TRUE;
						Uart.puts("�ֶ���������ģ��������ָ�� ");
					break;
					
					////////////////////////////////////  �����豸
					case 0x98:
						
						if(t==0x98)
							Sys.FirstPowerOn(TRUE);
						else if(t==90)
							Sys.FirstPowerOn(0);
						Uart.puts("�ָ��������� ");
					break;

					/////////////////////////////////// ����
					case 0x99:		// �����ն�						
						Uart.puts("�ֶ�����ϵͳ");
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
		case 0xe0:			//   ������ѯ������Ҳ��e0ָ��
			DataLen = 0;
			
		break;

		//////////////////////////////////////////////// 
		case 0xa4:			//   �յ���Ϣ����Ϣ�а��������ֱ��ִ��������ڲ�ѯ�����ò���
			// 0F A4 33 63 4E 31 32 33 34 35 36 37 38 39 30 31 32 33 34 35 36 37 38 39 30 
			// 43 00 2A 00 47 00 59 00 2C 00 30 00 30 00 30 00 2C 00 52 00 38 00 31 00 2C 00 31 00 32 00 33 00 34 00 35 00 36 00 23 AE F0 
			// U2 Rec Test Data: 0F A4 33 32 4E 34 30 30 36 30 32 30 38 36 33 FF FF FF FF FF FF FF FF FF FF 43 00 4A 00 54 00 31 00 2E 00 47 00 47 00 48 00 59 00 50 00 54 00 2E 00 4E 00 45 00 54 C0 F0 
			
						
			if (jt_auth_ok_flag != 1)  break;	   
			if (DispRecBuffer[LCM_FRAME_DATA_YH] == 0x4E)
			{
				// ����������"###"˵����Ҫ��������ƽ̨����Ϣ
				if ((DispRecBuffer[LCM_FRAME_DATA_YH+1] == '#') && (DispRecBuffer[LCM_FRAME_DATA_YH+2] == '#') &&
					(DispRecBuffer[LCM_FRAME_DATA_YH+3] == '#'))	       
				{
					s1 = 0;
					Len = DataLen - (1+LCM_PHONE_NUM_LEN_YH+1); // �������ĵ�ʵ�����ݶγ���

					if(GG_Evt.GPRS.AckGSMS == 1)
					{
						//if(Len<=2)
						{ 
						  GG_Evt.GPRS.AckGSMS = 0;
						  s1 = 0xAA;
						  Tp.Buffer[0] = Plat_Serial_No >> 8;//AckBuffer.Index>>8;
						  Tp.Buffer[1] = Plat_Serial_No & 0xff; //AckBuffer.Index;
						  //memmove(&Tp.Buffer[2], (uchar *)&DispRecBuffer[LCM_FRAME_DATA_YH+1+LCM_PHONE_NUM_LEN_YH+1], Len);
						  /*if (Len == 2) s = 1; 	// �ظ�һ�������֣�Ϊascii��  	 ���ƽ̨ ����IDΪ��0�� - ��6��	 �������ظ������� unicode��
						  else			s = 0;
		
						  memmove(&Tp.Buffer[2], (uchar *)&DispRecBuffer[LCM_FRAME_DATA_YH+1+LCM_PHONE_NUM_LEN_YH+1+s], Len-s);	 // ����00������00 31�� ֻ����31�Ϳ�����
						  //Len -= s;  

						  GPRS.JT_T808_NewDataPack(eBB_gCMD_TER_ASK_ACK, Len+2, 0, 0, (uchar *)Tp.Buffer);	//����Ӧ�� */
						  // 
						  // 0F A4 31 61 4E 23 23 23 23 FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF 43 00 27 00 31 EF F0 
						  s =  DispRecBuffer[LCM_FRAME_DATA_YH+1+LCM_PHONE_NUM_LEN_YH+1+1]; // ���ֵ�unicode���һ���ֽ�Ϊ0,��Ч�ǵڶ��ֽ�
						  if ((s > '0') && (s <= '9')) 	 
						  {
						  	Tp.Buffer[2] = s - 0x30;	//0x01;
						  }
						  else
						  {
						  	GG_Evt.GPRS.AckGSMS = 1;
							SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(9*2+1),(uchar*)"\x43\x8B\xF7\x56\xDE\x59\x0D\x6B\x63\x78\x6E\x76\x84\x65\x70\x5B\x57\x00\x21"); // ��ظ���ȷ������!
							DataLen = 0;
							break;
						  }	
						  Uart.puts("����Ӧ�� ");	 
						  GPRS.JT808_DataPackPushAlignment(0,eBB_gCMD_TER_ASK_ACK, 3, 0, (uchar *)Tp.Buffer);		//����Ӧ��
						}
					}
					/*if(s1==0)
					{
						Tp.Buffer[0] = 1;		            //��Ϣ����
						Tp.Buffer[1] = DataLen>>8;			//��Ϣ����
						Tp.Buffer[2] = DataLen;
						memmove(&Tp.Buffer[3], (uchar *)&DispRecBuffer[LCM_FRAME_DATA_YH+1+LCM_PHONE_NUM_LEN_YH+1], Len);
						GPRS.JT_T808_NewDataPack(eBB_gCMD_PLAT_INFO_SERVICE, Len+3, 0, 0, (uchar *)Tp.Buffer);		//��Ϣ����
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
				else  // �����ֻ��ϵĶ���Ϣ		
				{
				// 0F A4 31 61 4E 31 33 35 37 30 32 33 30 36 39 32 FF FF FF FF FF FF FF FF FF 43 4F 60 59 7D 31 F0 
					SetBufferValue((uchar *)SMS_SendNumber, LEN_SMS_NUMBER_EXT, 0);
					SMS_SendNumber[0] = 0x38;
					SMS_SendNumber[1] = 0x36;
	
					SMS_Flag = FALSE;
					s = 0xff;
					pT = SearchArray(20,(uchar *)&DispRecBuffer[LCM_FRAME_DATA_YH+1], 1, &s);
					if (pT == NULL)	s = 20;	                               
					else s = pT - &DispRecBuffer[LCM_FRAME_DATA_YH+1]; // �绰���볤��

					memmove(&SMS_SendNumber[2], &DispRecBuffer[LCM_FRAME_DATA_YH+1], s);	
	
					s1 = LCM_FRAME_DATA_YH+1+LCM_PHONE_NUM_LEN_YH+1;	 // ���������±�
					Len = DataLen - (1+LCM_PHONE_NUM_LEN_YH+1); 			// �������ݳ���

					if (s != 11)								// ����11λ�ֻ���, ֻ�ܷ���Ӣ�Ķ���	  �磺���ź���
					{
						for (i = 1, j = 0; i < Len; i += 2)
						{
							Tp.Buffer[j++] = DispRecBuffer[s1+i+1];  // unicode���Ӣ��תΪASCII���Ӣ��   00 41 -> 41
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
		case 0xac:			// ��������������͸������
			
			//GPRS.PushAlignment(DataLen+5,DispRecBuffer);	  // ��������ֱ�ӷ��͵��������
			LcmDataToServer = TRUE;
			s = WaitLcmDataToServer(500);	
			if(s==OK)
				Tp.Buffer[0] = 0x17;
			else
				Tp.Buffer[1] = 0x18;
			SendDataToLCM_YH(1,1,Tp.Buffer);
			DataLen = 0;
		break;   


		case 0xc0:		// ��¼
			//tl = Str2Int((char*)&DispRecBuffer[LCM_FRAME_DATA_YH]);	 //
			//s = DriverLoginLogout(0,tl);
			if(s==OK)		Tp.Buffer[0] = 0x30;	
			else				Tp.Buffer[1] = 0x31;
			SendDataToLCM_YH(0x21,1,Tp.Buffer);
			DataLen = 0;
		break;

		case 0xc1:		// �˳�
			//tl = Str2Int((char*)&DispRecBuffer[LCM_FRAME_DATA_YH]);	
		//	s = DriverLoginLogout(1,tl);    // ��ע�� xzb
			if(s==OK)		Tp.Buffer[0] = 0x30;	
			else				Tp.Buffer[1] = 0x31;
			SendDataToLCM_YH(0x22,1,Tp.Buffer);
			DataLen = 0;
		break;

		case 0xd0:		// ������ť
			
			VehicleStatus.VS6.Bits.CallHelpAlarm = 1;
			DataLen = 0;
		break;

		// added 2013.5
		case 0x51:			// ����ע��
			JT808_UnLock_Terminal(&DispRecBuffer[LCM_FRAME_DATA_YH], DataLen);	 // ʹ��ǰ���� ����	
			DataLen = 0;
		break;
		//

		case 0x56:			// װ��״̬

			if( DispRecBuffer[LCM_FRAME_DATA_YH]==0)		// ����
			{
				JT808Flag.VS2.Bits.CarLoadStatus = 0;
				strcpy((char*)Tp.Buffer,"�ֶ�����״̬������  \0");
			}
			else if( DispRecBuffer[LCM_FRAME_DATA_YH]==1)	// ����
			{
				JT808Flag.VS2.Bits.CarLoadStatus = 1;		
				strcpy((char*)Tp.Buffer,"�ֶ�����״̬������  \0");
			}
			else if( DispRecBuffer[LCM_FRAME_DATA_YH]==2)	// ����
			{
				JT808Flag.VS2.Bits.CarLoadStatus = 3;
				strcpy((char*)Tp.Buffer,"�ֶ�����״̬������  \0");
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
	�������ƣ�
   ����������
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
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
		//  SendDataToLCM_YH��pData[0] �� �����־������������
		if (ERROR==GSM.TTS_Opreation(Len-1,pData+1))
		{
			Uart.puts("TTS���ݷ���ʧ��\r\n");
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
			if((i%50)==0)	Uart.puts("������ʾ���ݳ�ʱ ");
			if(i>300)
			{
				Uart2.Init(Baudrate.U2_232,U2_FOR_DISPLAY);	// ����
				os_dly_wait(100);
				break;
			}
		}
		SendDataToLCM_Busing = TRUE;
	
		if(Len>(LEN_TO_HY_LCM_BUFFER-10))		Len = (LEN_TO_HY_LCM_BUFFER-10);			// DispBuffer��󳤶�300
	
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
	�������ƣ�
   ����������
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			KeyDispCls::CenterSendDataToLCM_YH(uint16 Len,uchar *pData)
{
	uchar tBuffer[256];
	uchar i,j;  //tmp;


	if(U2PinSelDevice!=U2_FOR_DISPLAY)	return;		// ¼��ģ��ʹ��U2
	
	j = 0;
	tBuffer[j++] = 0x4E; //  ����
	// 20λ���ĺ���
	memmove(tBuffer+j, "####", 4);
	j += 4;
	for (i=0; i<16; i++)	
	{
		tBuffer[j+i] = 0xFF;		
	}
	j += 16;
	//
	tBuffer[j++] = 0x54;   // ʱ��
	//
	Rtc.GetNowDateTimeString(0,&Rtc.DT, tBuffer+j);
	memmove(tBuffer+j, tBuffer+j+2, 12); // ȥ����ݵġ�20�� �磺2013������ͱ�ɣ�13
	for(i=0; i<12; i+=2)	  // ����YH��������ʱ��˳��
	{
		tBuffer[j+i] ^= tBuffer[j+i+1];
		tBuffer[j+i+1] ^= tBuffer[j+i];
		tBuffer[j+i] ^= tBuffer[j+i+1];

	}
	
	j += 12;
	tBuffer[j++] = 0x47; // GB //0x43;  // ��ʾ������Ϣ
	memmove(tBuffer+j, pData, Len);  // ��Ϣ����
	Len += j;
	SendDataToLCM_YH(eLCM_U_CENTER_MSG, Len, tBuffer);


		
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
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
	�������ƣ�
   ����������
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			KeyDispCls::SendGSMSignalToLCM(void)		  
{
	uchar tBuffer[24];

	// �����̣��й��ƶ�����
	memmove(tBuffer,(uchar *)"\x43\x54\x2D\x47\x53\x4D\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", 20); 
	tBuffer[20] = 0x2C;  // ���ŷָ���	   ����tBuffer[21] ��ʾ�ź�ֵ������״̬
	// 0x30 ����Ϣ,	 0x31 1����Ϣ��... 0x36 6���źţ�0x37 ��GPRS,����
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
	�������ƣ�
   ����������
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
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
	�������ƣ�SendDataToLCM 
   ���������� �������ݸ��ֱ�
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
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
	�������ƣ�CustomCmdProcess
   ����������ʹ�õ������Ķ��Ź��ܣ������Զ����ָ��޸Ļ��ȡ��������
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
	(DispRecBuffer[LCM_FRAME_DATA]=='*')
	
	����ָ���ʽʹ�� *GY,Gxx,x1,x2,...#
	*GY,          �̶��ַ���
	Gxx           ָ���룬��10�� ~ "99"
	x1            ����1
	x2            ����2
	#				  ������

*/


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uchar			KeyDispCls::CustomCmdProcess(void)
{
	uchar		s,Cmd;
	uchar		tBuffer[32];
	char		*pR;
	NOP();	NOP();
	s = ERROR;		// ����Ϊ�Ǵ����ָ��
	pR = MyStrStr((char*)&DispRecBuffer[LCM_FRAME_DATA],"*GY,G\0");
	if(pR != NULL)
	{
		SetBufferValue(tBuffer,32,0);
		Cmd = My_atol((char*)&DispRecBuffer[LCM_FRAME_DATA+5]);
		if((Cmd>9)&&(Cmd<100))   // 2λ��������
		{
			switch(Cmd)
			{
				case 10:		  // �޸�Э��ͷ�������������Ӣ����ĸ *GY,G10,TH#
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

				case 11:			// ����ָ�� *GY,G11,U,2,A,102#
					
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
	�������ƣ�	 TPMS_ReceiveProcessData
   ����������	 ��̥��ѹ���ݴ���
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺2014.04.16
   �޸�ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������	  
// 55 AA 08 00 00 3C 00 CB 55 AA 08 01 00 3C 08 C2 55 AA 08 10 00 3A 08 D5 55 AA 08 11 00 3B 00 DD 
void			KeyDispCls::TPMS_ReceiveProcessData(uint16 RecLen,uchar *pRecData)
{
	static uchar 	Index = 0, tCnt = 0;
	uchar	tBuffer[128+16], Num, i, Len;
	
	
//	if (TD.Display==tDISP_TEST_FOR_EXT)
//	{
//		Uart.puts("����2������̥���� Test Data:");
//		Uart.putb(1,RecLen, pRecData);
//		Uart.puts("\r\n");
//	}

	tCnt++;

	if (tCnt==15)	// 1���ӷ�һ��
	{

		tCnt = 0;
		
		Num = RecLen>>3; 		  // RecLen / 8	  ÿ��"55 aa" ��ʼ������Ϊ8���ֽ�,��ÿ����̥����Ϊ8���ֽ�
		Len = 0;
		tBuffer[Len++] = Num;				// ���ָ���
		tBuffer[Len++] = Index++;		   // ��ˮ��
		// �������� 	 С����ÿ������8�ֽڣ�	// 0:С���� 1����
		if ((RecLen%8) == 0)			  //  
		{
			tBuffer[Len++] = 0; 	
		}
		else
		{
			tBuffer[Len++] = 1; 
		}
	
	   tBuffer[Len++] = 0; // ����
	
		for (i=0; i<Num;i++)
		{
			memmove(&tBuffer[Len+i*4],&pRecData[i*8+3],4);
		}
		Len += Num * 4;
	
		GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TPMS_DATA_UPLOAD,Len,tBuffer);
	
		}	
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			KeyDispCls::ReceiveProcessData(void)
{
	
}






/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */  
