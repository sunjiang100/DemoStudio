

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
unsigned  char  const   fFirstSetName[] = {"DXXXXXX_XXXXXX_未知分类_未知车牌.VDR"};
void   Get_Fname(char *p)
{
	 unsigned  char  Buf[20];
	 strncpy(p, (const char *)fFirstSetName, 36);
	 //Rtc.GetRTC_BCD((RTC_STR *)Buf);
	 Rtc.GetNowDateTimeString(0, &Rtc.DT, Buf);
	 strncpy(p+1, (char *)&Buf[2], 6);
	 Rtc.GetNowTimeString(0, &Rtc.DT, Buf);
	 strncpy(p+8, (char *)&Buf[0], 6);
	 DFE.Read(FM_VEHICLE_CLASS, LEN_VEHICLE_CLASS,Buf);

	 memmove(p+15, Buf, 8);

	 DFE.Read(FM_VEHICLE_CODE, LEN_VEHICLE_CODE, Buf);

	 memmove(p+24, Buf, 8);
}
/*
extern		"C"   void     WrintDataToFile(void);

void     WrintDataToFile(void)
{

	unsigned  char  *buffer, ret, t, CheckSum;
	
	ret = 0; 
	//USBUseFlag = 1;
	//CheckSum = 0;
	//write_file_error_flag = 0;
  //while (USBUseFlag == 1)
  //{
  	  	
	  //USBUseFlag = 1;
	    // if (LenDataForUSB)
		  //{
		  	  #if(USB_DEBUG)
				SER_putString ("\n USB writing!!!\r\n\0");
				os_dly_wait(10);
				#endif       
			  buffer = 	&TxdTBuffer[0];
			 // 加校验
			  //for(t=0; t<LenDataForUSB; t++)	{ CheckSum ^= *(buffer+t);	}
				  
			  //while((ret==0)&&(write_file_error_flag==0)) 
			 // {	
			      if(USB_CONNECT & 0x01)
				     ret = fwrite(buffer, LenDataForUSB ,1 , gfile);	  //   写文件
					else 
					  ret = 0;
					//fflush(gfile);
					if (ferror(gfile)||(ret==0))								//(ferror(gfile)||
					{
						 //设置出错标志
						 //fclose (gfile);
						 //usbh_uninit ();
						 //funinit ("U0:");
		             //os_dly_wait(50); 
		             //finit ("U0:");  
						 write_file_error_flag = 1;
						 //wfile_flag = 0;
						 LenDataForUSB = 0;
						 #if(USB_DEBUG)
						 SER_putString ("\n USB write ERROR!!!\r\n\0");
						 os_dly_wait(2);
						 #endif       
				       return;
					}
					//os_dly_wait(1);
			 //} 
			   
			  if(TD.Display==tDISP_USB_RECODE)
			  {
			       SER_putb(LenDataForUSB, buffer);
					 os_dly_wait(LenDataForUSB/15);
			  }
			  LenDataForUSB = 0;
			//}
			//else
			 // os_dly_wait(1);							 
	  //}				  
	  // 写入检验数据
	  
	  //fwrite((char *)&CheckSum, 1, 1, gfile);
	  if(USBUseFlag==0)
	  {
		  if(fclose (gfile)!=0)
		  {
		  	 #if(USB_DEBUG)
			 SER_putString ("\n File Close ERROR!!!\r\n\0");
			 //funinit ("U0:");
			 os_dly_wait(2); 
			 #endif
			 //finit ("U0:"); 
			 //wfile_flag = 0; 
		  
		  }
		  else
		  {
			  gfile = NULL;
			  #if(USB_DEBUG)
			  SER_putString ("\nAuto write file finished!!!\r\n\0");                      // close the output file               
			  os_dly_wait(1);
			  #endif
			  //funinit("U0:");
			  //SER_putString("\nFile closed.\r\n\0");
		  }
	  }
}		*/
