
#define				BD_UPDATE_GLOBALS					1



#include	 	"BD_Update.h"
#include		"..\Common\Algol.h"
#include		"..\FLASH\DFE_25xxx.H"
#include		"..\FLASH\DFF_25xxx.H"
#include		"..\UART\Uart0.h"
#include		"..\UART\Uart3.h"
#include		"..\Common\FLASH_ADDR.H"		




	

#define		GNSS_UPDATE_FILE_IN_FLASH_PAGE_ADDR		FF_IAP2_CODE_START_PAGE				// 升级文件在FLASH中的页地址



extern uchar *pShareMem; 

uchar		StartBDUpdate = FALSE;



#if(USE_BD_UPDATE)

static uint16 AT_BUP_EscapeCharDecode(uint16 Len, uchar *pBuf)		 // 解转义
{

	uchar		c,c1;
	uchar		*pMal=NULL;
	uint16	i=0,j;
	NOP();	NOP();	
	
	while(pMal==NULL)
	{
		pMal = MyMalloc(Len);
		os_dly_wait(1);		
	}
	for(j=0,i=0;i<Len;i++)
	{
		c = *(pBuf+i);
		c1 = *(pBuf+i+1);
		if( (c == 0xdc)&&(c1==0xcb)	)	  	// 0xdc 0xcb -> 0xdb
		{
			*(pMal+j) = 0xdb;		j++;	
			i++;		
		}							
		else if( (c == 0xdc)&&(c1==0xcc)	)	// 0xdc 0xcc -> 0xdc
		{
			*(pMal+j) = 0xdc;		j++;			
			i++;
		}
		else if ( (c == 0xdc)&&(c1==0xce)	)		 	// 0xdc 0xce -> 0xde
		{
			*(pMal+j) = 0xde;		j++;			
			i++;
		}
		else
		{
			*(pMal+j) = c;		j++;
		}
	}	
	memmove(pBuf,pMal,j);
	MyFree(pMal);
	return j;
	
		 
}


static uint16 AT_BUP_EscapeCharEncode(uint16 Len, uchar *pBuf)	  //  转义
{
	uchar		c;
	uchar		*pMal=NULL;
	uint16	i=0,j;
	NOP();	NOP();	
	
	while(pMal==NULL)
	{
		pMal = MyMalloc(Len);
		os_dly_wait(1);		
	}
	for(j=0,i=0;i<Len;i++)
	{
		c = *(pBuf+i);
		if(c == 0xdb)		  	// 0xde -> 0xdc 0xcb
		{
			*(pMal+j) = 0xdc;		j++;
			*(pMal+j) = 0xcb;		j++;
		}							
		else if(c == 0xdc)	// 0xdc -> 0xdc 0xcc
		{
			*(pMal+j) = 0xdc;		j++;
			*(pMal+j) = 0xcc;		j++;
		}
		else if (c == 0xde)		 // 0xde -> 0xdc 0xce
		{
			*(pMal+j) = 0xdc;		j++;
			*(pMal+j) = 0xce;		j++;	
		}
		else
		{
			*(pMal+j) = c;		j++;
		}
	}	
	memmove(pBuf,pMal,j);
	MyFree(pMal);
	return j;
		 
}



static uint16 	AT_BUP_Pack(uchar Cmd, uint16 DataLen, uchar *pData, uchar *pReturnPackData)		 // 打包
{
	uchar Check;
	AT_BUP_PROTOCOL_HEAD_TYPE	 AtBup;
	uint16 Len;
	
	AtBup.Head = 0xdb;
	AtBup.Len = DataLen + 3;
	AtBup.DevId = 0x01;
	AtBup.Cmd = Cmd;

	Len =  sizeof(AT_BUP_PROTOCOL_HEAD_TYPE);
	memmove(pReturnPackData, (uchar *)&AtBup, Len);

	memmove(pReturnPackData+Len, pData, DataLen);

	Len += DataLen;

	Check = CalXorSum(Len-1, pReturnPackData+1); // 不包头0xdb

	//Uart.puts("Check Test Data:"); Uart.putb(1, 1, &Check);Uart.puts("\r\n");
											
	pReturnPackData[Len++] = Check;

	Len = AT_BUP_EscapeCharEncode(Len-1, pReturnPackData+1); // 转义不包括头
	
	Len += 1;
	pReturnPackData[Len++] = 0xde;


	return 	Len;
		
}


static uchar 	AT_BUP_UpPack_Proccess(uint16 DataLen, uchar *pData)	// 解包
{
	uchar Head = 0xdb, End = 0xde;
	uchar *pHead, *pEnd;
	uchar Len, Check;
	pAT_BUP_PROTOCOL_HEAD_TYPE	 pAtBup;
	char tBuffer[8];
	uint16	Tmp;

	pHead = (uchar *)SearchArray(DataLen, pData, 1, (uchar *)&Head);

	if (pHead != NULL)
	{
		Len =  DataLen - (pHead - pData) - 1;
		pEnd = (uchar *)SearchArray(Len, pHead, 1, &End);

		if (pEnd != NULL)
		{
			Len = pEnd - pHead + 1;
			AT_BUP_EscapeCharDecode(Len, pHead); // 

			pAtBup = (pAT_BUP_PROTOCOL_HEAD_TYPE)pHead;

			Check = CalXorSum(Len-3, pHead+1);

			if (Check != pHead[Len-2])
			{
				  Uart.puts("校验错误\r\n");
				  return ERROR;
			}



			switch(pAtBup->Cmd)
			{
			case	AT_BUP_UP_UPDATE_BAUDRATE_CMD:
				if (pHead[AT_BUP_DATA_INDEX+1] == 0x00) // ACK
				{
					Uart.puts("配置波特率成功");
					return OK;
				}
				else if ((pHead[AT_BUP_DATA_INDEX+1] == 0x01))
				{
					Uart.puts("不支持该波特率");
					
				}
				else if ((pHead[AT_BUP_DATA_INDEX+1] == 0x10))
				{
					Uart.puts("命令错误,需重发");
				
				}
				else
				{
					Uart.puts("其他错误");
					
				}
					

			break;

			case	AT_BUP_SET_FIRM_UPATE_PARA_CMD:
				Uart.puts("模块支持的最大数据包长度:");

				Tmp =   pHead[AT_BUP_DATA_INDEX] | (pHead[AT_BUP_DATA_INDEX+1]<<8); 
				Int2Str(Tmp, tBuffer);
				Uart.puts(tBuffer);
				Uart.puts("\r\n");
				
				if (pHead[AT_BUP_DATA_INDEX+2] == 0x00) // ACK
				{
					Uart.puts("支持当前固件的升级");
					return OK;
				}
				else if ((pHead[AT_BUP_DATA_INDEX+2] == 0x01))
				{
					Uart.puts("代码类型错误");
					
				}
				else if ((pHead[AT_BUP_DATA_INDEX+2] == 0x02))
				{
					Uart.puts("代码长度错误，代码长度必须小于256K");
					
				}
				else if ((pHead[AT_BUP_DATA_INDEX+2] == 0x10))
				{
					Uart.puts("命令错误,需重发");
					
				}

			break;

			case	AT_BUP_SEND_DATA_PACK_CMD:
				Tmp =   pHead[AT_BUP_DATA_INDEX] | (pHead[AT_BUP_DATA_INDEX+1]<<8); 
				Int2Str(Tmp, tBuffer);
				
				if (pHead[AT_BUP_DATA_INDEX+2] == 0x00) // ACK
				{
					Uart.puts("发送数据包:");
					Uart.puts(tBuffer);
					Uart.puts("\r\n");

					return OK;
				}
				else if ((pHead[AT_BUP_DATA_INDEX+2] == 0x01))
				{
					Uart.puts("程序代号、版本号等参数错误");
				}
				else if ((pHead[AT_BUP_DATA_INDEX+2] == 0x02))
				{
					Uart.puts("版本号没有变化");
					
				}
				else if ((pHead[AT_BUP_DATA_INDEX+2] == 0x10))
				{
					Uart.puts("命令错误,需重发");
					
				}

				break;

			case	AT_BUP_FIRM_UPDATE_FINISH:
				if (pHead[AT_BUP_DATA_INDEX] == 0x00) // ACK
				{
					Uart.puts("升级成功");

					return OK;
				}
				else if ((pHead[AT_BUP_DATA_INDEX] == 0x01))
				{
					Uart.puts("接收到的代码数据错误 ");
				}
				else if ((pHead[AT_BUP_DATA_INDEX] == 0x02))
				{
					Uart.puts("代码烧写错误 ");
					
				}
				else if ((pHead[AT_BUP_DATA_INDEX] == 0x03))
				{
					Uart.puts("代码验证错误");
					
				}
			

			break;

			case	AT_BUP_UPDATE_OK_RETSET_CMD:
				if (pHead[AT_BUP_DATA_INDEX] == 0x00) // ACK
				{
					Uart.puts("接收到正确的重启命令");

					return OK;
				}
				else if ((pHead[AT_BUP_DATA_INDEX] == 0x01))
				{
					Uart.puts("命令错误，需要重发");
				}
		

			break;
																								  

			default:
				break;
				
			}


			
		}
	}

	  	
	 return ERROR;
}






/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
  函数名称：  GNSSUpdateProccess
  功能描述：  定位模块升级处理
  入口参数: 
  出口参数：
  全局变量: 
  创建时间：
  修改时间：
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
static uchar tBuffer[256+4];
SET_GNSS_FIRW_UPDATE_PARA SetUpPara;
void GPSCls::GNSSUpdateProccess(void)
{
	
	uchar j,LeftLen,Pages, RSendTime;
	pBD_UPDATE_FILE_HEAD_TYPE pBD;
	pSEND_BD_UP_DATA_PACK pSendPack;
	uint16 FirmPages, TotalPack;
	uint32 Len, FirmLen;
	uint32 FirmInFlashPageAddr;
	uchar *pBuf, *pSendBuf;
	uint32	i;

	if (pShareMem == NULL)
	{
		Uart.puts("pShareMem 需分配空间\r\n");
	}

	pBuf = pShareMem;
	pSendBuf = pShareMem + 2048;  // 注意pMem有4K的空间


	// 从升级文获取升级信息
	SetBufferValue(tBuffer, 256,0);
	DFF.PageRead(GNSS_UPDATE_FILE_IN_FLASH_PAGE_ADDR,tBuffer);

	pBD =  (pBD_UPDATE_FILE_HEAD_TYPE)tBuffer;
	// 读取ATGM模块硬件信息

	//Uart.puts("BDUP Test Data:"); Uart.putb(1, sizeof(BD_UPDATE_FILE_HEAD_TYPE),tBuffer); Uart.puts("\r\n");

	// 检验头信息
	if (pBD->Head != 0x5441)	// "AT"
	{
		Uart.puts("FLASH不存在北斗固件代码\r\n");
		return;
	}

	 // 导航
	if ((pBD->FirmStartAddr==NAVIGATION_CODE_ADDR) && (pBD->FirmType==NAVIGATION_CODE_TYPE))		//  导航代码	 
	{
		Uart.puts("导航代码\r\n");

		SetUpPara.CodeType = pBD->FirmType;
		SetUpPara.Length = pBD->FirmLength;
		SetUpPara.StAdd = 0x00;	// 当 CodeType=3 时， StAdd=0x3e000；其它值时， StAdd=0 

		FirmLen = pBD->FirmLength;

		 FirmPages = pBD->FirmLength / 256;				 // 算出固件在FLASH多少页
		 LeftLen = pBD->FirmLength % 256;
		 if (LeftLen != 0)
		 {
		 	FirmPages += 1;
		 }

		 FirmInFlashPageAddr = GNSS_UPDATE_FILE_IN_FLASH_PAGE_ADDR + pBD->FirmInFileStartAddr / 256;	// 一页FLASH是256字节 从文件看，保存在FLASH中的固件开始地址正好是页对齐，先不考虑不对齐的情况

		 //SetBufferValue(tBuffer, 256,0);
		 //DFF.PageRead(FirmInFlashAddr,tBuffer);

		 //Uart.puts("BD Firm Test Data:"); Uart.putb(1, 256, tBuffer); Uart.puts("\r\n");
		 os_dly_wait(10);

		 //Uart3.puts("$PCAS06,1*1A\r\n");	//   模块硬件信息

		 // 启动在线升级命令
		 RSendTime = 3;				 // 重发3次
		 while(RSendTime)
		 {
			 //StartBDUpdate	= TRUE;
			 		 
			 SetBufferValue(tBuffer, 128,0);
			 Uart3.ClearRecBuffer();
	
			 Uart3.puts("$PCAS20*03\r\n");		
	
			 Uart3.WaitRecBuffer(700, 128,tBuffer);	  //  最长等7秒
			 
			 Uart.puts((char *)tBuffer);
			 os_dly_wait(1);
	
			 if (SearchArray(128, tBuffer, 7, (uchar *)"$PCAS30") == NULL)
			 {
			 	Uart.puts("\r\n在线升级启动失败\r\n");
				RSendTime--;
				if (RSendTime==0)
				{ 
					Uart3.Init(9600);
					return;	  // 错误三次
				}
			
			 }
			 else
			 {
			 	Uart.puts("\r\n在线升级启动成功\r\n");
			 	break;
			 }
		 }

		 // 提升波特率
		 Uart.puts("提升波特率\r\n");
		 pBuf[0] = 0x05;		// 0x01: 9600, 0x05: 115200   
		 Len = AT_BUP_Pack(AT_BUP_UP_UPDATE_BAUDRATE_CMD, 1, pBuf, pSendBuf);	

	
		 SetBufferValue(tBuffer, 128,0);
		 Uart3.ClearRecBuffer();

		 Uart.puts("BAUD  Test Data:"); Uart.putb(1, Len, pSendBuf); Uart.puts("\r\n");
		 Uart3.putb(Len, pSendBuf);
		 
		 Uart3.WaitRecBuffer(700, 128,tBuffer);	  //  最长等7秒
		 
		 Uart.puts("Ack BAUD Test Data:"); Uart.putb(1, 64, tBuffer); Uart.puts("\r\n");
		 os_dly_wait(1);


		 if (AT_BUP_UpPack_Proccess(128, tBuffer)	!= OK)
		 {
		 	  	 Uart.puts("提升波特率失败\r\n");
				 return;
		 }
	
		 Uart.puts("\r\n切换主机特率\r\n");
		 Uart3.Init(115200);						//  主机切换115200波特率
		 os_dly_wait(20);		      // 延时

		 // 设置固件升级参数	 /////////////////////////////////////////////////
		 Uart.puts("设置固件升级参数\r\n");

		Len =  sizeof(SET_GNSS_FIRW_UPDATE_PARA);
		memmove(pBuf, (uchar *)&SetUpPara, Len); 
		Len = AT_BUP_Pack(AT_BUP_SET_FIRM_UPATE_PARA_CMD, Len, pBuf, pSendBuf);	

		RSendTime = 3;				 // 重发3次
		 while(RSendTime)
		 {
		 	 SetBufferValue(tBuffer, 128,0);
			 Uart3.ClearRecBuffer();

			 Uart.puts("SetPara  Test Data:"); Uart.putb(1, Len, pSendBuf); Uart.puts("\r\n");
			 Uart3.putb(Len, pSendBuf);
			 
			 
			 Uart3.WaitRecBuffer(700, 128,tBuffer);	  //  最长等7秒
			 
			 Uart.puts("Ack SetPara Test Data:"); Uart.putb(1, 64, tBuffer); Uart.puts("\r\n");
			 os_dly_wait(1);
	
			 if (AT_BUP_UpPack_Proccess(128, tBuffer)	!= OK)
			 {
			 	  	 Uart.puts("设置固件升级参数失败\r\n");
	
					 //Uart3.Init(9600);
					 os_dly_wait(100);
					 RSendTime--;
					 //return;
					 if (RSendTime==0) return;	  // 错误三次
			 }
			 else
			 {
			 	break;
			 }
		 }

		 // 发送数据包	 //////////////////////////////////////////////////////////////////
		 Uart.puts("\r\n发送固件数据包\r\n");

		 TotalPack =  FirmLen / LEN_BD_UP_PACK_DATA;
		
		 if ((FirmLen % LEN_BD_UP_PACK_DATA) != 0)
		 {
		 	TotalPack += 1;
		 }

		 pSendPack = (pSEND_BD_UP_DATA_PACK)pBuf;

		 pSendPack->TotalPk = TotalPack;
			
		 //Uart.puts("TotalPack Test Data:"); Uart.putb(1, 2, (uchar *)&TotalPack); Uart.puts("\r\n"); 
		 //Uart.puts("FirmLen Test Data:"); Uart.putb(1, 4, (uchar *)&FirmLen); Uart.puts("\r\n"); 
		 //os_dly_wait(10);

		 // test 
		 //SetBufferValue(tBuffer, 256,0);
		 //DFF.PageRead(FirmInFlashPageAddr+0,(uchar *)&pSendPack->Data[0] );
		 //Uart.puts("BD Firm Test Data:"); Uart.putb(1, 256, (uchar *)&pSendPack->Data[0]); Uart.puts("\r\n");
		 //

		 

		 for (i = 1; i<=TotalPack; i++)			  // 包序从1开始 
		 {
		 	pSendPack->PkNo = i;
			if (i==TotalPack)			 // 最后一包
			{
				Len = FirmLen % LEN_BD_UP_PACK_DATA;		 // 少于LEN_BD_UP_PACK_DATA字节的数据长度

				// Uart.puts("Left Len Test Data:"); Uart.putb(1, 2, (uchar *)&Len); Uart.puts("\r\n");

				pSendPack->PkSize = Len;
	
				Pages = Len / 256;		
				LeftLen = Len % 256;		  // 少于256数据的长度
	
				if (LeftLen != 0)
				{
					Pages += 1;			 // 需要读FLASH多少页才最后一包数据读全	
				}

				Len += 6;		  // PkNo TotalPk PkSize
			}
			else
			{
				pSendPack->PkSize = LEN_BD_UP_PACK_DATA;
				Pages = 4;
				Len =  sizeof(SEND_BD_UP_DATA_PACK);
			}


			for (j=0; j<Pages; j++)
			{
				DFF.PageRead(FirmInFlashPageAddr+j,(uchar *)&pSendPack->Data[j*256] );
				os_dly_wait(1);
			}
			FirmInFlashPageAddr += Pages;			// 更新页地址
			
//			if (i==TotalPack)
//			{
//			 	Uart.puts("FirmInFlashPageAddr Test Data:"); Uart.putb(1, 4, (uchar *)&FirmInFlashPageAddr); Uart.puts("\r\n");
//				Uart.puts("Pages Test Data:"); Uart.putb(1, 1, (uchar *)&Pages); Uart.puts("\r\n");
//				Uart.puts("Send Len Test Data:"); Uart.putb(1, 2, (uchar *)&Len); Uart.puts("\r\n");
//				Uart.puts("读取最后一包数据 Test Data:"); Uart.putb(1, 256, (uchar *)&pSendPack->Data[768]); Uart.puts("\r\n");
//			}				 
		 	
		 	Len = AT_BUP_Pack(AT_BUP_SEND_DATA_PACK_CMD, Len, (uchar *)pSendPack, pSendBuf);	 

//			if (i==TotalPack)
//			{
//			 	Uart.puts("Send Len Test Data:"); Uart.putb(1, 2, (uchar *)&Len); Uart.puts("\r\n");
//			}
		

			RSendTime = 3;				 // 重发3次
			while(RSendTime)
			{
			 	SetBufferValue(tBuffer, 128,0);
			 	Uart3.ClearRecBuffer();
			
//				Uart.puts("发送升级包部分数据 Test Data:"); Uart.putb(1, Len, pSendBuf); Uart.puts("\r\n");
//			
//				if (i==TotalPack)
//				{
//			 		Uart.puts("发送最后一包数据 Test Data:"); Uart.putb(1, Len, pSendBuf); Uart.puts("\r\n");
//			 	}

			 	Uart3.putb(Len, pSendBuf);
			
				Uart3.WaitRecBuffer(700, 128,tBuffer);	  //  最长等7秒
			 
			 	//Uart.puts("Ack SetPara Test Data:"); Uart.putb(1, 20, tBuffer); Uart.puts("\r\n");
			   
				//os_dly_wait(1);			 // 延时
			
				 if (AT_BUP_UpPack_Proccess(128, tBuffer)	!= OK)
				 {
				 	  	 Uart.puts("发送升级包失败\r\n");
			
						 //Uart3.Init(9600);
						 os_dly_wait(100);
			
						 RSendTime--;
			
						 if(RSendTime==0)
						 {
							 Uart3.Init(9600);
						 	 return;
						 }
				 }
				 else
				 {
				 	 break;
				 }
			}

		 }

		 // 查询是否有升级完成通知	 ////////////////////////////////////////
		 //Uart3.ClearRecBuffer();
		 //SetBufferValue(tBuffer, 128,0);

		 AT_BUP_UpPack_Proccess(128, tBuffer+10); //		最后一包应答和升级完成通知可能粘包, tBuffer 要10字节

		 //Uart3.WaitRecBuffer(1500, 128,tBuffer);	  //  最长等15秒
		 RSendTime = 3;				 // 重查3次
		 while(RSendTime)
		 {
			 Uart3.ClearRecBuffer(); 
			 SetBufferValue(tBuffer, 128,0);
	
			 Uart3.WaitRecBuffer(1500, 128,tBuffer);	  //  最长等15秒
			 if (AT_BUP_UpPack_Proccess(128, tBuffer)	!= OK)
			 {
		 	 	Uart.puts("查询不到升级完成通知\r\n");
			 	//Uart3.Init(9600);
			 	os_dly_wait(10);

			
				 RSendTime--;
	
				 if(RSendTime==0)
				 {
					 Uart3.Init(9600);
				 	 return;
				 }

				
			 }
			 else
			 {
			 	break;
			 }
			 
		 }
		 
		 // 升级完成，重启模块
		 Uart.puts("升级完成，重启模块\r\n");
		 Len =  sizeof(SET_GNSS_FIRW_UPDATE_PARA);
		 Len = AT_BUP_Pack(AT_BUP_UPDATE_OK_RETSET_CMD, 0, pBuf, pSendBuf);	
		 
		 SetBufferValue(tBuffer, 128,0);
		 Uart3.ClearRecBuffer();

		 Uart.puts("SetPara  Test Data:"); Uart.putb(1, Len, pSendBuf); Uart.puts("\r\n");
		 Uart3.putb(Len, pSendBuf);
		 
		 Uart3.WaitRecBuffer(700, 128,tBuffer);	  //  最长等7秒
		 
		 Uart.puts("Ack SetPara Test Data:"); Uart.putb(1, 32, tBuffer); Uart.puts("\r\n");
		 os_dly_wait(1);

		 if (AT_BUP_UpPack_Proccess(128, tBuffer)	!= OK)
		 {
		 	  	 Uart.puts("重启模块错误\r\n");

				 Uart3.Init(9600);
				 os_dly_wait(10);
				 return;
		 }

		 NVIC_SystemReset();	 //				//升级完成整个系统重启


		 //StartBDUpdate	= FALSE;

		
		
	}


	
}

#endif


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
  函数名称：  GNSSUpdateProccess
  功能描述：  定位模块
  入口参数: 
  出口参数：
  全局变量: 
  创建时间：
  修改时间：
*/
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用

/// $GPTXT,01,01,02,HW=ATGM331C,0003112105070*1C
/// $GPTXT,01,01,02,SW=URANUS2,V2.4.3.0*19\r\n



static uchar  GetGNNSVer(char *pCmdStr, char *pSearch,	char *pVerBuf)
{
	 uchar tBuffer[128];
	 uchar Len, i = 0, RetryTimes;
	 uchar *pS, *pE;
	 
	 RetryTimes = 3;
	 while (RetryTimes) 
	 {

		 SetBufferValue(tBuffer, 128,0);
		 Uart3.ClearRecBuffer();
	
		 Uart3.puts(pCmdStr);			// 查北斗硬件版本
	
		 Uart3.WaitRecBuffer(300, 128,tBuffer);	  //  最长等3秒
	
		 pS = (uchar *)SearchArray(128, tBuffer, strlen(pSearch), (uchar *)pSearch);
		 if (pS != NULL)
		 {
		 	Len = pS - &tBuffer[0] + 1;
		 	pE = (uchar *)SearchArray(128-Len, pS, 2, (uchar *)"\r\n");
			if (pE != NULL)
			{
				Len = pE - pS - 3;				  // 	  减掉多出的3个字符   如  *19
				memmove(pVerBuf, pS, Len);				  // 提取版本信息  	SW=URANUS2,V2.4.3.0
	
				i = Len;

				break;
			}							
			 
		 }
		 else
		 {
		 	RetryTimes--;

			if (RetryTimes==0)
			{
				 pVerBuf[i++]	= '0';
		 		 pVerBuf[i++]	= ',';
		 		 pVerBuf[i++]	= '0';

				 break;
				
			}
		 }
	 }


	 return i;

}

uchar GPSCls::QueryGNSSVersion(char *pVer)
{
	 uchar Len, i;

	 if (pVer == NULL) return ERROR;

	  i = GetGNNSVer("$PCAS06,0*1B\r\n", "SW=", pVer);	// 软件版本
	  Len = i;
	  pVer[Len++] = ',';												// 分隔符
	  i = GetGNNSVer("$PCAS06,1*1A\r\n", "HW=",pVer+Len);		//	硬件版本	
	  Len += i;
	  pVer[Len]	 = 0;
	  
	  return OK;					
	
}

