			  

#ifndef      __USERCONFIG_H__
#define      __USERCONFIG_H__
/////////////////////////////// 

#ifdef      CFG_GLOBALS
#define     CFG_EXT
#else
#define     CFG_EXT    extern
#endif


#ifndef		__noinit__
#define 	__noinit__ 		__attribute__((zero_init))//��������ʼ��Ϊ0
#endif







#include		"VariableType.h"

//#include		".\Common\ConstCode.h"

#include		".\Common\Productor.h"

#include    <string.h>
				
#include 	<RTL.h>                      /* RTX kernel functions & defines      */
#include 	<LPC17xx.H>                  /* LPC17xx definitions                 */



//-------- <<< Use Configuration Wizard in Context Menu >>> -----------------

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	RTX��uCOS������Ӧ
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
#define 		MSG_ERROR 				0x02		  // ��Ϣ����
#endif
#ifndef 		MSG_NOT_SUPPORT
#define 		MSG_NOT_SUPPORT 		0x03		  // ��֧�ֵ���Ϣ
#endif
#ifndef 		MSG_ALARM_CONFIRM
#define 		MSG_ALARM_CONFIRM 	0x04		  // ��������ȷ��
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
//   	<o>IO_FOR_MODEL:     ��Ʒ�ͺ�IO���䶨��  <0=> GY_G8 	<1=> GY_G08Small
//   	<i> Default: GY_G8
*/
#define			IO_FOR_MODEL					0


#define			MODEL_GY_G8						0
#define			MODEL_GY_G08Small				1


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>APN_USE_GYJT:  APN����  <0=>  Ĭ���й��ƶ� 	<1=> ���ݽ�Ͷ
//   	<i> Default: 1
*/
#define		APN_USE_GYJT						1


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>LIMIT_CONST_ID_RANGE: ��ƷΨһID����  <0=> ������ID��Χ 	<1=> ����ID��Χ
//   	<i> Default: 0
*/
#define		LIMIT_CONST_ID_RANGE				1



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>RTC_ADJUST_VALUE: ����ʱ����GPS��׼ʱ��ĵ���ֵ(Сʱ��  <0-12>
//   	<i> default: 8
*/
#define			RTC_HOUR_ADJUST_VALUE		8





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>USE_TH_GPRS_DATA_PACK_PROTOCOL: ʹ��TH���ݰ�Э��  <0=> ��ʹ�� <1=> ʹ��
//   	<i> Default: ʹ��
*/
#define			USE_TH_GPRS_DATA_PACK_PROTOCOL		1



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>NEED_CODE_CRC: ϵͳ����CRC����  <0=> ����У�� 	<1=> ����У��
//   	<i> Default: 0
*/
#define			NEED_CODE_CRC		0








// <h>CPUʱ���������		  
																										  
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>MAIN_PLL_MSEL: MAIN_PLL��Ƶϵ�� <16-65535>
//   	<i> Default: 100
*/
#define 		MAIN_PLL_MSEL	      	100		 // ע�⣺�˴���ֵ������system_LPC17xx.c�еĳ���<CCLKCFG>��ƥ��

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>MAIN_PLL_NSEL: MAIN_PLL��Ƶϵ�� <1-16>
//   	<i> Default: 6
*/
#define 		MAIN_PLL_NSEL	      	6		 // ע�⣺�˴���ֵ������system_LPC17xx.c�еĳ���<CCLKCFG>��ƥ��


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>CCLK_DIV: CPUƵ�ʴ�PLL�ķ�Ƶϵ�� <1=> ����MAIN_PLL <2=> 2��Ƶ <4=> 4��Ƶ	 <8=> 8��Ƶ
//   	<i> Default: 4
*/
#define 		CCLK_DIV	      			4		 // ע�⣺�˴���ֵ������system_LPC17xx.c�еĳ���<CCLKCFG>��ƥ��




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>FoscSel: �ⲿ����Ƶ��(Hz)	 <1=> 11.0592MH  <2=> 12M
//   	<i> Default: 1
*/
#define			FoscSel			2

#if( FoscSel == 1)

#define 		Fosc			11059200L

#elif( FoscSel == 2)

#define 		Fosc			12000000L           //����Ƶ��,10MHz~25MHz��Ӧ����ʵ��һ��

#endif

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>OUTSIDE_WATCH_DOG: ѡ���Ź�   <0=> �ڲ����Ź�  <1=> �ⲿ���Ź�
//   	<i> Default: 	�ڲ����Ź�	
*/
#define		OUTSIDE_WATCH_DOG			0

#define		FmainPll		(2L*Fosc*MAIN_PLL_MSEL/MAIN_PLL_NSEL)

#define 		Fcclk1       (1L*FmainPll/CCLK_DIV)   	//ϵͳƵ�ʣ�����ΪFosc��������(1~32)����<=60MHZ

#define		Fcclk			100000000L


// </h>





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>VEHICLE_TYPE: ��������  <0=> ��ͨ���� <1=> ������
//   	<i> Default: ������
*/						 
#define		VEHICLE_TYPE				1


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>USE_EEPROM_SAVE_GPS_DATA: �Ƿ�ʹ��EEPROM�洢GPS��ʷ����  <0=> ��ʹ�� <1=> ʹ��
//   	<i> Default: ��ʹ��
*/						 
#define		USE_EEPROM_SAVE_GPS_DATA		0



// <h>ȫ�ֶ��������������



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>GPS_ANT_CHECK_COMMAND: GPS���߼��  <0=> ʹ��IO��� <1=> ʹ�������⿪��
//   	<i> default: 0
*/
#define			GPS_ANT_CHECK_COMMAND		1



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>LEN_PUBLIC_FREE_MEM: ��̬�ڴ���䳤��  <128-2048>
//   	<i> default: 512
*/
#define			LEN_PUBLIC_FREE_MEM		(1024+32)



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>CONST_ID: ʹ�ù̶�ID   <0=> ʹ�ÿ��޸ĵ� <1=> �̶�ID
//   	<i> Default: �̶�ID
*/
#define			CONST_ID			1

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>CONST_IP: ʹ�ù̶�IP   <0=> ʹ�ÿ��޸ĵ� <1=> �̶�IP
//   	<i> Default: �̶�IP
*/
#define			CONST_IP			0

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>ENCRYPT: ����CPU   <0=> ������ <1=> ����
//   	<i> Default: ����
*/
#define		ENCRYPT				1       // 0�������ܣ���0������


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>NEED_SERIAL_NUMBER_CODE_ADDRESS: ����CPU   <0=> ����Ҫ <1=> ��Ҫ
//   	<i> Default: ��Ҫ
*/
#define		NEED_SERIAL_NUMBER_CODE_ADDRESS		1       // 0������Ҫ����0����Ҫ



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>REMOTE_KEYBOARD: �Ƿ�Զ�̼���   <0=> һ��� <1=> Զ�̼���
//   	<i> Default: һ���
*/
#define		REMOTE_KEYBOARD				1



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>REMOTE_DISPLAY: �Ƿ�Զ����ʾ  <0=> һ��� <1=> Զ����ʾ
//   	<i> Default: һ���
*/
#define		REMOTE_DISPLAY					1

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>REMOTE_DRIVER_CARD: �Ƿ�Զ��˾����  <0=> һ��� <1=> Զ��˾����
//   	<i> Default: һ���
*/

#define		REMOTE_DRIVER_CARD			0




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>NAND_FLASH: �Ƿ�֧��NandFlash  <0=> ��֧�� <1=> ֧��
//   	<i> Default:   ��֧��
*/
#define		USE_NAND_FLASH					1


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>USE_GPS_MODEL: ʹ��GPSģ��  <0=> ��ʹ�� <1=> ʹ��
//   	<i> Default: ��ʹ��
*/						 
#define		USE_GPS_MODEL					1


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>USE_MOBILE_MODEL: ʹ���ֻ�ģ��  <0=> ��ʹ�� <1=> ʹ��
//   	<i> Default: ʹ��
*/						 
#define		USE_MOBILE_MODEL				1



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>NEED_ADC: ��Ҫʹ��ADC������  <0=> ����Ҫ <1=> ��Ҫ
//   	<i> Default: ����Ҫ
*/
#define		NEED_ADC							1


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>EXTERN_RTC_CLOCK: RTCʹ���ⲿ32K���� <0=> ʹ���ڲ���Ƶ <1=> ʹ���ⲿ32K����
//   	<i> Default: ʹ���ⲿ32K����
*/						 
#define		EXTERN_RTC_CLOCK				1



#if(VEHICLE_TYPE == 0)


// <e> ��ͨ����Ĭ�����ݲɼ�ʱ����

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>DEFAULT_ACC_RUN_SAMPLE_PERIOD: ����������ʱ�������� <5-3600>
//   	<i> default: 600
*/
#define			DEFAULT_ACC_RUN_SAMPLE_PERIOD		10

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>DEFAULT_ACC_STOP_SAMPLE_PERIOD: ������������ʱ�������� <5-3600>
//   	<i> default: 600
*/
#define			DEFAULT_ACC_STOP_SAMPLE_PERIOD		10

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>DEFAULT_MIN_SAMPLE_PERIOD: Ĭ����С�������� <10-60>
//   	<i> default: 600
*/
#define			DEFAULT_MIN_SAMPLE_PERIOD		10

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>DEFAULT_MAX_SAMPLE_PERIOD: Ĭ������������ <300-1800>
//   	<i> default: 600
*/
#define			DEFAULT_MAX_SAMPLE_PERIOD		600

// </e>

#elif(VEHICLE_TYPE == 1)


// <e> ��������Ĭ�����ݲɼ�ʱ����

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>DEFAULT_ACC_RUN_SAMPLE_PERIOD: ����������ʱ�������� <60-7200>
//   	<i> default: 600
*/
#define			DEFAULT_ACC_RUN_SAMPLE_PERIOD		300

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>DEFAULT_ACC_STOP_SAMPLE_PERIOD: ������������ʱ�������� <60-7200>
//   	<i> default: 600
*/
#define			DEFAULT_ACC_STOP_SAMPLE_PERIOD		300

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>DEFAULT_MIN_SAMPLE_PERIOD: Ĭ����С�������� <5-60>
//   	<i> default: 600
*/
#define			DEFAULT_MIN_SAMPLE_PERIOD		5


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>DEFAULT_MAX_SAMPLE_PERIOD: Ĭ������������ <300-1800>
//   	<i> default: 600
*/
#define			DEFAULT_MAX_SAMPLE_PERIOD		65535

// </e>

#endif

// </h>


// <h> ѡ���Ƿ����ĳЩ����


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>SYSTEM_BOOT_MESSAGE: ϵͳ������Ϣ���� <0=> ������ <1=> ����
//   	<i> Default: ������
*/
#define		SYSTEM_BOOT_MESSAGE					0


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>COMPILE_GPS_TEST_DATA_CODE: GPS�������ݴ��� <0=> ������ <1=> ����
//   	<i> Default: ������
*/
#define		COMPILE_GPS_TEST_DATA_CODE			1

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>COMPILE_GPRS_TEST_DATA_CODE: GPRS�������ݴ��� <0=> ������ <1=> ����
//   	<i> Default: ������
*/
#define		COMPILE_GPRS_TEST_DATA_CODE			1

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>COMPILE_GSM_GPRS_TEST_DATA_CODE: GSM_GPRS�������ݴ��� <0=> ������ <1=> ����
//   	<i> Default: ������
*/
#define		COMPILE_GSM_GPRS_TEST_DATA_CODE			1

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>COMPILE_ADC_TEST_DATA_CODE: ADת���������ݴ��� <0=> ������ <1=> ����
//   	<i> Default: ������
*/
#define		COMPILE_ADC_TEST_DATA_CODE			1

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>COMPILE_SW_TEST_DATA_CODE: �������������ݴ��� <0=> ������ <1=> ����
//   	<i> Default: ������
*/
#define		COMPILE_SW_TEST_DATA_CODE			1


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>DFE_DEBUG: DF�������ݴ��� <0=> ������ <1=> ����
//   	<i> Default: ������
*/
#define		DFE_DEBUG			1



// </h>



//////////////////////////// ���޸ĳ����ļ�ConstCode.h��constModelVersionʱ�����޸����漸���궨��
//#define		LEN_MODEL_VERSION		55
//#define		LEN_MODEL				7
//#define		MV_POS_MODEL			7
//#define		MV_POS_HV				36
//#define		MV_POS_SV				50
//#define		BUILD_DATE_POS			(LEN_MODEL_VERSION+1)
//#define		BUILD_TIME_POS			(BUILD_DATE_POS+12)


#define		WUXI_TEST				0			// 1: �������		0: ʵ��ʹ��


//------------- <<< end of configuration section >>> -----------------------



#define        UserConst  ((USER_CODE_CONST *)(CONST_BASE_ADDR))


//////// RFID ȱʡ��Դ����
#define		DEFAULT_POWER_ON_RFID		1
//


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   IO�����������ȫ�������ﶨ��
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



//// IC��λ����IC������ʹ����ͬIO�ڣ�����ʹ�ó��գ����뿨ʱ���ؿ�������Ϊ�ߵ�ƽ��
//#define		IC_CARD_RESET_PIN_HIGH		{ LPC_GPIO0->FIODIR |= (1UL<<0);	 	LPC_GPIO0->FIOSET |= (1UL<<0); }
//#define		IC_CARD_RESET_PIN_LOW		{ LPC_GPIO0->FIODIR |= (1UL<<0);	 	LPC_GPIO0->FIOCLR |= (1UL<<0); }

// IC��λ����IC������ʹ����ͬIO�ڣ�������G11����ʹ�ó��������뿨ʱ���رպϣ�����Ϊ�ӵأ�
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
#else			// G11��ָʾ��
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
//������	/////////// add by wrc  ///////////////								
//#define     BUZZER_ON			  {  LPC_GPIO1->FIODIR |= (1UL<<17);  LPC_GPIO1->FIOCLR |= (1UL<<17);}		
//#define     BUZZER_OFF			  {  LPC_GPIO1->FIOSET |= (1UL<<17);}		 //LPC_GPIO1->FIODIR &= ~(1UL<<17);		
							
#define     BUZZER_ON			  {  LPC_GPIO2->FIODIR |= (1UL<<4);  LPC_GPIO2->FIOSET |= (1UL<<4);}		
#define     BUZZER_OFF			  {  LPC_GPIO2->FIODIR |= (1UL<<4);  LPC_GPIO2->FIOCLR |= (1UL<<4);}		 //LPC_GPIO1->FIODIR &= ~(1UL<<17);		



///////////// �����ֻ�ģ������ߵ��������  /////////////////////////////////////////////
// HC596���ͳһ����������������ƣ������ᵼ�������������ʱԼ10ms������Ӱ�졣
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


// RFID ��Դ����////////////////////////////////////////////////////////////
#define		RFID_POWER_ON			  	{  LPC_GPIO1->FIODIR |= (1UL<<27);  LPC_GPIO1->FIOSET |= (1UL<<27);}
#define		RFID_POWER_OFF				{  LPC_GPIO1->FIODIR |= (1UL<<27);  LPC_GPIO1->FIOCLR |= (1UL<<27);}



//HC596���ͳһ����������������ƣ������ᵼ�������������ʱԼ10ms������Ӱ�졣
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
   �洢��Ƭѡ
*/
// SPI EEPROM/����洢Ƭѡ��
#define			DFE_CSEnable		{ LPC_GPIO0->FIODIR |= ((1UL<<19)|(1UL<<20)); LPC_GPIO0->FIOCLR |= (1UL<<20);LPC_GPIO0->FIOSET |= (1UL<<19);\
											  LPC_GPIO2->FIODIR |= (1UL<<10); LPC_GPIO2->FIOSET |= (1UL<<10);NOP();	NOP();}
#define			DFE_CSDisable		{ LPC_GPIO0->FIOSET |= (1UL<<20);  	}

// SPI FlashƬѡ
#define			DFF_CSEnable		{ LPC_GPIO0->FIODIR |= ((1UL<<19)|(1UL<<20)); LPC_GPIO0->FIOCLR |= (1UL<<19);LPC_GPIO0->FIOSET |= (1UL<<20);\
										  		LPC_GPIO2->FIODIR |= (1UL<<10); LPC_GPIO2->FIOSET |= (1UL<<10);NOP();	NOP();}
#define			DFF_CSDisable	 	{ LPC_GPIO0->FIOSET |= (1UL<<19);	}




//SSP0 ��ʾѡ��			 //��		  /////////// add by wrc  ///////////////
#define			Disp_CSEnable		{ LPC_GPIO1->FIODIR |= (1UL<<21); LPC_GPIO1->FIOCLR |= (1UL<<21); NOP();	NOP();}			
											
#define			Disp_CSDisable		{ LPC_GPIO1->FIODIR |= (1UL<<21); LPC_GPIO1->FIOSET |= (1UL<<21);  	}

//��ʾ��ָ�����ݷ���ʹ��	  //�ϵ�23   �µ�8
//#define			Disp_Data_WI_Enable		    { LPC_GPIO1->FIODIR |= (1UL<<23); LPC_GPIO1->FIOSET |= (1UL<<23); NOP();	NOP();}
//#define			Disp_Command_WI_Enable   	 { LPC_GPIO1->FIODIR |= (1UL<<23); LPC_GPIO1->FIOCLR |= (1UL<<23); NOP();	NOP();}

#define			Disp_Data_WI_Enable		    { LPC_GPIO1->FIODIR |= (1UL<<8); LPC_GPIO1->FIOSET |= (1UL<<8); NOP();	NOP();}
#define			Disp_Command_WI_Enable   	 { LPC_GPIO1->FIODIR |= (1UL<<8); LPC_GPIO1->FIOCLR |= (1UL<<8); NOP();	NOP();}


// LCM���ݡ�����ѡ��		   //�ϵ�23   �µ�8
//#define			LCM_DATA_ENABLE	{ LPC_GPIO1->FIODIR |= (1UL<<23);LPC_GPIO1->FIOSET |= (1UL<<23); }
//#define			LCM_CMD_ENABLE		{ LPC_GPIO1->FIODIR |= (1UL<<23);LPC_GPIO1->FIOCLR |= (1UL<<23); }

#define			LCM_DATA_ENABLE	{ LPC_GPIO1->FIODIR |= (1UL<<8);LPC_GPIO1->FIOSET |= (1UL<<8); }
#define			LCM_CMD_ENABLE		{ LPC_GPIO1->FIODIR |= (1UL<<8);LPC_GPIO1->FIOCLR |= (1UL<<8); }

// LCMƬѡ
#define			LCM_CS_ENABLE		{ LPC_GPIO1->FIODIR |= (1UL<<21);LPC_GPIO1->FIOCLR |= (1UL<<21); }
#define			LCM_CS_DISABLE		{ LPC_GPIO1->FIODIR |= (1UL<<21);LPC_GPIO1->FIOSET |= (1UL<<21); }


#define          LCM_POWER_ON   	{  LPC_GPIO0->FIODIR |= (1UL<<6);LPC_GPIO0->FIOCLR |= (1UL<<6);}
#define          LCM_POWER_OFF  	{  LPC_GPIO0->FIODIR |= (1UL<<6);LPC_GPIO0->FIOSET |= (1UL<<6);}
#define          RET_LCM_LOW    	LCM_POWER_OFF
#define          RET_LCM_HIGH    	LCM_POWER_ON		
#define          LCM_BL_ENABLE  	{  LPC_GPIO0->FIODIR |= (1UL<<7);LPC_GPIO0->FIOSET |= (1UL<<7);}
#define          LCM_BL_DISABLE    {  LPC_GPIO0->FIODIR |= (1UL<<7);LPC_GPIO0->FIOCLR |= (1UL<<7);}

//#if  (PRODUCT_TYPE==SHANGHAI_SIZENG_PRT)		  // �Ϻ�˼������IO��Ҫ�޸ĵ� //�ϵ�23   �µ�8
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



// ��ӡ����������
#define			PRINT_DATA_LATCH_HIGH	{ LPC_GPIO1->FIODIR |= (1UL<<17);LPC_GPIO1->FIOSET |= (1UL<<17); }
#define			PRINT_DATA_LATCH_LOW		{ LPC_GPIO1->FIOCLR |= (1UL<<17); }



		
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   IO������ܽ�
*/
#define			GET_IO_PIN_INIT				{ LPC_GPIO1->FIODIR &= ~((3UL<<18)); LPC_GPIO2->FIODIR &= ~(0x00000007<<11);\
														  LPC_GPIO0->FIODIR &= ~((1UL<<0)); LPC_GPIO3->FIODIR &= ~((3UL<<25));}
#define			GET_SPEED_PLUSE_STATUS		((LPC_GPIO0->FIOPIN>>23)&0x00000001) 	// ��ȡ�ٶ���������״̬
#define			GET_BRAKE_STATUS				((LPC_GPIO1->FIOPIN>>18)&0x00000001)	// ��ȡɲ�����״̬
#define			GET_ACC_STATUS					((LPC_GPIO1->FIOPIN>>19)&0x00000001)	// ��ȡACC���״̬
#define         GET_BUTTON_STATUS          ((LPC_GPIO2->FIOPIN>>11)&0x00000001)   // ��ȡ����������ť����״̬
#define			GET_DIPPED_BEAM				((LPC_GPIO2->FIOPIN>>12)&0x00000001)	// �����
#define			GET_HIGH_BEAM					((LPC_GPIO2->FIOPIN>>13)&0x00000001)	// Զ���
#define			GET_RIGHT_LIGHT				((LPC_GPIO3->FIOPIN>>25)&0x00000001)	// ��ת��
#define			GET_LEFT_LIGHT					((LPC_GPIO3->FIOPIN>>26)&0x00000001)	// ��ת��

#define			GET_MOTO_STATUS					((LPC_GPIO2->FIOPIN>>11)&0x00000001)	// MOTO	 �������ڽ�������


//#define			GET_HIGH_SPEED_PLUSE_STATUS		(NOP();)									  	// ��ȡ������������״̬


//#define			GET_GPS_ANT_INIT				{NOP();}
//#define			GET_GPS_ANT_SHORT_STATUS	(NOP();)													// ��ȡGPS����״̬
//#define			GET_GPS_ANT_OPEN_STATUS		(NOP();)													// ��ȡGPS����״̬

#define			GET_IC_CARD_STATUS			((LPC_GPIO0->FIOPIN>>0)&0x00000001)

//#define			GET_TF_CARD_STATUS			((LPC_GPIO0->FIOPIN>>16)&0x00000001)

#define        GET_USB_STATUS             ((LPC_GPIO0->FIOPIN>>29)&0x00000001)


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   
*/
		
// �ֱ�/��������Դ
//#define			HANDLE_POWER_ON			{ LPC_GPIO1->FIODIR |= (1UL<<27);LPC_GPIO1->FIOSET |= (1UL<<27); }			// �ֱ�/��������Դ	
//#define			HANDLE_POWER_OFF			{ LPC_GPIO1->FIODIR |= (1UL<<27);LPC_GPIO1->FIOCLR |= (1UL<<27); }

// ����ͷ��Դ����						 		
#define			CAMERA_POWER_ON			{ LPC_GPIO0->FIODIR |= (1UL<<5);LPC_GPIO0->FIOSET |= (1UL<<5); }		// ����ͷ��Դ
#define			CAMERA_POWER_OFF			{ LPC_GPIO0->FIODIR |= (1UL<<5);LPC_GPIO0->FIOCLR |= (1UL<<5); }

// ��ӡ����Դ����
#define			PRINT_POWER_ON          { 	LPC_GPIO0->FIODIR |= (1UL<<8);LPC_GPIO0->FIOCLR |= (1UL<<8);}
#define			PRINT_POWER_OFF			{ 	LPC_PINCON->PINMODE0 &= (1UL<<16); LPC_PINCON->PINMODE0 |= (1UL<<17);\
														LPC_PINCON->PINMODE_OD0 |= (1UL<<8);LPC_GPIO0->FIODIR |= (1UL<<8);\
														LPC_GPIO0->FIOSET |= (1UL<<8);  	}
// ��Դ����
//#define			POWER_SELF_LOCK_ON			//{   HC595.Out1.Bits.PowerSeldLock = 1;	}//HC595_OUTPUT; }		// 
//#define			POWER_SELF_LOCK_OFF			//{   HC595.Out1.Bits.PowerSeldLock = 0;	}//HC595_OUTPUT; }		// 

// LCMģ���Դ����

//#define			LCM_POWER_ON				{ LPC_GPIO0->FIODIR |= (1UL<<6);LPC_GPIO0->FIOCLR |= (1UL<<6);} 
//#define			LCM_POWER_OFF				{ LPC_GPIO0->FIODIR |= (1UL<<6);LPC_GPIO0->FIOSET |= (1UL<<6);} 

// �ⲿ5V�������
#define			EXT_5V_POWER_ON			{ LPC_GPIO1->FIODIR |= (1UL<<27);LPC_GPIO1->FIOSET |= (1UL<<27); }			// 
#define			EXT_5V_POWER_OFF			{ LPC_GPIO1->FIODIR |= (1UL<<27);LPC_GPIO1->FIOCLR |= (1UL<<27); }


// ��ӡ�����ȿ��ƣ�ÿ�μ���64�㣩��
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


// ��ӡ����������Դ����(���������
#define			STEP_MOTOR_POWER_ON			{   NOP(); }		// 
#define			STEP_MOTOR_POWER_OFF			{   NOP(); }

// LCM���� (���������ﶨ�������
#define			LCM_BL_POWER_ON				{ LPC_GPIO0->FIODIR |= (1UL<<7);LPC_GPIO0->FIOCLR |= (1UL<<7);  }//
#define			LCM_BL_POWER_OFF				{ LPC_GPIO0->FIODIR |= (1UL<<7);LPC_GPIO0->FIOSET |= (1UL<<7);  }//



// �������ʱ��,8��
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


// �л���Ƶͨ��
#define			JT808_HANDLE_AUDIO_OFF_CONTROL	{	Uart1.puts("AT+QAUDCH=2\r\n\0"); 	os_dly_wait(50);	\
														 	      Uart1.puts("AT+QAUDCH=2\r\n\0");	os_dly_wait(50);	}
#define			JT808_HANDLE_AUDIO_ON_CONTROL		{	Uart1.puts("AT+QAUDCH=2\r\n\0"); 	os_dly_wait(50);  \
															      Uart1.puts("AT+QAUDCH=2\r\n\0");	os_dly_wait(50);	} 

#define	SPEAKER_POWER_ON	{	LPC_GPIO1->FIODIR |= (1UL<<0);	 	LPC_GPIO1->FIOCLR |= (1UL<<0); }		 // ��ͨ�����ȹ���
#define	SPEAKER_POWER_OFF	{	LPC_GPIO1->FIODIR |= (1UL<<0);	 	LPC_GPIO1->FIOSET |= (1UL<<0); }		 //  �ر�ͨ�����ȹ���


///////////////////////////////// ������� ///////////////////////////////////////////////

//// ���������	 ԭ��ͼ�����ţ�OC0
#define			CONTROL_OUT1				//{	NOP(); }	// 	����
#define			NULL_CONTROL_OUT1			//{	NOP(); }

// �������2	 ԭ��ͼ�����ţ�OC1
#define			CONTROL_OUT2			//	{	NOP(); }	//    ����
#define			NULL_CONTROL_OUT2		//	{	NOP(); }

// �������3	 ԭ��ͼ�����ţ�OC2
//#define			CONTROL_OUT3				{	NOP(); }		
//#define			NULL_CONTROL_OUT3			{	NOP(); }		// 

// ���Ͷϵ�   ԭ��ͼ�����ţ�OC3
#define			STOP_OIL_CONTROL			{	LPC_GPIO1->FIODIR |= (1UL<<22);	 	LPC_GPIO1->FIOSET |= (1UL<<22); }
#define			REGAIN_OIL_CONTROL		{	LPC_GPIO1->FIODIR |= (1UL<<22);	 	LPC_GPIO1->FIOCLR |= (1UL<<22); }		// 



#define			ALL_OUTPUT_HIGH			{	STOP_OIL_CONTROL	}

#define			ALL_OUTPUT_LOW				{	REGAIN_OIL_CONTROL	}

//// ģ�⳵��ʹ��
//#define			SIMU_ACC_OUT_OFF			{	NOP(); }	// 
//#define			SIMU_ACC_OUT_ON			{	NOP(); }			
//#define			SIMU_MOTOR_OUT_OFF		{	NOP(); }		
//#define			SIMU_MOTOR_OUT_ON			{	NOP(); }		// 
//#define			SIMU_BRAKE_OUT_OFF		{	NOP(); }
//#define			SIMU_BRAKE_OUT_ON			{	NOP(); }		// 

// ��GPS��GPRS�ƹ����������
//#define		IDLE_LED_ON_CONTROL				{NOP();	}
//#define		IDLE_LED_OFF_CONTROL				{NOP();	}





#endif	
/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/
