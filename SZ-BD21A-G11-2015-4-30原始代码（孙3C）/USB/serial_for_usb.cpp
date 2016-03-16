

//#include <LPC17xx.H>                              /* LPC17xx definitions    */
//#include "Serial.h"



#include 		"serial_for_usb.h"
#include		   "..\UART\UART0.h"
#include			"..\Common\GB_T19056.h"
#include       "..\flash\DFE_25xxx.h"
#include			"..\RTC\RTC.h"
#include       "MSD_SD_File.h"
/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
int SER_putChar (int uart, int c) {
			
  	Uart.puts((char *)&c);
	

	return 	0;


//LPC_UART_TypeDef *pUart;
//
//  pUart = (LPC_UART_TypeDef *)LPC_UART0 ;
//  while (!(pUart->LSR & 0x20));
//  return (pUart->THR = c);
}


/*----------------------------------------------------------------------------
  Read character from Serial Port   (blocking read)
 *----------------------------------------------------------------------------*/
int SER_getString (unsigned char *str) {
  	

	if (Uart.GetRecBuffer(str))
	{
		return sizeof(str);
	}

	return 0;

//LPC_UART_TypeDef *pUart;
//
//  pUart = (uart == 0) ? (LPC_UART_TypeDef *)LPC_UART0 : (LPC_UART_TypeDef *)LPC_UART1;
//  while (!(pUart->LSR & 0x01));
//  return (pUart->RBR);

}


/*----------------------------------------------------------------------------
  Write character to Serial Port
 *----------------------------------------------------------------------------*/
void SER_putString (char *s) {

//  while (*s != 0) {
//   SER_putChar(0, *s++);
//  }

	Uart.puts(s);
}


/*----------------------------------------------------------------------------
  Write block to Serial Port
 *----------------------------------------------------------------------------*/
void SER_putb (unsigned short Len,unsigned char *p) {
	
	Uart.putb(0, Len, p);

}
/*----------------------------------------------------------------------------
  Get a name for file
 *----------------------------------------------------------------------------*/
//unsigned  char  const   fFirstSetName[] = {"DXXXXXX_XXXX_未知车牌.VDR\0\0\0\0"};
void   Get_Fname(uchar *p)
{
	 /*unsigned  char  Buf[20];

	 strncpy((char *)p, (const char *)fFirstSetName, 39);		
	 //Rtc.GetRTC_BCD((RTC_STR *)Buf);
	 Rtc.GetNowDateTimeString(0, &Rtc.DT, Buf);
	 strncpy((char *)(p+1), (char *)&Buf[2], 6);				// XXXXXX
	 Rtc.GetNowTimeString(0, &Rtc.DT, Buf);
	 //strncpy(p+8, (char *)&Buf[0], 6);
	 strncpy((char *)(p+8), (char *)&Buf[0], 4);
	 DFE.Read(FM_VEHICLE_CODE, LEN_VEHICLE_CODE,Buf);
	 //memmove(p+13, Buf, 8);		  // 待乱码没有解决，先不用这句,暂用下面的

	 Buf[0] = 'X';
	 Buf[1] = 'X';
	 memmove(p+13, Buf, 8);	 */


	 unsigned  char  Buf[20];
	 uchar Len = 0;
	 
	 p[Len++] = 'D';
	 //Rtc.GetRTC_BCD((RTC_STR *)Buf);
	 Rtc.GetNowDateTimeString(0, &Rtc.DT, Buf);
	 strncpy((char *)(p+Len), (char *)&Buf[2], 6);				// XXXXXX
	 Len += 6; 
	 p[Len++] = '_';
	 Rtc.GetNowTimeString(0, &Rtc.DT, Buf);
	 strncpy((char *)(p+Len), (char *)&Buf[0], 4);
	 Len += 4;
	 p[Len++] = '_';
	 DFE.Read(FM_VEHICLE_CODE, LEN_VEHICLE_CODE,Buf);

	 // 下面为了在U盘中改成支持中文名，所以 未知车牌 使用 "NNOO"代表，单个汉字使用 “X"表示，在生成U盘文件是一个字母变成unicode编码
	 //  就变成2个字节，然后使用USB写扇区的函数，把正确的汉字进去，实现U盘有中文名
	 if (Buf[0] == 0) // 车牌号为空   未知车牌
	 {
		 p[Len++] = 'U';
		 p[Len++] = 'N';
		 p[Len++] = 'K';
		 p[Len++] = 'W';

	 }
	 else			  // 设置过车牌
	 {
	 	p[Len++] = 'X';			 // 最后存在U盘的文件名编码是unicode码，遇到字符，会在字符后插一个00
		memmove(p+Len, Buf+2, 6); // 跳过GB汉字
		Len += 6;
	 }
	 memmove(p+Len, ".VDR", 4);	 
	 Len += 4;
	 p[Len++] = 0;




	 //Uart.puts(p);

	//Uart.puts("usb file name Test Data:"); Uart.putb(1,strlen((char *)p),(uchar *)p); Uart.puts("\r\n");

}


/*----------------------------------------------------------------------------
  获取车牌号中的汉字对应的Unicode码
 *----------------------------------------------------------------------------*/
extern  unsigned short		CN2UniCode(unsigned char   *pGBCode);

void Get_Vehicle_Code_GB(unsigned char *pUnicode)
{
	uchar Buf[8];
	uint16 uc;

	DFE.Read(FM_VEHICLE_CODE,2,Buf);		 // 仅读出汉字


	uc = CN2UniCode(Buf);


	pUnicode[0] = uc & 0xff;
	pUnicode[1]	= (uc >> 8) & 0xff;


}


