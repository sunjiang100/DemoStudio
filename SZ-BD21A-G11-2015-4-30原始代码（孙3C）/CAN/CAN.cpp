/* ----------------------------------------------------------------------------
 * Name:    Can.c
 * Purpose: CAN interface for for LPC17xx with MCB1700
 * Note(s): see also http://www.port.de/engl/canprod/sv_req_form.html
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *---------------------------------------------------------------------------- */

#include 	<LPC17xx.H>                             ///  LPC17xx definitions 
#include 	"CAN.h"                                 ///  LPC17xx CAN adaption layer 

#include		"..\Common\UserGlobals.h"
#include		"..\Config\RTOS.h"
#include		"..\Config\UserConfig.h"
#include		"..\common\Algol.h"

#include		"..\UART\Uart0.h"
#include		"..\Speed\Pulse.h"


//extern		 unsigned int  	PeriClock[];

/*  Values of bit time register for different baudrates
   NT = Nominal bit time = TSEG1 + TSEG2 + 3
   SP = Sample point     = ((TSEG2 +1) / (TSEG1 + TSEG2 + 3)) * 100%
                                        		     SAM,  SJW, TSEG1, TSEG2, NT,  SP 	*/
const uint32_t CAN_BIT_TIME[] = {          0, ///              not used             
                                           0, ///              not used             
                                           0, ///              not used             
                                           0, ///              not used             
                                  0x0001C000, ///  0+1,  3+1,   1+1,   0+1,  4, 75% 
                                           0, ///              not used             
                                  0x0012C000, ///  0+1,  3+1,   2+1,   1+1,  6, 67% 
                                           0, ///              not used             
                                  0x0023C000, ///  0+1,  3+1,   3+1,   2+1,  8, 63% 
                                           0, ///              not used             
                                  0x0025C000, ///  0+1,  3+1,   5+1,   2+1, 10, 70% 
                                           0, ///              not used             
                                  0x0036C000, ///  0+1,  3+1,   6+1,   3+1, 12, 67% 
                                           0, ///              not used             
                                           0, ///              not used             
                                  0x0048C000, ///  0+1,  3+1,   8+1,   4+1, 15, 67% 
                                  0x0049C000, ///  0+1,  3+1,   9+1,   4+1, 16, 69% 
                                };

/* ----------------------------------------------------------------------------
  configure the requested baudrate.  CAN controller (1..2)
 *----------------------------------------------------------------------------*/
void 		CanBusCls::CAN_cfgBaudrate (uint32_t ctrl, uint32_t baudrate)  
{
  U32	Fclk;
  LPC_CAN_TypeDef 	*pCAN;
  U32 result = 0;
  U32 nominal_time;

  pCAN = (ctrl == 1) ? LPC_CAN1 : LPC_CAN2;  /// CAN ctrl    

   Fclk = LPC_SC->PCLKSEL0;
	Fclk &= (3UL<<26);					// B.26.27
	Fclk >>= 26;
	Fclk = PeriClock[Fclk];

  /// Determine which nominal time to use for PCLK and baudrate            
  if (((Fcclk / 1000000) % 6) == 0) 
  {
    nominal_time = 12;
  }  
  else
  {
    nominal_time = 10;
  }

  /// Prepare value appropriate for bit time register                        
  result  = (Fclk / nominal_time) / baudrate - 1;
  result &= 0x000003FF;
  result |= CAN_BIT_TIME[nominal_time];

  pCAN->BTR  = result;                           ///Set bit timing

  
}


/* ----------------------------------------------------------------------------
  setup CAN interface.  CAN controller (1..2)
 *----------------------------------------------------------------------------*/
void 			CanBusCls::CAN_setup (uint32_t ctrl,uint32_t  Bps)  
{
  LPC_CAN_TypeDef *pCAN = (ctrl == 1) ? LPC_CAN1 : LPC_CAN2;

  if (ctrl == 1) 
  {
    LPC_SC->PCONP   |= 1UL << 13;           		// Enable clock for CAN1        
    LPC_PINCON->PINSEL1 |= (0x0fUL<<10);        	// Set CAN1 bits to b0101   
	
	 NVIC_SetPriority(CAN_IRQn, CAN_INT_PRIORITY); 
    NVIC_EnableIRQ(CAN_IRQn);                    ///  Enable CAN interrupt 
  } 
  else 
  {
    LPC_SC->PCONP       |=  (1 << 14);           ///  Enable power to CAN2 block 
    LPC_PINCON->PINSEL4 |=  (1 << 14);           ///  Pin P2.7 used as RD2 (CAN2) 
    LPC_PINCON->PINSEL4 |=  (1 << 16);           ///  Pin P2.8 used as TD2 (CAN2) 
    NVIC_SetPriority(CAN_IRQn, CAN_INT_PRIORITY); 
    NVIC_EnableIRQ(CAN_IRQn);                    ///  Enable CAN interrupt 
  }

  pCAN->MOD   = 1;                               ///  Enter reset mode 
  LPC_CANAF->AFMR = 2;                           ///  By default filter is not used   
  pCAN->CMR = (1 << 2); 
  pCAN->IER   = 0;                               ///  Disable all interrupts 
  pCAN->GSR   = 0;                               ///  Clear status register 
  CAN_cfgBaudrate(ctrl, Bps);                 ///  Set bit timing 
  pCAN->IER   = 0x0003;                          ///  Enable Tx and Rx interrupt 
  
}


/* ----------------------------------------------------------------------------
  leave initialisation mode.  CAN controller (1..2)
 *----------------------------------------------------------------------------*/
void 			CanBusCls::CAN_start (uint32_t ctrl)  
{
  LPC_CAN_TypeDef *pCAN = (ctrl == 1) ? LPC_CAN1 : LPC_CAN2;

  pCAN->MOD = 0;                                 ///  Enter normal operating mode 
}

/* ----------------------------------------------------------------------------
  check if transmit mailbox is empty
 *----------------------------------------------------------------------------*/
void 			CanBusCls::CAN_waitReady (uint32_t ctrl)  
{
  LPC_CAN_TypeDef *pCAN = (ctrl == 1) ? LPC_CAN1 : LPC_CAN2;

  while ((pCAN->SR & (1<<2)) == 0);              ///  Transmitter ready for transmission 
  CAN_TxRdy[ctrl-1] = 1;
}

/* ----------------------------------------------------------------------------
  wite a message to CAN peripheral and transmit it.  CAN controller (1..2)
 *----------------------------------------------------------------------------*/
void 			CanBusCls::CAN_wrMsg (uint32_t ctrl, CAN_msg *msg)  
{
  LPC_CAN_TypeDef *pCAN = (ctrl == 1) ? LPC_CAN1 : LPC_CAN2;
  uint32_t CANData;

  CANData = (((uint32_t) msg->len) << 16)     & 0x000F0000 | 
            (msg->format == EXTENDED_FORMAT ) * 0x80000000 |
            (msg->type   == REMOTE_FRAME)     * 0x40000000;

  if (pCAN->SR & (1<<2))  {                      ///  Transmit buffer 1 free 
    pCAN->TFI1  = CANData;                       ///  Write frame informations 
    pCAN->TID1 = msg->id;                        ///  Write CAN message identifier 
    pCAN->TDA1 = *(uint32_t *) &msg->data[0];    ///  Write first 4 data bytes 
    pCAN->TDB1 = *(uint32_t *) &msg->data[4];    ///  Write second 4 data bytes 
    //pCAN->CMR  = 0x31;                           ///  Select Tx1 for Self Tx/Rx 
    pCAN->CMR  = 0x21;                           ///  Start transmission without loop-back 
  }
}

/* ----------------------------------------------------------------------------
  read a message from CAN peripheral and release it.  CAN controller (1..2)
 *----------------------------------------------------------------------------*/
void 		CanBusCls::CAN_rdMsg (uint32_t ctrl, CAN_msg *msg)  
{
	union
	{
		uchar			Buffer[8];
		uint32_t		B32[2];
	}Tp;
  LPC_CAN_TypeDef *pCAN = (ctrl == 1) ? LPC_CAN1 : LPC_CAN2;
  uint32_t CANData;

                                                 ///  Read frame informations 
  CANData = pCAN->RFS;
  msg->format   = (CANData & 0x80000000) == 0x80000000;
  msg->type     = (CANData & 0x40000000) == 0x40000000;
  msg->len      = ((uint8_t)(CANData >> 16)) & 0x0F;

  msg->id = pCAN->RID;                           ///  Read CAN message identifier 

  if (msg->type == DATA_FRAME)                  ///  Read the data if received message was DATA FRAME   
  {
    Tp.B32[0] = pCAN->RDA;
    Tp.B32[1] = pCAN->RDB;
	 memmove( &msg->data[0],Tp.Buffer,8);
  }
}


/* ----------------------------------------------------------------------------
  setup acceptance filter.  CAN controller (1..2)
 *----------------------------------------------------------------------------*/
void 			CanBusCls::CAN_wrFilter (uint32_t ctrl, uint32_t id, uint8_t format)  
{
  static int CAN_std_cnt = 0;
  static int CAN_ext_cnt = 0;
         uint32_t buf0, buf1;
         int cnt1, cnt2, bound1;

  ///  Acceptance Filter Memory full 
  if ((((CAN_std_cnt + 1) >> 1) + CAN_ext_cnt) >= 512)
    return;                                       ///  error: objects full 

  ///  Setup Acceptance Filter Configuration  Acceptance Filter Mode Register = Off 
  	LPC_CANAF->AFMR = 0x00000001;

	 if (format == STANDARD_FORMAT)                ///  Add mask for standard identifiers 
	 {
    	id |= (ctrl-1) << 13;                        ///  Add controller number 
    	id &= 0x0000F7FF;                            ///  Mask out 16-bits of ID 
    	
    	///  Move all remaining extended mask entries one place up 
		///  if new entry will increase standard ID filters list 
    	if ((CAN_std_cnt & 0x0001) == 0 && CAN_ext_cnt != 0) 
    	{
    	  cnt1   = (CAN_std_cnt >> 1);
    	  bound1 = CAN_ext_cnt;
    	  buf0   = LPC_CANAF_RAM->mask[cnt1];
    	  while (bound1--)  {
    	    cnt1++;
    	    buf1 = LPC_CANAF_RAM->mask[cnt1];
    	    LPC_CANAF_RAM->mask[cnt1] = buf0;
    	    buf0 = buf1;
    	  }        
    	}
    	
    	if (CAN_std_cnt == 0)  
    	{                     ///  For entering first  ID 
    	  LPC_CANAF_RAM->mask[0] = 0x0000FFFF | (id << 16);
    	}  else if (CAN_std_cnt == 1)  {             ///  For entering second ID 
    	  if ((LPC_CANAF_RAM->mask[0] >> 16) > id)
    	    LPC_CANAF_RAM->mask[0] = (LPC_CANAF_RAM->mask[0] >> 16) | (id << 16);
    	  else
    	    LPC_CANAF_RAM->mask[0] = (LPC_CANAF_RAM->mask[0] & 0xFFFF0000) | id;
    	}  
    	else  
    	{
    	  ///  Find where to insert new ID 
    	  cnt1 = 0;
    	  cnt2 = CAN_std_cnt;
    	  bound1 = (CAN_std_cnt - 1) >> 1;
    	  while (cnt1 <= bound1)  {                  ///  Loop through standard existing IDs 
    	    if ((LPC_CANAF_RAM->mask[cnt1] >> 16) > id)  {
    	      cnt2 = cnt1 * 2;
    	      break;
    	    }
    	    if ((LPC_CANAF_RAM->mask[cnt1] & 0x0000FFFF) > id)  {
    	      cnt2 = cnt1 * 2 + 1;
    	      break;
    	    }
    	    cnt1++;                                  ///  cnt1 = U32 where to insert new ID 
    	  }                                          ///  cnt2 = U16 where to insert new ID 
    	
    	  if (cnt1 > bound1)  {                      ///  Adding ID as last entry 
    	    if ((CAN_std_cnt & 0x0001) == 0)         ///  Even number of IDs exists 
    	      LPC_CANAF_RAM->mask[cnt1]  = 0x0000FFFF | (id << 16);
    	    else                                     ///  Odd  number of IDs exists 
    	      LPC_CANAF_RAM->mask[cnt1]  = (LPC_CANAF_RAM->mask[cnt1] & 0xFFFF0000) | id;
    	  }  else  {
    	    buf0 = LPC_CANAF_RAM->mask[cnt1];        ///  Remember current entry 
    	    if ((cnt2 & 0x0001) == 0)                ///  Insert new mask to even address 
    	      buf1 = (id << 16) | (buf0 >> 16);
    	    else                                     ///  Insert new mask to odd  address 
    	      buf1 = (buf0 & 0xFFFF0000) | id;
    	 
    	    LPC_CANAF_RAM->mask[cnt1] = buf1;        ///  Insert mask 
    	
    	    bound1 = CAN_std_cnt >> 1;
    	    ///  Move all remaining standard mask entries one place up 
    	    while (cnt1 < bound1)  {
    	      cnt1++;
    	      buf1  = LPC_CANAF_RAM->mask[cnt1];
    	      LPC_CANAF_RAM->mask[cnt1] = (buf1 >> 16) | (buf0 << 16);
    	      buf0  = buf1;
    	    }
    	
    	    if ((CAN_std_cnt & 0x0001) == 0)         ///  Even number of IDs exists 
    	      LPC_CANAF_RAM->mask[cnt1] = (LPC_CANAF_RAM->mask[cnt1] & 0xFFFF0000) | (0x0000FFFF);
    	  }
    	}
    	CAN_std_cnt++;
  }  
  else  
  {                                     			///  Add mask for extended identifiers 
    	id |= (ctrl-1) << 29;                        ///  Add controller number 
    	
    	cnt1 = ((CAN_std_cnt + 1) >> 1);
    	cnt2 = 0;
    	while (cnt2 < CAN_ext_cnt)  {                ///  Loop through extended existing masks 
    	  if (LPC_CANAF_RAM->mask[cnt1] > id)
    	    break;
    	  cnt1++;                                    ///  cnt1 = U32 where to insert new mask 
    	  cnt2++;
    	}
    	
    	buf0 = LPC_CANAF_RAM->mask[cnt1];            ///  Remember current entry 
    	LPC_CANAF_RAM->mask[cnt1] = id;              ///  Insert mask 
    	
    	CAN_ext_cnt++;
    	
    	bound1 = CAN_ext_cnt - 1;
    	///  Move all remaining extended mask entries one place up 
    	while (cnt2 < bound1)  {
    	  cnt1++;
    	  cnt2++;
    	  buf1 = LPC_CANAF_RAM->mask[cnt1];
    	  LPC_CANAF_RAM->mask[cnt1] = buf0;
    	  buf0 = buf1;
    	}        
  }
  
  ///  Calculate std ID start address (buf0) and ext ID start address (buf1) 
  buf0 = ((CAN_std_cnt + 1) >> 1) << 2;
  buf1 = buf0 + (CAN_ext_cnt << 2);

  ///  Setup acceptance filter pointers 
  LPC_CANAF->SFF_sa     = 0;
  LPC_CANAF->SFF_GRP_sa = buf0;
  LPC_CANAF->EFF_sa     = buf0;
  LPC_CANAF->EFF_GRP_sa = buf1;
  LPC_CANAF->ENDofTable = buf1;

  LPC_CANAF->AFMR = 0x00000000;                  ///  Use acceptance filter 
}


/* ----------------------------------------------------------------------------
  CAN interrupt handler
 *----------------------------------------------------------------------------*/
__inline		void			CanBusCls::Interrupt(void)
{
  volatile uint32_t icr;

  U32 run1, run2, pconp;
  
  CAN_msg		RxMsg;
  // Check which channels are running 
  pconp = LPC_SC->PCONP;
  run1  = (pconp & (1 << 13)) != 0;
  run2  = (pconp & (1 << 14)) != 0;

  if(run1)
  {
	  icr = LPC_CAN1->ICR;                           ///  clear interrupts 

	  if (icr & (1 << 0))                           ///  CAN Controller #1 meassage is received 
	  {
		 CAN_rdMsg (1, &RxMsg);                		///   read the message 
	    LPC_CAN1->CMR = (1 << 2);                    ///  Release receive buffer 
	    
	    if(TD.CAN==tCAN_REC_DATA)
	    {
	    	Uart.puts("Can INT Test Data:");
			Uart.putb(1,LEN_ONE_CAN_DATA,(uchar*)&RxMsg);			
			Uart.puts("\r\n");
	    }

		 		 
		 GetAllDataCnt++;	
		 
		 if(CanBusNumberSID>10)
		 {
		 	if(CanOverTimeCnt>3)		// 指定ID已发送完毕，不再存储收到的数据
			{
				Uart.puts("指定CAN ID已接收完\t");
				return;
			}
		 }
		 
		 CanOverTimeCnt = 0;	
		 Can1RecOverTimeCnt = 0;		// 判断接收完上传的数据
		  
		 if(RxMsg.id != ByPassCanID )
		 {
			 if(CanBusNumberSID==0)	
			 {
				TestCanBus = TRUE;
				gCnt.Timer[eTMR_CAN_BUS] = 0;
				FlashPageCnt.CanBusDataSave = 0;
				FlashPageCnt.CanBusDataRead = 0;
				TestCanStartTime = gCnt.RtcSecond;
				SendTickCnt = 0;
				Pulse.RitInit(100);
			 }
			 CanBusNumberSID++;
			 if(RxMsg.id == GetCanID )				// 指定ID计数
			 	GetSpecifyCanIdCnt++;
			 /////////////////////////////////////// 存储数据到缓存
		    RxMsg.id = SwapINT32(RxMsg.id);			// 存储时，大端对齐
			 if(GetDataCnt>=(CAN_DATA_NUMBER_PER_PAGE<<1))
			 	GetDataCnt = 0;
			 memmove( &GetDataBuffer[GetDataCnt*LEN_ONE_CAN_DATA], &RxMsg, LEN_ONE_CAN_DATA );
			 GetDataCnt++;
			 if(GetDataCnt==(CAN_DATA_NUMBER_PER_PAGE<<1))		// 4Page
			 {
				memmove( TemporaryCanData,GetDataBuffer,512);
				GetDataCnt = 0;
			 	isr_evt_set( 0x0001, TaskID.Rece_CAN );
			 }
		  }
		  else
		  {
		  		ByPassCanCnt++;
		  }
	  }
	  else
	  {
	  	  Uart.puts("\r\n其它CAN中断 ");
		  Uart.putb(1,4,(uchar*)&icr);
		  Uart.puts("\r\n");
	  }

	  if (icr & (1 << 1))                           ///  CAN Controller #1 meassage is transmitted 
	  {
			CAN_TxRdy[0] = 1; 
	  }
	}
	else
	{
	  	  Uart.puts("\r\n其它CAN中断1 ");
		  Uart.putb(1,4,(uchar*)&icr);
		  Uart.puts("\r\n");
	  
	}

  if(run2)
  {
	  icr = LPC_CAN2->ICR;                           ///  clear interrupts 
	
	  if (icr & (1 << 0))                           ///  CAN Controller #2 meassage is received 
	  {
		CAN_rdMsg (2, &RxMsg);                ///   read the message 
	    LPC_CAN2->CMR = (1 << 2);                    ///  Release receive buffer 
	
	    CAN_RxRdy[1] = 1;                            ///   set receive flag 
	  }
	
	  if (icr & (1 << 1))                           ///  CAN Controller #2 meassage is transmitted 
	  {
		CAN_TxRdy[1] = 1; 
	  }
	}
}
















extern		"C"			void 			CAN_IRQHandler (void)  	__irq
{
	CanBus.Interrupt();
}

