/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												肖遥电子（广州）研发中心
$									Shawyoo Electronics (GuangZhou) R&D Center
$                      	http://www.shawyoo.com		 Email: autoele@qq.com
$						 QQ群：8502321/9069471/9069433/9524253		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 	 DriverCard.c
	功能描述：司机卡处理
	编译环境：
	目标CPU:  
	作者：    
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  
#include			"..\IIC\IIC0.h"
#include			<string.h>
#include			<stdlib.h>
#include			"..\RTC\RTC.h"
#include			"Speed.h"
#include			"..\KeyDisp\KeyDisp.h"
#include       "..\flash\DFE_25xxx.h"
#include       "..\flash\DFF_25xxx.h"
#include			"..\Common\GB_T19056.h"
#include			"..\UART\UART0.H"
#include			"..\GSM_GPRS\GSM_GPRS.H"
#include			"..\GSM_GPRS\JT_T808.H"
#include			"..\GPS\GPS.h"
#include			"..\Common\FLASH_ADDR.H"

// 从器件地址的高5 位固定为10100 接下来的2 位A1 A0 为器件的地址位
// 李珂 500382197911253837
// 王达 500382197611243839

// IIC与USB共用一个物理接口，通过CD4052切换
//#define			IIC_ENABLE			{ IODIR0 |= (1<<18); IOSET0 = (1<<18);}
//#define			USB_ENABLE			{ IODIR0 |= (1<<18); IOCLR0 = (1<<18);}

#define			CHECK_DRIVER_ID_VALID			1

#define        LEN_IC_CARD_INFO              256

#define    	   DRIVER_LOG_IN                  1
#define        DRIVER_LOG_OUT                 2
#define        CHANGE_DRIVER                  3
#define        SAME_DRIVER                    4
#define        NO_ACTION                      0xff

#define		   LEN_JT_DRIVER_INFO				 100

#define			USE_IIC_IC_CARD_MODULE				1


#define        IC_CARD_SIZE                    128
#define        NOACTION                      0xff

#define			DRIVER_IN							0x1
#define			DRIVER_OUT							0x2



extern uchar IC_CardAuthFailure;	
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：CheckNewDriver
   功能描述：检测是否是新司机身份验证(是否与前一次认证是同一司机)
   入口参数：无，使用类内结构变量CurrentDriverInfoTime
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
*/ ///////////////////////////////////////////////////////////////////////////////////////
uchar			SpeedCls::CheckNewDriver(void)
{
	#if(USE_IIC_IC_CARD_MODULE==1)
	uchar		s,NewCard, Ret;
	uint32	i,Addr,tl;
	union
	{
		uchar		Buffer[32];
		uint16	B16[16];
		uint32	B32[8];
	}Tp;
	//DRIVER_INFO_TIME		DriverInfoTime[2];
	
	
	NewCard = TRUE;		// 假设为新卡
	for(i=0;i<2;i++)
	{
		//Uart.puts("i Test Data:"); Uart.putb(1,4,(uchar *)&i); Uart.puts("\r\n");
		//Uart.puts("RecentDriverInfoTime[i] Test Data:"); Uart.putb(1, LEN_DRIVER_INFO_TIME,(uchar *)&RecentDriverInfoTime[i]); Uart.puts("\r\n");

		s = MyStrCmpNB( 18, (char*)&CurrentDriverInfoTime.ID,(char*)&RecentDriverInfoTime[i].ID);
		if(s==0)			// 找到相同ID的卡，当前插入的卡为旧卡
		{
			Uart.puts("找到相同的ID卡");
			NewCard = 0;

			if(( RecentDriverInfoTime[i].TravelTimeSec != 0)&&	// 驾驶时间非0，说明休息时间不够，不更新插卡时间和驾驶时间
				((RecentDriverInfoTime[i].OffTime+ RecentDriverInfoTime[i].RestTimeSec)<LimitTime.RestTimeSec))// 不在位时间加上之前的休息时小于最小休息时间
			{
				RecentDriverInfoTime[i].RestTimeSec += RecentDriverInfoTime[i].OffTime;
				RecentDriverInfoTime[i].OffTime = 0;

				KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(22*2+1),(uchar*)"\x47请注意：休息时间不足，重复插卡，视为连续驾驶");	
				memmove( (uchar*)&CurrentDriverInfoTime, (char*)&RecentDriverInfoTime[i], LEN_DRIVER_INFO_TIME );

				Ret = DRIVER_IN;
			}
			else		// 更新插卡时间，驾驶时间
			{				
				CurrentDriverInfoTime.Number = RecentDriverInfoTime[i].Number;		// 使用原来的编号
				Addr = LEN_DRIVER_INFO_TIME;
				Addr *= i;
				Addr += FM_RECENT_DRIVER_INFO_TIME;
				DFE.Write ( Addr, LEN_DRIVER_INFO_TIME, (uchar*)&CurrentDriverInfoTime );

				Ret = DRIVER_IN;
			}
			///////////////////////////// 修改
			DFE.Write ( FM_CURRENT_DRIVER_INFO_TIME, LEN_DRIVER_INFO_TIME, (uchar*)&CurrentDriverInfoTime );
			break;
		}
	}

	if(NewCard==TRUE)		// 未找到相同ID的卡，当前插入的卡为新卡
	{
		Uart.puts("新的ID卡");
		DFE.Read ( FM_DRIVER_SERIAL_NUMBER,4,Tp.Buffer);		// 自编号，5位数
		if( Tp.B32[0]<DRIVER_NUMBER_MIN)	Tp.B32[0] = DRIVER_NUMBER_MIN;
		Tp.B32[0]++;
		DFE.Write ( FM_DRIVER_SERIAL_NUMBER,4,Tp.Buffer);		// 自编号加1后存储
		NOP(); NOP();
		DFE.Write(FM_DRIVER_NUMBER, 4, Tp.Buffer);
		NOP(); NOP();
		CurrentDriverInfoTime.Number = Tp.B16[0];
		DFE.Write ( FM_CURRENT_DRIVER_INFO_TIME, LEN_DRIVER_INFO_TIME, (uchar*)&CurrentDriverInfoTime );
		
		Ret = DRIVER_IN;

		for(i=0;i<2;i++)
		{
			if( RecentDriverInfoTime[i].Number <DRIVER_NUMBER_MIN+1)		// 存储在未使用的空间
			{
				Addr = LEN_DRIVER_INFO_TIME;
				Addr *= i;
				Addr += FM_RECENT_DRIVER_INFO_TIME;
				DFE.Write ( Addr, LEN_DRIVER_INFO_TIME, (uchar*)&CurrentDriverInfoTime );
				RecentDriverInfoTime[i] = CurrentDriverInfoTime;
				NewCard = 0;
				break;
			}
		}
		if( NewCard==TRUE)		// 未存储，且无空间，需覆盖最旧的卡，（插卡时间最早就为最旧的）
		{
			tl = RecentDriverInfoTime[0].StartTimeSec;
			for(i=1;i<2;i++)	// 排序，找出最早插卡的时间
			{
				if(tl>RecentDriverInfoTime[i].StartTimeSec)	tl = RecentDriverInfoTime[i].StartTimeSec;
			}
			for(i=0;i<2;i++)
			{
				if(tl==RecentDriverInfoTime[i].StartTimeSec)	break;		// 找到，跳出
			}
			Addr = LEN_DRIVER_INFO_TIME;
			Addr *= i;
			Addr += FM_RECENT_DRIVER_INFO_TIME;
			DFE.Write ( Addr, LEN_DRIVER_INFO_TIME, (uchar*)&CurrentDriverInfoTime );
			RecentDriverInfoTime[i] = CurrentDriverInfoTime;

		}		
	}
	#endif
	return Ret;
}
//////////////////////////////////////////////// 













static uchar 	Connect_Specify_Center(uchar Action, uchar Center)
{
	uchar i, s, TcpIndex = 0;

	if (Center==CONNECT_MAIN_CENTER)	   	TcpIndex =  eFLAG_TCP;
	else if (Center==CONNECT_AUTH_CENTER)  TcpIndex =  eFLAG_TCP2;

	if (Action==CHECK_CENTER_ONLINE)
	{
		if (sIP.Online[TcpIndex]== ENABLE_FLAG) {	return OK; }
		else  { goto WAIT_FOR_SPECIFY_CENTER;	 }
	}

	if (Center==CONNECT_MAIN_CENTER)
	{
		JT808_Ctr_IP_DNS.Ctr = 1;
	 	s = 0;
	 	DFE.Write(EE_ENABLE_TCP2,1,(uchar*)&s);
		NOP(); 	NOP(); 	NOP(); 
		DFE.Write(EE_JT_NET_CTR ,1,(uchar*)&JT808_Ctr_IP_DNS.Ctr);
		NOP(); NOP(); NOP(); 
		JT808_Ctr_IP_DNS.Net_Time = 0;
		DFE.Write(EE_JT_NET_CTR_NET_TIME,2,(uchar*)&JT808_Ctr_IP_DNS.Net_Time);
		NOP(); NOP(); NOP();  
		
			
	}
	else if (Center==CONNECT_AUTH_CENTER)
	{
		SetBufferValue((uchar *)&JT808_Ctr_IP_DNS, sizeof(JT808_Ctr_IP_DNS), 0);
		SetBufferValue(&JT808_Ctr_IP_DNS.C_ANP[0], LEN_IPorDNS, 0);
		memmove(&JT808_Ctr_IP_DNS.C_ANP[0], &JT808_IP_DNS.M_APN[0], LEN_IPorDNS);
		DFE.Read(EE_JT_IC_AUTH_M_IP_DNS, LEN_IPorDNS, &JT808_Ctr_IP_DNS.C_IPorDNS[0]);
		NOP();
		DFE.Write(EE_JT_NET_CTR_IP_DNS, LEN_IPorDNS,&JT808_Ctr_IP_DNS.C_IPorDNS[0]);
		NOP();NOP();NOP(); 
		DFE.Read(EE_JT_IC_AUTH_TCP_PORT, 4, (uchar *)&JT808_Ctr_IP_DNS.C_TCP_Port);
		NOP();
		DFE.Write(EE_JT_NET_CTR_TCP_PORT, 2, (uchar *)&JT808_Ctr_IP_DNS.C_TCP_Port);
		NOP();NOP();NOP(); 	
		DFE.Read(EE_JT_IC_AUTH_UDP_PORT, 4, (uchar *)&JT808_Ctr_IP_DNS.C_UDP_Port);
		NOP();
		DFE.Write(EE_JT_NET_CTR_UDP_PORT, 2, (uchar *)&JT808_Ctr_IP_DNS.C_UDP_Port);
		NOP();NOP();NOP(); 
		// 不把临时连接写入铁电
	   JT808_Ctr_IP_DNS.Net_Time = 0; // 
		JT808_Ctr_IP_DNS.Ctr = 1; // 
	   DFE.Write(EE_JT_NET_CTR, LEN_JT_CTR_IP_DNS,(uchar *)&JT808_Ctr_IP_DNS);
		NOP(); NOP(); NOP();
		JT808_Ctr_IP_DNS.Net_Time = 2; 		// 2分钟
		JT808_Ctr_IP_DNS.Ctr = 0; 				// 临时仅启用TCP2	
		gCnt.Timer[eTMR_TMP_SERVER] = 1;
	}

	 sIP.EnableFlag[TcpIndex] = ENABLE_FLAG;					//JT808

	 DisConectSomeSocket = 1;				// 断开所有链接
	 os_dly_wait(100);					 	// 延时为了让GSM任务重启拔号

	if (Action!=CONNECT_CENTER_WAIT) return OK;

WAIT_FOR_SPECIFY_CENTER:

	i = 0;
	while (sIP.Online[TcpIndex]!= ENABLE_FLAG)	   // 等待上线
	{
		
		i++;	
		if (i > 300)	return ERROR;			 // 30秒超时

		os_dly_wait(10);
	}
	return OK;
}







///////////////////////////////////////////////

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称： CheckDriverCard
   功能描述： 检测是否是司机卡(驾驶员身份卡型号24C256)
   入口参数：
   出口参数： 
   全局变量: 
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uchar			SpeedCls::CheckDriverCard(void)
{
	static uint32 CheckCnt = 0;
	uchar       RetS, Len, i, s;
	//OS_RESULT 	result;
	//uchar		UserName[32+1];
	uchar	tBuffer[16];

	union 
	{
		
		uchar    ICCardAllData[IC_CARD_SIZE];
		struct
		{
		  uchar     CardTypeInfo[5];				//卡类型内容	 如果内容为： “GPSBD”, 为普通卡，其他为442卡
		  uchar     ProductResv[11];				//芯片厂商保留
		  uchar		DriverName[16];				//驾驶员姓名
		  uchar		DriverCardID[18];				//驾驶证号码
		  uchar		DriverCarVaildYear;        //驾驶证有效期-年	  BCD
		  uchar		DriverCarVaildMonth;       //驾驶证有效期-月	  BCD
		  uchar		DriverCarVaildDate;        //驾驶证有效期-日	  BCD
		  uchar     CertificateID[18];  			//从业资格证号
		  uchar     StandardResv[56];				//标准扩展预留
		  uchar     CheckSum;						//异或校验和
	   }ICCardData;
	}Tmp;
	uchar		CheckSum;
	pDRIVER_INFO_TIME pDriverInfo;
	NOP();	NOP();	
	//IIC_ENABLE;					// 选通IIC通道
	RetS = 0;			   // 先假设没有驾驶员身份认证卡
	//os_dly_wait(1);			// 延时一个节拍		
	//UserEvtFlag.USB_Enum = 0;
	//RetS=(I2C0.ReadEEPROM(CARD_DEVICE_ADDR,51,8,Tmp.ICCardAllData));

	//if (((DriverCardFlag==0))&&(StopFlag==0))	// 插入卡，没停车，没有驾驶卡标志

	if((GET_IC_CARD_STATUS)==0)		// IC卡已插入 （用于常开卡座）  	  // 这里修改的话，CheckIC_Card里也要修改
	//if((GET_IC_CARD_STATUS)!=0)		// IC卡已插入	(用于常闭卡座)
	{
		if ((StopFlag==0)&&(CheckCnt++ % 300) == 0)
		{
			if (DriverCardFlag==0)
			{
				KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(8*2+1),(uchar*)"\x47请先停车，再插卡");
			}
			else
			{
				//KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG,(9*2+1),(uchar*)"\x47正在行驶，拔卡无效");
			}
		}
	}
	else
	{
		if (StopFlag == 1)
		{
			CheckCnt = 0;
		}

	}

	if (StopFlag == 0) return 0;				  //   不停车，不进行下面操作

	I2C0.CheckIC_Card();				 //检测是否有卡

	//
	//if(RetS==ERROR)					 //读IC卡权限，成功则有IC卡
	if(IC_CardType==0)
	{		 
	 	//Uart.putb(1, 8,(uchar *)Tmp.ICCardAllData); // ("Driver Card Test1!\r\n\0");
		SetCarFlat = 0;
		if((DriverCardFlag==1)&&(IC_Card_Insert==TRUE))				//是否驾驶卡	 &&(StopFlag==1)
		{
		    DriverCardFlag = 0; 
	 	    DFE.Write(FM_ICCardInOrOut_FLAG,1,&DriverCardFlag);
			BEEP(9,0,4,0,5);
			NOP();
			RecodeInOut(DRIVER_OUT);				
			IC_Card_Insert = FALSE;	
			CheckCnt = 0;

			// 
			for(i=0;i<2;i++)
			{
				//pDriverInfo = (pDRIVER_INFO_TIME)&Tmp;
				//DFE.Read(FM_RECENT_DRIVER_INFO_TIME+i*LEN_DRIVER_INFO_TIME, LEN_DRIVER_INFO_TIME, (uchar*)pDriverInfo );
				//s = MyStrCmpNB( 18, (char*)&CurrentDriverInfoTime.ID,(char*)&pDriverInfo->ID);
				//if(s==0)			// 找到相同ID的卡，当前插入的卡为旧卡
				if (RecentDriverInfoTime[i].Number==CurrentDriverInfoTime.Number)
				{
					Uart.puts("保存当前驾驶员的信息");
					CurrentDriverInfoTime.OffTime = 0;
					RecentDriverInfoTime[i] = CurrentDriverInfoTime; 
					DFE.Write (FM_RECENT_DRIVER_INFO_TIME+LEN_DRIVER_INFO_TIME*i, LEN_DRIVER_INFO_TIME, (uchar*)&CurrentDriverInfoTime );
					NOP(); NOP();
					//Uart.puts("i Test Data:"); Uart.putb(1,4,(uchar *)&i); Uart.puts("\r\n");
					//Uart.puts("save RecentDriverInfoTime[i] Test Data:"); Uart.putb(1, LEN_DRIVER_INFO_TIME,(uchar *)&RecentDriverInfoTime[i]); Uart.puts("\r\n");
					break;
				}		
			}
			SetBufferValue((uchar*)&CurrentDriverInfoTime, LEN_DRIVER_INFO_TIME, 0);
			DFE.Write (FM_CURRENT_DRIVER_INFO_TIME,LEN_DRIVER_INFO_TIME,(uchar*)&CurrentDriverInfoTime); // 清空当前驾驶员信息

			GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DRIVER_INFO_REPORT,0,NULL);
	   //
															                 
	   }
//	   return FALSE;																					               //读卡不成功，无IC卡
	}
	else
	{	 
		if (DriverCardFlag) 	  //插入完成
		{
			IC_Card_Insert = TRUE;
			return 0;
		}

		SetBufferValue(Tmp.ICCardAllData, 128,0);
		if(IC_CardType==IC_AT24C_CARD)
		{
		   I2C0.ReadEEPROM(CARD_DEVICE_ADDR,0,128,Tmp.ICCardAllData);
		}
		else
		{
		  I2C0.PinFor4442(1);
		  os_dly_wait(1);
		  I2C0.Reset4442Card();
		  I2C0.Read4442Card(0,128,Tmp.ICCardAllData);
		  I2C0.PinFor4442(0);
		  Uart.putb(1, 256,Tmp.ICCardAllData);
		}


		if ((Tmp.ICCardData.DriverCardID[0]<'0') || (Tmp.ICCardData.DriverCardID[0]>'9') || 
		   (Tmp.ICCardData.DriverCardID[1]<'0') || (Tmp.ICCardData.DriverCardID[1]>'9'))		//  驾驶证号是否合理
		{
			return 0;
		} 

		RetS = Tmp.ICCardAllData[0];
		CheckSum = CalXorSum(127, Tmp.ICCardAllData);

		//Uart.puts("IC Test Data:"); Uart.putb(1,128, Tmp.ICCardAllData); Uart.puts("\r\n");
		//Uart.puts("Check Test Data:");	Uart.putb(1,1, &CheckSum); Uart.puts("\r\n");
		

		if (CheckSum == Tmp.ICCardData.CheckSum)  // 校验码是否正确
		{	
				if(DriverCardFlag==0)																						     //是否新插入卡
				{
					SetBufferValue((uchar *)&CurrentDriverInfoTime,LEN_DRIVER_INFO_TIME,0);
					Len = 18;
					memmove(CurrentDriverInfo.ID, Tmp.ICCardData.DriverCardID, Len);
					memmove(CurrentDriverInfoTime.ID, Tmp.ICCardData.DriverCardID, Len);
					DFE.Write(FM_DRIVER_CODE, Len, CurrentDriverInfo.ID);  // 18位从业资格证号 兼容以前的存储
					NOP(); NOP();
					DFE.Write(EE_JT_DRIVER_CARD_ID, Len, CurrentDriverInfo.ID);

					// added 2015.1.19
					memmove((uchar*)&CurrentDriverInfoTime.StartLocatoin,(uchar*)&ValidLocationInfo,LEN_LOCATION_INFO);
					
					
					memmove(CurrentDriverInfo.Name, Tmp.ICCardData.DriverName, Len);
					CurrentDriverInfo.Name[15] = 0;	  // 强制数组最后一字节置0， 为下面的strlen
					Len = strlen((char *)CurrentDriverInfo.Name);
					DFE.Write(EE_JT_DRIVER_NAME_LEN, 1, &Len);
					NOP();
					DFE.Write(EE_JT_DRIVER_NAME, Len, CurrentDriverInfo.Name);

					IC_CardAuthFailure = 0; // 认证成功

					Len = 0;
					DFE.Write(EE_JT_CERTIFICATE_NAME_LEN, 1, (uchar *)&Len);

					SetBufferValue(tBuffer,16,0);
					tBuffer[0] = 0x20;
					memmove(&tBuffer[1], &Tmp.ICCardData.DriverCarVaildYear,3);
					DFE.Write(EE_JT_DRIVER_CARD_VALID_DATE, 4, &tBuffer[0]);

					RetS = CheckNewDriver();																  //判断是否新卡，并记录相关驾驶数据
					if(RetS==NOACTION)
					{  
					   //Uart.puts("Driver Card Error!\r\n\0");
						return ERROR;
				   }
					else
					{	
					    IC_Card_Insert = TRUE;
						 GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DRIVER_INFO_REPORT,0,NULL); 
						//GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_I2C_DRIVER_INFO_REPORT,0,NULL);		//I2C卡读到的驾驶员信息采集上报
						BEEP(9,0,2,0,10);
					  //Uart.puts("Driver Card 0K!\r\n\0");
						DriverCardFlag = 1; 
				 	   DFE.Write(FM_ICCardInOrOut_FLAG,1,&DriverCardFlag);
						RecodeInOut(RetS);												      //记录登录记录		
						
					}
				}
			}

						
	    
	}	
	NOP();	NOP();
	//USB_ENABLE;
	NOP();	NOP();
	return RetS;
} // End of <CheckDriverCard> function

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称： RecodeInOut
   功能描述： 记录登录退出记录
   入口参数：
   出口参数： 
   全局变量: 
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
void			SpeedCls::RecodeInOut(uchar InOrOut)
{	
	uint32 tl;
	uint32 Addr;
	DRIVER_IN_OUT_STATUS Status;
	uchar tBuffer[256];


	Rtc.GetRTC_BCD((RTC_STR *)tBuffer);
	memmove(Status.Time,tBuffer,6);
	memmove(Status.ID,CurrentDriverInfoTime.ID,18);
	Status.EvenType = InOrOut;

	DFE.Read(FM_DRIVER_OUT_IN_CNT,4,(uchar*)&tl);
	Addr = tl%10;
	Addr *= LEN_DRIVER_IN_OUT_STATUS;
	Addr += FM_DRIVER_OUT_IN;		  	
	DFE.Write( Addr, LEN_DRIVER_IN_OUT_STATUS,(uchar *)&Status);
	tl++;
	DFE.Write(FM_DRIVER_OUT_IN_CNT,4,(uchar*)&tl);	 	// 驾驶员登入登出计数器加1后存储到铁电	
	//
	Uart.puts("更新保存驾驶登入登出记录到铁电 "); 
	if ((tl)&&(tl%10)==0)
	{
		// 铁电充满近一页的Flash的空间，则放到Flash上
		if( (FlashPageCnt.DriverLogInOutSave%FF_PAGE_PER_SECTOR)==0)
		{
			Addr = FF_DRIVER_IN_OUT_START_PAGE;
			Addr += FlashPageCnt.DriverLogInOutSave%FF_DRIVER_IN_OUT_TOTAL_PAGE;
			
			DFF.SectorEraseWaitOK(Addr/FF_PAGE_PER_SECTOR,10);
		}				
		///////////////////////////////////////////存储到Flash  这样1页flash存有10条驾驶员登入登出记录
		DFE.Read(FM_DRIVER_OUT_IN,LEN_DRIVER_IN_OUT_STATUS*10,tBuffer); // 

		Addr = FF_DRIVER_IN_OUT_START_PAGE;
		Addr += FlashPageCnt.DriverLogInOutSave%FF_DRIVER_IN_OUT_TOTAL_PAGE;
		DFF.PageProgramWaitOK(Addr,tBuffer,3);
		FlashPageCnt.DriverLogInOutSave++;

		DFE.Write(FM_FLASH_PAGE_CNT,LEN_FLASH_PAGE_CNT,(uchar*)&FlashPageCnt);

  		Uart.puts("保存驾驶登入登出记录到Flash"); 
		
	}
					
}





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称： SendDataToRFID
   功能描述： 发送数据到RFID模块
   入口参数：
   出口参数： 
   全局变量: 
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用

#define		RFID_I2C_ADDR		0x50

uchar		SpeedCls::SendDataToRFID(uchar Cmd, uchar *pData, uint16 Len)
{
	uchar tBuffer[128];
	uchar  i;
	uint32	CheckSum;
	JT808_DEVICE_PROTOCOL_HEAD DeviceCom;

	Uart.puts(" 写IC卡数据S1 ");

	if (Len > 128)	return ERROR;
	
	
	DeviceCom.VersionNo = 0;//0x0101;
	DeviceCom.ManufacturerNo = 0;//0x1201;    // 未知，待查,暂时定一个值	
	DeviceCom.DeviceTypeNo = 0x0B;     // JT808 2013
	DeviceCom.Cmd	= Cmd;

	SetBufferValue(tBuffer, 128, 0);
	tBuffer[0] = 0x7e;			// 标识字节

	memmove(&tBuffer[2], (uchar *)&DeviceCom.VersionNo, 6);	   // 从版本号开始 共6个字节
	if ((Len != 0) && (pData != NULL) )
		memmove(&tBuffer[8], pData, Len);

	CheckSum = 0;
	for (i = 0; i < Len + 6; i++)
	{
		CheckSum += tBuffer[i+2];  
	}


	tBuffer[1] = DeviceCom.CheckSum = CheckSum & 0xff;	   // 填充校验码

	i = EscapeCharEncode(Len+7, &tBuffer[1]);  // 含校验码

	tBuffer[i+1] = 0x7e;		  // 标识字节

	i += 2;     // 头尾两个标识字节

	if (I2C_Busy_Now_Flag == 1) { os_dly_wait(10); }
	I2C_Busy_Now_Flag = 1;

		
	I2C0.Write(RFID_I2C_ADDR, 0, i, tBuffer);
	os_dly_wait(1);

	I2C_Busy_Now_Flag = 0;

	Uart.puts(" 写IC卡数据S2 ");
	
   return OK;

	 
	
}	


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：GetDataFromRFID
   功能描述： 接收RFID模块的数据
   入口参数：
   出口参数： 
   全局变量: 
   创建时间：
   修改时间：
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uchar		SpeedCls::GetDataFromRFID(uchar Cmd, uchar *pData, uint16 Len)
{
	uchar tBuffer[200];
	uchar  i, j, Length,*pS, *pE;
	uint32	CheckSum;

	if(Cmd != 0xff)
	{	
		Uart.puts(" 读取IC卡数据S1 ");
		if (I2C_Busy_Now_Flag == 1) { os_dly_wait(10); }
		I2C_Busy_Now_Flag = 1;
	
		SetBufferValue(tBuffer, 128, 0);
		I2C0.Read(RFID_I2C_ADDR, 0, 128, tBuffer);	
		os_dly_wait(20);
	
		I2C_Busy_Now_Flag = 0;
	}
	else
	{
		memmove( tBuffer,pData,Len);
	}


	i =0x7E;
	pS = SearchArray(128, tBuffer,1, &i);

	if (pS == NULL)	return 0xff;
	Uart.puts(" 读取IC卡数据S2 ");

	Length = pS - &tBuffer[0];
	Length = 128 - Length - 1 - 7;				   // 搜索剩余数据的长度，JT808外设的两个7E之间至少有7个字节,  再减去“7e"一个字节
	pE = SearchArray(Length , pS+8, 1, &i);	   // 按JT808外设协议，pS+8指向用户数据或 7E

	//Uart.puts("CCC1"); os_dly_wait(1);

	if (pE == NULL)		return 0xff;			 // 没找到第二个7E,数据无效

	Uart.puts(" 读取IC卡数据S3 ");
    
	Length = pE - pS - 1;

	Length = EscapeCharDecode(Length, pS+1);       // 转义还原 

	// 校验数据
	CheckSum = 0;
	j = Length;
	for (i = 1; i < j; i++)		 //  
	{
		CheckSum += *(pS+i+1);  
	}

	if ((CheckSum & 0xff) != *(pS+1))  
	{
		 return 0xff;
	}
	Uart.puts(" 读取IC卡数据S4 ");
	if (pData != NULL)
	{
		j =  Length + 2;  // 返回RFID的数据

		memmove(pData, pS, j); 
		return j;
			
	}

   Uart.puts(" 读取IC卡数据S5 ");
	return 0;

}





uchar			SpeedCls::U2_SendDataToRFID(uchar Cmd, uchar *pData, uint16 Len)
{
	uchar tBuffer[128];
	uchar  i;
	uint32	CheckSum;
	JT808_DEVICE_PROTOCOL_HEAD DeviceCom;

	Uart.puts(" U2写IC卡数据S1 ");

	if (Len > 128)	return ERROR;
	
	
	DeviceCom.VersionNo = 0;//0x0101;
	DeviceCom.ManufacturerNo = 0;//0x1201;    // 未知，待查,暂时定一个值	
	DeviceCom.DeviceTypeNo = 0x0B;     // JT808 2013
	DeviceCom.Cmd	= Cmd;

	SetBufferValue(tBuffer, 128, 0);
	tBuffer[0] = 0x7e;			// 标识字节

	memmove(&tBuffer[2], (uchar *)&DeviceCom.VersionNo, 6);	   // 从版本号开始 共6个字节
	if ((Len != 0) && (pData != NULL) )
		memmove(&tBuffer[8], pData, Len);

	CheckSum = 0;
	for (i = 0; i < Len + 6; i++)
	{
		CheckSum += tBuffer[i+2];  
	}


	tBuffer[1] = DeviceCom.CheckSum = CheckSum & 0xff;	   // 填充校验码

	i = EscapeCharEncode(Len+7, &tBuffer[1]);  // 含校验码

	tBuffer[i+1] = 0x7e;		  // 标识字节

	i += 2;     // 头尾两个标识字节

	Uart2.putb(1,i,tBuffer);
	Uart2.SendTask();

	Uart.puts(" U2写IC卡数据S2 ");
	
   return OK;
}







//7E 89 00 00 19 00 01 38 86 08 88 04 02 0B 00 01 5B F7 DD E8 FF 1D 2D 48 00 00 00 00 00 00 00 00 00 00 00 00 B0 AF A2 7E 
uchar				WaitIC_CardAuthTime,IC_CardAuthFailure;

void				SpeedCls::U2_ForRFID_Fun(void)
{
	uint16		i,s,Len;
	uchar			Sum,Index,tBuffer[200];
	pJT808_EXT_DEVICE_DATA	pRFID;
	RTC_STR		Rtc1;
	#pragma		pack(1)
	struct	
	{
		uchar		DriverNum[18];				//驾驶证号码
		uchar		DriverName[20];			//驾驶员姓名
		uint32	DriverCode;					//驾驶员代码
	
	}DriverInfo;

	static		uchar Get_Driver_Info_OK = FALSE;

	tBuffer[0] = tBuffer[1] = tBuffer[2] = 0;
	Len = Uart2.RFID_ReceiveData(tBuffer);
	if( (Len == 0)||(tBuffer[0]!=0x7e) )		return;

	if(TD.MainTask==tMAIN_IC_CARD_DATA)
	{
		Uart.puts("\r\nRFID Test Data:");
		Uart.putb(1,Len,tBuffer);
		Uart.puts("\r\n");
	}

	////////////////////////////////////////////////
	if(Len>200)
	{
		Uart.puts(" RFID数据大于200字节 ");
		Len = 200;
	}

	Len = EscapeCharDecode( Len-1, &tBuffer[1]);

  	if(TD.MainTask==tMAIN_IC_CARD_DATA)
	{
		Uart.puts("\r\nRFID Test Data:");
		Uart.putb(1,Len,tBuffer);
		Uart.puts("\r\n");
	}

	for(Sum=0,i=4;i<Len;i++)
	{
		Sum += tBuffer[i];		
	}
	if( Sum != tBuffer[1] )		Uart.puts(" RFID数据校验错误 ");

	pRFID = (pJT808_EXT_DEVICE_DATA)&tBuffer[0];
	if( (pRFID->Cmd==0x40)&&(pRFID->Data.Buffer[0]==0) )		// 读卡
	{
		IC_Card_Auth_GPRS = TRUE;
		WaitIC_CardAuthTime = 90;
		IC_Card_Auth_Flag = 0;
		GPRS.InitAlig('A');	

		Rtc.GetRTC_BCD((RTC_STR *)&Rtc1);
		DFE.Write(FM_RUN_TIME_START,6,(uchar*)&Rtc1);
		NOP(); 	NOP(); NOP(); NOP(); NOP(); NOP(); 
		
		s = Connect_Specify_Center(CONNECT_AUTH_WAIT, CONNECT_AUTH_CENTER); // 连接认证中心
		os_dly_wait(100);
		if(s!=OK)
		{
			Uart.puts("  链接IC卡认证中心服务器失败 ");
			return;
		}
		/////////////////////////////////// 发送数据到中心
		pRFID->Data.Buffer[0] = 0x0b;
		for (i=0; i<2; i++)			  // 如果认证中心没有回复，重发一次
		{
			Uart.puts("透传驾驶员信息到认证中心，入队 ");			
	   	GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DATA_TRAN_UPLOAD,(64+1),pRFID->Data.Buffer);
			for (s=0; s<(60*100); s++)		  // 10s超时
			{
				os_dly_wait(1);
				if( (s%100)==0)
				{
					if(WaitIC_CardAuthTime)	WaitIC_CardAuthTime--;
				}	
				if (IC_Card_Auth_Flag==TRUE)	
				{
					WaitIC_CardAuthTime = 0;
					break;		// 认证中心已返回								
				}
			}
			if (IC_Card_Auth_Flag==TRUE)		
			{
				WaitIC_CardAuthTime = 0;
				break;
			}
		} 
	}
	else if( (pRFID->Cmd==0x41) )			// 认证结果	
	{
		if( pRFID->Data.Buffer[0]==0 )		// 成功
		{
			IC_CardAuthFailure = 0;
			Uart.puts("\r\nIC卡认证成功 ");
			U2_SendDataToRFID(0x41, NULL, 0);		// 回复成功
			WaitIC_CardAuthTime = 0;
			Get_Driver_Info_OK = TRUE;
      	
			DriverCardFlag = TRUE; 
   		DFE.Write(FM_ICCardInOrOut_FLAG,1,&DriverCardFlag);
      	
			Index = JT_DRIVER_NAME_LEN_INDEX;	
			Len =  tBuffer[Index];			   //  驾驶员姓名长度
			//RetS = Len;
			if (Len > 20) s = 20;	  			// 只能存20个字节
			else		  	  s = Len;
			DFE.Write(EE_JT_DRIVER_NAME_LEN, 1+s, &tBuffer[JT_DRIVER_NAME_LEN_INDEX]);	     // 记录驾驶员姓名长度及姓名
			NOP(); NOP();NOP();NOP();NOP();NOP();NOP();
			memmove(DriverInfo.DriverName, &tBuffer[JT_DRIVER_NAME_LEN_INDEX], 1+s);
      	
			// add 2013.6.21
			memmove(CurrentDriverInfo.Name, &tBuffer[JT_DRIVER_NAME_LEN_INDEX], 1+s);
			//
			
			Index += (Len + 1);
			DFE.Write(EE_JT_DRIVER_CARD_ID, 20, &tBuffer[Index]);  // 20位从业资格证号
			NOP(); NOP();NOP();NOP();NOP();NOP();NOP();
			DFE.Write(FM_DRIVER_CODE, 18, &tBuffer[Index]);  // 18位从业资格证号 兼容以前的存储
			NOP(); NOP();NOP();NOP();NOP();NOP();NOP();
			memmove(DriverInfo.DriverNum, &tBuffer[Index], 18);
			
			// add 2013.6.21
			memmove(CurrentDriverInfo.ID, &tBuffer[Index], 18);
			//
			
			
			// 因JT808没驾驶员代码，暂时自定义按下面定驾驶员代码
			DriverInfo.DriverCode = (DriverInfo.DriverNum[17]-0x30)+(DriverInfo.DriverNum[16]-0x30) * 10+(DriverInfo.DriverNum[15]-0x30) * 100;
			DriverInfo.DriverCode = SwapINT32(DriverInfo.DriverCode); 
			DFE.Write(FM_DRIVER_NUMBER, 4, (uchar *)&DriverInfo.DriverCode); 
			NOP(); NOP();NOP();
			
			// add 2013.6.21
			CurrentDriverInfo.Number = DriverInfo.DriverCode;
			//
			
			Index += 20;
			Len =  tBuffer[Index];
			if (Len > 60) s = 60;                   // 只能存60个字节
			else		  s = Len;
			DFE.Write(EE_JT_CERTIFICATE_NAME_LEN, s+1, &tBuffer[Index]);  // 记录发证机构名称长度和名称
			NOP(); NOP();NOP();NOP();NOP();NOP();NOP();
      	
			Index += (Len + 1);
			DFE.Write(EE_JT_DRIVER_CARD_VALID_DATE, 4, &tBuffer[Index]);  // 20位从业资格证号
			NOP(); NOP();NOP();NOP();NOP();NOP();NOP();
      	
      	Uart.puts(" 透传驾驶员信息到认证中心，入队2 ");
			GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DATA_TRAN_UPLOAD,(64+1),pRFID->Data.Buffer);
      	      	
			os_dly_wait(500);
			
			IC_Card_Auth_GPRS = 0;
			IC_Card_Insert = TRUE;
			Connect_Specify_Center(CONNECT_CENTER_WAIT, CONNECT_MAIN_CENTER);	   // 连接监控中心,无等待
			os_dly_wait(200);
			Uart.puts(" 透传驾驶员信息到监控中心，入队 ");
			GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DRIVER_INFO_REPORT,0,NULL);
			
		}
		else //if( pRFID->Data.Buffer[0]==0 ) 1:失败，密码不对；2：失败，卡片被锁；3、失败、卡片被拔出；4、失败，数据校验错误
		{
			IC_CardAuthFailure = pRFID->Data.Buffer[0];
	
			IC_Card_Insert = TRUE;
			Connect_Specify_Center(CONNECT_CENTER_WAIT, CONNECT_MAIN_CENTER);	   // 连接监控中心,无等待
			os_dly_wait(300);
			Uart.puts(" 读卡失败，通知监控中心 ");
			GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DRIVER_INFO_REPORT,0,NULL);
			
		}

	}	//7E 4E 00 01 01 00 0B 42 7E 
	else if(	(pRFID->Cmd==0x42) )	  			// IC卡拔出
	{
			IC_Card_Insert = FALSE;	
			DriverCardFlag = 0; 
	 	   DFE.Write(FM_ICCardInOrOut_FLAG,1,&DriverCardFlag);
			BEEP(9,0,4,0,5);
			NOP();NOP();NOP();
			os_dly_wait(5);
			WaitIC_CardAuthTime = 0;			
			U2_SendDataToRFID(0x42, NULL, 0);	  // 拔卡通知	   	直接关掉了，不需要通知RFID
			os_dly_wait(10);
			U2_SendDataToRFID(0x42, NULL, 0);	  // 拔卡通知
			os_dly_wait(10); 
			
			if (Get_Driver_Info_OK == TRUE)
			{
				Uart.puts("上传驾驶员认证报告1，入队 ");
				GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DRIVER_INFO_REPORT,0,NULL);	  // 发向监控中心
				os_dly_wait(500);
	
				IC_Card_Auth_GPRS = TRUE;
				if (ERROR == Connect_Specify_Center(CONNECT_CENTER_WAIT, CONNECT_AUTH_CENTER)) // 连接认证中心，并按超时等待上线，
				{
					Uart.puts("Can not Connect Auth Center 2\r\n");
					os_dly_wait(1);
				}
				else
				{
			   	GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DRIVER_INFO_REPORT,0,NULL);
			  		os_dly_wait(500);
					Uart.puts("上传驾驶员认证报告，入队 ");
				}
				IC_Card_Auth_GPRS = 0;
				// 清空驾驶员信息
				SetBufferValue(tBuffer, 128, 0);
				DFE.Write(EE_JT_DRIVER_NAME_LEN, LEN_JT_DRIVER_IC_CARD_INFO, tBuffer);
				NOP(); 	NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); 
				
			}
			Get_Driver_Info_OK = FALSE;
			
			Connect_Specify_Center(CONNECT_CENTER_NO_WAIT, CONNECT_MAIN_CENTER);	   // 连接监控中心,无等待	
	}
	else if(	pRFID->Cmd==0x05 )	
	{
		
	}

}







uchar			SpeedCls::RFID_ReaderSeltTest(void)
{
	EXT_5V_POWER_ON;
	os_dly_wait(100);
	U2PinSelDevice = U2_FOR_RFID;
	os_dly_wait(100);
	Uart2.Init( 9600, U2_FOR_RFID);
	os_dly_wait(100);
	U2_SendDataToRFID(0x05, NULL, 0);			// 自检
	
	return OK;
}













/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */

