/***************************************  Copyright (c)  **************************************
**									 		
**								
**                      
**	  
****************************************  Copyright (c)  **************************************
**                                         �ļ�����
** �ļ���:   Camera.cpp
** ������������������ͷ�����ļ�
** ���뻷����Keil uVision3 + RTX
** Ŀ��CPU:  LPC2378
** �ӿ�ģ��: CRC.CPP  Uart.cpp	 
** ���ߣ�	 	
** ����ʱ�䣺2009.06.18
** �޸�ʱ��: 2009.07.18
*************************************************************************************************/	
#define      	CAMERA_GLOBALS		 1



#include 		"..\camera\camera.h"
#include 		"..\Common\flash_addr.h"
#include			"..\FLASH\DFE_25xxx.H"
#include			"..\FLASH\DFF_25xxx.H"
#include			"..\Uart\Uart1.h"
#include			"..\GSM_GPRS\GSM_GPRS.h"
#include		   "..\GPS\GPS.h"
#include			"..\Speed\Speed.h"

#include 	"..\GSM_GPRS\jt_t808.h"
#include	"..\Record\RecordModel.h"



 
//uchar			CamBuffer[800];//	__attribute__((at(0x7fe03000)));		// 1KB

//uchar			Cam_cBuffer[800];//		__attribute__((at(0x7fe03400)));		// 1KB

uchar    *CamBuffer;
uchar    *Cam_cBuffer;
uint16	set_camera_check_time;		  // ���ü������ͷʱ��
//static  uchar		jt_camera_count = 0;				  // ��ʼ��ʱ���������ͷ����
uchar   CameraUsing = 0;
uchar   TakePhotoErr = 0;

uchar		PicDataUpLoadFlag = 0;
uchar		TakePhoneAckFlag = 0;

extern   uchar  *pShareMem;

//extern uint32	jt_media_id;
extern uchar   jt_pic_send_flag;
extern uchar   jt_send_cmd_flag;
extern uchar	JT_ManulCameraID;
extern uchar	PicRetryOrMulPic;
extern uchar	ter_or_veh_reg_result;
extern uchar 	JTDelPicFlag;

uchar			NeedTakePhotoAck;


#define	USE_CARMERA_MODEL 		1






/*************************************************************************************************
** ��������: TaskCameraTrans()
** ��������: ͼƬ��������
** �������: ��
** �������: ��
** ȫ�ֱ���: ��
** �޸�ʱ��: 2009.08.21
************************************************************************************************/
extern		uchar			RecordTaskRun;
 __task		void			TaskCamera(void)
{
	uint32	cTickCnt;
	NOP();	NOP();
	#if (WUXI_TEST != 1)
	Camera.Init();	  
	NOP();	NOP();
	Record.Init();
	#else
	CAMERA_POWER_OFF;
	#endif
	set_camera_check_time = JT_CAMERA_CHECK_TIME;
	

		
	while(1)
	{
		 TC.Camera = 0;
		 CamererTaskNow = 0;

		 if(TD.MainTask==tTASK_RUN_FLAG)	  Uart.puts("Camera Task Run");

		 #if(WUXI_TEST == 1)
		 os_dly_wait(1000);
		 continue;
		 #else

		 if(U2PinSelDevice == U2_FOR_RFID)
		 	os_dly_wait(1);
		 else
		 	os_dly_wait(10);
		 
		 if( SamlePeriod1S || TestAccuracy || TestBlind || TestCanBus || U0_Iap_Flag )	
		 {
		 		TC.Camera = 0;
		 		continue;	// ���Ȳ��Ի�ä�����ԣ��������������
		 }
		 
		 if (TD.MainTask == tMAIN_RECORD_START)	 // init¼��
		 {
			TD.MainTask = 0;
		    Uart.puts("init record\r\n");
		    os_dly_wait(10);
			
			Record.StartRecord(20);	
		 }
		 if( RecordTaskRun )	 //123ʹ����
		 {
		  		if(RecordTaskRun==TRUE)	
					cTickCnt = 0;
			   RecordTaskRun = 0x5a;
				cTickCnt++;
				Record.RecordTask();
				TC.Camera = 0;
		 }
		 else if(U2PinSelDevice == U2_FOR_RFID)		 
		 {
		 	 SpeedC.U2_ForRFID_Fun();
		 }
		 else if((USBTaskCreat==0)&&(PrintTaskCreat==0)&&(U0_Iap_Flag==0) && (GG_Status.Flag.Bits.UpdateRemoteSet == 0)) 
		 {
		 		#if (JT_TEST_FREIGHT != 1)
			 	if (((gCnt.SystemSecond % set_camera_check_time) == 0) || (TD.Camera == tCAMERA_CHCEK))
				{
					if (TD.Camera == tCAMERA_CHCEK)		 
					{
						Uart.puts("Check Camera\r\n");
						os_dly_wait(1);
					}
					
					if (CameraUsing == 0)		 // ��������ʱ
						Camera.CheckCamera(); 						
					
				}
				#endif

				Camera.eTask();
		}	
		#endif	
	}
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void		CameraCls::eTask(void)
{
	#if(USE_CARMERA_MODEL==1)
	//uchar		s;
	uint32	i,k;
	uint32	Length,Addr;
	uint16	TotalPack,PackNo;
	union
	{
		uchar		Buffer[32];
		uint16	B16[2];
		uint32	B32;
	}Tp;
	//uchar		*pEnd;
	pSAVE_PIC_HEAD		pSaveHead;
	uchar     CurrentCameraID = Pic.CameraID;
	
	TickCnt++;
	
	TimeToPowerOff++;
	if(TimeToPowerOff==(600*10))// ʮ���Ӻ��Զ��ض�����ͷ��Դ
	{
		ForcePowerOn = 0;		
		CameraPowerOn = 0;
	}
	
	if(ForcePowerOn==TRUE)
	{
		CAMERA_POWER_ON;
		CameraPowerOn = TRUE;
	}
	
	if(CameraPowerOn==TRUE)
	{	CAMERA_POWER_ON;	}
	else
	{	CAMERA_POWER_OFF;	}



	///////////////////////////////////////////////////////////////
	NOP();	NOP();
	if ((Pic.TakeNumber) &&  (JT808Flag.VS6.Bits.CameraError == 0))
	{
		CamererTaskNow = 1;
		#if( TEST_CODE_JUST_FOR_TEST_LUOYANG==0 )
		if (JT_ManulCameraID != 0)	CurrentCameraID = JT_ManulCameraID;		// JT_ManulCameraID ��Ϊ�գ�����Ϊ����
		else		CurrentCameraID =  Pic.CameraID;
		#else
		CurrentCameraID = Pic.CameraID;
		#endif

		if((Pic.TakeStart==TRUE))
		{
			Uart2.Init(Baudrate.U2_485,U2_FOR_CAMERA);	// ����
			NOP();	NOP();
			while(CameraUsing)		{	os_dly_wait(1);	}
       	     CameraUsing = TRUE;		
			if(CameraPowerOn!=TRUE)
			{
				CameraPowerOn = TRUE;
				TestErrorCnt = 0;				
				CAMERA_POWER_ON;	 	// ������Դ����ʱ
				Uart.puts("Camera power on\r\n\0");
				for(i=0;i<15;i++)
				{
					os_dly_wait(100);
					CAMERA_POWER_ON;							
				}
			}
			TimeToPowerOff = 0;
			strcpy((char*)CamBuffer,"Take photo num: ");
			Int32ToDecChar(Pic.TakeNumber,(char*)&CamBuffer[16]);
			strcat((char*)CamBuffer,"\r\n\0");
			Uart.puts((char*)CamBuffer);
			/// 

			if (JT_ManulCameraID != 0)	CurrentCameraID = JT_ManulCameraID;		// JT_ManulCameraID ��Ϊ�գ�����Ϊ����
			else		CurrentCameraID =  Pic.CameraID;

			for (i=0; i<3; i++)
			{
				//Length = SetCommand(CurrentCameraID,eCAM_TEST,0,0,CamBuffer);
				Length = SetCommand(CurrentCameraID,eCAM_TEST,2,0,CamBuffer);
				if(Length==0xffff)	
				{
					Uart.puts("Test camera error\r\n\0");
					VehicleStatus.VS5.Bits.CameraOK = 0;
					//TestErrorCnt++;
					//if(TestErrorCnt>3)  	// 3��ʧ�ܣ�������������ͷ
					if(i==2)  	// 3��ʧ�ܣ�������������ͷ
					{
						if ((!(JT808_Alarm_Word.Shield & (0x01 << 11))))
						{
						  	if (JT808_Alarm_Word.SendTextSMS & (0x01 << 11))		     								// ���Ͷ���
						  	{
							 	GSM.SetSMS_Buffer((char *)&JT808_Number.TextSMS[0],"Camera	Error Alarm!!!");		
						  	}
							JT808Flag.VS6.Bits.CameraError = 1;
						}
						CamererTaskNow = 0;
						Pic.TakeNumber = 0;
						JT_ManulCameraID = 0;
						Jt_photo_upload = 0;
						Pic.SaveAndSend = 0;
						CameraUsing = 0;
						jt_pic_send_flag = 0;
						if(!Protocol.GPRS[JT_PROTOCOL])				// �ǲ����򱨸�����ͷ����
							GPRS.TH_NewDataPack((uchar*)"S39\0",(uchar*)"Camera error or disconnected\0");
					}
				}
				else
				{
					if (JT808Flag.VS6.Bits.CameraError == 1)
					{
						JT808_GPRS_TempPack = TRUE;
						os_dly_wait(10);
						Uart.puts("TP. Camera ER to OK");
					}					
	
					JT808Flag.VS6.Bits.CameraError = 0;
					VehicleStatus.VS5.Bits.CameraOK = 1;
					Pic.TakeStart = 0;
				}
			}
			if(VehicleStatus.VS5.Bits.CameraOK == 1)
			{
				os_dly_wait(20);			
			//	Length = SetCommand(CurrentCameraID,eCAM_SET_RESOLUTION,1,Pic.Resolution,CamBuffer);	 //
				Length = SetCommand(CurrentCameraID,eCAM_SET_RESOLUTION,2,Pic.Resolution,CamBuffer);	 //
				if(Length==0xffff)	{Uart.puts("Set resultion error\r\n\0");}
				os_dly_wait(20);
				/// 
//				Length = SetCommand(CurrentCameraID,eCAM_SET_PACK_SIZE,2,512,CamBuffer);	   //delete
//				if(Length==0xffff)	{Uart.puts("Set pack size error\r\n\0");}
//				os_dly_wait(20);
			}
		}
		///////////////////////////////////////// ����ͼƬ
		if(VehicleStatus.VS5.Bits.CameraOK == 1)
		{
		//	Length = SetCommand(CurrentCameraID,eCAM_TAKE_PHOTO_RET_SIZE,0,0,CamBuffer);
			Length = SetCommand(CurrentCameraID,eCAM_TAKE_PHOTO_RET_SIZE,2,0,CamBuffer);	//�ڲ��������޸�
			if(Length==0xffff)	
			{
				Uart.puts("Take photo error\r\n\0");

				if (++TakePhotoErr > 3)
				{
					Pic.TakeNumber = 0;
					JT_ManulCameraID = 0;
					Jt_photo_upload = 0;
					Pic.SaveAndSend = 0;
					CameraUsing = 0;
					jt_pic_send_flag = 0;
					CamererTaskNow = 0;

					Uart2.Init(Baudrate.U2_232,U2_FOR_DISPLAY);	// ����
					return;
					
				}
				if (++TakePhotoErr > 2)			  // �����������ֻ��һ�ţ����ľͳ��������
				{
					CAMERA_POWER_OFF;
					os_dly_wait(100);
					Pic.TakeStart = TRUE;
					CameraPowerOn = !TRUE;		
					CameraUsing = 0;
					Uart2.Init(Baudrate.U2_232,U2_FOR_DISPLAY);	// ����
					return;
				}				
			}
			else
			{	
				TakePhotoErr = 0;
				Tp.B32 = 0;
				Tp.Buffer[0] = CamBuffer[5]; 	
				Tp.Buffer[1] = CamBuffer[6];
				TotalPack = Tp.B32;	  				// ͼƬ�ܰ���
				
				if (TotalPack > 256)	TotalPack = 256;			
				///////////////////////////
				Uart.puts("1111 ");
				Int2Str(TotalPack, (char *)Tp.Buffer);
				Uart.puts((char *)Tp.Buffer);
				Uart.puts("\r\n");
				///
				
				Addr = FF_PIC_START_PAGE;
				k = (FlashPageCnt.PicSave%FF_PIC_TOTAL_PAGE);
				Length = Addr - k;		// 
				if(Length<(TotalPack<<1))
				{	// ���洢�ռ䲻���洢1��ͼƬ����ͷ��ʼ
					FlashPageCnt.PicSave += Length;
				}
				
				PackNo = 0;
				for(k=0;k<TotalPack;k++)
				{
					Length = SetCommand(CurrentCameraID,eCAM_REQ_DATA_PACK,2,k,CamBuffer);
					if(Length==0xffff)
					{
						Uart.puts("Read photo data error\r\n\0");
						ter_or_veh_reg_result = ERROR;  // ʧ��
						CameraUsing = 0;
						CamererTaskNow = 0;
						JT_ManulCameraID = 0;
						Jt_photo_upload = 0;
						Pic.SaveAndSend = 0;
						GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_TAKE_PHOTO_ACK,0,NULL);
						os_dly_wait(10);
						Uart.puts("����11 ");	 
						Uart2.Init(Baudrate.U2_232,U2_FOR_DISPLAY);	// ����	
						
					}
					else if(Length<600)
					{
						Addr = FF_PIC_START_PAGE;
						Addr += (FlashPageCnt.PicSave%FF_PIC_TOTAL_PAGE);
		
						if((Addr%FF_PAGE_PER_SECTOR)==0)
						{
							DFF.SectorEraseWaitOK( Addr/FF_PAGE_PER_SECTOR ,10);
						}

						// add by xzb
						if (k == 0)		   // ��1��ǰ���Ӷ�ý��ͷ��Ϣ��λ����Ϣ��	�����ڲ���
						{
							SetBufferValue(Cam_cBuffer,800, 0);
										  			
							MultiMediaReport.MediaId = SwapINT32(Pic.TotalNumber+1);	// ��֤��ý��ID����0
					  	 	MultiMediaReport.MediaType = 0;			                         // ����:ͼ��	  
					 		MultiMediaReport.MediaFormat = 0;										 // ��ʽ��JPEG
						 	MultiMediaReport.MediaEvent = MediaEventFlag;
							//MultiMediaReport.MediaChannel = 0x02;	   // ���õڶ�ͨ��
							#if( TEST_CODE_JUST_FOR_TEST_LUOYANG==1 )
							MultiMediaReport.MediaChannel = 0x01;	  
							#else								 
							MultiMediaReport.MediaChannel = CurrentCameraID;
							#endif
							// ͼƬ�洢����ҳ����ÿ��512B��2ҳ��
							pSaveHead = (pSAVE_PIC_HEAD)&Cam_cBuffer[0];
							
							pSaveHead->HeadFlag = SAVE_PIC_HEAD_FLAG;				// JPEG
							pSaveHead->TotalNumber = Pic.TotalNumber;				// ������							
							pSaveHead->StartTimeSec = gCnt.RtcSecond;				// ����ʱ��							
							pSaveHead->TotalPage = ((TotalPack<<1)+1);			// ����ͷҳ��ͼƬ��Ϣ���������ԣ�
							memmove(&Cam_cBuffer[16], (uchar *)&MultiMediaReport, 8);
							memmove(&Cam_cBuffer[24], (uchar*)&GPS.JTPack, LEN_JT808_GPS_PACK); 							
							DFF.PageProgramWaitOK(Addr, Cam_cBuffer,3);		// �洢���յ��ļ�ͷ��Ϣ 							
							FlashPageCnt.PicSave++;

							/// ��0����ֱ��ʹ��Cam_cBuffer����
							memmove(&Cam_cBuffer[0], (uchar *)&MultiMediaReport, 8);
							memmove(&Cam_cBuffer[8], (uchar*)&GPS.JTPack, LEN_JT808_GPS_PACK); 	
							memmove(&Cam_cBuffer[8+LEN_JT808_GPS_PACK],&CamBuffer[3],512); 

							if(Protocol.GPRS[JT_PROTOCOL])
							{
								if (TakePhotoAck.MediaNum<10)
								{
									TakePhotoAck.MediaID[TakePhotoAck.MediaNum] = (Pic.TotalNumber%TOTAL_PHOTO_NUM) + 1;
									TakePhotoAck.MediaNum++;
								}
								
							}
						 }
						///////////////////// ÿ��2ҳ
						Addr = FF_PIC_START_PAGE;
						Addr += (FlashPageCnt.PicSave%FF_PIC_TOTAL_PAGE);
						DFF.PageProgramWaitOK(Addr,&CamBuffer[3],3);
						FlashPageCnt.PicSave++;

						Addr = FF_PIC_START_PAGE;
						Addr += (FlashPageCnt.PicSave%FF_PIC_TOTAL_PAGE);
						DFF.PageProgramWaitOK(Addr,&CamBuffer[3+256],3);
						FlashPageCnt.PicSave++;
						PackNo++;
						if(Pic.SaveAndSend || Jt_photo_upload)
						{
							//if(Protocol.GPRS[GY_PROTOCOL])
							//	PictureDataToGPRS((Pic.TotalNumber%TOTAL_PHOTO_NUM),TotalPack,k,&CamBuffer[3]);
							if(Protocol.GPRS[JT_PROTOCOL] || Jt_photo_upload)
							{
								if( (k == 0) &&(NeedTakePhotoAck==TRUE))
								{
									if(TakePhoneAckFlag==TRUE)
									{
										ter_or_veh_reg_result = OK;
										GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_TAKE_PHOTO_ACK,0,NULL);
										Uart.puts(" ƽ̨����ָ��Ӧ�� ");	 								
										TakePhoneAckFlag = 0;
									}
									else
									{
										GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_MEDIA_EVENT_UPLOAD,0,NULL);	// ���Ͷ�ý���ϴ��¼�
										Uart.puts(" �ϱ���ý���¼����� ");	 								
									}
									
									
								  	for(i = 0; i < 500; i++)		// �����ʱ2��
									{
										if(SendPicData)
											os_dly_wait(1);
										else
											break;		
									}
									PicDataUpLoadFlag = TRUE;
									os_dly_wait(200);
								}
								if(k==0)		JT_PictureDataToGPRS((512+8+LEN_JT808_GPS_PACK),TotalPack,k,NULL);
								else			JT_PictureDataToGPRS(512,TotalPack,k,&CamBuffer[3]);

							}
						}
					}					
				}
				if(PackNo==TotalPack)	  // ��ȡ������ͼƬ����
				{
					Pic.TotalNumber++;
					Uart.puts("Take 1 photo OK\r\n\0");
				}
				else						  		// ͼƬ����δ��ȡ����
				{
					Uart.puts("\r\n\0");	
					Uart.putb(1,4,Tp.Buffer);
					Uart.puts("Pack1\r\n\0");					
					Uart.putb(1,4,(uchar*)&TotalPack);
					Uart.puts("Pack2\r\n\0");
				}	
				Pic.TakeNumber--;		// ����ɹ�1��
				CameraPowerOn = 0;
				jt_pic_send_flag = 0;
				TimeToPowerOff = 0;
				DFE.Write(EE_PICTURE_TOTAL_NUM,4,(uchar*)&Pic.TotalNumber);	
				if(Pic.TakeNumber==0)
				{
					
					if(Protocol.GPRS[JT_PROTOCOL])
					{
						os_dly_wait(20);		// ��GSM�������һ��ͼƬ����
						ter_or_veh_reg_result = OK;  // �ɹ�
						GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_TAKE_PHOTO_ACK,0,NULL);
						Uart.puts("�洢������Ƭ������� ");
						os_dly_wait(10);
							
					}
					CameraUsing = 0;
					CamererTaskNow = 0;
					JT_ManulCameraID = 0;
					Jt_photo_upload = 0;
					Pic.SaveAndSend = 0;
					Uart2.Init(Baudrate.U2_232,U2_FOR_DISPLAY);	// ����	
				}		 				 							
			}			
		}
	}
	else if(Pic.SendPicNo)		// �ϴ�ָ����ͼƬ
	{
		NOP();	NOP();
		while(CameraUsing)		{	os_dly_wait(1);	}
      CameraUsing = TRUE;	
      PicDataUpLoadFlag = TRUE;
      TotalPack = Length = 0;
      for(Addr=FF_PIC_START_PAGE;Addr<FF_PIC_END_PAGE;Addr++)
		{
			DFF.PageRead(Addr,Cam_cBuffer);
			pSaveHead = (pSAVE_PIC_HEAD)&Cam_cBuffer[0];
			Tp.B32 = SwapINT32(pSaveHead->MediaId);					// 
			if( Tp.B32 == Pic.SendPicNo )
			{
				TotalPack = (((pSaveHead->TotalPage)-1)>>1);
				memmove( CamBuffer,&Cam_cBuffer[16],LEN_SEND_PIC_HEAD);
				Length = LEN_SEND_PIC_HEAD;
				Addr++;
				Uart.puts(" �Ѽ�����ָ��ID��ͼƬ ");
				break;				
			}
			else if( pSaveHead->HeadFlag == 0xffffffff )
			{
				Uart.puts(" δ������ָ��ID��ͼƬ ");
				break;
			}
		}					   
		
		os_dly_wait(100);	 
		if(TotalPack)		
		{
			SetBufferValue(Tp.Buffer,32,0);	
			Uart.puts("ָ��ID: ");
			Int2Str( Pic.SendPicNo,(char*)Tp.Buffer);	
			strcat( (char*)Tp.Buffer,",PA: ");
			k = strlen( (char*)Tp.Buffer);
		   Int2Str( k,(char*)&Tp.Buffer[k]);	
			strcat( (char*)Tp.Buffer,"\r\n");
			Uart.puts((char*)Tp.Buffer);
		}
		for(k=0;k<TotalPack;k++)
      {	// ��1��Length�ĳ�ʼֵΪLEN_SEND_PIC_HEAD
      	DFF.PageRead(Addr,&CamBuffer[Length]);		
      	Addr++;
      	Length += 256;
      	DFF.PageRead(Addr,&CamBuffer[Length]);
      	Addr++;
      	Length += 256;
      	
      	JT_PictureDataToGPRS(Length,TotalPack,k,CamBuffer);
      	Length = 0;
      	
      }
      os_dly_wait(100);
      PicDataUpLoadFlag = 0;
      CameraUsing = 0;
      Pic.SendPicNo = 0;
      
      
	}
	else if(Pic.SendNumber)			// �ϴ�ȫ��δ�ϴ���ͼƬ
	{
		/*
		CamererTaskNow = 1;		
		if(Pic.SendPicNo==255)	/// ��������
			Pic.SendPicNo = (Pic.TotalNumber%TOTAL_PHOTO_NUM)-1;
		else
			Pic.SendPicNo = Pic.SendPicNo%TOTAL_PHOTO_NUM;

		k = Pic.SendPackNo;
		if(Pic.SendPackNo==255)
			DFE.Read(((Pic.SendPicNo<<1)+EE_PER_PIC_TOTAL_PACK),2,(uchar*)&TotalPack);	// ��ȡ�ܰ���
		else
			TotalPack = 1;
		Pic.SendPackNo = Pic.SendPackNo%128;		// ���128����
		
		PageAddr = (PIC_BLOCK_ADDR_BASE + Pic.SendPicNo) << 8 ;   // ��ȡ�Ķ�ý��ҳ��ַ��ַ
		PageAddr++;				// ����λ����Ϣ

		if(TotalPack==1)
			PageAddr += (Pic.SendPackNo << 1);

		if (TotalPack > 128)			// 		  ͼƬ���ܳ���64K = 512 * 128
		{
			TotalPack = 128;              // 1 - 128       ��128�� ��ÿ��512�ֽ�
		}

		for(i=0;i<TotalPack;i++)
		{	
			DFF.PageRead(PageAddr, &CamBuffer[0]);				PageAddr++;
			DFF.PageRead(PageAddr,&CamBuffer[256]);		   PageAddr++;	

			NOP();	NOP();	NOP();	NOP();
			NOP();	NOP();	NOP();	NOP();
		
			if(k==255)		PackNo = i;
			else	PackNo = Pic.SendPackNo;

			if(Protocol.GPRS[GY_PROTOCOL])
			{
				PictureDataToGPRS(Pic.SendPicNo,TotalPack,PackNo,CamBuffer);
			}	
			if(Protocol.GPRS[JT_PROTOCOL])
			{
				JT_PictureDataToGPRS(Pic.SendPicNo,TotalPack,PackNo,CamBuffer);
			}			
		}
		Pic.SendNumber--;

		if (Pic.SendNumber == 0)
		{
			CameraUsing = 0;
			JTDelPicFlag = 0;
			Protocol.GPRS[JT_PROTOCOL] = 0;
			CamererTaskNow = 0;

			//if (PicRetryOrMulPic == 2)   PicRetryOrMulPic = 0;		// ����洢��ý������
			PicRetryOrMulPic = 0;
		}
		*/
	}
	
	///////////////////////////////////////////////////////////
	/*if((TD.Camera>0) && (TD.Camera!=tCAMERA_ALL_DATA)&&(CameraUsing == 0))
	{
		Uart2.Init(Baudrate.U2_485,U2_FOR_CAMERA);
		os_dly_wait(10);
		TD.Camera &= 0x00ff;

		TD.Camera &= 0x00ff;
		if(TD.Camera=='A')
			Pic.CameraID = 0x01;
		else if(TD.Camera=='B')
			Pic.CameraID = 0xff;

		//if(TD.Camera==0x01)
		if(TD.Camera=='5')
		{
			ForcePowerOn = TRUE;
			TimeToPowerOff = 0;
			Length = SetCommand(Pic.CameraID,TD.Camera,0,0,CamBuffer);
			PicPackNum = 0;
		}
		if(TD.Camera=='0')
		{
			Length = SetCommand(Pic.CameraID,TD.Camera,0,0,CamBuffer);
		}
		else if(TD.Camera=='1')
		{
			Length = SetCommand(Pic.CameraID,TD.Camera,2,PicPackNum,CamBuffer);
			PicPackNum++;
		}
		else if(TD.Camera=='2')
		{
			Length = SetCommand(Pic.CameraID,TD.Camera,2,512,CamBuffer);
		}
		else if(TD.Camera=='3')
		{
			Length = SetCommand(Pic.CameraID,TD.Camera,0,0,CamBuffer);
		}
		else if(TD.Camera=='4')
		{
			Length = SetCommand(Pic.CameraID,TD.Camera,1,Pic.Resolution,CamBuffer);
		}
		else 
		{				
			if((TD.Camera>100)&&(TD.Camera<150))
				Pic.SendNumber = TD.Camera-100;
			if((TD.Camera>200)&&(TD.Camera<250))
				Pic.TakeNumber = TD.Camera-200;
			Length = 1000;
		}
		if(Length<800)
		{
			Tp.B32 = Length;		
			Uart.putb(1,4,Tp.Buffer);
			Uart.putb(1,Length,CamBuffer);			
		}
		TD.Camera = 0;
		Uart2.Init(Baudrate.U2_232,U2_FOR_DISPLAY);
		os_dly_wait(10);
	}	*/
	///////////////////////////////////////////////////////////
	if((TD.Camera>0) && (TD.Camera!=tCAMERA_ALL_DATA)&&(CameraUsing == 0))
	{
		CAMERA_POWER_ON;
		os_dly_wait(500);
		CAMERA_POWER_ON;
		Uart2.Init(Baudrate.U2_485,U2_FOR_CAMERA);
		os_dly_wait(10);

		TD.Camera &= 0x00ff;
		if(TD.Camera=='A')
			Pic.CameraID = 0x01;
		else if(TD.Camera=='B')
			Pic.CameraID = 0xff;

		//if(TD.Camera==0x01)
		if(TD.Camera=='5')
		{
			ForcePowerOn = TRUE;
			TimeToPowerOff = 0;
			Length = SetCommand(Pic.CameraID,TD.Camera,0,0,CamBuffer);
			PicPackNum = 0;
		}
		if(TD.Camera=='0')
		{
			Length = SetCommand(Pic.CameraID,TD.Camera,0,0,CamBuffer);
		}
		else if(TD.Camera=='1')
		{
			Length = SetCommand(Pic.CameraID,TD.Camera,2,PicPackNum,CamBuffer);
			PicPackNum++;
		}
		else if(TD.Camera=='2')
		{
			Length = SetCommand(Pic.CameraID,TD.Camera,2,512,CamBuffer);
		}
		else if(TD.Camera=='3')
		{
			Length = SetCommand(Pic.CameraID,TD.Camera,0,0,CamBuffer);
		}
		else if(TD.Camera=='4')
		{
			Length = SetCommand(Pic.CameraID,TD.Camera,1,Pic.Resolution,CamBuffer);
		} 
		else if ((TD.Camera>=0x1d)&&(TD.Camera<=0x4d))		// �޸�����ͷID    0x1d ��ӦIDΪ1�ţ�0x2dΪ2��...
		{
			i = ((TD.Camera - 0x0d) >> 4) & 0x0f;		// Ҫ�޸ĳɵ�����ͷID��
			SetCommand(0xff,eCAM_CHANGE_ID,2,i,CamBuffer);
			Length = SetCommand(i,eCAM_TEST,2,0,CamBuffer);
			if( (Length!=0xffff))
			{
				Uart.puts("�ֶ��޸�����ͷ�ɹ�,��ID: ");
				Int2Str(i, (char *)Tp.Buffer);
				Uart.puts((char *)Tp.Buffer);
				Uart.puts("\r\n");
			}
			
			
		}
		else 
		{				
			if((TD.Camera>100)&&(TD.Camera<150))
				Pic.SendNumber = TD.Camera-100;
			if((TD.Camera>200)&&(TD.Camera<250))
				Pic.TakeNumber = TD.Camera-200;
			Length = 1000;
		}
		if(Length<800)
		{
			Tp.B32 = Length;		
			Uart.putb(1,4,Tp.Buffer);
			Uart.putb(1,Length,CamBuffer);			
		}

		TD.Camera = 0;
		Uart2.Init(Baudrate.U2_232,U2_FOR_DISPLAY);
		os_dly_wait(10);
		CAMERA_POWER_OFF;

		
	}		
	#endif
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
*GY,000,S39,131400,3,0,0,3#
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������



extern		uchar			jt_auth_ok_flag;
void			CameraCls::JT_PictureDataToGPRS(uint16 Length,uint16 TotalPack,uint16 PackNo,uchar *pPic)
{
	#if (USE_CARMERA_MODEL==1)
	uint32	i;
	uchar 	s;
	union
	{
		uint32	B32;
		struct
		{
			uint16	Total;
			uint16	No;
		}TN;
	}Pack;
	uchar		tBuffer[32]={0};
	//JT808_MULTY_MEDIA_REPORT  MultiMediaReport;

	NOP();	NOP();

	
	for(i=0;i<(30*100UL);i++)		// �����ʱ20��
	{
		if( (SendPicData)||(jt_auth_ok_flag==0) )
			os_dly_wait(1);
		else
			break;		
	}
	
	if(i>=(30*100UL))		
	{
		Uart.puts("ͼƬ���ͳ�ʱ");		
	}
	
	SendPicData = TRUE;  
	PicDataUpLoadFlag = TRUE;

	if(pPic!=NULL)		memmove(Cam_cBuffer,pPic,Length);
	
	Pack.TN.Total = TotalPack;
	Pack.TN.No = PackNo+1;			// ��1��ʼ
	GPRS.JT808_DataPackPushAlignment(0,eBB_gCMD_TER_MEDIA_DATA_UPLOAD, Length, Pack.B32, Cam_cBuffer);	 
	
	strcpy( (char*)tBuffer,"�ϱ�ͼƬ����: ");
	Int2Str( Pack.TN.No, (char*)&tBuffer[14]);
	strcat( (char*)tBuffer,",");
	s = strlen( (char*)tBuffer);
	Int2Str( Pack.TN.Total, (char*)&tBuffer[s]);
	Uart.puts((char*)tBuffer);
	if(Pack.TN.No == Pack.TN.Total)
	{
		os_dly_wait(100);
		PicDataUpLoadFlag = 0;
		Uart.puts(" ����ͼƬ�����ϴ���� ");
	}
	if(TD.Camera == tCAMERA_ALL_DATA)
	{
		os_dly_wait(50);
		Uart.puts("Pic Test Data:");
		Uart.putb(1,Length,Cam_cBuffer);
		Uart.puts("\r\n");
	}

	#endif

}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
*GY,000,S39,131400,3,0,0,3#
*/ 	

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			CameraCls::PictureDataToGPRS(uchar PicNo,uchar TotalPack,uchar PackNo,uchar *pPic)
{

}








/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������
   ��ڲ�����
   ���ڲ��������ز���Ϊ0����OK������0xffff�����������Ϊ���ȵȲ���������������pDataָ����
   ȫ�ֱ���: 
   ����ʱ�䣺
*/ 	
extern	"C"		unsigned short 		CalCRC16( unsigned char * ptr, unsigned short len);

// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
uint16			CameraCls::SetCommand(uchar ID,uchar Cmd,uint16 Length,uint32 Data,uchar *pData)
{
	#if (USE_CARMERA_MODEL==1)
	uint32	i;
	uint16	Crc,CrcR;
	uchar		*pFrame,HeadFlag[4];
	union
	{
		uchar		Buffer[4];
		uint16	B16[2];
		uint32	B32;
	}Tp;
	NOP();	NOP();
	//if( ((ID>8)&&(ID!=0xff))||(ID==0))		// 0x00 �㲥��ַ��0xff����Ĭ�ϵ�ַ  ֧�ְ�·����ͷ
//	if( ((ID>4)&&(ID!=0xff))||(ID==0))		// 0x00 �㲥��ַ��0xff����Ĭ�ϵ�ַ	  ֧����·����ͷ
//		ID = 1;

	if( ((ID>4)&&(ID!=0xff)))		// 0x00 �㲥��ַ��0xff����Ĭ�ϵ�ַ	  ֧����·����ͷ
		ID = 1;
	
	i = 0;
	Cam_cBuffer[i++] = 0x90;
	Cam_cBuffer[i++] = 0xeb;
	Cam_cBuffer[i++] = ID;
	Cam_cBuffer[i++] = Cmd;
	Cam_cBuffer[i++] = (uchar)Length;
	Cam_cBuffer[i++] = Length>>8;
	
	Tp.B32 = Data;
	
	switch(Cmd)
	{		
		///////////////////////////////////////0x01��������ͷ
		case eCAM_TEST:
			Cam_cBuffer[i++] = 0x55;
			Cam_cBuffer[i++] = 0xaa;
		break;

		///////////////////////////////////////0x0d�޸�����ͷID
		case eCAM_CHANGE_ID:
			Cam_cBuffer[i++] = Tp.Buffer[0];
			Cam_cBuffer[i++] = 0;
		
		///////////////////////////////////////0x30�������գ�����ͼƬ��С
		case eCAM_TAKE_PHOTO_RET_SIZE:
			// ��������������
			Cam_cBuffer[i++] = 0x03;  //add
			Cam_cBuffer[i++] = 0x01;
		break;
		
		///////////////////////////////////////0x31������ָ������ͼ������
		case eCAM_REQ_DATA_PACK:
			Cam_cBuffer[i++] = Tp.Buffer[0];
			Cam_cBuffer[i++] = Tp.Buffer[1];
		break;
		
		///////////////////////////////////////0x32���÷ְ�����İ���С
		case eCAM_SET_PACK_SIZE:
			Cam_cBuffer[i++] = Tp.Buffer[0];
			Cam_cBuffer[i++] = Tp.Buffer[1];
		break;
		
		///////////////////////////////////////0x33�����ϴ��������������ͼƬ���ݴ�С
		case eCAM_REQ_PRE_PIC_SIZE:
			// ��������������
		break;
		
		///////////////////////////////////////0x34����Ĭ�ϵ�ͼ��ֱ���
		case eCAM_SET_RESOLUTION:
			Cam_cBuffer[i++] = Tp.Buffer[0];
			Cam_cBuffer[i++] = 0x00;

		break;
		
		///////////////////////////////////////
		case 0xf0:
		
		break;
		
		///////////////////////////////////////
		default: 	break;
	}

	
	Crc = CalCRC16(&Cam_cBuffer[2],(Length+4) );
	Cam_cBuffer[i++] = (uchar)Crc;
	Cam_cBuffer[i++] = Crc>>8;

	Uart2.ClearRecFlag();	
	Uart2.putb(1,i,Cam_cBuffer);	

	//Uart.puts("Send to Camera Test Data:"); Uart.putb(1,i,Cam_cBuffer);Uart.puts("\r\n");

	//os_dly_wait(1);
	Uart2.SendTask();	
	os_dly_wait(1);
	////////////////////////////////////////// 
	SetBufferValue(Cam_cBuffer,(512+128),0);
	Length = Uart2.WaitGetRecBuffer(Cam_cBuffer);	 	
	if ((Length>0)&&(Cmd != eCAM_REQ_DATA_PACK) )
	{	
		//Uart.puts("camera Test Data:"); Uart.putb(1, Length,Cam_cBuffer); Uart.puts("\r\n");	
		//HeadFlag[0] = 0x90;	HeadFlag[1] = 0xeb;	HeadFlag[2] = ID; HeadFlag[3] = 0;
		HeadFlag[0] = 0x90;	HeadFlag[1] = 0xeb;	HeadFlag[2] = ID;
		//Uart.puts("CAM ID Test Data:");Uart.putb(1,3,HeadFlag);Uart.puts("\r\n"); 
		pFrame = (uchar*)SearchArray(Length,(uchar*)Cam_cBuffer,3,(uchar*)HeadFlag);
		/*if(pFrame != NULL)
		{			
			Length = *(pFrame+5);
			Length <<= 8;
			Length |= *(pFrame+4);
			CrcR = *(pFrame+7+Length);
			CrcR <<= 8;
			CrcR |= *(pFrame+6+Length);
			Crc = CalCRC16((pFrame+2),(Length+4));
			if( (Crc == CrcR)||(Cmd==eCAM_REQ_DATA_PACK))	// �ְ����䲻����CRC	 
			{
				memmove(pData,pFrame+6,Length);		// �������������ݶΣ���״̬��
				if(pData != NULL)
				{
					return Length;
				}
			}
			else
			{	
				if(TD.Camera==tCAMERA_CMD_ERROR)
				{	Uart.puts("Rec CRC Error\r\n\0");	 	os_dly_wait(10);	}				
			}
		}
		else
		{	
			if(TD.Camera==tCAMERA_CMD_ERROR)
			{	Uart.puts("FrameHead Error\r\n\0");	 	os_dly_wait(10);	}
		} */
		//   // ������ͷ����У��		 2014.5.17
		if(pFrame != NULL)
		{			
			Length = *(pFrame+5);
			Length <<= 8;
			Length |= *(pFrame+4);
			
			if(pData != NULL)
			{
				memmove(pData,pFrame+6,Length);		// �������������ݶΣ���״̬��
				return Length;
			}
		}
		else
		{	
			if(TD.Camera==tCAMERA_CMD_ERROR)
			{	Uart.puts("FrameHead Error\r\n\0");	 	os_dly_wait(10);	}
		}
	}	
	//
	else if (Cmd == eCAM_REQ_DATA_PACK)
	{
	 	HeadFlag[0] = 0xeb;	HeadFlag[1] = ID;	HeadFlag[2] = 0x31; 
		pFrame = (uchar*)SearchArray(Length,(uchar*)Cam_cBuffer,3,(uchar*)HeadFlag);
		if(pFrame != NULL)
		{			
			Length = *(pFrame+4);
			Length <<= 8;
			Length |= *(pFrame+3);
			
			if(pData != NULL)
			{
				memmove(pData,pFrame+6,Length);		// �������������ݶΣ���״̬��
				return Length;
			}
		}
	}
   //
	else 
	{	
		if(TD.Camera==tCAMERA_CMD_ERROR)
		{	Uart.puts("Camera no data\r\n\0");	 	os_dly_wait(10);	}
	}

	return 0xffff;
	#else
	return 0xffff;
	#endif
}





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void		CameraCls::SetPower(uchar s)
{
	#if (USE_CARMERA_MODEL==1)
	if(s==1)
	{
		CameraPowerOn = TRUE;
		CAMERA_POWER_ON;
	}
	else if(s==2)
	{
		CameraPowerOn = TRUE;
		ForcePowerOn = TRUE;
		CAMERA_POWER_ON;
	}
	else
	{
		CameraPowerOn = FALSE;
		CAMERA_POWER_OFF;
	}

	#endif
}





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void		CameraCls::SetTakeNumber(uchar Cnt,uchar Head)
{
	#if (USE_CARMERA_MODEL==1)
	uint32	i,Addr;
	Pic.TakeNumber = Cnt;
	Pic.AddPackHead = Head;
	Pic.TakeStart	= TRUE;
	
	JT808Flag.VS6.Bits.CameraError = 0;			// ��������ͷ�õ�
	VehicleStatus.VS5.Bits.CameraOK = 1; 
	
	U2PinSelDevice = U2_FOR_CAMERA;
	os_dly_wait(100);
	Uart2.Init( Baudrate.U2_485,U2_FOR_CAMERA);
	
	#if(TEST_CODE_JUST_FOR_TEST_LUOYANG_NEW==1)
	Pic.TotalNumber 	= 0;
	FlashPageCnt.PicSave = FlashPageCnt.PicRead = 0;
	Uart.puts(" ����ǰ�����Ƭ���� ");
	Addr = FF_PIC_START_PAGE;
	for(i=0;i<16;i++)
	{				
		DFF.SectorEraseWaitOK(Addr/FF_PAGE_PER_SECTOR,10);
		Addr += FF_PAGE_PER_SECTOR;
	}
	#else
	NOP();	NOP();//	lf
	#endif

	#endif
	
}












/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			CameraCls::RestoreFactoryParameter(void)
{
	#if (USE_CARMERA_MODEL==1)
	uint32	i;
	union
	{
		uchar		Buffer[32];
		uint16	B16[16];
		uint32	B32[8];
	}Tp;
	Pic.TotalNumber 	= 0;
	Pic.Resolution		= 3;
	Pic.DuiBiDu			= 0x80;
	Pic.Lingth			= 0x80;
	Pic.IR				= 0xcc;
	Pic.TakeNumber		= 0;
	Pic.SendNumber		= 0;
	Pic.SendPicNo		= 0;
	Pic.SendPackNo		= 0;	
	Pic.SaveAndSend	= 0;
	Pic.TakeStart		= 0;
	Pic.AddPackHead	= 0;		
	DFE.Write(EE_PICTURE_TOTAL_NUM,8,(uchar*)&Pic);
	FlashPageCnt.PicSave = FlashPageCnt.PicRead = 0;
	
	#endif		
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			CameraCls::SetOperateParameter(uchar s,PIC_STRUCT *pPic)
{	
	#if (USE_CARMERA_MODEL==1)
	Pic.TotalNumber 	= pPic->TotalNumber  ;
	Pic.Resolution		= pPic->Resolution	;
	Pic.DuiBiDu			= pPic->DuiBiDu		;
	Pic.Lingth			= pPic->Lingth	      ;
	Pic.IR				= pPic->IR		      ;
	Pic.DelPicNo		= pPic->DelPicNo		;
	Pic.TakeNumber		= pPic->TakeNumber	;	
	Pic.SendNumber		= pPic->SendNumber	;	
	Pic.SendPicNo		= pPic->SendPicNo	   ;	
	Pic.SendPackNo		= pPic->SendPackNo	;			
	Pic.SaveAndSend	= pPic->SaveAndSend  ;
	Pic.TakeStart		= pPic->TakeStart		;
	Pic.AddPackHead	= pPic->AddPackHead	;
	if(Pic.Resolution>7)	Pic.Resolution = 5;		// 
	if(s)		DFE.Write(EE_PICTURE_TOTAL_NUM,8,(uchar*)&Pic);	
	
	#endif	
		
}

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			CameraCls::GetOperateParameter(uchar s,PIC_STRUCT *pPic)
{	
	#if (USE_CARMERA_MODEL==1)
	if(s)		DFE.Read(EE_PICTURE_TOTAL_NUM,8,(uchar*)&Pic);
	if(Pic.Resolution>7)		Pic.Resolution = 5;		//
  	if(Pic.Resolution==0)	Pic.Resolution = 3;		//
	
	if(pPic == NULL)	return;

	pPic->TotalNumber  = Pic.TotalNumber ;
	pPic->Resolution	 = Pic.Resolution	 ;
	pPic->DuiBiDu	    = Pic.DuiBiDu		 ;
	pPic->Lingth	    = Pic.Lingth	    ;
	pPic->IR		       = Pic.IR		    ;
	pPic->DelPicNo		 = Pic.DelPicNo	 ;
  	pPic->TakeNumber	 = Pic.TakeNumber	 ;
   pPic->SendNumber	 = Pic.SendNumber	 ;
   pPic->SendPicNo	 = Pic.SendPicNo	 ;
   pPic->SendPackNo	 = Pic.SendPackNo	 ;   
   pPic->SaveAndSend  = Pic.SaveAndSend ;
	pPic->TakeStart	 = Pic.TakeStart	 ;
	pPic->AddPackHead	 = Pic.AddPackHead;
	pPic->CameraID		 = Pic.CameraID;
	
	#endif

	
}



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	�������ƣ�
   ����������
   ��ڲ�����
   ���ڲ�����
   ȫ�ֱ���: 
   ����ʱ�䣺
*/ 	
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ ����ԭ�������ĺ���������������
void			CameraCls::Init(void)
{

	uchar		i,Cnt;
	uint32	Length;
	char		tBuffer[32];
	uchar		ID;
	NOP();	NOP();	
	CamBuffer = pShareMem;
   Cam_cBuffer = pShareMem + 1024;


	#if (USE_CARMERA_MODEL==1)

	CAMERA_POWER_ON;
	os_dly_wait(500);
	CAMERA_POWER_ON;
	Pic.CameraID = 0;

  if((USBTaskCreat != 0)||(PrintTaskCreat != 0))  { os_dly_wait(1); }
  SetBufferValue(CamBuffer, 800, 0);

	#if(SYSTEM_BOOT_MESSAGE)
	Uart.puts("Uart2 init......\r\n\0");		Uart.PopAlignmentSend();	RESET_WATCH_DOG;		os_dly_wait(10);
	#endif
	Uart2.Init(Baudrate.U2_485,U2_FOR_CAMERA);
	
	NCameraID[0] =  NCameraID[1] = NCameraID[2] = NCameraID[3] = 0;
	//NCameraID[4] =  NCameraID[5] = NCameraID[6] = NCameraID[7] = 0;
	#if (JT_TEST_FREIGHT==1)
	JT808Flag.VS6.Bits.CameraError = 0;		 // ����ƽ̨��������ͷ�����ֳ�Ҫ�󣬲�����
	#else
	if ((!(JT808_Alarm_Word.Shield & (0x01 << 11))))
		JT808Flag.VS6.Bits.CameraError = 1;
	#endif
	ID = 1;
	//for(i=0;i<8;i++)
	for(i=0;i<4;i++)
	{
		SetBufferValue((uchar*)tBuffer,32,0);
		strcpy(tBuffer,"Camera   ");
		tBuffer[8] = ID+'0';
		Length = SetCommand(ID,eCAM_TEST,2,0,CamBuffer);
		if(Length!=0xffff)
		{
			NCameraID[i] = ID;					
			strcat(tBuffer," OK\r\n\0");
		}
		else
		{
			
			strcat(tBuffer," ER\r\n\0");
		}
		ID++;		
		//if(TD.Camera==tCAMERA_CMD_ERROR)
		{		Uart.puts(tBuffer);		os_dly_wait(5);	}

	}

	/// test  �ֶ��޸�����ͷID
//	if (TD.Camera==tCAMERA_CMD_ERROR)
//	{
//		i = 1;
//		Length = SetCommand(0xff,eCAM_CHANGE_ID,1,i,CamBuffer);
//		if(Length!=0xffff)
//		{
//		 	Length = SetCommand(i,eCAM_TEST,2,0,CamBuffer);
//		 	if( (Length!=0xffff))
//		 	{
//				
//		 		Uart.puts("�޸�����ͷ�ɹ�");
//		 	}
//		}
//	}
	//

	////////////////////////////////////////////////////////
	for(Cnt=0;Cnt<8;Cnt++)		 // ��������0x00��0xff��ַ 	  ��·����ͷ
	{
		if(Cnt&0x01)	ID = 0xff;
		else				ID = 0;
		Length = SetCommand(ID,eCAM_TEST,2,0,CamBuffer);
		if(Length!=0xffff)
		{
			//for(i=0;i<8;i++)
			for(i=0;i<4;i++)
			{
				if(NCameraID[i]==0)
				{
					//NCameraID[i] = i+1;
					////////////////////////////// �޸�ID
					Length = SetCommand(ID,eCAM_CHANGE_ID,2,i+1,CamBuffer);
					if(Length!=0xffff)
					{
					 	Length = SetCommand(i+1,eCAM_TEST,2,0,CamBuffer);
					 	if( (Length!=0xffff)&&(TD.Camera==tCAMERA_CMD_ERROR) )
					 	{
							NCameraID[i] = i+1;
					 		strcpy(tBuffer,"Modify camera ID :   \r\n\0");
							tBuffer[20] = NCameraID[i]+'0';
					 		Uart.puts(tBuffer);
					 	}
					}
					break;
				}
			}
		}
	}
	VehicleStatus.VS5.Bits.CameraOK = 0;

	SetBufferValue((uchar*)tBuffer,32,0);
	strcpy(tBuffer,"Camera ID list  ");
	//for(i=0;i<8;i++)
	Cnt = 0;
	for(i=0;i<4;i++)
	{
		if(NCameraID[i])	
		{	
			Pic.CameraID = NCameraID[i];
			tBuffer[i+16] = NCameraID[i]+'0';
			VehicleStatus.VS5.Bits.CameraOK = 1;
			JT808Flag.VS6.Bits.CameraError = 0;	
			Cnt++;
		}
		else
		{					
			tBuffer[i+16] = 'X';
			NCameraID[i] = 0;
		}
	}
	if (Cnt == 0)					  // �Ҳ�������ͷ
	{
		SelfTestResust |= eST_ER_CAMERA;
		if ((!(JT808_Alarm_Word.Shield & (0x01 << 11))))
		{
			
			if (JT808_Alarm_Word.SendTextSMS & (0x01 << 11))		     								// ���Ͷ���
		  	{
			 	GSM.SetSMS_Buffer((char *)&JT808_Number.TextSMS[0],"Camera	Error Alarm!!!");		
		  	}
			JT808_GPRS_TempPack = TRUE;
			Uart.puts("TP. Camera Init,No camera");
		}
	}
	if( VehicleStatus.VS5.Bits.CameraOK == 0)
		SelfTestResust |= eST_ER_CAMERA;
	else
		SelfTestResust &= ~eST_ER_CAMERA;

	strcat(tBuffer,"\r\n\0");
	Uart.puts(tBuffer);
	
	GetOperateParameter(1,NULL);
	
	//Uart.putb(1,4,(uchar*)&Pic.TotalNumber);
	//Uart.puts("\r\n\0");
	CAMERA_POWER_OFF;

	Uart2.Init(Baudrate.U2_232,U2_FOR_DISPLAY);

	#else
   Uart2.Init(Baudrate.U2_232,U2_FOR_DISPLAY);
	#endif
}	


uchar	CameraCls::DeletePhotoDataFromDF(uchar PicNo)
{
	

	return OK;


}


void			CameraCls::CheckCamera(void)
{
	#if (USE_CARMERA_MODEL==1)
	uchar		i,Cnt, ID, CameraOK;
	char		tBuffer[32];
	uint32	Length;
	

	CAMERA_POWER_ON;
	os_dly_wait(500);
	CAMERA_POWER_ON;

	Cnt = 0;

  	SetBufferValue((uchar *)tBuffer, 32, 0);

   Uart2.Init(Baudrate.U2_485,U2_FOR_CAMERA);
	
	
	for(i=0;i<4;i++)		// ֻ֧����·����ͷ
	{
		Length = SetCommand(i+1,eCAM_TEST,2,0,(uchar *)tBuffer);
		if(Length!=0xffff)
		{
			if (NCameraID[i] == 0)  NCameraID[i] = i + 1;

			Cnt++;										
		}
		else
		{
			NCameraID[i] = 0;
		}		
	}


	if (Cnt == 0)       // �Ҳ�����������ͷ
	{
		CameraOK = 0;
		for(Cnt=0;Cnt<8;Cnt++)		 // ��������0x00��0xff��ַ 	�����޸ĸճ�������ͷ��	ID Ϊ0x00��0xff
		{
			if(Cnt&0x01)	ID = 0xff;
			else				ID = 0;
			Length = SetCommand(ID,eCAM_TEST,2,0,(uchar *)tBuffer);
			if(Length!=0xffff)
			{
				//for(i=0;i<8;i++)
				for(i=0;i<4;i++)
				{
					if(NCameraID[i]==0)
					{
						////////////////////////////// �޸�ID
						Length = SetCommand(ID,eCAM_CHANGE_ID,2,i+1,CamBuffer);
						if(Length!=0xffff)
						{
						 	Length = SetCommand(i+1,eCAM_TEST,2,0,CamBuffer);
						 	if( (Length!=0xffff)&&(TD.Camera==tCAMERA_CMD_ERROR) )
						 	{
								CameraOK++;
							    NCameraID[i] = i+1;
						 		strcpy(tBuffer,"Modify camera ID :   \r\n\0");
								tBuffer[20] = NCameraID[i]+'0';
						 		Uart.puts(tBuffer);
						 	}
						}
						break;
					}
				}
			}
		}

		if ((CameraOK == 0) && (!(JT808_Alarm_Word.Shield & (0x01 << 11))))
		{
			if (JT808Flag.VS6.Bits.CameraError == 0)
			{
				JT808_GPRS_TempPack = TRUE;
				Uart.puts("TP. Camera ER");
			}
		   
			JT808Flag.VS6.Bits.CameraError = 1;
			VehicleStatus.VS5.Bits.CameraOK = 0;
			SelfTestResust &= ~eST_ER_CAMERA;
			if (JT808_Alarm_Word.SendTextSMS & (0x01 << 11))		     								// ���Ͷ���
		  	{
			 	GSM.SetSMS_Buffer((char *)&JT808_Number.TextSMS[0],"Camera	Error Alarm!!!");		
		  	}
						
		}
		else
		{
			goto CameraIsOK;
		}
		
	} 
	else
	{
CameraIsOK:
		if (JT808Flag.VS6.Bits.CameraError == 1)
		{
			JT808_GPRS_TempPack = TRUE;
			Uart.puts("TP. Camera ER to OK");
		}

		JT808Flag.VS6.Bits.CameraError = 0;
		VehicleStatus.VS5.Bits.CameraOK = 1;
		SelfTestResust &= ~eST_ER_CAMERA;
	}

	CAMERA_POWER_OFF;
	Uart2.Init(Baudrate.U2_232,U2_FOR_DISPLAY);

  #endif

}


CameraCls::CameraCls()
{
  
}

/****************************** end of file ****************************************/











