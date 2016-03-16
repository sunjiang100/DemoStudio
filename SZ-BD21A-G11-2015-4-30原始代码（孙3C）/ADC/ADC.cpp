/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												广州国业电子科技有限公司
$									Shawyoo Electronics (GuangZhou) R&D Center
$                      	http://www.shawyoo.com		 Email: autoele@qq.com
$						 QQ群：8502321/9069471/9069433/9524253		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名:  
	功能描述：
	编译环境：
	目标CPU:  
	作者：    
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  
#include    "..\Config\UserConfig.h"
#include		"..\Common\UserGlobals.h"

#include    "..\UserSys.h"

#include    "..\UART\UART0.h"

#include		<string.h>
#include			"..\common\Algol.H"

#define		ADC_GLOBALS			1
#include		"ADC.h"

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   函数名: TaskSample
   功能：  ADC处理程序.ADC缓冲处理，计算每通道的累加和及平均值
   入口：  无
   出口：  无
*/
void       ADCCls::eTaskADC(void)			// 10ms调用1次
{  
    char		PutAdc[228];
    uchar 	tKeyCode,j;
	uint16  tempdata;
    union
	{
		uint32	B32;
		uint16	B16[2];
	} AR;
    uchar    i,ch,Len; 	
    uint32     uiAdc;	
    //unsigned long ulTemp;
    NOP();   NOP();   
    TickCnt++;
    AR.B32 = Get_ADC();         // ADC conversion

	     
    uiAdc = AR.B16[0]&0x0fff;
    ch = AR.B16[1]&0x00ff;

	// test 
//	if ((TickCnt % 50 == 0) && (ch == 5))
//	{
//		SetBufferValue((uchar *)PutAdc,128,0);
//		strcpy(PutAdc,"adc: ");
//		i = strlen(PutAdc);
//		Int2Str(uiAdc,PutAdc+i);
//		strcat(PutAdc,"    ch: ");
//		i = strlen(PutAdc);
//		Int2Str(ch,PutAdc+i);
//		strcat(PutAdc,"\r\n");
//		Uart.puts(PutAdc);
//	}

	//
    
   if(ch<=MAX_CHANNEL)
	{
	  
	   if(ch>=MAX_CHANNEL)
		{
			AdcR.Counter++;	
			if(AdcR.Counter>=SAMPLE_TIMES)
			{
				AdcR.Counter = 0;
			}
		}  	
		if( (ch&0x01)==0 )				// 偶数通道号，实为通道0
    	  ch = 0;
      else
    	  ch = (ch+1)>>1;
		//if(uiAdc!=0x0fff)
		if(ch != 3)
		{
			if(AdcR.Sum[ch] >= AdcR.Buffer[ch][AdcR.Counter])
			AdcR.Sum[ch] -= AdcR.Buffer[ch][AdcR.Counter];
			AdcR.Buffer[ch][AdcR.Counter] = uiAdc;
			AdcR.Sum[ch] += uiAdc; //AdcR.Buffer[ch][AdcR.Counter];
		   AdcR.Avg[ch] = AdcR.Sum[ch]/SAMPLE_TIMES; 
		}
      if(ch == 3)
		{
		  
		 AdcR.Buffer[3][sunchannel]=uiAdc;
		  //AdcR.Avg[4]= uiAdc;		  
		  sunchannel++;
		  if(sunchannel == 10)
			{
				sunchannel = 0;
				for(i=0;i<10;i++)
				{
				 for(j=i;j<10;j++)
				  {
				   if(AdcR.Buffer[3][i]>AdcR.Buffer[3][j])
				   {
				    tempdata=AdcR.Buffer[3][i];
					AdcR.Buffer[3][i]=AdcR.Buffer[3][j];
					AdcR.Buffer[3][j]=tempdata;
				   }
				  }
				}  
				 				
				for(i=2;i<8;i++)
				{
				 AdcR.Sum[3]+=AdcR.Buffer[3][i];
				}
			  AdcR.Avg[3] = AdcR.Sum[3]/6;
			  //AdcR.Avg[3] = uiAdc;
			  AdcR.Sum[3]=0;
			  //
			
			   
			 }//
		}

   	switch(ch)
   	{  
   	case CHANNEL_0:
		
		  	//Uart.puts("uiAdc Test Data:"); Uart.putb(1,2, (uchar *)&uiAdc);Uart.puts("\r\n");
			if(TD.ADC==tADC_KEY_BOARD)	
			{
				Uart.puts("KEY ADC: ");
				Int2Str(uiAdc,PutAdc);
				Uart.puts(PutAdc);
				Uart.puts("\r\n");
			}
		 	// 键盘通道，不能滤波，否则按键按下或弹起时，平均值有缓慢变化过程，导致误判为多按键		  
			if(uiAdc>(KEY1_AD+KEY_AD_RANGE))																// 无按键  3.19V  4083-4089
		 	{
		 		tKeyCode = 0xff;
		 	}
			if((uiAdc>(KEY1_AD-KEY_AD_RANGE))&&(uiAdc<(KEY1_AD+KEY_AD_RANGE)))		  			// Key1  3.14V    4017-4025
	  		{	  			
     		 	tKeyCode = 4;     		 	
	  		}
   		    else if((uiAdc>(KEY2_AD-KEY_AD_RANGE))&&(uiAdc<(KEY2_AD+KEY_AD_RANGE)))			// Key2	3.06V	 3913-3918
     		{
     		 	tKeyCode = 3;
	  		}
	 		else if((uiAdc>(KEY3_AD-KEY_AD_RANGE))&&(uiAdc<(KEY3_AD+KEY_AD_RANGE)))			// Key3	2.84V	 3635-3642
    		{
    		  	tKeyCode = 2;
	 		}

	 		else if((uiAdc>(KEY4_AD-KEY_AD_RANGE))&&(uiAdc<(KEY4_AD+KEY_AD_RANGE)))			// Key4	2.25V	 2872-2874	
	 		{
    		 	tKeyCode = 1;
	 		}		
	 		else if(uiAdc<KEY_AD_RANGE+10)	                											// Key5	0V	   12-16		由于硬件差异，所以这里加10
    		{                                                                                    	
    		  	tKeyCode = 0;    		  	
	 		}								
	 		else
	 		{
	 			tKeyCode = 0xff;	 			
    		}
			

					              	
    		///////////////////////////////////////////////////////////////////////////    			
    		Key.Fifo[0] = Key.Fifo[1];		Key.Fifo[1] = Key.Fifo[2];		Key.Fifo[2] = tKeyCode;
    		Key.KeyDown = Key.KeyPress = Key.KeyUp = Key.KeyFree = 0;
    		if( (Key.Fifo[0]==0xff)&&(Key.Fifo[1]==Key.Fifo[2])&&(Key.Fifo[2]<=4) )	// 按下
    			Key.KeyDown = TRUE;
    		else if( (Key.Fifo[0]==Key.Fifo[1])&&(Key.Fifo[1]==Key.Fifo[2])&&(Key.Fifo[2]<=4) )	// 按住
    			Key.KeyPress = TRUE;
    		else if( (Key.Fifo[0]<=4)&&(Key.Fifo[0]==Key.Fifo[1])&&(Key.Fifo[2]==0xff) )		// 弹出
    			Key.KeyUp = TRUE;
    		else
    			Key.KeyFree = TRUE;
    		
    		
    		if(Key.Fifo[1]==Key.Fifo[2])
    			Key.KeyCode = Key.Fifo[1];
    		else	
    			Key.KeyCode = 0xff;
    			
    		if(TD.ADC==tADC_KEY_BOARD)	
    		{
    			if(Key.KeyDown == TRUE)				Uart.puts("Key down. ");
    			else if(Key.KeyPress == TRUE)		Uart.puts("Key press. ");
    			else if(Key.KeyUp == TRUE)			Uart.puts("Key up. ");
    			if( Key.KeyCode <= 4)
    			{
    				strcpy(PutAdc,"Code  \r\n\0");
    				PutAdc[5] = Key.KeyCode+'0';
    				Uart.puts(PutAdc);
    			}
    		}    		    		
   	   uiAdc *= MV_PER_DOT; 
			uiAdc /= 100;
   		AdcR.Vol[0] = uiAdc; 
		   		    		
		break;
//	case CHANNEL_1:   	          	 //打印机状态
//		uiAdc = AdcR.Avg[1];
//		uiAdc *= MV_PER_DOT; 
//		uiAdc /= 100;
//   		AdcR.Vol[1] = uiAdc;   	    
//   			   	
//		break;
   case CHANNEL_1:	         		  //打印机缺纸检测
   	uiAdc = AdcR.Avg[1];
		uiAdc *= MV_PER_DOT; 
		uiAdc /= 100;
   	AdcR.Vol[1] = uiAdc; 
   	         
   	break;   
	case  CHANNEL_2:							 //主电源检测
		uiAdc = AdcR.Avg[2];
		uiAdc *= MV_PER_DOT;
		uiAdc *= 87;   		// 8.7 放大10，所以下面要多除10
		uiAdc /= 1000;
		uiAdc += 900;      // 二极管及其他修正值
		AdcR.Vol[2] = uiAdc; 

//		SetBufferValue((uchar *)PutAdc,128,0);
//		strcpy(PutAdc,"MapinPower: ");
//		i = strlen(PutAdc);
//		Int2Str(uiAdc,PutAdc+i);
//		strcat(PutAdc,"\r\n");
//		Uart.puts(PutAdc);

		MainPowerValue = uiAdc/100;
		SystemLog.VDR.Power1_2	|=  ((MainPowerValue  * 100)& 0xffff);   // 主电源占低两个字节	    			
							
	break;
	case  CHANNEL_3:
	/*
		uiAdc = AdcR.Avg[3];
		uiAdc *= MV_PER_DOT;
		uiAdc *= 87;   
		uiAdc /= 1000;
		AdcR.Vol[3] = uiAdc;     			
	*/
	AdcR.Vol[3]=AdcR.Avg[3];						
	break;

	case  CHANNEL_4:
		uiAdc = AdcR.Avg[4];
		uiAdc *= MV_PER_DOT;
		uiAdc *= 87;   
		uiAdc /= 1000;
		AdcR.Vol[4] = uiAdc;     			
							
	break;

		default:		break;
   	}
   }
   /////////////////////////////////////////////   
	
	if( (TD.TestMode==TEST_MODE_AUTO)&&(TickCnt%(3*10)==0) )
  	{
  		SetBufferValue((uchar*)PutAdc,128,0);
  		strcpy(PutAdc,"GY-TD-AD");
		memmove(&PutAdc[8],(uchar*)&AdcR.Vol,26);
		PutAdc[34] = '\r';
		PutAdc[35] = '\n';
		Uart.putb(1,36,(uchar*)PutAdc);		
  	}
   else if(((TD.TestMode==TEST_MODE)||(TD.ADC==tADC_ALL))&&(TickCnt%300==0))
	//if((TD.ADC==tADC_HEX)&&(TickCnt%100==0))
	{
		Uart.puts("\r\n\r\n/*&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& ADC Test  AIN0~ AIN12 &&&&&&&&&&&&&&&&&&&&&&&&&&&&&&*/\r\n\0");
		SetBufferValue((uchar*)PutAdc,128,0);	
		Len = 0;			
		for(i=0;i<TOTAL_CHANNEL/2+1;i++)
		{
			Int16ToDecChar(AdcR.Vol[i],&PutAdc[Len]);
			strcat(PutAdc," , ");
			Len = strlen(PutAdc);
		}
		strcat(PutAdc,"\r\n\0");
		Uart.puts(PutAdc);
		os_dly_wait(10);		
	}
	if((TD.ADC==tADC_HEX)&&(TickCnt%50==0))
	{
	  SetBufferValue((uchar*)PutAdc,228,0);	
	  Len = 0;		
	  for(ch = 4; ch<7; ch++)
	  {
	  for(i=0; i<5; i++)
	  {
		  Int16ToDecChar(AdcR.Buffer[ch][i],&PutAdc[Len]);
		  	strcat(PutAdc," , ");
			Len = strlen(PutAdc);
	  }
	  Int16ToDecChar(AdcR.Sum[ch],&PutAdc[Len]);
	  strcat(PutAdc," , ");
	  Len = strlen(PutAdc);
	  Int16ToDecChar(AdcR.Avg[ch],&PutAdc[Len]);
	  strcat(PutAdc,":");
	  Len = strlen(PutAdc);
	  }
	  strcat(PutAdc," \r\n\0");
	  Uart.puts(PutAdc);
	  os_dly_wait(1);		

		//Uart.putb(1,22,(uchar*)&AdcR.Avg);
		//Uart.putb(1, 20, (uchar *)&AdcR.Buffer[4][0]);
		//Uart.puts("\r\n\0");
	}
}


uint32			ADCCls::GetKeyCode(void)
{
	union
	{
		uint32	B32;
		uchar 	Buffer[4];
	} Tp;
	Tp.Buffer[0] = Key.KeyDown;
	Tp.Buffer[1] = Key.KeyPress;
	Tp.Buffer[2] = Key.KeyUp;
	Tp.Buffer[3] = Key.KeyCode;
	
	return Tp.B32;	
}	



void			ADCCls::GetAllResult(uint16	*pAdc)
{
	uint32	i;	
	for(i=0;i<TOTAL_CHANNEL/2+1;i++)
	{
		*(pAdc+i) = AdcR.Vol[i];					//
	}	
}




const	 unsigned char	ValidChannel[9] = {3,2,3,1,3,4,3,5,3};

uint32      ADCCls::Get_ADC(void)
{
	uchar		tCh;
	union
	{
		uint32	B32;
		uint16	B16[2];
	} AR;
	uint32	tl; 
	uint16   uiAdcR;

	NOP();   NOP();

   if(ChannelValid==1)
	{	
		NOP();	NOP();
	  	//LPC_ADC->ADCR &= 0xFFFFFF00;
		//LPC_ADC->ADCR |= (1<<(ValidChannel[ucChannel]))|(1UL << 24);    // 切换通道并进行转换		
		//Sys.Delay_uS(200);
		tl = LPC_ADC->ADGDR;			// 读取ADC值

		ChannelValid = 0;

		//Uart.puts("ADGDR Test Data:"); Uart.putb(1,4,(uchar *)&tl); Uart.puts("\r\n");

		if((((tl>>31)&0x01)==0) || ((tl&(ValidChannel[ucChannel]<<24))==0))      
		{	
		  AR.B32 = 0xffffffff;
		  return AR.B32;
		}					 		 
	
					
		uiAdcR = (tl>>4);						// 
		uiAdcR &= 0x0fff;
		AR.B16[0] = uiAdcR;					//	AR.B16[0] = 0x98;
		AR.B16[1] = ucChannel;		
		
		ucChannel++;       // point to next channel
		if(ucChannel>MAX_CHANNEL)
		   ucChannel=MIN_CHANNEL;
		
				  				
//		LPC_PINCON->PINSEL1 |= ((1UL<<16)|(1UL<<18)|(1UL<<20));	// 选择ADC输入引脚P0.23~26
//		LPC_PINCON->PINSEL3 |= ((3UL<<28) |(3UL<<30));								 // 选择 P1.30, P1.31
//		LPC_PINCON->PINMODE1 |= ((2UL<<16)|(2UL<<18)|(2UL<<20));
//		LPC_PINCON->PINMODE3 |= ((2UL<<28) | ((2UL<<30)));	
		
	}	
	else if (JT_Ctr_PowerDown == 0) //if(ChannelValid==0)  
	{		          
		LPC_ADC->ADCR &= 0xFFFFFF00;
		LPC_ADC->ADCR |= (1<<(ValidChannel[ucChannel]))|(1UL << 24);    // 切换通道并进行转换		
		ChannelValid = 1;
		AR.B32 =  0xffffffff;
	}
	else
	{
		LPC_ADC->ADCR &= ~(1UL << 24);	
		AR.B16[1] = 0xff;
	}	
	return AR.B32;	
}




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   函数名: ADC_BufferInit
   功能：  ADC缓冲区初始化程序
   入口：  无
   出口：  无
*/
//ADC initialisation
void   ADCCls::Init(void)
{  
	
   uint32	Fclk;
   ChannelValid = 0; 
	LPC_PINCON->PINSEL1 |= ((1UL<<16)|(1UL<<18)|(1UL<<20));	// 选择ADC输入引脚P0.24.25.26
	LPC_PINCON->PINSEL3 |= ((3UL<<28) | (3UL<<30));								// P1.30	 P1.31

	LPC_PINCON->PINMODE1 |= ((2UL<<16)|(2UL<<18)|(2UL<<20));
	LPC_PINCON->PINMODE3 |= ((2UL<<28)|(2UL<<30));

	
	Fclk = LPC_SC->PCLKSEL0;
	Fclk &= (0x0000003<<24);					// B.24.25
	Fclk >>= 24;
	Fclk = PeriClock[Fclk];
	LPC_ADC->RESERVED0 = 0x00000000;
	LPC_ADC->ADCR =(2 << 0)             | 				// SEL = 1 ，选择通道1
          ((Fclk/250000-1) << 8) 		| 				// CLKDIV = Fpclk / 1000000 - 1 ，即转换时钟为MHz
          (0 << 16)                    | 				// BURST = 0 ，软件控制转换操作
          (0 << 17)                    | 				// CLKS = 0 ，使用11clock转换
          (1 << 21)                    | 				// PDN = 1 ， 正常工作模式(非掉电转换模式)
          (0 << 22)                    | 				// TEST1:0 = 00 ，正常工作模式(非测试模式)
          (1 << 24)                    | 				// START = 1 ，直接启动ADC转换
          (0 << 27);             		  				// EDGE = 0 (CAP/MAT引脚下降沿触发ADC转换)              
	os_dly_wait(1);
   Fclk = LPC_ADC->ADGDR;         				      		// 读取ADC结果,清标志

	if(Fclk)	NOP();
	  
	ucChannel = 0;	 	 	 
   AdcR.Counter=0; 


} // End of <ADC_BufferInit> function




ADCCls::ADCCls(void)
{
}




/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */

