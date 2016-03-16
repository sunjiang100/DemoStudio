/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												Фң���ӣ����ݣ��з�����
$										Shawyoo Electronics (GuangZhou) R&D Center
$                       	http://www.shawyoo.com		 Email: autoele@qq.com
$								 QQȺ��9069471/9069433/9524253		All rights reserved.	  
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


#ifndef      __LEDe308_H__
#define      __LEDe308_H__


#ifdef      E308_GLOBALS
#define     E308_EXT
#else
#define     E308_EXT    extern
#endif

#include		"..\Config\RTOS.h"

#define  uint unsigned int
#define  uint16  unsigned short
#define  uchar   unsigned char
#define  uint32  unsigned int
#define  ulong   unsigned long
//#include		"RTOS.h"
// $$$$$$$$$$$$$$$$$$ <<< Use Configuration Wizard in Context Menu >>> $$$$$$$$$$$$$$$$$
// <h>S6B0719��ض���


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>S6B0719_BUS_TYPE_C68: ��������8080��C68  <0=> 8080�� <1=> C6800
//   	<i> Default:  C6800��
*/
#define			E308_BUS_TYPE			1		// ��������


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>S6B0719_TOTAL_LINE:  LCM��S6B0719��������  <32-200>
//   	<i> Default:  104
*/
#define			E308_TOTAL_LINE			32		// ������


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>S6B0719_TOTAL_COLUMN:  LCM��S6B0719��������  <32-200>
//   	<i> Default:  104
*/
#define			E308_TOTAL_COLUMN			128		// ������

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>S6B0719_TOTAL_PAGE:  LCM��S6B0719����ҳ  <32-200>
//   	<i> Default:  104
*/
#define			E308_TOTAL_PAGE			4		// ��ҳ��


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>S6B0719_TOTAL_CN_LIME:  LCM��S6B0719��������ʾ�������  <1-20>
//   	<i> Default:  6
*/
#define			E308_TOTAL_CN_LINE		4		// һ����ʾ����������



//void			DispScreen(uchar  *OLED_pBuffer);
// </h>

// $$$$$$$$$$$$$$$$$$$ <<< end of configuration section >>> $$$$$$$$$$$$$$$$$$$$$$$$$$$$

#define        Power_Crl_Latch8Bit  		22						   //P1.22	 LED��Դ����


#define        Power_Crl_Latch8Bit_ON		{	FIO1SET = (FIO1CLR & (~(1<<Power_Crl_Latch8Bit))) | (1<<(Power_Crl_Latch8Bit));	}
#define        Power_Crl_Latch8Bit_Off   	{	FIO1CLR = (FIO1CLR & (~(1<<Power_Crl_Latch8Bit))) | (1<<(Power_Crl_Latch8Bit));	}


#define			WD_DELAY	{ 	NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();}



#define		MAX_SCREEN_NUM				10

#define		MAIN_MENU_MAX_LINE		2

//#define		MAX_SUM_MENU				12

////////////// ������ʾ���ĸ���������
#define		CLEAR_DISP			0x01
#define		SET_AC				0x02
#define		BASIC_COMM			0x38
#define		EXPAND_COMM			((1<<5)|(1<<4)|(1<<2))

E308_EXT		uchar		ucaMilage[6];

////////////// ���������Ļ��ʾ�Ŀ�ʼ��////////////////
									

//����ʾ���ַ����棬2��8��
//#pragma		pack(1)
typedef   struct _Disp
{	
	uint16	CN[2][8];
	uchar	Buffer[2][16];
} Disp;

E308_EXT	Disp   Dispg;




//��ʾ��ָ���б�
E308_EXT		enum
{
	E308_CMD_SET_PAGE_ADDR			= 0xb0,
	E308_CMD_SET_COL_ADDR_MSB		= 0x10,
	E308_CMD_SET_COL_ADDR_LSB 		= 0x00,
	E308_CMD_SET_MODIFY_READ 		= 0xe0,
	E308_CMD_RESET_MODIFY_READ 	    = 0xee,
	E308_CMD_DISPLAY_OFF 			= 0xae,
	E308_CMD_DISPLAY_ON  			= 0xaf,
	//E308_CMD_RESET						= 0xe2,
	E308_CMD_OSC_ON_START			= 0xab,
	E308_CMD_SLEEP_MODE				= 0xa9,
	E308_CMD_STANDBY_MODE			= 0xa8,
	E308_CMD_RELEASE_POWER_SAVE   = 0xe1,

	//E308_CMD_FORCE_ALL_ON			= 0xa5,
	//E308_CMD_FORCE_ALL_OFF			= 0xa4

	//S6B0719_CMD_TEST						= 0xf0,

} E308_COMMAND_ENUM;
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	����������
*/ 	
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	OLED��
*/
class			OLEDCls
{
public:
	OLEDCls(void);
	void		Init(void);
	void		DataDisplaying(uchar Line,uchar Page,uchar Column,uint16 Length,uchar *pData);
	void		WriteCommand(uchar aa);
	void     Give_Pwr(uchar PWR);	
	void     OLEDTest(uchar TDa); 
	void 		ClearLCD(void);
	void		SetLED_Status(uchar no,uchar Data);
  	void		ReceiveProcessData(void);	
	uchar		WaitLcmDataToServer(uint16);
	void		PowerOnTiming(void);
	void     Lcd_Data(uchar data);
	void     Lcd_Command(uchar cmd);
	void     disp_all(uint x1,uint y1,uint x2,uint y2,uint x3,uint y3,uint x4,uint y4);


	void     OLEDDisplay_ZT1616(uchar y,uchar x,uint16 CodeAddr,uchar InvertFlag);
	void 		OLEDDisplay_ASC816(uchar y,uchar x,uchar CodeAddr,uchar InvertFlag);
	void     OLEDDispScreen(uchar xs,uchar ys,uchar len);
private:
	uint32	DispCnt;
	uint16	NoDriverCnt;
//	LAMP   	Lamp;
	uchar		DriverLogStatus;
	////////////////////////////
	
//	void		eTaskLED(void);
	void		TurnOffLed(uchar);
	void		TurnOnLed(uchar);	
	
};


E308_EXT		__noinit__		OLEDCls		OLED;






#endif
/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
