/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$									���ݹ�ҵ���ӿƼ����޹�˾
$									
$                       	http://www.guoyee.com		 Email: guoyegps@126.com	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
�ļ���:   gsm_send.cPP
����������GSM��������
���뻷����
Ŀ��CPU: 
���ߣ�
����ʱ�䣺
����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/	
#define			AT_CMD_GLOBALS			1
#include			"ATCmd.h"
#include		   ".\UserSys.h"
#include			"GSM_GPRS.h"

 
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	AT����
*/
//uchar		const		atrLen[MAX_AT_CMD_NUM] = {7,6,7,7,7,6,6,9,6,9,14,8,14,6,6};

// // �˱�˳�������ATCmd.h��AT����ö�ٶ�Ӧ���������
char		const		AT_CMD_Tab[MAX_AT_CMD_NUM][32] = 
{	
	{"AT\0"},							// 0, 
	{"AT\0"},							// 0, 
	{"ATE0\0"},							// 0, 						// 
	{"AT+IFC=\0"},			// Ӳ������
	{"AT+QIHEAD=0\0"},			// ��������ͷ����Ϣ
	{"AT+QIPROMPT=0\0"},		// ���÷����������'>'		
	/////////////////////////////
	{"AT+COPS?\0"},					// �����Ӫ�̱�ʶ
	{"AT+QIMUX=1\0"},			//	���ö�·TCP/UDP����		1�� �����·		0��ֻ����һ·
	{"AT+QICSGP=1,\"\0"},				// ��Ӫ�����磬����ʹ��"CMNET"
	{"AT+QIFGCNT=0\0"},			//  ����ǰ�ó���	 
	{"AT+QIREGAPP\0"},			// ע��TCP/IPЭ��ջ
	{"AT+QIACT\0"},      		// ����GPRS����
	{"AT+QIDEACT\0"},			// �ر�GPRS	����
	{"AT+QINDI=\0"},			// �����Ƿ񻺴���յ���GPRS���ݣ� 1 ��ʹ�û���   0�������棬ֱ�����������	
	{"AT+QICLOSE=\0"},				// �ر�Socket,����׷��ͨ����
	{"AT+QIOPEN=\0"},					// ����Socket TCPͨ��	  �˴��붨���eAT_QIOPEN_TCP���ж�Ӧ
	{"AT+QIOPEN=\0"},					// ����Socket UDPͨ��	  �˴��붨���eAT_QIOPEN_UDP���ж�Ӧ
	{"AT+QISTAT\0"},				// ��ѯGSM״̬
	{"AT+QILOCIP\0"},			// ��ѯGSMģ�鱾��IP��ַ	
	{"AT+QIMODE=0\0"},           // ��͸��ģʽ	 1: ͸��  	0�� ��͸��
	{"AT+QIDNSGIP=\0"},					// ������������IP��ַ
	{"AT+QIDNSIP=0\0"},			//   ����ͨ��IP��DNS����	0��ͨ��IP����	1��ͨ��DNS����
	/////////////////////////////
	{"AT+CMGF=0\0"},					// =0: PDU; =1: TEXT
	{"AT+CMGF=1\0"},					// =0: PDU; =1: TEXT
	("AT+CSCS=\"GSM\"\0"),		// ѡ��TE�ַ���
	{"AT+CPMS?\0"},		// ��ѯ����Ϣ�洢��		 	//{"AT+CPMS=\"SM\"\0"},		// ѡ�����Ϣ�洢��
	{"AT+CMGD=0,2\0"},				// ɾ���Ѷ����ѷ��Ͷ���	 {"AT+CMGD=0,4\0"}, // ɾ�����ж���
	{"AT+CNMI=2,2,0,0,0\0"},	// ����Ϣ��������			 // //	{"AT+CNMI=0,0\0"},				// ������ʾ
	{"AT+CSCA=\0"},					// ���ö��ŷ������ĺ���
	{"AT+CSCA?\0"},					// ��ѯ���ŷ������ĺ���
	/////////////////////////////
	{"AT+CLIP=1\0"},					// ������ʾ	
	{"AT+CRC=1\0"},
	{"ATS0=2\0"},					  	// ���������Զ�Ӧ��
	{"ATD\0"},							// ��绰,����׷�ӵ绰����
	{"ATDL;\0"},						// �ز�����Ǵ�ȥ��
	{"ATH\0"},							// �һ�
	{"ATA\0"},							// ժ��
	{"AT+CLVL=\0"},					// �ܻ�����
	{"AT+CLVL?\0"},	  				// ��ѯ�ܻ�����
	/////////////////////////////
	{"AT+CPIN?\0"},					// SIM��״̬
	{"AT+CSQ\0"},						// �ź�����
	{"AT+CREG?\0"},					// ע������״̬	
	//////////////////////////// 
	{"AT+CNUM\0"},		 				// SIM���绰���루����Ӫ��֧�֣�
	{"AT+QCCID?\0"},				  	// ��SIM������
	{"AT+CGSN\0"},						// ��ͨѶģ��IEMI��
	{"AT+CGMR\0"},						// ��ͨѶģ��汾��	
	{"AT+CIMI\0"},						// ����Ӫ�̹���ID	
	{"AT+QMIC=0,12\0"},			 // ������˷������		
	{"AT+QISACK=\0"},	
	{"\0"},	
};
	

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������
   ��ڲ���������š���������ָ�롢��������ָ��
   ���ڲ������ɹ�����OK�����󷵻������
   ȫ�ֱ���:   
   ����ʱ�䣺
   �޸�ʱ�䣺
	ע����֧�����������ATָ���ʹ��ר�ú��������շ����š��շ�GPRS����
*/ 
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
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

	strcpy(atBuffer,(char*)&AT_CMD_Tab[Cmd][0]);	 	// ȡATָ��
	if(pData!=NULL)	strcat(atBuffer,(char*)pData);							// ׷������
	strcat(atBuffer,"\r\n\0");								// ׷�ӻس�����
	
	if((TD.GPRS == tGPRS_GSM_ALL_DATA)||(Cmd==eAT_QIOPEN_TCP)||(Cmd==eAT_QIDNSGIP) )
		Uart.puts(atBuffer);
		
	Uart1.puts(atBuffer);										// ����ATָ��
	SetBufferValue((uchar*)atBuffer,128,0);
	Len = Uart1.WaitRecBuffer((30*100),128,(uchar*)atBuffer);	  		// �ȴ�����

	if(Len==0)	{ AT_Command_Busing = FALSE; return Cmd; }	  // ��Ӧ�������ж���������

	MOBILE_POWER_ON;
	//TC.GG_Rec = TC.GG_Send = 0;
	
	pS = MyStrStr((char*)atBuffer,(char*)"ERROR\0");
	pOK = MyStrStr((char*)atBuffer,(char*)"OK\r\n\0");

	if(TD.GPRS == tGPRS_GSM_ALL_DATA)	Uart.puts(atBuffer);
	
	//////////////////////////////////////
	RetStatus = Cmd;				// Ĭ�Ϸ�OK
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
		//case eAT_D:           	// \r\nOK\r\n   �޷ֺ�Ҳ�ܴ�ͨ���ܻ���������ʾ"����ͨ��"
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
			
		break;					// �������ֻ����OK��ERROR�������ݣ����账��
			
		/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
		*/

		case eAT_QIDNSGIP:		  		  
			if ((pOK != NULL) && (NULL == MyStrStr((char*)atBuffer,(char*)"ERROR")))  // M35 
			{
				os_dly_wait(150);
				SetBufferValue((uchar *)atBuffer, 64,0);
				Uart1.WaitRecBuffer((30*100),64,(uchar*)atBuffer);	  		// M35ģ�鷵�سɹ���־�Ƚ���,��Ҫ�ٴ�ȷ��

				if(pRet == NULL)   break;

				pS = MyStrStr((char *)atBuffer, (char *)"\r\n\0");
				if (pS != NULL)				  // �ѻس������滻�ɿո�
				{
					pOK = MyStrStr((char *)&atBuffer[2], (char *)"\r\0");
					if (pOK != NULL)				  // �ѻس������滻�ɿո�
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
			pS = MyStrStr((char*)atBuffer,(char*)"CONNECT OK\0");				 // TCP ���ӳɹ�
			if (pS != NULL)
			{
				NetConnectCnt = 0;
				RetStatus = OK;
			}
			else
			{
				os_dly_wait(150);
				SetBufferValue((uchar *)atBuffer, 64,0);
				Uart1.WaitRecBuffer((30*100),64,(uchar*)atBuffer);	  		// M35ģ�鷵�سɹ���־�Ƚ���,��Ҫ�ٴ�ȷ��
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
				if(SearchArrayCount(Len, (uchar *)atBuffer, 1,(uchar *)".") >= 3)  // �ҵ�IP���� 
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
			  //KeyDisp.SendDataToLCM_YH(0xa7,0,(uchar*)NULL);		// ժ����֪ͨ�ֱ�
			  KeyDisp.SendDataToLCM_YH(eLCM_U_RING_OFF,0,(uchar*)NULL);		// ժ����֪ͨ�ֱ�
			  RetStatus = OK;
			}
			pS = MyStrStr((char*)atBuffer,(char*)"NO CARRIER\0");		// ���Ӳ��ɹ�
			if(pS != NULL)		
			{
				Uart.puts("NoC HON\r\n\0");
				PhoneCall.GsmHang = TRUE;
			}
	
			pS = MyStrStr((char*)atBuffer,(char*)"BUSY\0");		// ��·æµ
			if(pS != NULL)		
			{
			   Uart.puts("Busy HON\r\n\0");
				PhoneCall.GsmHang = TRUE;
			}
			
			pS = MyStrStr((char*)atBuffer,(char*)"NO ANSWER\0");		// ��·æµ
			if(pS != NULL)		
			{
			   Uart.puts("No Answer HON\r\n\0");
				PhoneCall.GsmHang = TRUE;
			} 

		break;
		
		////////////////////////// ��ѯ�Է����յ�������
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

	GSM.DisposeCalling((uchar *)atBuffer);	// ��������

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


