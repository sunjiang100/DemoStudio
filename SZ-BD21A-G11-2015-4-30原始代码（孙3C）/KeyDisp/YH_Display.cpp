/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												���ݹ�ҵ���ӿƼ����޹�˾
$										Shawyoo Electronics (GuangZhou) R&D Center
$                       	http://www.shawyoo.com		 Email: autoele@qq.com
$								 QQȺ��9069471/9069433/9524253		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
	�ļ���: 	 SMS.cpp
	����������SMS��غ���
	���뻷����
	Ŀ��CPU: 
	���ߣ�
	����ʱ�䣺
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  

#include			"KeyDisp.h"

#include			"..\UART\UART0.H"
#include			"..\GSM_GPRS\GSM_GPRS.h"
#include			"..\GPS\GPS.h"

#include			"..\Main.h"



extern	uchar			DBusing;
extern	uchar			DispBuffer[];//	1024 	__attribute__((at(0x7fe03800)));


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	
/*
0F 03 30 42 31 33 38 32 38 37 39 34 31 36 31 4D F0 FF
	03^30^42^31^33^38^32^38^37^39^34^31^36^31=4D
*/

//uchar		const		TestCmdData[] = {0x0F,0xA4,0x33,0x44,0x4E,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x30,0x43,0x00,0x2A,0x00,0x47,0x00,0x59,0x00,0x2C,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x2C,0x00,0x52,0x00,0x38,0x00,0x31,0x00,0x2C,0x00,0x31,0x00,0x32,0x00,0x33,0x00,0x34,0x00,0x35,0x00,0x36,0x00,0x23,0xAE,0x27,0xF0,0xFF};

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			KeyDispCls::YH_ReceiveProcessData(void)
{
	uchar		Cmd,s,s1;
	uint16	i,j,DataLen,Len;
	uint32	tl,ID;
	uchar		tBuffer[256];
	
	//memmove(U0_RecBuffer,TestCmdData,sizeof(TestCmdData));
	
	Cmd = U0_RecBuffer[1];


	DataLen = HexCharToByte((char*)&U0_RecBuffer[2]);		// 24 01 0c 31 32 33 34 35 36 37 38 39 30 28 ff

	s = CalXorSum((DataLen+3),&U0_RecBuffer[1]);

	if( s != U0_RecBuffer[(DataLen+4)] )	
	{
		SendDataToLCM_YH(0xfe,0,tBuffer);  	// �������
		return;
	}

	
	SetBufferValue((uchar*)tBuffer,256,0);
	///////////////////////////////////////////////////
	switch(Cmd)
	{
		//////////////////////////////////////////////// 
		case 0x01:	 // ���ֽ�ָ��
		{
			Cmd = U0_RecBuffer[4];
			switch(Cmd)
			{
				case 0x01:	break;
				case 0x02: 	PhoneCall.CalledAnswer = TRUE;	break;
				case 0x03:	PhoneCall.KeyHang = TRUE;			break;

				case 0x06:	// ��������
					PhoneCall.DialCnt = 0;
					PhoneCall.Length = DataLen;
					memmove(PhoneCall.Buffer,sCenterNum,strlen(sCenterNum));	
					PhoneCall.DialFlag = TRUE;	
				break;
				default:		break;
			}
			DataLen = 0;
		}
		break;

		//////////////////////////////////////////////// 
		case 0x02:			//  ����
			PhoneCall.DialCnt = 0;
			PhoneCall.Length = DataLen;
			memmove(PhoneCall.Buffer,&U0_RecBuffer[LCM_FRAME_DATA_YH],DataLen);			
			PhoneCall.DialFlag = TRUE;
			DataLen = 0;
		break;

		//////////////////////////////////////////////// 
		case 0x03:			// 
			DataLen = 0;
			
				
		break;


		/////////////////////////////////////////////////
		case 0x0c:
			if( (U0_RecBuffer[LCM_FRAME_DATA_YH]=='0')&&(U0_RecBuffer[LCM_FRAME_DATA_YH+1]=='*')  )
			{
				os_dly_wait(200);
				if( (U0_RecBuffer[LCM_FRAME_DATA_YH+8]=='*')&&(U0_RecBuffer[LCM_FRAME_DATA_YH+9]=='*') )
					s1 = 0;
				else if( (U0_RecBuffer[LCM_FRAME_DATA_YH+8]=='#')&&(U0_RecBuffer[LCM_FRAME_DATA_YH+9]=='#') )
					s1 = 1;
				else
				{
					s1 = 0xaa;
				}
				tBuffer[0] =  U0_RecBuffer[LCM_FRAME_DATA_YH+2];
				tBuffer[1] =  U0_RecBuffer[LCM_FRAME_DATA_YH+3];
				tBuffer[2] =  U0_RecBuffer[LCM_FRAME_DATA_YH+4];
				tBuffer[3] =  U0_RecBuffer[LCM_FRAME_DATA_YH+5];
				tBuffer[4] =  U0_RecBuffer[LCM_FRAME_DATA_YH+6];
				tBuffer[5] =  U0_RecBuffer[LCM_FRAME_DATA_YH+7];
				tBuffer[6] =  U0_RecBuffer[LCM_FRAME_DATA_YH+8];
				tBuffer[7] =  U0_RecBuffer[LCM_FRAME_DATA_YH+9];
				tBuffer[8] =  0;		
				ID = Str2Int((char*)tBuffer);
				if(  (ID==DriverID[0])||(ID==DriverID[1])||(ID==DriverID[2])||(ID==DriverID[3])\
						||(ID==DriverID[4])||(ID==DriverID[5])||(ID==DriverID[6])||(ID==DriverID[7])\
						||(ID==99900011L)||(ID==99900012L)||(ID==99900013L)||(ID==99900014L) )		
				{
					DriverLoginLogout(s1,ID);
				}
				else
				{
					strcpy((char*)tBuffer,(uchar*)"Gδ��Ȩ���룡\0");
					SendDataToLCM_YH(0xa8,13,(uchar*)tBuffer);					
					strcpy((char*)tBuffer,sNowTimeString);
					strcat((char*)tBuffer,",ER_ID,\0");		 		// �������������ݴ���
					Len = strlen((char*)tBuffer);
					memmove((uchar*)&tBuffer[Len],(uchar*)&U0_RecBuffer[LCM_FRAME_DATA_YH],DataLen);
					strcat((char*)tBuffer,"#\0");
					GPRS.TH_NewDataPack((uchar*)"I2\0",(uchar*)tBuffer);
				}				
			}

		break;

		//////////////////////////////////////////////// 
		case 0xe0:			//   ������ѯ������Ҳ��e0ָ��
			DataLen = 0;
			
		break;

		//////////////////////////////////////////////// 
		case 0xa4:			//   �յ���Ϣ����Ϣ�а��������ֱ��ִ��������ڲ�ѯ�����ò���
			// 0F A4 33 63 4E 31 32 33 34 35 36 37 38 39 30 31 32 33 34 35 36 37 38 39 30 
			// 43 00 2A 00 47 00 59 00 2C 00 30 00 30 00 30 00 2C 00 52 00 38 00 31 00 2C 00 31 00 32 00 33 00 34 00 35 00 36 00 23 AE F0 
			memmove(tBuffer,U0_RecBuffer,DataLen+5);			
			DataLen = IgnoreSpecifyCharacter(0,DataLen-20,(uchar*)&U0_RecBuffer[25]);
			HY_DispCmd = TRUE;
			s = GPRS.ProcessReceiveData(DataLen,(char*)&U0_RecBuffer[25]);
			HY_DispCmd = 0;			
			if(s==0)
			{
				GPRS.PushAlignment(DataLen+5,tBuffer);	  // ���͵��������
				LcmDataToServer = TRUE;
				s = WaitLcmDataToServer(500);	
				if(s==OK)
					tBuffer[0] = 0x17;
				else
					tBuffer[1] = 0x18;
				SendDataToLCM_YH(1,1,tBuffer);	
			}
			DataLen = 0;			
		break;

		//////////////////////////////////////////////// 
		case 0xac:			// ��������������͸������
			
			GPRS.PushAlignment(DataLen+5,U0_RecBuffer);	  // ��������ֱ�ӷ��͵��������
			LcmDataToServer = TRUE;
			s = WaitLcmDataToServer(500);	
			if(s==OK)
				tBuffer[0] = 0x17;
			else
				tBuffer[1] = 0x18;
			SendDataToLCM_YH(1,1,tBuffer);
			DataLen = 0;
		break;   


		case 0xc0:		// ��¼
			tl = Str2Int((char*)&U0_RecBuffer[LCM_FRAME_DATA_YH]);
			s = DriverLoginLogout(0,tl);
			if(s==OK)		tBuffer[0] = 0x30;	
			else				tBuffer[1] = 0x31;
			SendDataToLCM_YH(0x21,1,tBuffer);
			DataLen = 0;
		break;

		case 0xc1:		// �˳�
			tl = Str2Int((char*)&U0_RecBuffer[LCM_FRAME_DATA_YH]);
			s = DriverLoginLogout(1,tl);
			if(s==OK)		tBuffer[0] = 0x30;	
			else				tBuffer[1] = 0x31;
			SendDataToLCM_YH(0x22,1,tBuffer);
			DataLen = 0;
		break;

		case 0xd0:		// ������ť
			
			VehicleStatus.VS6.Bits.CallHelpAlarm = 1;
			DataLen = 0;
		break;


		default:	DataLen = 0;		break;
	}

	if(DataLen)
	{
		SendDataToLCM_YH(Cmd,DataLen,tBuffer);
	}

}











/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			KeyDispCls::SendDataToLCM_YH(uchar Cmd,uchar Len,uchar *pData)
{
	uchar		Xor;
	while(DBusing)	{ os_dly_wait(1);	}
	DBusing = TRUE;

	DispBuffer[0] = 0x0f;
	DispBuffer[1] = Cmd;
	DispBuffer[2] = HalfByteToHexChar(Len>>4);
	DispBuffer[3] = HalfByteToHexChar(Len&0x0f);
	if(Len)	
		memmove(&DispBuffer[4],pData,Len);
	Len += 3;
	Xor = CalXorSum(Len,&DispBuffer[1]);
	Len ++;
	DispBuffer[Len] = Xor;	Len++;
	DispBuffer[Len] = 0xf0;	Len++;
	DispBuffer[Len] = 0xff;	Len++;
	Uart.putb(1,Len,DispBuffer);
	DBusing = 0;
}






/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������
   ��ڲ�������
   ���ڲ�������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			KeyDispCls::SendHeartDataToLCM_YH(void)
{
	uchar		tBuffer[16];
	//0F 0D 30 31 00 0C F0 FF
	tBuffer[0] = 0x0f;		tBuffer[1] = 0x0d;	tBuffer[2] = 0x30;
	tBuffer[3] = 0x31;		tBuffer[4] = 0x00;	tBuffer[5] = 0x0c;
	tBuffer[6] = 0xf0;		tBuffer[7] = 0xff;
	Uart.putb(1,8,tBuffer);	
}

















/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */  

