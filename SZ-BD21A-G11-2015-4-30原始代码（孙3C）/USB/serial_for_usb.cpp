

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
//unsigned  char  const   fFirstSetName[] = {"DXXXXXX_XXXX_δ֪����.VDR\0\0\0\0"};
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
	 //memmove(p+13, Buf, 8);		  // ������û�н�����Ȳ������,���������

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

	 // ����Ϊ����U���иĳ�֧�������������� δ֪���� ʹ�� "NNOO"������������ʹ�� ��X"��ʾ��������U���ļ���һ����ĸ���unicode����
	 //  �ͱ��2���ֽڣ�Ȼ��ʹ��USBд�����ĺ���������ȷ�ĺ��ֽ�ȥ��ʵ��U����������
	 if (Buf[0] == 0) // ���ƺ�Ϊ��   δ֪����
	 {
		 p[Len++] = 'U';
		 p[Len++] = 'N';
		 p[Len++] = 'K';
		 p[Len++] = 'W';

	 }
	 else			  // ���ù�����
	 {
	 	p[Len++] = 'X';			 // ������U�̵��ļ���������unicode�룬�����ַ��������ַ����һ��00
		memmove(p+Len, Buf+2, 6); // ����GB����
		Len += 6;
	 }
	 memmove(p+Len, ".VDR", 4);	 
	 Len += 4;
	 p[Len++] = 0;




	 //Uart.puts(p);

	//Uart.puts("usb file name Test Data:"); Uart.putb(1,strlen((char *)p),(uchar *)p); Uart.puts("\r\n");

}


/*----------------------------------------------------------------------------
  ��ȡ���ƺ��еĺ��ֶ�Ӧ��Unicode��
 *----------------------------------------------------------------------------*/
extern  unsigned short		CN2UniCode(unsigned char   *pGBCode);

void Get_Vehicle_Code_GB(unsigned char *pUnicode)
{
	uchar Buf[8];
	uint16 uc;

	DFE.Read(FM_VEHICLE_CODE,2,Buf);		 // ����������


	uc = CN2UniCode(Buf);


	pUnicode[0] = uc & 0xff;
	pUnicode[1]	= (uc >> 8) & 0xff;


}


