
#include "kionix_kxtf9_common.h"	
#include "kionix_kxtf9_driver.h"
#include "..\IIC\iic0.h"
#include "..\Uart\Uart0.h"	
#include			"..\GSM_GPRS\GSM_GPRS.h"
#include "..\Camera\Camera.h"	 

//extern uchar	JT_read_direction;
//extern uchar	JT_read_motion_event;
uchar	accel_tilt_pos;
uchar	accel_tap_pos;

static uchar event = ACCEL_NO_EVENT;





/*----------------------------------------------------------------------------
  Read I2C in byte
 *----------------------------------------------------------------------------*/
uchar I2C_read_for_accel      (uchar SAddr, uchar RAddr, uchar Len, uchar *data)
{
	uchar ret;

	if (I2C_Busy_Now_Flag == 1) { os_dly_wait(1); }
	I2C_Busy_Now_Flag = 1;
	if (Current_Use_4442_Flag == 1) 		// ���ʹ�ù�sle4442IC��������Ҫ����ͨģʽ�л���I2Cģʽ
	{
		Current_Use_4442_Flag = 0;  
		I2C0_PIN_SELECT;
		os_dly_wait(5);
	}
	

	SAddr = SAddr << 1;
	ret = I2C0.ReadEEPROM(SAddr, RAddr, Len, data);

	I2C_Busy_Now_Flag = 0;

	if (ret == OK)
	{
		//Uart.puts("CCCCC1");
		//os_dly_wait(1);

		return TRUE;
	}

		//Uart.puts("EEE1");
		//os_dly_wait(1);

	return FALSE;

	

}


/*----------------------------------------------------------------------------
  write I2C in byte
 *----------------------------------------------------------------------------*/
uchar I2C_write_for_accel      (uchar SAddr, uchar RAddr, uchar Len, uchar *data)
{
	uchar ret;

	if (I2C_Busy_Now_Flag == 1) { os_dly_wait(1); }
	I2C_Busy_Now_Flag = 1;
	if (Current_Use_4442_Flag == 1) 
	{
		Current_Use_4442_Flag = 0;  
		I2C0_PIN_SELECT;
		os_dly_wait(5);
	}
		
	

	SAddr = SAddr << 1;
	ret = I2C0.WriteEEPROM(SAddr, 0, RAddr, Len, data);

	I2C_Busy_Now_Flag = 0;

	if (ret == OK)
	{
		//Uart.puts("CCCCC2");
		//os_dly_wait(1);
		
		return TRUE;

	}

	//Uart.puts("EEE2");
	//	os_dly_wait(1);

	return FALSE;
}


void accel_default_para(void)
{
	ACCEL_INFO accel_info;

  	// System Feature	 Settings
	accel_info.resolution = 12;
	accel_info.g_range = 8;  //2;		����JT808�涨��ײ���ٶȷ�ΧΪ��0 - 79 , ��0g - 7.9g �����������������Ϊ8g

	// Tillt Settings
	accel_info.tiltinfo.enable = 1;		 // 0 �����ã�����������
	accel_info.tiltinfo.ODR = 12; 		// 50;		 	// 50Hz
	accel_info.tiltinfo.timer = 0x01;//10;		// 0.2s
	accel_info.tiltinfo.angle = 27; 		// Ϊ0(26��), 27(60��)�� ��ʹ��Ĭ��ֵ0x0c  26��   c = (uchar)(sin((float)(c) / 180 * 3.14) * 32);	
	accel_info.tiltinfo.hysteresis = 0;  //0x14  +/- 15��
	
	// Motion Detect Settings
	accel_info.wufinfo.enable = 0;		 // 1 ���ã� 0������ �� Ŀǰ��G11û���øù���
	accel_info.wufinfo.ODR = 50;  		// 50: 50Hz
	accel_info.wufinfo.timer = 100;		// 2s
	accel_info.wufinfo.thresh = 100; 		//0x08;   // 0.5g

	// Tap Double Tap(TDT) Settings
   accel_info.tdtinfo.enable = 1;
   accel_info.tdtinfo.ODR = 5;          		// ����Ϊ50Hz	 (5: 50Hz, 10: 100Hz, 20: 200Hz, 40: 400Hz) 
   accel_info.tdtinfo.tdt_timer = 0;		//0x78;   ��ʾ 0.3s		 ���¼���Ϊ0����ʾʹ��Ĭ��ֵ
   accel_info.tdtinfo.tdt_h_thresh = 0;	//0xb6;	 �����ֵ
   accel_info.tdtinfo.tdt_l_thresh = 150;	//0x1a;	 �����ֵ
   accel_info.tdtinfo.tdt_tap_timer = 0;	//0xa2; // 0.005s
   accel_info.tdtinfo.tdt_total_timer = 0; 	//0x24;  // 0.09s
   accel_info.tdtinfo.tdt_latency_timer = 0; //0x28;	  // 0.1s
   accel_info.tdtinfo.tdt_window_timer = 0;	//0xa0;	  // 0.4s
	
	// Mask info
	//	accel_info.maskinfo.tilt_mask_enable = 1;
	//	accel_info.maskinfo.tdt_mask_enable = 1;
	//	accel_info.maskinfo.axis_mask_enable = 1;
	//	accel_info.maskinfo.mask_le = 1;
	//	accel_info.maskinfo.mask_ri = 1;
	//	accel_info.maskinfo.mask_do = 1;
	//	accel_info.maskinfo.mask_up = 1;
	//	accel_info.maskinfo.mask_fd = 1;
	//	accel_info.maskinfo.mask_fu = 1;
	//	accel_info.maskinfo.mask_tle = 1;
	//	accel_info.maskinfo.mask_tri = 1;
	//	accel_info.maskinfo.mask_tdo = 1;
	//	accel_info.maskinfo.mask_tup = 1;
	//	accel_info.maskinfo.mask_tfd = 1;
	//	accel_info.maskinfo.mask_tfu = 1;
	//	accel_info.maskinfo.x_mask = 1;
	//	accel_info.maskinfo.y_mask = 1;
	//	accel_info.maskinfo.z_mask = 1; 
	accel_info.maskinfo.B32 = 0x0003ffff;		  // 1�� ����  0������/����


	// Interrupt control
	accel_info.intinfo.enable  = 1;		//1 = enable
	accel_info.intinfo.polarity = 1;		//1 = active high
	accel_info.intinfo.latch = 0; 		//1;		//1 = unlatched
	accel_info.intinfo.alt_latch = 0; 	 //0 = wuf int latch response	 1 = no latch

	DFE.Write(EE_ACCEL_PARA_BASE, LEN_ACCEL_PARA, (uchar *)&accel_info);
	NOP(); 	NOP(); NOP();
	
}


/*----------------------------------------------------------------------------
  	 �������ƣ�accel_kionix_init
	 �������ܣ���ʼ�����ٶȴ���������
	 ����������first_use: ����1ʱ�� ��Ϊ�ǵ�һ��ʹ�ã�����Ĭ�ϲ���������
	 			  ���������ȡ����
	 ��������ֵ����
 *----------------------------------------------------------------------------*/
static void accel_kionix_init(uchar	first_use)
{
	ACCEL_INFO accel_info;
	//uchar	size;
		
	kxtf9_reset();
	os_dly_wait(50);
	


	if (first_use)
	{
		accel_default_para();
		DFE.Read(EE_ACCEL_PARA_BASE, LEN_ACCEL_PARA, (uchar *)&accel_info);     //
	}
	else 
	{
		DFE.Read(EE_ACCEL_PARA_BASE, LEN_ACCEL_PARA, (uchar *)&accel_info);     // 

		if ((TD.MainTask == tMAIN_ACCEL)||(TD.TestMode == TEST_MODE))
		{
			Uart.puts("Accelerometer's parameter: ");
			Uart.putb(1, LEN_ACCEL_PARA, (uchar *)&accel_info);	
			os_dly_wait(1);
		}
	}
    
  

   kxtf9_init(&accel_info, 0);

   kxtf9_enable_outputs(); //set sensor to operation mode. Note, this should be the last step to perform

	
}



// ���ٴ������жϴ���
#if (0)
/*************************  interrupt_service_for_accel ************************/

void interrupt_service_for_accel(void)
{
	INT8U direction;
	INT8U interrupt_source;
	INT8U tilt_register=0;
	INT8U tap_register=0;
	INT8U Motion_event;
	//initialize global values
	Motion_event = ACCEL_NO_EVENT; //assume Motion_event & constants are declared globally
	direction = ACCEL_NO_CHANGE; //assume direction & constant are declared globally
	//direction = 0xfa;
	//disable accelerometer interrupt first.
	kxtf9_disable_int();


	//Read from INT_SRC_REG2 to determine the type of events
	//This function also read the INT_REL register which sets interrupt pin 7 to an inactive state
	kxtf9_service_interrupt(&interrupt_source);
	if(interrupt_source & 0x01) //if tilt event
	{
		Motion_event=ACCEL_TILT; //assume Motion_event & constants are declared globally
		//kxtf9_read_position_status (&tilt_register); //read current tilt register
		kxtf9_read_current_tilt_position(&tilt_register);
		switch(tilt_register)
		{
			case 0x01: direction = ACCEL_FACE_UP; //assume direction & constant are declared globally
			break;
			case 0x02: direction = ACCEL_FACE_DOWN;
			break;
			case 0x04: direction = ACCEL_UP;
			break;
			case 0x08: direction = ACCEL_DOWN;
			break;
			case 0x10: direction = ACCEL_LEFT;
			break;
			case 0x20: direction = ACCEL_RIGHT;
			break;
			default: direction = ACCEL_NO_CHANGE;
			//return;
		}

	
	}
	else if(interrupt_source & 0x0C) //if Tap event
	{
		if(interrupt_source & 0x04)
		{
			Motion_event= ACCEL_SINGLE_TAP; //assume Motion_event & constants are declared globally
		}
		else if(interrupt_source & 0x08)
		{
			Motion_event= ACCEL_DOUBLE_TAP; //assume Motion_event & constants are declared globally
		}
		kxtf9_read_tap_direction (&tap_register); //read current tilt register
		switch(tap_register)
		{
			case 0x01: direction = ACCEL_FACE_UP; //assume direction & constant are declared globally
			           break;
			case 0x02: direction = ACCEL_FACE_DOWN;
			           break;
			case 0x04: direction = ACCEL_UP;
			           break;
			case 0x08: direction = ACCEL_DOWN;
			           break;
			case 0x10: direction = ACCEL_LEFT;
						  break;
			case 0x20: direction = ACCEL_RIGHT;
						  break;
			default: direction = ACCEL_NO_CHANGE;
						break;
		}
	}

	JT_read_direction = direction;
	JT_read_motion_event = Motion_event;
	//enable accelerometer interrupt again
	kxtf9_enable_int();
}
#endif






static void 	disposition_tilt_or_tap(uchar status)
{
	uchar 	Index;

	if (JT808_Alarm_Word.Shield & (0x01 << 29))   return;

	if (status == 0)
	{
		if (JT808Flag.VS8.Bits.CrashPreAlarm == 1) 	
		{
			JT808_GPRS_TempPack = TRUE;
			JT808Flag.VS8.Bits.CrashPreAlarm = 0;

		}

		if (JT808Flag.VS8.Bits.TiltPreAlarm == 1)
		{
			JT808_GPRS_TempPack = TRUE;
			JT808Flag.VS8.Bits.TiltPreAlarm = 0;
		}
		//Cnt = 0;
		//JT808Flag.VS8.Bits.CollisionAlarm = 0;
		VehicleStatus.VS6.Bits.VibrationAlarm = 0;

		
	}
	else
	{
		//Cnt++;

		//if (!(JT808_Alarm_Word.Shield & (0x01 << 29)))// && Cnt > 3)				 // 	���3�Σ���Ϊ����ײ ��෭
		{
			//Cnt = 0;
			Index = 0xff;

			if ((JT808Flag.VS8.Bits.CrashPreAlarm == 0) && (event == ACCEL_TAP))
			{
				JT808Flag.VS8.Bits.CrashPreAlarm = 1;
				Index = 29;
				//Uart.puts(" ��ײ���� ");
			}

			if ((JT808Flag.VS8.Bits.TiltPreAlarm == 0) && (event == ACCEL_TILT))
			{
				JT808Flag.VS8.Bits.TiltPreAlarm = 1;
				Index = 30;
			}
			
			if (Index != 0xff)
			{
				JT808_GPRS_TempPack = TRUE;
				
				//JT808Flag.VS8.Bits.CollisionAlarm = 1;
				VehicleStatus.VS6.Bits.VibrationAlarm = 1;
				#if ( TEST_CODE_JUST_FOR_TEST_LUOYANG==1)	  // Ӧ������ʹ��
				NOP();	NOP();
				#else
				//jflsja f	
				if (JT808_Alarm_Word.SendTextSMS & (0x01 << Index))		     								// ���Ͷ���
				{
					GSM.SetSMS_Buffer((char *)&JT808_Number.TextSMS[0], (Index == 29)? "Crash Alarm!!!":"Tilt Alarm");	
						
				}
				//if (JT808_Alarm_Word.TakePhoto & (0x01 << 29))	Camera.SetTakeNumber(2,TRUE);		  // ����
				if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))
				{
			  		if (!(JT808_Alarm_Word.SavePhoto & (0x01 << Index)))	Jt_photo_upload = 1;		  // �ϴ���ƽ̨
	
					MediaEventFlag = eBB_MEDIA_EVENT_CRASH;
			  		Camera.SetTakeNumber(2,TRUE);		  // ���� 
			  }
			  #endif
			}
		}
		
	}

}

#if (0)
static void accel_test_function(void)
{

	interrupt_service_for_accel();

	switch (JT_read_motion_event)
	{
		// �¼�
		case ACCEL_NO_EVENT:	
			Uart.puts("No Event   ");
		break;

		case	ACCEL_TILT:
			Uart.puts("Tilt Event   ");
		break;

		case ACCEL_SINGLE_TAP:
			Uart.puts("Single Tap Event   ");
		break;

		case ACCEL_DOUBLE_TAP:
			Uart.puts("Double Tap Event   ");
		break;
	}

	switch (JT_read_direction)
	{
		///// ����
		case	ACCEL_NO_CHANGE:
			Uart.puts("No Change\r\n\0");
		break;

		case	ACCEL_FACE_UP:
			Uart.puts("Face Up\r\n\0");
		break;

		case	ACCEL_FACE_DOWN:
			Uart.puts("Face down\r\n\0");
		break;

		case	ACCEL_UP:
		Uart.puts("Up\r\n\0");
		break;

		case	ACCEL_DOWN:
		Uart.puts("Down\r\n\0");
		break;

		case	ACCEL_LEFT:
		Uart.puts("Left\r\n\0");
		break;

		case	ACCEL_RIGHT:
		Uart.puts("Rigth\r\n\0");
		break;

	}

	os_dly_wait(1);
		
}
#endif 


void Check_Collision(void)
{
	uchar	tmp;
	//static uchar event = ACCEL_NO_EVENT;
	static uchar Cnt = 0;

	Cnt++;


	// TEST
//	if (TD.MainTask == tMAIN_ACCEL) 
//	{	 
//		TD.MainTask = 0;
//		if(kxtf9_read_current_tilt_position(&tmp)==TRUE)
//		{
//			Uart.puts("��������  Test Data:"); Uart.putb(1,1,&tmp); Uart.puts("\r\n");
//		} 
//	}
	//

	// ��ײ��෭���

	if (accel_interrupt_flag == 1)
	{
		accel_interrupt_flag = 0;
		kxtf9_disable_int();
	
		kxtf9_service_interrupt(&tmp);		//
	
		if ((tmp & 0x01) && (accel_tilt_pos != 0x0c))	// accel_tilt_pos Ϊ��Чֵʱ
		{
			tmp = 0xc0;   // ����һ����Чֵ	
			if (kxtf9_read_current_tilt_position(&tmp) == TRUE)
			{
				 //Uart.puts("CCC1"); Uart.putb(1, 1, &accel_tilt_pos); Uart.putb(1, 1, &tmp);	os_dly_wait(1);
				//if (accel_tilt_pos == tmp)	{ disposition_tilt_or_tap(0); } // ״̬û�иı�	, û�в෭
				//else 								{ disposition_tilt_or_tap(1);	 );
				if (accel_tilt_pos != tmp)	
				{ 
					event = ACCEL_TILT;
					disposition_tilt_or_tap(1); 
					
					if ((TD.MainTask == tMAIN_ACCEL)||(TD.TestMode == TEST_MODE))
					{
						Uart.puts("Tilt Event");
						os_dly_wait(1);
					}
				}
			}

			
	
			
	   }
	   //else if ((tmp & 0x0c) && (accel_tap_pos != 0x0c )) 	 // ���û�в෭�����Ƿ���ײ
	   if ((tmp & 0x0c) && (accel_tap_pos != 0x0c )) 	 // ���û�в෭�����Ƿ���   modified 2013.3
	   {
			if ((TD.MainTask == tMAIN_ACCEL)||(TD.TestMode == TEST_MODE))
			{
				Uart.puts("Tap Event");
				os_dly_wait(1);
			}
			event = ACCEL_TAP;
			disposition_tilt_or_tap(1);
			
	
		}

		if (kxtf9_enable_int() != TRUE)
		{
			kxtf9_enable_int();
			
		}
	}

	if ((Cnt % 200) == 0)  //  2s 	���״̬�����Ƿ񱨾����������������屨����־
	{
		if (event == ACCEL_TILT)		// �෭��Ҫ���״̬
		{
			//Uart.puts("BBB8");
			tmp = 0xc0;   // ����һ����Чֵ	
			if (kxtf9_read_current_tilt_position(&tmp) == TRUE)	// ״̬û�иı�	, û�в෭
			{
				if (accel_tilt_pos == tmp)	
				{ 
					disposition_tilt_or_tap(0); 
					event = ACCEL_NO_EVENT;

					if ((TD.MainTask == tMAIN_ACCEL)||(TD.TestMode == TEST_MODE))
					{
						Uart.puts("Clear Tilt Event");
						os_dly_wait(1);
					}
				} 	
				else 								
				{ 
					disposition_tilt_or_tap(1);	
				}	
	
			}
				
		}
		//else	if (event == ACCEL_TAP)		//	  �������ײ�����������־����
		if (event == ACCEL_TAP)		//	  �������ײ�����������־����		 modified 2013.3
		{
			
			disposition_tilt_or_tap(0);
			event = ACCEL_NO_EVENT;

			if ((TD.MainTask == tMAIN_ACCEL)||(TD.TestMode == TEST_MODE))
			{
				Uart.puts("Clear Tap Event");
				os_dly_wait(1);
			}
					
		}

		os_dly_wait(1);
		
	}



	
}

/*----------------------------------------------------------------------------
  	 �������ƣ�Accelerometer_init
	 �������ܣ���ʼ�����ٶȴ�������ʹ��: �����жϣ���ʼ����������������ȡ���� 
	 			   ����ʼ״ֵ̬
	 ����������first_use: ����1ʱ�� ��Ϊ�ǵ�һ��ʹ�ã�����Ĭ�ϲ���������
	 			  ���������ȡ����
	 ��������ֵ����
 *----------------------------------------------------------------------------*/
void Accelerometer_init(uchar	first_use)		//   22644384 / 22644383  ��������Ƭ�� 
{
	// �����ж� p0.1
	 ACCEL_INTERRUPT_PIN;				// P0.1 ����ΪGPIO	  ��������
	 LPC_GPIOINT->IO0IntEnR |= (1UL << 1);   // ����p0.1Ϊ���������ж�
	 LPC_GPIOINT->IO0IntClr = 0xffffffff;


	// ���ж�
 	NVIC_SetPriority(EINT3_IRQn, GPIO_EINT3_PRIORITY);     
    NVIC_EnableIRQ(EINT3_IRQn);

	// ��������ʼ��
	accel_kionix_init(first_use);		 // ���ٶȴ�������ʼ��

	os_dly_wait(10);


	// ��ȡ����״ֵ̬
	if (kxtf9_read_current_tilt_position(&accel_tilt_pos) != TRUE)
	{
		accel_tilt_pos = 0xc0;	  // ��Чֵ
	}

	if (kxtf9_read_tap_direction (&accel_tap_pos) != TRUE)
	{
		accel_tap_pos = 0xc0;     // ��Чֵ
	}

	
}


extern 	"C"		void EINT3_IRQHandler(void)	__irq
{
	//interrupt_service_for_accel();

	if (LPC_GPIOINT->IO0IntStatR & (1 << 1))
	{
		accel_interrupt_flag = 1;

		LPC_GPIOINT->IO0IntClr |= (1UL << 1);   // ���ж�

	}

}









