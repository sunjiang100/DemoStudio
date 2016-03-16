/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 		  
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
	�ļ���: 	 GPRS.c
	����������GRPS��غ���
	���뻷����
	Ŀ��CPU: 
	���ߣ�
	����ʱ�䣺
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					

// ע��// ��ҵ�ڲ�����ľ�γ�ȱ�JT808��10���������þ�γ�ȵ�ʱ��Ҫ����10

#define			JT808_GLOBALS		1
#include			"JT_T808.h"
#include			"GSM_GPRS.h"

#include		"..\Common\GB_T19056.h"
#include		"..\Common\FLASH_Addr.h"
#include		".\UserSys.h"
#include		"..\GPS\GPS.h"
#include		"..\Speed\Speed.h"
#include		"ATCmd.h"
#include		"..\CAN\CAN.h"
#include		"..\Uart\Uart2.h"
#include		"..\Record\RecordModel.h"

	
//uchar		SEND_FLAG = 5;	 
//uchar		AUTH_OK_FLAG = 0;
extern		uchar		TakePhoneAckFlag;

uchar		 	jt_recv_flag = 0;	 	  // bit0: 	Ϊ1, �յ�ע��Ӧ��ɹ�   bit1:	 Ϊ1�� ��Ȩ�ɹ�    bit2: Ϊ1���յ�����Ӧ��  bit3: Ϊ1���յ�λ���ϱ�Ӧ��
uchar			jt_auth_ok_flag = 0;
uchar			ter_or_veh_reg_result;	 // �ն�ע��Ӧ����


uchar 		jt_save_pos_flag;                 // ����λ����Ϣ��־		

uint16		jt_send_cmd_flag;						// �ѷ��������־	 
uint32		jt_media_id;                     // ��ý��ID
uchar			jt_pic_send_flag;                // ͼƬ����ʹ�ܱ�־ 
uchar			jt_dis_reg_flag;						// �ն�ע����־
uchar			PicRetryOrMulPic;					   // ����ͼƬ�ش����� 1  ; ���ʹ洢��ý�壨���ţ�: 2
uchar 		JTDelPicFlag = 0;                    // ɾ��ͼƬ��־



#define		TOTAL_EVT_INFO_ID_NUMBER		0x64
uchar		JT808_EventID[TOTAL_EVT_INFO_ID_NUMBER]={0};			// ��ǰ��ĵ�Ԫ�洢��ǰ����
uchar		JT808_InfoID[TOTAL_EVT_INFO_ID_NUMBER]={0};




static		PIC_STRUCT		cPic;
uchar			JT_ManulCameraID = 0;				// ��ʱ����ͨ��ID


char		jt_auth_code[LEN_AUTH_CODE];		// ��Ȩ��

extern  	uchar			*Cam_cBuffer;
extern  	uchar			*CamBuffer;
extern  	uchar        CameraUsing,OnlyUseBD_GPS;
extern	uchar 		Connect_Create_USer_Center_Or_Main_Center(uchar Type, uchar Wait);

extern	uchar			IC_CardAuthFailure;									  

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ� get_position_info
   ���������� ��ȡλ����Ϣ��
   ��ڲ����� pos_data: ����ָ��      AckLen: 0, û��Ӧ�����  1��Ӧ�����
   ���ڲ����� pos_data: λ������
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
ע��

����Ĭ��ʱ��Ϊ20��
02 00000020 04 00000000 00000029 04 00000014 

*/

uchar GPRSCls::get_position_info(uchar Last,uchar *pos_data, uchar AckLen)
{
	uchar 	Len;
	union
	{
		uchar		Buffer[4];
		uint16	B16[2];
		uint32	B32;
	} Tp;

	// λ�û�����Ϣ
	
	if(Last>=0x80)
	{
		Len = 0;				// ֻȡ������Ϣ
	}
	else
	{
		Len = LEN_JT808_GPS_PACK;
		if(Last)		//  �����Ч��λ��Ϣ
	  		memmove(pos_data, (uchar*)&GPS.LastJTPack, LEN_JT808_GPS_PACK);	
		else			// ��ǰ��Ϣ�������Ƿ�λ
	  		memmove(pos_data, (uchar*)&GPS.JTPack, LEN_JT808_GPS_PACK);  
	
				////// ���±���λ��״̬λ
		Tp.B32 = ~JT808_Alarm_Word.Shield;		// ����������
		
		pos_data[0] = JT808Flag.VS8.Byte&Tp.Buffer[3];
		pos_data[1] = JT808Flag.VS7.Byte&Tp.Buffer[2];
		pos_data[2] = JT808Flag.VS6.Byte&Tp.Buffer[1];
		pos_data[3] = JT808Flag.VS5.Byte&Tp.Buffer[0];
		
		pos_data[4] = JT808Flag.VS4.Byte;
		pos_data[5] = JT808Flag.VS3.Byte;
		pos_data[6] = JT808Flag.VS2.Byte;
		pos_data[7] = JT808Flag.VS1.Byte;

		if (TD.GPRS==tGPRS_TMP_TEST)
		{
			Uart.puts("Alarm Status Test Data:");
			Uart.putb(1,8,pos_data);
			Uart.putb(1,8,(uchar*)&JT808ExtenVsFlag);
			Uart.putb(1,4,(uchar*)&JT808_Alarm_Word.Shield);		
			Uart.puts("\r\n\0");
		}

	}	

	/////////////////////////////////////////////////////////////////////////////// ������Ϣ
	if(UseGPS_Mil==0)
		Tp.B32 = SystemLog.GPS.TotalMileage/100;		// �ڲ�����λ���ף��ϴ���λ0.1km
	else
		Tp.B32 = SystemLog.VDR.SpeedSensorPulsCnt/VehiclePulseKV;		// GPS�������̵�λ����
	
	pos_data[Len++] = eBB_POS_ADD_MIL;
	pos_data[Len++] = 0x04;
	pos_data[Len++] = Tp.Buffer[3];
	pos_data[Len++] = Tp.Buffer[2];
	pos_data[Len++] = Tp.Buffer[1];
	pos_data[Len++] = Tp.Buffer[0];

	// add 2013.3  	
	pos_data[Len++] = eBB_POS_ADD_OIL;		   // ������ ��ʱû��ʵ�ִ˹��ܣ���0
	pos_data[Len++] = 0x02;
	pos_data[Len++] = 1;	   						// �������ֽ�
	pos_data[Len++] = 0xc4;	   						// �������ֽ�  

	//	
	pos_data[Len++] = eBB_POS_ADD_SPEED;		// ��¼���ϵ��ٶ�
	pos_data[Len++] = 0x02;
	Tp.B16[0] = uiSpeed;
	pos_data[Len++] = Tp.Buffer[1];	               
	pos_data[Len++] = Tp.Buffer[0]; 
	
	if( (InOrOutArea==eBB_IN_AREA)||(InOrOutArea==eBB_OUT_AREA) )
	{
		if(JT808Flag.VS7.Bits.InOutArea==1)		// ֻ�о���������Ҫ�˹�ȷ��
		{
			pos_data[Len++] = eBB_POS_ADD_MANUL_ALARM_EVENT_ID;  // ��Ҫ�˹�ȷ�ϱ����¼���ID
			pos_data[Len++] = 0x02;
			if(InOrOutArea==eBB_IN_AREA)			ManulAlarmEventID = 1;
			else											ManulAlarmEventID = 2;
			Tp.B16[0] = ManulAlarmEventID;
			pos_data[Len++] = Tp.Buffer[1];
			pos_data[Len++] = Tp.Buffer[0];
		}
	}
		

	//////////////////////////////////// Χ��������Ϣ
	if( (InOrOutArea==eBB_IN_AREA)||(InOrOutArea==eBB_OUT_AREA) )
	{
		pos_data[Len++] =  eBB_POS_ADD_INOUT_AREA_ROAD_ALARM;
		pos_data[Len++] = 0x06;
		pos_data[Len++] = InOutAreaType;	
		Tp.B32 = FenceOrRoadAlarmID;			  
		pos_data[Len++] = Tp.Buffer[3];
		pos_data[Len++] = Tp.Buffer[2];
		pos_data[Len++] = Tp.Buffer[1];
		pos_data[Len++] = Tp.Buffer[0];
		pos_data[Len++] = InOrOutArea;                // ����
	}

	// added 2013.3		 // �����Ϣ ������22���ֽ�
	pos_data[Len++] =  eBB_POS_ADD_EXTEN_VS_STATUS;		   // ��չ�����ź�
	pos_data[Len++] = 0x04;
	pos_data[Len++]	= JT808ExtenVsFlag.VS4.Byte;
	pos_data[Len++]	= JT808ExtenVsFlag.VS3.Byte;
	pos_data[Len++]	= JT808ExtenVsFlag.VS2.Byte;
	pos_data[Len++]	= JT808ExtenVsFlag.VS1.Byte;

	pos_data[Len++] =  eBB_POS_ADD_IO_STATUS;				// IO״̬λ
	pos_data[Len++] = 0x02;
	pos_data[Len++] = JT808IOStatusFlag.VS2.Byte;
	pos_data[Len++] = JT808IOStatusFlag.VS1.Byte;

	pos_data[Len++] =  eBB_POS_ADD_ANALOG;				 // ��·ģ����
	pos_data[Len++] = 0x04;	
   pos_data[Len++] = JT808Analog1 >> 8;            // ģ����3
	pos_data[Len++] = JT808Analog1 & 0xff;
   pos_data[Len++] = JT808Analog2 >> 8;            // ģ����2	   ����Ͷ�Ϊ����Դ��ģ����
	pos_data[Len++] = JT808Analog2 & 0xff;

	
	pos_data[Len++] = eBB_POS_ADD_GSM_SIGNAL;		   // ����ͨ�ŵ��ź�ǿ��
	pos_data[Len++] = 0x01;
    pos_data[Len++] = GG_Status.Signal;        
	
	pos_data[Len++] = eBB_POS_ADD_GNSS_SATELLITE_NUMS;	   // GNSS��λ������
	pos_data[Len++] = 0x01;
	if( (JT808Flag.VS3.Byte&0x0c)==0x0c)
		pos_data[Len++] = GPS.GPS_SatelNum + GPS.BD_SatelNum;
	else if(JT808Flag.VS3.Bits.UseGPS==1)
		pos_data[Len++] = GPS.GPS_SatelNum;
	else if(JT808Flag.VS3.Bits.UseBD==1)
		pos_data[Len++] = GPS.BD_SatelNum;

	pos_data[Len++] = eBB_POS_ADD_EXT_ALARM;
	pos_data[Len++] = 0x04;
	pos_data[Len++] = 0;	
	pos_data[Len++] = JT808ExtenVsFlag.VS7.Byte;
	pos_data[Len++] = 0;
	pos_data[Len++] = 0;

	
	
	Len += AckLen;
	return Len;
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ� save_or_send_position_info
   ���������� ���������·������ʱ��λ����Ϣ��Ӧ��ʱ�ĺ��б�����Ϣ��λ����Ϣ
   ��ڲ����� type: 0,  ��·������      1�� 	���б�����Ϣ  Isave: 1, ����  0, ����
   ���ڲ����� ��
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
ע��1ҳFlashֻ�洢1������

*/ 
uchar	 	GPRSCls::GetRegInfo(uchar *pData)
{
	union
	{
		uchar		Buffer[4];
		uint16	B16[2];
		uint32	B32;
	}Tp;
	uchar		Len;
	Len = 0;
	
	DFE.Read(EE_PROVINCE_REGION_ID, 2, Tp.Buffer);
	*(pData+Len) = Tp.Buffer[1];		Len++;
	*(pData+Len) = Tp.Buffer[0];		Len++;

	// ������
	
	DFE.Read(EE_PREFECTURE_REGION_ID, 2, Tp.Buffer);
	*(pData+Len) = Tp.Buffer[1];		Len++;
	*(pData+Len) = Tp.Buffer[0];		Len++;

	// ������ �� �ն�ID
	DFE.Read(EE_VENDOR_ID, 32, (pData+Len));	 
	#if (WUXI_TEST != 1)
	memmove( (pData+Len+25),(uchar*)&sOEM_ID[3],7);
	#endif

	Len += 32; 

	// ������ɫ
	DFE.Read(FM_CAR_COLOUR, 1, (pData+Len));
		 
	if (*(pData+Len) != 0)	   // 
	{    
		Len++;
		DFE.Read(FM_VEHICLE_CODE, 9, (pData+Len));	  // ���ƺ�
		Len += 9;
	}
	else		// ����0��ȡVIN��
	{
		Len++;
		DFE.Read(FM_VEHICLE_VIN, LEN_VEHICLE_VIN, (pData+Len));  // ����VIN��	 
		Len += LEN_VEHICLE_VIN;
	}
			
	return Len;
}
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
ע��

*/ 
//extern	char		SoftwareVersion[];
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
extern		uchar		OutOfRoadCnt;

void			GPRSCls::BBJT808_GPRS_UploadCmd(uint16 Cmd,uint16 SrcLen,uchar *pSrcData)
{		
	#define		LEN_TP_BUFFER			128
	union
	{
		 uint32     B32[24];
		 uint16     B16[48];
		 uchar		Buffer[LEN_TP_BUFFER];		 // ע�⣺tBuffer�Ŀռ�Ҫ�� 	LEN_POSITION_REPORT ��
	}Tp;
	union
	{
		uchar		Buffer[4];
		uint16	B16[2];
		uint32	B32;
	} Tp1;
	uchar		RandData;
	uint16	AckID=0,Len = 0, tmp, i, j, k;
	uint32   Addr;
	int s;
	//pJT808_MULTY_MEDIA_REPORT pMedia;
	//pJT808_GPS_PACK  pPos;
	pQUERY_TER_PRO	pTerPro;

	pMEDIA_SEARCH_STRUCT		pSearch;
	pSAVE_PIC_HEAD				pSaveHead;

	//#if (WUXI_TEST==1)
	//return;
	//#endif


	NOP();	NOP();	NOP();	NOP();
	Tp1.B32 = 0;
	while(UploadCmdBusing)	
	{
		os_dly_wait(1);
		if(++Tp1.B32>1000)
		{
			Uart.puts("GPRS�ϴ��������æ����ʱ���� ");
			return;
		}
	}
	UploadCmdBusing = TRUE;
	SetBufferValue(Tp.Buffer,128,0);
	
	////////////////////////////////////////////////////// ���������ݴ���
	switch(Cmd)
	{
		///////////////
		case	eBB_gCMD_TER_ACK:
		   Tp.Buffer[0] = AckBuffer.Index>>8;
			Tp.Buffer[1] = AckBuffer.Index;
			Tp.Buffer[2] = AckBuffer.MsgId>>8;
			Tp.Buffer[3] = AckBuffer.MsgId;
			Tp.Buffer[4] = ter_or_veh_reg_result;		   // ���
			
			Len = 5;
		break;

		///////////////
		case	eBB_gCMD_TER_HEARTBEAT:							// ����
			Len = 0;
			Tp.Buffer[0]	= '\0'; 
			AckID = Cmd;
		break;

		///////////////
		case	eBB_gCMD_TER_REG:					// �ն�ע��		

			AckID = Cmd;
			// modified 2013.5
			SetBufferValue(Tp.Buffer,90,0);
			
			Len = GetRegInfo(Tp.Buffer);

			Uart.puts(" �ն�ע�᣺");
			Uart.putb(1,Len,Tp.Buffer);

			ReSend.OverTime = ReSend.Times = 0;	  // Ӧ��ʱ�� 10s
			ReSend.PreOverTime = SendOVT.TCP_ACK_OverTime; 

		break;

		///////////////
		case	eBB_gCMD_TER_DEREG:		      // �ն�ע��
			Len = 0;
			Tp.Buffer[0]	= '\0';
			AckID = Cmd;
			ReSend.OverTime = ReSend.Times = 0;	  // Ӧ��ʱ�� 10s
			ReSend.PreOverTime = SendOVT.TCP_ACK_OverTime; 
			Uart.puts("��ƽ̨ע��\r\n");
		break;

		//////////////
		case	 eBB_gCMD_TER_AUTH:		  			// ���ͼ�Ȩ��
			DFE.Read(EE_AUTH_CODE, LEN_AUTH_CODE, &Tp.Buffer[0]);			
			Len = strlen((char*)Tp.Buffer);
			Uart.puts((char*)Tp.Buffer);			// ��ӡ������
			AckID = Cmd;
			ReSend.OverTime = ReSend.Times = 0;	  // Ӧ��ʱ�� 10s
			ReSend.PreOverTime = SendOVT.TCP_ACK_OverTime; 
	   break;

	  ///////////////
	   case	eBB_gCMD_TER_POS_REPORT:			// λ���ϱ�
			
			//Len = get_position_info(0,&Tp.Buffer[0], 0);
			Len = get_position_info(1,&Tp.Buffer[0], 0);	 //��һ��1��ʾlast����
			AckID = eBB_gCMD_TER_POS_REPORT;
	   break;

	  ///////////////
	   case	eBB_gCMD_TER_POS_QUERY_ACK:		// λ�ò�ѯӦ��
	  		Tp.Buffer[0] = AckBuffer.Index >> 8;
			Tp.Buffer[1] = AckBuffer.Index & 0xFF; 
			Len = get_position_info(0,&Tp.Buffer[2], 2);
	   break;

	   ///////////////
	  case	eBB_gCMD_TER_MEDIA_EVENT_UPLOAD:		  // ��ý���¼���Ϣ�ϴ�    

			NOP();	NOP();
			PDU_SMS.Ack = 0;          //������ǿ�ƴ�GPRSͨ������	�����SMSͨ���´�Ĳɼ�ָ����ݴ�GPRS����
			Tp.Buffer[0] = MultiMediaReport.MediaId >> 24;
			Tp.Buffer[1] = MultiMediaReport.MediaId >> 16;
			Tp.Buffer[2] = MultiMediaReport.MediaId >> 8;
			Tp.Buffer[3] = MultiMediaReport.MediaId >> 0;

		  	Tp.Buffer[4] = MultiMediaReport.MediaType;
			Tp.Buffer[5] = MultiMediaReport.MediaFormat;
			Tp.Buffer[6] = MultiMediaReport.MediaEvent;
			Tp.Buffer[7] = MultiMediaReport.MediaChannel;
			

			Len = 8;
			//jt_send_cmd_flag = Cmd;
	  break;
	  
	  ///////////////
	  case	eBB_gCMD_TER_MEDIA_DATA_UPLOAD:		      // ��ý�������ϴ�
			
			PDU_SMS.Ack = 0;          //������ǿ�ƴ�GPRSͨ������	�����SMSͨ���´�Ĳɼ�ָ����ݴ�GPRS����
			JtSendPic.Index = 0;
			jt_pic_send_flag = 1;
			//CameraUsing = 1;
			Protocol.GPRS[JT_PROTOCOL] = 1;  // ����Э��ʹ��	
			UploadCmdBusing = 0;
			return;                    // ���ú���������� Camera�����Զ��������õ�ͼƬ������ִ������Ĵ������

	  ///////////////
	  case	eBB_gCMD_TER_MEDIA_SERCH_ACK:			// ��ý�����ݼ���Ӧ��
	  	
       	while(CameraUsing)		{	os_dly_wait(1);	}
       	CameraUsing = TRUE;
       	
  		   PDU_SMS.Ack = 0;          //������ǿ�ƴ�GPRSͨ������	�����SMSͨ���´�Ĳɼ�ָ����ݴ�GPRS����
			SetBufferValue((uchar *)&CamBuffer[0], 1024, 0);
	
		  	CamBuffer[0] = AckBuffer.Index >> 8;		// Ӧ��0x8802����ˮ��
			CamBuffer[1] = AckBuffer.Index & 0xFF;
		
		   Len = 4;
			k = 0;		        // ��������	pSAVE_PIC_HEAD

			
		  	if (MulMeDa.SavedMediaSearch.MediaType == 0)
			{								
				for(Addr=FF_PIC_START_PAGE;Addr<FF_PIC_END_PAGE;Addr++)
				{
					DFF.PageRead(Addr,Cam_cBuffer);
					pSaveHead = (pSAVE_PIC_HEAD)&Cam_cBuffer[0];
					if( pSaveHead->HeadFlag == SAVE_PIC_HEAD_FLAG )			// JPEG��־
					{
						pSearch = (pMEDIA_SEARCH_STRUCT)&CamBuffer[Len];
						pSearch->MediaId		= pSaveHead->MediaId;
						pSearch->MediaType	= pSaveHead->MediaType;
						pSearch->Channel		= pSaveHead->MediaChannel;
						pSearch->Event			= pSaveHead->MediaEvent;
						memmove( (uchar*)&(pSearch->LocationInfo[0]), (uchar*)&(pSaveHead->LocationInfo[0]), LEN_JT808_GPS_PACK );						
						Len += LEN_MEDIA_SEARCH_STRUCT;
						Addr += pSaveHead->TotalPage;
						k++;
					}
					else if( pSaveHead->HeadFlag == 0xffffffff )
					{
						break;
					}
				}
			}	 

			CamBuffer[2] = k >> 8;		   // ��ý��������
			CamBuffer[3] = k & 0xFF;	
		 	JT808_DataPackPushAlignment(0,Cmd, Len, 0, (uchar *)&CamBuffer[0]);
			SetBufferValue( Cam_cBuffer,64,0);
			strcpy((char*)Cam_cBuffer," ��ý�����ݼ���Ӧ�� ");
			Int2Str( k, (char*)&Cam_cBuffer[20]);
			strcat((char*)Cam_cBuffer,"����Ϣ");
		   Uart.puts((char*)Cam_cBuffer);
			CameraUsing = 0;
			UploadCmdBusing = 0;
			return;

	  //break;	
	  ///////////////
	  case	eBB_gCMD_TER_DATA_TRAN_UPLOAD:			//   ����͸��
	     
			if( (pSrcData != NULL) )
	     	{
	     		JT808_DataPackPushAlignment(0,Cmd, SrcLen, 0, pSrcData);
	     		UploadCmdBusing = 0;
			  	return;
			}
			else
	     	{
		     if ((JT_tran_upload.ptype_and_data == NULL) || (JT_tran_upload.Len == 0))   
			  {
			  		UploadCmdBusing = 0;
			  	 	return;
				}
			 		
			  Len = JT_tran_upload.Len;
	
			  if (Len > 1024)		Len = 1024;
	
			  JT808_DataPackPushAlignment(0,Cmd, Len, 0, JT_tran_upload.ptype_and_data);
			  Uart.puts("�ϱ�--͸������ ");
			  UploadCmdBusing = 0;
			  return;
			}


	  case   eBB_gCMD_TER_VDR_DATA_UPLOAD:			 ////////////ȡ��¼����
			 
		  Len = 0;
		  if( pSrcData !=NULL)
		  {
	     		JT808_DataPackPushAlignment(0,Cmd, SrcLen, 0, pSrcData);
	     		UploadCmdBusing = 0;
			  	return;
			}
		
		break;
	
		case  eBB_gCMD_TER_DRIVER_INFO_REPORT:        // ��ʻԱ����ϴ�
		case	eBB_gCMD_I2C_DRIVER_INFO_REPORT:

		Len = 0;
		Tp.Buffer[Len++] = (IC_Card_Insert == TRUE)? IC_CARD_INSERT :	IC_CARD_IS_OUT	;
		DFE.Read(FM_RUN_TIME_START,6,&Tp.Buffer[Len]);
		Len +=6;
		if(IC_Card_Insert == TRUE)
		{
				Tp.Buffer[Len++] = IC_CardAuthFailure; 	// 0���ɹ�������ʧ��
		
				tmp = 0;
				DFE.Read(EE_JT_DRIVER_NAME_LEN, 1, (uchar*)&tmp);
				NOP(); 
				DFE.Read(EE_JT_DRIVER_NAME_LEN, 1+tmp, &Tp.Buffer[Len]);
				NOP(); 
				Len += tmp+1;
		
				DFE.Read(EE_JT_DRIVER_CARD_ID, 20, &Tp.Buffer[Len]);	//��ҵ�ʸ�֤��
				NOP(); 
				Len += 20;
				/*
				tmp = 0;
				DFE.Read(EE_JT_CERTIFICATE_NAME_LEN, 1, (uchar *)&tmp);
				NOP(); 
				DFE.Read(EE_JT_CERTIFICATE_NAME_LEN, 1+tmp, &Tp.Buffer[Len]);
				NOP(); 
				Len += tmp+1;
				*/
				Len++;
				Tp.Buffer[Len]=0x04;
				Len++;
				Tp.Buffer[Len]=0x58;
				Len++;
				Tp.Buffer[Len]=0x58;
				Len++;
				Tp.Buffer[Len]=0x58;
				Len++;
				Tp.Buffer[Len]=0x58;
				Len++;
				DFE.Read(EE_JT_DRIVER_CARD_VALID_DATE, 4, &Tp.Buffer[Len]);
				NOP(); 
				Len += 4;	
		  }//IC��������Ч

//		Tp.Buffer[Len++] = IC_CardAuthFailure; 	// 0���ɹ�������ʧ��
//
//		tmp = 0;
//		DFE.Read(EE_JT_DRIVER_NAME_LEN, 1, (uchar*)&tmp);
//		NOP(); 
//		DFE.Read(EE_JT_DRIVER_NAME_LEN, 1+tmp, &Tp.Buffer[Len]);
//		NOP(); 
//		Len += tmp+1;
//
//		DFE.Read(EE_JT_DRIVER_CARD_ID, 20, &Tp.Buffer[Len]);
//		NOP(); 
//		Len += 20;
//
//		tmp = 0;
//		DFE.Read(EE_JT_CERTIFICATE_NAME_LEN, 1, (uchar *)&tmp);
//		NOP(); 
//		DFE.Read(EE_JT_CERTIFICATE_NAME_LEN, 1+tmp, &Tp.Buffer[Len]);
//		NOP(); 
//		Len += tmp+1;
//
//		DFE.Read(EE_JT_DRIVER_CARD_VALID_DATE, 4, &Tp.Buffer[Len]);
//		NOP(); 
//		Len += 4;	
		
				
		break;	


	//
	//added 2013.4
	case	eBB_gCMD_TER_QUERY_PRO_ACK:					  // ��ѯ�ն�����Ӧ��
		SetBufferValue(Tp.Buffer, 96, 0);
		pTerPro = (pQUERY_TER_PRO)Tp.Buffer;
		pTerPro->Type = TER_SUPPORT_CAR_TYPE;
		DFE.Read(EE_VENDOR_ID, 32, &pTerPro->ProductorID[0]);  // ������ID���ն��ͺţ��ն�ID
		NOP();
		Len = 1 + 32;
		
		SetBufferValue((uchar *)&AckBuffer.Data[0], 32, 0);		
		DFE.Read(EE_SIM_CARD_ICCID, LEN_SIM_CARD_ICCID, (uchar *)&AckBuffer.Data[0]);	  // SIM������	898600821912A0247283
		s = strlen((char *)&AckBuffer.Data[0]);
		if (s > 0)
		{
			for (i=0,j=0;i<20;i += 2,j++)
			{
				k = AckBuffer.Data[i];
				pTerPro->SimCardICCID[j] = (((k>=0x30)&&(k<=0x39))?(k-0x30):(k-0x37)) << 4;
				k = AckBuffer.Data[i+1];
				pTerPro->SimCardICCID[j] |=  (((k>=0x30)&&(k<=0x39))?(k-0x30):(k-0x37));	   // ICCID���ǡ�0�� - ��9���� ��A' -'Z';
			}
		}
		Len += 10;
			
		Sys.GetHS_Version((char *)&pTerPro->LenHardVer, 1);  
		Len += (2 + pTerPro->LenHardVer + pTerPro->LenFirmwareVer);
		
		pTerPro->GNSSPro = TER_GNSS_PRO;
		pTerPro->GSMPro  = TER_GSM_PRO;
		Len += 2;			
		
	break;
	
	case	eBB_gCMD_TER_TAKE_PHOTO_ACK:		 // ����ͷ������������Ӧ��
			i = 0;
			Tp.Buffer[i++] = Plat_Serial_No>>8;
			Tp.Buffer[i++] = Plat_Serial_No;
			Tp.Buffer[i++] = ter_or_veh_reg_result;
			
			//Len = 5;
			if (ter_or_veh_reg_result == OK)
			{
				Tp.Buffer[i++] = 0x00;
				Tp.Buffer[i++] = TakePhotoAck.MediaNum;	// 

				for (j=0; j<TakePhotoAck.MediaNum; j += 4)
				{
					Tp.Buffer[i+j] = 0x00;
					Tp.Buffer[i+j+1] = 0x00;
					Tp.Buffer[i+j+2] = 0x00;
					Tp.Buffer[i+j+3] = TakePhotoAck.MediaID[j];
				}

				Len	= (i + j);
					
			}
			else
			{
				Len = i;
			}

		
	break;		

	//
	// added 2013.5
	/*case	eBB_gCMD_TER_CAN_DATA_UPLOAD:	  						// CAN���������ϴ�
			
		
	break; */

	case eBB_gCMD_TER_CREATE_USER:						  // ʹ��ǰ���������ٿ�����	
		
			
		if(pSrcData!=NULL)
		{
			AckID = Cmd;
			memmove( Tp.Buffer,pSrcData,SrcLen);
			Len = SrcLen;
						
			Uart.puts("����  Test Data:");
			Uart.putb(1,Len,Tp.Buffer);
			Uart.puts("\r\n");
			
			ReSend.OverTime = ReSend.Times = 0;	  // Ӧ��ʱ�� 10s
			ReSend.PreOverTime = SendOVT.TCP_ACK_OverTime;
		}
			 
	break;
	//

	// �¼�����
	case eBB_gCMD_TER_EVENT_REPORT:
		AckID = Cmd;
		tmp = JT808_EventID[0];	  									// ��ǰ��Ϣ�ܸ���
		if(tmp)
			Tp.Buffer[0] = JT808_EventID[1];
		else
			Tp.Buffer[0]	= *pSrcData;							// �¼�ID����ڲ���		
		Len = 1;
		Uart.puts("JT808_EventID ");
		Uart.putb(1,(JT808_EventID[0]+1),JT808_EventID);  	// 
	break;

	// ��Ϣ�㲥��/ȡ��
	case eBB_gCMD_TER_GET_INFO:
		AckID = Cmd;
		tmp = JT808_InfoID[0];						// ��ǰ��Ϣ������
		if(tmp)		
			Tp.Buffer[0]	= JT808_InfoID[1];	// ���ʹ�ã�ֻȡ��1����Ϣ
		else
			Tp.Buffer[0] = 0x01;
		Tp.Buffer[1]	= *pSrcData;  				// �㲥��ȡ����ʶ����ڲ���
		Len = 2;
		//Uart.puts("JT808_InfoID ");
		//Uart.putb(1,(JT808_InfoID[0]+1),JT808_InfoID);
	break;

	// �����˵�
	case eBB_gCMD_TER_ELE_WAYBILL_UPLOAD:
		

	break;

	//added 2013.6
	case   eBB_gCMD_TER_POS_INFO_MULTI_UPLOAD:						// ��λ��Ϣ�����ϴ�	  / �� ä������
		
		UploadCmdBusing = 0;
	return;
	//

	case	eBB_gCMD_TPMS_DATA_UPLOAD:							// ��̥��ѹ�����ϱ�
		if( (pSrcData != NULL) )
     	{
     		JT808_DataPackPushAlignment(0,Cmd, SrcLen, 0, pSrcData);
			Uart.puts("\r\n��̥�����ϱ�\r\n");
     		UploadCmdBusing = 0;
		  	return;
		}
	
	break;

	default: break;
		
	}

	JT808_DataPackPushAlignment(AckID,Cmd, Len, 0, Tp.Buffer);
	if (TD.GPRS==tGPRS_TMP_TEST) { Uart.puts("ͨ�ô���ϱ���ID:");	}	  		// ������ӡ����жϼ�Ȩ�ɹ���ų��ӡ�
	SetBufferValue(Tp.Buffer,64,0);
	IntToHexChar( Cmd, 4, (char*)Tp.Buffer);	
	strcat((char*)Tp.Buffer,",");
	tmp = strlen( (char*)Tp.Buffer);
	Int2Str( Len, (char*)&Tp.Buffer[tmp]);
	if (TD.GPRS==tGPRS_TMP_TEST) {Uart.puts((char*)Tp.Buffer); }
	UploadCmdBusing = 0;
	#undef		LEN_TP_BUFFER
}



void			GPRSCls::UnRegTerminal(void)
{
	uchar		tBuffer[LEN_AUTH_CODE];
	jt_auth_ok_flag = 0;
	jt_dis_reg_flag = 0;
  	GG_Status.Flag.Bits.FirstReg = 0;
  	GG_Status.Flag.Word = GG_FLAG_INIT_DATA;	// ע������������
  
  	NeedRegServer = ENABLE_FLAG;	
   DFE.Write(EE_NEED_REG_SERVER, 1, &NeedRegServer);	 
  	NOP();NOP();       

	SetBufferValue(tBuffer,LEN_AUTH_CODE,0);
	SetBufferValue(  (uchar*)jt_auth_code, LEN_AUTH_CODE,0);
   DFE.Write(EE_AUTH_CODE, LEN_AUTH_CODE,tBuffer);			//  ��ռ�Ȩ��

		 uLED.MainMenuCnt = DEFAULT_DISP;
//  	uLED.MainMenuCnt = Logout_OK_MENU;
//  	sKEY_RM.LongTimeNoKey = 20*5;
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ��������������GPRS��������	��Ӧ��
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
   �޸�ʱ�䣺
ע��

*/ 
extern		uchar			NeedTakePhotoAck;
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uchar			GPRSCls::BBJT808_GPRS_CmdExe(uint16 Length)
{

	uchar		s, tel,tmp, k, result = 0, isPack,   ComtAck = 0, n, m,Cmd2;//,VirtualAuth=0;
	uchar 	*ptr;
	uint16	DataLen, MsgId, MsgAttr, Index, DataBlockAddr;//DataBlockAddr;
	uint32	i=0,j,Len,Addr;

	//int 	ret;
	pRTC_STR		pRtc;

	

	pJT808_MSG_HEAD				pBB;
	//pJT808_MULTY_MEDIA_REPORT 	pMedia;
	//pJT808_GPS_PACK  				pPos;
	pJT808_CAMERA_TAKE_PHOTO 	pCamTakePhoto; 

	pELE_FENCE_STRUCT				pFence;
	pROAD_LINE_STRUCT				pRoadLine;
	pPOLYGON_STRUCT				pPolygon;
	
	static union 
	{
		 uint32     B32[8];
		 uint16     B16[16];
		 uchar		Buffer[256+4];
	}Tp;
	union 
	{
		 uint32     B32;
		 uint16     B16[2];
		 uchar		Buffer[4];
	}Tp1;

	///////////////////////////////////////////
	NOP();	NOP();
	
	///////////////////////////////////////////////////////////
	pBB 		= (pJT808_MSG_HEAD)&OneFrame[1];
	//pAck		= (pJT808_MSG_HEAD)&tBuffer;
	DataLen  = Length;							   //��Ϣ�峤��
	MsgId	= SwapINT16(pBB->Cmd);             // ��ȡ��ϢID
	MsgAttr  = SwapINT16(pBB->Attr.B16);		 // ��ȡ��Ϣ������
	isPack = (MsgAttr >> 13) & 0x01;           //  ��ȡ��װ��־ 
	Index  = SwapINT16(pBB->Index);          // ��ȡ��Ϣ��ˮ��
	//Plat_Serial_No = Index;  // ������;����AckBuffer.Index��ͬ
	//Index  = pBB->Index; 
	if (isPack)		           
	{
		DataBlockAddr = BB_CMD_ADDR_DATA_BLOCK;
	}
	else					// ����û�зְ�
	{
		DataBlockAddr = BB_CMD_ADDR_DATA_BLOCK - LEN_BB_PACK_ITEM;
	}


	s = CalXorSum((Length + DataBlockAddr-1),(uchar*)&OneFrame[BB_CMD_ADDR_MSG_ID]);
	//tel = memcmp(&OneFrame[BB_CMD_ADDR_SIM], &sTelNum.SimBCD_JT[0], 6);	

	if((s!=OneFrame[Length + DataBlockAddr]))//   ||(tel != 0))		 // У���벻��ȷ�� ���ֻ��Ų���ȷ 
	{
		Uart.puts("�յ��������У����� ");
		return ERROR;  //return OK;          //s = ERROR;
	}

	if ( (jt_auth_ok_flag != TRUE)&&(Uart0CommandFlag==FALSE)) 	  // δ��Ȩ��ֻ��������ָ��: ͨ��Ӧ��ע��Ӧ������͸��, ����ʱ�����Դ���
	{
		if( (MsgId!=eBB_gCMD_PLAT_ACK)&&(MsgId!=eBB_gCMD_PLAT_TO_TER_REG_ACK)&&(MsgId!=eBB_gCMD_PLAT_SEND_TRAN_DATA) )
			return OK;
	}

		  
	////////////////////////////////////////////////////// ���������ݴ���
	switch(MsgId)
	{
			
		///////////ƽ̨Ӧ��
		case eBB_gCMD_PLAT_ACK:			//		  
			Index = OneFrame[DataBlockAddr];
			Index <<= 8;
			Index |= OneFrame[DataBlockAddr + 1];	 	// ȡ��Ϣ���У��ն˷��͵���ˮ��

			MsgId = OneFrame[DataBlockAddr + 2];
			MsgId	<<= 8;
			MsgId |= OneFrame[DataBlockAddr + 3];		// ȡ��Ϣ���У��ն˷��͵���ϢID

			result = OneFrame[DataBlockAddr + 4];		// Ӧ����

			TC.GPRS_LongTimeNoAck = 0;
			
			///////////////////////////// �ն˵ȴ�Ӧ��

			///////////////////////////////////////////////// ����ȷ�Ϻ������ر�־
			if( result == eBB_ACK_ALARM_ACK)
			{
				if( (MsgId == eBB_gCMD_TER_POS_REPORT)&&(JT808Flag.VS7.Bits.InOutArea==1))
				{
					if( (InOrOutArea==eBB_IN_AREA)||(InOrOutArea==eBB_OUT_AREA) )
					{
						InOrOutArea = eBB_NO_IN_OUT;
						JT808Flag.VS7.Bits.InOutArea = 0;
					}
				}
				JT808Flag.VS5.Byte = JT808Flag.VS6.Byte = JT808Flag.VS7.Byte = JT808Flag.VS8.Byte = 0;
			}
			
			
			if (MsgId == eBB_gCMD_TER_CREATE_USER)				  // ʹ��ǰ��������	 
			{
				//Uart.puts("From jt1 Center' Data:");
				Uart.putb(1, Length, &OneFrame[DataBlockAddr]);  
				os_dly_wait(1);

				Creat_User_Reging = FALSE;

				if ((result == 0) )//&& (CreateUserFlag == NEED_CREATE_USER))
				{
					Uart.puts("�����ɹ�!!!\r\n");	os_dly_wait(1);
					CreateUserFlag = OK;
					DFE.Write(EE_CREATE_USER_FLAG, 1, &CreateUserFlag);
					NOP(); NOP(); NOP();
					s = 0x00;		  // �ɹ�
					
				}
				else
				{
					s = 0x01;		// ʧ��
					Uart.puts("����ʧ��!!!\r\n");	os_dly_wait(1);
				}

				// ������Ϣ�����������ظ�ע��״̬
				KeyDisp.SendDataToLCM_YH(eLCM_D_CAR_REG_RESULT, 1, &s); 
				os_dly_wait(100);

				Connect_Create_USer_Center_Or_Main_Center(CONNECT_MAIN_CENTER, 0);			 // �����Ƿ�ɹ��������ӻ�������
					
				
			} 	

			if ((result == 0) || (result == 4))		  	// 0���ɹ���4����������
			{

				if (MsgId == eBB_gCMD_TER_AUTH)
				{
					jt_recv_flag |= 0x01 << eBB_gFLAG_AUTH_INDEX;		    // 
					jt_auth_ok_flag = 1;
					GG_Status.Flag.Bits.RegServer = 1;
					AuthServerOverTime = 0;
					GPRS.InitAlig('P');		// ������ȳ��ӵ�����
					Uart.puts("��Ȩ�ɹ��� ");
						 
					// �����ط�ʱ��
					ReSend.OverTime = ReSend.Times = 0;	  // Ӧ��ʱ�� 10s
					ReSend.PreOverTime = SendOVT.TCP_ACK_OverTime;  				 						
				}
				
				else 	if (MsgId == eBB_gCMD_TER_HEARTBEAT)
				{
					 jt_recv_flag |= 0x01 << eBB_gFLAG_HEART_INDEX;		    // 

					 // �����ط�ʱ��
					ReSend.OverTime = ReSend.Times = 0;	  // Ӧ��ʱ�� 10s
					ReSend.PreOverTime = SendOVT.TCP_ACK_OverTime;  
				}
				else if ((MsgId == eBB_gCMD_TER_POS_REPORT) || (MsgId == eBB_gCMD_TER_POS_INFO_MULTI_UPLOAD))
				{
					 jt_recv_flag |= 0x01 << eBB_gFLAG_POS_INDEX;		    //	�յ�Ӧ���ñ�־

					 if (result == 4)	  //  ����ȷ��
					 {
						 if (JT808Flag.VS5.Bits.EmergencyAlarm ||
						 	 JT808Flag.VS5.Bits.Warning ||
						    JT808Flag.VS7.Bits.InOutArea ||
						    JT808Flag.VS7.Bits.InOutLine ||
						    JT808Flag.VS7.Bits.TravelTime ||
						    JT808Flag.VS8.Bits.IllegalPowerOn||
						    JT808Flag.VS8.Bits.IllegalMove)
						 {
							 JT808Flag.VS5.Bits.EmergencyAlarm = 0;
							 VehicleStatus.VS4.Bits.RobAlarm = 0;
							 JT808Flag.VS5.Bits.Warning = 0;
							 JT808Flag.VS7.Bits.InOutArea = 0;
							 JT808Flag.VS7.Bits.InOutLine = 0;
							 JT808Flag.VS7.Bits.TravelTime = 0;
							 JT808Flag.VS8.Bits.IllegalPowerOn = 0;
							 VehicleStatus.VS4.Bits.LawlessPowerOn = 0;
							 JT808Flag.VS8.Bits.IllegalMove = 0;
							 VehicleStatus.VS6.Bits.CarBeTrail = 0;
	
							 JT808_GPRS_TempPack = TRUE; 
							 Uart.puts("TP. Clear alarm flag"); 
						 }  

					 }
					  
					ReSend.OverTime = ReSend.Times = 0;  // Ӧ��ʱ�� 10s
					ReSend.PreOverTime = SendOVT.TCP_ACK_OverTime;  
					
				}
				else  if ((MsgId == eBB_gCMD_TER_DEREG) && (MsgId == AckBuffer.MsgId))	 // �ն�ע��
				{
					  
					  jt_recv_flag |= 0x01 << eBB_gFlag_DEREG_INDEX;
					  jt_auth_ok_flag = 0;
					  ReSend.OverTime = ReSend.Times = 0;	  // Ӧ��ʱ�� 10s
						ReSend.PreOverTime = SendOVT.TCP_ACK_OverTime;  
						
					  UnRegTerminal();
					  GPRS.InitAlig('P');		// ������ȳ��ӵ�����
					  Uart.puts("ƽ̨ע���ɹ�\r\n");

				}
			    else //if (MsgId == eBB_gCMD_TER_MEDIA_EVENT_UPLOAD)
				{
					  jt_send_cmd_flag = 0;
					  jt_recv_flag |= 0x01 << eBB_gFLAG_OTHER_INDEX;

					  	ReSend.OverTime = ReSend.Times = 0;	  // Ӧ��ʱ�� 10s
						ReSend.PreOverTime = SendOVT.TCP_ACK_OverTime;  						
				}
			}
			else if (result== 1)
			{
				Uart.puts("�յ�Ӧ�𣬴�����ʧ�� \n");
				if ((MsgId == eBB_gCMD_TER_AUTH) && (AuthServerOverTime++ > 5))
				{
					GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DEREG,0,NULL);
					//UnRegTerminal();
				}
			}
			else if (result == 2)
			{
				Uart.puts("�յ�Ӧ����Ϣ����!!!\n");
				if ((MsgId == eBB_gCMD_TER_AUTH)  && (AuthServerOverTime++ > 5))
				{
					GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DEREG,0,NULL);
					//UnRegTerminal();
				}
			}
			else if (result == 3)
			{
				Uart.puts("�յ�Ӧ�𣬲�֧�ָ�ָ��!!!\n");
				if ((MsgId == eBB_gCMD_TER_AUTH) && (AuthServerOverTime++ > 5))
				{
					GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DEREG,0,NULL);
					//UnRegTerminal();
				}
			}
			else
			{
				Uart.puts("Ӧ��ID��ԭID ");
				Uart.putb(1, 2 , (uchar *)&MsgId);
				os_dly_wait(5);
				Uart.putb(1, 2 , (uchar *)&AckBuffer.MsgId);
				os_dly_wait(5);
			}
			
			//GG_Status.Flag.Bits.RegServer = 1;
				
		break;
		
		////////////ע��Ӧ��	
		case eBB_gCMD_PLAT_TO_TER_REG_ACK:	//,						  
			
			Index = OneFrame[DataBlockAddr+0];
			Index <<= 8;
			Index |= OneFrame[DataBlockAddr+1];

					
			result = OneFrame[DataBlockAddr + 2];
			ter_or_veh_reg_result = result;
			// �ж�result		
			if (result == 0)			// ע��ɹ�
			{
				NeedRegServer = 0;	
				DFE.Write(EE_NEED_REG_SERVER, 1, &NeedRegServer);	
				SystemLog.GSM.RegServerOK++;	
				
				SetBufferValue((uchar *)&FlashPageCnt.AlarmMsgSave, 16, 0);
						   
				jt_recv_flag |= 1 << eBB_gFLAG_REG_INDEX;

			  	// �����ط�ʱ��
				AckBuffer.MsgId = 0;		//�Ա�������η���									  	
				ReSend.OverTime = ReSend.Times = 0;	  // Ӧ��ʱ�� 10s
				ReSend.PreOverTime = SendOVT.TCP_ACK_OverTime;  
				
				Uart.puts(" ע��ɹ�\n");
				GPRS.InitAlig('P');		// ������ȳ��ӵ����ݣ�ע��ʹ�����ȳ��ӣ�
					
				// ��ȡ��Ȩ��	 
				SetBufferValue(  (uchar*)jt_auth_code, LEN_AUTH_CODE,0);
				memmove(&jt_auth_code[0], &OneFrame[DataBlockAddr + 3], DataLen - 3);
				DFE.Write(EE_AUTH_CODE, LEN_AUTH_CODE,(uchar *)&jt_auth_code[0]);			//  д��Ȩ�뵽����
				
			}
			else if (result == 1)	// �����ѱ�ע��
			{
				jt_recv_flag |= 1 << eBB_gFLAG_POS_INDEX;
				Uart.puts("�����ѱ�ע��\n");
				GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DEREG,0,NULL);
			}
			else if (result == 2)   // ���ݿ����޸ó���
			{
				 jt_recv_flag |= 1 << eBB_gFLAG_POS_INDEX;
				 Uart.puts("���ݿ����޸ó���!\n");
				 RegisterWithoutTer++; //���½�������������IP
			}
			else if (result == 3)   // �ն��ѱ�ע��
			{
				  jt_recv_flag |= 1 << eBB_gFLAG_POS_INDEX;
				  Uart.puts("�ն��ѱ�ע��!\n");
				  GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DEREG,0,NULL);
			}
			else if (result == 4)  // ���ݿ����޸��ն�
			{
				  jt_recv_flag |= 1 << eBB_gFLAG_POS_INDEX;
				  Uart.puts("���ݿ����޸��ն�!\n");
				  RegisterWithoutTer++; //���½�������������IP
			}
			


		break;
		case eBB_gCMD_PLAT_QUERY_TER_POS:					// λ����Ϣ��ѯ
			

			AckBuffer.MsgId = MsgId;
			AckBuffer.Index = Index; 
			Uart.puts("��ѯλ����ϢӦ�� ");	 
			BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_POS_QUERY_ACK,0,NULL);
		break;
		/*
		//////////////////
		// ��ý�������ϴ�Ӧ��ע���ն˷�����ȫ����ý�����ݺ�ƽ̨�·���ָ���Ӧ���޶�ý�����ͣ���������ϴ�����ȷ��
		*/	
		case eBB_gCMD_PLAT_MEDIA_DATA_ACK:				
			
	        if (PicRetryOrMulPic == 2)    return OK;		//  ���ڷ��ʹ洢��ý��æµ, ֱ�ӷ���
			PicRetryOrMulPic = 1;                     

			PDU_SMS.Ack = 0;          //������ǿ�ƴ�GPRSͨ������	�����SMSͨ���´�Ĳɼ�ָ����ݴ�GPRS����
			Tp.Buffer[3] = OneFrame[DataBlockAddr];
			Tp.Buffer[2] = OneFrame[DataBlockAddr + 1];
			Tp.Buffer[1] = OneFrame[DataBlockAddr + 2];
			Tp.Buffer[0] = OneFrame[DataBlockAddr + 3];

			jt_media_id = 	 Tp.B32[0];

			//if (Tp.B32[0] == jt_media_id)
			{
				Tp.Buffer[0] = OneFrame[DataBlockAddr + 4];				 // �ش�������

				if ((!Tp.Buffer[0]) || (!(DataLen - (DataBlockAddr + 1) - 4 - 1 - i * 4)))			 // �յ�ȫ����
				{
					jt_send_cmd_flag = 0;
					jt_recv_flag |= 0x01 << eBB_gFLAG_OTHER_INDEX;

					//Protocol.GPRS[JT_PROTOCOL] = 0;  
					PicRetryOrMulPic = 0; 
					ReSend.OverTime = ReSend.Times = 0;	  // Ӧ��ʱ�� 10s
					ReSend.PreOverTime = SendOVT.TCP_ACK_OverTime;  
					
				}
				else
				{
					jt_recv_flag |= 0x01 << eBB_gFLAG_OTHER_INDEX;
				   //set_send_pic_time = BB_SEND_PIC_TIME;				// Ӧ�ŵ�ϵͳ��ʼ��
					

					j =Tp.Buffer[0];	  // �ش�������
					JtSendPic.SendPicNo = jt_media_id - 1;  // ͼƬ��� : ��ý��ID - 1
				   JtSendPic.TotalPack = j;

					SetBufferValue(JtSendPic.SendPic.SendPackNo, 125, 0); 

					  
					//for (i = 1; ((DataLen - (DataBlockAddr + 1) - 4 - 1 - i) != 0) && (i <= j); i++)
					for (i = 0; i < j; i++)
					{			
						//JtSendPic.SendPic.SendPackNo[i] = OneFrame[DataBlockAddr + 4 + i + 1];
						JtSendPic.SendPic.SendPackNo[i] = OneFrame[DataBlockAddr + 4 + i + 1] - 1;   //	ʵ�ʴ���flash��������Ǵ�0��ʼ��������涨�Ǵ�1��ʼ�����Լ�ȥ1
					}

					//send_record.n = 0;
					//send_record.Tn = send_record.Tn_1 = set_send_pic_time;			 // ���÷���ͼƬ��ʱʱ��
					JtSendPic.Index = 0;
					jt_pic_send_flag = 1;
					Uart.puts("��ý������Ӧ�� ");	 
					BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_MEDIA_DATA_UPLOAD,0,NULL);
				}
			}
		break;
		/*
		//////////////////
		// ����ͷ�������㡣��ָ����ͨ��Ӧ��ʹ��ר��Ӧ��
		*/	
		case eBB_gCMD_PLAT_TAKE_PHOTO:					
		//case eBB_gCMD_PALT_ONE_MEDIA_DATA_SERCH_UPLOAD:			//��ʱʹ��	
			
			NeedTakePhotoAck = TRUE;
		
		 	if (CameraUsing == TRUE)	
		 	{
		 		ter_or_veh_reg_result = 0x01;  // ͨ����֧��
				BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_TAKE_PHOTO_ACK,0,NULL);
		 		Uart.puts(" Զ���������ջ�¼�񣬵�����ͷæ������ ");
		 		return OK;			 // ����ʹ����ͷֱ�ӵķ���
		 	}

			PDU_SMS.Ack = 0;          //������ǿ�ƴ�GPRSͨ������	�����SMSͨ���´�Ĳɼ�ָ����ݴ�GPRS����
			//AckBuffer.MsgId = MsgId;
			//AckBuffer.Index = Index;	 
			Plat_Serial_No = Index;	   // ������;����AckBuffer.Index��ͬ
			
			pCamTakePhoto = (pJT808_CAMERA_TAKE_PHOTO)&OneFrame[DataBlockAddr];
			pCamTakePhoto->Cmd = SwapINT16(pCamTakePhoto->Cmd);

			s = pCamTakePhoto->Channel;
		    
			
			if ((pCamTakePhoto->Cmd  == 0xFFFF) || ((!s  || (s > 4)) ))//|| (!Camera.NCameraID[s-1]))) 	 // ¼�񣬻�����ͷ������
			{
				ter_or_veh_reg_result = 0x02;  // ͨ����֧��
				BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_TAKE_PHOTO_ACK,0,NULL);
				Uart.puts(" Զ���������ջ�¼��ͨ����֧�ֻ�¼��֧�� ");
				return OK;  
			}
			
			JT_ManulCameraID = s;		 // ʹ�õ�ָ��������ͷ����
			MediaEventFlag = eBB_MEDIA_EVENT_PLAT;

			Camera.GetOperateParameter(0,&cPic);	 	// �ȶ�ȡԭ������
			if (pCamTakePhoto->SaveFlag)			cPic.SaveAndSend = 0;
			else											cPic.SaveAndSend = TRUE;			
			if (pCamTakePhoto->Res == 0x01)		cPic.Resolution = 0x03;	 	// ֻ֧��320*240 �� 640*480
			else if(pCamTakePhoto->Res == 0x02)	cPic.Resolution = 0x05;
			else											cPic.Resolution = 0x03;
			Camera.SetOperateParameter(0, &cPic);		// ����

			if ( (pCamTakePhoto->Cmd)==0)
			{
				Camera.SetTakeNumber(0, 0);				
			}
			else 								  // ����¼���ܣ���������
			{
				
				SetBufferValue((uchar *)&TakePhotoAck, LEN_TAKE_PHOTO_ACK+1, 0);
				Protocol.GPRS[JT_PROTOCOL] = TRUE;
				Camera.SetTakeNumber(pCamTakePhoto->Cmd, 0);				
				SetBufferValue( Tp.Buffer,32,0);
				strcpy( (char*)Tp.Buffer,"Զ��������������");
				Int2Str( pCamTakePhoto->Cmd, (char*)&Tp.Buffer[16]);
				strcat( (char*)Tp.Buffer,"��");
				Uart.puts((char*)Tp.Buffer);
				TakePhoneAckFlag = TRUE;
				TakePhotoAck.MediaNum = 0;				
				return OK;
			}

		break;
		/*
		//////////////////
      // ������ý�����ݣ������ϴ�
		*/	
		case eBB_gCMD_PALT_ONE_MEDIA_DATA_SERCH_UPLOAD:					

			#if(0)
			MediaEventFlag = eBB_MEDIA_EVENT_PLAT;
			Protocol.GPRS[JT_PROTOCOL] = TRUE;
			Camera.SetTakeNumber(1, 0);
			NeedTakePhotoAck = 0;	
			Jt_photo_upload = TRUE;
			#else
	      Camera.GetOperateParameter(0, &cPic);	 	// �ȶ�ȡԭ������
	      
	      Tp1.Buffer[3] = OneFrame[DataBlockAddr++];
	      Tp1.Buffer[2] = OneFrame[DataBlockAddr++];
	      Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
	      Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
	      cPic.SendPicNo = Tp1.B32;
	      
	      strcpy( (char*)Tp.Buffer,"�������ϴ���");
	      Int2Str( Tp1.B32, (char*)&Tp.Buffer[12]);
	      strcat( (char*)Tp.Buffer,"��ͼƬ");
	      if( OneFrame[DataBlockAddr] )
	      {	// ɾ��ͼƬ
	      	strcat( (char*)Tp.Buffer,"����ɾ��ͼƬ ");
	      	cPic.DelPicNo = Tp1.B32;
	      }
	      
	      Uart.puts((char*)Tp.Buffer);	 
			Camera.SetOperateParameter(0, &cPic);		// ����
			#endif
			return OK;
		break;			 
		/*
		//////////////////
		//  �洢��ý�����ݼ�������ʱֻ����ͼƬ��
		*/	
		case eBB_gCMD_PLAT_MEDIA_DATA_SERCH:					

			PDU_SMS.Ack = 0;     //������ǿ�ƴ�GPRSͨ������	�����SMSͨ���´�Ĳɼ�ָ����ݴ�GPRS����
			
			AckBuffer.MsgId = MsgId;
			AckBuffer.Index = Index;
			
			MulMeDa.SavedMediaSearch.MediaType = OneFrame[DataBlockAddr];
			MulMeDa.SavedMediaSearch.Channel = OneFrame[DataBlockAddr + 1];
			MulMeDa.SavedMediaSearch.Event = OneFrame[DataBlockAddr + 2];
			
			memmove(&MulMeDa.SavedMediaSearch.StartTime[0], &OneFrame[DataBlockAddr + 3], 12);	  // BCD: 12 = 6 + 6 

			BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_MEDIA_SERCH_ACK,0,NULL);
			
		break;
		/*
		//////////////////
		//	�洢��ý�������ϴ�
		*/	
		case eBB_gCMD_PLAT_SET_MEDIA_DATA_UPLOAD:	
				
			if (CameraUsing == 1)		return OK;			

			if (PicRetryOrMulPic == 1)  return OK;				 // �ڷ��ش�����ֱ�ӷ���
			PicRetryOrMulPic = 2;

			while((USBTaskCreat!=0) || (PrintTaskCreat!=0)) 
			{
				os_dly_wait(1);
			}
			
			ComtAck = 1;
		break;


		/*
		/////////���ò���
		*/
		case   eBB_gCMD_PLAT_SET_TER_PARA:			           
		  
		  tmp = OneFrame[DataBlockAddr];
		  DataLen = DataBlockAddr+1;
		  result = 0;
		  //Uart.putb(1,1,(uchar *)&tmp); 	os_dly_wait(5);  Uart.puts("\r\n"); os_dly_wait(1);
		  if(tmp==0) {result = 3; } 
		  
		  while(tmp&&((result==0)||(result==80)))
		  {
			  //j = ByteToINT32(OneFrame[DataLen+3],OneFrame[DataLen+2],OneFrame[DataLen+1],OneFrame[DataLen]);
			  j = ByteToINT32(OneFrame[DataLen+0],OneFrame[DataLen+1],OneFrame[DataLen+2],OneFrame[DataLen+3]);
			  DataLen += 4;
			  s = OneFrame[DataLen];
			  DataLen += 1;  
			  if(TD.GPRS == tGPRS_GSM_ALL_DATA)	{	Uart.putb(1,s+5,&OneFrame[DataLen-5]);	os_dly_wait(5); Uart.puts("\r\n"); os_dly_wait(1);	 }
			  else if(s!=0)
			    result = JT_Set_Get_Para((uchar *)&OneFrame[DataLen], s, j);
			  else
			  {
			  	  result = 3;
				  break;
			  }
			  DataLen += s;
			  tmp--;
		  }
		  //result = j;
		  if(result==80)
		  { 
		     ComtAck = 2;
			  result = 0;
		  }
		  else
		    ComtAck = 1;
		break;
	   /*	
		//////////��ѯ���ò���
		*/
		case   eBB_gCMD_PLAT_QUERY_TER_PARA:
		  
		  PDU_SMS.Ack = 0;          //������ǿ�ƴ�GPRSͨ������	�����SMSͨ���´�Ĳɼ�ָ����ݴ�GPRS����
		  tmp = 0;
		  i = 0;	
		  result = 0;
		  Len = 2+1;
		  //while(i<=0x0084)

		  SetBufferValue( (uchar *)&OneFrame[Len+5],(LEN_ONE_FRAME-32),0);

		  while (i <= 0x000110) // modified 2013.3
		  {
			  result = JT_Set_Get_Para((uchar *)&OneFrame[Len+5], 0, i);
			  if(result!=0)
			  {  
			     j = SwapINT32(i);
				  memmove(&OneFrame[Len],(uchar *)&j, 4);
				  Len += 4;
				  OneFrame[Len] = result;
			     Len += 1;
				  Len += result;
				  tmp++;
			  }
			  i++;
		  }

		  OneFrame[0] = Index>>8;
		  OneFrame[1] = Index;
		  //memmove(OneFrame, (uchar *)&Index, 2);				//Ӧ�����ˮ��
		  OneFrame[2] = tmp;
		  GPRS.JT808_DataPackPushAlignment(0,eBB_gCMD_TER_QUERY_PARAM_ACK, Len, 0, OneFrame);
		  Uart.puts("��ѯ����Ӧ��3333 ");	 
		  ComtAck = 3;
		break;
	   /*
		///////////�ն˿���
		*/
		case   eBB_gCMD_PLAT_CTRL_TER:	
		   
			tmp = OneFrame[DataBlockAddr];
			result = 0;	
			ComtAck = 1;

			#if ( TEST_CODE_JUST_FOR_TEST_LUOYANG == 1) 
			break;
			#else
			NOP();	NOP();//	;fjlkds fs f
			#endif

			//Uart.putb(1, 64, &OneFrame[DataBlockAddr]);		os_dly_wait(15);   	
			if(tmp==1)					  //��������������
			{
				if (SearchArrayCount(DataLen - 1,&OneFrame[DataBlockAddr+1], 1, (uchar*)";") != 10)	// ��������������֮��Ӧ��10���ֺ�
				{
					result = 1;
					break;
				}

			   SetBufferValue((uchar *)&JT808_Ctr_IP_DNS, LEN_JT_CTR_IP_DNS, 0);
				SetBufferValue((uchar *)&UpdatePara,UpdatePara_Len, 0);
				Len = DataBlockAddr+1;
				i = DataLen; 				 // ��Ϣ�峤��
				j = 0;					 	 // ��־������ʽ

			   //if((DataLen = MyStrLen((char *)&OneFrame[Len], ';'))!=0)
				if((ptr = SearchArray(i - (Len - DataBlockAddr) , &OneFrame[Len], 1, (uchar *)";"))!= &OneFrame[Len])
				{
					DataLen = ptr - &OneFrame[Len]; 
					if ((ptr = SearchArray(DataLen, (uchar *)&OneFrame[Len], 6, (uchar *)"ftp://")) == NULL)
					{
				   	strncpy((char *)&JT808_Ctr_IP_DNS.URL[0], (char *)&OneFrame[Len], DataLen);
					}
					else
					{
						//strncpy((char *)&UpdatePara.FileList[0], (char *)&OneFrame[Len], DataLen);				   //FTP����  Ŀ¼	
						if ((ptr = SearchArray(DataLen - 6, ptr + 6, 1, (uchar *)"/")) == NULL)
						{
							result = 1;
							break;	
						}

						memmove(UpdatePara.FileList, "..", 2);   // ftpվ��ĸ�Ŀ¼		G11�����ļ�Ĭ�Ϸŵ���
						memmove(UpdatePara.FileName, ptr + 1, DataLen - (ptr + 1 - &OneFrame[Len]));    // �����ļ���

						j = 1;   
					}
				}
				Len += (DataLen + 1);
				if((DataLen = MyStrLen((char *)&OneFrame[Len], ';'))!=0)
				{
				   strncpy((char *)&JT808_Ctr_IP_DNS.C_ANP[0], (char *)&OneFrame[Len], DataLen);
				}
				Len += (DataLen + 1);
				if((DataLen = MyStrLen((char *)&OneFrame[Len], ';'))!=0)
				{
				   strncpy((char *)&JT808_Ctr_IP_DNS.C_IP_Name[0], (char *)&OneFrame[Len], DataLen);
					strncpy((char *)&UpdatePara.FTPname[0], (char *)&OneFrame[Len], DataLen);					 //FTP����	��¼�û���
				}						 
				Len += (DataLen + 1);
				if((DataLen = MyStrLen((char *)&OneFrame[Len], ';'))!=0)
				{
				   strncpy((char *)&JT808_Ctr_IP_DNS.C_IP_Secrier[0], (char *)&OneFrame[Len], DataLen);
				   strncpy((char *)&UpdatePara.FTPpassword[0], (char *)&OneFrame[Len], DataLen);				  //FTP����	 ��¼����
				}
				Len += (DataLen + 1);
				if((DataLen = MyStrLen((char *)&OneFrame[Len], ';'))!=0)
				{
				   strncpy((char *)&JT808_Ctr_IP_DNS.C_IPorDNS[0], (char *)&OneFrame[Len], DataLen);
					strncpy((char *)&UpdatePara.RemoteAddr[0], (char *)&OneFrame[Len], DataLen);				 //FTP����	 ���ӵ�IP��DNS
				}
				else
				  result = 1;

				Len += (DataLen + 1);


				//JT808_Ctr_IP_DNS.C_TCP_Port = ByteToINT16(OneFrame[Len], OneFrame[Len+1]);
				//Int16ToDecChar((char )JT808_Ctr_IP_DNS.C_TCP_Port, (char *)&UpdatePara.RemotePort[0]);		  //FTP����	  �˿�

				//	Len += 3;
				//JT808_Ctr_IP_DNS.C_UDP_Port = ByteToINT16(OneFrame[Len], OneFrame[Len+1]);
				//	Len += 3;
				//memmove(&JT808_Ctr_IP_DNS.CP_ID[0], &OneFrame[Len], 5);
				//Len += 6;
//				if((DataLen = MyStrLen((char *)&OneFrame[Len], ';'))!=0)
//				   strncpy((char *)&JT808_Ctr_IP_DNS.HVS[0], (char *)&OneFrame[Len], DataLen);
//				Len += (DataLen + 1);
//         	if((DataLen = MyStrLen((char *)&OneFrame[Len], ';'))!=0)
//				   strncpy((char *)&JT808_Ctr_IP_DNS.HWVS[0], (char *)&OneFrame[Len], DataLen);
//				Len += (DataLen + 1);
//	         JT808_Ctr_IP_DNS.Net_Time = ByteToINT16(OneFrame[Len], OneFrame[Len+1]);


				s = 0;
				if((ptr = SearchArray(i - (Len - DataBlockAddr) , &OneFrame[Len], 1, (uchar *)";"))!= &OneFrame[Len])
				{
					DataLen = ptr -  &OneFrame[Len];	
					if (DataLen != 2)
					{
						result = 1;
						break;
					}
					JT808_Ctr_IP_DNS.C_TCP_Port = ByteToINT16(OneFrame[Len], OneFrame[Len+1]);
					Int16ToDecChar((char )JT808_Ctr_IP_DNS.C_TCP_Port, (char *)&UpdatePara.RemotePort[0]);		  //FTP����	  �˿�
					s = 1;
				}
				else if (j == 1)				 // ftp������ʽ�� TCP�˿ڲ�����
				{
					result = 1;
					break;
				}
				else 
				{
					DataLen = 0;
				}
				Len += (DataLen + 1);

				if((ptr = SearchArray(i - (Len - DataBlockAddr) , &OneFrame[Len], 1, (uchar *)";"))!= &OneFrame[Len])
				{
					DataLen = ptr - &OneFrame[Len];
					if (DataLen != 2)
					{
						result = 1;
						break;
					}
					JT808_Ctr_IP_DNS.C_UDP_Port = ByteToINT16(OneFrame[Len], OneFrame[Len+1]);
					s = 1;
				}
				else 
				{
					DataLen = 0;
				}
				Len += (DataLen + 1);

				if (s == 0)				 // ��û��TCP�˿ڲ���	  Ҳû��	UDP����
				{
					result = 1;
					break;
				}


				if((DataLen = MyStrLen((char *)&OneFrame[Len], ';'))!=0)
				{
					if (DataLen != 5)			 // �ն������̱������Ϊ5���ֽ�
					{
						result = 1;
						break;	
					}
					memmove(&JT808_Ctr_IP_DNS.CP_ID[0], &OneFrame[Len], DataLen);			
				}
				Len += (DataLen + 1);

				if((DataLen = MyStrLen((char *)&OneFrame[Len], ';'))!=0)
				   strncpy((char *)&JT808_Ctr_IP_DNS.HVS[0], (char *)&OneFrame[Len], DataLen);
				Len += (DataLen + 1);
         		if((DataLen = MyStrLen((char *)&OneFrame[Len], ';'))!=0)
				   strncpy((char *)&JT808_Ctr_IP_DNS.HWVS[0], (char *)&OneFrame[Len], DataLen);
				Len += (DataLen + 1);

				if ((Len - DataBlockAddr) <= (i - 2))	// С����Ϣ�峤�ȼ�2����Ϊ��������ֶκϷ�
				{
            	JT808_Ctr_IP_DNS.Net_Time = ByteToINT16(OneFrame[Len], OneFrame[Len+1]);
				}
				else if (j != 1)
				{
					result = 1;
					break;
				}
				
	         if(result==0)
				{
				   //if(JT808_Ctr_IP_DNS.Net_Time==0xffff)   	 //FTP��ʽ����
					if (j == 1)			// FTP������ʽ
					{
						GG_Status.Flag.Bits.Updating = 0; 		
						UpdatePara.Enable = ENABLE_FLAG;			//ʹ��
					}
				   else
					{
					   //JT808_Ctr_IP_DNS.Ctr = 2;
						JT808_Ctr_IP_DNS.Ctr = 0;
					   DFE.Write(EE_JT_NET_CTR, LEN_JT_CTR_IP_DNS,(uchar *)&JT808_Ctr_IP_DNS);
					   sIP.EnableFlag[eFLAG_TCP2] = ENABLE_FLAG;					//JT808
		            s = ENABLE_FLAG;
			         DFE.Write(EE_ENABLE_TCP2,1,(uchar*)&s);
		            //GG_Status.Flag.Word = GG_FLAG_INIT_DATA;	
		            //Uart.puts("GRRS Close\r\n\0");
				   }
				   ComtAck = 2;
					
				}
			}
			else if(tmp==2)			  //����ָ��������
			{	
				if ((OneFrame[DataBlockAddr+1] == 0x00) && 	  // ������ָ�������������Ҳ���֮��Ӧ��8���ֺ�
					 (SearchArrayCount(DataLen -1,&OneFrame[DataBlockAddr+1], 1, (uchar*)";") != 8))	
				{
					result = 1;
					break;
				}

				SetBufferValue((uchar *)&JT808_Ctr_IP_DNS, LEN_JT_CTR_IP_DNS, 0);
				if(OneFrame[DataBlockAddr+1]<2)
				{
					///JT808_Ctr_IP_DNS.Ctr = OneFrame[DataBlockAddr+1];
					//if(JT808_Ctr_IP_DNS.Ctr==1)		  	//�ر���ʱ���ӷ�����
					if (OneFrame[DataBlockAddr+1] == 1)
					{
						JT808_Ctr_IP_DNS.Ctr = 1;
						sIP.EnableFlag[eFLAG_TCP] = ENABLE_FLAG;  // �л�����������ʹ��

		            s = 0;
			        	DFE.Write(EE_ENABLE_TCP2,1,(uchar*)&s);
						DFE.Write(EE_JT_NET_CTR ,1,(uchar*)&JT808_Ctr_IP_DNS.Ctr);
						JT808_Ctr_IP_DNS.Net_Time = 0;
						DFE.Write(EE_JT_NET_CTR_NET_TIME,2,(uchar*)&JT808_Ctr_IP_DNS.Net_Time);
						ComtAck = 2;
					}
					else
					{
						Len = DataBlockAddr+3;
						i = DataLen;                 // ��Ϣ�峤��


						if((DataLen = MyStrLen((char *)&OneFrame[Len], ';'))!=0)
						   strncpy((char *)&JT808_Ctr_IP_DNS.Akey[0], (char *)&OneFrame[Len], DataLen);						     //��ʱ���ӵ�ƽ̨��Ȩ��
						Len += (DataLen + 1);
						if((DataLen = MyStrLen((char *)&OneFrame[Len], ';'))!=0)
						   strncpy((char *)&JT808_Ctr_IP_DNS.C_ANP[0], (char *)&OneFrame[Len], DataLen);		
						Len += (DataLen + 1);
						if((DataLen = MyStrLen((char *)&OneFrame[Len], ';'))!=0)
						   strncpy((char *)&JT808_Ctr_IP_DNS.C_IP_Name[0], (char *)&OneFrame[Len], DataLen);	
						Len += (DataLen + 1);
						if((DataLen = MyStrLen((char *)&OneFrame[Len], ';'))!=0)
						   strncpy((char *)&JT808_Ctr_IP_DNS.C_IP_Secrier[0], (char *)&OneFrame[Len], DataLen);	
						Len += (DataLen + 1);
						if((DataLen = MyStrLen((char *)&OneFrame[Len], ';'))!=0)
						   strncpy((char *)&JT808_Ctr_IP_DNS.C_IPorDNS[0], (char *)&OneFrame[Len], DataLen);
						else
						  result = 1;	
						Len += (DataLen + 1);

						s = 0;
						if((ptr = SearchArray(i - (Len - DataBlockAddr) , &OneFrame[Len], 1, (uchar *)";"))!= &OneFrame[Len])
						{
							DataLen = ptr - &OneFrame[Len];
							if (DataLen != 2)
							{
								result = 1;
								break;
							}
							JT808_Ctr_IP_DNS.C_TCP_Port = ByteToINT16(OneFrame[Len], OneFrame[Len+1]);
							s = 1;
						}
						else
						{
							DataLen = 0;
						}
						Len += (DataLen + 1);

						if((ptr = SearchArray(i - (Len - DataBlockAddr) , &OneFrame[Len], 1, (uchar *)";"))!= &OneFrame[Len])
						{
							DataLen = ptr - &OneFrame[Len];
							if (DataLen != 2)
							{
//								Uart.puts("AAA");
//								Uart.putb(1, 2, (uchar *)&DataLen);
//								os_dly_wait(1);

								result = 1;
								break;
							}
							JT808_Ctr_IP_DNS.C_UDP_Port = ByteToINT16(OneFrame[Len], OneFrame[Len+1]);
							s = 1;
						}
						else
						{
							DataLen = 0;
						}
						Len += (DataLen + 1);



						if (s == 0)				 // ��û��TCP�˿ڲ���	  Ҳû��	UDP����
						{
							result = 1;
						}
						
						if ((Len - DataBlockAddr) <= (i - 2))	// С����Ϣ�峤�ȼ�2����Ϊ��������ֶκϷ�
						{
		            		JT808_Ctr_IP_DNS.Net_Time = ByteToINT16(OneFrame[Len], OneFrame[Len+1]);
						}
						else
						{
							result = 1;
						}

						if(result==0)
						{
						   DFE.Write(EE_JT_NET_CTR, LEN_JT_CTR_IP_DNS,(uchar *)&JT808_Ctr_IP_DNS);
						   sIP.EnableFlag[eFLAG_TCP2] = ENABLE_FLAG;					//JT808
		                   s = ENABLE_FLAG;
			               DFE.Write(EE_ENABLE_TCP2,1,(uchar*)&s);
							//NeedRegServer = ENABLE_FLAG;
		               //GG_Status.Flag.Word = 0;	
		               //Uart.puts("GRRS Close\r\n\0");	
							ComtAck = 2;

						}
						//else 
							//JT808_Ctr_IP_DNS.Ctr = 0xFF;	
				   }
				}
				else
				  result = 3;
			}
			else if(tmp==3)
			{
				Uart.puts("Shut Sys...\r\n\0"); os_dly_wait(10);	
				JT_Ctr_PowerDown = 1;		//�ػ��������е�Դ�ص���ֻ�������Żָ�������������ACC����������   0x87EBB7DE			����ûʡʲô����
				//LPC_SC->PCONP  = 0x00689786;		
			}
	  	   else if(tmp==4)		    //��λ								 ��Ȼ������������ʲô���⣿	�Ƿ��û���ⲿ���Ź��й�
			{
			   Uart.puts("Restart Sys...\r\n\0"); os_dly_wait(10);	
				/*DisableIRQ();
	        	NOP();	NOP();	NOP();	NOP();
	       		NOP();	NOP();	NOP();	NOP();
	      		for(;;)
	       		{
	        		while(1);	
	      		}	*/
				NVIC_SystemReset();
			}
			else if(tmp==5)		    //�ָ���������	��̣���¼���㣬����ϵ���ָ�����ʻԱ�ָ���������Ϣ�ָ�����1������������������IP��غͺ���������㣬�����Ȩ�룬�ָ��ٶ�����ֵ��ƣ�ͼ�ʻ����ֵ��
			{
				Uart.puts("Reset Factory config...\r\n\0"); os_dly_wait(10);	
				//DFE.Read(EE_JT_MAIN_SERVER, LEN_JT808_IP_DNS, Tp.Buffer);
				os_dly_wait(1);
				//Uart.EreadFRAM_RecodeMem(16, 0x2000-16);									   //������ƬFram	  ������������ַ
				os_dly_wait(1);
				//DFE.Write(EE_JT_MAIN_SERVER, LEN_JT808_IP_DNS, Tp.Buffer);
		      //Uart.EreadFirstBlock();	
				Sys.FirstPowerOn(FALSE);  				//�ָ���������
				os_dly_wait(10);
				DisableIRQ();
	        	NOP();	NOP();	NOP();	NOP();
	       	NOP();	NOP();	NOP();	NOP();
				NVIC_SystemReset();						   //�ָ��������û���Ҫ����һ��
			}
			else if(tmp==6)			//�ر�����ͨ��	 Ŀǰֻ�رմ���0��USB��CAN���ߣ���û�йرյ�Դ��ֻ�������Żָ�
			{
			   if(JT_ShutCommunication==0)
				{
					Uart.puts("Close Communication...��\r\n\0"); os_dly_wait(10);	
					JT_ShutCommunication = 1;
					
				}
				else
				{
				   Uart.puts("Open Communication...\r\n\0"); os_dly_wait(10);	
					JT_ShutCommunication = 0;
				
				}
			}
			else if(tmp==7)		   //�ر���������ͨ��
			{
				Uart.puts("�ն˿��ƣ��ر�ͨѶģ��\r\n\0"); os_dly_wait(10);	
			   JT_ShutMobileCnt	= TRUE;
				//TestBlind = TRUE;
			}
			else if(tmp==0x69)				 //���ӿ�����
			{
				
				Sys.FirstPowerOn(TRUE);  				//�ָ���������	  �ض� �����������ݰ���IP��ַ�ͼ�¼������
				NVIC_SystemReset();          
			}
			else 
			{
			  result = 3;			  //��֧��
			}
			
		break;
		/*
		 //��ʱλ�ø��ٿ���		�����ã����Ϻ���ʵ��
		 */
		case   eBB_gCMD_PLAT_TRACE_TER_POS:		            
		   
			Len = DataBlockAddr;
			SendInterval.T_Second = ByteToINT16(OneFrame[Len],OneFrame[Len+1]);			 //0ֹͣ���� 	,��������ʱ����						
			SendInterval.T_ContinuedTime = ByteToINT32(OneFrame[Len+2],OneFrame[Len+3],OneFrame[Len+4],OneFrame[Len+5]) ;			                              
			DFE.Write(EE_JT_SEND_T_SECOND, 8, (uchar *)&SendInterval.T_ContinuedTime);	
			GSM.CalculateSendInterval(0);			
			ComtAck = 1;
		break;	
		/*
		/////////��������
		*/
		case   eBB_gCMD_PLAT_VEH_CTRL:
		   
			//GPRS.RemoteLockDoor(&OneFrame[DataBlockAddr]);
			s = OneFrame[DataBlockAddr];
			if(s&0x01)
			{									  //����
			   CONTROL_OUT1;
				os_dly_wait(20);
			   NULL_CONTROL_OUT1;
				JT808Flag.VS2.Bits.LockDoor = 1;
			}
			else
			{									 //����
			  CONTROL_OUT2;
			  os_dly_wait(20);
			  NULL_CONTROL_OUT2;
			  JT808Flag.VS2.Bits.LockDoor = 1;
			}
			Tp.Buffer[0] = Index>>8;
		   Tp.Buffer[1] = Index;
			Len = get_position_info(0,&Tp.Buffer[2], 2);
         GPRS.JT808_DataPackPushAlignment(0,eBB_gCMD_TER_VEH_CTRL_ACK, Len+2, 0, Tp.Buffer);
			Uart.puts("��������Ӧ�� ");	 
			ComtAck = 0;
		break;
		/*
		/////////����	�Զ���ָ�� ��Ϣ�� 00#ȡԤ��ļ������벦�ż���; 01,13580474097# ȡ�·��ĺ��벦�ż���;������Ч
		*/
		case    eBB_gCMD_PLAT_LISTEN_QUIET:
			#if( TEST_CODE_JUST_FOR_TEST_LUOYANG == 1) 
			Len = strlen( (char*)&sTelNum.Center[0] );
			PhoneCall.DialFlag = TRUE;																 
		   PhoneCall.Length = Len;
			PhoneCall.ListeningFlag = TRUE;			
			memmove((uchar*)&PhoneCall.Buffer[0],(char*)&sTelNum.Center[0], Len);		// ���������ʱʹ�����õ����ĺ���
			
			#else
			
		  	//fsd fs
			s = OneFrame[DataBlockAddr];
			SetBufferValue(Tp.Buffer, 17, 0);
			if(s==0)
			{
			   DFE.Read(EE_JT_JT_NUMBER, 16, (uchar *)Tp.Buffer);
				Len = strlen((char *)Tp.Buffer); 
			}
			else
			{
			   Len = MyStrLen((char *)&OneFrame[DataBlockAddr+2], '#');			
				memmove((uchar *)Tp.Buffer, &OneFrame[DataBlockAddr+2], Len );
			}

			if((result = CprDataValue(Len, 12 ,(uchar) '0', (uchar) '9', Tp.Buffer))==0)
			{  
			   PhoneCall.DialFlag = TRUE;																 
			   PhoneCall.Length = Len;
				PhoneCall.ListeningFlag = TRUE;
		      memmove((uchar*)&PhoneCall.Buffer[0],(uchar *)Tp.Buffer, Len);
			}
			#endif
			Uart.puts("Listenning cmd...");
		   ComtAck = 1;
		break;
	   /*
		/////////�绰�ز� ��Ϣ�� \x00 13580474097��ͨͨ��; \x01 13580474097 ����;������Ч
		*/
		case    eBB_gCMD_PLAT_TEL_DIAL:
			
			
			s = OneFrame[DataBlockAddr];
			SetBufferValue(Tp.Buffer, 20, 0);
			Len = DataLen - 1;
			memmove((uchar *)Tp.Buffer, &OneFrame[DataBlockAddr+1], Len );	
			if((result = CprDataValue(Len, 12 ,(uchar) '0', (uchar) '9', Tp.Buffer))==0)
			{  
			    PhoneCall.DialFlag = TRUE;																 
			    PhoneCall.Length = Len;
				PhoneCall.ListeningFlag = TRUE;
				if(s==0)
				  PhoneCall.ListeningFlag = 0;
		      memmove((uchar*)&PhoneCall.Buffer[0],(uchar *)Tp.Buffer, Len);		  //�绰�ز�ʱ��ʾ������ʾ���Ž��̺�����ͨ����ʾ������֧��
		      PhoneCall.CenterCall = TRUE;
			}
			else
			  result = 3;
			
			Uart.puts("Callback cmd...");
		   ComtAck = 1;
		break;
		/*
		/////////�������� ���͵�          1,0 ��̬   1,1��̬   0,0 �ָ�
		*/
		case   eBB_gCMD_PLAT_CUT_POWER:				 
			
			s = OneFrame[DataBlockAddr];
			tmp = OneFrame[DataBlockAddr+2];

			if(((s!='0')&&(s!='1'))||((tmp!='0')&&(tmp!='1')))										
			{
				result = 3;
			}
			else if(s=='1')												 		// ����·
			{
				if(tmp=='1')											 		// ��̬����·
				{
					VehicleControl.Lock = LOCK_VEHICLE_ENABLE_INV;					//����������ʱ������
				}
				else 																// ���Ͷϵ�
				{		
					VehicleControl.ForceLock = 0x00;
					VehicleControl.Lock = LOCK_VEHICLE_ENABLE_INV;		// ��̬����·		  //����������ʱ������
					gCnt.PulsControl = 30;	  									// 1����
				}		
				DFE.Write(EE_LOCK_FLAG,4,(uchar *)&VehicleControl);	 	// ���ͱ�־д������
			}
			else																	// �ָ���·
			{
				VehicleControl.Lock = 0x00;
				VehicleControl.ForceLock = 0x00; 			  			// 
				DFE.Write(EE_LOCK_FLAG,4,(uchar *)&VehicleControl);	 	// ���ͱ�־д������
			}
			ComtAck = 1;
		break;
		/*	
		//////////����Բ������
		*/
		case   eBB_gCMD_PLAT_SET_ROUND_REGION:
		   
			
		
				         
			ComtAck = 1;	
		break;	
		/*
		//////////ɾ��Բ������
		*/
		case   eBB_gCMD_PLAT_DEL_ROUND_REGION:	
		   
			
		
			        
			
			 ComtAck = 1;
		break;	

		// ���þ�������
		//uchar		s, tel,tmp, k, result = 0, isPack,   ComtAck = 0, n, m,Cmd2,VirtualAuth=0;
		//uchar 	*ptr;
		//uint16	DataLen, MsgId, MsgAttr, Index, DataBlockAddr;//DataBlockAddr;
		//uint32	i=0,j,Len,Addr;
		case eBB_gCMD_PLAT_SET_RECT_REGION:

			ptr = NULL;
			while(ptr==NULL)	{	ptr = MyMalloc(512);	os_dly_wait(1);	}
			s = OneFrame[DataBlockAddr++];		// ״̬����
			m = OneFrame[DataBlockAddr++];		// Χ������
			if( s == 0 )			// ����
			{
				
			}
			else if( s == 1 )		// ׷��
			{				
				//DFE.Read(EE_RECTANGLE_FENCE_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RectFenceAttr);			
				// ����������ȡ����Ϊϵͳ����ʱ�Ѷ�ȡ
				if(RectFenceAttr.Total>=128)
				{
					MyFree(ptr);
					ptr = NULL;
					ComtAck = 1;
		   		result = 1;		// ʧ��Ӧ��		   	
					Uart.puts("����������Χ����������ɾ�� ");
					break;
				}
				
				//7E 86 02 00 18 01 38 86 08 88 88 16 CA 
				//01 01 	00 00 00 66 	40 00 	02 62 54 81 	06 BC 3E 40 	02 38 A9 58 	07 17 CB C0 	70 7E 
				// ��ҵ�ڲ�����ľ�γ�ȱ�JT808��10��
				for(i=0;i<m;i++)
				{
					Addr = FF_RECTANGLE_FENCE_START_PAGE;
					Addr += (RectFenceAttr.Total/8);
					
					DFF.PageRead( Addr, ptr);				// �ȶ�ȡ���ٲ���������ǰ8����Ҫ����
					
					if(Addr%FF_PAGE_PER_SECTOR==0)
					{
						DFF.SectorEraseWaitOK( Addr/FF_PAGE_PER_SECTOR ,10);						
					}
					
					pFence = (pELE_FENCE_STRUCT)( ptr+((RectFenceAttr.Total%8)*LEN_ELE_FENCE_STRUCT) );		// ָ�򹫹�������
					Tp1.Buffer[3] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[2] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
					pFence->ID = GPS.TestOneRectFence.ID = Tp1.B32;							// Χ��ID
					Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
					pFence->Attr.Word = GPS.TestOneRectFence.Attr.Word = Tp1.B16[0];	//����
					
					Tp1.Buffer[3] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[2] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
					pFence->Lat1 = GPS.TestOneRectFence.Lat1 = Tp1.B32*10;		// ���Ͻ�γ��	ע����ͬ�������Ϸ�ʽ
					Tp1.Buffer[3] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[2] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
					pFence->Lon1 = GPS.TestOneRectFence.Lon1 = Tp1.B32*10;		// ���ϽǾ���
					Tp1.Buffer[3] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[2] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
					pFence->Lat2 = GPS.TestOneRectFence.Lat2 = Tp1.B32*10;		// ���½�γ��
					Tp1.Buffer[3] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[2] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
					pFence->Lon2 = GPS.TestOneRectFence.Lon2 = Tp1.B32*10;		// ���½Ǿ���
					
					pRtc = (pRTC_STR)&OneFrame[DataBlockAddr];
					pFence->StartTime = GPS.TestOneRectFence.StartTime = Rtc.DateTimeToSecond( pRtc );
					DataBlockAddr += 6;
					pRtc = (pRTC_STR)&OneFrame[DataBlockAddr];
					pFence->EndTime = GPS.TestOneRectFence.EndTime = Rtc.DateTimeToSecond( pRtc );
					DataBlockAddr += 6;
					
					if( pFence->Attr.Bits.SpeedLimit)
					{					
						Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
						Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
						pFence->LimitSpeed = GPS.TestOneRectFence.LimitSpeed = Tp1.B16[0];			// ȡ��8λ				
						pFence->ContinueTime = GPS.TestOneRectFence.ContinueTime = OneFrame[DataBlockAddr++];
					}
					else
					{
						pFence->LimitSpeed = 0;						
						pFence->ContinueTime = 0;
					}
										
					DFF.PageProgramWaitOK( Addr,ptr,3);
					
					memmove( (uchar*)&GPS.TestOneRectFence,(uchar*)pFence,LEN_ELE_FENCE_STRUCT);
					
					Uart.puts("Զ��׷�Ӿ��ε���Χ�������ԣ�");
					if( (pFence->Attr.Word)&0x0001)	Uart.puts("����ʱ�䡢");
					if( (pFence->Attr.Word)&0x0002)	Uart.puts("���١�");
					if( (pFence->Attr.Word)&0x003c)	Uart.puts("����Χ��������");
					if( (pFence->Attr.Word)&0x4000)
					{
						ReadyTestBlind = TRUE;
						Uart.puts("�ر�GSMͨѶ��");
					}
					os_dly_wait(5);
					
					k = RectFenceAttr.Total>>3;				// ����8
					tmp = RectFenceAttr.Total%8;					
					RectFenceAttr.Enable[k] |= (1<<tmp);	// ʹ�ܶ�Ӧ��Χ����־
					
					RectFenceAttr.Total++;
				}
				DFE.Write(EE_RECTANGLE_FENCE_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RectFenceAttr);
			}
			else if( s == 2 )		// �޸�
			{
				
			}
			MyFree(ptr);
			ptr = NULL;
			ComtAck = 1;
			
		break;		// ��  Q 602045858


		case eBB_gCMD_PLAT_DEL_RECT_REGION:
			
			m = 	OneFrame[DataBlockAddr++];
			if(m==0)		// ɾ�����о���Χ��
			{
				SetBufferValue((uchar*)&RectFenceAttr,LEN_EE_FENCE_ATTR,0);
				DFE.Write(EE_RECTANGLE_FENCE_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RectFenceAttr);
								
				Uart.puts("Զ��ɾ�����о��ε���Χ�� ");
			}
			else
			{
				if(m<RectFenceAttr.Total)		RectFenceAttr.Total -= m;
				// �Ƚ�ID��ɾ�����Ӧ��Χ����
				
				DFE.Write(EE_RECTANGLE_FENCE_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RectFenceAttr);
			}
			
			ComtAck = 1;
		break;
		
		/////////////////////////////////////////////////////// �����
		//uchar		s, tel,tmp, k, result = 0, isPack,   ComtAck = 0, n, m,Cmd2,VirtualAuth=0;
		//uchar 	*ptr;
		//uint16	DataLen, MsgId, MsgAttr, Index, DataBlockAddr;//DataBlockAddr;
		//uint32	i=0,j,Len,Addr;
		case eBB_gCMD_PLAT_SET_POLY_REGION:
			
			//DFE.Read(EE_ROAD_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&PolygonFenceAttr);
		   // ����������ȡ����Ϊϵͳ����ʱ�Ѷ�ȡ
		   if( (PolygonFenceAttr.Total>=128) )
		   {
		   	result = 1;		// ʧ��Ӧ��
		   	ComtAck = 1;
		   	Uart.puts("�����Χ���洢�ռ�����������ɾ�� ");
		   	break;
		   }
			
			ptr = NULL;
			while(ptr==NULL)	{	ptr = MyMalloc(512);		os_dly_wait(1); }
			
			
			pPolygon = (pPOLYGON_STRUCT)(ptr);
			
			Tp1.Buffer[3] = OneFrame[DataBlockAddr++];
			Tp1.Buffer[2] = OneFrame[DataBlockAddr++];
			Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
			Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
			pPolygon->ID = Tp1.B32;				// �����ID
			
			Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
			Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
			pPolygon->Attr.Word = Tp1.B16[0];		// ����

			if( (pPolygon->Attr.Word)&0x0001)
			{			
				pRtc = (pRTC_STR)&OneFrame[DataBlockAddr];
				pPolygon->StartTime = Rtc.DateTimeToSecond( pRtc );
				DataBlockAddr += 6;							// ��ʼʱ��
				
				pRtc = (pRTC_STR)&OneFrame[DataBlockAddr];
				pPolygon->EndTime = Rtc.DateTimeToSecond( pRtc );
				DataBlockAddr += 6;							// ����ʱ��
			}
			
			if( pPolygon->Attr.Bits.SpeedLimit)
			{					
				Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
				Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
				pPolygon->LimitSpeed = Tp1.B16[0];						
				pPolygon->ContinueTime = OneFrame[DataBlockAddr++];
			}
			else
			{
				pPolygon->LimitSpeed = 0;						
				pPolygon->ContinueTime = 0;
			}
			Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
			Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
			m = pPolygon->DotNumber = Tp1.B16[0];
			
			if(m>MAX_POLY_DOT_NUMBER)
			{
		   	result = 1;		// ʧ��Ӧ��
		   	ComtAck = 1;
		   	MyFree(ptr);
		   	Uart.puts("���󣬶������ඥ����Ϊ30 ");
		   	break;
		   }
			//// ��ҵ�ڲ�����ľ�γ�ȱ�JT808��10��
			for(i=0;i<m;i++)
			{
				Tp1.Buffer[3] = OneFrame[DataBlockAddr++];
				Tp1.Buffer[2] = OneFrame[DataBlockAddr++];
				Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
				Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
				pPolygon->Dot[i].Lat = Tp1.B32*10;
				
				Tp1.Buffer[3] = OneFrame[DataBlockAddr++];
				Tp1.Buffer[2] = OneFrame[DataBlockAddr++];
				Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
				Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
				pPolygon->Dot[i].Lon = Tp1.B32*10;
			}
				
			
			
			Uart.puts("Զ��׷�Ӷ���ε���Χ�������ԣ�");
			if( (pPolygon->Attr.Word)&0x0001)	Uart.puts("����ʱ�䡢");
			if( (pPolygon->Attr.Word)&0x0002)	Uart.puts("���١�");
			if( (pPolygon->Attr.Word)&0x003c)	Uart.puts("����Χ��������");
			if( (pPolygon->Attr.Word)&0x4000)	Uart.puts("�ر�GSMͨѶ��");
			os_dly_wait(5);
			
			k = PolygonFenceAttr.Total>>3;				// ����8
			tmp = PolygonFenceAttr.Total%8;					
			PolygonFenceAttr.Enable[k] |= (1<<tmp);	// ʹ�ܶ�Ӧ��Χ����־
			
			PolygonFenceAttr.Total++;

			DFE.Write(EE_POLYGON_FENCE_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&PolygonFenceAttr);
			
			MyFree(ptr);
			ComtAck = 1;
		break;
			
		case eBB_gCMD_PLAT_DEL_POLY_REGION:
		
			m = 	OneFrame[DataBlockAddr++];
			if(m==0)		// ɾ������
			{
				SetBufferValue((uchar*)&PolygonFenceAttr,LEN_EE_FENCE_ATTR,0);
				DFE.Write(EE_POLYGON_FENCE_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&PolygonFenceAttr);
				Uart.puts("Զ��ɾ�����ж���ε���Χ�� ");
			}
			else
			{				
				if(m<PolygonFenceAttr.Total)		PolygonFenceAttr.Total -= m;
				// �Ƚ�ID��ɾ�����Ӧ��Χ����
				
				DFE.Write(EE_POLYGON_FENCE_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&PolygonFenceAttr);
			}
			
			ComtAck = 1;
		break;

		
		// ����·��
		//uchar		s, tel,tmp, k, result = 0, isPack,   ComtAck = 0, n, m,Cmd2,VirtualAuth=0;
		//uchar 	*ptr;
		//uint16	DataLen, MsgId, MsgAttr, Index, DataBlockAddr;//DataBlockAddr;
		//uint32	i=0,j,Len,Addr;
		case   eBB_gCMD_PLAT_SET_PATH:
			
		   
		   		   
		   //DFE.Read(EE_ROAD_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RoadAttr);
		   // ����������ȡ����Ϊϵͳ����ʱ�Ѷ�ȡ
		   if( RoadAttr.Total>=128 ) 
		   {
		   	result = 1;		// ʧ��Ӧ��
		   	ComtAck = 1;
		   	Uart.puts("�������·����128������ɾ�� ");
		   	break;
		   }
			
			ptr = NULL;
			while(ptr==NULL)	{	ptr = MyMalloc(512);		os_dly_wait(1); }
			SetBufferValue( ptr,512,0);
			
			pRoadLine = (pROAD_LINE_STRUCT)(ptr);
			
			Tp1.Buffer[3] = OneFrame[DataBlockAddr++];
			Tp1.Buffer[2] = OneFrame[DataBlockAddr++];
			Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
			Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
			pRoadLine->ID = Tp1.B32;			// ·��ID
			
			Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
			Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
			pRoadLine->Attr.Word = Tp1.B16[0];		// ����

			if( (pRoadLine->Attr.Word)&0x0001)
			{ 			
				pRtc = (pRTC_STR)&OneFrame[DataBlockAddr];
				pRoadLine->StartTime = Rtc.DateTimeToSecond( pRtc );
				DataBlockAddr += 6;							// ��ʼʱ��
				
				pRtc = (pRTC_STR)&OneFrame[DataBlockAddr];
				pRoadLine->EndTime = Rtc.DateTimeToSecond( pRtc );
				DataBlockAddr += 6;							// ����ʱ��
			}
			else
			{
				pRoadLine->StartTime = 0;
				pRoadLine->EndTime = 0;
			}
			
			Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
			Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
			m = pRoadLine->DotNumber = Tp1.Buffer[0];	// �ܹյ���
			
			if(m>MAX_ROAD_DOT_NUM)
			{
		   	result = 1;		// ʧ��Ӧ��
		   	ComtAck = 1;
		   	MyFree(ptr);
		   	Uart.puts("ͬһ��·�����29��·�� ");
		   	break;
		   }
			
			for(i=0;i<m;i++)									// ������յ�
			{	
				Tp1.Buffer[3] = OneFrame[DataBlockAddr++];
				Tp1.Buffer[2] = OneFrame[DataBlockAddr++];
				Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
				Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
				pRoadLine->Dot[i].ID = Tp1.B32;
								
				Tp1.Buffer[3] = OneFrame[DataBlockAddr++];
				Tp1.Buffer[2] = OneFrame[DataBlockAddr++];
				Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
				Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
				pRoadLine->Dot[i].SecID = Tp1.B32;
				
				// ��ҵ�ڲ�����ľ�γ�ȱ�JT808��10��
				Tp1.Buffer[3] = OneFrame[DataBlockAddr++];
				Tp1.Buffer[2] = OneFrame[DataBlockAddr++];
				Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
				Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
				pRoadLine->Dot[i].Lat = Tp1.B32*10;		// �յ�γ��
				
				Tp1.Buffer[3] = OneFrame[DataBlockAddr++];
				Tp1.Buffer[2] = OneFrame[DataBlockAddr++];
				Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
				Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
				pRoadLine->Dot[i].Lon = Tp1.B32*10;		// �յ㾭��
				
				pRoadLine->Dot[i].Width = OneFrame[DataBlockAddr++];			// ·��
				pRoadLine->Dot[i].Attr.Byte = OneFrame[DataBlockAddr++];	// ·������
				
				if( pRoadLine->Dot[i].Attr.Bits.DriveTime==1)
				{
					Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
					pRoadLine->Dot[i].MaxDriveTime = Tp1.B16[0];				// �������ʻʱ��
					
					Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
					pRoadLine->Dot[i].MinDriveTime = Tp1.B16[0];				// ���������ʻʱ��
				}
				else
				{
					pRoadLine->Dot[i].MaxDriveTime = 0;
					pRoadLine->Dot[i].MinDriveTime = 0;
				}
				if( pRoadLine->Dot[i].Attr.Bits.LimitSpeed==1)
				{
					Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
					pRoadLine->Dot[i].LimitSpeed = Tp1.B16[0];							// ����									
					pRoadLine->Dot[i].ContinueTime = OneFrame[DataBlockAddr++];	// ��ʱ����ʱ��
				}
				else
				{
					pRoadLine->Dot[i].LimitSpeed = 0;					// ����									
					pRoadLine->Dot[i].ContinueTime = 0;				// ��ʱ����ʱ��
				}
			}
			
			Len = m ;
			Len *= LEN_ROAD_DOT_STRUCT;
			Len += LEN_ROAD_LINE_STRUCT;			// ��洢��·�����ݳ���
			
			Addr = FF_ROAD_PLAN_START_PAGE;		// 
			Addr += (RoadAttr.Total<<1);			// ÿ��·�߹̶��洢����Ϊ2ҳFlash
						
						
			// Ӧ�����ԣ��ȴ洢�����磨��1����
			#if( TEST_CODE_JUST_FOR_TEST_LUOYANG_NEW==1)
			DFE.Write(EE_ONE_ROAD_LINE_DATA,256,ptr);
			TestRoadLine = TRUE;
			#endif
				
			for(i=0;i<Len;i+=FF_BYTES_PER_PAGE)
			{
				if(Addr%FF_PAGE_PER_SECTOR==0)
				{
					DFF.SectorEraseWaitOK( Addr/FF_PAGE_PER_SECTOR ,10);						
				}
				DFF.PageProgramWaitOK( Addr, ptr,5);
				ptr += FF_BYTES_PER_PAGE;
				Addr++;				
			}
			
			TD.GPS = tGPS_USE_GPS ;
			OnlyUseBD_GPS = 0;
			
			Uart.puts("Զ��׷��·�߹滮�����ԣ�");
			if( (pRoadLine->Attr.Word)&0x0001)	Uart.puts("����ʱ�䡢");
			if( (pRoadLine->Attr.Word)&0x003c)	Uart.puts("����Χ��������");
			if( (pRoadLine->Attr.Word)&0x0002)	Uart.puts("���� ");
			os_dly_wait(5);
			
			k = RoadAttr.Total>>3;				// ����8
			tmp = RoadAttr.Total%8;					
			RoadAttr.Enable[k] |= (1<<tmp);	// ʹ�ܶ�Ӧ��Χ����־
			
			RoadAttr.Total++;		
			DFE.Write(EE_ROAD_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RoadAttr);
			
		   MyFree(ptr);
			ComtAck = 1;		  
		break;	
		 /*
		 //////////ɾ��·��
		 */
		case   eBB_gCMD_PLAT_DEL_PATH:
		   
				             
			m = 	OneFrame[DataBlockAddr++];
			if(m==0)		// ɾ������
			{
				SetBufferValue((uchar*)&RoadAttr,LEN_EE_FENCE_ATTR,0);
				DFE.Write(EE_ROAD_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RoadAttr);
				
				Uart.puts("Զ��ɾ������·�߼ƻ� ");
			}
			else
			{
				DFE.Read(EE_ROAD_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RoadAttr);
				if(m<RoadAttr.Total)		RoadAttr.Total -= m;
				// �Ƚ�ID��ɾ�����Ӧ��·��
				
				DFE.Write(EE_ROAD_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RoadAttr);
			}
			
			ComtAck = 1;	   
		
		break;
		/*
		////////////��¼�����ݲɼ�
		*/
		case   eBB_gCMD_PLAT_GET_VDR_DATA:
			
			m = OneFrame[DataBlockAddr];
		
			/*EnableVDR_Upload = TRUE;
						
			if(m<eGB_rCMD_GET_SPEED_PER_SECOND)		
			{
				VDR_CmdTab[m] = m;
				GetVDR_Index[m] = Index;
				//if( m == eGB_rCMD_GET_STD_VERSION )
				{		 
					GetVDR_DataFlag = TRUE;
					GetVDR_Cmd = 0;
				}
			}
			else	//if(m>=eGB_rCMD_GET_SPEED_PER_SECOND)
			{
				if(m<64)			GetVDR_Index[m] = Index;
				NOP();	NOP();
				switch(m)
				{
					case eGB_rCMD_GET_SPEED_PER_SECOND:	
						FlashPageCnt.Speed48HSave = 1440;				// 126B/���ӣ�2����/ҳ��
						FlashPageCnt.Speed48HRead = 0;
					break;
					case eGB_rCMD_GET_POSITION_INFO:	
						FlashPageCnt.Location360HSave = 1080;			// 666B/Сʱ��3ҳ/Сʱ
						FlashPageCnt.Location360HRead = 0;
					break;
					case eGB_rCMD_GET_PARKING_DATA:	
						FlashPageCnt.ParkingDataSave = 100;				// 234B/����1��/ҳ��100��
						FlashPageCnt.ParkingDataRead = 0;
					break;
					case eGB_rCMD_GET_OVER_TIME_DRIVE:	
						FlashPageCnt.FatigueDrivingSave = 20;			// 50B/����5��/ҳ��100��
						FlashPageCnt.FatigueDrivingRead = 0;
					break;
					case eGB_rCMD_GET_GET_LOG_IN_OUT_RECORD:	
						FlashPageCnt.DriverLogInOutSave = 20;			// 25B/����10��/ҳ��200��
						FlashPageCnt.DriverLogInOutRead = 0;
					break;
					case eGB_rCMD_GET_POWER_RECORD:	
						FlashPageCnt.PowerOnOffSave = 6;					// 7B/����36��/ҳ����108��
						FlashPageCnt.PowerOnOffRead = 0;
					break;
					case eGB_rCMD_GET_MODIFY_PARA_RECORD:				
						FlashPageCnt.ParaModifySave = 3;					// 7B/����36��/ҳ����108��
						FlashPageCnt.ParaModifyRead = 0;
					break;
					case eGB_rCMD_GET_SPEED_STATUS_LOG:
						FlashPageCnt.SpeedStatusSave = 6;					// 7B/����36��/ҳ����108��
						FlashPageCnt.SpeedStatusRead = 0;
					break;
					
				}
			} */

			s = OK;  // ERROR�� �����־�� OK���޴���
			if(m<eGB_rCMD_GET_SPEED_PER_SECOND) 	//  ���� 00H - 07H ƽ̨�·��� AA 75 	
			{
				GetVDR_DataFlag = TRUE;
				GetVDR_Cmd = m;
				GetVDR_Index[m] = Index;
			}
			else 	if (m <= eGB_rCMD_GET_SPEED_STATUS_LOG)	  // ����08H - 15H �� ƽ̨�·�  AA 75 
			{

				if (DataLen>=6)
				{
						Len = DataLen-1;
						
			  			Uart.GB19056Process(Len,&OneFrame[DataBlockAddr+1],&Tp.Buffer[3]);  // 
						GB_LargeData.GetVDRFlag = FALSE;  //  ���豾�����
						JT_VehRecord.TotalPack	= 0 ;
						JT_VehRecord.PackIndex = 0;
						JT_RemoteReadRecodMoreDataCmd = m;	//  ����ֻ�����ñ�־��ʵ�ʵ�������GPRSCls::PopAlignmentSend(uchar Signal)����
						GetVDR_Index[m] = Index;
						GetVDR_Cmd = m;

						 
				}
				else
				{
					s = ERROR;
				}
			}
			else				//   ����Ĳɼ�����
			{
				s = ERROR;
				JT_RemoteReadRecodMoreDataCmd = 0;
			}

			if (s==ERROR)
			{
				Tp1.B16[0] = Index;
				Tp.Buffer[0] = Tp1.Buffer[1];
				Tp.Buffer[1] = Tp1.Buffer[0];
				Tp.Buffer[2] = m; 		  //  ƽ̨�·���������
				Tp.Buffer[3] = 0x55;
				Tp.Buffer[4] = 0x7a;
				Tp.Buffer[5] = 0xfa;
				Tp.Buffer[6] = 0x00;
				Tp.Buffer[7] = CalXorSum(8,&Tp.Buffer[3]);
				Length = 8;	
	
				AckBuffer.MsgId = MsgId;
	 			AckBuffer.Index = Index;	
	  			BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_VDR_DATA_UPLOAD,Length,Tp.Buffer);
				Uart.puts("Զ�̲ɼ��������ִ���");	
			}
		  #if (0)//#if (WUXI_TEST != 1)		// ��ѩ������ƽ̨ʱ�����û�ͨ��Ӧ��
		  ComtAck = 1;	
		  #endif   
		
		break;	
		 /*
		 ///////////��¼�������´�
		 */
		case   eBB_gCMD_PLAT_SET_VDR_PARA:
		  
		  //		  GetGB_Data_Flag = 3;	       
//		  Uart.SetGB_Down_CMDHead(U0_RecBuffer,DataLen-1, OneFrame[DataBlockAddr]);
//		  memmove(&U0_RecBuffer[LEN_U0_GB_FRAME_HEAD], &OneFrame[DataBlockAddr+1], DataLen-1);
//		  Uart.SendDownCMDtoU0(U0_RecBuffer, LEN_U0_GB_FRAME_HEAD+DataLen-1);
//		  GetGB_Data_Flag = 0;
		  m = OneFrame[DataBlockAddr];		// ����������
		  s = OK;
		  if (DataLen<6)
		  {
		  	  s = ERROR;
		  }
		  else
		  {
			  //k =CalXorSum(DataLen-2, &OneFrame[DataBlockAddr+1]);		 // ��������һ���ֽڵ�У���ֽ�,���Գ��ȼ�2	 
			  if ((((m>=eGB_rCMD_SET_VEHICLE_VIN) && (m<= eGB_rCMD_SET_SIGNAL_STATUS)) ||			// 82H - 84H
			  	  ((m>=eGB_rCMD_SET_REAL_TIME) && (m<= eGB_rCMD_SET_INIT_MIL)) ))				    //   C2H - C4H
			  
			  {
			  		Len = DataLen-1;
			  		Length = Uart.GB19056Process(Len,&OneFrame[DataBlockAddr+1],&Tp.Buffer[3]);
					if (Tp.Buffer[3+2] == 0xfb)
					{ 
						s = ERROR;
					}
	
			  }
			  else
			  {
			  		s = ERROR;
			  }
		  
		  }
		  
		  if (s==ERROR)
		  {
				result = 1;	   	
			
				Uart.puts("Զ���´�������������");;	
		  }

	   	AckBuffer.MsgId = MsgId;
		 	AckBuffer.Index = Index;	

		  	//BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_VDR_DATA_UPLOAD,Length,Tp.Buffer);

		   ComtAck = 1;	  				// �±�׼��Ϊ����Ҫ�ظ�ͨ��Ӧ�������
		
		break;
		 /*
		 ///////////�����豸SIM������
		 */
		case   eBB_gCMD_SET_SIM_NUMBER:					  //
		   
		   SetBufferValue(sTelNum.Sim, 16, 0);
			sTelNum.Sim[0] = 0x30;
		    memmove((uchar*)&sTelNum.Sim[1],(uchar*)&OneFrame[DataBlockAddr],11);
			//sTelNum.Sim[16] = 0;
			Length = strlen((char *)&sTelNum.Sim[0]);
			DFE.Write(EE_SIM_NUMBER,16,(uchar*)&sTelNum.Sim[0]);
			//Uart.putb(1, 16, &sTelNum.Sim[0]);  os_dly_wait(5);
			DecCharToPressBCD(Length,(char *)sTelNum.Sim,SimNumberBCD);
			//for(i=0;i<8;i++)	{ if(SimNumberBCD[i]==0)	SimNumberBCD[i] = 0x0f;	}
			memmove(sTelNum.SimBCD_JT, SimNumberBCD, 6);
			result = 0;
			ComtAck = 1;
		 break;
		/*
		 ///////////��ѯ�豸SIM������
		*/
		case   eBB_gCMD_GET_SIM_NUMBER:						 //
		  
		  memmove(OneFrame, (uchar *)&Index, 2);				//Ӧ�����ˮ��
		  Len = strlen((char *)&sTelNum.Sim[1]);
        memmove(&OneFrame[2],&sTelNum.Sim[1], Len);
		  Len += 2;
		  GPRS.JT808_DataPackPushAlignment(0,eBB_gCMD_GET_SIM_NUMBER_ACK, Len, 0, OneFrame);
		  Uart.puts("��ѯSIM ����Ӧ�� ");	 
		  ComtAck = 3;

		break;
		/*
		 ///////////����ADͨ��������������/�ͼ���־
		*/
		case  eBB_gCMD_SET_ADSW_FLAG:
		  
		  HighLowLevel  = ByteToINT32(OneFrame[DataBlockAddr], OneFrame[DataBlockAddr+1],OneFrame[DataBlockAddr+2],OneFrame[DataBlockAddr+3]);;
		  DFE.Write(EE_HIGH_LOW_LEVEL,2,(uchar*)&HighLowLevel);
		  ComtAck=1;
		  result = 0;

		break;
		/*
		 ///////////��ѯADͨ��������������/�ͼ���־
		*/
		case  eBB_gCMD_GET_ADSW_FLAG:
		 
		 i = SwapINT32(HighLowLevel);
		 GPRS.JT808_DataPackPushAlignment(0,eBB_gCMD_GET_ADSW_FLAG_ACK, 4, 0, (uchar *)&i);
		 Uart.puts("��ѯ������Ӧ�� ");	 
		 ComtAck = 3;
		break;
		/*
		 ///////////�ı���Ϣ�·�
		*/
		case  eBB_gCMD_PLAT_SEND_TEXT_INFO:
		 
		 //Uart.puts("CCC1");os_dly_wait(1);

		 s = OneFrame[DataBlockAddr];
		 Len = Length - 1;
		 if(Len>LEN_CMD_DATA_TO_LCM)
		   Len = LEN_CMD_DATA_TO_LCM;
		 if(s&0x04)		  //���͵��ֱ�/��ʾ��
		 {
		    KeyDisp.CenterSendDataToLCM_YH(Len, (uchar *)&OneFrame[DataBlockAddr + 1]);

		 }
		 else if(s&0x08)	
		 {
		 	OneFrame[DataBlockAddr] = 0x47; // GB�� ������
			Len += 1;
		 	KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG, Len, (uchar *)&OneFrame[DataBlockAddr]);
		 }	   //��������
		 else if(s&0x10)  {}		//�������ʾ
		 //GPRS.JT_T808_NewDataPack(eBB_gCMD_GET_ADSW_FLAG_ACK, 4, 0, 0, (uchar *)&i);
		 ComtAck = 1;
		break;
		/*
		 ///////////��Ϣ����
		*/
		case  eBB_gCMD_PLAT_INFO_SERVICE:
		 
		 s = OneFrame[DataBlockAddr];
		 Len = Length - 3;
		 if(Len>LEN_CMD_DATA_TO_LCM)
		   Len = LEN_CMD_DATA_TO_LCM;
		 //KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_MSG, Len, (uchar *)&OneFrame[DataBlockAddr + 3]);		  //���͵��ֱ�/��ʾ��
		 KeyDisp.CenterSendDataToLCM_YH(Len, (uchar *)&OneFrame[DataBlockAddr + 3]);		  //���͵��ֱ�/��ʾ��
		 ComtAck = 1;
		break;
	   /*
		 ///////////�����·�
		*/
		case  eBB_gCMD_PLAT_SEND_ASK:
		 
		 Plat_Serial_No = Index;
		 s = OneFrame[DataBlockAddr];
		 tel = OneFrame[DataBlockAddr + 1];
		 memmove(Tp.Buffer, &OneFrame[DataBlockAddr + 2], tel);

		 Len = Length - tel - 2;
		 if((Len+tel)>LEN_CMD_DATA_TO_LCM)
		   Len = LEN_CMD_DATA_TO_LCM - tel;
		 tmp = tel + 2;
		 while(Len)
		 {
		 	 n = OneFrame[DataBlockAddr + tmp];
			 if(n>=10)	 //���10����ѡ��
			   break;
			 DataLen = OneFrame[DataBlockAddr + tmp + 1];
			 DataLen <<= 8;
			 DataLen = OneFrame[DataBlockAddr + tmp + 2];
			 if(Len>=(DataLen+3))
			 {
			   Len -= DataLen;
				Len -= 3;
			 }
			 else
			 {
			   DataLen = Len;
			   Len = 0;
			 }
			 n += 0x30;
			 Tp.Buffer[tel] = n;
			 tel++;
			 Tp.Buffer[tel] = 0x2E;
			 tel++;
			 memmove(&Tp.Buffer[tel], &OneFrame[DataBlockAddr + tmp + 3], DataLen);
			 tel += DataLen;
			 tmp = tmp + 3 + DataLen;
		 }
		 GG_Evt.GPRS.AckGSMS = 1;
		 //KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_MSG, tel, (uchar *)Tp.Buffer);		  //���͵��ֱ�/��ʾ��
		 if (tel < 256 - 20) { memmove(Tp.Buffer+tel, (uchar *)"\xA3\xA8\xD7\xA2\xA3\xBA\xC7\xEB\xBB\xD8\xB8\xB4\xB4\xF0\xB0\xB8\xD0\xF2\xBA\xC5\xA3\xA9", 11 * 2); tel += 22; }
		 KeyDisp.CenterSendDataToLCM_YH(tel, (uchar *)Tp.Buffer);		  //���͵��ֱ�/��ʾ��
		 ComtAck = 1;
		break;
												
		/*
		//////////////////
		//   ��ѯ�豸�汾����Ϣ
		*/	
		case  eBB_gCMD_GET_VERSION_MSG:
		 SetBufferValue(Tp.Buffer, 50, 0);
		 U0_Iap_Flag = 9;
		 Sys.GetVersion((char *)Tp.Buffer);
		 U0_Iap_Flag = 0;
		 DataLen = strlen((char *)Tp.Buffer);
		 GPRS.JT808_DataPackPushAlignment(0,eBB_gCMD_GET_VERSION_MSG_ACK, DataLen, 0, (uchar *)Tp.Buffer);
		 Uart.puts("��ѯ�汾Ӧ�� ");	 
		 ComtAck = 3;
		break;
		
		
		/*
		//////////////////
7E 89 00 00 19 00 01 38 86 08 88 04 02 0B 00 01 5B F7 DD E8 FF 1D 2D 48 00 00 00 00 00 00 00 00 00 00 00 00 B0 AF A2 7E 
		//   ����͸��
		*/	
		case eBB_gCMD_PLAT_SEND_TRAN_DATA:	

			
			ComtAck=1;			// ͸�����ݣ������ȼ�Ȩ
		   //Uart.puts(" ����͸������ ");
			if (OneFrame[DataBlockAddr] == 0x0B)	
			{
				Uart.puts(" ��ҵ�ʸ�֤��֤���Ļظ� ");
				//for (s = 0; s < 3; s++)
				{
					IC_Card_Auth_Flag = TRUE;
					OneFrame[DataBlockAddr] = 0x00;
					SetBufferValue(IC_Card_Auth_Buffer,32,0);
					memmove(IC_Card_Auth_Buffer,&OneFrame[DataBlockAddr], 25);
					if( U2PinSelDevice == U2_FOR_RFID)
					{
						SpeedC.U2_SendDataToRFID(0x40, &OneFrame[DataBlockAddr], 25);
					}
					else
					{
						SpeedC.SendDataToRFID(0x40, &OneFrame[DataBlockAddr], 25);  // 1�ֽ���֤�����24�ֽ���֤���� ͸����RFID
					}
				}	
			}
			else	  // ����͸��
			{
				Uart.putb(1,DataLen,&OneFrame[DataBlockAddr]);
				Uart2.putb(1,DataLen,&OneFrame[DataBlockAddr]);
			}
		
		break;
		
		// added 2013.3
		case eBB_gCMD_PLAT_QUERY_SOME_TER_PARA:							// ��ѯָ���ն˲���
			
		  	PDU_SMS.Ack = 0;          //������ǿ�ƴ�GPRSͨ������	�����SMSͨ���´�Ĳɼ�ָ����ݴ�GPRS����
		  	tmp = 0;
			i = 0;	
			result = 0;
			Len = 1+2;
			
			while((USBTaskCreat!=0) || (PrintTaskCreat!=0)) 
		    {
				os_dly_wait(1);
		    }
		    SetBufferValue((uchar *)&CamBuffer[0], 800, 0);
			
			memmove(CamBuffer, &OneFrame[DataBlockAddr], Length);
		    s = 0;
			while (s < CamBuffer[0])	  // 	CamBuffer[0] �������ǲ�������
			{ 
			  i = ByteToINT32(*(CamBuffer+1+s),*(CamBuffer+2+s), *(CamBuffer+3+s),*(CamBuffer+4+s));
			  result = JT_Set_Get_Para((uchar *)&OneFrame[Len+5], 0, i);
			  if(result!=0)
			  {  
			     j = SwapINT32(i);
				  memmove(&OneFrame[Len],(uchar *)&j, 4);
				  Len += 4;
				  OneFrame[Len] = result;
			      Len += 1;
				  Len += result;
				  tmp++;
			  }
			  s += 4;
			}
			
			OneFrame[0] = Index>>8;
			OneFrame[1] = Index;
			//memmove(OneFrame, (uchar *)&Index, 2);				//Ӧ�����ˮ��
			OneFrame[2] = tmp;
			GPRS.JT808_DataPackPushAlignment(0,eBB_gCMD_TER_QUERY_PARAM_ACK, Len, 0, OneFrame);
			ComtAck = 3;	
			Uart.puts("��ѯ����Ӧ��11 ");	 
		break;

		case		eBB_gCMD_PLAT_QUERY_TER_PRO:								// ��ѯ�ն�����	
			
			BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_QUERY_PRO_ACK,0,NULL);
			Uart.puts("��ѯ�ն�����Ӧ�� ");	 
			ComtAck = 0;	
		
		break;

		// added 2013.4
		case	eBB_gCMD_PLAT_DRIVER_INFO_REQUEST:                              // �ϱ���ʻԱ�����Ϣ����
		case	eBB_gCMD_I2C_DRIVER_INFO_REQUEST:
			
			BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DRIVER_INFO_REPORT,0,NULL);
			Uart.puts("�ϱ���ʻԱ�����Ϣ���� ");	 
			ComtAck = 0;	  // ֱ��Ӧ������ͨ��Ӧ��
		
		break;

		//	0x8301 �¼�����
		case eBB_gCMD_PLAT_SET_EVENT:
			

			Cmd2 = OneFrame[DataBlockAddr+0];
			if( ( Cmd2 == 0) || (Cmd2 == 1) )	// ɾ��ȫ�������¼�
			{
				SetBufferValue(JT808_EventID,TOTAL_EVT_INFO_ID_NUMBER,0);
			}
			m = JT808_EventID[0];				// ��ǰ����
			if( (Cmd2 == 1)||(Cmd2 == 2))	// ׷���¼�
			{
				n = OneFrame[DataBlockAddr+1];	 	// �����β������ܸ���
				Index = DataBlockAddr+2;				// ��1��ID���ڵ�ַ
				for(s=0;s<n;s++)
				{
					if(m<TOTAL_EVT_INFO_ID_NUMBER)
					{
						JT808_EventID[m+1] = OneFrame[Index];		// ��ȡID�ţ����洢��ID�б�
						m++;
					}
					Len = OneFrame[Index++];					
					Index += Len;					
				}
			}
			else if ( Cmd2 == 3 )			// �޸�
			{
				
			}
			else if ( Cmd2 == 4 )			// ɾ���ض�����
			{
				
			}
			

			ComtAck = 1;

		break;


		//	0x8303��Ϣ�㲥�Ų˵�	 0:ɾ��ȫ��  1������   2��׷��   3���޸�	 // ���½��ͣ���ɾ��ȫ�����ٴ���׷�Ӷ���
		case eBB_gCMD_PLAT_SET_INFO_MENU:
			

			Cmd2 = OneFrame[DataBlockAddr+0];
			if( ( Cmd2 == 0) || (Cmd2 == 1) )	// ɾ��ȫ�������¼�
			{
				SetBufferValue(JT808_InfoID,TOTAL_EVT_INFO_ID_NUMBER,0);
			}
			m = JT808_InfoID[0];				// ��ǰ����
			if( (Cmd2 == 1)||(Cmd2 == 2))	// ׷���¼�
			{
				n = OneFrame[DataBlockAddr+1];	 	// �����β������ܸ���
				Index = DataBlockAddr+2;				// ��1��ID���ڵ�ַ
				for(s=0;s<n;s++)
				{
					if(m<TOTAL_EVT_INFO_ID_NUMBER)
					{
						JT808_EventID[m+1] = OneFrame[Index];		// ��ȡID�ţ����洢��ID�б�
						m++;
					}
					Len = OneFrame[Index++];
					Len <<= 8;
					Len += OneFrame[Index++];
					Index += Len;					
				}
			}
			if( Cmd2 == 3 ) // �޸�
			{
				
			}					

			ComtAck = 1;

		break;


		case 	eBB_gCMD_PLAT_SET_TEL_BOOK:

			ComtAck = 1;

		break;
		
		
		///////////////// ¼����ʼ����
		case eBB_gCMD_PLAT_START_RECORD:

			 
			 s = OneFrame[DataBlockAddr++];
			 Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
			 Tp1.Buffer[0] = OneFrame[DataBlockAddr++];	
			 Tp1.Buffer[2] = OneFrame[DataBlockAddr++];
			 
			 
			 if (s == 0x01)		 // ��ʼ¼��
			 {
			 	result = 0;										
			 	Record.StartRecord( Tp1.B16[0]+3 );		// ������ʧ��ʱ�䣬Լ2~3��
			 }
			 else if(s == 0x00)   // ֹͣ¼��
			 {
			 	result = 0;				
			 	Record.StopRecord();
			 }

			 
			 if (Tp1.Buffer[2] == 0x01)		 // ����
			 {
			 	// 
			 }
			 else if (Tp1.Buffer[2] == 0x00)   // ʵʱ�ϴ�
			 {
			 	//
				Record.UploadRecordData(RECORD_ALL_PACK_UPLOAD);
			 }
			ComtAck = 1;

		break;
			
		default:
		
			result = 0;			/// ��֧�ֵ���ϢID
			ComtAck = 1;
			Uart.puts(" �յ�����ָ�����ϢID��֧�� ");
		break;
			//break;		
	}


	if(ComtAck!=0)
	{ 
	  
	  if((ComtAck==1)||(ComtAck==2))
	  {
		 AckBuffer.MsgId = MsgId;
		 AckBuffer.Index = Index;
		 ter_or_veh_reg_result = result;
		 Uart.puts("ͨ��Ӧ�� ");
		 BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_ACK,0,NULL);			//ͨ��Ӧ��		 	 
	  }
	}

	return  OK;								//��������								 
}



extern		uchar		OnlyUseBD_GPS;

uchar       GPRSCls::JT_Set_Get_Para(uchar *p, uchar s, uint32 CmdID)
{
	  uchar  i,j,k,t = 0, tB[33], s1;
	  uint32 Dw = 0;
		union
		{
			uchar		Buffer[32];
			uint16	B16[16];
			uint32	B32[8];
		}Tp;
		
	  SetBufferValue( Tp.Buffer,32,0);
	  switch(CmdID)
	  {

			case		SET_ACC_DEC_PARA	:
				
				if(s!=0)
				{
					k = 0;
					for(i=0;i<5;i++)
					{
						Tp.Buffer[0] =  Tp.Buffer[1] = Tp.Buffer[2] = Tp.Buffer[3] = Tp.Buffer[4] = 0;
						for(j=0;j<5;j++)
						{
							t = *(p+k); 
							k++;
							if((t==',')||(t==';'))	break;
							else	Tp.Buffer[j] = t;														
						}
						j++;
						Dw = DecCharToUint32(j,(char*)Tp.Buffer);
						AccelPara[0].Start[i] 	= Dw;			 		// ��ʼ�ٶ�

						Tp.Buffer[0] =  Tp.Buffer[1] = Tp.Buffer[2] = Tp.Buffer[3] = Tp.Buffer[4] = 0;
						for(j=0;j<5;j++)
						{
							t = *(p+k); 
							k++;
							if((t==',')||(t==';'))	break;
							else	Tp.Buffer[j] = t;														
						}
						j++;
						Dw = DecCharToUint32(j,(char*)Tp.Buffer);
						AccelPara[0].End[i] 		= Dw;					// �����ٶ�

						Tp.Buffer[0] =  Tp.Buffer[1] = Tp.Buffer[2] = Tp.Buffer[3] = Tp.Buffer[4] = 0;
						for(j=0;j<5;j++)
						{
							t = *(p+k); 
							k++;
							if((t==',')||(t==';'))	break;
							else	Tp.Buffer[j] = t;														
						}
						j++;
						Dw = DecCharToUint32(j,(char*)Tp.Buffer);
						Dw *= 36;			// *3600 / 100
						Dw /= 1000;			// m/s2��ת��Ϊkm/s2
						AccelPara[0].Value[i] 	= Dw;	  				// ������ٶ�
					}
					DFE.Write(EE_ACCEL_ADD_DEC_PARA,15,(uchar*)&AccelPara[0]);
					Uart.puts("Accel Dec Test Data:");
					Uart.putb(1,15,(uchar*)&AccelPara[0]);
					Uart.puts("\r\n\0");
				}
				else
				{
					k = 0;
					for(i=0;i<5;i++)
					{
						Dw = AccelPara[0].Start[i];
						Int2Str( Dw, (char*)(p+k) );
						strcat( (char*)p, ",");
						k = strlen( (char*)p);

						Dw = AccelPara[0].End[i];
						Int2Str( Dw, (char*)(p+k) );
						strcat( (char*)p, ",");
						k = strlen( (char*)p);

						Dw = AccelPara[0].Value[i];
						Tp.B32[0] = Tp.B32[1] = 0;
						Dw *= 1000;
						Dw /= 36;
						Int2Str( Dw, (char*)Tp.Buffer );
						*(p+k) = Tp.Buffer[0];	k++;
						*(p+k) = '.';				k++;
						*(p+k) = Tp.Buffer[1];	k++;
						*(p+k) = Tp.Buffer[2];	k++;
						*(p+k) = ';';				k++;
					}
					t = k;
				}
				//AccelPara[0];
			break;

			case		SET_ACC_PARA:

				if(s!=0)
				{
					k = 0;
					for(i=0;i<5;i++)
					{
						Tp.Buffer[0] =  Tp.Buffer[1] = Tp.Buffer[2] = Tp.Buffer[3] = Tp.Buffer[4] = 0;
						for(j=0;j<5;j++)
						{
							t = *(p+k); 
							k++;
							if((t==',')||(t==';'))	break;
							else	Tp.Buffer[j] = t;														
						}
						j++;
						Dw = DecCharToUint32(j,(char*)Tp.Buffer);
						AccelPara[1].Start[i] 	= Dw;			 		// ��ʼ�ٶ�

						Tp.Buffer[0] =  Tp.Buffer[1] = Tp.Buffer[2] = Tp.Buffer[3] = Tp.Buffer[4] = 0;
						for(j=0;j<5;j++)
						{
							t = *(p+k); 
							k++;
							if((t==',')||(t==';'))	break;
							else	Tp.Buffer[j] = t;														
						}
						j++;
						Dw = DecCharToUint32(j,(char*)Tp.Buffer);
						AccelPara[1].End[i] 		= Dw;					// �����ٶ�

						Tp.Buffer[0] =  Tp.Buffer[1] = Tp.Buffer[2] = Tp.Buffer[3] = Tp.Buffer[4] = 0;
						for(j=0;j<5;j++)
						{
							t = *(p+k); 
							k++;
							if((t==',')||(t==';'))	break;
							else	Tp.Buffer[j] = t;														
						}
						j++;
						Dw = DecCharToUint32(j,(char*)Tp.Buffer);
						Dw *= 36;			// *3600 / 100	  (��λС����
						Dw /= 1000;			// m/s2��ת��Ϊkm/s2
						AccelPara[1].Value[i] 	= Dw;	  				// ������ٶ�
					}
					DFE.Write(EE_ACCEL_ADD_DEC_PARA+15,15,(uchar*)&AccelPara[1]);
					Uart.puts("Accel Add Test Data:");
					Uart.putb(1,15,(uchar*)&AccelPara[1]);
					Uart.puts("\r\n\0");
				}
				else
				{
					k = 0;
					for(i=0;i<5;i++)
					{
						Dw = AccelPara[1].Start[i];
						Int2Str( Dw, (char*)(p+k) );
						strcat( (char*)p, ",");
						k = strlen( (char*)p);

						Dw = AccelPara[1].End[i];
						Int2Str( Dw, (char*)(p+k) );
						strcat( (char*)p, ",");
						k = strlen( (char*)p);

						Dw = AccelPara[1].Value[i];
						Tp.B32[0] = Tp.B32[1] = 0;
						Dw *= 1000;
						Dw /= 36;
						Int2Str( Dw, (char*)Tp.Buffer );
						*(p+k) = Tp.Buffer[0];	k++;
						*(p+k) = '.';				k++;
						*(p+k) = Tp.Buffer[1];	k++;
						*(p+k) = Tp.Buffer[2];	k++;
						*(p+k) = ';';				k++;
					}
					t = k;
				}

				//AccelPara[1];
			break;


	     case 	   SET_HEART_BEAT_ID:	                                      // �ն��������ͼ��
		      if(s!=0)
				{
				  SendInterval.HeartbeatTime = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  DFE.Write(EE_JT_HEART_BEAT_TIME, 4,(uchar *)&SendInterval.HeartbeatTime);
				  Uart.puts("Զ�����������������");
				  Int2Str( SendInterval.HeartbeatTime,(char*)Tp.Buffer);
				  strcat((char*)Tp.Buffer," ");
				  Uart.puts( (char*)Tp.Buffer);
					GSM.CalculateSendInterval(0);
				}
				else
				{
				  DFE.Read(EE_JT_HEART_BEAT_TIME, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  memmove(p, (uchar *)&Dw, 4);
				  t = 4;
				}	
 
		   	break;

	     case 		SET_TCP_ACK_OVER_TIME_ID:										     // TCPӦ��ʱʱ��
		  	   if(s!=0)
			   {	
				  SendOVT.TCP_ACK_OverTime = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  DFE.Write(EE_JT_TCP_ACK_TIME, 4,(uchar *)&SendOVT.TCP_ACK_OverTime);
		      }
				else
				{
				  DFE.Read(EE_JT_TCP_ACK_TIME, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  memmove(p, (uchar *)&Dw, 4);
				  t = 4;
				}		

		   	break;
	     case 		SET_TCP_RESEND_TIMES_ID:								           //TCP�ش�����
				if(s!=0)
			   {	
				  SendOVT.TCP_ResendTimes = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  DFE.Write(EE_JT_TCP_RESEND_TIMES, 4,(uchar *)&SendOVT.TCP_ResendTimes);	
		  		}
				else
				{
				  DFE.Read(EE_JT_TCP_RESEND_TIMES, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  memmove(p, (uchar *)&Dw, 4);
				  t = 4;
				}		

		   	break;
	     case 		SET_UDP_ACK_OVER_TIME_ID:											  //  UDPӦ��ʱʱ��
		  		if(s!=0)
			   {	
				  SendOVT.UDP_ACK_OverTime = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  DFE.Write(EE_JT_UDP_ACK_TIME, 4,(uchar *)&SendOVT.UDP_ACK_OverTime);	
				}
				else
				{
				  DFE.Read(EE_JT_UDP_ACK_TIME, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  memmove(p, (uchar *)&Dw, 4);
				  t = 4;
				}		
		   	break;
	     case 		SET_UDP_RESEND_TIMES_ID:											  // UDP�ش�����
		  		if(s!=0)
			   {
					SendOVT.UDP_ResendTimes = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
					DFE.Write(EE_JT_UDP_RESEND_TIMES, 4,(uchar *)&SendOVT.UDP_ResendTimes);	
				}
				else
				{
				  DFE.Read(EE_JT_UDP_RESEND_TIMES, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  memmove(p, (uchar *)&Dw, 4);
				  t = 4;
				}		
		   	break;
	     case 		SET_SMS_ACK_OVER_TIME_ID:											  //  SMSӦ��ʱʱ��
		  		if(s!=0)
			   {
					SendOVT.SMS_ACK_OverTime = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
					DFE.Write(EE_JT_SMS_ACK_TIME, 4,(uchar *)&SendOVT.SMS_ACK_OverTime);	
				}
				else
				{
				  DFE.Read(EE_JT_SMS_ACK_TIME, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  memmove(p, (uchar *)&Dw, 4);
				  t = 4;
				}
		   	break;
	     case 		SET_SMS_RESEND_TIMES_ID:											  // SMS�ش�����
		  	   if(s!=0)
			   {
					SendOVT.SMS_ResendTimes = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
					DFE.Write(EE_JT_SMS_RESEND_TIMES, 4,(uchar *)&SendOVT.SMS_ResendTimes);	
				}
				else
				{
				  DFE.Read(EE_JT_SMS_RESEND_TIMES, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  memmove(p, (uchar *)&Dw, 4);
				  t = 4;
				}
		   	break;
	
	     case 		SET_M_APN_ID:								   		              //  ��������APN
		     if(s!=0)
			  {
				  if(s<=LEN_IPorDNS)               //APNĿǰֻ֧��32B����
				  {
				  	  SetBufferValue(&JT808_IP_DNS.M_APN[0], LEN_IPorDNS, 0);
					  memmove((char *)&JT808_IP_DNS.M_APN[0], (char *)p, s);
					  DFE.Write(EE_JT_M_APN, LEN_IPorDNS, (uchar *)&JT808_IP_DNS.M_APN[0]);
					  t = 80;	
				  } 
				  else
				    t = 3;
			  }
			  else
		     {  
				  DFE.Read(EE_JT_M_APN, LEN_IPorDNS,(uchar *)tB);
				  t = strlen((char *)tB);
				  memmove(p, tB, t);
			  }
		  

		   	break;
	     case 		SET_M_USE_NAME_ID:											        // ���������û���
		     if(s!=0)
			  {
				  if(s<=LEN_IP_NAME)               // �û���Ŀǰֻ֧��16B����    
				  {
				  	  SetBufferValue(&JT808_IP_DNS.M_IP_Name[0], LEN_IP_NAME, 0);
					  memmove((char *)&JT808_IP_DNS.M_IP_Name[0], (char *)p, s);
					  DFE.Write(EE_JT_M_USER, LEN_IP_NAME, (uchar *)&JT808_IP_DNS.M_IP_Name[0]);	
				  } 
				  else
				    t = 3;
		  	  }
			  else
		     {  
				  DFE.Read(EE_JT_M_USER, LEN_IP_NAME,(uchar *)tB);
				  t = strlen((char *)tB);
				  memmove(p, tB, t);
			  }

		   	break;
	     case 		SET_M_USE_SECRIERS_ID:							 			        // ����������������
		     if(s!=0)
			  {
				  if(s<=LEN_IP_SECRIER)               // �û�����Ŀǰֻ֧��8B����     
				  {
				  	  SetBufferValue(&JT808_IP_DNS.M_IP_Secrier[0], LEN_IP_NAME, 0);
					  memmove((char *)&JT808_IP_DNS.M_IP_Secrier[0], (char *)p, s);
					  DFE.Write(EE_JT_M_SERIERS, LEN_IP_NAME, (uchar *)&JT808_IP_DNS.M_IP_Secrier[0]);	
				  } 
				  else
				    t = 3; 
		  	  }
			  else
		     {  
				  DFE.Read(EE_JT_M_SERIERS, LEN_IP_NAME,(uchar *)tB);
				  t = strlen((char *)tB);
				  memmove(p, tB, t);
			  }

		   	break;
	     case 	   SET_M_IP_DNS_ID:											           // ��������IP��ַ������

		  	if(s!=0)
			  {
				  if(s<=LEN_IPorDNS)               //IP��DNSĿǰֻ֧��32B����
				  {
				  	  SetBufferValue(&JT808_IP_DNS.M_IPorDNS[0], LEN_IPorDNS, 0);
					  memmove((char *)&JT808_IP_DNS.M_IPorDNS[0], (char *)p, s);
					  if(ValidIP_Port(JT808_IP_DNS.M_IPorDNS)==OK)
					  {
						  sIP.EnableFlag[eFLAG_TCP] = ENABLE_FLAG;								              //����������TCP,UDP����
	                 //sIP.EnableFlag[eFLAG_UDP] = ENABLE_FLAG;	
	                 	DFE.Write(EE_ENABLE_TCP,1,(uchar*)&sIP.EnableFlag[eFLAG_TCP]);
	                 //DFE.Write(EE_ENABLE_UDP,1,(uchar*)&sIP.EnableFlag[eFLAG_UDP]);
							DFE.Write(EE_JT_M_IP_DNS, LEN_IPorDNS, (uchar *)&JT808_IP_DNS.M_IPorDNS[0]);
						 	t = 80;
					  }
					  else
					  {
					  	  //sIP.EnableFlag[eFLAG_TCP] = 0;								              //�ر���������TCP,UDP����
	                 //sIP.EnableFlag[eFLAG_UDP] = 0;	
	                 //DFE.Write(EE_ENABLE_TCP,1,(uchar*)&sIP.EnableFlag[eFLAG_TCP]);
	                 //DFE.Write(EE_ENABLE_UDP,1,(uchar*)&sIP.EnableFlag[eFLAG_UDP]);
					  	  t = 3;
					  }
				  } 
				  else
				    t = 3;
			  }
			  else
		     {  
				  DFE.Read(EE_JT_M_IP_DNS, LEN_IPorDNS,(uchar *)tB);
				  t = strlen((char *)tB);
				  memmove(p, tB, t);
			  }
		   	break;
	     case 		SET_B_APN_ID:											              //  ���ݷ�����APN
		     if(s!=0)
			  {
				  if(s<=LEN_IPorDNS)               //APNĿǰֻ֧��32B����
				  {
				  	  SetBufferValue(&JT808_IP_DNS.B_APN[0], LEN_IPorDNS, 0);
					  memmove((char *)&JT808_IP_DNS.B_APN[0], (char *)p, s);
					  DFE.Write(EE_JT_B_APN, LEN_IPorDNS, (uchar *)&JT808_IP_DNS.B_APN[0]);
					  t = 80;	
				  } 
				  else
				    t = 3;
		  	  }
			  else
		     {  
				  DFE.Read(EE_JT_B_APN, LEN_IPorDNS,(uchar *)tB);
				  t = strlen((char *)tB);
				  memmove(p, tB, t);
			  }

		   	break;
	     case 		SET_B_USE_NAME_ID:											        // ���ݷ������û���
		     if(s!=0)
			  {
				  if(s<=LEN_IP_NAME)               // �û���Ŀǰֻ֧��16B����    
				  {
				  	  SetBufferValue(&JT808_IP_DNS.B_IP_Name[0], LEN_IP_NAME, 0);
					  memmove((char *)&JT808_IP_DNS.B_IP_Name[0], (char *)p, s);
					  DFE.Write(EE_JT_B_USER, LEN_IP_NAME, (uchar *)&JT808_IP_DNS.B_IP_Name[0]);	
				  } 
				  else
				    t = 3;
		  	  }
			  else
		     {  
				  DFE.Read(EE_JT_B_USER, LEN_IPorDNS,(uchar *)tB);
				  t = strlen((char *)tB);
				  memmove(p, tB, t);
			  }

		   	break;
	     case 		SET_B_USE_SECRIERS_ID:							 			        // ���ݷ�������������
		     if(s!=0)
			  {
				  if(s<=LEN_IP_SECRIER)               // �û�����Ŀǰֻ֧��8B����     
				  {
				  	  SetBufferValue(&JT808_IP_DNS.B_IP_Secrier[0], LEN_IP_NAME, 0);
					  memmove((char *)&JT808_IP_DNS.B_IP_Secrier[0], (char *)p, s);
					  DFE.Write(EE_JT_B_SERIERS, LEN_IP_NAME, (uchar *)&JT808_IP_DNS.B_IP_Secrier[0]);	
				  } 
				  else
				    t = 3; 
		  	  }
			  else
		     {  
				  DFE.Read(EE_JT_B_SERIERS, LEN_IPorDNS,(uchar *)tB);
				  t = strlen((char *)tB);
				  memmove(p, tB, t);
			  }
		   	break;
	     case 	   SET_B_IP_DNS_ID:											           // ���ݷ�����IP��ַ������

		  	if(s!=0)
			  {
				  if(s<=LEN_IPorDNS)               //IP��DNSĿǰֻ֧��32B����
				  {
				  	  SetBufferValue(&JT808_IP_DNS.B_IPorDNS[0], LEN_IPorDNS, 0);
					  
					  memmove((char *)&JT808_IP_DNS.B_IPorDNS[0], (char *)p, s);
					  if(ValidIP_Port(JT808_IP_DNS.M_IPorDNS)==OK)
					  {
						  sIP.EnableFlag[eFLAG_TCP1] = ENABLE_FLAG;								              //�򿪱��ݷ�����TCP,UDP����
	                 //sIP.EnableFlag[eFLAG_UDP1] = ENABLE_FLAG;	
	                 TestMultiCenter = TRUE;		// Ӧ����⣬
	                 DFE.Write(EE_ENABLE_TCP1,1,(uchar*)&sIP.EnableFlag[eFLAG_TCP1]);
	                 //DFE.Write(EE_ENABLE_UDP1,1,(uchar*)&sIP.EnableFlag[eFLAG_UDP1]);
						  DFE.Write(EE_JT_B_IP_DNS, LEN_IPorDNS, (uchar *)&JT808_IP_DNS.B_IPorDNS[0]);
					     t = 80;
					  }
					  else
					  {
					  	  //sIP.EnableFlag[eFLAG_TCP] = 0;								              //�ر���������TCP,UDP����
	                 //sIP.EnableFlag[eFLAG_UDP] = 0;	
	                 //DFE.Write(EE_ENABLE_TCP,1,(uchar*)&sIP.EnableFlag[eFLAG_TCP]);
	                 //DFE.Write(EE_ENABLE_UDP,1,(uchar*)&sIP.EnableFlag[eFLAG_UDP]);
					  	  t = 3;
					  }
				  } 
				  else
				    t = 3;
		  	  }
			  else
		     {  
				  DFE.Read(EE_JT_B_IP_DNS, LEN_IPorDNS,(uchar *)tB);
				  t = strlen((char *)tB);
				  memmove(p, tB, t);
			  }

		   	break;
	     case 		SET_SERVER_TCP_PORT_ID:							 			        // ������TCP�˿�
		  	  if(s!=0)
			  {
				  t = 80;
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  if(Dw==0)
				  {
				  	  sIP.EnableFlag[eFLAG_TCP] = 0;								              //�ر���������TCP����
	              	  DFE.Write(EE_ENABLE_TCP,1,(uchar*)&sIP.EnableFlag[eFLAG_TCP]);
				  }
				  else if(Dw==1)
				  {
				  	  sIP.EnableFlag[eFLAG_TCP1] = 0;											 //�رձ��ݷ�����TCP1����
	                  DFE.Write(EE_ENABLE_TCP1,1,(uchar*)&sIP.EnableFlag[eFLAG_TCP1]);				  
				  }
              else if(Dw==2)
				  {
				  	  sIP.EnableFlag[eFLAG_TCP] = ENABLE_FLAG;								              //����������TCP����
	              DFE.Write(EE_ENABLE_TCP,1,(uchar*)&sIP.EnableFlag[eFLAG_TCP]);
				  }
				  else if(Dw==3)
				  {
				  	  sIP.EnableFlag[eFLAG_TCP1] = ENABLE_FLAG;											 //�򿪱��ݷ�����TCP1����
	              DFE.Write(EE_ENABLE_TCP1,1,(uchar*)&sIP.EnableFlag[eFLAG_TCP1]);				  
				  }
				  else if(Dw<65535)
				  {
				  	  JT808_IP_DNS.TCP_Port = Dw;						
				     DFE.Write(EE_JT_TCP_PORT, 4,(uchar *)&JT808_IP_DNS.TCP_Port);
				  }
				  else
				    t = 3;
				  
			  }
			  else
			  {
				  DFE.Read(EE_JT_TCP_PORT, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  memmove(p, (uchar *)&Dw, 4);
				  t = 4;
			   }  	

		   	break;
	     case 	   SET_SERVER_UDP_PORT_ID:											     // ������UDP�˿�
		  	  if(s!=0)
			  {
				  t = 80;
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  if(Dw==0)
				  {
				  	  sIP.EnableFlag[eFLAG_UDP] = 0;								              //�ر���������UDP����
	              DFE.Write(EE_ENABLE_UDP,1,(uchar*)&sIP.EnableFlag[eFLAG_UDP]);
	              
				  }
				  else if(Dw==1)
				  {
				  	  sIP.EnableFlag[eFLAG_UDP1] = 0;											 //�رձ��ݷ�����UDP1����
				  	  DFE.Write(EE_ENABLE_UDP1,1,(uchar*)&sIP.EnableFlag[eFLAG_UDP1]);
				  }
				  else if(Dw==2)
				  {
				  	  sIP.EnableFlag[eFLAG_UDP] = ENABLE_FLAG;								              //����������UDP����
	              DFE.Write(EE_ENABLE_UDP,1,(uchar*)&sIP.EnableFlag[eFLAG_UDP]);
				  }
				  else if(Dw==3)
				  {
				  	  sIP.EnableFlag[eFLAG_UDP1] = ENABLE_FLAG;											 //�򿪱��ݷ�����UDP1����
				  	  DFE.Write(EE_ENABLE_UDP1,1,(uchar*)&sIP.EnableFlag[eFLAG_UDP1]);
				  }
				  else if(Dw<65535)
				  {
					  #if( TEST_CODE_JUST_FOR_TEST_LUOYANG == 1) 
						JT808_IP_DNS.UDP_Port = 7804;
						#else	
					  	JT808_IP_DNS.UDP_Port = Dw;
						#endif
				      DFE.Write(EE_JT_UDP_PORT, 4,(uchar *)&JT808_IP_DNS.UDP_Port);
				  }
				  else
				   t = 3;
				  	
			  }
			  else
			  {
				  DFE.Read(EE_JT_UDP_PORT, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  memmove(p, (uchar *)&Dw, 4);
				  t = 4;
			   }  	
		   	break;

	     case 		SET_POSITION_UP_MODES_ID:			                          // �ر�����
			  if(s!=0)
			  {
				  if(*(p+3)<=2)
				  {
				     SendInterval.MCA.Byte.Mode = *(p+3);
				     DFE.Write(EE_JT_SEND_INTERVAL_MODE, 1,(uchar *)&SendInterval.MCA.Byte.Mode);
					  GSM.CalculateSendInterval(0);
				  }
				  else
				    t=3;
			  }
			  else
			  {
				  DFE.Read(EE_JT_SEND_INTERVAL_MODE, 1,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  memmove(p, (uchar *)&Dw, 4);
				  t = 4;
			  }  	

		   	break;
	     case 		SET_POSITION_UP_PORJECT_ID:							 			  // λ�ûر�����
		     if(s!=0)
			  {
				  if(*(p+3)<=1)
				  {
				     SendInterval.MCA.Byte.Condition = *(p+3);
				     DFE.Write(EE_JT_SEND_INTERVAL_METHOD, 1,(uchar *)&SendInterval.MCA.Byte.Condition);
					  GSM.CalculateSendInterval(0);
				  }
				  else
				    t=3;
		  	  }
			  else
			  {
				  DFE.Read(EE_JT_SEND_INTERVAL_METHOD, 1,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  memmove(p, (uchar *)&Dw, 4);
				  t = 4;
			   }  	

		   	break;
	     case 		SET_POSITION_UP_TIME_NO_DRIVER_ID:							     // ��ʻԱδ��¼�ر�ʱ����
		     if(s!=0)
			  {
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  if(Dw!=0)
				  {
				     SendInterval.NoDriverTime = Dw;
				     DFE.Write(EE_JT_SEND_TIME_NO_DRIVER, 4,(uchar *)&Dw);
					  GSM.CalculateSendInterval(0);
				  }
				  else
				    t=3;
		  	  }
			  else
			  {
				  DFE.Read(EE_JT_SEND_TIME_NO_DRIVER, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  memmove(p, (uchar *)&Dw, 4);
				  t = 4;
			   } 

		   	break;

	     case 		SET_POSITION_UP_TIME_IDEL_ID:			                       //����ʱ�ر�ʱ����
		     if(s!=0)
			  {
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  if(Dw!=0)
				  {
				     SendInterval.IDEL_ModeTime = Dw;
				     DFE.Write(EE_JT_SEND_TIME_IDEL, 4,(uchar *)&Dw);
					  GSM.CalculateSendInterval(0);
				  }
				  else
				    t = 3;
			  }
			  else
			  {
				  DFE.Read(EE_JT_SEND_TIME_IDEL, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  memmove(p, (uchar *)&Dw, 4);
				  t = 4;
			   } 
		   	break;
	     case 	   SET_POSITION_UP_TIME_ALARM_ID:									  //��������ʱ��ر����
		  	  if(s!=0)
			  {
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  if(Dw!=0)
				  {
				     SendInterval.AlarmModeTime = Dw;
				     DFE.Write(EE_JT_SEND_TIME_ALARM, 4,(uchar *)&Dw);
					  GSM.CalculateSendInterval(0);
				  }
				  else
				    t=3;
			  }
			  else
			  {
				  DFE.Read(EE_JT_SEND_TIME_ALARM, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  memmove(p, (uchar *)&Dw, 4);
				  t = 4;
			   } 
		   	break;
	     case 		SET_POSITION_UP_TIME_DEFAULT_ID:									  //ȱʡ��ʱ�ر����
		     if(s!=0)
			  {
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  if(Dw!=0)
				  {
				     SendInterval.DefauletTime = Dw;
					  GSM.CalculateSendInterval(0);
				     DFE.Write(EE_JT_SEND_TIME_DEFAULET, 4,(uchar *)&Dw);

					   Uart.puts("����ȱʡ��ʱ�ر���");
				  		Int2Str( Dw,(char*)Tp.Buffer);
				  		strcat((char*)Tp.Buffer,",");
						s = strlen( (char*)Tp.Buffer);
						Int2Str( SendInterval.Second,(char*)&Tp.Buffer[s]);
				  		Uart.puts( (char*)Tp.Buffer);
						
						
						#if( TEST_CODE_JUST_FOR_TEST_LUOYANG_NEW==1)
						if( Dw == 1 )
						{
							SamlePeriod1S = TRUE;							
						}
						else
						{
							SamlePeriod1S = 0;
							TestAccuracy = TestBlind = 0;
						}
						

						#endif
				  }
				  else
				    t=3;
		  	  }
			  else
			  {
				  DFE.Read(EE_JT_SEND_TIME_DEFAULET, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  memmove(p, (uchar *)&Dw, 4);
				  t = 4;
			   } 
		   	break;
	     case 		SET_POSITION_UP_DISTANCE_DEFAULT_ID:		                 //ȱʡ����ر����
		  	  if(s!=0)
			  {
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  if(Dw!=0)
				  {
				     SendInterval.DefauletDistance = Dw;
				     DFE.Write(EE_JT_SEND_DISTANCE_DEFAULET, 4,(uchar *)&Dw);
					  GSM.CalculateSendInterval(0);
				  }
				  else
				    t=3;
			  }
			  else
			  {
				  DFE.Read(EE_JT_SEND_DISTANCE_DEFAULET, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  memmove(p, (uchar *)&Dw, 4);
				  t = 4;
			   } 
		   	break;
	     case 		SET_POSITION_UP_DISTANCE_NO_DRIVER_ID:							  // ��ʻԱδ��¼�ر�������
		  	  if(s!=0)
			  {
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  if(Dw!=0)
				  {
				     SendInterval.NoDriverDistance = Dw;
				     DFE.Write(EE_JT_SEND_DISTANCE_NO_DRIVER, 4,(uchar *)&Dw);
					  GSM.CalculateSendInterval(0);
				  }
				  else
				    t=3;
			  }
			  else
			  {
				  DFE.Read(EE_JT_SEND_DISTANCE_NO_DRIVER, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  memmove(p, (uchar *)&Dw, 4);
				  t = 4;
			   } 
		   	break;
	     case 		SET_POSITION_UP_DISTANCE_IDEL_ID:				 			     //����ʱ�ر�������
		  	  if(s!=0)
			  {
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  if(Dw!=0)
				  {
				     SendInterval.IDEL_ModeDistance = Dw;
				     DFE.Write(EE_JT_SEND_DISTANCE_IDEL, 4,(uchar *)&Dw);
					  GSM.CalculateSendInterval(0);
				  }
				  else
				    t=3;
			  }
			  else
			  {
				  DFE.Read(EE_JT_SEND_DISTANCE_IDEL, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  memmove(p, (uchar *)&Dw, 4);
				  t = 4;
			   } 
		   	break;
	     case 	   SET_POSITION_UP_DISTANCE_ALARM_ID:							     //���������ر�������
		     if(s!=0)
			  {
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  if(Dw!=0)
				  {
				     SendInterval.AlarmModeDistance = Dw;
				     DFE.Write(EE_JT_SEND_DISTANCE_ALARM, 4,(uchar *)&Dw);
					  GSM.CalculateSendInterval(0);
				  }
				  else
				    t=3;
			  }
			  else
			  {
				  DFE.Read(EE_JT_SEND_DISTANCE_ALARM, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  memmove(p, (uchar *)&Dw, 4);
				  t = 4;
			   } 
		   	break;
	     case 		SET_BREAK_POINT_ANGLE_ID:										     //�սǲ����Ƕ�
		  	  if(s!=0)
			  {
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  if(Dw<180)
				  {
				     SendInterval.MCA.Byte.Angle = *(p+3);
				     DFE.Write(EE_JT_SEND_ANGLE, 1,(uchar *)&SendInterval.MCA.Byte.Angle);
					  GSM.CalculateSendInterval(0);
				  }
				  else
				    t=3;
			   }
			  else
			  {
				  DFE.Read(EE_JT_SEND_ANGLE, 1,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  memmove(p, (uchar *)&Dw, 4);
				  t = 4;
			   } 
		   	break;

	     case 		SET_PHONE_MONITOR_CENTER_ID:	                             //���ƽ̨����
		  	  if(s!=0)
			  {
				  if(s<=LEN_NUMBER)               // ������ĺ���Ŀǰֻ֧��16B����     
				  {
				  	  SetBufferValue(&JT808_Number.Center[0], LEN_NUMBER, 0);
					  memmove((char *)&JT808_Number.Center[0], (char *)p, s);
					  DFE.Write(EE_JT_CENTER_NUMBER, LEN_NUMBER, (uchar *)&JT808_Number.Center[0]);	
				  } 
				  else
				    t = 3; 
			  }
			  else
		     {  
				  DFE.Read(EE_JT_CENTER_NUMBER, LEN_NUMBER,(uchar *)tB);
				  t = strlen((char *)tB);
				  memmove(p, tB, t);
			  }
		   	break;
	     case 		SET_PHONE_RESET_ID:									              //��λ����
		  	  if(s!=0)
			  {
				  if(s<=LEN_NUMBER)               // ��λ�绰����Ŀǰֻ֧��16B����     
				  {
				  	  SetBufferValue(&JT808_Number.Reset[0], LEN_NUMBER, 0);
					  memmove((char *)&JT808_Number.Reset[0], (char *)p, s);
					  DFE.Write(EE_JT_RESET_NUMBER, LEN_NUMBER, (uchar *)&JT808_Number.Reset[0]);	
				  } 
				  else
				    t = 3; 
			  }
			  else
		     {  
				  DFE.Read(EE_JT_RESET_NUMBER, LEN_NUMBER,(uchar *)tB);
				  t = strlen((char *)tB);
				  memmove(p, tB, t);
			  }
		   	break;
	     case 		SET_PHONE_RESET_FACTORY_ID:									     //�ָ��������ú���
		  	  if(s!=0)
			  {
				  if(s<=LEN_NUMBER)               // �ָ��������õ绰����Ŀǰֻ֧��16B����     
				  {
				  	  SetBufferValue(&JT808_Number.ResetFactory[0], LEN_NUMBER, 0);
					  memmove((char *)&JT808_Number.ResetFactory[0], (char *)p, s);
					  DFE.Write(EE_JT_RESET_FACTORY_NUMBER, LEN_NUMBER, (uchar *)&JT808_Number.ResetFactory[0]);	
				  } 
				  else
				    t = 3;
			  }
			  else
		     {  
				  DFE.Read(EE_JT_RESET_FACTORY_NUMBER, LEN_NUMBER,(uchar *)tB);
				  t = strlen((char *)tB);
				  memmove(p, tB, t);
			  }	  
		   	break;
	     case 		SET_PHONE_SMS_MONITOR_CENTER_ID:								     //���ƽ̨SMS����
		  	  if(s!=0)
			  {
				  if(s<=LEN_NUMBER)               // �绰����Ŀǰֻ֧��16B����     
				  {
				  	  SetBufferValue(&JT808_Number.CenterSMS[0], LEN_NUMBER, 0);
					  memmove((char *)&JT808_Number.CenterSMS[0], (char *)p, s);
					  DFE.Write(EE_JT_CENTER_SMS_NUMBER, LEN_NUMBER, (uchar *)&JT808_Number.CenterSMS[0]);	
				  } 
				  else
				    t = 3; 
			  }
			  else
		     {  
				  DFE.Read(EE_JT_CENTER_SMS_NUMBER, LEN_NUMBER,(uchar *)tB);
				  t = strlen((char *)tB);
				  memmove(p, tB, t);
			  }	 
		   	break;
	     case 		SET_PHONE_TEXT_SMS_ALARM_ID:									     //�����ı�SMS��������
		  	  if(s!=0)
			  {
				  if(s<=LEN_NUMBER)               // �绰����Ŀǰֻ֧��16B����     
				  {
				  	  SetBufferValue(&JT808_Number.TextSMS[0], LEN_NUMBER, 0);
					  memmove((char *)&JT808_Number.TextSMS[0], (char *)p, s);
					  DFE.Write(EE_JT_TEXT_SMS_NUMBER, LEN_NUMBER, (uchar *)&JT808_Number.TextSMS[0]);	
				  } 
				  else
				    t = 3; 
			  }
			  else
		     {  
				  DFE.Read(EE_JT_TEXT_SMS_NUMBER, LEN_NUMBER,(uchar *)tB);
				  t = strlen((char *)tB);
				  memmove(p, tB, t);
			  }	 
		   	break;
	     case 		SET_PHONE_GET_MODES_ID:											     //�绰��ͨ����
		  	  if(s!=0)
			  {
				  if(*(p+3)<=1)
				  {
				     JT808_Number.GetCallMode = *(p+3);
				     DFE.Write(EE_JT_GET_CALL_MODE, 1,(uchar *)&JT808_Number.GetCallMode);
				  }
				  else
				    t=3;
			  }
			  else
		     {  
				  DFE.Read(EE_JT_GET_CALL_MODE, 1,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  t = 4;
				  memmove(p, (uchar *)&Dw, t);
			  }	 
		   	break;
	     case 		SET_PHONE_ONLINE_TIME_TIMES_LIMIT_ID:						     //ÿ���ͨ��ʱ��
		  	  if(s!=0)
			  {
				  JT808_Number.CallTime = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  DFE.Write(EE_JT_CALL_TIME_LIMIT, 4,(uchar *)&JT808_Number.CallTime);	
			  }
			  else
		     {  
				  DFE.Read(EE_JT_CALL_TIME_LIMIT, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  t = 4;
				  memmove(p, (uchar *)&Dw, t);
			  }	 
		   	break;
	     case 		SET_PHONE_ONLINE_TIME_MONTH_LIMIT_ID:						     //���������ͨ��ʱ��
		  	  if(s!=0)
			  {
				  JT808_Number.CallTimeMonth = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  DFE.Write(EE_JT_CALL_TIME_LIMIT_MONTH, 4,(uchar *)&JT808_Number.CallTimeMonth);	
			  }
			  else
		     {  
				  DFE.Read(EE_JT_CALL_TIME_LIMIT_MONTH, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  t = 4;
				  memmove(p, (uchar *)&Dw, t);
			  }	 
		   	break;
	     case 		SET_PHONE_MONITORING_ID:											  //�����绰����
		  	  if(s!=0)
			  {
				  if(s<=LEN_NUMBER)               // �绰����Ŀǰֻ֧��16B����     
				  {
				  	  SetBufferValue(&JT808_Number.JT[0], LEN_NUMBER, 0);
					  memmove((char *)&JT808_Number.JT[0], (char *)p, s);
					  DFE.Write(EE_JT_JT_NUMBER, LEN_NUMBER, (uchar *)&JT808_Number.JT[0]);	
				  } 
				  else
				    t = 3; 
			  }
			  else
		     {  
				  DFE.Read(EE_JT_JT_NUMBER, LEN_NUMBER,(uchar *)tB);
				  t = strlen((char *)tB);
				  memmove(p, tB, t);
			  }	 
		   	break;
	     case 		SET_PHONE_PRIVILEGE_SMS_ID:									     //��Ȩ���ź���
		  	  if(s!=0)
			  {
				  if(s<=LEN_NUMBER)               // �绰����Ŀǰֻ֧��16B����     
				  {
				  	  SetBufferValue(&JT808_Number.Privilege[0], LEN_NUMBER+1, 0);
					  memmove((char *)&JT808_Number.Privilege[0], (char *)p, s);
					  DFE.Write(EE_JT_PRIVILILEGE_SMS_NUMBER, LEN_NUMBER, (uchar *)&JT808_Number.Privilege[0]);	
				  } 
				  else
				    t = 3; 
			  }
			  else
		     {  
				  DFE.Read(EE_JT_PRIVILILEGE_SMS_NUMBER, LEN_NUMBER,(uchar *)tB);
				  t = strlen((char *)tB);
				  memmove(p, tB, t);
			  }	 
		   	break;
	     case 		SET_WORD_FOR_NO_ALARM_ID:	                                //����������
		  	  if(s!=0)
			  {
				  JT808_Alarm_Word.Shield = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  DFE.Write(EE_JT_SHIELD_WORD, 4,(uchar *)&JT808_Alarm_Word.Shield);	
			  }
			  else
		     {  
				  DFE.Read(EE_JT_SHIELD_WORD, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  t = 4;
				  memmove(p, (uchar *)&Dw, t);
			  }	 
		   	break;
	     case 		SET_WORD_FOR_TEST_SMS_ALARM_ID:								     //���������ı�SMS����
		  	  if(s!=0)
			  {
				  JT808_Alarm_Word.SendTextSMS = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  DFE.Write(EE_JT_TEXT_SMS_WORD, 4,(uchar *)&JT808_Alarm_Word.SendTextSMS);	
			  }
			  else
		     {  
				  DFE.Read(EE_JT_TEXT_SMS_WORD, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  t = 4;
				  memmove(p, (uchar *)&Dw, t);
			  }	
		   	break;
	     case 		SET_WORD_FOR_TAKE_PHOTO_ALARM_ID:								  //�������տ���
		  	  
			  if(s!=0)
			  {
				  JT808_Alarm_Word.TakePhoto = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  DFE.Write(EE_JT_TAKE_PHOTO_WORD, 4,(uchar *)&JT808_Alarm_Word.TakePhoto);	
			  }
			  else
		     {  
				  DFE.Read(EE_JT_TAKE_PHOTO_WORD, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  t = 4;
				  memmove(p, (uchar *)&Dw, t);
			  }
		   	break;
	     case 		SET_WORD_FOR_TAKE_PHOTO_SAVE_ALARM_ID:						     //�������ձ��濪��
		  	  if(s!=0)
			  {
				  JT808_Alarm_Word.SavePhoto = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  DFE.Write(EE_JT_SAVE_PHOTO_WORD, 4,(uchar *)&JT808_Alarm_Word.SavePhoto);	
			  }
			  else
		     {  
				  DFE.Read(EE_JT_SAVE_PHOTO_WORD, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  t = 4;
				  memmove(p, (uchar *)&Dw, t);
			  }	
		   	break;
	     case 		SET_WORD_FOR_KEY_ALARM_ID:										     //�ؼ�������־
		  	  if(s!=0)
			  {
				  JT808_Alarm_Word.KeyAlarm = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  DFE.Write(EE_JT_KEY_WORD, 4,(uchar *)&JT808_Alarm_Word.KeyAlarm);	
			  }
			  else
		     {  
				  DFE.Read(EE_JT_KEY_WORD, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  t = 4;
				  memmove(p, (uchar *)&Dw, t);
			  }	
		   	break;
	     case 		SET_SPEED_HITH_LIMIT_ID:											  //����ٶ�
		  	  if(s!=0)
			  {
			  	  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  if(Dw>500)	    Dw = 250;				  
				  DFE.Write(FM_HIGH_SPEED_LIMIT, 1,(uchar *)&Dw);		
				  // ��ʱ��������													 
				  SpeedC.SetGetPara( eSPEED_CMD_GET_OVER_SPEED,LEN_OVER_SPEED_PARA,Tp.Buffer);
				  Tp.Buffer[0] = TRUE;
				  Tp.Buffer[1] = Dw;
				  SpeedC.SetGetPara( eSPEED_CMD_SET_OVER_SPEED,LEN_OVER_SPEED_PARA,Tp.Buffer);
				  Uart.puts("��������ٶȣ�");
				  Int2Str( Dw,(char*)Tp.Buffer);
				  strcat((char*)Tp.Buffer," ");
				  Uart.puts( (char*)Tp.Buffer);
			  }
			  else
		     {  
				  Dw = 0;
				  DFE.Read(FM_HIGH_SPEED_LIMIT, 1,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  t = 4;
				  memmove(p, (uchar *)&Dw, t);
			  }	
		   	break;
	     case 		SET_TIME_OVER_LIMIT_SPEED_ID:									     //���ٳ���ʱ��
		  	  if(s!=0)
			  {
			  	//	ClearOverSpeedCntFlag = 0xA5;
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  if(Dw>65535)	Dw = 65535;
				  DFE.Write(FM_OVP_ENABLE_TIME, 2,(uchar *)&Dw);	
				  // ��ʱ��������													 
				  SpeedC.SetGetPara( eSPEED_CMD_GET_OVER_SPEED,LEN_OVER_SPEED_PARA,Tp.Buffer);
				  Tp.Buffer[0] = TRUE;
				  Tp.B16[2] = Dw;
				  SpeedC.SetGetPara( eSPEED_CMD_SET_OVER_SPEED,LEN_OVER_SPEED_PARA,Tp.Buffer);
				  Uart.puts("���ó�������ʱ�䣺");
				  Int2Str( Dw,(char*)Tp.Buffer);
				  strcat((char*)Tp.Buffer," ");
				  Uart.puts( (char*)Tp.Buffer);
			  }
			  else
		     {  
			  		Dw = 0;
				  DFE.Read(FM_OVP_ENABLE_TIME, 2,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  t = 4;
				  memmove(p, (uchar *)&Dw, t);
			  }	
		   	break;
	     case 		SET_TIME_DRIVEING_LIMIT_ID:										  //������ʻʱ������
		  	  if(s!=0)
			  {			  		
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  DFE.Write(FM_DRIVEING_TIME, 4,(uchar *)&Dw);	
				  // ��ʱ��������													 
				  SpeedC.SetGetPara( eSPEED_CMD_GET_DRIVING_TIME,LEN_DRIVING_TIME_LIMIT,Tp.Buffer);
				  Tp.B32[0] = Dw;
				  Tp.Buffer[18] = TRUE;
				  SpeedC.SetGetPara( eSPEED_CMD_SET_DRIVING_TIME,LEN_DRIVING_TIME_LIMIT,Tp.Buffer);
				  Uart.puts("���ó�����ʻʱ�����ޣ�");
				  Int2Str( Dw,(char*)Tp.Buffer);
				  strcat((char*)Tp.Buffer," ");
				  Uart.puts( (char*)Tp.Buffer);
			  }
			  else
		     {  
				  DFE.Read(FM_DRIVEING_TIME, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  t = 4;
				  memmove(p, (uchar *)&Dw, t);
			  }	
		   	break;
	     case 		SET_TIME_DRIVEING_LIMIT_ONE_DAY_ID:							     //һ���ۼƼ�ʻʱ������
		  	  if(s!=0)
			  {
			  		Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
			  		DFE.Write(FM_DRIVE_ONE_DAY_LIMINT, 4,(uchar *)&Dw);	
			  		// ��ʱ��������													 
				  SpeedC.SetGetPara( eSPEED_CMD_GET_DRIVING_TIME,LEN_DRIVING_TIME_LIMIT,Tp.Buffer);
				  Tp.B32[2] = Dw;
				  Tp.Buffer[18] = TRUE;
				  SpeedC.SetGetPara( eSPEED_CMD_SET_DRIVING_TIME,LEN_DRIVING_TIME_LIMIT,Tp.Buffer);
			  }
			  else
		     {  
				  DFE.Read(FM_DRIVE_ONE_DAY_LIMINT, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  t = 4;
				  memmove(p, (uchar *)&Dw, t);
			  }
		   	break;

	     case 		SET_TIME_REST_ID:													     //��С��Ϣʱ��
		  	  if(s!=0)
			  {			  		
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  DFE.Write(FM_REST_TIME, 4,(uchar *)&Dw);	
				  // ��ʱ��������													 
				  SpeedC.SetGetPara( eSPEED_CMD_GET_DRIVING_TIME,LEN_DRIVING_TIME_LIMIT,Tp.Buffer);
				  Tp.B32[1] = Dw;
				  Tp.Buffer[18] = TRUE;
				  SpeedC.SetGetPara( eSPEED_CMD_SET_DRIVING_TIME,LEN_DRIVING_TIME_LIMIT,Tp.Buffer);
				  Uart.puts("������С��Ϣʱ�䣺");
				  Int2Str( Dw,(char*)Tp.Buffer);
				  strcat((char*)Tp.Buffer," ");
				  Uart.puts( (char*)Tp.Buffer);
			  }
			  else
		     {  
				  DFE.Read(FM_REST_TIME, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  t = 4;
				  memmove(p, (uchar *)&Dw, t);
			  }	
		   	break;
	     case 		SET_TIME_STOP_CAR_ID:												  //�ͣ��ʱ��
		  	  if(s!=0)
			  {			  		
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  DFE.Write(FM_PARKING_TIME_LIMINT, 4,(uchar *)&Dw);	
				  // ��ʱ��������													 
				  SpeedC.SetGetPara( eSPEED_CMD_GET_DRIVING_TIME,LEN_DRIVING_TIME_LIMIT,Tp.Buffer);
				  Tp.B32[3] = Dw;
				  Tp.Buffer[19] = TRUE;
				  SpeedC.SetGetPara( eSPEED_CMD_SET_DRIVING_TIME,LEN_DRIVING_TIME_LIMIT,Tp.Buffer);
				  Uart.puts("�����ͣ��ʱ�䣺");
				  Int2Str( Dw,(char*)Tp.Buffer);
				  strcat((char*)Tp.Buffer," ");
				  Uart.puts( (char*)Tp.Buffer);
			  }
			  else
		     {  
				  DFE.Read(FM_PARKING_TIME_LIMINT, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  t = 4;
				  memmove(p, (uchar *)&Dw, t);
			  }	
		   	break;
	     case 		SET_VIDIO_QUOLITY_ID:	                                   //ͼ��/��Ƭ����
		  	  if(s!=0)
			  {
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  if((Dw!=0)&&(Dw<=10))
				  {
				     JT808_Camera_Para.Quality = Dw;
				     DFE.Write(EE_JT_VIDIO_QUALITY, 1,(uchar *)&JT808_Camera_Para.Quality);
				  }
				  else
				    t=3;
			  }
			  else
		     {  
				  DFE.Read(EE_JT_VIDIO_QUALITY, 1,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  t = 4;
				  memmove(p, (uchar *)&Dw, t);
			  }
		   	break;
	     case 		SET_VIDIO_BRIGHTNESS_ID:										     //����
		  	  if(s!=0)
			  {
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  if(Dw<=255)
				  {
				     JT808_Camera_Para.Light = Dw;
				     DFE.Write(EE_JT_VIDIO_LIGHT, 1,(uchar *)&JT808_Camera_Para.Light);
				  }
				  else
				    t=3;
			  }
			  else
		     {  
				  DFE.Read(EE_JT_VIDIO_LIGHT, 1,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  t = 4;
				  memmove(p, (uchar *)&Dw, t);
			  }
		   	break;
	     case 		SET_VIDIO_CONTRAST_ID:										        //�Աȶ�
		  	  if(s!=0)
			  {
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  if(Dw<=127)
				  {
				     JT808_Camera_Para.Contrast = Dw;
				     DFE.Write(EE_JT_VIDIO_CONTRAST, 1,(uchar *)&JT808_Camera_Para.Contrast);
				  }
				  else
				    t=3;
			  }
			  else
		     {  
				  DFE.Read(EE_JT_VIDIO_CONTRAST, 1,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  t = 4;
				  memmove(p, (uchar *)&Dw, t);
			  }
		   	break;
	     case 		SET_VIDIO_SATULATION_ID:										     //���Ͷ�
		  	  if(s!=0)
			  {
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  if(Dw<=127)
				  {
				     JT808_Camera_Para.Satulation = Dw;
				     DFE.Write(EE_JT_VIDIO_SATULATION, 1,(uchar *)&JT808_Camera_Para.Satulation);
				  }
				  else
				    t=3;
			  }
			  else
		     {  
				  DFE.Read(EE_JT_VIDIO_SATULATION, 1,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  t = 4;
				  memmove(p, (uchar *)&Dw, t);
			  }
		   	break;
	     case 		SET_VIDIO_CHROMA_ID:											        //ɫ��
		  	  if(s!=0)
			  {
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  if(Dw<=255)
				  {
				     JT808_Camera_Para.Colour = Dw;
				     DFE.Write(EE_JT_VIDIO_CALOUR, 1,(uchar *)&JT808_Camera_Para.Colour);
				  }
				  else
				    t=3;
			  }
			  else
		     {  
				  DFE.Read(EE_JT_VIDIO_CALOUR, 1,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  t = 4;
				  memmove(p, (uchar *)&Dw, t);
			  }
		   	break;
	     case 		SET_ODOMETER_VALUS_ID:	                                   //��̱����
		  	  if(s!=0)
			  {
			    t=0;
			  }
			  else
		     {  
				  DFE.Read(FM_MILAGE_HEX, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  t = 4;
				  memmove(p, (uchar *)&Dw, t);
			  }
		   	break;
	     case 		SET_PROVINCIAL_ID:													  //ʡ��
		  	  if(s!=0)
			  {
			    DFE.Write(EE_PROVINCE_REGION_ID, 1, p+1); 
				NOP(); NOP(); NOP();
				DFE.Write(EE_PROVINCE_REGION_ID+1, 1, p); 
				NOP(); NOP(); NOP();	 
			  }
			  else
		      {  
				  DFE.Read(EE_PROVINCE_REGION_ID, 2,(uchar *)&Dw);		// �洢��С�����ݣ������������Ǵ������
				  Dw = SwapINT16(Dw);
				  t = 2;
				  memmove(p, (uchar *)&Dw, t);
			  }
		   	break;
	     case 		SET_CITY_WHERE_ID:													  //����
		  	  if(s!=0)
			  { 
				DFE.Write(EE_PREFECTURE_REGION_ID, 1, p+1); 
				NOP(); NOP(); NOP();
				DFE.Write(EE_PREFECTURE_REGION_ID+1, 1, p); 
				NOP(); NOP(); NOP();	
			  }
			  else
		      {  
				  DFE.Read(EE_PREFECTURE_REGION_ID, 2,(uchar *)&Dw);
				  Dw = SwapINT16(Dw);
				  t = 2;
				  memmove(p, (uchar *)&Dw, t);
			  }
		   	break;
	     case 		SET_CAR_NUBER_ID:													     //���ƺ���
		  	  if(s!=0)
			  {
				  if(s<=LEN_VEHICLE_CODE)                 
				  {
				  	  SetBufferValue(tB, LEN_VEHICLE_CODE+1, 0);
					  memmove(tB, p, s);
					  DFE.Write(FM_VEHICLE_CODE, LEN_VEHICLE_CODE, (uchar *)tB);	
				  } 
				  else
				    t = 3; 
			  }
			  else
		     {  
				  DFE.Read(FM_VEHICLE_CODE, LEN_VEHICLE_CODE,(uchar *)tB);
				  t = strlen((char *)tB);
				  memmove(p, tB, t);
			  }	 
		   	break;
	     case 		SET_CAR_COLOUR_ID:													  //������ɫ
		  	  if(s!=0)
			  {
			    DFE.Write(FM_CAR_COLOUR, 1, p);	
			  }
			  else
		     {  
				  DFE.Read(FM_CAR_COLOUR, 1,(uchar *)&Dw);
				  t = 1;
				  memmove(p, (uchar *)&Dw, t);
			  }
		   	break;

		/**************************************** add 2013.3   *******************************************************************/
		case SET_SERVER_IC_AUTH_MAIN_IP:												// ��·����֤IC ����֤��������IP��ַ������
			if(s!=0)
			 {
				  if(s<=LEN_IPorDNS)               //IP��DNSĿǰֻ֧��32B����
				  {
				  	  SetBufferValue(&JT808_IP_DNS.IC_Auth_M_IPorDNS[0], LEN_IPorDNS, 0);
					  memmove((char *)&JT808_IP_DNS.IC_Auth_M_IPorDNS[0], (char *)p, s);
					  if(ValidIP_Port(JT808_IP_DNS.IC_Auth_M_IPorDNS)==OK)
					  {
						  sIP.EnableFlag[eFLAG_IC_AUTH_TCP] = ENABLE_FLAG;								              //����������TCP,UDP����
	                 	  //sIP.EnableFlag[eFLAG_UDP] = ENABLE_FLAG;	
	                      DFE.Write(EE_ENABLE_IC_AUTH_TCP,1,(uchar*)&sIP.EnableFlag[eFLAG_IC_AUTH_TCP]);
	                     //DFE.Write(EE_ENABLE_UDP,1,(uchar*)&sIP.EnableFlag[eFLAG_UDP]);
						  DFE.Write(EE_JT_IC_AUTH_M_IP_DNS, LEN_IPorDNS, (uchar *)&JT808_IP_DNS.IC_Auth_M_IPorDNS[0]);
						  t = 80;
						  	Uart.puts("����IC����֤IP��ַ��");
					 		Int2Str( Dw, (char*)Tp.Buffer);
					 		Uart.puts((char *)&JT808_IP_DNS.IC_Auth_M_IPorDNS[0]);

							U2PinSelDevice = U2_FOR_RFID;
							os_dly_wait(100);
							Uart2.Init( 9600, U2_FOR_RFID);

					  }
					  else
					  {
					  	  //sIP.EnableFlag[eFLAG_TCP] = 0;								              //�ر���������TCP,UDP����
	                 //sIP.EnableFlag[eFLAG_UDP] = 0;	
	                 //DFE.Write(EE_ENABLE_TCP,1,(uchar*)&sIP.EnableFlag[eFLAG_TCP]);
	                 //DFE.Write(EE_ENABLE_UDP,1,(uchar*)&sIP.EnableFlag[eFLAG_UDP]);
					  	  t = 3;
					  }
				  } 
				  else
				    t = 3;
			  }
			  else
		     {  
				  DFE.Read(EE_JT_IC_AUTH_M_IP_DNS, LEN_IPorDNS,(uchar *)tB);
				  t = strlen((char *)tB);
				  memmove(p, tB, t);
			  }
			break;

		case	SET_SERVER_IC_AUTH_TCP:											    				// ��·����֤IC ����֤��������TCP�˿�
			  if(s!=0)
			  {
				  t = 80;
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  if(Dw==0)
				  {
				  	  sIP.EnableFlag[eFLAG_IC_AUTH_TCP] = 0;								              //�ر�IC����֤��������TCP����
	              	  DFE.Write(EE_ENABLE_IC_AUTH_TCP,1,(uchar*)&sIP.EnableFlag[eFLAG_IC_AUTH_TCP]);
				  }
				  else if(Dw==1)
				  {
				  	  sIP.EnableFlag[eFLAG_IC_AUTH_B_TCP] = 0;											 //�ر�IC����֤���ݷ�����TCP1����
	                  DFE.Write(EE_ENABLE_IC_AUTH_B_TCP,1,(uchar*)&sIP.EnableFlag[eFLAG_IC_AUTH_B_TCP]);				  
				  }
                  else if(Dw==2)
				  {
				  	  sIP.EnableFlag[eFLAG_IC_AUTH_TCP] = ENABLE_FLAG;								              //��IC����֤��������TCP����
	                  DFE.Write(EE_ENABLE_IC_AUTH_TCP,1,(uchar*)&sIP.EnableFlag[eFLAG_IC_AUTH_TCP]);
				  }
				  else if(Dw==3)
				  {
				  	  sIP.EnableFlag[eFLAG_IC_AUTH_B_TCP] = ENABLE_FLAG;											 //��IC����֤�����ݷ�����TCP1����
	                  DFE.Write(EE_ENABLE_IC_AUTH_B_TCP,1,(uchar*)&sIP.EnableFlag[eFLAG_IC_AUTH_B_TCP]);				  
				  }
				  else if(Dw<65535)
				  {
				  	 JT808_IP_DNS.IC_Auth_TCP_Port = Dw;
				    DFE.Write(EE_JT_IC_AUTH_TCP_PORT, 4,(uchar *)&Dw);
					 Uart.puts("����IC����֤�˿ڣ�");
					 Int2Str( JT808_IP_DNS.IC_Auth_TCP_Port, (char*)Tp.Buffer);
					 Uart.puts((char*)Tp.Buffer);
				  }
				  else
				    t = 3;
				  
			  }
			  else
			  {
				  DFE.Read(EE_JT_IC_AUTH_TCP_PORT, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  memmove(p, (uchar *)&Dw, 4);
				  t = 4;
			   }  	

			break;

		case	SET_SERVER_IC_AUTH_UDP:											    				// ��·����֤IC ����֤��������UDP�˿�
			  if(s!=0)
			  {
				  t = 80;
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  if(Dw==0)
				  {
				  	  sIP.EnableFlag[eFLAG_IC_AUTH_UDP] = 0;								              //�ر�IC����֤��������UDP����
	              	  DFE.Write(EE_ENABLE_IC_AUTH_UDP,1,(uchar*)&sIP.EnableFlag[eFLAG_IC_AUTH_UDP]);
	              
				  }
				  else if(Dw==1)
				  {
				  	  sIP.EnableFlag[eFLAG_IC_AUTH_B_UDP] = 0;											 //�ر�IC����֤���ݷ�����UDP1����
				  	  DFE.Write(EE_ENABLE_IC_AUTH_B_UDP,1,(uchar*)&sIP.EnableFlag[eFLAG_IC_AUTH_B_UDP]);
				  }
				  else if(Dw==2)
				  {
				  	  sIP.EnableFlag[eFLAG_IC_AUTH_UDP] = ENABLE_FLAG;								              //��IC����֤����������UDP����
	                  DFE.Write(EE_ENABLE_IC_AUTH_UDP,1,(uchar*)&sIP.EnableFlag[eFLAG_IC_AUTH_UDP]);
				  }
				  else if(Dw==3)
				  {
				  	  sIP.EnableFlag[eFLAG_IC_AUTH_B_UDP] = ENABLE_FLAG;											 //��IC����֤�����ݷ�����UDP1����
				  	  DFE.Write(EE_ENABLE_IC_AUTH_B_UDP,1,(uchar*)&sIP.EnableFlag[eFLAG_IC_AUTH_B_UDP]);
				  }
				  else if(Dw<65535)
				  {
					  JT808_IP_DNS.IC_Auth_UDP_Port = Dw;
				      DFE.Write(EE_JT_IC_AUTH_UDP_PORT, 4,(uchar *)&JT808_IP_DNS.IC_Auth_UDP_Port);
				  }
				  else
				   t = 3;
				  	
			  }
			  else
			  {
				  DFE.Read(EE_JT_IC_AUTH_UDP_PORT, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  memmove(p, (uchar *)&Dw, 4);
				  t = 4;
			   }  	

			break;

		case	SET_SERVER_IC_AUTH_BAK_IP:									// ��·����֤IC ����֤���ݷ�����IP��ַ���������˿�ͬ��������
			if(s!=0)
			{			
				  if(s<=LEN_IPorDNS)               //IP��DNSĿǰֻ֧��32B����
				  {
				  	  SetBufferValue(&JT808_IP_DNS.IC_Auth_B_IPorDNS[0], LEN_IPorDNS, 0);
					  memmove((char *)&JT808_IP_DNS.IC_Auth_B_IPorDNS[0], (char *)p, s);
					  if(ValidIP_Port(JT808_IP_DNS.IC_Auth_B_IPorDNS)==OK)
					  {
						  sIP.EnableFlag[eFLAG_IC_AUTH_B_TCP] = ENABLE_FLAG;								   //�򿪱��ݷ�����TCP,UDP����
	                 	  //sIP.EnableFlag[eFLAG_UDP1] = ENABLE_FLAG;	
	                	  DFE.Write(EE_ENABLE_IC_AUTH_B_TCP,1,(uchar*)&sIP.EnableFlag[eFLAG_IC_AUTH_B_TCP]);
	                	  //DFE.Write(EE_ENABLE_UDP1,1,(uchar*)&sIP.EnableFlag[eFLAG_UDP1]);
						  DFE.Write(EE_JT_IC_AUTH_B_IP_DNS, LEN_IPorDNS, (uchar *)&JT808_IP_DNS.IC_Auth_B_IPorDNS[0]);
					     t = 80;
					  }
					  else
					  {
					  	  //sIP.EnableFlag[eFLAG_TCP] = 0;								              //�ر���������TCP,UDP����
	                 //sIP.EnableFlag[eFLAG_UDP] = 0;	
	                 //DFE.Write(EE_ENABLE_TCP,1,(uchar*)&sIP.EnableFlag[eFLAG_TCP]);
	                 //DFE.Write(EE_ENABLE_UDP,1,(uchar*)&sIP.EnableFlag[eFLAG_UDP]);
					  	  t = 3;
					  }
				  } 
				  else
				    t = 3;
		  	 }
			 else
		    {  			 	
			
				  DFE.Read(EE_JT_IC_AUTH_B_IP_DNS, LEN_IPorDNS,(uchar *)tB);
				  t = strlen((char *)tB);
				  memmove(p, tB, t);
			 }

			break;

		case	SET_ELE_RAID_RADIUS_ID:			   // ����Χ���뾶	���Ƿ�λ����ֵ�� ����λΪ��
			if(s!=0)
			{	
				Dw = ByteToINT16(*p, *(p+1));
			    DFE.Write(EE_JT_ELE_RAID_RADIUS, 2, (uchar *)&Dw); 	 
			}
			else
		    {  
				DFE.Read(EE_JT_ELE_RAID_RADIUS, 2,(uchar *)&Dw);
				t = 2;
				Dw = SwapINT16(Dw);
				memmove(p, (uchar *)&Dw, t);
			 }	
			
			break;


		case	SET_OVER_SPEED_PRE_ALARM_ID:	 // ���ٱ���Ԥ����ֵ
			if(s!=0)
			{
				Dw = ByteToINT16(*p, *(p+1));
			   DFE.Write(EE_JT_OVER_SPEED_PRE_ALARM, 2, (uchar *)&Dw); 	
			    // ��ʱ��������													 
				  SpeedC.SetGetPara( eSPEED_CMD_GET_OVER_SPEED,LEN_OVER_SPEED_PARA,Tp.Buffer);
				  Tp.Buffer[3] = Dw/10;		// ��ȡС��
				  SpeedC.SetGetPara( eSPEED_CMD_SET_OVER_SPEED,LEN_OVER_SPEED_PARA,Tp.Buffer);
			}
			else
		    {  
				  DFE.Read(EE_JT_OVER_SPEED_PRE_ALARM, 2,(uchar *)&Dw);
				  t = 2;
				  Dw = SwapINT16(Dw);
				  memmove(p, (uchar *)&Dw, t);
			}	
			
			break;

		case	SET_TRIAL_FATIGUE_DRIVE_PRE_ALARM_ID:							 // ƣ�ͼ�ʻԤ����ֵ
			if(s!=0)
			{
				Dw = ByteToINT16(*p, *(p+1));
			    DFE.Write(EE_JT_FATIGUE_DRIVE_PRE_ALARM, 2, (uchar *)&Dw); 
			    // ��ʱ��������													 
				  SpeedC.SetGetPara( eSPEED_CMD_GET_DRIVING_TIME,LEN_DRIVING_TIME_LIMIT,Tp.Buffer);
				  Tp.B32[4] = Dw;
				  SpeedC.SetGetPara( eSPEED_CMD_SET_DRIVING_TIME,LEN_DRIVING_TIME_LIMIT,Tp.Buffer);
			}
			else
		    {  
				  DFE.Read(EE_JT_FATIGUE_DRIVE_PRE_ALARM, 2,(uchar *)&Dw);
				  t = 2;
				  Dw = SwapINT16(Dw);
				  memmove(p, (uchar *)&Dw, t);
			}	
			
			break;

		case	SET_CRASH_ALARM_PARA_ID:									 // ��ײ������������			  
			if(s!=0)
			{
				Dw = ByteToINT16(*p, *(p+1));
			    DFE.Write(EE_JT_CRASH_ALARM_PARA, 2, (uchar *)&Dw); 	 
			}
			else
		    {  
				  DFE.Read(EE_JT_CRASH_ALARM_PARA, 2,(uchar *)&Dw);
				  t = 2;
				  Dw = SwapINT16(Dw);
				  memmove(p, (uchar *)&Dw, t);
			}	
			
			break;
	

		case	SET_TILT_ALARM_PARA_ID:									 // �෭������������			  
			if(s!=0)
			{
				Dw = ByteToINT16(*p, *(p+1));
			   	DFE.Write(EE_JT_TILT_ALARM_PARA, 2, (uchar *)&Dw); 	
				NOP(); NOP(); NOP(); 
				DFE.Write(EE_ACCEL_TILT_PARA + 3, 1, p); 		 // �Ƕ�ֵ	ע�� ֻдһ���ֽ�
				NOP(); NOP(); NOP(); 
			}
			else
		    {  
				  DFE.Read(EE_JT_TILT_ALARM_PARA, 2,(uchar *)&Dw);
				  t = 2;
				  Dw = SwapINT16(Dw);
				  memmove(p, (uchar *)&Dw, t);
			}	
			
			break;
		
		case	 SET_FIXED_TIME_TAKE_PHOTO_CTL_ID:						 // ��ʱ���տ���	

			if(s!=0)
			{
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  DFE.Write(EE_JT_FIXED_TIME_TAKE_PHOTO_CTL, 4,(uchar *)&Dw);	
			 }
			 else
		     {  
				  DFE.Read(EE_JT_FIXED_TIME_TAKE_PHOTO_CTL, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  t = 4;
				  memmove(p, (uchar *)&Dw, t);
			 }	
			
			break;

		case	 SET_DISTANCE_TAKE_PHOTO_CTL_ID:						 // �������տ���	

			if(s!=0)
			{
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  DFE.Write(EE_JT_DISTANCE_TIME_TAKE_PHOTO_CTL, 4,(uchar *)&Dw);	
			 }
			 else
		     {  
				  DFE.Read(EE_JT_DISTANCE_TIME_TAKE_PHOTO_CTL, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  t = 4;
				  memmove(p, (uchar *)&Dw, t);
			 }	
			
			break;

		case SET_GNNS_POS_MODE_ID:									// GNSS ��λģʽ
			if(s!=0)
			{				
			   DFE.Write(EE_JT_GNNS_POS_MODE, 1, p);
				JT808Flag.VS3.Bits.UseBD = (*p >> 1) & 0x01;
				JT808Flag.VS3.Bits.UseGPS = (*p >> 0) & 0x01;	
				////// Ӧ�����ԣ����Ȳ���ʹ��
				#if( TEST_CODE_JUST_FOR_TEST_LUOYANG_NEW==1 )
				TestAccuracy = TRUE;		

				if( (JT808Flag.VS3.Byte&0x0c)==0x0c)
				{
					Uart.puts("Զ������˫ģ��λģʽ ");
				}
				else if(JT808Flag.VS3.Bits.UseGPS==1)
				{
					Uart.puts("Զ�����õ�GPS��λģʽ ");
				}
				else if(JT808Flag.VS3.Bits.UseBD==1)
				{
					Uart.puts("Զ�����õ�������λģʽ ");
				}		
				#endif
			}
			else
		    {  
				DFE.Read(EE_JT_GNNS_POS_MODE, 1,(uchar *)&Dw);
				t = 1;
				memmove(p, (uchar *)&Dw, t);
			}
			
			break;
		case	SET_GNSS_BAUD_RATE_ID:						  			// GNSS������
			if(s!=0)
			{
			    DFE.Write(EE_JT_GNSS_BAUD_RATE, 1, p);	
				NOP(); NOP(); NOP(); 
				s1 = *p;
				if (s1 == 0x00)	Dw = 4800UL;
				else if (s1 == 0x01)		Dw = 9600UL;
				else if (s1 == 0x02)		Dw = 19200UL;
				else if (s1 == 0x03)		Dw = 38400UL;
				else if (s1 == 0x04)		Dw = 57600UL;
				else if (s1 == 0x05)		Dw = 115200UL;
				else						Dw = 115200UL;

				DFE.Write(EE_BAUDRATE_U3_232, 4, (uchar *)&Dw);
				NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); NOP(); 

			}
			else
		    {  
				DFE.Read(EE_JT_GNSS_BAUD_RATE, 1,(uchar *)&Dw);
				t = 1;
				memmove(p, (uchar *)&Dw, t);
			}

			break;
		case	SET_GNSS_DATA_OUTPUT_FREQUENCY_ID:							// GNSSģ����ϸ��λ�������Ƶ��
			if(s!=0)
			{
			    DFE.Write(EE_JT_GNSS_DATA_OUTPUT_FREQUENCY, 1, p);	
			}
			else
		    {  
				DFE.Read(EE_JT_GNSS_DATA_OUTPUT_FREQUENCY, 1,(uchar *)&Dw);
				t = 1;
				memmove(p, (uchar *)&Dw, t);
			}

			break;
		case	SET_GNSS_DATA_COLLECT_FREQUENCY_ID:							// GNSSģ����ϸ��λ���ݲɼ�Ƶ��
			if(s!=0)
			{
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  DFE.Write(EE_JT_GNSS_DATA_COLLECT_FREQUENCY, 4,(uchar *)&Dw);	
			 }
			 else
		     {  
				  DFE.Read(EE_JT_GNSS_DATA_COLLECT_FREQUENCY, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  t = 4;
				  memmove(p, (uchar *)&Dw, t);
			 }	
		
			break;

		case	SET_GNSS_DATA_UPLOAD_MODE_ID:							    // GNSSģ����ϸ��λ�����ϴ���ʽ
			if(s!=0)
			{
			    DFE.Write(EE_JT_GNSS_DATA_UPLOAD_MODE, 1, p);	
			}
			else
		    {  
				DFE.Read(EE_JT_GNSS_DATA_UPLOAD_MODE, 1,(uchar *)&Dw);
				t = 1;
				memmove(p, (uchar *)&Dw, t);
			}
			//GetGnssDataFlag = TRUE;
			//TD.GPS = tGPS_USE_GPS_BD;
			//OnlyUseBD_GPS = 0;
			Uart.puts("Զ�����òɼ�����ԭʼ���ݵ�ģʽ ");
			break;
		case	SET_GNSS_DATA_UPLOAD_SET_ID:							    // GNSSģ����ϸ��λ�����ϴ�����
			if(s!=0)
			{
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  DFE.Write(EE_JT_GNSS_DATA_UPLOAD_SET, 4,(uchar *)&Dw);	
			 }
			 else
		     {  
				  DFE.Read(EE_JT_GNSS_DATA_UPLOAD_SET, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  t = 4;
				  memmove(p, (uchar *)&Dw, t);
			 }	
			//GetGnssDataFlag = TRUE;
			//TD.GPS = tGPS_USE_GPS_BD;
			//OnlyUseBD_GPS = 0;
			Uart.puts("Զ�����òɼ�����ԭʼ���ݵļ�� ");
			break;	
	
		case	SET_CAN_CHAN_1_COLLECT_TIME_INTERVAL_ID:		         // CAN����ͨ��1 �ɼ�ʱ����(ms)��0��ʾ���ɼ�
			if(s!=0)
			{
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  DFE.Write(EE_JT_CAN_CHAN_1_COLLECT_TIME_INTERVAL, 4,(uchar *)&Dw);	
				  JT_CanSet.Can1SampleInterval = Dw;
			 }
			 else
		     {  
				  DFE.Read(EE_JT_CAN_CHAN_1_COLLECT_TIME_INTERVAL, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  t = 4;
				  memmove(p, (uchar *)&Dw, t);
			 }

			break;

		case	SET_CAN_CHAN_1_UPLOAD_TIME_INTERVAL_ID:			        	// CAN����ͨ��1 �ϴ�ʱ����(s)��0��ʾ���ɼ�
			if(s!=0)
			{
				  Dw = ByteToINT16(*p, *(p+1));
				  DFE.Write(EE_JT_CAN_CHAN_1_UPLOAD_TIME_INTERVAL, 2,(uchar *)&Dw);	
				  JT_CanSet.Can1UploadInterval = Dw;
				  Uart.puts("Զ���豸CANͨ��1�ϴ������");
				  Int2Str( JT_CanSet.Can1UploadInterval, (char*)Tp.Buffer);
				  Uart.puts((char*)Tp.Buffer);
			 }
			 else
		     {  
				  DFE.Read(EE_JT_CAN_CHAN_1_UPLOAD_TIME_INTERVAL, 2,(uchar *)&Dw);
				  Dw = SwapINT16(Dw);
				  t = 2;
				  memmove(p, (uchar *)&Dw, t);
			 }

			break;

		case SET_CAN_CHAN_2_COLLECT_TIME_INTERVAL_ID:         			// CAN����ͨ��2 �ɼ�ʱ����(ms)��0��ʾ���ɼ�
			if(s!=0)
			{
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  DFE.Write(EE_JT_CAN_CHAN_2_COLLECT_TIME_INTERVAL, 4,(uchar *)&Dw);
				  JT_CanSet.Can2SampleInterval = Dw;	
			 }
			 else
		     {  
				  DFE.Read(EE_JT_CAN_CHAN_1_COLLECT_TIME_INTERVAL, 4,(uchar *)&Dw);
				  Dw = SwapINT32(Dw);
				  t = 4;
				  memmove(p, (uchar *)&Dw, t);
			 }

			break;

		case	SET_CAN_CHAN_2_UPLOAD_TIME_INTERVAL_ID:			        	// CAN����ͨ��2 �ϴ�ʱ����(s)��0��ʾ���ɼ�
			if(s!=0)
			{
				  Dw = ByteToINT16(*p, *(p+1));
				  DFE.Write(EE_JT_CAN_CHAN_2_UPLOAD_TIME_INTERVAL, 2,(uchar *)&Dw);	
				  JT_CanSet.Can2UploadInterval = Dw;
			 }
			 else
		     {  
				  DFE.Read(EE_JT_CAN_CHAN_2_UPLOAD_TIME_INTERVAL, 2,(uchar *)&Dw);
				  Dw = SwapINT16(Dw);
				  t = 2;
				  memmove(p, (uchar *)&Dw, t);
			 }

			break;


		 		
	    default:

		 	///////////// CAN ID����ʱֻ����16��
		   if ((CmdID>=SET_CAN_ID_COLLECT_SET_0_ID)	&& (CmdID<=SET_CAN_ID_COLLECT_SET_15_ID))	  
		   {
		   	s1 =  CmdID - SET_CAN_ID_COLLECT_SET_0_ID;		// ƫ��
		   	if (s!=0)
				{
					DFE.Write(EE_JT_CAN_ID_COLLECT_SET_BASE+8*s1, 8, p);	
					NOP(); NOP(); NOP(); 
					CanBus.JTSetSpecifyCanID(s1, p); 	

				}
				else
				{
					DFE.Read(EE_JT_CAN_ID_COLLECT_SET_BASE+8*s1, 8, p);
				  	t = 8;
				  	memmove(p, (uchar *)&Dw, t);
					
				}
				break;	
		   }		

		   if(s!=0) t = 3;
			
		 	break;
	  }
 return  t;

}










void		GPRSCls::RemoteGetVDR_Data(void)
{
	static	uchar		rCnt;
	uint16	Length;
	union
	{
		uchar		Buffer[256];		
		uint16	B16[256/2];
		uint32	B32[256/4];
	}Tp;
   union
	{
		uchar		Buffer[4];		
		uint16	B16[2];
		uint32	B32;
	}Tp1;
	/*if( GetVDR_DataFlag==TRUE)
	{		
		rCnt++;
		if( rCnt&0x01 )
		{	
			SetBufferValue(Tp.Buffer,256,0);
			Tp.Buffer[0] = 0xaa;
			Tp.Buffer[1] = 0x75;
			Tp.Buffer[2] = GetVDR_Cmd;
			Tp.Buffer[3] = Tp.Buffer[4] = 0;
			Tp.Buffer[5] = 0;
			Length = Uart.GB19056Process(6,Tp.Buffer,(uchar*)&Tp.Buffer[3]);
			Tp1.B16[0] = GetVDR_Index[GetVDR_Cmd];
			Tp.Buffer[0] = Tp1.Buffer[1];
			Tp.Buffer[1] = Tp1.Buffer[0];
			Tp.Buffer[2] = GetVDR_Cmd;
			BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_VDR_DATA_UPLOAD,(Length+3),Tp.Buffer);
			SetBufferValue(Tp.Buffer,256,0);
			strcpy( (char*)Tp.Buffer,"\r\nԶ�̶�ȡ��ʻ��¼������  ");
			ByteToHexChar( GetVDR_Cmd,(char*)&Tp.Buffer[26]);		
			Uart.puts((char*)Tp.Buffer);
			GetVDR_Cmd++;
			if(GetVDR_Cmd>7)		GetVDR_DataFlag = 0;
		}
	} */

	if( GetVDR_DataFlag==TRUE)
	{
		GetVDR_DataFlag = FALSE;
		SetBufferValue(Tp.Buffer,256,0);
			
		Tp.Buffer[0] = 0xaa;
		Tp.Buffer[1] = 0x75;
		Tp.Buffer[2] = GetVDR_Cmd;
		Tp.Buffer[3] = Tp.Buffer[4] = 0;
		Tp.Buffer[5] = 0;
		Length = Uart.GB19056Process(6,Tp.Buffer,(uchar*)&Tp.Buffer[3]);
		Tp1.B16[0] = GetVDR_Index[GetVDR_Cmd];
		Tp.Buffer[0] = Tp1.Buffer[1];
		Tp.Buffer[1] = Tp1.Buffer[0];
		Tp.Buffer[2] = GetVDR_Cmd;
		BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_VDR_DATA_UPLOAD,(Length+3),Tp.Buffer);
		
		SetBufferValue(Tp.Buffer,256,0);
		strcpy( (char*)Tp.Buffer,"\r\nԶ�̶�ȡ��ʻ��¼������  ");
		ByteToHexChar( GetVDR_Cmd,(char*)&Tp.Buffer[26]);		
		Uart.puts((char*)Tp.Buffer);
		
	}

}


// 08H - 15H ��¼�����ݲɼ�
uint16		GPRSCls::RemoteGetVDR_Data_08H_15H(uchar *pBuffer)
{
	uint32 Temp;
	uint16 Len = 0,TotalItem;
	uint32 Size = 0, L;
	uchar *pData;


	pData  =  TxdTBuffer;
	pData[0] = GetVDR_Index[GetVDR_Cmd]>>8;
	pData[1] = GetVDR_Index[GetVDR_Cmd];		
	pData[2] = GetVDR_Cmd;

	pData += 3;

	if (JT_VehRecord.PackIndex==0)
	{
		// ��ȡ�����ܳ�
		GB_LargeData.SearchFlag = 0;
		TotalItem = GB_LargeData.TotalItem;	 
		L = 1;
		JT_VehRecord.TotalPack = 0;
		while (L)
		{
			L = Uart.SendVDR_Data_08H_To_15H(1024,pData+6);
			if (L)  
			{
				Size += L;	//  ���ݳ��Ȳ�Ϊ0
				JT_VehRecord.TotalPack++;
			}
			else	  break;	
		}

		//  ȡ��һ������
		GB_LargeData.SearchFlag = 0;
		GB_LargeData.TotalItem = TotalItem;

		if ((JT_VehRecord.TotalPack==0))  JT_VehRecord.TotalPack = 1;
		
	}
	JT_VehRecord.PackIndex++;


	L = Uart.SendVDR_Data_08H_To_15H(1024,pData+6);

	pData[0] = 0x55;
	pData[1] = 0x7a;
	pData[2] = GetVDR_Cmd;
	pData[3] = L >> 8;				 
	pData[4] = L & 0xff;
	pData[5] = 0;
	Len = L+6;
	pData[Len++] = CalXorSum(Len,pData);	 

//  	if (JT_VehRecord.PackIndex==0)
// 	{
//		//Size += 3 + 6 + 1;	 // 3���ֽ�Ϊ�����ֺ�ƽ̨��ˮ�ţ�6���ֽ�ΪGB19056��ͷ����1���ֽ�ΪУ����
//		//JT_VehRecord.TotalPack = Size / Len;
//		//if ((Size % Len) || (JT_VehRecord.TotalPack==0))  JT_VehRecord.TotalPack += 1;
//		JT_VehRecord.PackIndex = 1;	 ///////////// ��1��ʼ
//	}
//	else
//	{
//		JT_VehRecord.PackIndex++; 
//	}

	pData -= 3;  // ָ��	 TxdTBuffer
	Len += 3;

	Temp =  (JT_VehRecord.PackIndex << 16) | JT_VehRecord.TotalPack;
	//if (JT_VehRecord.TotalPack==1) Temp = 0;
	 Temp = 0;	  // ��������ƽ̨����д�ܰ�������һ��һ���Ĵ���������������Ϊ0
	Len = JT808_Packaging(eBB_gCMD_TER_VDR_DATA_UPLOAD,Len,Temp,pData,(uchar*)pBuffer);

	

	Uart.puts("���� Test Data:");	Uart.putb(1,2,(uchar *)&JT_VehRecord.PackIndex); Uart.puts("\r\n");
	Uart.puts("���� Test Data:");	Uart.putb(1,2,(uchar *)&JT_VehRecord.TotalPack); Uart.puts("\r\n");


	os_dly_wait(80); // ��ʱ

	return Len;

}









/**************************************** End of File *************************************************/






