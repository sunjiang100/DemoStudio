/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$									广州国业电子科技有限公司
$									
$                       	http://www.guoyee.com		 Email: guoyegps@126.com	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
文件名:   gsm_send.cPP
功能描述：GSM发送任务
编译环境：
目标CPU: 
作者：
创建时间：
建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/	
#define			AT_CMD_GLOBALS			1
#include			"ATCmd.h"
#include		   ".\UserSys.h"
#include			"GSM_GPRS.h"

 
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	AT命令
*/
//uchar		const		atrLen[MAX_AT_CMD_NUM] = {7,6,7,7,7,6,6,9,6,9,14,8,14,6,6};

// // 此表顺序必须与ATCmd.h的AT命令枚举对应，否则出错
char		const		AT_CMD_Tab[MAX_AT_CMD_NUM][32] = 
{	
	{"AT\0"},							// 0, 
	{"AT\0"},							// 0, 
	{"ATE0\0"},							// 0, 						// 
	{"AT+IFC=\0"},			// 硬件流控
	{"AT+QIHEAD=0\0"},			// 设置数据头部信息
	{"AT+QIPROMPT=0\0"},		// 设置发送命令不返回'>'		
	/////////////////////////////
	{"AT+COPS?\0"},					// 检测运营商标识
	{"AT+QIMUX=1\0"},			//	设置多路TCP/UDP连接		1： 允许多路		0：只允许一路
	{"AT+QICSGP=1,\"\0"},				// 运营商网络，国内使用"CMNET"
	{"AT+QIFGCNT=0\0"},			//  配置前置场景	 
	{"AT+QIREGAPP\0"},			// 注册TCP/IP协议栈
	{"AT+QIACT\0"},      		// 激活GPRS场景
	{"AT+QIDEACT\0"},			// 关闭GPRS	场景
	{"AT+QINDI=\0"},			// 配置是否缓存接收到的GPRS数据： 1 ：使用缓存   0：不缓存，直接输出到串口	
	{"AT+QICLOSE=\0"},				// 关闭Socket,后面追加通道号
	{"AT+QIOPEN=\0"},					// 建立Socket TCP通道	  此处与定义的eAT_QIOPEN_TCP那行对应
	{"AT+QIOPEN=\0"},					// 建立Socket UDP通道	  此处与定义的eAT_QIOPEN_UDP那行对应
	{"AT+QISTAT\0"},				// 查询GSM状态
	{"AT+QILOCIP\0"},			// 查询GSM模块本地IP地址	
	{"AT+QIMODE=0\0"},           // 非透传模式	 1: 透传  	0： 非透传
	{"AT+QIDNSGIP=\0"},					// 把域名解析成IP地址
	{"AT+QIDNSIP=0\0"},			//   设置通过IP或DNS连接	0：通过IP连接	1：通过DNS连接
	/////////////////////////////
	{"AT+CMGF=0\0"},					// =0: PDU; =1: TEXT
	{"AT+CMGF=1\0"},					// =0: PDU; =1: TEXT
	("AT+CSCS=\"GSM\"\0"),		// 选择TE字符集
	{"AT+CPMS?\0"},		// 查询短消息存储器		 	//{"AT+CPMS=\"SM\"\0"},		// 选择短消息存储器
	{"AT+CMGD=0,2\0"},				// 删除已读和已发送短信	 {"AT+CMGD=0,4\0"}, // 删除所有短信
	{"AT+CNMI=2,2,0,0,0\0"},	// 短消息主动读入			 // //	{"AT+CNMI=0,0\0"},				// 短信提示
	{"AT+CSCA=\0"},					// 设置短信服务中心号码
	{"AT+CSCA?\0"},					// 查询短信服务中心号码
	/////////////////////////////
	{"AT+CLIP=1\0"},					// 来电显示	
	{"AT+CRC=1\0"},
	{"ATS0=2\0"},					  	// 两次振铃自动应答
	{"ATD\0"},							// 打电话,后面追加电话号码
	{"ATDL;\0"},						// 重拨最后那次去电
	{"ATH\0"},							// 挂机
	{"ATA\0"},							// 摘机
	{"AT+CLVL=\0"},					// 受话音量
	{"AT+CLVL?\0"},	  				// 查询受话音量
	/////////////////////////////
	{"AT+CPIN?\0"},					// SIM卡状态
	{"AT+CSQ\0"},						// 信号质量
	{"AT+CREG?\0"},					// 注册网络状态	
	//////////////////////////// 
	{"AT+CNUM\0"},		 				// SIM卡电话号码（需运营商支持）
	{"AT+QCCID?\0"},				  	// 读SIM卡串号
	{"AT+CGSN\0"},						// 读通讯模块IEMI号
	{"AT+CGMR\0"},						// 读通讯模块版本号	
	{"AT+CIMI\0"},						// 读运营商国际ID	
	{"AT+QMIC=0,12\0"},			 // 设置麦克风的增益		
	{"AT+QISACK=\0"},	
	{"\0"},	
};
	

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：命令号、发送数据指针、返回数据指针
   出口参数：成功返回OK，错误返回命令号
   全局变量:   
   创建时间：
   修改时间：
	注：不支持数据量大的AT指令，需使用专用函数，如收发短信、收发GPRS数据
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
uchar			GSMCls::AT_CommandWaitRetStatus(uchar Cmd,uchar *pData,uchar *pRet)
{
	uchar		c;
	char		atBuffer[128];	
	char		*pS,*pOK,RetStatus;
	uint16	i,j,Len;	
	uint32	t;
	union
	{
		uchar		Buffer[16];
		uint16	B16[8];
		uint32	B32[4];
	}Tp;
	if(WaitGSM_Free()==ERROR)		return Cmd;

	AT_Command_Busing = TRUE;

	if (TD.GPRS == tGPRS_AT_ACK_CHECK)			   // for debug
	{
		LPC_UART0->THR = 'B'; 	LPC_UART0->THR = '1'; 
	}

	MOBILE_POWER_ON;
	
	
	SetBufferValue((uchar*)atBuffer,128,0);
	Uart1.ClearReceiveFlag(0);

	strcpy(atBuffer,(char*)&AT_CMD_Tab[Cmd][0]);	 	// 取AT指令
	if(pData!=NULL)	strcat(atBuffer,(char*)pData);							// 追加数据
	strcat(atBuffer,"\r\n\0");								// 追加回车换行
	
	if((TD.GPRS == tGPRS_GSM_ALL_DATA)||(Cmd==eAT_QIOPEN_TCP)||(Cmd==eAT_QIDNSGIP) )
		Uart.puts(atBuffer);
		
	Uart1.puts(atBuffer);										// 发送AT指令
	SetBufferValue((uchar*)atBuffer,128,0);
	Len = Uart1.WaitRecBuffer((30*100),128,(uchar*)atBuffer);	  		// 等待返回

	if(Len==0)	{ AT_Command_Busing = FALSE; return Cmd; }	  // 无应答，无需判断其它数据

	MOBILE_POWER_ON;
	//TC.GG_Rec = TC.GG_Send = 0;
	
	pS = MyStrStr((char*)atBuffer,(char*)"ERROR\0");
	pOK = MyStrStr((char*)atBuffer,(char*)"OK\r\n\0");

	if(TD.GPRS == tGPRS_GSM_ALL_DATA)	Uart.puts(atBuffer);
	
	//////////////////////////////////////
	RetStatus = Cmd;				// 默认非OK
	switch(Cmd)
	{		
		case eAT_OK:          	// \r\nOK\r\n	   or		\r\nERROR\r\n
		case eAT_E0:          	// \r\nOK\r\n
		case eAT_IFC:
		case eAT_CIPHEAD:
		case eAT_QIPROMPT: 
		case eAT_QICSGP:     	
			
		///////////////	// ///////////////
		case eAT_CMGF0:       	// \r\nOK\r\n
		case eAT_CMGF1:       	// \r\nOK\r\n
		case eAT_CSCS:
		case eAT_CPMS:
		case eAT_CMGD_ALL:     	// \r\nOK\r\n
		case eAT_CNMI:        	// \r\nOK\r\n
		case eAT_CSCA_SET:    	// \r\nOK\r\n	
		case eAT_CLIP:        	// \r\nOK\r\n
		case eAT_S0:          	// \r\nOK\r\n
		//case eAT_D:           	// \r\nOK\r\n   无分号也能打通，受话端来电提示"数据通话"
		case eAT_DL:          	// \r\nOK\r\n
		case eAT_H:           	// \r\nOK\r\n
		case eAT_A:           	// \r\nOK\r\n
		case eAT_CLVL_SET:    	// \r\nOK\r\n
		case eAT_QIMUX:
		case eAT_QIDNSIP:	
		case eAT_QMIC:
		case eAT_QIFGCNT:
		case eAT_QIREGAPP:
		case eAT_QIACT:
		case eAT_QIMODE:
		case eAT_QINDI:

			if(pOK != NULL)
				RetStatus = OK;
			
		break;					// 参上命令，只返回OK或ERROR，无数据，无需处理。
			
		/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
		*/

		case eAT_QIDNSGIP:		  		  
			if ((pOK != NULL) && (NULL == MyStrStr((char*)atBuffer,(char*)"ERROR")))  // M35 
			{
				os_dly_wait(150);
				SetBufferValue((uchar *)atBuffer, 64,0);
				Uart1.WaitRecBuffer((30*100),64,(uchar*)atBuffer);	  		// M35模块返回成功标志比较慢,需要再次确认

				if(pRet == NULL)   break;

				pS = MyStrStr((char *)atBuffer, (char *)"\r\n\0");
				if (pS != NULL)				  // 把回车换行替换成空格
				{
					pOK = MyStrStr((char *)&atBuffer[2], (char *)"\r\0");
					if (pOK != NULL)				  // 把回车换行替换成空格
					{
						Len = pOK - pS - 2;
						//Uart.puts("DNS:");
						//Uart.putb(1, Len, (uchar *)(pS+2));
					}
				}

				if(pOK != NULL)
				{
					pS += 2;				
					for(j=0,i=0;i<Len;i++)
					{
						c = *pS++;
						if( (c=='.') || ((c>='0')&&(c<='9')) )	
						{
							*(pRet+j) = c;
							j++;
						}
						else break;
					}	
					RetStatus = OK;
				}									
			}

		break;
			
		case eAT_QICLOSE:
		
			/*if( (pOK!=NULL) )
			{
				RetStatus = OK;
			}*/

			pS = MyStrStr((char*)atBuffer,(char*)", CLOSE OK");	 //  0, CLOSE OK
			if (pS != NULL)
			{
				c = Str2Int(pS-1);
				RetStatus = OK;
			}		
		break;
		
		
		case eAT_COPS:        	// \r\n+COPS: 0:"CHINA MOBILE"\r\n\r\nOK\r\n
			pS = MyStrStr((char*)atBuffer,(char*)"+COPS\0");
			if( (pS != NULL)&&(pOK!=NULL))
			{
				RetStatus = OK;
				pS = MyStrStr((char*)atBuffer,(char*)"CHINA\0");
				if(pS != NULL)
				{
					pS = MyStrStr((char*)atBuffer,(char*)"MOBILE\0");
					if(pS != NULL)
					{
						GG_Status.Flag.Bits.ChinaMobile = 1;
						GG_Status.Registration = TRUE;
						//GSM_Registration_Type=0x10;
					}
					pS = MyStrStr((char*)atBuffer,(char*)"UNICOM\0");
					if(pS != NULL)
					{
						GG_Status.Flag.Bits.ChinaUnion = 1;
						GG_Status.Registration = TRUE;
						//GSM_Registration_Type=0x10;
					}
				}	
				else
				{
					GG_Status.Flag.Bits.ChinaMobile = 0;
					GG_Status.Registration = TRUE;
					SystemLog.GSM.UnRegTime++;
				}
			}
		break;
		
		case eAT_QIOPEN_TCP:
		case eAT_QIOPEN_UDP:	 		
			pS = MyStrStr((char*)atBuffer,(char*)"CONNECT OK\0");				 // TCP 连接成功
			if (pS != NULL)
			{
				NetConnectCnt = 0;
				RetStatus = OK;
			}
			else
			{
				os_dly_wait(150);
				SetBufferValue((uchar *)atBuffer, 64,0);
				Uart1.WaitRecBuffer((30*100),64,(uchar*)atBuffer);	  		// M35模块返回成功标志比较慢,需要再次确认
				if(TD.GPRS == tGPRS_GSM_ALL_DATA) 			Uart.puts(atBuffer);
				if(NULL != MyStrStr((char*)atBuffer,(char*)"CONNECT OK\0"))
				{
					NetConnectCnt = 0;
					RetStatus = OK;
				}
			}	
			pS = MyStrStr((char*)atBuffer,(char*)"ALREAD CONNECT\0");
			if (pS != NULL)
			{
				NetConnectCnt = 0;
				RetStatus = OK;
			}
		break;

		case eAT_QISTAT:
			pS = MyStrStr((char*)atBuffer,(char*)"+QISTAT:\0");
			//if ((pS != NULL) && (pOK != NULL))
			if (pS != NULL)
			{  
				if (NULL != MyStrStr((char*)pS,(char*)"TCP\0"))
				{
					RetStatus = OK;
				}

				if (NULL != MyStrStr((char*)pS,(char*)"UDP\0"))
				{
					RetStatus = OK;
				}
			}
		break;

		case eAT_QILOCIP:
			pS = MyStrStr((char*)atBuffer,(char*)"\r\n");
			if (pS != NULL)
			{
				pS += 2;
				if(SearchArrayCount(Len, (uchar *)atBuffer, 1,(uchar *)".") >= 3)  // 找到IP特征 
				{	
					for(j=0,i=0;i<20;i++)
					{
						c = *pS++;
						
						if( (c=='.') || ((c>='0')&&(c<='9')) )	
						{
							*(pRet+j) = c;
							j++;
						}
						else break;
					}
					RetStatus = OK;	
				}
			}
			
		break;
		
		case eAT_CSCA_GET:    	// \r\n+CSCA: "+8613800200500",145\r\n   \r\nOK\r\n
			pS = MyStrStr((char*)atBuffer,(char*)"+CSCA\0");
			if( (pS != NULL)&&(pOK!=NULL))
			{
				if(pRet != NULL)
				{
					pS += 11;
					for(j=0,i=0;i<20;i++)
					{
						c = *pS++;
						if((c>='0')&&(c<='9')) 
						{
							*(pRet+j) = c;
							j++;
						}
						else break;
					}	
				}
				RetStatus = OK;
			}
		break;
		///////////////	// ///////////////
		
		case eAT_CLVL_GET:    	// \r\n+CLVL: 80\r\n    \r\nOK\r\n
			pS = MyStrStr((char*)atBuffer,(char*)"+CVLV\0");
			if( (pS != NULL)&&(pOK!=NULL))
			{
				c = *(pS+7);
				
				RetStatus = OK;
			}
		break;
		
		///////////////	// ///////////////
		case eAT_CPIN:        	// \r\n+CPIN: READY\r\n\r\nOK\r\n
			pS = MyStrStr((char*)atBuffer,(char*)"+CPIN: READY\0");
			if( (pS != NULL)&&(pOK!=NULL))
			{
				RetStatus = OK;
				GG_Status.Flag.Bits.SimOK = 1;
			}
			else
				GG_Status.Flag.Bits.SimOK = 0;
		break;
		
		case eAT_CSQ:         	// \r\n+CSQ: 19,99\r\n   \r\nOK\r\n
			pS = MyStrStr((char*)atBuffer,(char*)"+CSQ\0");
			if( (pS != NULL)&&(pOK!=NULL))
			{
				RetStatus = OK;
				GG_Status.Signal = My_atol(pS+6);
				SystemLog.GG1.Gsm_Info |= 	 GG_Status.Signal << 8;

				//GSM_Signal = GG_Status.Signal;
				if(GG_Status.Signal>5)
					GG_Status.Flag.Bits.SignalOK = 1;
				else
					GG_Status.Flag.Bits.SignalOK = 0;
			}
		break;   
		
		case eAT_CREG:        	// \r\n+CREG: 0,1\r\n\r\nOK\r\n
			pS = MyStrStr((char*)atBuffer,(char*)"+CREG\0");
			if( (pS != NULL)&&(pOK!=NULL))
			{
				if( (*(pS+9))=='1')
					GG_Status.Flag.Bits.RegGSM = 1;
				else
					GG_Status.Flag.Bits.RegGSM = 0;				
				if( (*(pS+9))=='5')
					GG_Status.Flag.Bits.Roaming = 1;
				else
					GG_Status.Flag.Bits.Roaming = 0;
				RetStatus = OK;
				VehicleStatus.VS5.Bits.MobilSignalStatus = 1;
			}
			else
			{
				VehicleStatus.VS5.Bits.MobilSignalStatus = 0;
			}
		break;
		///////////////	// ///////////////
		case eAT_CNUM:        	//   +CNUM: "","13539961594",129, 
			
			pS = MyStrStr((char*)atBuffer,(char*)"+CNUM\0");
			if( (pS != NULL)&&(pOK!=NULL))
			{
				if(pRet != NULL)
				{
					//pS += 7;		  // UBLOX
					pS += 11;		  // M35
					for(j=0,i=0;i<20;i++)
					{
						c = *(pS+i);
						if( (c>='0')&&(c<='9') )
						{
							*(pRet+j) = c;
							j++;
						}
					}
					if(j==11)	RetStatus = OK;
				}				
			}
		break;

		case eAT_QCCID:        	// \r\n+CCID: 89860080190739733825\r\n   \r\nOK\r\n
			
			pS = MyStrStr((char*)atBuffer,(char*)"+CCID\0");
			if( (pS != NULL)&&(pOK!=NULL))
			{
				if(pRet != NULL)
				{
					pS += 7;
					for(j=0,i=0;i<20;i++)
					{
						c = *(pS+i);
						if( (c>='0')&&(c<='9') )
						{
							*(pRet+j) = c;
							j++;
						}
					}
					if(j==20)	
					{
						DFE.Write(EE_SIM_CARD_ICCID, j, pRet);
						NOP(); NOP();NOP();NOP();NOP();NOP();

						RetStatus = OK;
					}
				}				
			}
		break;
		
		case eAT_CGSN:        	// \r\n357852037769893\r\n   \r\nOK\r\n
			
			pS = MyStrStr((char*)atBuffer,(char*)"\r\n\0");
			if( (pS != NULL)&&(pOK!=NULL))
			{
				if(pRet != NULL)
				{
					pS += 2;
					for(j=0,i=0;i<20;i++)
					{
						c = *(pS+i);
						if( (c>='0')&&(c<='9') )
						{
							*(pRet+j) = c;
							j++;
						}
					}
					if(j==15)
					{
						DFE.Write(EE_GSM_IMEI, j, pRet);
						NOP(); NOP();NOP();NOP();NOP();NOP();
						RetStatus = OK;
					}
				}
			}			
			
		break;
		case eAT_CGMR:        	// \r\n07.60.00\r\n   \r\nOK\r\n

			if(pOK!=NULL)
			{
				pS = MyStrStr((char*)atBuffer,(char*)"\r\n\0");
			  	pS += 2;
			   pOK = MyStrStr(pS,(char*)"\r\n\0");
				for(j=0;pS<pOK;pS++)
				{
					c = *pS;
					*(pRet+j) = c;
					j++;
				}
				RetStatus = OK;
			}

			
		break;

		case eAT_CIMI:		  // \r\n460002037769893\r\n   \r\nOK\r\n
		 pS = MyStrStr((char*)atBuffer,(char*)"46000\0");
		 if( (pS != NULL))
		 {  GG_Status.Registration = TRUE;
			RetStatus = OK;
		 }
		 else
		 {
		 	//GG_Status.Registration = 0;
		 }
		break;

		case eAT_D:        	// \r\nOK\r\n
			if(pOK!=NULL)                                 
			{
			  //KeyDisp.SendDataToLCM_YH(0xa7,0,(uchar*)NULL);		// 摘机后通知手柄
			  KeyDisp.SendDataToLCM_YH(eLCM_U_RING_OFF,0,(uchar*)NULL);		// 摘机后通知手柄
			  RetStatus = OK;
			}
			pS = MyStrStr((char*)atBuffer,(char*)"NO CARRIER\0");		// 连接不成功
			if(pS != NULL)		
			{
				Uart.puts("NoC HON\r\n\0");
				PhoneCall.GsmHang = TRUE;
			}
	
			pS = MyStrStr((char*)atBuffer,(char*)"BUSY\0");		// 线路忙碌
			if(pS != NULL)		
			{
			   Uart.puts("Busy HON\r\n\0");
				PhoneCall.GsmHang = TRUE;
			}
			
			pS = MyStrStr((char*)atBuffer,(char*)"NO ANSWER\0");		// 线路忙碌
			if(pS != NULL)		
			{
			   Uart.puts("No Answer HON\r\n\0");
				PhoneCall.GsmHang = TRUE;
			} 

		break;
		
		////////////////////////// 查询对方已收到的数据
		case eAT_QISACK:
			
			if(pOK!=NULL)				// QISACK:<Sent>,<Ack>,<nAck>
			{
				SetBufferValue( Tp.Buffer,16,0);
				pS = MyStrStr((char*)atBuffer,(char*)"+QISACK:\0");
				if(pS != NULL)
				{					
					j = 0;
					Len = strlen( (char*)pS);
					for(i=0;i<Len;i++)
					{
						c = *(pS+i);
						if( (c==',')||(c==':') )							
						{
							Tp.B32[j++] = Str2Int( (pS+i+1) );							
						}
					}
					memmove(pRet,Tp.Buffer,16);
					return OK;
				}
			}
			
		break;

		/////////////////////////////		
		default:
		   
		
		break;
	}

	GSM.DisposeCalling((uchar *)atBuffer);	// 处理来电

	AT_Command_Busing = FALSE;

	if (TD.GPRS == tGPRS_AT_ACK_CHECK)			   // for debug
	{
		LPC_UART0->THR = 'B'; 	LPC_UART0->THR = '2'; 
	}

	return  RetStatus;
}













/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */


