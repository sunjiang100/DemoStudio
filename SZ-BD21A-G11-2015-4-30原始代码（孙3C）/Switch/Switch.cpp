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
$$ �������ƣ�eTask()                                 
$$	�������ܣ�������������������
$$	�����������
$$	�������	 ��
$$	ȫ�ֱ�����
$$	�޸�ʱ�䣺
$$	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$*/
void			SwitchCls::eTask(void)										// 10ms����һ��
{
	char		SW32Bit[128];
	uint16	i,SwStatus;
	
	NOP();  NOP();  
	NOP();  NOP();
	TickCnt++;

	if (GG_Status.Flag.Bits.UpdateRemoteSet ==1) return;			 // Զ������
			
	if(TickCnt>1500)														// 100ms������һ����������״̬	
	{																				
		SwStatus = ReadIOState();	
		NOP();	NOP();		
		CheckSwitchAlarm(SwStatus);									   // ������������� 
		NOP();	NOP();
	}
	NOP();	NOP();

	TH_VehicleControlOutput();				

	NOP();	NOP();									  					
	

	if(TickCnt%5==0)															
	{																				
		CheckMotoStop();														// 50ms����Ƿ��з�����ת���ź�		
	}

	
	
	Check_Collision();			// ��ײ��෭���	 
	
	if(TickCnt%10==0)															// ÿ100ms���һ��
	{																				
		
		
		if (!(JT808_Alarm_Word.Shield & 0x01))		      CheckRobAlarm();			// �پ����
		//if (!(JT808_Alarm_Word.Shield & (0x01 << 26)))	CheckStealAlarm();		// �������				//����ⲿ����������
		
		//if (!(JT808_Alarm_Word.Shield & (0x01 << 28)))	CheckTrailCarAlarm();	// �ϳ����				�Ƿ�λ��
		//CheckNoSetScanAlarm();												// δ������
		//CheckHelpAlarm();														// �����������
		//CheckTireDriveAlarm();												// ƣ�ͼ�ʻ���
		//CheckIllegalOpenDoor();												// �Ƿ����ż��
		//if (!(JT808_Alarm_Word.Shield & (0x01 << 27)))  CheckIllegalMakeFire();						// �Ƿ������
		//CheckMachineSelfTest();												// �ն��Լ�(24Сʱ����һ��)
		//CheckShakeAlarm();													   // �𶯼��
		//CheckLoginAlarm();													   // ��½������󱨾����
		if (!(JT808_Alarm_Word.Shield & (0x01 << 8)))	CheckMainPower();			// ����Դ������	
		
		//CheckUnlockAlarm();													// ��δ���ñ���
		//CheckHandleWatchCar();												// �ֱ��������
		//AutoLockDoor();														// �Զ�����
	}

	
	
	if(TickCnt%100==0)														// 1���Ӽ��һ�η�����������
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
** �������ƣ�Init()
** ������������������ʼ��
** ��ڲ�������
** ���ڲ�������
** ȫ�ֱ���: ��
** �޸�ʱ�䣺2009.08.21   
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
**	�������ƣ�ReadIOState()
** ������������ȡ����IO���ŵ�״̬
** ��ڲ�������
** ���ڲ������ɹ�����0
** ȫ�ֱ���: SP_State,SG1_State
** ����ʱ�䣺2009.08.21
** �޸�ʱ�䣻
** ˵����SP_State,SG1_State��λ���ⲿ�������Ӧ������,SP��IO������Ϊ�ߵ�ƽ,SP_State���Ӧ
** ��λΪ1,����Ϊ0;SG1��IO������Ϊ�͵�ƽ,SG1_State���Ӧ��λΪ1,����Ϊ0;
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
	
	/////////// ��ȡI/O���� ////////////////////////////
	GET_IO_PIN_INIT;
	ACC_Status		= !GET_ACC_STATUS;								  // ���ʱ,��־Ϊ1		SW1
	MOTO_Status 	= !GET_MOTO_STATUS;							  // ����ʱ,��־Ϊ1		 SW3
	Button_Status 	= !GET_BUTTON_STATUS;		//EMButton_Statu;      //!GET_BUTTON_STATUS;		������ť״̬��>2s��������					  
	
	Adc.GetAllResult(AdcSW);

   	// added 2013.3
	JT808Analog1 = AdcSW[3];    //  
	JT808Analog2 = AdcSW[2];    // ����Դ

	//

	if(AdcSW[1]<1100)		  //ȱֽ�ж�
	{
	   PrintPaperStu = 0;		// ȱֽ
	}
	else
	{
		PrintPaperStu = 1;
	}

	if (AdcSW[2] < 6000) //  ���磬 JT808
	{
		MainPower_Status = 0;
		DOOR_Status = 1;  // ����Դ���磬�Ŵ�һֱ��Ϊ����
	}
	else	
	{
		MainPower_Status = 1;
	}
	
	if(MainPowerValue>180)			LowPowerAlarmValue = 180;			// ����Դ����16V����Ϊ��24V�ĳ�
	else if(MainPowerValue<170)	LowPowerAlarmValue = 110;			// jt808Ҫ�� 24V����Ĭ��17V��ѹ���� 
		
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
		if(MainPowerLowCnt>(15*100L))		  	// 10mS����һ��
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
			
					if (JT808_Alarm_Word.SendTextSMS & (0x01 << Index))		     								// ���Ͷ���
					{
						GSM.SetSMS_Buffer((char *)&JT808_Number.TextSMS[0],"Main Power Low Alarm!!!");		
					}
				
				   if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))
				   {
			  			if (!(JT808_Alarm_Word.SavePhoto & (0x01 << Index)))	Jt_photo_upload = 1;		  // �ϴ���ƽ̨
						MediaEventFlag =eBB_MEDIA_EVENT_OTHER;
			  			Camera.SetTakeNumber(2,TRUE);		  // ���� 
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

	  	if(MainPowerHighCnt>(15*100L))		  	// 10mS����һ��
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
	if(AdcSW[2]<18000) 		// 12V��
	{
		th = 8000;
		tl = 4000;
	}
	else						// 24V��
	{
		th = 18000;
		tl = 8000;
	}
	for(i=0;i<2;i++)		// ��·ģ����
	{				
		Tp.B8[0] >>= 1;
		if((HighLowLevel&(1UL<<i))!=0)			// �ߵ�ƽ��Ч
		{
			if(AdcSW[i+3]>th)
				Tp.B8[0] |= 0x80;
		}
		else												// �͵�ƽ��Ч
		{
			if(AdcSW[i+3]<tl)
				Tp.B8[0] |= 0x80;
		}
		
	}

	
	/////////////////////////////////////////////////////////// �������緢����
	//	 ģ����
	
	if(Tp.B16[0]&0x0001)			// Adc04		��						   //
	{   
		 DOOR_Status = 1;
		 if (JT808Flag.VS2.Bits.Door1 == 0);
		 {
		 	JT808Flag.VS2.Bits.Door1 = 1;
		 	//Uart.puts("����\r\n\0");//os_dly_wait(1);
		 }
		 VehicleStatus.VS3.Bits.DoorOpen = 1;
	}
	else
	{
		DOOR_Status = 0;
		if (JT808Flag.VS2.Bits.Door1 == 1);
		{
		 	JT808Flag.VS2.Bits.Door1 = 0;
		 	//Uart.puts("����\r\n\0");os_dly_wait(1);
		}
		VehicleStatus.VS3.Bits.DoorOpen = 0;
	}
	
	if(Tp.B16[0]&0x0002)				// 	Adc05 	����	
	{
		JT808ExtenVsFlag.VS1.Bits.ReversGear = 1;
	}
	else
	{
		JT808ExtenVsFlag.VS1.Bits.ReversGear = 0;
	}

	/*if(Tp.B16[0]&0x0004)									 //����		  Ӳ��û�ж�Ӧ�Ľӿڣ�������������
		J808ExtenVsFlag.VS2.Bits.Speaker = 1;
	else
	   J808ExtenVsFlag.VS2.Bits.Speaker = 0;	*/
   // 

	/////// ������ ///////////////////////////////////
	if(!GET_BRAKE_STATUS)			// SW2				 // �ƶ�	 
		JT808ExtenVsFlag.VS1.Bits.Brake = 1;
	else						
		JT808ExtenVsFlag.VS1.Bits.Brake = 0;

	if(!GET_DIPPED_BEAM)			 // 	 SW4	 // �����	  
		JT808ExtenVsFlag.VS1.Bits.DippedHeadLight = 1;
	else
		JT808ExtenVsFlag.VS1.Bits.DippedHeadLight = 0;		 

	// modified 2015.1.8
	if(!GET_HIGH_BEAM)			//		SW5	 //  Զ���
	   JT808ExtenVsFlag.VS1.Bits.HighLight = 1;
	else
	  JT808ExtenVsFlag.VS1.Bits.HighLight = 0;

	if(!GET_LEFT_LIGHT)		// SW6							 //��ת��� 
	   JT808ExtenVsFlag.VS1.Bits.LTurnLight = 1;
	else
	   JT808ExtenVsFlag.VS1.Bits.LTurnLight = 0;

	if(!GET_RIGHT_LIGHT)					// SW7				 //��ת���	
	   JT808ExtenVsFlag.VS1.Bits.RTurnLight = 1;
	else
	   JT808ExtenVsFlag.VS1.Bits.RTurnLight = 0;

		
	return Tp.B16[0];
}




/****************************************************************************************************
** �������ƣ�CheckMotoStop()
** ����������������ֹͣ���
** ��ڲ�������
** ���ڲ�������
** ȫ�ֱ���: ��
** �޸�ʱ�䣺2009.08.21   
*****************************************************************************************************/ 
void		SwitchCls::CheckMotoStop(void)								  //50ms
{
	if(MOTO_Status)	MotoRotateSpeedCnt++;	
}



/****************************************************************************************************
**	�������ƣ�CheckMotoRunOrNot
** ������������ⷢ�����Ƿ��ڹ���
** ��ڲ�������
** ���ڲ�������
** ȫ�ֱ���: ��
** �޸�ʱ�䣺
****************************************************************************************************/ 	
void			SwitchCls::CheckMotoRunOrNot(void)								// 1���ֵ���һ��
{
	
	char		Puls[16];
	
	static	volatile	 uint32	MotoRunSecondCnt;
	NOP();	NOP();	NOP();
		
	if((MotoRotateSpeedCnt>10))		// 10���������ϻ�ά�ָߵ�ƽ2S����		 //||MOTO_Statu)
	{
		MotoRotateSpeedCnt = 0;
		VehicleStatus.VS3.Bits.Moto = 1;
		if ( JT808Flag.VS1.Bits.Operations == 0)
		{
			JT808_GPRS_TempPack = TRUE;
			Uart.puts("TP. Moto off");
		}

		JT808Flag.VS1.Bits.Operations = 1;		  //��Ӫ״̬
		MotoRunFlag = 1;
		MotoRunSecondCnt++;

		if(MotoRunSecondCnt>=60)
		{
			MotoRunSecondCnt = 0;						
			SystemLog.VDR.MotoRunTimeCnt += 60;	// �˴����洢,ÿ��30��ͳһ�洢
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

		JT808Flag.VS1.Bits.Operations = 0;		 //ͣ��״̬
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
**	�������ƣ�CheckSwitchAlarm()
** �����������������������
** ��ڲ�����IO����״̬SwStatus
** ���ڲ�������
** ȫ�ֱ���: ��
** �޸�ʱ�䣺2009.09.04  
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
		// ϵͳ����11���Ժ�ACC�ɿ����أ���������ź�
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
			if(AccPreStatus==1)		// �б仯������	 ACC ����Ҫ����
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
		}							 //ACC�����ް�������ʱ��60���������߹ص���ͨ��ģ�������������Դ������GPSģ��	����ỽ�����ߣ�ͨ������������������
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
			
			if(AccPreStatus==0)		// �б仯������
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
				NVIC_SystemReset();					  // ����
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
** ��������: TH_VehicleControlOutput()
** ��������: �������������
** �������: ��
** �������: ��
** ȫ�ֱ���: ��
** �޸�ʱ��: 2009.08.31
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
	


	if( MotoRunFlag )		// ���跢��������ʱ��������������VehicleControl.Lock = LOCK_VEHICLE_ENABLE_INV;
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
	else		// ������ֹͣ
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
	
	/*if(SimuSpeed.Enable==ENABLE_FLAG)							   //��ģ���г�ʱ����·�������ɲ�����
	{	  		
		if(SimuSpeed.Brake)	{	SIMU_BRAKE_OUT_ON;	}
		else						{	SIMU_BRAKE_OUT_OFF;	}
	}
	else */
	if(JT808Flag.VS2.Bits.OffOil==1)
	{	STOP_OIL_CONTROL;	}
	else
	{	REGAIN_OIL_CONTROL;	}
	
	
	
	///////////////////// ������·�����ֱ��ʹ�ù���������������ƽ������//////////////////////////////
	//////////////////////////////////////////  ���������
	if(VehicleControl.ControlOut1==0)			// �����Ч
	{
		PulsStatus = 1;
	}
	else if(VehicleControl.ControlOut1>MAX_PLUS_PERIOD)	// �����Ч
	{
		PulsStatus = 0;
	}
	else			// �������
	{
		if(TickCnt%VehicleControl.ControlOut1==0)		// PulsOutEnable�����ڣ���λ10ms
		{
			PulsStatus = !PulsStatus;		// ȡ��
		}
	}
	
		
	
//	if(SimuSpeed.Enable!=ENABLE_FLAG)			  //��ģ���г�ʱ����·��������ٶ��������
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
	//////////////////////////////////////////  ���������
	if(VehicleControl.ControlOut2==0)			// �����Ч
	{
		PulsStatus = 1;
	}
	else if(VehicleControl.ControlOut2>MAX_PLUS_PERIOD) 	// �����Ч
	{
		PulsStatus = 0;
	}
	else			// �������
	{
		if(TickCnt%VehicleControl.ControlOut2==0)		// PulsOutEnable�����ڣ���λ10ms
		{
			PulsStatus = !PulsStatus;		// ȡ��
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

	//////////////////////////////////////////  �������3
	if(VehicleControl.ControlOut3==0)			// �����Ч
	{
		PulsStatus = 1;
	}
	else if(VehicleControl.ControlOut3>MAX_PLUS_PERIOD)	// �����Ч
	{
		PulsStatus = 0;
	}
	else			// �������
	{
		if(TickCnt%VehicleControl.ControlOut3==0)		// PulsOutEnable�����ڣ���λ10ms
		{
			PulsStatus = !PulsStatus;		// ȡ��
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


//////////////////////////// �����Ǳ�����ⲿ�ִ��� //////////////////////////////////////////////


/*************************************************************************************************
** ��������: CheckRobAlarm()
** ��������: �پ����
** �������: ��
** �������: ��
** ȫ�ֱ���: ��
** ����ʱ�䣺2009.09.02
** �޸�ʱ��: 
************************************************************************************************/
void		SwitchCls::CheckRobAlarm()
{
	static	uchar		Pressflag=0,Alarmflag=0;
	static	uint32	LastTime;
	uchar		Index = 0;   // ��Ӧ���ñ����ֵĵڼ�λ


	//if(Enable.EN1.Bits.RobAlarm==1)																			// �������ʹ�ýپ�		JT808 ʼ������
	{
		//if(VehicleStatus.VS4.Bits.RobAlarm==0)													// δ�����پ�
		if(JT808Flag.VS5.Bits.EmergencyAlarm==0)
		{
			
			if((MainPower_Status !=0) && (Button_Status==1)&&(Pressflag==0))		// �������Դ�������Ұ��½�����ť, 
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
					if (JT808_Alarm_Word.SendTextSMS & (0x01 << Index))		     								// ���Ͷ���
					{
						GSM.SetSMS_Buffer((char *)&JT808_Number.TextSMS[0],"Emergency	Alarm!!!");		
					}
					//if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))	Camera.SetTakeNumber(2,TRUE);		  // ����
					if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))
				   {
			  			if (!(JT808_Alarm_Word.SavePhoto & (0x01 << Index)))	Jt_photo_upload = 1;		  // �ϴ���ƽ̨
						MediaEventFlag = eBB_MEDIA_EVENT_ROB;
			  			Camera.SetTakeNumber(2,TRUE);		  // ���� 
		  		   } 
					#endif	

					Uart.puts("GTP rob  Alarm\r\n\0");				  																																								// �پ�����ʱ���ű�־
				 }
			 }
		}
		else
		{
		 	Alarmflag=0;
			if((Button_Status==1)&&(Pressflag==0))									// ������½�����ť
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
	 	if((gCnt.SystemSecond-LastTime)>10)											// 10���,�������ٰ��°�ť
		{
			Pressflag=0;
		}
	}

}

/*************************************************************************************************
** ��������: CheckStealAlarm()
** ��������: �������
** �������: ��
** �������: ��
** ȫ�ֱ���: ��
** ����ʱ�䣺2009.09.04
** �޸�ʱ��: 2009.09.10
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
		  #if ( TEST_CODE_JUST_FOR_TEST_LUOYANG==1)	  // Ӧ������ʹ��
			NOP();	NOP();
			#else
		//	jflsja f
		  if ((JT808_Alarm_Word.SendTextSMS >> Index) & 0x01 )	     								// ���Ͷ���
		  {
			 GSM.SetSMS_Buffer((char *)&JT808_Number.TextSMS[0],"Steal	Alarm!!!");		
		  }
		  //if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))	Camera.SetTakeNumber(2,TRUE);		  // ���� 
		  if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))
		  {
	  			if (!(JT808_Alarm_Word.SavePhoto & (0x01 << Index)))	Jt_photo_upload = 1;		  // �ϴ���ƽ̨
				MediaEventFlag =eBB_MEDIA_EVENT_OTHER;
	  			Camera.SetTakeNumber(2,TRUE);		  // ���� 
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
	//if(Enable.EN1.Bits.StealAlarm==1)																// �������������,�������״̬	  JT808 Ĭ�ϼ���ⲿ�����ź�
   if(0)
	{
		if(SetScanAlarmFlag==1)
		{	if(ACC_Status==1)												   			// ������ֵ��
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
																			// ������,�رձ�������
		}
	}
	#endif

}


/*************************************************************************************************
** ��������: CheckTrailCarAlarm()
** ��������: �ϳ����							
** �������: ��
** �������: ��
** ȫ�ֱ���: ��
** ����ʱ�䣺2009.09.04
** �޸�ʱ��: 2009.09.10
ע��JT808����Ҫ��ACC��GPSģ�����ػ�״̬�������Ͳ��ܼ���ϳ������ˣ����ԣ�������ڰ���
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
	//if(Enable.EN1.Bits.TrailCarAlarm==1)					// ��������ϳ����		 JT808 Ĭ�ϼ���ϳ�
	{
		//if(VehicleStatus.VS6.Bits.CarBeTrail==0
		if(JT808Flag.VS8.Bits.IllegalMove==0)										  		
		{
			if((GPSValidFlag==GPS_VALID_FLAG)&&(ACC_Status==0))			// ��λ��Ч,��ACCϨ��
			{
				/*speed = GPS.GetSpeedX100();
				if(speed>500)										  // ������ֳ���λ���ƶ�
				{
					SpeedCnt++;
				}
				else
					SpeedCnt = 0;
				if( (SpeedCnt>200))//&&(speed!=200)&&(speed!=300) )	// �ϳ����.(����/Сʱ)		
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
					#if ( TEST_CODE_JUST_FOR_TEST_LUOYANG==1)	  // Ӧ������ʹ��
					NOP();	NOP();
					#else
					//jflsja f
					if (JT808_Alarm_Word.SendTextSMS & (0x01 << Index))		     								// ���Ͷ���
					{
						GSM.SetSMS_Buffer((char *)&JT808_Number.TextSMS[0],"Trail Car Alarm!!!");		
					}
					//if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))	Camera.SetTakeNumber(2,TRUE);		  // ����
					if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))
				   {
			  			if (!(JT808_Alarm_Word.SavePhoto & (0x01 << Index)))	Jt_photo_upload = 1;		  // �ϴ���ƽ̨
						MediaEventFlag =eBB_MEDIA_EVENT_OTHER;
			  			Camera.SetTakeNumber(2,TRUE);		  // ���� 
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
** ��������: CheckNoSetScanAlarm()
** ��������: δ������
** �������: ��
** �������: ��
** ȫ�ֱ���: ��
** ����ʱ�䣺2009.09.04
** �޸�ʱ��: 
************************************************************************************************/
void		SwitchCls::CheckNoSetScanAlarm()					 							// 50ms
{
	#if ( TEST_CODE_JUST_FOR_TEST_LUOYANG == 1)
	NOP();	NOP();
	#else
	static	uchar		flag=0;
	static	uint32	LastTime;
	
	if(EnableFlag.EN1.Bits.NoSetScanAlarm==1)																// �������δ������
	{
		if((SetScanAlarmFlag==0)&&(VehicleStatus.VS3.Bits.ACC==1))					// ͣ��δ���
		{
			if(VehicleStatus.VS6.Bits.UndefendAlarm==0)										// ���û�з���δ�������
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
		
				if(((gCnt.SystemSecond-LastTime)>15)&&(flag==1))											// 15��
				{		
					VehicleStatus.VS6.Bits.UndefendAlarm = 1;								// ��������
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
					//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$	 																		// �����������򿪱�������
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
** ��������: CheckHelpAlarm()
** ��������: �����������
** �������: ��
** �������: ��
** ȫ�ֱ���: ��
** ����ʱ�䣺2009.09.04
** �޸�ʱ��: 
************************************************************************************************/
void		SwitchCls::CheckHelpAlarm()					 							// 50ms
{
	uchar		i;
	static	uchar		PressFlag=0,AlarmFlag=0;
	static	uint32	LastTime;
	
	if(EnableFlag.EN1.Bits.HelpAlarm==1)																// ������������������
	{
		if(GG_Status.Registration!=TRUE)	return ;
		
		if(((Button_Status==1)||(WakeUpType==WAKE_TYPE_HELP))&&(PressFlag==0))							// ������½�����ť
		{						
			PressFlag = 1;
			LastTime = gCnt.SystemSecond;

			if(AlarmFlag==0)
			{
				AlarmFlag=1;
				VehicleStatus.VS6.Bits.CallHelpAlarm = 1;								// ��������
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
				//Uart.puts("׼����������...\r\n\0");	Uart.PopAlignmentSend();
				GPRS.DialHelpPhone();													// ����Ԥ��õ������绰
			}
			else
			{
			 	AlarmFlag=0;
				VehicleStatus.VS6.Bits.CallHelpAlarm = 0;								// ��������
				VehicleStatus.VS4.Bits.RobAlarm = 0;
				for(uchar i=0;i<8;i++)									 				// ��Ҫʱ����һ�
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
		 	if((gCnt.SystemSecond-LastTime)>10)											// 10���,�������ٰ��°�ť
			{
				PressFlag=0;
			}
		}
	}
	
}




/*************************************************************************************************
** ��������: CheckTireDriveAlarm()
** ��������: ƣ�ͼ�ʻ����
** �������: ��
** �������: ��
** ȫ�ֱ���: ��
** ����ʱ�䣺2009.09.08
** �޸�ʱ��: 
************************************************************************************************/
void		SwitchCls::CheckTireDriveAlarm()
{
	static	uchar		flag=0;
	static	uint32	LastTime;

	if(EnableFlag.EN1.Bits.TireDriveAlarm==1)														// �������ƣ�ͼ�ʻ����
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
	
			 	if((gCnt.SystemSecond-LastTime)>3600*4)								// 4Сʱ��
			 	{
					VehicleStatus.VS6.Bits.FTdrivingAlarm = 1;									// ����
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
** ��������: CheckIllegalOpenDoor()
** ��������: �Ƿ����ż��
** �������: ��
** �������: ��
** ȫ�ֱ���: ��
** ����ʱ�䣺2009.09.08
** �޸�ʱ��: 
************************************************************************************************/
void		SwitchCls::CheckIllegalOpenDoor()
{
	static	uint16 Cnt=0;
	
	if((EnableFlag.EN2.Bits.IllegalOpenDoor==1)&&(DOOR_Status==0))				   	// �������Ƿ����ű��������ű���
	{ 
		if(SetScanAlarmFlag==1)											// ��������
		{
			if( (VehicleStatus.VS3.Bits.DoorOpen==0)|| (WakeUpType==WAKE_TYPE_DOOR) )		// ��������,�������Ŵ�
			{
				Cnt++;
				if(Cnt>10)
				{
					Cnt = 0;
					VehicleStatus.VS3.Bits.DoorOpen = 1;		// �����Ƿ����ű���
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
** ��������: CheckIllegalMakeFire()
** ��������: �Ƿ������
** �������: ��
** �������: ��
** ȫ�ֱ���: ��
** ����ʱ�䣺2009.09.08
** �޸�ʱ��: 
************************************************************************************************/
void		SwitchCls::CheckIllegalMakeFire()
{
	uchar Index = 27;
	//if(Enable.EN2.Bits.IllegalMakeFire==1)																		// �������Ƿ���𱨾�
	{ 
		if(SetScanAlarmFlag==1)																			// ���������
		{
			//if( ((VehicleStatus.VS4.Bits.LawlessPowerOn==0)&&(ACC_Status==1))\
			if( ((JT808Flag.VS8.Bits.IllegalPowerOn==0)&&(ACC_Status==1))\
			     || (WakeUpType==WAKE_TYPE_ACC) )				// ����������
			{
				JT808_GPRS_TempPack = TRUE;
				Uart.puts("TP. ACC on");
				#if ( TEST_CODE_JUST_FOR_TEST_LUOYANG==1)	  // Ӧ������ʹ��
				NOP();	NOP();
				#else
				//jflsja f
				if (JT808_Alarm_Word.SendTextSMS & (0x01 << Index))		     								// ���Ͷ���
				{
					GSM.SetSMS_Buffer((char *)&JT808_Number.TextSMS[0],"Trail Car Alarm!!!");		
				}
				//if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))	Camera.SetTakeNumber(2,TRUE);		  // ����
				if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))
			   {
		  			if (!(JT808_Alarm_Word.SavePhoto & (0x01 << Index)))	Jt_photo_upload = 1;		  // �ϴ���ƽ̨
					MediaEventFlag =eBB_MEDIA_EVENT_OTHER;
		  			Camera.SetTakeNumber(2,TRUE);		  // ���� 
	  		   } 
				#endif
				JT808Flag.VS8.Bits.IllegalPowerOn = 1;
				VehicleStatus.VS4.Bits.LawlessPowerOn = 1;										// �Ƿ���𱨾�
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
** ��������: CheckMachineSlfe()
** ��������: �ն��Լ�
** �������: ��
** �������: ��
** ȫ�ֱ���: ��
** ����ʱ�䣺2009.09.08
** �޸�ʱ��: 
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
		//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$																						// ��ȡ����ʱ��ϵͳʱ��
	 }

	if(MOTO_Status)		return;		// ������������������
	 
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
		for(i=0;i<100;i++)													  				// �ȴ�30��,�ϴ�λ����Ϣ���
		{
		 	RESET_WATCH_DOG;
			os_dly_wait(30);
		}
			
		//if((SetScanAlarmFlag==0)&&(ACC_Status==1))									// �������,�ҳ�Կ�׿� (�����е�,����ʱ���׼ȷ��ʱ���Բ�����)
		//{

			Uart.puts("self test while1\r\n\0");
			Uart.PopAlignmentSend();
			os_dly_wait(100);
		 	NOP();	NOP();	NOP();	NOP();
			Switch.SaveVehicleStatus();			// 
			DisableIRQ();
			NOP();	NOP();	NOP();	NOP();
			for(;;)																					// ����һ��
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
** ��������: CheckShakeAlarm()
** ��������: �𶯼��
** �������: ��
** �������: ��
** ȫ�ֱ���: ��
** ����ʱ�䣺2009.09.08
** �޸�ʱ��: 
************************************************************************************************/
static	uint32	ShakeCnt;
void		SwitchCls::CheckShakeAlarm()
{
	if(EnableFlag.EN2.Bits.CheckShake==1)																				// ��������𶯼��	
	{ 
		if(SetScanAlarmFlag==1)																			// ������
		{
			if((VehicleStatus.VS6.Bits.VibrationAlarm==0)&&((IO_Status&swSHAKE)!=0))			// ������
			{
				if(++ShakeCnt>50)
				{
					VehicleStatus.VS6.Bits.VibrationAlarm = 1;												// �𶯱���
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
** ��������: CheckLoginAlarm()
** ��������: ��½������󱨾����
** �������: ��
** �������: ��
** ȫ�ֱ���: ��
** ����ʱ�䣺2009.09.08
** �޸�ʱ��: 
************************************************************************************************/
void		SwitchCls::CheckLoginAlarm()
{
	// ���ֱ�����ģ����ʵ��


}

/*************************************************************************************************
** ��������: CheckMainPower()
** ��������: ����Դ������
** �������: ��
** �������: ��
** ȫ�ֱ���: ��
** ����ʱ�䣺2009.09.08
** �޸�ʱ��: 
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
			if (1) 		  // ����󱨾�
			#else
			if((gCnt.SystemSecond-LastTime)>0)		  // ����2��󱨾�
			#endif
			{													 
				VehicleStatus.VS1.Bits.MainPowerLost = 1;
				JT808Flag.VS6.Bits.MainPowerOff = 1;						
				LastTime = gCnt.SystemSecond;
				Uart.puts("TP.Main power off");
				JT808_GPRS_TempPack = TRUE;
				
				PowerStatus.Status = 2;  // �ϵ�

				#if ( TEST_CODE_JUST_FOR_TEST_LUOYANG==1)
				NOP();	NOP();
				#else
				//fdsfs 
				if (JT808_Alarm_Word.SendTextSMS & (0x01 << Index))		     								// ���Ͷ���
					{
						GSM.SetSMS_Buffer((char *)&JT808_Number.TextSMS[0],"Main Power Off Alarm!!!");		
					}
				//if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))	Camera.SetTakeNumber(2,TRUE);		  // ���� 
				if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))
			   {
		  			if (!(JT808_Alarm_Word.SavePhoto & (0x01 << Index)))	Jt_photo_upload = 1;		  // �ϴ���ƽ̨
					MediaEventFlag =eBB_MEDIA_EVENT_OTHER;
		  			Camera.SetTakeNumber(2,TRUE);		  // ���� 
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
			PowerStatus.Status = 1;  // ͨ��
		 	Uart.puts("TP.Main power Restoration");
		}
			
		JT808Flag.VS6.Bits.MainPowerOff = 0;				
	}

	// 2015.1.22	 // �����ⲿ����״̬
	if (PowerStatus.Status)
	{
		//if ((TickCnt<1000)&&(PowerStatus.Status==2)) return;	// ����10s���������е����¼

		Rtc.GetRTC_BCD((RTC_STR *)tBuffer);
		memmove(PowerStatus.Time,tBuffer, 6);
		DFE.Read(FM_EXT_POWER_STATUS_CNT,4,(uchar*)&tl);
		Addr = tl%36;							  // 	 36��
		Addr *= LEN_EXT_POWER_STATUS;
		Addr += FM_EXT_POWER_STATUS;		  	
		DFE.Write( Addr, LEN_EXT_POWER_STATUS,(uchar *)&PowerStatus);
		tl++;
		DFE.Write(FM_EXT_POWER_STATUS_CNT,4,(uchar*)&tl);	 	// ��ʻԱ����ǳ���������1��洢������	
		//
		Uart.puts("�����ⲿ�����¼������ "); 
		if ((tl)&&(tl%36)==0)
		{
			// ���������һҳ��Flash�Ŀռ䣬��ŵ�Flash��
			if( (FlashPageCnt.PowerOnOffSave%FF_PAGE_PER_SECTOR)==0)
			{
				Addr = FF_POWER_ON_OFF_START_PAGE;
				Addr += FlashPageCnt.PowerOnOffSave%FF_POWER_ON_OFF_TOTAL_PAGE;
				
				DFF.SectorEraseWaitOK(Addr/FF_PAGE_PER_SECTOR,10);
			}				
			///////////////////////////////////////////�洢��Flash  ����1ҳflash����36���ⲿ�����¼
			DFE.Read(FM_EXT_POWER_STATUS,LEN_EXT_POWER_STATUS*36,tBuffer); // 
	
			Addr = FF_POWER_ON_OFF_START_PAGE;
			Addr += FlashPageCnt.PowerOnOffSave%FF_POWER_ON_OFF_TOTAL_PAGE;
			DFF.PageProgramWaitOK(Addr,tBuffer,3);
			FlashPageCnt.PowerOnOffSave++;

			DFE.Write( FM_FLASH_PAGE_CNT, LEN_FLASH_PAGE_CNT, (uchar*)&FlashPageCnt);

	
	  		Uart.puts("�����ⲿ�����¼��Flash"); 
			Uart.puts("Addr Test Data:"); Uart.putb(1,4, (uchar *)&Addr); Uart.puts("\r\n");
		
		}
		
	}
}



/*************************************************************************************************
** ��������: CheckUnlockAlarm()
** ��������: ���δ���ű���
** �������: ��
** �������: ��
** ȫ�ֱ���: ��
** ����ʱ�䣺2009.09.08
** �޸�ʱ��: 
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
		if(((gCnt.SystemSecond-LastTime)>15)&&(flag==1))											// 15��
		{		
			LastTime = gCnt.SystemSecond;
			if(LockDoorSignal!=1)											 // 
			{
				if(VehicleStatus.VS8.Bits.DoorUnlockAlarm==0)
					GPRS_TempPack = TRUE;
				VehicleStatus.VS8.Bits.DoorUnlockAlarm = 1;								// ��������				
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
** ��������: CheckHandleWatchCar()
** ��������: �ֱ��������
** �������: ��
** �������: ��
** ȫ�ֱ���: ��
** ����ʱ�䣺2009.10.21
** �޸�ʱ��: 
************************************************************************************************/
void		SwitchCls::CheckHandleWatchCar()
{
	static	uchar		flag=0;
	static	uint32	LastTime,HoldPhoneTime;
	
	if(EnableFlag.EN2.Bits.HandleWatchCar==1)									  				// ��������ֱ�����
	{
		 if(HandleWatchCarMode==1)									  				// �Զ�����
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
						SetScanAlarmFlag = 1;										// �Զ����
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
				//////// ���� ///////////////////////////
				if(HandleLoginSuccess!=1)						  				// ��������¼���
				{
					if(HandleHoldFlag==1)
					{
						HoldPhoneTime = gCnt.SystemSecond;
						HandleHoldFlag = 0;
					}
					if((gCnt.SystemSecond-HoldPhoneTime)>15)						// 15s
					{
						//DisplayLcd.SendDataToLcd(0xa5,0,NULL);					// �ֱ�����
						HoldPhoneTime = gCnt.SystemSecond;
					}
				}
			 }
		 }
		 else																 	 			// �ֶ�����
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
					//DisplayLcd.SendDataToLcd(0xa5,0,NULL);					// �ֱ�����
					HoldPhoneTime = gCnt.SystemSecond;
				}
			 }
		 }	
	}
}



/*************************************************************************************************
** ��������: PowerSaveMode()
** ��������: ʡ��ģʽ����
** �������: ��
** �������: ��
** ȫ�ֱ���: ��
** ����ʱ�䣺2009.09.08
** �޸�ʱ��:
	ACC_Status		= !GET_ACC_STATUS;							  	  // ���ʱ,��־Ϊ1
	MOTO_Status 	= !GET_MOTO_STATUS;								  // ����ʱ,��־Ϊ1
	DOOR_Status 	= !GET_DOOR_STATUS;								  // ����ʱ,��־Ϊ1
	Button_Status 	= !GET_BUTTON_STATUS;	 
************************************************************************************************/
void		SwitchCls::PowerSaveMode()					// ÿ��ִ��1��
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
	
	if((Idle.SecondCnt%((Time-2)*60)==0))		// ����ʡ��ģʽǰ����һ��ʡ���־����Ϣ
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



/////////////// ʡ��ģʽ���� //////////////////////////
void			SwitchCls::WakeUp(void)
{
	static	uint32	IdleTaskCnt=0;
	uchar		t,type;
	
	IdleTaskCnt++;
	//if(IdleTaskCnt%20==0)
	gCnt.SystemSecond++;
										  
	/////////// ��ȡI/O���� ////////////////////////////
	GET_IO_PIN_INIT;
	////////////////////////// ����
	if( ((GET_ACC_STATUS)==0) || ((GET_MOTO_STATUS)==0)||( gCnt.SystemSecond>(Idle.WakeUpTime*60))     )
	{																		
		type = WAKE_TYPE_NOTHING;
		if((GET_ACC_STATUS)==0)			type = WAKE_TYPE_ACC;							// ��¼���ѵ�ԭ��
		if((GET_MOTO_STATUS)==0)		type = WAKE_TYPE_MOTO;
		//if((GET_BUTTON_STATUS)==0)		type = WAKE_TYPE_HELP;
		//if(GET_DOOR_STATUS==1)			type = WAKE_TYPE_DOOR;
		if(gCnt.SystemSecond>(Idle.WakeUpTime*60))	type = WAKE_TYPE_TIME;
				


		//os_dly_wait(100);		
	 	t = 0;	// �´�����������ʡ��ģʽ�����������������ĵط��޸ģ�	                                
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
	
		
	//////////////////////////////////////// ʡ��ģʽ�ĵ���
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
** ��������: AutoLockDoor()
** ��������: �Զ�����
** �������: ��
** �������: ��
** ȫ�ֱ���: ��
** ����ʱ�䣺2009.09.08
** �޸�ʱ��: 
************************************************************************************************/
void		SwitchCls::AutoLockDoor()
{
	static	uchar		flag=0;
	uchar 	speed;

	if(EnableFlag.EN2.Bits.AutoLockDoor==1)																	// ��������Զ�����
	{					  
		if(flag==0)
		{
			// ������ʽ1
			if(IO_Status&swBRAKE)																	// ��⵽ɲ���ź�
			{
				os_dly_wait(1);																		// ��ʱ10MSȥ������
				if(IO_Status&swBRAKE)																// �ٴμ��ɲ���ź�
				{
					if((ACC_Status==1)&&(LockDoorSignal==2))		 					// ACC����û������
					{	
						LockDoorFlag=1;																// ����		
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
			// ������ʽ2 	
			if(GPSValidFlag==GPS_VALID_FLAG)
			{
				speed = GPS.GetSpeed();																
				if((speed>16)&&(LockDoorSignal==2))
				{																							// ������ǿ���
					LockDoorFlag = 1;																	// ����
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
		  //if((LockDoorSignal==1)&&((IO_Status&swACC)==0))						// ����������ҳ�Կ�״�
		  //{
		  //		flag = 0;	
		  //}
		  if(LockDoorFlag!=1)														// �������		
		  {
				flag = 0;	
		  }
		}
	}
}


extern	"C"		void       Delay_mS(unsigned char  c);
/*************************************************************************************************
** ��������: CheckRadioControl()
** ��������: ����ң�ؽ���
** �������: ��
** �������: ��
** ȫ�ֱ���: ��
** ����ʱ�䣺2009.09.02
** �޸�ʱ��: 
************************************************************************************************/
//static		uint32		KeyCnt1=0,KeyCnt2=0,NoKeyCnt=0;
void			SwitchCls::CheckRadioControl(uchar	sleep)										// 50ms
{
	//uchar		tmp,Data;	
   


}






/*************************************************************************************************
** ��������: SaveVehicleStatus()
** ��������: д����״̬��־������
** �������: ��
** �������: ��
** ȫ�ֱ���: ��
** �޸�ʱ��: 2009.12.19
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
** ��������: GetVehicleStatus()
** ��������: ��ȡ����״̬
** �������: ��
** �������: ��
** ȫ�ֱ���: ��
** �޸�ʱ��: 2009.12.19
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
	GPRS.PushAlignment(1,strlen(buf2),(uchar *)buf2);					// ��һ��״̬���
	VehicleStatus.VS1.Bits.IdleMode = 0;
	//Uart.putb(1,4,buf);
	//Uart.putb(1,strlen(buf2),(uchar *)buf2);	 */
}

SwitchCls::SwitchCls(void)
{
	
}


/********************************  End of File *************************************************/
