


#define		USER_GLOBALS		1		 
#include		"Common\UserGlobals.h"



#define		USER_OS_GLOBALS		1
#include		"Config\RTOS.h"

			 

#define		CFG_GLOBALS			1
#include		"Config\UserConfig.h"

#include		"UserSys.h"	

#include		"SPI\SPI.h"

#include		"FLASH\DFE_25xxx.H"
#include		"FLASH\DFF_25xxx.H"
#include    "KeyDisp\LEDe308.h"
#include		"Common\Algol.h"

#include		".\SPI\SSP.h"
#include			".\Print\Print.h"


#define			ConstPARA	((uchar *)(0x2000+0x0020))
#define			ConstCRC1	((uchar *)(0x2000+0x0090))
#define			ConstCRC2	((uchar *)(0x2000+0x0092))
#define        ConstPID  	((uchar *)(0x2000+0x00a0))
#define        ConstCRC3  	((uchar *)(0x2000+0x013e))




unsigned short	Encrypt3TimesCalCRC16(ENCRY_KEY *pKey,unsigned char *pData,unsigned long DataLen)
{
	ENCRY_KEY	Key;
	unsigned long	tl;
	
	memmove((uchar*)&Key,(uchar*)pKey,16);
	
	///////////////////////////////////////////////////////// 1次加密
	Encrypt((ENCRY_KEY*)&Key,pData,DataLen);
	
	///////////////////////////////////////////////////////// 2次加密
	tl = Key.Key;
	Key.Key	= Key.A;
	Key.A		= Key.B;
	Key.B		= Key.C;
	Key.C		= tl;
	Encrypt((ENCRY_KEY*)&Key,pData,DataLen);
	
	///////////////////////////////////////////////////////// 3次加密
	tl = Key.Key;
	Key.Key	= Key.A;
	Key.A		= Key.B;
	Key.B		= Key.C;
	Key.C		= tl;
	Encrypt((ENCRY_KEY*)&Key,pData,DataLen);
	
	return CalCRC16(pData,DataLen);

}





__task		void 		MainTask(void);

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：Main
   功能描述: 主函数
   入口参数：无
   出口参数：无
   全局变量: 
   创建时间：
   修改时间：	
*/ 	
extern	"C"	void SystemInit (void);
unsigned short		UniCode2CN(unsigned short   uCode);
extern	"C"	unsigned short 		CalCRC16( unsigned char * ptr, unsigned short len) ;
extern	"C"	unsigned char 	GetDeviceSerialNumber(unsigned char *pData);
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
int			main (void) 
{
	uint32	 i,Sum;
	uint16	 Crc16_1,Crc16_2,Crc16_3;
  	uint32	 volatile start;
	union
	{
		uchar		Buffer[40];
		uint16	B16[20];
		uint32	B32[10];
	}Tp;
	ENCRY_KEY	Key;
	unsigned char	*pData=NULL;


	NOP();	NOP();
	//Tp.B16[0] = UniCode2CN( 0x6CAA );

	///////////////////////////////////////////////////////////
	RFID_POWER_OFF;
	//#endif
	SSP0.Init();
	BUZZER_OFF;	
		

	MOBILE_POWER_ON;


	STEP_MOTOR_PIN_OUT;
	STEP_MOTOR_STOP;

	/////////////////////////////////////////////////////////////////////////////////////
	//unsigned char 	GetDeviceSerialNumber(unsigned char *pData)
	SetBufferValue( Tp.Buffer,40,0);
	memmove((uchar*)Tp.Buffer,(uchar*)ConstPID, 32);	
	Crc16_1 = CalCRC16(Tp.Buffer,32);
	
	SetBufferValue( Tp.Buffer,40,0);
	start = GetDeviceSerialNumber( Tp.Buffer);

	
	if(start==0)
	{
		for(Sum=0,i=0;i<16;i++)
		{
			Sum += Tp.Buffer[i];
		}
		Crc16_2 = CalCRC16(Tp.Buffer,32);
		Sum <<= 8;
		Key.Key = Tp.B32[0]|Sum;
		Key.A	 = Tp.B32[1]|Sum;
		Key.B	 = Tp.B32[2]|Sum;
		Key.C	 = Tp.B32[3]|Sum;

		pData = NULL;
		while(pData==NULL)	{	pData=MyMalloc(512);	}

		memmove( pData, (uchar*)ConstPARA, 286);

		Crc16_3 = Encrypt3TimesCalCRC16( (ENCRY_KEY*)&Key, pData, 286); 

		memmove( Tp.Buffer,(uchar*)ConstCRC1,4);	
		memmove( &Tp.Buffer[4],(uchar*)ConstCRC3,2);	
		gKey.A = Crc16_1 ^ Tp.B16[0];
		gKey.B = Crc16_2 ^ Tp.B16[1];
		gKey.C = Crc16_3 ^ Tp.B16[2];
		gKey.Key =  ( (gKey.A ^ gKey.B )<<16 )	+ gKey.C;
	}
	else
	{
		gKey.Key = 0x55aa7ffe;
		gKey.A 	= 0xaa55fe7f;
		gKey.B 	= 0x5aa5f7fe;
		gKey.C 	= 0xa55aeffe;
	}

	MyFree(pData);



	/////////////////////////////////////////////////////////////////////////////////////
	NOP();	
	NOP();
	osInitFlag = 0;
	SystemInit();		// 初始化时钟源、PLL等
	NOP();	
	NOP();
	
	Sys.Delay_mS(10);

	Sys.InitWatchDog(24000000);		// 3秒
	
	/* Reset all GPIO pins to default: primary function */
	RESET_WATCH_DOG;

	//POWER_SELF_LOCK_ON;
	PowerSelfLock = TRUE;
	REGAIN_OIL_CONTROL;	  	// 启动后不自动锁车
	VehicleControl.Lock = LOCK_VEHICLE_DISABLE;

	RESET_WATCH_DOG;

	NOP();	NOP();	
	Sys.IdleMode();
	NOP();	NOP();	
  	//Sys.TestFunction();
   //RFID_POWER_ON;
   NOP();	NOP();	
   NOP();	NOP();
   RESET_WATCH_DOG;
   BUZZER_ON;

  	os_sys_init (MainTask);  
	//os_sys_init_user (MainTask, 1, MTstk, LEN_MTstk);  
}




















/*********************************************************************************************************
  End Of File
*********************************************************************************************************/
