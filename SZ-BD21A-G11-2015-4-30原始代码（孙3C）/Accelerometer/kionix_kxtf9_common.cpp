
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
	if (Current_Use_4442_Flag == 1) 		// 如果使用过sle4442IC卡，则需要把普通模式切换回I2C模式
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
	accel_info.g_range = 8;  //2;		由于JT808规定碰撞加速度范围为：0 - 79 , 即0g - 7.9g ，所以重力最大设置为8g

	// Tillt Settings
	accel_info.tiltinfo.enable = 1;		 // 0 不设置，按出厂设置
	accel_info.tiltinfo.ODR = 12; 		// 50;		 	// 50Hz
	accel_info.tiltinfo.timer = 0x01;//10;		// 0.2s
	accel_info.tiltinfo.angle = 27; 		// 为0(26度), 27(60度)， 则使用默认值0x0c  26度   c = (uchar)(sin((float)(c) / 180 * 3.14) * 32);	
	accel_info.tiltinfo.hysteresis = 0;  //0x14  +/- 15度
	
	// Motion Detect Settings
	accel_info.wufinfo.enable = 0;		 // 1 启用， 0：禁用 ， 目前的G11没有用该功能
	accel_info.wufinfo.ODR = 50;  		// 50: 50Hz
	accel_info.wufinfo.timer = 100;		// 2s
	accel_info.wufinfo.thresh = 100; 		//0x08;   // 0.5g

	// Tap Double Tap(TDT) Settings
   accel_info.tdtinfo.enable = 1;
   accel_info.tdtinfo.ODR = 5;          		// 设置为50Hz	 (5: 50Hz, 10: 100Hz, 20: 200Hz, 40: 400Hz) 
   accel_info.tdtinfo.tdt_timer = 0;		//0x78;   表示 0.3s		 以下几个为0，表示使用默认值
   accel_info.tdtinfo.tdt_h_thresh = 0;	//0xb6;	 最高阈值
   accel_info.tdtinfo.tdt_l_thresh = 150;	//0x1a;	 最低阈值
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
	accel_info.maskinfo.B32 = 0x0003ffff;		  // 1： 启用  0：禁用/屏蔽


	// Interrupt control
	accel_info.intinfo.enable  = 1;		//1 = enable
	accel_info.intinfo.polarity = 1;		//1 = active high
	accel_info.intinfo.latch = 0; 		//1;		//1 = unlatched
	accel_info.intinfo.alt_latch = 0; 	 //0 = wuf int latch response	 1 = no latch

	DFE.Write(EE_ACCEL_PARA_BASE, LEN_ACCEL_PARA, (uchar *)&accel_info);
	NOP(); 	NOP(); NOP();
	
}


/*----------------------------------------------------------------------------
  	 函数名称：accel_kionix_init
	 函数功能：初始化加速度传感器参数
	 函数参数：first_use: 设置1时， 认为是第一次使用，加载默认参数，否则，
	 			  从铁电里读取参数
	 函数返回值：空
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



// 加速传感器中断处理
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

		//if (!(JT808_Alarm_Word.Shield & (0x01 << 29)))// && Cnt > 3)				 // 	检测3次，认为有碰撞 或侧翻
		{
			//Cnt = 0;
			Index = 0xff;

			if ((JT808Flag.VS8.Bits.CrashPreAlarm == 0) && (event == ACCEL_TAP))
			{
				JT808Flag.VS8.Bits.CrashPreAlarm = 1;
				Index = 29;
				//Uart.puts(" 碰撞报警 ");
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
				#if ( TEST_CODE_JUST_FOR_TEST_LUOYANG==1)	  // 应付测试使用
				NOP();	NOP();
				#else
				//jflsja f	
				if (JT808_Alarm_Word.SendTextSMS & (0x01 << Index))		     								// 发送短信
				{
					GSM.SetSMS_Buffer((char *)&JT808_Number.TextSMS[0], (Index == 29)? "Crash Alarm!!!":"Tilt Alarm");	
						
				}
				//if (JT808_Alarm_Word.TakePhoto & (0x01 << 29))	Camera.SetTakeNumber(2,TRUE);		  // 拍照
				if (JT808_Alarm_Word.TakePhoto & (0x01 << Index))
				{
			  		if (!(JT808_Alarm_Word.SavePhoto & (0x01 << Index)))	Jt_photo_upload = 1;		  // 上传到平台
	
					MediaEventFlag = eBB_MEDIA_EVENT_CRASH;
			  		Camera.SetTakeNumber(2,TRUE);		  // 拍照 
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
		// 事件
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
		///// 方向
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
//			Uart.puts("读到方向  Test Data:"); Uart.putb(1,1,&tmp); Uart.puts("\r\n");
//		} 
//	}
	//

	// 碰撞或侧翻检测

	if (accel_interrupt_flag == 1)
	{
		accel_interrupt_flag = 0;
		kxtf9_disable_int();
	
		kxtf9_service_interrupt(&tmp);		//
	
		if ((tmp & 0x01) && (accel_tilt_pos != 0x0c))	// accel_tilt_pos 为有效值时
		{
			tmp = 0xc0;   // 设置一个无效值	
			if (kxtf9_read_current_tilt_position(&tmp) == TRUE)
			{
				 //Uart.puts("CCC1"); Uart.putb(1, 1, &accel_tilt_pos); Uart.putb(1, 1, &tmp);	os_dly_wait(1);
				//if (accel_tilt_pos == tmp)	{ disposition_tilt_or_tap(0); } // 状态没有改变	, 没有侧翻
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
	   //else if ((tmp & 0x0c) && (accel_tap_pos != 0x0c )) 	 // 如果没有侧翻则检查是否碰撞
	   if ((tmp & 0x0c) && (accel_tap_pos != 0x0c )) 	 // 如果没有侧翻则检查是否碰   modified 2013.3
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

	if ((Cnt % 200) == 0)  //  2s 	检查状态，看是否报警条件解除，解除则清报警标志
	{
		if (event == ACCEL_TILT)		// 侧翻需要检查状态
		{
			//Uart.puts("BBB8");
			tmp = 0xc0;   // 设置一个无效值	
			if (kxtf9_read_current_tilt_position(&tmp) == TRUE)	// 状态没有改变	, 没有侧翻
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
		//else	if (event == ACCEL_TAP)		//	  如果是碰撞，清除报警标志即可
		if (event == ACCEL_TAP)		//	  如果是碰撞，清除报警标志即可		 modified 2013.3
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
  	 函数名称：Accelerometer_init
	 函数功能：初始化加速度传感器的使用: 配置中断，初始化传感器参数，读取传感 
	 			   器初始状态值
	 函数参数：first_use: 设置1时， 认为是第一次使用，加载默认参数，否则，
	 			  从铁电里读取参数
	 函数返回值：空
 *----------------------------------------------------------------------------*/
void Accelerometer_init(uchar	first_use)		//   22644384 / 22644383  周立功单片机 
{
	// 配置中断 p0.1
	 ACCEL_INTERRUPT_PIN;				// P0.1 设置为GPIO	  配置上拉
	 LPC_GPIOINT->IO0IntEnR |= (1UL << 1);   // 设置p0.1为上升触发中断
	 LPC_GPIOINT->IO0IntClr = 0xffffffff;


	// 开中断
 	NVIC_SetPriority(EINT3_IRQn, GPIO_EINT3_PRIORITY);     
    NVIC_EnableIRQ(EINT3_IRQn);

	// 传感器初始化
	accel_kionix_init(first_use);		 // 加速度传感器初始化

	os_dly_wait(10);


	// 获取初次状态值
	if (kxtf9_read_current_tilt_position(&accel_tilt_pos) != TRUE)
	{
		accel_tilt_pos = 0xc0;	  // 无效值
	}

	if (kxtf9_read_tap_direction (&accel_tap_pos) != TRUE)
	{
		accel_tap_pos = 0xc0;     // 无效值
	}

	
}


extern 	"C"		void EINT3_IRQHandler(void)	__irq
{
	//interrupt_service_for_accel();

	if (LPC_GPIOINT->IO0IntStatR & (1 << 1))
	{
		accel_interrupt_flag = 1;

		LPC_GPIOINT->IO0IntClr |= (1UL << 1);   // 清中断

	}

}









