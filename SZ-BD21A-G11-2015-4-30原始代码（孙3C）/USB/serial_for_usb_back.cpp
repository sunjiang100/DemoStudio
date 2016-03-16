

//#include <LPC17xx.H>                              /* LPC17xx definitions    */
//#include "Serial.h"



#include 		"serial_for_usb.h"
#include		"..\UART\UART0.h"


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
