			  

#ifndef      __USERCONFIG_H__
#define      __USERCONFIG_H__
/////////////////////////////// 

#ifdef      CFG_GLOBALS
#define     CFG_EXT
#else
#define     CFG_EXT    extern
#endif


#ifndef		__noinit__
#define 	__noinit__ 		__attribute__((zero_init))//变量不初始化为0
#endif







#include		"VariableType.h"

//#include		".\Common\ConstCode.h"

#include		".\Common\Productor.h"

#include    <string.h>
				
#include 	<RTL.h>                      /* RTX kernel functions & defines      */
#include 	<LPC17xx.H>                  /* LPC17xx definitions                 */



//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	RTX和uCOS函数对应
E:\Guoyee\GY-G11\GY-G11\ZY_CODE\uCOS-II\cpu\os_cpu.h 
E:\Guoyee\GY-G11\GY-G11\Config\UserConfig.h
*/
//#define			os_dly_wait(i)		OSTimeDly(i)
#define			DisableIRQ()		OS_ENTER_CRITICAL()
#define			EnableIRQ()			OS_EXIT_CRITICAL()

#define    		 NOP()      __nop()
//#define    		 NOP() 





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

*/
#ifndef 		TRUE
#define 		TRUE      	0x01
#endif
#ifndef 		FALSE
#define 		FALSE 		0x00
#endif
///////////////////////////////
#ifndef 		SUCCESS		
#define 		SUCCESS 		0x00
#endif
#ifndef 		FAILURE
#define 		FAILURE 		0x01
#endif
///////////////////////////////
#ifndef 		OK
#define 		OK		 		0x00
#endif
#ifndef 		NOT_OK
#define 		NOT_OK		0x01
#endif
#ifndef 		ERROR
#define 		ERROR 		0x01


#endif


#ifndef 		MSG_ERROR
#define 		MSG_ERROR 				0x02		  // 消息有误
#endif
#ifndef 		MSG_NOT_SUPPORT
#define 		MSG_NOT_SUPPORT 		0x03		  // 不支持的消息
#endif
#ifndef 		MSG_ALARM_CONFIRM
#define 		MSG_ALARM_CONFIRM 	0x04		  // 报警处理确认
#endif		

#ifndef 		IP
#define 		IP 		            0x05
#endif
#ifndef 		DNS
#define 		DNS 		            0x06
#endif

#define		ENABLE_FLAG					0x5a



#define			RESERVER_NO_USE		0x00000001
#define			POWER_CONTROL_VAL		0x8300609A

#define			PCONP_RSV0				0
#define			PCONP_TIMER0			1
#define			PCONP_TIMER1			2
#define			PCONP_UART0				3
#define			PCONP_UART1				4
#define			PCONP_PWM0				5
#define			PCONP_PWM1				6
#define			PCONP_I2C0				7
#define			PCONP_SPI				8
#define			PCONP_RTC				9
#define			PCONP_SSP1				10
#define			PCONP_EMC				11
#define			PCONP_ADC				12
#define			PCONP_CAN1				13
#define			PCONP_CAN2				14
#define			PCONP_RSV15				15
#define			PCONP_RIT				16
#define			PCONP_MCPWM				17
#define			PCONP_QEI				18
#define			PCONP_I2C1				19
#define			PCONP_RSV20				20
#define			PCONP_SSP0				21
#define			PCONP_TIMER2			22
#define			PCONP_TIMER3			23
#define			PCONP_UART2				24
#define			PCONP_UART3				25
#define			PCONP_I2C2				26
#define			PCONP_I2S				27
#define			PCONP_SDC				28
#define			PCONP_GPDMA				29
#define			PCONP_ENET				30
#define			PCONP_USB				31








#define			CLOCK_SETUP          1
#define			SCS_Val              0x00000020
#define			CLKSRCSEL_Val        0x00000001
#define			PLLCFG_Val           0x00000005
#define			CCLKCFG_Val          0x00000003
#define			USBCLKCFG_Val        0x00000003
#define			PCLKSEL0_Val         0x00000001
#define			PCLKSEL1_Val         0x00000000



			  

#define		FIFO_LENGTH			14




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>IO_FOR_MODEL:     产品型号IO分配定义  <0=> GY_G8 	<1=> GY_G08Small
//   	<i> Default: GY_G8
*/
#define			IO_FOR_MODEL					0


#define			MODEL_GY_G8						0
#define			MODEL_GY_G08Small				1


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>APN_USE_GYJT:  APN设置  <0=>  默认中国移动 	<1=> 广州交投
//   	<i> Default: 1
*/
#define		APN_USE_GYJT						1


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>LIMIT_CONST_ID_RANGE: 产品唯一ID限制  <0=> 不限制ID范围 	<1=> 限制ID范围
//   	<i> Default: 0
*/
#define		LIMIT_CONST_ID_RANGE				1



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>RTC_ADJUST_VALUE: 当地时间与GPS标准时间的调整值(小时）  <0-12>
//   	<i> default: 8
*/
#define			RTC_HOUR_ADJUST_VALUE		8





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>USE_TH_GPRS_DATA_PACK_PROTOCOL: 使用TH数据包协议  <0=> 不使用 <1=> 使用
//   	<i> Default: 使用
*/
#define			USE_TH_GPRS_DATA_PACK_PROTOCOL		1



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>NEED_CODE_CRC: 系统代码CRC检验  <0=> 无需校验 	<1=> 必须校验
//   	<i> Default: 0
*/
#define			NEED_CODE_CRC		0








// <h>CPU时钟相关配置		  
																										  
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>MAIN_PLL_MSEL: MAIN_PLL倍频系数 <16-65535>
//   	<i> Default: 100
*/
#define 		MAIN_PLL_MSEL	      	100		 // 注意：此处的值必须与system_LPC17xx.c中的常量<CCLKCFG>相匹配

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>MAIN_PLL_NSEL: MAIN_PLL倍频系数 <1-16>
//   	<i> Default: 6
*/
#define 		MAIN_PLL_NSEL	      	6		 // 注意：此处的值必须与system_LPC17xx.c中的常量<CCLKCFG>相匹配


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>CCLK_DIV: CPU频率从PLL的分频系数 <1=> 等于MAIN_PLL <2=> 2分频 <4=> 4分频	 <8=> 8分频
//   	<i> Default: 4
*/
#define 		CCLK_DIV	      			4		 // 注意：此处的值必须与system_LPC17xx.c中的常量<CCLKCFG>相匹配




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>FoscSel: 外部晶振频率(Hz)	 <1=> 11.0592MH  <2=> 12M
//   	<i> Default: 1
*/
#define			FoscSel			2

#if( FoscSel == 1)

#define 		Fosc			11059200L

#elif( FoscSel == 2)

#define 		Fosc			12000000L           //晶振频率,10MHz~25MHz，应当与实际一至

#endif

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>OUTSIDE_WATCH_DOG: 选择看门狗   <0=> 内部看门狗  <1=> 外部看门狗
//   	<i> Default: 	内部看门狗	
*/
#define		OUTSIDE_WATCH_DOG			0

#define		FmainPll		(2L*Fosc*MAIN_PLL_MSEL/MAIN_PLL_NSEL)

#define 		Fcclk1       (1L*FmainPll/CCLK_DIV)   	//系统频率，必须为Fosc的整数倍(1~32)，且<=60MHZ

#define		Fcclk			100000000L


// </h>





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>VEHICLE_TYPE: 车辆类型  <0=> 普通汽车 <1=> 邮政车
//   	<i> Default: 邮政车
*/						 
#define		VEHICLE_TYPE				1


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>USE_EEPROM_SAVE_GPS_DATA: 是否使用EEPROM存储GPS历史数据  <0=> 不使用 <1=> 使用
//   	<i> Default: 不使用
*/						 
#define		USE_EEPROM_SAVE_GPS_DATA		0



// <h>全局定义或变量相关设置



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>GPS_ANT_CHECK_COMMAND: GPS天线检测  <0=> 使用IO检测 <1=> 使用命令检测开线
//   	<i> default: 0
*/
#define			GPS_ANT_CHECK_COMMAND		1



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>LEN_PUBLIC_FREE_MEM: 动态内存分配长度  <128-2048>
//   	<i> default: 512
*/
#define			LEN_PUBLIC_FREE_MEM		(1024+32)



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>CONST_ID: 使用固定ID   <0=> 使用可修改的 <1=> 固定ID
//   	<i> Default: 固定ID
*/
#define			CONST_ID			1

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>CONST_IP: 使用固定IP   <0=> 使用可修改的 <1=> 固定IP
//   	<i> Default: 固定IP
*/
#define			CONST_IP			0

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>ENCRYPT: 加密CPU   <0=> 不加密 <1=> 加密
//   	<i> Default: 加密
*/
#define		ENCRYPT				1       // 0：不加密，非0：加密


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>NEED_SERIAL_NUMBER_CODE_ADDRESS: 加密CPU   <0=> 不需要 <1=> 需要
//   	<i> Default: 需要
*/
#define		NEED_SERIAL_NUMBER_CODE_ADDRESS		1       // 0：不需要，非0：需要



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>REMOTE_KEYBOARD: 是否远程键盘   <0=> 一体机 <1=> 远程键盘
//   	<i> Default: 一体机
*/
#define		REMOTE_KEYBOARD				1



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>REMOTE_DISPLAY: 是否远程显示  <0=> 一体机 <1=> 远程显示
//   	<i> Default: 一体机
*/
#define		REMOTE_DISPLAY					1

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>REMOTE_DRIVER_CARD: 是否远程司机卡  <0=> 一体机 <1=> 远程司机卡
//   	<i> Default: 一体机
*/

#define		REMOTE_DRIVER_CARD			0




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>NAND_FLASH: 是否支持NandFlash  <0=> 不支持 <1=> 支持
//   	<i> Default:   不支持
*/
#define		USE_NAND_FLASH					1


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>USE_GPS_MODEL: 使用GPS模块  <0=> 不使用 <1=> 使用
//   	<i> Default: 不使用
*/						 
#define		USE_GPS_MODEL					1


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>USE_MOBILE_MODEL: 使用手机模块  <0=> 不使用 <1=> 使用
//   	<i> Default: 使用
*/						 
#define		USE_MOBILE_MODEL				1



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>NEED_ADC: 需要使用ADC程序吗？  <0=> 不需要 <1=> 需要
//   	<i> Default: 不需要
*/
#define		NEED_ADC							1


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>EXTERN_RTC_CLOCK: RTC使用外部32K晶振 <0=> 使用内部分频 <1=> 使用外部32K晶振
//   	<i> Default: 使用外部32K晶振
*/						 
#define		EXTERN_RTC_CLOCK				1



#if(VEHICLE_TYPE == 0)


// <e> 普通汽车默认数据采集时间间隔

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>DEFAULT_ACC_RUN_SAMPLE_PERIOD: 发动机工作时采样周期 <5-3600>
//   	<i> default: 600
*/
#define			DEFAULT_ACC_RUN_SAMPLE_PERIOD		10

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>DEFAULT_ACC_STOP_SAMPLE_PERIOD: 发动机不工作时采样周期 <5-3600>
//   	<i> default: 600
*/
#define			DEFAULT_ACC_STOP_SAMPLE_PERIOD		10

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>DEFAULT_MIN_SAMPLE_PERIOD: 默认最小采样周期 <10-60>
//   	<i> default: 600
*/
#define			DEFAULT_MIN_SAMPLE_PERIOD		10

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>DEFAULT_MAX_SAMPLE_PERIOD: 默认最大采样周期 <300-1800>
//   	<i> default: 600
*/
#define			DEFAULT_MAX_SAMPLE_PERIOD		600

// </e>

#elif(VEHICLE_TYPE == 1)


// <e> 邮政车辆默认数据采集时间间隔

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>DEFAULT_ACC_RUN_SAMPLE_PERIOD: 发动机工作时采样周期 <60-7200>
//   	<i> default: 600
*/
#define			DEFAULT_ACC_RUN_SAMPLE_PERIOD		300

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>DEFAULT_ACC_STOP_SAMPLE_PERIOD: 发动机不工作时采样周期 <60-7200>
//   	<i> default: 600
*/
#define			DEFAULT_ACC_STOP_SAMPLE_PERIOD		300

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>DEFAULT_MIN_SAMPLE_PERIOD: 默认最小采样周期 <5-60>
//   	<i> default: 600
*/
#define			DEFAULT_MIN_SAMPLE_PERIOD		5


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>DEFAULT_MAX_SAMPLE_PERIOD: 默认最大采样周期 <300-1800>
//   	<i> default: 600
*/
#define			DEFAULT_MAX_SAMPLE_PERIOD		65535

// </e>

#endif

// </h>


// <h> 选择是否编译某些代码


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>SYSTEM_BOOT_MESSAGE: 系统启动信息代码 <0=> 不编译 <1=> 编译
//   	<i> Default: 不编译
*/
#define		SYSTEM_BOOT_MESSAGE					0


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>COMPILE_GPS_TEST_DATA_CODE: GPS测试数据代码 <0=> 不编译 <1=> 编译
//   	<i> Default: 不编译
*/
#define		COMPILE_GPS_TEST_DATA_CODE			1

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>COMPILE_GPRS_TEST_DATA_CODE: GPRS测试数据代码 <0=> 不编译 <1=> 编译
//   	<i> Default: 不编译
*/
#define		COMPILE_GPRS_TEST_DATA_CODE			1

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>COMPILE_GSM_GPRS_TEST_DATA_CODE: GSM_GPRS测试数据代码 <0=> 不编译 <1=> 编译
//   	<i> Default: 不编译
*/
#define		COMPILE_GSM_GPRS_TEST_DATA_CODE			1

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>COMPILE_ADC_TEST_DATA_CODE: AD转换测试数据代码 <0=> 不编译 <1=> 编译
//   	<i> Default: 不编译
*/
#define		COMPILE_ADC_TEST_DATA_CODE			1

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>COMPILE_SW_TEST_DATA_CODE: 开关量测试数据代码 <0=> 不编译 <1=> 编译
//   	<i> Default: 不编译
*/
#define		COMPILE_SW_TEST_DATA_CODE			1


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>DFE_DEBUG: DF测试数据代码 <0=> 不编译 <1=> 编译
//   	<i> Default: 不编译
*/
#define		DFE_DEBUG			1



// </h>



//////////////////////////// 当修改常量文件ConstCode.h中constModelVersion时，需修改下面几个宏定义
//#define		LEN_MODEL_VERSION		55
//#define		LEN_MODEL				7
//#define		MV_POS_MODEL			7
//#define		MV_POS_HV				36
//#define		MV_POS_SV				50
//#define		BUILD_DATE_POS			(LEN_MODEL_VERSION+1)
//#define		BUILD_TIME_POS			(BUILD_DATE_POS+12)


#define		WUXI_TEST				0			// 1: 无锡检测		0: 实际使用


//------------- <<< end of configuration section >>> -----------------------



#define        UserConst  ((USER_CODE_CONST *)(CONST_BASE_ADDR))


//////// RFID 缺省电源控制
#define		DEFAULT_POWER_ON_RFID		1
//


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   IO口输入输出，全部在这里定义
*/

#define		UART0_PIN_SELECT		{	LPC_PINCON->PINSEL0 |= 0x00000050;	}

#define		UART1_PIN_SELECT		{	LPC_PINCON->PINSEL4 |= (2UL<<0)|(2UL<<2);	}

#define		UART2_PIN_FOR_RS485	{  LPC_PINCON->PINSEL0 &= ~((1UL<<20)|(1UL<<22)); 	LPC_PINCON->PINSEL4 |= (2UL<<16)|(2UL<<18);	}

#define		UART2_PIN_FOR_RS232	{	LPC_PINCON->PINSEL4 &= ~((3UL<<16)|(3UL<<18));	LPC_PINCON->PINSEL0 |= (1UL<<20)|(1UL<<22);	}

#define		UART3_PIN_SELECT		{	LPC_PINCON->PINSEL9 |=((3UL<<24)|(3UL<<26)); 	}

#define		I2C0_PIN_SELECT	 	{	LPC_PINCON->PINSEL1 |=((1UL<<22)|(1UL<<24)); 	}


#define		CAP30_PIN_SELECT		{	LPC_PINCON->PINSEL1 |= (3UL<<14);	}	    //

#define		CAP01_PIN_SELECT		{	LPC_PINCON->PINSEL3 |= (3UL<<22);	}

#define		CAP11_PIN_SELECT		{	LPC_PINCON->PINSEL3 |=  (3UL<<6);	}


//#define  	RS485_TRANSMIT_ENABLE   {LPC_GPIO0->FIODIR &= ~(1UL<<9);  NOP();   NOP(); }	
//#define  	RS485_RECEIVE_ENABLE  	{LPC_GPIO0->FIODIR |= (1UL<<9);  LPC_GPIO0->FIOCLR |= (1UL<<9); 	NOP();   NOP(); }		



#define		I2C0_PIN_FOR_GPIO		{	LPC_PINCON->PINSEL1 &= ~((3UL<<22)|(3UL<<24));}
#define		I2C0_SCL_PIN_OUT		{	LPC_GPIO0->FIODIR |= (1UL<<28);	}
#define		I2C0_SDA_PIN_OUT		{	LPC_GPIO0->FIODIR |= (1UL<<27);	}
#define		I2C0_SDA_PIN_IN		{	LPC_GPIO0->FIODIR &= ~(1UL<<27);	}
#define		I2C0_SCL_PIN_HIGH		{	LPC_GPIO0->FIOSET |= (1UL<<28);	}
#define		I2C0_SCL_PIN_LOW		{	LPC_GPIO0->FIOCLR |= (1UL<<28);	}
#define		I2C0_SDA_PIN_HIGH		{	LPC_GPIO0->FIOSET |= (1UL<<27);	}
#define		I2C0_SDA_PIN_LOW		{	LPC_GPIO0->FIOCLR |= (1UL<<27);	}
#define		GET_I2C0_SDA_STATUS	((LPC_GPIO0->FIOPIN>>27)&0x00000001)

#define 		ACCEL_INTERRUPT_PIN	{	LPC_PINCON->PINSEL0 &= ~(3UL << 2); LPC_PINCON->PINMODE0 &= ~(3UL << 2);  }



//// IC复位脚与IC卡检测脚使用相同IO口（卡座使用常闭，插入卡时开关开，引脚为高电平）
//#define		IC_CARD_RESET_PIN_HIGH		{ LPC_GPIO0->FIODIR |= (1UL<<0);	 	LPC_GPIO0->FIOSET |= (1UL<<0); }
//#define		IC_CARD_RESET_PIN_LOW		{ LPC_GPIO0->FIODIR |= (1UL<<0);	 	LPC_GPIO0->FIOCLR |= (1UL<<0); }

// IC复位脚与IC卡检测脚使用相同IO口（精简板的G11卡座使用常开，插入卡时开关闭合，引脚为接地）
#define		IC_CARD_RESET_PIN_HIGH		{ LPC_GPIO1->FIODIR |= (1UL<<17);	 	LPC_GPIO1->FIOSET |= (1UL<<17); }
#define		IC_CARD_RESET_PIN_LOW		{ LPC_GPIO1->FIODIR |= (1UL<<17);	 	LPC_GPIO1->FIOCLR |= (1UL<<17); }

#define  	RS485_TRANSMIT_ENABLE   {LPC_GPIO1->FIODIR &= ~(1UL<<1);  NOP();   NOP(); }	
#define  	RS485_RECEIVE_ENABLE  	{LPC_GPIO1->FIODIR |= (1UL<<1);  LPC_GPIO1->FIOCLR |= (1UL<<1); 	NOP();   NOP(); }		



#define		USE_LED				0
#if(USE_LED)				
#define		GPRS_LED_ON				{ LPC_GPIO1->FIODIR |= (1UL<<28);  LPC_GPIO1->FIOCLR |= (1UL<<28);}
#define		GPRS_LED_OFF			{ LPC_GPIO1->FIODIR |= (1UL<<28); LPC_GPIO1->FIOSET |= (1UL<<28);}
#define		GPS_LED_ON				{ LPC_GPIO1->FIODIR |= (1UL<<29);  LPC_GPIO1->FIOCLR |= (1UL<<29); }
#define		GPS_LED_OFF				{ LPC_GPIO1->FIODIR |= (1UL<<29);  LPC_GPIO1->FIOSET |= (1UL<<29);}
#define		ALL_LED_ON				{ LPC_GPIO1->FIODIR |= (1UL<<28);	LPC_GPIO1->FIOCLR |= (1UL<<28);  LPC_GPIO1->FIODIR |= (1UL<<29); LPC_GPIO1->FIOCLR |= (1UL<<29);	}
#define		ALL_LED_OFF				{ LPC_GPIO1->FIOSET |= (1UL<<28);  LPC_GPIO1->FIOSET |= (1UL<<29);	}
#else			// G11无指示灯
#define		GPRS_LED_ON				NOP()
#define		GPRS_LED_OFF			NOP()
#define		GPS_LED_ON				NOP()
#define		GPS_LED_OFF				NOP()
#define		ALL_LED_ON				NOP()
#define		ALL_LED_OFF				NOP()
#endif

#define		CLEAR_RTS				{	LPC_GPIO2->FIODIR |= (1UL<<7); LPC_GPIO2->FIODIR &= ~(1UL<<2);\
												NOP();	NOP();	NOP();	NOP();LPC_GPIO2->FIOCLR |= (1UL<<7);\
											};
#define		GET_CTS					(LPC_GPIO2->FIOPIN&(1UL<<2))
//蜂鸣器	/////////// add by wrc  ///////////////								
//#define     BUZZER_ON			  {  LPC_GPIO1->FIODIR |= (1UL<<17);  LPC_GPIO1->FIOCLR |= (1UL<<17);}		
//#define     BUZZER_OFF			  {  LPC_GPIO1->FIOSET |= (1UL<<17);}		 //LPC_GPIO1->FIODIR &= ~(1UL<<17);		
							
#define     BUZZER_ON			  {  LPC_GPIO2->FIODIR |= (1UL<<4);  LPC_GPIO2->FIOSET |= (1UL<<4);}		
#define     BUZZER_OFF			  {  LPC_GPIO2->FIODIR |= (1UL<<4);  LPC_GPIO2->FIOCLR |= (1UL<<4);}		 //LPC_GPIO1->FIODIR &= ~(1UL<<17);		



///////////// 定义手机模块控制线的相关引脚  /////////////////////////////////////////////
// HC596输出统一在主任务里输出控制，这样会导致其它输出会延时约10ms，但不影响。
#define		CTRL_MOBILE_POWER		   1
#if(CTRL_MOBILE_POWER)
#define		MOBILE_POWER_ON			{  LPC_GPIO0->FIODIR |= (1UL<<9);  LPC_GPIO0->FIOSET |= (1UL<<9);}	
#define		MOBILE_POWER_OFF			{  LPC_GPIO0->FIODIR |= (1UL<<9);  LPC_GPIO0->FIOCLR |= (1UL<<9);}
#define		MOBILE_POWER_KEY_HIGH	{  LPC_GPIO2->FIODIR |= (1UL<<3);  LPC_GPIO2->FIOCLR |= (1UL<<3);}
#define		MOBILE_POWER_KEY_LOW		{  LPC_GPIO2->FIODIR |= (1UL<<3);  LPC_GPIO2->FIOSET |= (1UL<<3);}

#else
#define		MOBILE_POWER_OFF			NOP()
#define		MOBILE_POWER_ON			NOP()
#endif


// RFID 电源控制////////////////////////////////////////////////////////////
#define		RFID_POWER_ON			  	{  LPC_GPIO1->FIODIR |= (1UL<<27);  LPC_GPIO1->FIOSET |= (1UL<<27);}
#define		RFID_POWER_OFF				{  LPC_GPIO1->FIODIR |= (1UL<<27);  LPC_GPIO1->FIOCLR |= (1UL<<27);}



//HC596输出统一在主任务里输出控制，这样会导致其它输出会延时约10ms，但不影响。
#define		CTRL_GPS_POWER		   1
#if(CTRL_GPS_POWER)
#define		GPS_POWER_ON			{  LPC_GPIO0->FIODIR |= (1UL<<4);  LPC_GPIO0->FIOCLR |= (1UL<<4);}
#define		GPS_POWER_OFF			{  LPC_GPIO0->FIODIR |= (1UL<<4);  LPC_GPIO0->FIOSET |= (1UL<<4);}
#else
#define		GPS_POWER_OFF			NOP()
#define		GPS_POWER_ON			NOP()
#endif


#if(OUTSIDE_WATCH_DOG)
#define		RESET_WATCH_DOG		{ LPC_GPIO2->FIODIR |= (1UL<<10); 	NOP();   NOP();\
											  LPC_GPIO2->FIOCLR |= (1UL<<10); 	NOP();	NOP();\
											  LPC_GPIO2->FIOSET |= (1UL<<10); 	NOP();	NOP();\
											  LPC_GPIO2->FIOCLR |= (1UL<<10); 	NOP();	NOP();\
											  LPC_GPIO2->FIOSET |= (1UL<<10); 	NOP();	NOP();\
											} ;
#else
#define		RESET_WATCH_DOG	   Reset_Watch_Dog(); /*	{ DisableIRQ();LPC_WDT->WDFEED = 0xAA; LPC_WDT->WDFEED = 0x55;EnableIRQ();\
											  LPC_GPIO2->FIODIR |= (1UL<<10); 	NOP();   NOP();\
											  LPC_GPIO2->FIOCLR |= (1UL<<10); 	NOP();	NOP();\
											  LPC_GPIO2->FIOSET |= (1UL<<10); 	NOP();	NOP();\
											  LPC_GPIO2->FIOCLR |= (1UL<<10); 	NOP();	NOP();\
											  LPC_GPIO2->FIOSET |= (1UL<<10); 	NOP();	NOP();\
                                 } ;*/
#endif




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   存储器片选
*/
// SPI EEPROM/铁电存储片选　
#define			DFE_CSEnable		{ LPC_GPIO0->FIODIR |= ((1UL<<19)|(1UL<<20)); LPC_GPIO0->FIOCLR |= (1UL<<20);LPC_GPIO0->FIOSET |= (1UL<<19);\
											  LPC_GPIO2->FIODIR |= (1UL<<10); LPC_GPIO2->FIOSET |= (1UL<<10);NOP();	NOP();}
#define			DFE_CSDisable		{ LPC_GPIO0->FIOSET |= (1UL<<20);  	}

// SPI Flash片选
#define			DFF_CSEnable		{ LPC_GPIO0->FIODIR |= ((1UL<<19)|(1UL<<20)); LPC_GPIO0->FIOCLR |= (1UL<<19);LPC_GPIO0->FIOSET |= (1UL<<20);\
										  		LPC_GPIO2->FIODIR |= (1UL<<10); LPC_GPIO2->FIOSET |= (1UL<<10);NOP();	NOP();}
#define			DFF_CSDisable	 	{ LPC_GPIO0->FIOSET |= (1UL<<19);	}




//SSP0 显示选择			 //王		  /////////// add by wrc  ///////////////
#define			Disp_CSEnable		{ LPC_GPIO1->FIODIR |= (1UL<<21); LPC_GPIO1->FIOCLR |= (1UL<<21); NOP();	NOP();}			
											
#define			Disp_CSDisable		{ LPC_GPIO1->FIODIR |= (1UL<<21); LPC_GPIO1->FIOSET |= (1UL<<21);  	}

//显示屏指令数据发送使能	  //老的23   新的8
//#define			Disp_Data_WI_Enable		    { LPC_GPIO1->FIODIR |= (1UL<<23); LPC_GPIO1->FIOSET |= (1UL<<23); NOP();	NOP();}
//#define			Disp_Command_WI_Enable   	 { LPC_GPIO1->FIODIR |= (1UL<<23); LPC_GPIO1->FIOCLR |= (1UL<<23); NOP();	NOP();}

#define			Disp_Data_WI_Enable		    { LPC_GPIO1->FIODIR |= (1UL<<8); LPC_GPIO1->FIOSET |= (1UL<<8); NOP();	NOP();}
#define			Disp_Command_WI_Enable   	 { LPC_GPIO1->FIODIR |= (1UL<<8); LPC_GPIO1->FIOCLR |= (1UL<<8); NOP();	NOP();}


// LCM数据、命令选择		   //老的23   新的8
//#define			LCM_DATA_ENABLE	{ LPC_GPIO1->FIODIR |= (1UL<<23);LPC_GPIO1->FIOSET |= (1UL<<23); }
//#define			LCM_CMD_ENABLE		{ LPC_GPIO1->FIODIR |= (1UL<<23);LPC_GPIO1->FIOCLR |= (1UL<<23); }

#define			LCM_DATA_ENABLE	{ LPC_GPIO1->FIODIR |= (1UL<<8);LPC_GPIO1->FIOSET |= (1UL<<8); }
#define			LCM_CMD_ENABLE		{ LPC_GPIO1->FIODIR |= (1UL<<8);LPC_GPIO1->FIOCLR |= (1UL<<8); }

// LCM片选
#define			LCM_CS_ENABLE		{ LPC_GPIO1->FIODIR |= (1UL<<21);LPC_GPIO1->FIOCLR |= (1UL<<21); }
#define			LCM_CS_DISABLE		{ LPC_GPIO1->FIODIR |= (1UL<<21);LPC_GPIO1->FIOSET |= (1UL<<21); }


#define          LCM_POWER_ON   	{  LPC_GPIO0->FIODIR |= (1UL<<6);LPC_GPIO0->FIOCLR |= (1UL<<6);}
#define          LCM_POWER_OFF  	{  LPC_GPIO0->FIODIR |= (1UL<<6);LPC_GPIO0->FIOSET |= (1UL<<6);}
#define          RET_LCM_LOW    	LCM_POWER_OFF
#define          RET_LCM_HIGH    	LCM_POWER_ON		
#define          LCM_BL_ENABLE  	{  LPC_GPIO0->FIODIR |= (1UL<<7);LPC_GPIO0->FIOSET |= (1UL<<7);}
#define          LCM_BL_DISABLE    {  LPC_GPIO0->FIODIR |= (1UL<<7);LPC_GPIO0->FIOCLR |= (1UL<<7);}

//#if  (PRODUCT_TYPE==SHANGHAI_SIZENG_PRT)		  // 上海思增的屏IO需要修改的 //老的23   新的8
#define          RS_LCM_LOW			{  LPC_GPIO1->FIODIR |= (1UL<<8); LPC_GPIO1->FIOCLR |= (1UL<<8); } 
#define          RS_LCM_HIGH			{  LPC_GPIO1->FIODIR |= (1UL<<8); LPC_GPIO1->FIOSET |= (1UL<<8); }
//#else
//#define          RS_LCM_LOW			{  LPC_GPIO1->FIODIR |= (1UL<<23); LPC_GPIO1->FIOCLR |= (1UL<<23); }	  
//#define          RS_LCM_HIGH			{  LPC_GPIO1->FIODIR |= (1UL<<23); LPC_GPIO1->FIOSET |= (1UL<<23); }
//#endif

#define          CS_DISP_ENABLE		{  LPC_GPIO1->FIODIR |= (1UL<<21); LPC_GPIO1->FIOCLR |= (1UL<<21); }
#define          CS_DISP_DISABLE   {  LPC_GPIO1->FIODIR |= (1UL<<21); LPC_GPIO1->FIOSET |= (1UL<<21); }


// #define          RST_DISP_ENABLE		{  LPC_GPIO1->FIODIR |= (1UL<<21); LPC_GPIO1->FIOCLR |= (1UL<<21); }
// #define          RST_DISP_DISABLE		{  LPC_GPIO1->FIODIR |= (1UL<<21); LPC_GPIO1->FIOCLR |= (1UL<<21); }



// 打印机数据锁存
#define			PRINT_DATA_LATCH_HIGH	{ LPC_GPIO1->FIODIR |= (1UL<<17);LPC_GPIO1->FIOSET |= (1UL<<17); }
#define			PRINT_DATA_LATCH_LOW		{ LPC_GPIO1->FIOCLR |= (1UL<<17); }



		
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   IO输入检测管脚
*/
#define			GET_IO_PIN_INIT				{ LPC_GPIO1->FIODIR &= ~((3UL<<18)); LPC_GPIO2->FIODIR &= ~(0x00000007<<11);\
														  LPC_GPIO0->FIODIR &= ~((1UL<<0)); LPC_GPIO3->FIODIR &= ~((3UL<<25));}
#define			GET_SPEED_PLUSE_STATUS		((LPC_GPIO0->FIOPIN>>23)&0x00000001) 	// 读取速度脉冲引脚状态
#define			GET_BRAKE_STATUS				((LPC_GPIO1->FIOPIN>>18)&0x00000001)	// 读取刹车检测状态
#define			GET_ACC_STATUS					((LPC_GPIO1->FIOPIN>>19)&0x00000001)	// 读取ACC检测状态
#define         GET_BUTTON_STATUS          ((LPC_GPIO2->FIOPIN>>11)&0x00000001)   // 读取紧急报警按钮引脚状态
#define			GET_DIPPED_BEAM				((LPC_GPIO2->FIOPIN>>12)&0x00000001)	// 近光灯
#define			GET_HIGH_BEAM					((LPC_GPIO2->FIOPIN>>13)&0x00000001)	// 远光灯
#define			GET_RIGHT_LIGHT				((LPC_GPIO3->FIOPIN>>25)&0x00000001)	// 右转灯
#define			GET_LEFT_LIGHT					((LPC_GPIO3->FIOPIN>>26)&0x00000001)	// 左转灯

#define			GET_MOTO_STATUS					((LPC_GPIO2->FIOPIN>>11)&0x00000001)	// MOTO	 现在用于紧急报警


//#define			GET_HIGH_SPEED_PLUSE_STATUS		(NOP();)									  	// 读取高速脉冲引脚状态


//#define			GET_GPS_ANT_INIT				{NOP();}
//#define			GET_GPS_ANT_SHORT_STATUS	(NOP();)													// 读取GPS天线状态
//#define			GET_GPS_ANT_OPEN_STATUS		(NOP();)													// 读取GPS天线状态

#define			GET_IC_CARD_STATUS			((LPC_GPIO0->FIOPIN>>0)&0x00000001)

//#define			GET_TF_CARD_STATUS			((LPC_GPIO0->FIOPIN>>16)&0x00000001)

#define        GET_USB_STATUS             ((LPC_GPIO0->FIOPIN>>29)&0x00000001)


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   
*/
		
// 手柄/调度屏电源
//#define			HANDLE_POWER_ON			{ LPC_GPIO1->FIODIR |= (1UL<<27);LPC_GPIO1->FIOSET |= (1UL<<27); }			// 手柄/调度屏电源	
//#define			HANDLE_POWER_OFF			{ LPC_GPIO1->FIODIR |= (1UL<<27);LPC_GPIO1->FIOCLR |= (1UL<<27); }

// 摄像头电源控制						 		
#define			CAMERA_POWER_ON			{ LPC_GPIO0->FIODIR |= (1UL<<5);LPC_GPIO0->FIOSET |= (1UL<<5); }		// 摄像头电源
#define			CAMERA_POWER_OFF			{ LPC_GPIO0->FIODIR |= (1UL<<5);LPC_GPIO0->FIOCLR |= (1UL<<5); }

// 打印机电源控制
#define			PRINT_POWER_ON          { 	LPC_GPIO0->FIODIR |= (1UL<<8);LPC_GPIO0->FIOCLR |= (1UL<<8);}
#define			PRINT_POWER_OFF			{ 	LPC_PINCON->PINMODE0 &= (1UL<<16); LPC_PINCON->PINMODE0 |= (1UL<<17);\
														LPC_PINCON->PINMODE_OD0 |= (1UL<<8);LPC_GPIO0->FIODIR |= (1UL<<8);\
														LPC_GPIO0->FIOSET |= (1UL<<8);  	}
// 电源自锁
//#define			POWER_SELF_LOCK_ON			//{   HC595.Out1.Bits.PowerSeldLock = 1;	}//HC595_OUTPUT; }		// 
//#define			POWER_SELF_LOCK_OFF			//{   HC595.Out1.Bits.PowerSeldLock = 0;	}//HC595_OUTPUT; }		// 

// LCM模块电源控制

//#define			LCM_POWER_ON				{ LPC_GPIO0->FIODIR |= (1UL<<6);LPC_GPIO0->FIOCLR |= (1UL<<6);} 
//#define			LCM_POWER_OFF				{ LPC_GPIO0->FIODIR |= (1UL<<6);LPC_GPIO0->FIOSET |= (1UL<<6);} 

// 外部5V供电控制
#define			EXT_5V_POWER_ON			{ LPC_GPIO1->FIODIR |= (1UL<<27);LPC_GPIO1->FIOSET |= (1UL<<27); }			// 
#define			EXT_5V_POWER_OFF			{ LPC_GPIO1->FIODIR |= (1UL<<27);LPC_GPIO1->FIOCLR |= (1UL<<27); }


// 打印机加热控制（每次加热64点）。
#define			PRINT_HOT_OFF					{  LPC_GPIO1->FIODIR |= ((1UL<<10)|(1UL<<9)|(1UL<<8)|(1UL<<4)|(1UL<<1)|(1UL<<0));\
															LPC_GPIO1->FIOCLR |= ((1UL<<10)|(1UL<<9)|(1UL<<8)|(1UL<<4)|(1UL<<1)|(1UL<<0)); }		// 
#define			PRINT_HOT_ON					{  LPC_GPIO1->FIODIR |= ((1UL<<10)|(1UL<<9)|(1UL<<8)|(1UL<<4)|(1UL<<1)|(1UL<<0));\
															LPC_GPIO1->FIOSET |= ((1UL<<10)|(1UL<<9)|(1UL<<8)|(1UL<<4)|(1UL<<1)|(1UL<<0)); }		// 

#define			PRINT_HOT_1_ON					{ 	PRINT_HOT_OFF;	LPC_GPIO1->FIOSET |= (1UL<<10); }	//
#define			PRINT_HOT_2_ON					{ 	PRINT_HOT_OFF;	LPC_GPIO1->FIOSET |= (1UL<<9); }		//
#define			PRINT_HOT_3_ON					{ 	PRINT_HOT_OFF;	LPC_GPIO1->FIOSET |= (1UL<<8); }		//
#define			PRINT_HOT_4_ON					{ 	PRINT_HOT_OFF;	LPC_GPIO1->FIOSET |= (1UL<<4); }		//
#define			PRINT_HOT_5_ON					{ 	PRINT_HOT_OFF;	LPC_GPIO1->FIOSET |= (1UL<<1); }		//
#define			PRINT_HOT_6_ON					{ 	PRINT_HOT_OFF;	LPC_GPIO1->FIOSET |= (1UL<<0); }		//


// 打印机步进马达电源控制(立即输出）
#define			STEP_MOTOR_POWER_ON			{   NOP(); }		// 
#define			STEP_MOTOR_POWER_OFF			{   NOP(); }

// LCM背光 (在主任务里定期输出）
#define			LCM_BL_POWER_ON				{ LPC_GPIO0->FIODIR |= (1UL<<7);LPC_GPIO0->FIOCLR |= (1UL<<7);  }//
#define			LCM_BL_POWER_OFF				{ LPC_GPIO0->FIODIR |= (1UL<<7);LPC_GPIO0->FIOSET |= (1UL<<7);  }//



// 步进马达时序,8拍
#define			STEP_MOTOR_PIN_MODE	   	{ LPC_PINCON->PINMODE3 |= ~((3UL << 18)|(3UL << 20)|(3UL << 24)|(3UL << 26));   }
//#define			STEP_MOTOR_PIN_IN		   	{ LPC_GPIO1->FIODIR &= ~((3UL<<25)|(3UL<<28)); }
#define			STEP_MOTOR_PIN_OUT			{ LPC_GPIO1->FIODIR |= ((3UL<<25)|(3UL<<28)); }
#define			STEP_MOTOR_STOP				{ LPC_GPIO1->FIOSET |= ((3UL<<25)|(3UL<<28)); }
#define			STEP_MOTOR_TICK_1				{ LPC_GPIO1->FIOSET |= ((1UL<<25)|(1UL<<29)); LPC_GPIO1->FIOCLR |= ((1UL<<26)|(1UL<<28)); }
#define			STEP_MOTOR_TICK_2				{ LPC_GPIO1->FIOSET |= ((1UL<<29)); LPC_GPIO1->FIOCLR |= ((1UL<<25)|(1UL<<26)|(1UL<<28)); }
#define			STEP_MOTOR_TICK_3				{ LPC_GPIO1->FIOSET |= ((1UL<<26)|(1UL<<29)); LPC_GPIO1->FIOCLR |= ((1UL<<25)|(1UL<<28)); }
#define			STEP_MOTOR_TICK_4				{ LPC_GPIO1->FIOSET |= (1UL<<26); LPC_GPIO1->FIOCLR |= ((1UL<<25)|(1UL<<28)|(1UL<<29)); }
#define			STEP_MOTOR_TICK_5				{ LPC_GPIO1->FIOSET |= ((1UL<<26)|(1UL<<28)); LPC_GPIO1->FIOCLR |= ((1UL<<25)|(1UL<<29)); }
#define			STEP_MOTOR_TICK_6				{ LPC_GPIO1->FIOSET |= (1UL<<28); LPC_GPIO1->FIOCLR |= ((1UL<<25)|(1UL<<26)|(1UL<<29)); }
#define			STEP_MOTOR_TICK_7				{ LPC_GPIO1->FIOSET |= ((1UL<<25)|(1UL<<28)); LPC_GPIO1->FIOCLR |= ((1UL<<26)|(1UL<<29)); }
#define			STEP_MOTOR_TICK_8				{ LPC_GPIO1->FIOSET |= (1UL<<25); LPC_GPIO1->FIOCLR |= ((1UL<<26)|(1UL<<28)|(1UL<<29)); }


// 切换音频通道
#define			JT808_HANDLE_AUDIO_OFF_CONTROL	{	Uart1.puts("AT+QAUDCH=2\r\n\0"); 	os_dly_wait(50);	\
														 	      Uart1.puts("AT+QAUDCH=2\r\n\0");	os_dly_wait(50);	}
#define			JT808_HANDLE_AUDIO_ON_CONTROL		{	Uart1.puts("AT+QAUDCH=2\r\n\0"); 	os_dly_wait(50);  \
															      Uart1.puts("AT+QAUDCH=2\r\n\0");	os_dly_wait(50);	} 

#define	SPEAKER_POWER_ON	{	LPC_GPIO1->FIODIR |= (1UL<<0);	 	LPC_GPIO1->FIOCLR |= (1UL<<0); }		 // 打开通话喇叭功放
#define	SPEAKER_POWER_OFF	{	LPC_GPIO1->FIODIR |= (1UL<<0);	 	LPC_GPIO1->FIOSET |= (1UL<<0); }		 //  关闭通话喇叭功放


///////////////////////////////// 输出控制 ///////////////////////////////////////////////

//// 控制输出１	 原理图网络标号：OC0
#define			CONTROL_OUT1				//{	NOP(); }	// 	锁门
#define			NULL_CONTROL_OUT1			//{	NOP(); }

// 控制输出2	 原理图网络标号：OC1
#define			CONTROL_OUT2			//	{	NOP(); }	//    开门
#define			NULL_CONTROL_OUT2		//	{	NOP(); }

// 控制输出3	 原理图网络标号：OC2
//#define			CONTROL_OUT3				{	NOP(); }		
//#define			NULL_CONTROL_OUT3			{	NOP(); }		// 

// 断油断电   原理图网络标号：OC3
#define			STOP_OIL_CONTROL			{	LPC_GPIO1->FIODIR |= (1UL<<22);	 	LPC_GPIO1->FIOSET |= (1UL<<22); }
#define			REGAIN_OIL_CONTROL		{	LPC_GPIO1->FIODIR |= (1UL<<22);	 	LPC_GPIO1->FIOCLR |= (1UL<<22); }		// 



#define			ALL_OUTPUT_HIGH			{	STOP_OIL_CONTROL	}

#define			ALL_OUTPUT_LOW				{	REGAIN_OIL_CONTROL	}

//// 模拟车速使用
//#define			SIMU_ACC_OUT_OFF			{	NOP(); }	// 
//#define			SIMU_ACC_OUT_ON			{	NOP(); }			
//#define			SIMU_MOTOR_OUT_OFF		{	NOP(); }		
//#define			SIMU_MOTOR_OUT_ON			{	NOP(); }		// 
//#define			SIMU_BRAKE_OUT_OFF		{	NOP(); }
//#define			SIMU_BRAKE_OUT_ON			{	NOP(); }		// 

// 和GPS、GPRS灯共享输出控制
//#define		IDLE_LED_ON_CONTROL				{NOP();	}
//#define		IDLE_LED_OFF_CONTROL				{NOP();	}





#endif	
/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/
