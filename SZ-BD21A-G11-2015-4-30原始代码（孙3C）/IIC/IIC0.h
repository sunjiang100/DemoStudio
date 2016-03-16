/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												���ݹ�ҵ���ӿƼ����޹�˾
$										Shawyoo Electronics (GuangZhou) R&D Center
$                       	http://www.guoyee.com		 Email: guoyegps@126.com
$								 QQȺ��9069471/9069433/9524253		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
	�ļ���: 	  IIC0.h
	���������� IIC0�ײ���������ͷ�ļ�
	���뻷���� Keil for ARM + ARTX
	Ŀ��CPU:   LPC2000
	���ߣ�	  Ф����
	����ʱ�䣺 
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  

#ifndef        __IIC0_H__
#define			__IIC0_H__



#ifdef      IIC0_GLOBALS
#define     IIC0_EXT
#else
#define     IIC0_EXT    extern
#endif


#include		"TWI.h"

#include		"..\Config\RTOS.h"

// $$$$$$$$$$$$$$$$$$ <<< Use Configuration Wizard in Context Menu >>> $$$$$$$$$$$$$$$$$
// <h>IIC0�������


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>I2C_FREQUENCY: IIC0���������� <100000-400000>
//   	<i> Default: 100000
*/
#define		I2C_FREQUENCY      100000

#if (I2C_FREQUENCY >= 400000)
#error  "Out of IIC MAX Frequency"
#endif	



#define			MAX_PAGE_LEN			32

#define			CARD_DEVICE_ADDR		0xa0			// A1 = A0 = 0
// </h>
// $$$$$$$$$$$$$$$$$$$ <<< end of configuration section >>> $$$$$$$$$$$$$$$$$$$$$$$$$$$$


						
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    IIC�ӻ�������ַ
*/
IIC0_EXT		enum
{
	IIC_DA_MEGA16 = 0x08,
	
	////////////////////////////////////
	IIC_KIONIX_KXTF9 = 0x1E,			   // ��ַ����0x0f ��������һλ�ĵõ� 
	EE_DA_AT24C01_16_IC_CARD	= 0xa0,
	EE_DA_PARAMETER_CFG = 0xa2,
	EE_DA_ELE_RAIL = 0xa4,
	EE_DA_VTDR = 0xa6,
	RTC_DEVICE_ADDR_R8025 = 0x64,
	
} IIC0_DEVICE_ADDRESS;
#define		EE_DA_GPS_SAVE2			0xa0

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    IIC��������ö��
*/
IIC0_EXT		enum
{
	IIC_GET_SPEED_COMMAND = 1,
	IIC_GET_GPS_COMMAND,
	IIC_GET_T485_COMMAND,
	IIC_GET_GYRO_COMMAND,	
	IIC_EEPROM_UART0,
	IIC_EEPROM_GSM_GPRS,
	IIC_READ_COMM,						// IIC������(û���ظ���ʼ�ź�)
	IIC_WRITE_COMM,					// IICд(û���ظ���ʼ�ź�)
	IIC_RW_COMM,						// IICд��(���ظ���ʼ�ź�)
	IIC_INIT_CHECK_COMM = 'I',
	IIC_BEEP_LED_COMM  = 'B',
	IIC_SET_LED_BUZZER_COMMAND = 0x81,
} IIC_COMMAND_ENUM;




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	IIC��
*/
class			IICCls
{
public:
	IICCls(void);
	void		Init(void);
	bool		Read(uchar SAddr,uchar Cmd,uint16 Len,uchar *pBuffer);
	bool		Write(uchar SAddr,uchar Cmd,uint16 Len,uchar *pBuffer);
	bool		WriteRead(uchar SAddr,uchar Cmd,uint16 WLen,uchar *pW,uint16 RLen,uchar *pR);
	bool		ReadEEPROM(uchar DAddr,uint16 RAddr,uint16 RLen,uchar *pData);
	bool		WriteEEPROM(uchar  DAddr,uchar PageLen,uint16 WAddr,uint16 WLen,uchar *pData);
	__inline		void		Interrupt(void);		

	uint32	 Reset4442Card(void);
	bool     Read4442Card(uint16 Addr,uint16 Len,uchar *pBuffer);
	bool     Write4442Card(uint16 Addr,uint16 Len,uchar *pBuffer);
	void     WriteCmd4442Card(uchar Cmd,uchar Addr,uchar Data);
	void     WriteByte4442Card(uchar Data);
	uchar    ReadByte4442Card(void);
	void		PinFor4442(uchar Pin);	
	void		Start4442Card(void);
	void		Stop4442Card(void);
	void		Break4442Card(void);
	void		Test4442Card(void);
	void		CheckIC_Card(void);
private:
	
	uchar		volatile	Command;	
	uchar		volatile	Success;
	uchar		volatile	ReadWrite;
	uchar		volatile	SlaverAddress;		
	uchar		volatile	I2C_Busy;
	uchar		volatile	RW_Type;
	uchar		volatile	Status;
	uint16  	volatile	ReadLength;
	uint16	volatile	WriteLength;
	uchar		volatile	*pReadBuffer;
	uchar		volatile	*pWriteBuffer;
	//uchar		WriteBuffer[256];					// �ⲿ����д��������ʹ�þֲ�������������ȴ�
};



IIC0_EXT	__noinit__		IICCls		I2C0;










#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
