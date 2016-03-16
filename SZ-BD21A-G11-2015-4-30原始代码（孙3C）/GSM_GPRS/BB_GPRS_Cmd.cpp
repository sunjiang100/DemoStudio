/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 		  
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 	 GPRS.c
	功能描述：GRPS相关函数
	编译环境：
	目标CPU: 
	作者：
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					

// 注：// 国业内部运算的经纬度比JT808大10倍，在设置经纬度的时候，要乘以10

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

uchar		 	jt_recv_flag = 0;	 	  // bit0: 	为1, 收到注册应答成功   bit1:	 为1， 鉴权成功    bit2: 为1，收到心跳应答  bit3: 为1，收到位置上报应答
uchar			jt_auth_ok_flag = 0;
uchar			ter_or_veh_reg_result;	 // 终端注册应答结果


uchar 		jt_save_pos_flag;                 // 保存位置信息标志		

uint16		jt_send_cmd_flag;						// 已发送命令标志	 
uint32		jt_media_id;                     // 多媒体ID
uchar			jt_pic_send_flag;                // 图片发送使能标志 
uchar			jt_dis_reg_flag;						// 终端注销标志
uchar			PicRetryOrMulPic;					   // 发送图片重传包： 1  ; 发送存储多媒体（多张）: 2
uchar 		JTDelPicFlag = 0;                    // 删除图片标志



#define		TOTAL_EVT_INFO_ID_NUMBER		0x64
uchar		JT808_EventID[TOTAL_EVT_INFO_ID_NUMBER]={0};			// 最前面的单元存储当前个数
uchar		JT808_InfoID[TOTAL_EVT_INFO_ID_NUMBER]={0};




static		PIC_STRUCT		cPic;
uchar			JT_ManulCameraID = 0;				// 即时拍摄通道ID


char		jt_auth_code[LEN_AUTH_CODE];		// 鉴权码

extern  	uchar			*Cam_cBuffer;
extern  	uchar			*CamBuffer;
extern  	uchar        CameraUsing,OnlyUseBD_GPS;
extern	uchar 		Connect_Create_USer_Center_Or_Main_Center(uchar Type, uchar Wait);

extern	uchar			IC_CardAuthFailure;									  

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称： get_position_info
   功能描述： 获取位置消息体
   入口参数： pos_data: 数据指针      AckLen: 0, 没有应答项长度  1，应答项长度
   出口参数： pos_data: 位置数据
   全局变量: 
   创建时间：
   修改时间：
注：

设置默认时间为20秒
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

	// 位置基本信息
	
	if(Last>=0x80)
	{
		Len = 0;				// 只取附件信息
	}
	else
	{
		Len = LEN_JT808_GPS_PACK;
		if(Last)		//  最后有效定位信息
	  		memmove(pos_data, (uchar*)&GPS.LastJTPack, LEN_JT808_GPS_PACK);	
		else			// 当前信息，不管是否定位
	  		memmove(pos_data, (uchar*)&GPS.JTPack, LEN_JT808_GPS_PACK);  
	
				////// 更新报警位和状态位
		Tp.B32 = ~JT808_Alarm_Word.Shield;		// 报警屏蔽字
		
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

	/////////////////////////////////////////////////////////////////////////////// 附加信息
	if(UseGPS_Mil==0)
		Tp.B32 = SystemLog.GPS.TotalMileage/100;		// 内部处理单位：米，上传单位0.1km
	else
		Tp.B32 = SystemLog.VDR.SpeedSensorPulsCnt/VehiclePulseKV;		// GPS计算的里程单位是米
	
	pos_data[Len++] = eBB_POS_ADD_MIL;
	pos_data[Len++] = 0x04;
	pos_data[Len++] = Tp.Buffer[3];
	pos_data[Len++] = Tp.Buffer[2];
	pos_data[Len++] = Tp.Buffer[1];
	pos_data[Len++] = Tp.Buffer[0];

	// add 2013.3  	
	pos_data[Len++] = eBB_POS_ADD_OIL;		   // 油量， 暂时没有实现此功能，置0
	pos_data[Len++] = 0x02;
	pos_data[Len++] = 1;	   						// 油量高字节
	pos_data[Len++] = 0xc4;	   						// 油量低字节  

	//	
	pos_data[Len++] = eBB_POS_ADD_SPEED;		// 记录仪上的速度
	pos_data[Len++] = 0x02;
	Tp.B16[0] = uiSpeed;
	pos_data[Len++] = Tp.Buffer[1];	               
	pos_data[Len++] = Tp.Buffer[0]; 
	
	if( (InOrOutArea==eBB_IN_AREA)||(InOrOutArea==eBB_OUT_AREA) )
	{
		if(JT808Flag.VS7.Bits.InOutArea==1)		// 只有矩形区域需要人工确认
		{
			pos_data[Len++] = eBB_POS_ADD_MANUL_ALARM_EVENT_ID;  // 需要人工确认报警事件的ID
			pos_data[Len++] = 0x02;
			if(InOrOutArea==eBB_IN_AREA)			ManulAlarmEventID = 1;
			else											ManulAlarmEventID = 2;
			Tp.B16[0] = ManulAlarmEventID;
			pos_data[Len++] = Tp.Buffer[1];
			pos_data[Len++] = Tp.Buffer[0];
		}
	}
		

	//////////////////////////////////// 围栏报警信息
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
		pos_data[Len++] = InOrOutArea;                // 方向
	}

	// added 2013.3		 // 添加信息 增加了22个字节
	pos_data[Len++] =  eBB_POS_ADD_EXTEN_VS_STATUS;		   // 扩展车辆信号
	pos_data[Len++] = 0x04;
	pos_data[Len++]	= JT808ExtenVsFlag.VS4.Byte;
	pos_data[Len++]	= JT808ExtenVsFlag.VS3.Byte;
	pos_data[Len++]	= JT808ExtenVsFlag.VS2.Byte;
	pos_data[Len++]	= JT808ExtenVsFlag.VS1.Byte;

	pos_data[Len++] =  eBB_POS_ADD_IO_STATUS;				// IO状态位
	pos_data[Len++] = 0x02;
	pos_data[Len++] = JT808IOStatusFlag.VS2.Byte;
	pos_data[Len++] = JT808IOStatusFlag.VS1.Byte;

	pos_data[Len++] =  eBB_POS_ADD_ANALOG;				 // 两路模拟量
	pos_data[Len++] = 0x04;	
   pos_data[Len++] = JT808Analog1 >> 8;            // 模拟量3
	pos_data[Len++] = JT808Analog1 & 0xff;
   pos_data[Len++] = JT808Analog2 >> 8;            // 模拟量2	   这里就定为主电源的模拟量
	pos_data[Len++] = JT808Analog2 & 0xff;

	
	pos_data[Len++] = eBB_POS_ADD_GSM_SIGNAL;		   // 无线通信的信号强度
	pos_data[Len++] = 0x01;
    pos_data[Len++] = GG_Status.Signal;        
	
	pos_data[Len++] = eBB_POS_ADD_GNSS_SATELLITE_NUMS;	   // GNSS定位卫星数
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
	函数名称： save_or_send_position_info
   功能描述： 保存或发送链路有问题时的位置消息与应答超时的含有报警信息的位置消息
   入口参数： type: 0,  链路有问题      1， 	含有报警信息  Isave: 1, 保存  0, 发送
   出口参数： 空
   全局变量: 
   创建时间：
   修改时间：
注：1页Flash只存储1条数据

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

	// 市县域
	
	DFE.Read(EE_PREFECTURE_REGION_ID, 2, Tp.Buffer);
	*(pData+Len) = Tp.Buffer[1];		Len++;
	*(pData+Len) = Tp.Buffer[0];		Len++;

	// 制造商 到 终端ID
	DFE.Read(EE_VENDOR_ID, 32, (pData+Len));	 
	#if (WUXI_TEST != 1)
	memmove( (pData+Len+25),(uchar*)&sOEM_ID[3],7);
	#endif

	Len += 32; 

	// 车的颜色
	DFE.Read(FM_CAR_COLOUR, 1, (pData+Len));
		 
	if (*(pData+Len) != 0)	   // 
	{    
		Len++;
		DFE.Read(FM_VEHICLE_CODE, 9, (pData+Len));	  // 车牌号
		Len += 9;
	}
	else		// 等于0，取VIN号
	{
		Len++;
		DFE.Read(FM_VEHICLE_VIN, LEN_VEHICLE_VIN, (pData+Len));  // 车辆VIN号	 
		Len += LEN_VEHICLE_VIN;
	}
			
	return Len;
}
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
注：

*/ 
//extern	char		SoftwareVersion[];
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
extern		uchar		OutOfRoadCnt;

void			GPRSCls::BBJT808_GPRS_UploadCmd(uint16 Cmd,uint16 SrcLen,uchar *pSrcData)
{		
	#define		LEN_TP_BUFFER			128
	union
	{
		 uint32     B32[24];
		 uint16     B16[48];
		 uchar		Buffer[LEN_TP_BUFFER];		 // 注意：tBuffer的空间要比 	LEN_POSITION_REPORT 大
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
			Uart.puts("GPRS上传打包函数忙，超时返回 ");
			return;
		}
	}
	UploadCmdBusing = TRUE;
	SetBufferValue(Tp.Buffer,128,0);
	
	////////////////////////////////////////////////////// 各命令数据处理
	switch(Cmd)
	{
		///////////////
		case	eBB_gCMD_TER_ACK:
		   Tp.Buffer[0] = AckBuffer.Index>>8;
			Tp.Buffer[1] = AckBuffer.Index;
			Tp.Buffer[2] = AckBuffer.MsgId>>8;
			Tp.Buffer[3] = AckBuffer.MsgId;
			Tp.Buffer[4] = ter_or_veh_reg_result;		   // 结果
			
			Len = 5;
		break;

		///////////////
		case	eBB_gCMD_TER_HEARTBEAT:							// 心跳
			Len = 0;
			Tp.Buffer[0]	= '\0'; 
			AckID = Cmd;
		break;

		///////////////
		case	eBB_gCMD_TER_REG:					// 终端注册		

			AckID = Cmd;
			// modified 2013.5
			SetBufferValue(Tp.Buffer,90,0);
			
			Len = GetRegInfo(Tp.Buffer);

			Uart.puts(" 终端注册：");
			Uart.putb(1,Len,Tp.Buffer);

			ReSend.OverTime = ReSend.Times = 0;	  // 应答超时间 10s
			ReSend.PreOverTime = SendOVT.TCP_ACK_OverTime; 

		break;

		///////////////
		case	eBB_gCMD_TER_DEREG:		      // 终端注消
			Len = 0;
			Tp.Buffer[0]	= '\0';
			AckID = Cmd;
			ReSend.OverTime = ReSend.Times = 0;	  // 应答超时间 10s
			ReSend.PreOverTime = SendOVT.TCP_ACK_OverTime; 
			Uart.puts("向平台注销\r\n");
		break;

		//////////////
		case	 eBB_gCMD_TER_AUTH:		  			// 发送鉴权码
			DFE.Read(EE_AUTH_CODE, LEN_AUTH_CODE, &Tp.Buffer[0]);			
			Len = strlen((char*)Tp.Buffer);
			Uart.puts((char*)Tp.Buffer);			// 打印到串口
			AckID = Cmd;
			ReSend.OverTime = ReSend.Times = 0;	  // 应答超时间 10s
			ReSend.PreOverTime = SendOVT.TCP_ACK_OverTime; 
	   break;

	  ///////////////
	   case	eBB_gCMD_TER_POS_REPORT:			// 位置上报
			
			//Len = get_position_info(0,&Tp.Buffer[0], 0);
			Len = get_position_info(1,&Tp.Buffer[0], 0);	 //第一个1表示last数据
			AckID = eBB_gCMD_TER_POS_REPORT;
	   break;

	  ///////////////
	   case	eBB_gCMD_TER_POS_QUERY_ACK:		// 位置查询应答
	  		Tp.Buffer[0] = AckBuffer.Index >> 8;
			Tp.Buffer[1] = AckBuffer.Index & 0xFF; 
			Len = get_position_info(0,&Tp.Buffer[2], 2);
	   break;

	   ///////////////
	  case	eBB_gCMD_TER_MEDIA_EVENT_UPLOAD:		  // 多媒体事件信息上传    

			NOP();	NOP();
			PDU_SMS.Ack = 0;          //大数据强制从GPRS通道传输	如果是SMS通道下达的采集指令，数据从GPRS返回
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
	  case	eBB_gCMD_TER_MEDIA_DATA_UPLOAD:		      // 多媒体数据上传
			
			PDU_SMS.Ack = 0;          //大数据强制从GPRS通道传输	如果是SMS通道下达的采集指令，数据从GPRS返回
			JtSendPic.Index = 0;
			jt_pic_send_flag = 1;
			//CameraUsing = 1;
			Protocol.GPRS[JT_PROTOCOL] = 1;  // 部标协议使能	
			UploadCmdBusing = 0;
			return;                    // 设置好上面参数， Camera任务自动发送设置的图片，不需执行下面的打包函数

	  ///////////////
	  case	eBB_gCMD_TER_MEDIA_SERCH_ACK:			// 多媒体数据检索应答
	  	
       	while(CameraUsing)		{	os_dly_wait(1);	}
       	CameraUsing = TRUE;
       	
  		   PDU_SMS.Ack = 0;          //大数据强制从GPRS通道传输	如果是SMS通道下达的采集指令，数据从GPRS返回
			SetBufferValue((uchar *)&CamBuffer[0], 1024, 0);
	
		  	CamBuffer[0] = AckBuffer.Index >> 8;		// 应答0x8802的流水号
			CamBuffer[1] = AckBuffer.Index & 0xFF;
		
		   Len = 4;
			k = 0;		        // 检索项数	pSAVE_PIC_HEAD

			
		  	if (MulMeDa.SavedMediaSearch.MediaType == 0)
			{								
				for(Addr=FF_PIC_START_PAGE;Addr<FF_PIC_END_PAGE;Addr++)
				{
					DFF.PageRead(Addr,Cam_cBuffer);
					pSaveHead = (pSAVE_PIC_HEAD)&Cam_cBuffer[0];
					if( pSaveHead->HeadFlag == SAVE_PIC_HEAD_FLAG )			// JPEG标志
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

			CamBuffer[2] = k >> 8;		   // 多媒体总项数
			CamBuffer[3] = k & 0xFF;	
		 	JT808_DataPackPushAlignment(0,Cmd, Len, 0, (uchar *)&CamBuffer[0]);
			SetBufferValue( Cam_cBuffer,64,0);
			strcpy((char*)Cam_cBuffer," 多媒体数据检索应答 ");
			Int2Str( k, (char*)&Cam_cBuffer[20]);
			strcat((char*)Cam_cBuffer,"条信息");
		   Uart.puts((char*)Cam_cBuffer);
			CameraUsing = 0;
			UploadCmdBusing = 0;
			return;

	  //break;	
	  ///////////////
	  case	eBB_gCMD_TER_DATA_TRAN_UPLOAD:			//   上行透传
	     
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
			  Uart.puts("上报--透传数据 ");
			  UploadCmdBusing = 0;
			  return;
			}


	  case   eBB_gCMD_TER_VDR_DATA_UPLOAD:			 ////////////取记录数据
			 
		  Len = 0;
		  if( pSrcData !=NULL)
		  {
	     		JT808_DataPackPushAlignment(0,Cmd, SrcLen, 0, pSrcData);
	     		UploadCmdBusing = 0;
			  	return;
			}
		
		break;
	
		case  eBB_gCMD_TER_DRIVER_INFO_REPORT:        // 驾驶员身份上传
		case	eBB_gCMD_I2C_DRIVER_INFO_REPORT:

		Len = 0;
		Tp.Buffer[Len++] = (IC_Card_Insert == TRUE)? IC_CARD_INSERT :	IC_CARD_IS_OUT	;
		DFE.Read(FM_RUN_TIME_START,6,&Tp.Buffer[Len]);
		Len +=6;
		if(IC_Card_Insert == TRUE)
		{
				Tp.Buffer[Len++] = IC_CardAuthFailure; 	// 0：成功，其它失败
		
				tmp = 0;
				DFE.Read(EE_JT_DRIVER_NAME_LEN, 1, (uchar*)&tmp);
				NOP(); 
				DFE.Read(EE_JT_DRIVER_NAME_LEN, 1+tmp, &Tp.Buffer[Len]);
				NOP(); 
				Len += tmp+1;
		
				DFE.Read(EE_JT_DRIVER_CARD_ID, 20, &Tp.Buffer[Len]);	//从业资格证号
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
		  }//IC卡插入有效

//		Tp.Buffer[Len++] = IC_CardAuthFailure; 	// 0：成功，其它失败
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
	case	eBB_gCMD_TER_QUERY_PRO_ACK:					  // 查询终端属性应答
		SetBufferValue(Tp.Buffer, 96, 0);
		pTerPro = (pQUERY_TER_PRO)Tp.Buffer;
		pTerPro->Type = TER_SUPPORT_CAR_TYPE;
		DFE.Read(EE_VENDOR_ID, 32, &pTerPro->ProductorID[0]);  // 制造商ID，终端型号，终端ID
		NOP();
		Len = 1 + 32;
		
		SetBufferValue((uchar *)&AckBuffer.Data[0], 32, 0);		
		DFE.Read(EE_SIM_CARD_ICCID, LEN_SIM_CARD_ICCID, (uchar *)&AckBuffer.Data[0]);	  // SIM卡串号	898600821912A0247283
		s = strlen((char *)&AckBuffer.Data[0]);
		if (s > 0)
		{
			for (i=0,j=0;i<20;i += 2,j++)
			{
				k = AckBuffer.Data[i];
				pTerPro->SimCardICCID[j] = (((k>=0x30)&&(k<=0x39))?(k-0x30):(k-0x37)) << 4;
				k = AckBuffer.Data[i+1];
				pTerPro->SimCardICCID[j] |=  (((k>=0x30)&&(k<=0x39))?(k-0x30):(k-0x37));	   // ICCID号是‘0’ - ‘9’， ‘A' -'Z';
			}
		}
		Len += 10;
			
		Sys.GetHS_Version((char *)&pTerPro->LenHardVer, 1);  
		Len += (2 + pTerPro->LenHardVer + pTerPro->LenFirmwareVer);
		
		pTerPro->GNSSPro = TER_GNSS_PRO;
		pTerPro->GSMPro  = TER_GSM_PRO;
		Len += 2;			
		
	break;
	
	case	eBB_gCMD_TER_TAKE_PHOTO_ACK:		 // 摄像头立即拍摄命令应答
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
	/*case	eBB_gCMD_TER_CAN_DATA_UPLOAD:	  						// CAN总线数据上传
			
		
	break; */

	case eBB_gCMD_TER_CREATE_USER:						  // 使用前锁定（快速开户）	
		
			
		if(pSrcData!=NULL)
		{
			AckID = Cmd;
			memmove( Tp.Buffer,pSrcData,SrcLen);
			Len = SrcLen;
						
			Uart.puts("开户  Test Data:");
			Uart.putb(1,Len,Tp.Buffer);
			Uart.puts("\r\n");
			
			ReSend.OverTime = ReSend.Times = 0;	  // 应答超时间 10s
			ReSend.PreOverTime = SendOVT.TCP_ACK_OverTime;
		}
			 
	break;
	//

	// 事件报告
	case eBB_gCMD_TER_EVENT_REPORT:
		AckID = Cmd;
		tmp = JT808_EventID[0];	  									// 当前信息总个数
		if(tmp)
			Tp.Buffer[0] = JT808_EventID[1];
		else
			Tp.Buffer[0]	= *pSrcData;							// 事件ID在入口参数		
		Len = 1;
		Uart.puts("JT808_EventID ");
		Uart.putb(1,(JT808_EventID[0]+1),JT808_EventID);  	// 
	break;

	// 信息点播放/取消
	case eBB_gCMD_TER_GET_INFO:
		AckID = Cmd;
		tmp = JT808_InfoID[0];						// 当前信息总条数
		if(tmp)		
			Tp.Buffer[0]	= JT808_InfoID[1];	// 检测使用，只取第1条信息
		else
			Tp.Buffer[0] = 0x01;
		Tp.Buffer[1]	= *pSrcData;  				// 点播或取消标识在入口参数
		Len = 2;
		//Uart.puts("JT808_InfoID ");
		//Uart.putb(1,(JT808_InfoID[0]+1),JT808_InfoID);
	break;

	// 电子运单
	case eBB_gCMD_TER_ELE_WAYBILL_UPLOAD:
		

	break;

	//added 2013.6
	case   eBB_gCMD_TER_POS_INFO_MULTI_UPLOAD:						// 定位信息批量上传	  / 或 盲区补报
		
		UploadCmdBusing = 0;
	return;
	//

	case	eBB_gCMD_TPMS_DATA_UPLOAD:							// 轮胎气压数据上报
		if( (pSrcData != NULL) )
     	{
     		JT808_DataPackPushAlignment(0,Cmd, SrcLen, 0, pSrcData);
			Uart.puts("\r\n轮胎数据上报\r\n");
     		UploadCmdBusing = 0;
		  	return;
		}
	
	break;

	default: break;
		
	}

	JT808_DataPackPushAlignment(AckID,Cmd, Len, 0, Tp.Buffer);
	if (TD.GPRS==tGPRS_TMP_TEST) { Uart.puts("通用打包上报，ID:");	}	  		// 打包进队。需判断鉴权成功后才出队。
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
  	GG_Status.Flag.Word = GG_FLAG_INIT_DATA;	// 注销后重新链接
  
  	NeedRegServer = ENABLE_FLAG;	
   DFE.Write(EE_NEED_REG_SERVER, 1, &NeedRegServer);	 
  	NOP();NOP();       

	SetBufferValue(tBuffer,LEN_AUTH_CODE,0);
	SetBufferValue(  (uchar*)jt_auth_code, LEN_AUTH_CODE,0);
   DFE.Write(EE_AUTH_CODE, LEN_AUTH_CODE,tBuffer);			//  清空鉴权码

		 uLED.MainMenuCnt = DEFAULT_DISP;
//  	uLED.MainMenuCnt = Logout_OK_MENU;
//  	sKEY_RM.LongTimeNoKey = 20*5;
}


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	函数名称：
   功能描述：处理GPRS下行数据	并应答
   入口参数：
   出口参数：
   全局变量: 
   创建时间：
   修改时间：
注：

*/ 
extern		uchar			NeedTakePhotoAck;
// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ 以上原形声明的函数供本函数调用
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
	DataLen  = Length;							   //消息体长度
	MsgId	= SwapINT16(pBB->Cmd);             // 获取消息ID
	MsgAttr  = SwapINT16(pBB->Attr.B16);		 // 获取消息体属性
	isPack = (MsgAttr >> 13) & 0x01;           //  获取封装标志 
	Index  = SwapINT16(pBB->Index);          // 获取消息流水号
	//Plat_Serial_No = Index;  // 特殊用途，与AckBuffer.Index不同
	//Index  = pBB->Index; 
	if (isPack)		           
	{
		DataBlockAddr = BB_CMD_ADDR_DATA_BLOCK;
	}
	else					// 数据没有分包
	{
		DataBlockAddr = BB_CMD_ADDR_DATA_BLOCK - LEN_BB_PACK_ITEM;
	}


	s = CalXorSum((Length + DataBlockAddr-1),(uchar*)&OneFrame[BB_CMD_ADDR_MSG_ID]);
	//tel = memcmp(&OneFrame[BB_CMD_ADDR_SIM], &sTelNum.SimBCD_JT[0], 6);	

	if((s!=OneFrame[Length + DataBlockAddr]))//   ||(tel != 0))		 // 校验码不正确， 或手机号不正确 
	{
		Uart.puts("收到中心命令，校验错误 ");
		return ERROR;  //return OK;          //s = ERROR;
	}

	if ( (jt_auth_ok_flag != TRUE)&&(Uart0CommandFlag==FALSE)) 	  // 未鉴权，只处理以下指令: 通用应答、注册应答、下行透传, 串口时，可以处理
	{
		if( (MsgId!=eBB_gCMD_PLAT_ACK)&&(MsgId!=eBB_gCMD_PLAT_TO_TER_REG_ACK)&&(MsgId!=eBB_gCMD_PLAT_SEND_TRAN_DATA) )
			return OK;
	}

		  
	////////////////////////////////////////////////////// 各命令数据处理
	switch(MsgId)
	{
			
		///////////平台应答
		case eBB_gCMD_PLAT_ACK:			//		  
			Index = OneFrame[DataBlockAddr];
			Index <<= 8;
			Index |= OneFrame[DataBlockAddr + 1];	 	// 取消息体中，终端发送的流水号

			MsgId = OneFrame[DataBlockAddr + 2];
			MsgId	<<= 8;
			MsgId |= OneFrame[DataBlockAddr + 3];		// 取消息体中，终端发送的消息ID

			result = OneFrame[DataBlockAddr + 4];		// 应答结果

			TC.GPRS_LongTimeNoAck = 0;
			
			///////////////////////////// 终端等待应答

			///////////////////////////////////////////////// 报警确认后，清除相关标志
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
			
			
			if (MsgId == eBB_gCMD_TER_CREATE_USER)				  // 使用前锁定功能	 
			{
				//Uart.puts("From jt1 Center' Data:");
				Uart.putb(1, Length, &OneFrame[DataBlockAddr]);  
				os_dly_wait(1);

				Creat_User_Reging = FALSE;

				if ((result == 0) )//&& (CreateUserFlag == NEED_CREATE_USER))
				{
					Uart.puts("开户成功!!!\r\n");	os_dly_wait(1);
					CreateUserFlag = OK;
					DFE.Write(EE_CREATE_USER_FLAG, 1, &CreateUserFlag);
					NOP(); NOP(); NOP();
					s = 0x00;		  // 成功
					
				}
				else
				{
					s = 0x01;		// 失败
					Uart.puts("开户失败!!!\r\n");	os_dly_wait(1);
				}

				// 发送信息到调度屏，回复注册状态
				KeyDisp.SendDataToLCM_YH(eLCM_D_CAR_REG_RESULT, 1, &s); 
				os_dly_wait(100);

				Connect_Create_USer_Center_Or_Main_Center(CONNECT_MAIN_CENTER, 0);			 // 不管是否成功，还连接回主中心
					
				
			} 	

			if ((result == 0) || (result == 4))		  	// 0：成功，4：报警处理
			{

				if (MsgId == eBB_gCMD_TER_AUTH)
				{
					jt_recv_flag |= 0x01 << eBB_gFLAG_AUTH_INDEX;		    // 
					jt_auth_ok_flag = 1;
					GG_Status.Flag.Bits.RegServer = 1;
					AuthServerOverTime = 0;
					GPRS.InitAlig('P');		// 清除优先出队的数据
					Uart.puts("鉴权成功！ ");
						 
					// 重置重发时间
					ReSend.OverTime = ReSend.Times = 0;	  // 应答超时间 10s
					ReSend.PreOverTime = SendOVT.TCP_ACK_OverTime;  				 						
				}
				
				else 	if (MsgId == eBB_gCMD_TER_HEARTBEAT)
				{
					 jt_recv_flag |= 0x01 << eBB_gFLAG_HEART_INDEX;		    // 

					 // 重置重发时间
					ReSend.OverTime = ReSend.Times = 0;	  // 应答超时间 10s
					ReSend.PreOverTime = SendOVT.TCP_ACK_OverTime;  
				}
				else if ((MsgId == eBB_gCMD_TER_POS_REPORT) || (MsgId == eBB_gCMD_TER_POS_INFO_MULTI_UPLOAD))
				{
					 jt_recv_flag |= 0x01 << eBB_gFLAG_POS_INDEX;		    //	收到应答，置标志

					 if (result == 4)	  //  报警确认
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
					  
					ReSend.OverTime = ReSend.Times = 0;  // 应答超时间 10s
					ReSend.PreOverTime = SendOVT.TCP_ACK_OverTime;  
					
				}
				else  if ((MsgId == eBB_gCMD_TER_DEREG) && (MsgId == AckBuffer.MsgId))	 // 终端注销
				{
					  
					  jt_recv_flag |= 0x01 << eBB_gFlag_DEREG_INDEX;
					  jt_auth_ok_flag = 0;
					  ReSend.OverTime = ReSend.Times = 0;	  // 应答超时间 10s
						ReSend.PreOverTime = SendOVT.TCP_ACK_OverTime;  
						
					  UnRegTerminal();
					  GPRS.InitAlig('P');		// 清除优先出队的数据
					  Uart.puts("平台注销成功\r\n");

				}
			    else //if (MsgId == eBB_gCMD_TER_MEDIA_EVENT_UPLOAD)
				{
					  jt_send_cmd_flag = 0;
					  jt_recv_flag |= 0x01 << eBB_gFLAG_OTHER_INDEX;

					  	ReSend.OverTime = ReSend.Times = 0;	  // 应答超时间 10s
						ReSend.PreOverTime = SendOVT.TCP_ACK_OverTime;  						
				}
			}
			else if (result== 1)
			{
				Uart.puts("收到应答，处理结果失败 \n");
				if ((MsgId == eBB_gCMD_TER_AUTH) && (AuthServerOverTime++ > 5))
				{
					GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DEREG,0,NULL);
					//UnRegTerminal();
				}
			}
			else if (result == 2)
			{
				Uart.puts("收到应答，消息错误!!!\n");
				if ((MsgId == eBB_gCMD_TER_AUTH)  && (AuthServerOverTime++ > 5))
				{
					GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DEREG,0,NULL);
					//UnRegTerminal();
				}
			}
			else if (result == 3)
			{
				Uart.puts("收到应答，不支持该指令!!!\n");
				if ((MsgId == eBB_gCMD_TER_AUTH) && (AuthServerOverTime++ > 5))
				{
					GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DEREG,0,NULL);
					//UnRegTerminal();
				}
			}
			else
			{
				Uart.puts("应答ID与原ID ");
				Uart.putb(1, 2 , (uchar *)&MsgId);
				os_dly_wait(5);
				Uart.putb(1, 2 , (uchar *)&AckBuffer.MsgId);
				os_dly_wait(5);
			}
			
			//GG_Status.Flag.Bits.RegServer = 1;
				
		break;
		
		////////////注册应答	
		case eBB_gCMD_PLAT_TO_TER_REG_ACK:	//,						  
			
			Index = OneFrame[DataBlockAddr+0];
			Index <<= 8;
			Index |= OneFrame[DataBlockAddr+1];

					
			result = OneFrame[DataBlockAddr + 2];
			ter_or_veh_reg_result = result;
			// 判断result		
			if (result == 0)			// 注册成功
			{
				NeedRegServer = 0;	
				DFE.Write(EE_NEED_REG_SERVER, 1, &NeedRegServer);	
				SystemLog.GSM.RegServerOK++;	
				
				SetBufferValue((uchar *)&FlashPageCnt.AlarmMsgSave, 16, 0);
						   
				jt_recv_flag |= 1 << eBB_gFLAG_REG_INDEX;

			  	// 重置重发时间
				AckBuffer.MsgId = 0;		//以便跳出多次发送									  	
				ReSend.OverTime = ReSend.Times = 0;	  // 应答超时间 10s
				ReSend.PreOverTime = SendOVT.TCP_ACK_OverTime;  
				
				Uart.puts(" 注册成功\n");
				GPRS.InitAlig('P');		// 清除优先出队的数据（注册使用优先出队）
					
				// 获取鉴权码	 
				SetBufferValue(  (uchar*)jt_auth_code, LEN_AUTH_CODE,0);
				memmove(&jt_auth_code[0], &OneFrame[DataBlockAddr + 3], DataLen - 3);
				DFE.Write(EE_AUTH_CODE, LEN_AUTH_CODE,(uchar *)&jt_auth_code[0]);			//  写鉴权码到铁电
				
			}
			else if (result == 1)	// 车辆已被注册
			{
				jt_recv_flag |= 1 << eBB_gFLAG_POS_INDEX;
				Uart.puts("车辆已被注册\n");
				GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DEREG,0,NULL);
			}
			else if (result == 2)   // 数据库中无该车辆
			{
				 jt_recv_flag |= 1 << eBB_gFLAG_POS_INDEX;
				 Uart.puts("数据库中无该车辆!\n");
				 RegisterWithoutTer++; //重新解析域名或连接IP
			}
			else if (result == 3)   // 终端已被注册
			{
				  jt_recv_flag |= 1 << eBB_gFLAG_POS_INDEX;
				  Uart.puts("终端已被注册!\n");
				  GPRS.BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DEREG,0,NULL);
			}
			else if (result == 4)  // 数据库中无该终端
			{
				  jt_recv_flag |= 1 << eBB_gFLAG_POS_INDEX;
				  Uart.puts("数据库中无该终端!\n");
				  RegisterWithoutTer++; //重新解析域名或连接IP
			}
			


		break;
		case eBB_gCMD_PLAT_QUERY_TER_POS:					// 位置信息查询
			

			AckBuffer.MsgId = MsgId;
			AckBuffer.Index = Index; 
			Uart.puts("查询位置信息应答 ");	 
			BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_POS_QUERY_ACK,0,NULL);
		break;
		/*
		//////////////////
		// 多媒体数据上传应答。注：终端发送完全部多媒体数据后，平台下发该指令。此应答无多媒体类型，根据最近上传类型确定
		*/	
		case eBB_gCMD_PLAT_MEDIA_DATA_ACK:				
			
	        if (PicRetryOrMulPic == 2)    return OK;		//  正在发送存储多媒体忙碌, 直接返回
			PicRetryOrMulPic = 1;                     

			PDU_SMS.Ack = 0;          //大数据强制从GPRS通道传输	如果是SMS通道下达的采集指令，数据从GPRS返回
			Tp.Buffer[3] = OneFrame[DataBlockAddr];
			Tp.Buffer[2] = OneFrame[DataBlockAddr + 1];
			Tp.Buffer[1] = OneFrame[DataBlockAddr + 2];
			Tp.Buffer[0] = OneFrame[DataBlockAddr + 3];

			jt_media_id = 	 Tp.B32[0];

			//if (Tp.B32[0] == jt_media_id)
			{
				Tp.Buffer[0] = OneFrame[DataBlockAddr + 4];				 // 重传包总数

				if ((!Tp.Buffer[0]) || (!(DataLen - (DataBlockAddr + 1) - 4 - 1 - i * 4)))			 // 收到全部包
				{
					jt_send_cmd_flag = 0;
					jt_recv_flag |= 0x01 << eBB_gFLAG_OTHER_INDEX;

					//Protocol.GPRS[JT_PROTOCOL] = 0;  
					PicRetryOrMulPic = 0; 
					ReSend.OverTime = ReSend.Times = 0;	  // 应答超时间 10s
					ReSend.PreOverTime = SendOVT.TCP_ACK_OverTime;  
					
				}
				else
				{
					jt_recv_flag |= 0x01 << eBB_gFLAG_OTHER_INDEX;
				   //set_send_pic_time = BB_SEND_PIC_TIME;				// 应放到系统初始化
					

					j =Tp.Buffer[0];	  // 重传包总数
					JtSendPic.SendPicNo = jt_media_id - 1;  // 图片序号 : 多媒体ID - 1
				   JtSendPic.TotalPack = j;

					SetBufferValue(JtSendPic.SendPic.SendPackNo, 125, 0); 

					  
					//for (i = 1; ((DataLen - (DataBlockAddr + 1) - 4 - 1 - i) != 0) && (i <= j); i++)
					for (i = 0; i < j; i++)
					{			
						//JtSendPic.SendPic.SendPackNo[i] = OneFrame[DataBlockAddr + 4 + i + 1];
						JtSendPic.SendPic.SendPackNo[i] = OneFrame[DataBlockAddr + 4 + i + 1] - 1;   //	实际存在flash包的序号是从0开始，而部标规定是从1开始，所以减去1
					}

					//send_record.n = 0;
					//send_record.Tn = send_record.Tn_1 = set_send_pic_time;			 // 设置发送图片超时时间
					JtSendPic.Index = 0;
					jt_pic_send_flag = 1;
					Uart.puts("多媒体数据应答 ");	 
					BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_MEDIA_DATA_UPLOAD,0,NULL);
				}
			}
		break;
		/*
		//////////////////
		// 摄像头立即拍摄。本指令无通用应答，使用专用应答
		*/	
		case eBB_gCMD_PLAT_TAKE_PHOTO:					
		//case eBB_gCMD_PALT_ONE_MEDIA_DATA_SERCH_UPLOAD:			//临时使用	
			
			NeedTakePhotoAck = TRUE;
		
		 	if (CameraUsing == TRUE)	
		 	{
		 		ter_or_veh_reg_result = 0x01;  // 通道不支持
				BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_TAKE_PHOTO_ACK,0,NULL);
		 		Uart.puts(" 远程设置拍照或录像，但摄像头忙，返回 ");
		 		return OK;			 // 正在使用摄头直接的返回
		 	}

			PDU_SMS.Ack = 0;          //大数据强制从GPRS通道传输	如果是SMS通道下达的采集指令，数据从GPRS返回
			//AckBuffer.MsgId = MsgId;
			//AckBuffer.Index = Index;	 
			Plat_Serial_No = Index;	   // 特殊用途，与AckBuffer.Index不同
			
			pCamTakePhoto = (pJT808_CAMERA_TAKE_PHOTO)&OneFrame[DataBlockAddr];
			pCamTakePhoto->Cmd = SwapINT16(pCamTakePhoto->Cmd);

			s = pCamTakePhoto->Channel;
		    
			
			if ((pCamTakePhoto->Cmd  == 0xFFFF) || ((!s  || (s > 4)) ))//|| (!Camera.NCameraID[s-1]))) 	 // 录像，或摄像头不存在
			{
				ter_or_veh_reg_result = 0x02;  // 通道不支持
				BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_TAKE_PHOTO_ACK,0,NULL);
				Uart.puts(" 远程设置拍照或录像，通道不支持或录像不支持 ");
				return OK;  
			}
			
			JT_ManulCameraID = s;		 // 使用的指定的摄像头拍照
			MediaEventFlag = eBB_MEDIA_EVENT_PLAT;

			Camera.GetOperateParameter(0,&cPic);	 	// 先读取原来参数
			if (pCamTakePhoto->SaveFlag)			cPic.SaveAndSend = 0;
			else											cPic.SaveAndSend = TRUE;			
			if (pCamTakePhoto->Res == 0x01)		cPic.Resolution = 0x03;	 	// 只支持320*240 和 640*480
			else if(pCamTakePhoto->Res == 0x02)	cPic.Resolution = 0x05;
			else											cPic.Resolution = 0x03;
			Camera.SetOperateParameter(0, &cPic);		// 更新

			if ( (pCamTakePhoto->Cmd)==0)
			{
				Camera.SetTakeNumber(0, 0);				
			}
			else 								  // 暂无录像功能，不作考虑
			{
				
				SetBufferValue((uchar *)&TakePhotoAck, LEN_TAKE_PHOTO_ACK+1, 0);
				Protocol.GPRS[JT_PROTOCOL] = TRUE;
				Camera.SetTakeNumber(pCamTakePhoto->Cmd, 0);				
				SetBufferValue( Tp.Buffer,32,0);
				strcpy( (char*)Tp.Buffer,"远程设置立即拍照");
				Int2Str( pCamTakePhoto->Cmd, (char*)&Tp.Buffer[16]);
				strcat( (char*)Tp.Buffer,"张");
				Uart.puts((char*)Tp.Buffer);
				TakePhoneAckFlag = TRUE;
				TakePhotoAck.MediaNum = 0;				
				return OK;
			}

		break;
		/*
		//////////////////
      // 单条多媒体数据，检索上传
		*/	
		case eBB_gCMD_PALT_ONE_MEDIA_DATA_SERCH_UPLOAD:					

			#if(0)
			MediaEventFlag = eBB_MEDIA_EVENT_PLAT;
			Protocol.GPRS[JT_PROTOCOL] = TRUE;
			Camera.SetTakeNumber(1, 0);
			NeedTakePhotoAck = 0;	
			Jt_photo_upload = TRUE;
			#else
	      Camera.GetOperateParameter(0, &cPic);	 	// 先读取原来参数
	      
	      Tp1.Buffer[3] = OneFrame[DataBlockAddr++];
	      Tp1.Buffer[2] = OneFrame[DataBlockAddr++];
	      Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
	      Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
	      cPic.SendPicNo = Tp1.B32;
	      
	      strcpy( (char*)Tp.Buffer,"检索并上传第");
	      Int2Str( Tp1.B32, (char*)&Tp.Buffer[12]);
	      strcat( (char*)Tp.Buffer,"张图片");
	      if( OneFrame[DataBlockAddr] )
	      {	// 删除图片
	      	strcat( (char*)Tp.Buffer,"，并删除图片 ");
	      	cPic.DelPicNo = Tp1.B32;
	      }
	      
	      Uart.puts((char*)Tp.Buffer);	 
			Camera.SetOperateParameter(0, &cPic);		// 更新
			#endif
			return OK;
		break;			 
		/*
		//////////////////
		//  存储多媒体数据检索（暂时只检索图片）
		*/	
		case eBB_gCMD_PLAT_MEDIA_DATA_SERCH:					

			PDU_SMS.Ack = 0;     //大数据强制从GPRS通道传输	如果是SMS通道下达的采集指令，数据从GPRS返回
			
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
		//	存储多媒体数据上传
		*/	
		case eBB_gCMD_PLAT_SET_MEDIA_DATA_UPLOAD:	
				
			if (CameraUsing == 1)		return OK;			

			if (PicRetryOrMulPic == 1)  return OK;				 // 在发重传包，直接返回
			PicRetryOrMulPic = 2;

			while((USBTaskCreat!=0) || (PrintTaskCreat!=0)) 
			{
				os_dly_wait(1);
			}
			
			ComtAck = 1;
		break;


		/*
		/////////设置参数
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
		//////////查询设置参数
		*/
		case   eBB_gCMD_PLAT_QUERY_TER_PARA:
		  
		  PDU_SMS.Ack = 0;          //大数据强制从GPRS通道传输	如果是SMS通道下达的采集指令，数据从GPRS返回
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
		  //memmove(OneFrame, (uchar *)&Index, 2);				//应答包流水号
		  OneFrame[2] = tmp;
		  GPRS.JT808_DataPackPushAlignment(0,eBB_gCMD_TER_QUERY_PARAM_ACK, Len, 0, OneFrame);
		  Uart.puts("查询参数应答3333 ");	 
		  ComtAck = 3;
		break;
	   /*
		///////////终端控制
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
			if(tmp==1)					  //连接升级服务器
			{
				if (SearchArrayCount(DataLen - 1,&OneFrame[DataBlockAddr+1], 1, (uchar*)";") != 10)	// 无线升级，参数之间应有10个分号
				{
					result = 1;
					break;
				}

			   SetBufferValue((uchar *)&JT808_Ctr_IP_DNS, LEN_JT_CTR_IP_DNS, 0);
				SetBufferValue((uchar *)&UpdatePara,UpdatePara_Len, 0);
				Len = DataBlockAddr+1;
				i = DataLen; 				 // 消息体长度
				j = 0;					 	 // 标志升级方式

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
						//strncpy((char *)&UpdatePara.FileList[0], (char *)&OneFrame[Len], DataLen);				   //FTP参数  目录	
						if ((ptr = SearchArray(DataLen - 6, ptr + 6, 1, (uchar *)"/")) == NULL)
						{
							result = 1;
							break;	
						}

						memmove(UpdatePara.FileList, "..", 2);   // ftp站点的根目录		G11升级文件默认放到此
						memmove(UpdatePara.FileName, ptr + 1, DataLen - (ptr + 1 - &OneFrame[Len]));    // 升级文件名

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
					strncpy((char *)&UpdatePara.FTPname[0], (char *)&OneFrame[Len], DataLen);					 //FTP参数	登录用户名
				}						 
				Len += (DataLen + 1);
				if((DataLen = MyStrLen((char *)&OneFrame[Len], ';'))!=0)
				{
				   strncpy((char *)&JT808_Ctr_IP_DNS.C_IP_Secrier[0], (char *)&OneFrame[Len], DataLen);
				   strncpy((char *)&UpdatePara.FTPpassword[0], (char *)&OneFrame[Len], DataLen);				  //FTP参数	 登录密码
				}
				Len += (DataLen + 1);
				if((DataLen = MyStrLen((char *)&OneFrame[Len], ';'))!=0)
				{
				   strncpy((char *)&JT808_Ctr_IP_DNS.C_IPorDNS[0], (char *)&OneFrame[Len], DataLen);
					strncpy((char *)&UpdatePara.RemoteAddr[0], (char *)&OneFrame[Len], DataLen);				 //FTP参数	 连接的IP或DNS
				}
				else
				  result = 1;

				Len += (DataLen + 1);


				//JT808_Ctr_IP_DNS.C_TCP_Port = ByteToINT16(OneFrame[Len], OneFrame[Len+1]);
				//Int16ToDecChar((char )JT808_Ctr_IP_DNS.C_TCP_Port, (char *)&UpdatePara.RemotePort[0]);		  //FTP参数	  端口

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
					Int16ToDecChar((char )JT808_Ctr_IP_DNS.C_TCP_Port, (char *)&UpdatePara.RemotePort[0]);		  //FTP参数	  端口
					s = 1;
				}
				else if (j == 1)				 // ftp升级方式， TCP端口不存在
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

				if (s == 0)				 // 若没有TCP端口参数	  也没有	UDP参数
				{
					result = 1;
					break;
				}


				if((DataLen = MyStrLen((char *)&OneFrame[Len], ';'))!=0)
				{
					if (DataLen != 5)			 // 终端制造商编码必须为5个字节
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

				if ((Len - DataBlockAddr) <= (i - 2))	// 小于消息体长度减2，认为参数最后字段合法
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
				   //if(JT808_Ctr_IP_DNS.Net_Time==0xffff)   	 //FTP方式升级
					if (j == 1)			// FTP升级方式
					{
						GG_Status.Flag.Bits.Updating = 0; 		
						UpdatePara.Enable = ENABLE_FLAG;			//使能
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
			else if(tmp==2)			  //连接指定服务器
			{	
				if ((OneFrame[DataBlockAddr+1] == 0x00) && 	  // 是连接指定服务器，并且参数之间应有8个分号
					 (SearchArrayCount(DataLen -1,&OneFrame[DataBlockAddr+1], 1, (uchar*)";") != 8))	
				{
					result = 1;
					break;
				}

				SetBufferValue((uchar *)&JT808_Ctr_IP_DNS, LEN_JT_CTR_IP_DNS, 0);
				if(OneFrame[DataBlockAddr+1]<2)
				{
					///JT808_Ctr_IP_DNS.Ctr = OneFrame[DataBlockAddr+1];
					//if(JT808_Ctr_IP_DNS.Ctr==1)		  	//关闭临时连接服务器
					if (OneFrame[DataBlockAddr+1] == 1)
					{
						JT808_Ctr_IP_DNS.Ctr = 1;
						sIP.EnableFlag[eFLAG_TCP] = ENABLE_FLAG;  // 切换到主服务器使能

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
						i = DataLen;                 // 消息体长度


						if((DataLen = MyStrLen((char *)&OneFrame[Len], ';'))!=0)
						   strncpy((char *)&JT808_Ctr_IP_DNS.Akey[0], (char *)&OneFrame[Len], DataLen);						     //临时连接的平台鉴权码
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



						if (s == 0)				 // 若没有TCP端口参数	  也没有	UDP参数
						{
							result = 1;
						}
						
						if ((Len - DataBlockAddr) <= (i - 2))	// 小于消息体长度减2，认为参数最后字段合法
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
				JT_Ctr_PowerDown = 1;		//关机，把所有电源关掉，只有重启才恢复工作，可以是ACC开触发重启   0x87EBB7DE			好像没省什么电呢
				//LPC_SC->PCONP  = 0x00689786;		
			}
	  	   else if(tmp==4)		    //复位								 居然死掉不重启，什么问题？	是否跟没用外部看门狗有关
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
			else if(tmp==5)		    //恢复出厂设置	里程，记录清零，特征系数恢复，驾驶员恢复，车辆信息恢复，除1主服务器参数外其他IP相关和号码相关清零，清除鉴权码，恢复速度限制值，疲劳驾驶参数值，
			{
				Uart.puts("Reset Factory config...\r\n\0"); os_dly_wait(10);	
				//DFE.Read(EE_JT_MAIN_SERVER, LEN_JT808_IP_DNS, Tp.Buffer);
				os_dly_wait(1);
				//Uart.EreadFRAM_RecodeMem(16, 0x2000-16);									   //擦除整片Fram	  除主服务器地址
				os_dly_wait(1);
				//DFE.Write(EE_JT_MAIN_SERVER, LEN_JT808_IP_DNS, Tp.Buffer);
		      //Uart.EreadFirstBlock();	
				Sys.FirstPowerOn(FALSE);  				//恢复出厂设置
				os_dly_wait(10);
				DisableIRQ();
	        	NOP();	NOP();	NOP();	NOP();
	       	NOP();	NOP();	NOP();	NOP();
				NVIC_SystemReset();						   //恢复出厂设置还是要重启一下
			}
			else if(tmp==6)			//关闭数据通信	 目前只关闭串口0、USB、CAN总线，并没有关闭电源，只有重启才恢复
			{
			   if(JT_ShutCommunication==0)
				{
					Uart.puts("Close Communication...。\r\n\0"); os_dly_wait(10);	
					JT_ShutCommunication = 1;
					
				}
				else
				{
				   Uart.puts("Open Communication...\r\n\0"); os_dly_wait(10);	
					JT_ShutCommunication = 0;
				
				}
			}
			else if(tmp==7)		   //关闭所有无线通信
			{
				Uart.puts("终端控制，关闭通讯模块\r\n\0"); os_dly_wait(10);	
			   JT_ShutMobileCnt	= TRUE;
				//TestBlind = TRUE;
			}
			else if(tmp==0x69)				 //附加控制项
			{
				
				Sys.FirstPowerOn(TRUE);  				//恢复出厂设置	  重度 ，清所有数据包括IP地址和记录仪数据
				NVIC_SystemReset();          
			}
			else 
			{
			  result = 3;			  //不支持
			}
			
		break;
		/*
		 //临时位置跟踪控制		先设置，整合后再实现
		 */
		case   eBB_gCMD_PLAT_TRACE_TER_POS:		            
		   
			Len = DataBlockAddr;
			SendInterval.T_Second = ByteToINT16(OneFrame[Len],OneFrame[Len+1]);			 //0停止跟踪 	,其他，即时跟踪						
			SendInterval.T_ContinuedTime = ByteToINT32(OneFrame[Len+2],OneFrame[Len+3],OneFrame[Len+4],OneFrame[Len+5]) ;			                              
			DFE.Write(EE_JT_SEND_T_SECOND, 8, (uchar *)&SendInterval.T_ContinuedTime);	
			GSM.CalculateSendInterval(0);			
			ComtAck = 1;
		break;	
		/*
		/////////车辆控制
		*/
		case   eBB_gCMD_PLAT_VEH_CTRL:
		   
			//GPRS.RemoteLockDoor(&OneFrame[DataBlockAddr]);
			s = OneFrame[DataBlockAddr];
			if(s&0x01)
			{									  //上锁
			   CONTROL_OUT1;
				os_dly_wait(20);
			   NULL_CONTROL_OUT1;
				JT808Flag.VS2.Bits.LockDoor = 1;
			}
			else
			{									 //开锁
			  CONTROL_OUT2;
			  os_dly_wait(20);
			  NULL_CONTROL_OUT2;
			  JT808Flag.VS2.Bits.LockDoor = 1;
			}
			Tp.Buffer[0] = Index>>8;
		   Tp.Buffer[1] = Index;
			Len = get_position_info(0,&Tp.Buffer[2], 2);
         GPRS.JT808_DataPackPushAlignment(0,eBB_gCMD_TER_VEH_CTRL_ACK, Len+2, 0, Tp.Buffer);
			Uart.puts("车辆控制应答 ");	 
			ComtAck = 0;
		break;
		/*
		/////////监听	自定义指令 消息体 00#取预存的监听号码拨号监听; 01,13580474097# 取下发的号码拨号监听;其他无效
		*/
		case    eBB_gCMD_PLAT_LISTEN_QUIET:
			#if( TEST_CODE_JUST_FOR_TEST_LUOYANG == 1) 
			Len = strlen( (char*)&sTelNum.Center[0] );
			PhoneCall.DialFlag = TRUE;																 
		   PhoneCall.Length = Len;
			PhoneCall.ListeningFlag = TRUE;			
			memmove((uchar*)&PhoneCall.Buffer[0],(char*)&sTelNum.Center[0], Len);		// 检测中心临时使用设置的中心号码
			
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
		/////////电话回拨 消息体 \x00 13580474097普通通话; \x01 13580474097 监听;其他无效
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
		      memmove((uchar*)&PhoneCall.Buffer[0],(uchar *)Tp.Buffer, Len);		  //电话回拨时显示屏不显示拨号进程和正在通话提示，屏不支持
		      PhoneCall.CenterCall = TRUE;
			}
			else
			  result = 3;
			
			Uart.puts("Callback cmd...");
		   ComtAck = 1;
		break;
		/*
		/////////车辆控制 断油电          1,0 动态   1,1静态   0,0 恢复
		*/
		case   eBB_gCMD_PLAT_CUT_POWER:				 
			
			s = OneFrame[DataBlockAddr];
			tmp = OneFrame[DataBlockAddr+2];

			if(((s!='0')&&(s!='1'))||((tmp!='0')&&(tmp!='1')))										
			{
				result = 3;
			}
			else if(s=='1')												 		// 断油路
			{
				if(tmp=='1')											 		// 静态断油路
				{
					VehicleControl.Lock = LOCK_VEHICLE_ENABLE_INV;					//发动机工作时不锁车
				}
				else 																// 断油断电
				{		
					VehicleControl.ForceLock = 0x00;
					VehicleControl.Lock = LOCK_VEHICLE_ENABLE_INV;		// 动态断油路		  //发动机工作时不锁车
					gCnt.PulsControl = 30;	  									// 1分钟
				}		
				DFE.Write(EE_LOCK_FLAG,4,(uchar *)&VehicleControl);	 	// 断油标志写入铁电
			}
			else																	// 恢复油路
			{
				VehicleControl.Lock = 0x00;
				VehicleControl.ForceLock = 0x00; 			  			// 
				DFE.Write(EE_LOCK_FLAG,4,(uchar *)&VehicleControl);	 	// 断油标志写入铁电
			}
			ComtAck = 1;
		break;
		/*	
		//////////设置圆形区域
		*/
		case   eBB_gCMD_PLAT_SET_ROUND_REGION:
		   
			
		
				         
			ComtAck = 1;	
		break;	
		/*
		//////////删除圆形区域
		*/
		case   eBB_gCMD_PLAT_DEL_ROUND_REGION:	
		   
			
		
			        
			
			 ComtAck = 1;
		break;	

		// 设置矩形区域
		//uchar		s, tel,tmp, k, result = 0, isPack,   ComtAck = 0, n, m,Cmd2,VirtualAuth=0;
		//uchar 	*ptr;
		//uint16	DataLen, MsgId, MsgAttr, Index, DataBlockAddr;//DataBlockAddr;
		//uint32	i=0,j,Len,Addr;
		case eBB_gCMD_PLAT_SET_RECT_REGION:

			ptr = NULL;
			while(ptr==NULL)	{	ptr = MyMalloc(512);	os_dly_wait(1);	}
			s = OneFrame[DataBlockAddr++];		// 状态类型
			m = OneFrame[DataBlockAddr++];		// 围栏个数
			if( s == 0 )			// 更新
			{
				
			}
			else if( s == 1 )		// 追加
			{				
				//DFE.Read(EE_RECTANGLE_FENCE_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RectFenceAttr);			
				// 无需从铁电读取，因为系统启动时已读取
				if(RectFenceAttr.Total>=128)
				{
					MyFree(ptr);
					ptr = NULL;
					ComtAck = 1;
		   		result = 1;		// 失败应答		   	
					Uart.puts("超出最大矩形围栏数，请先删除 ");
					break;
				}
				
				//7E 86 02 00 18 01 38 86 08 88 88 16 CA 
				//01 01 	00 00 00 66 	40 00 	02 62 54 81 	06 BC 3E 40 	02 38 A9 58 	07 17 CB C0 	70 7E 
				// 国业内部运算的经纬度比JT808大10倍
				for(i=0;i<m;i++)
				{
					Addr = FF_RECTANGLE_FENCE_START_PAGE;
					Addr += (RectFenceAttr.Total/8);
					
					DFF.PageRead( Addr, ptr);				// 先读取，再擦除。设置前8个都要擦除
					
					if(Addr%FF_PAGE_PER_SECTOR==0)
					{
						DFF.SectorEraseWaitOK( Addr/FF_PAGE_PER_SECTOR ,10);						
					}
					
					pFence = (pELE_FENCE_STRUCT)( ptr+((RectFenceAttr.Total%8)*LEN_ELE_FENCE_STRUCT) );		// 指向公共数据区
					Tp1.Buffer[3] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[2] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
					pFence->ID = GPS.TestOneRectFence.ID = Tp1.B32;							// 围栏ID
					Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
					pFence->Attr.Word = GPS.TestOneRectFence.Attr.Word = Tp1.B16[0];	//属性
					
					Tp1.Buffer[3] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[2] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
					pFence->Lat1 = GPS.TestOneRectFence.Lat1 = Tp1.B32*10;		// 左上角纬度	注：不同左下右上方式
					Tp1.Buffer[3] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[2] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
					pFence->Lon1 = GPS.TestOneRectFence.Lon1 = Tp1.B32*10;		// 左上角经茺
					Tp1.Buffer[3] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[2] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
					pFence->Lat2 = GPS.TestOneRectFence.Lat2 = Tp1.B32*10;		// 右下角纬度
					Tp1.Buffer[3] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[2] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
					pFence->Lon2 = GPS.TestOneRectFence.Lon2 = Tp1.B32*10;		// 右下角经度
					
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
						pFence->LimitSpeed = GPS.TestOneRectFence.LimitSpeed = Tp1.B16[0];			// 取低8位				
						pFence->ContinueTime = GPS.TestOneRectFence.ContinueTime = OneFrame[DataBlockAddr++];
					}
					else
					{
						pFence->LimitSpeed = 0;						
						pFence->ContinueTime = 0;
					}
										
					DFF.PageProgramWaitOK( Addr,ptr,3);
					
					memmove( (uchar*)&GPS.TestOneRectFence,(uchar*)pFence,LEN_ELE_FENCE_STRUCT);
					
					Uart.puts("远程追加矩形电子围栏，属性：");
					if( (pFence->Attr.Word)&0x0001)	Uart.puts("根据时间、");
					if( (pFence->Attr.Word)&0x0002)	Uart.puts("限速、");
					if( (pFence->Attr.Word)&0x003c)	Uart.puts("进出围栏报警、");
					if( (pFence->Attr.Word)&0x4000)
					{
						ReadyTestBlind = TRUE;
						Uart.puts("关闭GSM通讯、");
					}
					os_dly_wait(5);
					
					k = RectFenceAttr.Total>>3;				// 除以8
					tmp = RectFenceAttr.Total%8;					
					RectFenceAttr.Enable[k] |= (1<<tmp);	// 使能对应的围栏标志
					
					RectFenceAttr.Total++;
				}
				DFE.Write(EE_RECTANGLE_FENCE_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RectFenceAttr);
			}
			else if( s == 2 )		// 修改
			{
				
			}
			MyFree(ptr);
			ptr = NULL;
			ComtAck = 1;
			
		break;		// 榜  Q 602045858


		case eBB_gCMD_PLAT_DEL_RECT_REGION:
			
			m = 	OneFrame[DataBlockAddr++];
			if(m==0)		// 删除所有矩形围栏
			{
				SetBufferValue((uchar*)&RectFenceAttr,LEN_EE_FENCE_ATTR,0);
				DFE.Write(EE_RECTANGLE_FENCE_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RectFenceAttr);
								
				Uart.puts("远程删除所有矩形电子围栏 ");
			}
			else
			{
				if(m<RectFenceAttr.Total)		RectFenceAttr.Total -= m;
				// 比较ID，删除相对应的围栏号
				
				DFE.Write(EE_RECTANGLE_FENCE_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RectFenceAttr);
			}
			
			ComtAck = 1;
		break;
		
		/////////////////////////////////////////////////////// 多边形
		//uchar		s, tel,tmp, k, result = 0, isPack,   ComtAck = 0, n, m,Cmd2,VirtualAuth=0;
		//uchar 	*ptr;
		//uint16	DataLen, MsgId, MsgAttr, Index, DataBlockAddr;//DataBlockAddr;
		//uint32	i=0,j,Len,Addr;
		case eBB_gCMD_PLAT_SET_POLY_REGION:
			
			//DFE.Read(EE_ROAD_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&PolygonFenceAttr);
		   // 无需从铁电读取，因为系统启动时已读取
		   if( (PolygonFenceAttr.Total>=128) )
		   {
		   	result = 1;		// 失败应答
		   	ComtAck = 1;
		   	Uart.puts("多边形围栏存储空间已满，请先删除 ");
		   	break;
		   }
			
			ptr = NULL;
			while(ptr==NULL)	{	ptr = MyMalloc(512);		os_dly_wait(1); }
			
			
			pPolygon = (pPOLYGON_STRUCT)(ptr);
			
			Tp1.Buffer[3] = OneFrame[DataBlockAddr++];
			Tp1.Buffer[2] = OneFrame[DataBlockAddr++];
			Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
			Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
			pPolygon->ID = Tp1.B32;				// 多边形ID
			
			Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
			Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
			pPolygon->Attr.Word = Tp1.B16[0];		// 属性

			if( (pPolygon->Attr.Word)&0x0001)
			{			
				pRtc = (pRTC_STR)&OneFrame[DataBlockAddr];
				pPolygon->StartTime = Rtc.DateTimeToSecond( pRtc );
				DataBlockAddr += 6;							// 开始时间
				
				pRtc = (pRTC_STR)&OneFrame[DataBlockAddr];
				pPolygon->EndTime = Rtc.DateTimeToSecond( pRtc );
				DataBlockAddr += 6;							// 结束时间
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
		   	result = 1;		// 失败应答
		   	ComtAck = 1;
		   	MyFree(ptr);
		   	Uart.puts("错误，多边形最多顶点数为30 ");
		   	break;
		   }
			//// 国业内部运算的经纬度比JT808大10倍
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
				
			
			
			Uart.puts("远程追加多边形电子围栏，属性：");
			if( (pPolygon->Attr.Word)&0x0001)	Uart.puts("根据时间、");
			if( (pPolygon->Attr.Word)&0x0002)	Uart.puts("限速、");
			if( (pPolygon->Attr.Word)&0x003c)	Uart.puts("进出围栏报警、");
			if( (pPolygon->Attr.Word)&0x4000)	Uart.puts("关闭GSM通讯、");
			os_dly_wait(5);
			
			k = PolygonFenceAttr.Total>>3;				// 除以8
			tmp = PolygonFenceAttr.Total%8;					
			PolygonFenceAttr.Enable[k] |= (1<<tmp);	// 使能对应的围栏标志
			
			PolygonFenceAttr.Total++;

			DFE.Write(EE_POLYGON_FENCE_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&PolygonFenceAttr);
			
			MyFree(ptr);
			ComtAck = 1;
		break;
			
		case eBB_gCMD_PLAT_DEL_POLY_REGION:
		
			m = 	OneFrame[DataBlockAddr++];
			if(m==0)		// 删除所有
			{
				SetBufferValue((uchar*)&PolygonFenceAttr,LEN_EE_FENCE_ATTR,0);
				DFE.Write(EE_POLYGON_FENCE_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&PolygonFenceAttr);
				Uart.puts("远程删除所有多边形电子围栏 ");
			}
			else
			{				
				if(m<PolygonFenceAttr.Total)		PolygonFenceAttr.Total -= m;
				// 比较ID，删除相对应的围栏号
				
				DFE.Write(EE_POLYGON_FENCE_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&PolygonFenceAttr);
			}
			
			ComtAck = 1;
		break;

		
		// 设置路线
		//uchar		s, tel,tmp, k, result = 0, isPack,   ComtAck = 0, n, m,Cmd2,VirtualAuth=0;
		//uchar 	*ptr;
		//uint16	DataLen, MsgId, MsgAttr, Index, DataBlockAddr;//DataBlockAddr;
		//uint32	i=0,j,Len,Addr;
		case   eBB_gCMD_PLAT_SET_PATH:
			
		   
		   		   
		   //DFE.Read(EE_ROAD_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RoadAttr);
		   // 无需从铁电读取，因为系统启动时已读取
		   if( RoadAttr.Total>=128 ) 
		   {
		   	result = 1;		// 失败应答
		   	ComtAck = 1;
		   	Uart.puts("超出最大路线数128，请先删除 ");
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
			pRoadLine->ID = Tp1.B32;			// 路线ID
			
			Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
			Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
			pRoadLine->Attr.Word = Tp1.B16[0];		// 属性

			if( (pRoadLine->Attr.Word)&0x0001)
			{ 			
				pRtc = (pRTC_STR)&OneFrame[DataBlockAddr];
				pRoadLine->StartTime = Rtc.DateTimeToSecond( pRtc );
				DataBlockAddr += 6;							// 开始时间
				
				pRtc = (pRTC_STR)&OneFrame[DataBlockAddr];
				pRoadLine->EndTime = Rtc.DateTimeToSecond( pRtc );
				DataBlockAddr += 6;							// 结束时间
			}
			else
			{
				pRoadLine->StartTime = 0;
				pRoadLine->EndTime = 0;
			}
			
			Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
			Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
			m = pRoadLine->DotNumber = Tp1.Buffer[0];	// 总拐点数
			
			if(m>MAX_ROAD_DOT_NUM)
			{
		   	result = 1;		// 失败应答
		   	ComtAck = 1;
		   	MyFree(ptr);
		   	Uart.puts("同一条路线最多29个路段 ");
		   	break;
		   }
			
			for(i=0;i<m;i++)									// 处理各拐点
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
				
				// 国业内部运算的经纬度比JT808大10倍
				Tp1.Buffer[3] = OneFrame[DataBlockAddr++];
				Tp1.Buffer[2] = OneFrame[DataBlockAddr++];
				Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
				Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
				pRoadLine->Dot[i].Lat = Tp1.B32*10;		// 拐点纬度
				
				Tp1.Buffer[3] = OneFrame[DataBlockAddr++];
				Tp1.Buffer[2] = OneFrame[DataBlockAddr++];
				Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
				Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
				pRoadLine->Dot[i].Lon = Tp1.B32*10;		// 拐点经度
				
				pRoadLine->Dot[i].Width = OneFrame[DataBlockAddr++];			// 路宽
				pRoadLine->Dot[i].Attr.Byte = OneFrame[DataBlockAddr++];	// 路段属性
				
				if( pRoadLine->Dot[i].Attr.Bits.DriveTime==1)
				{
					Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
					pRoadLine->Dot[i].MaxDriveTime = Tp1.B16[0];				// 允许最长行驶时间
					
					Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
					Tp1.Buffer[0] = OneFrame[DataBlockAddr++];
					pRoadLine->Dot[i].MinDriveTime = Tp1.B16[0];				// 允许最短行驶时间
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
					pRoadLine->Dot[i].LimitSpeed = Tp1.B16[0];							// 限速									
					pRoadLine->Dot[i].ContinueTime = OneFrame[DataBlockAddr++];	// 超时允许时间
				}
				else
				{
					pRoadLine->Dot[i].LimitSpeed = 0;					// 限速									
					pRoadLine->Dot[i].ContinueTime = 0;				// 超时允许时间
				}
			}
			
			Len = m ;
			Len *= LEN_ROAD_DOT_STRUCT;
			Len += LEN_ROAD_LINE_STRUCT;			// 需存储的路线数据长度
			
			Addr = FF_ROAD_PLAN_START_PAGE;		// 
			Addr += (RoadAttr.Total<<1);			// 每条路线固定存储长度为2页Flash
						
						
			// 应付测试，先存储在铁电（仅1条）
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
			
			Uart.puts("远程追加路线规划，属性：");
			if( (pRoadLine->Attr.Word)&0x0001)	Uart.puts("根据时间、");
			if( (pRoadLine->Attr.Word)&0x003c)	Uart.puts("进出围栏报警、");
			if( (pRoadLine->Attr.Word)&0x0002)	Uart.puts("限速 ");
			os_dly_wait(5);
			
			k = RoadAttr.Total>>3;				// 除以8
			tmp = RoadAttr.Total%8;					
			RoadAttr.Enable[k] |= (1<<tmp);	// 使能对应的围栏标志
			
			RoadAttr.Total++;		
			DFE.Write(EE_ROAD_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RoadAttr);
			
		   MyFree(ptr);
			ComtAck = 1;		  
		break;	
		 /*
		 //////////删除路线
		 */
		case   eBB_gCMD_PLAT_DEL_PATH:
		   
				             
			m = 	OneFrame[DataBlockAddr++];
			if(m==0)		// 删除所有
			{
				SetBufferValue((uchar*)&RoadAttr,LEN_EE_FENCE_ATTR,0);
				DFE.Write(EE_ROAD_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RoadAttr);
				
				Uart.puts("远程删除所有路线计划 ");
			}
			else
			{
				DFE.Read(EE_ROAD_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RoadAttr);
				if(m<RoadAttr.Total)		RoadAttr.Total -= m;
				// 比较ID，删除相对应的路线
				
				DFE.Write(EE_ROAD_ATTR,LEN_EE_FENCE_ATTR,(uchar*)&RoadAttr);
			}
			
			ComtAck = 1;	   
		
		break;
		/*
		////////////记录仪数据采集
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
						FlashPageCnt.Speed48HSave = 1440;				// 126B/分钟，2分钟/页，
						FlashPageCnt.Speed48HRead = 0;
					break;
					case eGB_rCMD_GET_POSITION_INFO:	
						FlashPageCnt.Location360HSave = 1080;			// 666B/小时，3页/小时
						FlashPageCnt.Location360HRead = 0;
					break;
					case eGB_rCMD_GET_PARKING_DATA:	
						FlashPageCnt.ParkingDataSave = 100;				// 234B/条，1条/页，100条
						FlashPageCnt.ParkingDataRead = 0;
					break;
					case eGB_rCMD_GET_OVER_TIME_DRIVE:	
						FlashPageCnt.FatigueDrivingSave = 20;			// 50B/条，5条/页，100条
						FlashPageCnt.FatigueDrivingRead = 0;
					break;
					case eGB_rCMD_GET_GET_LOG_IN_OUT_RECORD:	
						FlashPageCnt.DriverLogInOutSave = 20;			// 25B/条，10条/页，200条
						FlashPageCnt.DriverLogInOutRead = 0;
					break;
					case eGB_rCMD_GET_POWER_RECORD:	
						FlashPageCnt.PowerOnOffSave = 6;					// 7B/条，36条/页，共108条
						FlashPageCnt.PowerOnOffRead = 0;
					break;
					case eGB_rCMD_GET_MODIFY_PARA_RECORD:				
						FlashPageCnt.ParaModifySave = 3;					// 7B/条，36条/页，共108条
						FlashPageCnt.ParaModifyRead = 0;
					break;
					case eGB_rCMD_GET_SPEED_STATUS_LOG:
						FlashPageCnt.SpeedStatusSave = 6;					// 7B/条，36条/页，共108条
						FlashPageCnt.SpeedStatusRead = 0;
					break;
					
				}
			} */

			s = OK;  // ERROR： 错误标志， OK：无错误
			if(m<eGB_rCMD_GET_SPEED_PER_SECOND) 	//  对于 00H - 07H 平台下发无 AA 75 	
			{
				GetVDR_DataFlag = TRUE;
				GetVDR_Cmd = m;
				GetVDR_Index[m] = Index;
			}
			else 	if (m <= eGB_rCMD_GET_SPEED_STATUS_LOG)	  // 对于08H - 15H 有 平台下发  AA 75 
			{

				if (DataLen>=6)
				{
						Len = DataLen-1;
						
			  			Uart.GB19056Process(Len,&OneFrame[DataBlockAddr+1],&Tp.Buffer[3]);  // 
						GB_LargeData.GetVDRFlag = FALSE;  //  不需本地输出
						JT_VehRecord.TotalPack	= 0 ;
						JT_VehRecord.PackIndex = 0;
						JT_RemoteReadRecodMoreDataCmd = m;	//  这里只是设置标志，实际的数据由GPRSCls::PopAlignmentSend(uchar Signal)处理
						GetVDR_Index[m] = Index;
						GetVDR_Cmd = m;

						 
				}
				else
				{
					s = ERROR;
				}
			}
			else				//   错误的采集命令
			{
				s = ERROR;
				JT_RemoteReadRecodMoreDataCmd = 0;
			}

			if (s==ERROR)
			{
				Tp1.B16[0] = Index;
				Tp.Buffer[0] = Tp1.Buffer[1];
				Tp.Buffer[1] = Tp1.Buffer[0];
				Tp.Buffer[2] = m; 		  //  平台下发的命令字
				Tp.Buffer[3] = 0x55;
				Tp.Buffer[4] = 0x7a;
				Tp.Buffer[5] = 0xfa;
				Tp.Buffer[6] = 0x00;
				Tp.Buffer[7] = CalXorSum(8,&Tp.Buffer[3]);
				Length = 8;	
	
				AckBuffer.MsgId = MsgId;
	 			AckBuffer.Index = Index;	
	  			BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_VDR_DATA_UPLOAD,Length,Tp.Buffer);
				Uart.puts("远程采集的命令字错误");	
			}
		  #if (0)//#if (WUXI_TEST != 1)		// 用雪利曼的平台时，不用回通用应答
		  ComtAck = 1;	
		  #endif   
		
		break;	
		 /*
		 ///////////记录仪数据下传
		 */
		case   eBB_gCMD_PLAT_SET_VDR_PARA:
		  
		  //		  GetGB_Data_Flag = 3;	       
//		  Uart.SetGB_Down_CMDHead(U0_RecBuffer,DataLen-1, OneFrame[DataBlockAddr]);
//		  memmove(&U0_RecBuffer[LEN_U0_GB_FRAME_HEAD], &OneFrame[DataBlockAddr+1], DataLen-1);
//		  Uart.SendDownCMDtoU0(U0_RecBuffer, LEN_U0_GB_FRAME_HEAD+DataLen-1);
//		  GetGB_Data_Flag = 0;
		  m = OneFrame[DataBlockAddr];		// 设置命令字
		  s = OK;
		  if (DataLen<6)
		  {
		  	  s = ERROR;
		  }
		  else
		  {
			  //k =CalXorSum(DataLen-2, &OneFrame[DataBlockAddr+1]);		 // 不包含最一包字节的校验字节,所以长度减2	 
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
			
				Uart.puts("远程下传的命令字有误");;	
		  }

	   	AckBuffer.MsgId = MsgId;
		 	AckBuffer.Index = Index;	

		  	//BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_VDR_DATA_UPLOAD,Length,Tp.Buffer);

		   ComtAck = 1;	  				// 新标准改为：需要回复通用应答就行了
		
		break;
		 /*
		 ///////////设置设备SIM卡号码
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
		 ///////////查询设备SIM卡号码
		*/
		case   eBB_gCMD_GET_SIM_NUMBER:						 //
		  
		  memmove(OneFrame, (uchar *)&Index, 2);				//应答包流水号
		  Len = strlen((char *)&sTelNum.Sim[1]);
        memmove(&OneFrame[2],&sTelNum.Sim[1], Len);
		  Len += 2;
		  GPRS.JT808_DataPackPushAlignment(0,eBB_gCMD_GET_SIM_NUMBER_ACK, Len, 0, OneFrame);
		  Uart.puts("查询SIM 卡号应答 ");	 
		  ComtAck = 3;

		break;
		/*
		 ///////////设置AD通道做开关量检测高/低检测标志
		*/
		case  eBB_gCMD_SET_ADSW_FLAG:
		  
		  HighLowLevel  = ByteToINT32(OneFrame[DataBlockAddr], OneFrame[DataBlockAddr+1],OneFrame[DataBlockAddr+2],OneFrame[DataBlockAddr+3]);;
		  DFE.Write(EE_HIGH_LOW_LEVEL,2,(uchar*)&HighLowLevel);
		  ComtAck=1;
		  result = 0;

		break;
		/*
		 ///////////查询AD通道做开关量检测高/低检测标志
		*/
		case  eBB_gCMD_GET_ADSW_FLAG:
		 
		 i = SwapINT32(HighLowLevel);
		 GPRS.JT808_DataPackPushAlignment(0,eBB_gCMD_GET_ADSW_FLAG_ACK, 4, 0, (uchar *)&i);
		 Uart.puts("查询开关量应答 ");	 
		 ComtAck = 3;
		break;
		/*
		 ///////////文本信息下发
		*/
		case  eBB_gCMD_PLAT_SEND_TEXT_INFO:
		 
		 //Uart.puts("CCC1");os_dly_wait(1);

		 s = OneFrame[DataBlockAddr];
		 Len = Length - 1;
		 if(Len>LEN_CMD_DATA_TO_LCM)
		   Len = LEN_CMD_DATA_TO_LCM;
		 if(s&0x04)		  //发送到手柄/显示屏
		 {
		    KeyDisp.CenterSendDataToLCM_YH(Len, (uchar *)&OneFrame[DataBlockAddr + 1]);

		 }
		 else if(s&0x08)	
		 {
		 	OneFrame[DataBlockAddr] = 0x47; // GB码 国标码
			Len += 1;
		 	KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_TMP_MSG, Len, (uchar *)&OneFrame[DataBlockAddr]);
		 }	   //语音播报
		 else if(s&0x10)  {}		//广告屏显示
		 //GPRS.JT_T808_NewDataPack(eBB_gCMD_GET_ADSW_FLAG_ACK, 4, 0, 0, (uchar *)&i);
		 ComtAck = 1;
		break;
		/*
		 ///////////信息服务
		*/
		case  eBB_gCMD_PLAT_INFO_SERVICE:
		 
		 s = OneFrame[DataBlockAddr];
		 Len = Length - 3;
		 if(Len>LEN_CMD_DATA_TO_LCM)
		   Len = LEN_CMD_DATA_TO_LCM;
		 //KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_MSG, Len, (uchar *)&OneFrame[DataBlockAddr + 3]);		  //发送到手柄/显示屏
		 KeyDisp.CenterSendDataToLCM_YH(Len, (uchar *)&OneFrame[DataBlockAddr + 3]);		  //发送到手柄/显示屏
		 ComtAck = 1;
		break;
	   /*
		 ///////////提问下发
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
			 if(n>=10)	 //最多10个备选答案
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
		 //KeyDisp.SendDataToLCM_YH(eLCM_U_CENTER_MSG, tel, (uchar *)Tp.Buffer);		  //发送到手柄/显示屏
		 if (tel < 256 - 20) { memmove(Tp.Buffer+tel, (uchar *)"\xA3\xA8\xD7\xA2\xA3\xBA\xC7\xEB\xBB\xD8\xB8\xB4\xB4\xF0\xB0\xB8\xD0\xF2\xBA\xC5\xA3\xA9", 11 * 2); tel += 22; }
		 KeyDisp.CenterSendDataToLCM_YH(tel, (uchar *)Tp.Buffer);		  //发送到手柄/显示屏
		 ComtAck = 1;
		break;
												
		/*
		//////////////////
		//   查询设备版本号信息
		*/	
		case  eBB_gCMD_GET_VERSION_MSG:
		 SetBufferValue(Tp.Buffer, 50, 0);
		 U0_Iap_Flag = 9;
		 Sys.GetVersion((char *)Tp.Buffer);
		 U0_Iap_Flag = 0;
		 DataLen = strlen((char *)Tp.Buffer);
		 GPRS.JT808_DataPackPushAlignment(0,eBB_gCMD_GET_VERSION_MSG_ACK, DataLen, 0, (uchar *)Tp.Buffer);
		 Uart.puts("查询版本应答 ");	 
		 ComtAck = 3;
		break;
		
		
		/*
		//////////////////
7E 89 00 00 19 00 01 38 86 08 88 04 02 0B 00 01 5B F7 DD E8 FF 1D 2D 48 00 00 00 00 00 00 00 00 00 00 00 00 B0 AF A2 7E 
		//   下行透传
		*/	
		case eBB_gCMD_PLAT_SEND_TRAN_DATA:	

			
			ComtAck=1;			// 透传数据，无需先鉴权
		   //Uart.puts(" 下行透传数据 ");
			if (OneFrame[DataBlockAddr] == 0x0B)	
			{
				Uart.puts(" 从业资格证认证中心回复 ");
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
						SpeedC.SendDataToRFID(0x40, &OneFrame[DataBlockAddr], 25);  // 1字节认证结果，24字节认证数据 透传到RFID
					}
				}	
			}
			else	  // 串口透传
			{
				Uart.putb(1,DataLen,&OneFrame[DataBlockAddr]);
				Uart2.putb(1,DataLen,&OneFrame[DataBlockAddr]);
			}
		
		break;
		
		// added 2013.3
		case eBB_gCMD_PLAT_QUERY_SOME_TER_PARA:							// 查询指定终端参数
			
		  	PDU_SMS.Ack = 0;          //大数据强制从GPRS通道传输	如果是SMS通道下达的采集指令，数据从GPRS返回
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
			while (s < CamBuffer[0])	  // 	CamBuffer[0] 的内容是参数个数
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
			//memmove(OneFrame, (uchar *)&Index, 2);				//应答包流水号
			OneFrame[2] = tmp;
			GPRS.JT808_DataPackPushAlignment(0,eBB_gCMD_TER_QUERY_PARAM_ACK, Len, 0, OneFrame);
			ComtAck = 3;	
			Uart.puts("查询参数应答11 ");	 
		break;

		case		eBB_gCMD_PLAT_QUERY_TER_PRO:								// 查询终端属性	
			
			BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_QUERY_PRO_ACK,0,NULL);
			Uart.puts("查询终端属性应答 ");	 
			ComtAck = 0;	
		
		break;

		// added 2013.4
		case	eBB_gCMD_PLAT_DRIVER_INFO_REQUEST:                              // 上报驾驶员身份信息请求
		case	eBB_gCMD_I2C_DRIVER_INFO_REQUEST:
			
			BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_DRIVER_INFO_REPORT,0,NULL);
			Uart.puts("上报驾驶员身份信息请求 ");	 
			ComtAck = 0;	  // 直接应答，无需通用应答
		
		break;

		//	0x8301 事件设置
		case eBB_gCMD_PLAT_SET_EVENT:
			

			Cmd2 = OneFrame[DataBlockAddr+0];
			if( ( Cmd2 == 0) || (Cmd2 == 1) )	// 删除全部现有事件
			{
				SetBufferValue(JT808_EventID,TOTAL_EVT_INFO_ID_NUMBER,0);
			}
			m = JT808_EventID[0];				// 当前个数
			if( (Cmd2 == 1)||(Cmd2 == 2))	// 追加事件
			{
				n = OneFrame[DataBlockAddr+1];	 	// 读本次操作的总个数
				Index = DataBlockAddr+2;				// 第1个ID所在地址
				for(s=0;s<n;s++)
				{
					if(m<TOTAL_EVT_INFO_ID_NUMBER)
					{
						JT808_EventID[m+1] = OneFrame[Index];		// 读取ID号，并存储到ID列表
						m++;
					}
					Len = OneFrame[Index++];					
					Index += Len;					
				}
			}
			else if ( Cmd2 == 3 )			// 修改
			{
				
			}
			else if ( Cmd2 == 4 )			// 删除特定几项
			{
				
			}
			

			ComtAck = 1;

		break;


		//	0x8303信息点播放菜单	 0:删除全部  1：更新   2：追加   3：修改	 // 更新解释，先删除全部，再从零追加多条
		case eBB_gCMD_PLAT_SET_INFO_MENU:
			

			Cmd2 = OneFrame[DataBlockAddr+0];
			if( ( Cmd2 == 0) || (Cmd2 == 1) )	// 删除全部现有事件
			{
				SetBufferValue(JT808_InfoID,TOTAL_EVT_INFO_ID_NUMBER,0);
			}
			m = JT808_InfoID[0];				// 当前个数
			if( (Cmd2 == 1)||(Cmd2 == 2))	// 追加事件
			{
				n = OneFrame[DataBlockAddr+1];	 	// 读本次操作的总个数
				Index = DataBlockAddr+2;				// 第1个ID所在地址
				for(s=0;s<n;s++)
				{
					if(m<TOTAL_EVT_INFO_ID_NUMBER)
					{
						JT808_EventID[m+1] = OneFrame[Index];		// 读取ID号，并存储到ID列表
						m++;
					}
					Len = OneFrame[Index++];
					Len <<= 8;
					Len += OneFrame[Index++];
					Index += Len;					
				}
			}
			if( Cmd2 == 3 ) // 修改
			{
				
			}					

			ComtAck = 1;

		break;


		case 	eBB_gCMD_PLAT_SET_TEL_BOOK:

			ComtAck = 1;

		break;
		
		
		///////////////// 录音开始命令
		case eBB_gCMD_PLAT_START_RECORD:

			 
			 s = OneFrame[DataBlockAddr++];
			 Tp1.Buffer[1] = OneFrame[DataBlockAddr++];
			 Tp1.Buffer[0] = OneFrame[DataBlockAddr++];	
			 Tp1.Buffer[2] = OneFrame[DataBlockAddr++];
			 
			 
			 if (s == 0x01)		 // 开始录音
			 {
			 	result = 0;										
			 	Record.StartRecord( Tp1.B16[0]+3 );		// 加上损失的时间，约2~3秒
			 }
			 else if(s == 0x00)   // 停止录音
			 {
			 	result = 0;				
			 	Record.StopRecord();
			 }

			 
			 if (Tp1.Buffer[2] == 0x01)		 // 保存
			 {
			 	// 
			 }
			 else if (Tp1.Buffer[2] == 0x00)   // 实时上传
			 {
			 	//
				Record.UploadRecordData(RECORD_ALL_PACK_UPLOAD);
			 }
			ComtAck = 1;

		break;
			
		default:
		
			result = 0;			/// 不支持的消息ID
			ComtAck = 1;
			Uart.puts(" 收到中心指令，但消息ID不支持 ");
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
		 Uart.puts("通用应答 ");
		 BBJT808_GPRS_UploadCmd(eBB_gCMD_TER_ACK,0,NULL);			//通用应答		 	 
	  }
	}

	return  OK;								//立即发送								 
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
						AccelPara[0].Start[i] 	= Dw;			 		// 起始速度

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
						AccelPara[0].End[i] 		= Dw;					// 结束速度

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
						Dw /= 1000;			// m/s2，转换为km/s2
						AccelPara[0].Value[i] 	= Dw;	  				// 允许加速度
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
						AccelPara[1].Start[i] 	= Dw;			 		// 起始速度

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
						AccelPara[1].End[i] 		= Dw;					// 结束速度

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
						Dw *= 36;			// *3600 / 100	  (两位小数）
						Dw /= 1000;			// m/s2，转换为km/s2
						AccelPara[1].Value[i] 	= Dw;	  				// 允许加速度
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


	     case 	   SET_HEART_BEAT_ID:	                                      // 终端心跳发送间隔
		      if(s!=0)
				{
				  SendInterval.HeartbeatTime = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  DFE.Write(EE_JT_HEART_BEAT_TIME, 4,(uchar *)&SendInterval.HeartbeatTime);
				  Uart.puts("远程设置心跳包间隔：");
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

	     case 		SET_TCP_ACK_OVER_TIME_ID:										     // TCP应答超时时间
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
	     case 		SET_TCP_RESEND_TIMES_ID:								           //TCP重传次数
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
	     case 		SET_UDP_ACK_OVER_TIME_ID:											  //  UDP应答超时时间
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
	     case 		SET_UDP_RESEND_TIMES_ID:											  // UDP重传次数
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
	     case 		SET_SMS_ACK_OVER_TIME_ID:											  //  SMS应答超时时间
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
	     case 		SET_SMS_RESEND_TIMES_ID:											  // SMS重传次数
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
	
	     case 		SET_M_APN_ID:								   		              //  主服务器APN
		     if(s!=0)
			  {
				  if(s<=LEN_IPorDNS)               //APN目前只支持32B长度
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
	     case 		SET_M_USE_NAME_ID:											        // 主服务器用户名
		     if(s!=0)
			  {
				  if(s<=LEN_IP_NAME)               // 用户名目前只支持16B长度    
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
	     case 		SET_M_USE_SECRIERS_ID:							 			        // 主服务器拨号密码
		     if(s!=0)
			  {
				  if(s<=LEN_IP_SECRIER)               // 用户密码目前只支持8B长度     
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
	     case 	   SET_M_IP_DNS_ID:											           // 主服务器IP地址或域名

		  	if(s!=0)
			  {
				  if(s<=LEN_IPorDNS)               //IP或DNS目前只支持32B长度
				  {
				  	  SetBufferValue(&JT808_IP_DNS.M_IPorDNS[0], LEN_IPorDNS, 0);
					  memmove((char *)&JT808_IP_DNS.M_IPorDNS[0], (char *)p, s);
					  if(ValidIP_Port(JT808_IP_DNS.M_IPorDNS)==OK)
					  {
						  sIP.EnableFlag[eFLAG_TCP] = ENABLE_FLAG;								              //打开主服务器TCP,UDP连接
	                 //sIP.EnableFlag[eFLAG_UDP] = ENABLE_FLAG;	
	                 	DFE.Write(EE_ENABLE_TCP,1,(uchar*)&sIP.EnableFlag[eFLAG_TCP]);
	                 //DFE.Write(EE_ENABLE_UDP,1,(uchar*)&sIP.EnableFlag[eFLAG_UDP]);
							DFE.Write(EE_JT_M_IP_DNS, LEN_IPorDNS, (uchar *)&JT808_IP_DNS.M_IPorDNS[0]);
						 	t = 80;
					  }
					  else
					  {
					  	  //sIP.EnableFlag[eFLAG_TCP] = 0;								              //关闭主服务器TCP,UDP连接
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
	     case 		SET_B_APN_ID:											              //  备份服务器APN
		     if(s!=0)
			  {
				  if(s<=LEN_IPorDNS)               //APN目前只支持32B长度
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
	     case 		SET_B_USE_NAME_ID:											        // 备份服务器用户名
		     if(s!=0)
			  {
				  if(s<=LEN_IP_NAME)               // 用户名目前只支持16B长度    
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
	     case 		SET_B_USE_SECRIERS_ID:							 			        // 备份服务器拨号密码
		     if(s!=0)
			  {
				  if(s<=LEN_IP_SECRIER)               // 用户密码目前只支持8B长度     
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
	     case 	   SET_B_IP_DNS_ID:											           // 备份服务器IP地址或域名

		  	if(s!=0)
			  {
				  if(s<=LEN_IPorDNS)               //IP或DNS目前只支持32B长度
				  {
				  	  SetBufferValue(&JT808_IP_DNS.B_IPorDNS[0], LEN_IPorDNS, 0);
					  
					  memmove((char *)&JT808_IP_DNS.B_IPorDNS[0], (char *)p, s);
					  if(ValidIP_Port(JT808_IP_DNS.M_IPorDNS)==OK)
					  {
						  sIP.EnableFlag[eFLAG_TCP1] = ENABLE_FLAG;								              //打开备份服务器TCP,UDP连接
	                 //sIP.EnableFlag[eFLAG_UDP1] = ENABLE_FLAG;	
	                 TestMultiCenter = TRUE;		// 应付检测，
	                 DFE.Write(EE_ENABLE_TCP1,1,(uchar*)&sIP.EnableFlag[eFLAG_TCP1]);
	                 //DFE.Write(EE_ENABLE_UDP1,1,(uchar*)&sIP.EnableFlag[eFLAG_UDP1]);
						  DFE.Write(EE_JT_B_IP_DNS, LEN_IPorDNS, (uchar *)&JT808_IP_DNS.B_IPorDNS[0]);
					     t = 80;
					  }
					  else
					  {
					  	  //sIP.EnableFlag[eFLAG_TCP] = 0;								              //关闭主服务器TCP,UDP连接
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
	     case 		SET_SERVER_TCP_PORT_ID:							 			        // 服务器TCP端口
		  	  if(s!=0)
			  {
				  t = 80;
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  if(Dw==0)
				  {
				  	  sIP.EnableFlag[eFLAG_TCP] = 0;								              //关闭主服务器TCP连接
	              	  DFE.Write(EE_ENABLE_TCP,1,(uchar*)&sIP.EnableFlag[eFLAG_TCP]);
				  }
				  else if(Dw==1)
				  {
				  	  sIP.EnableFlag[eFLAG_TCP1] = 0;											 //关闭备份服务器TCP1连接
	                  DFE.Write(EE_ENABLE_TCP1,1,(uchar*)&sIP.EnableFlag[eFLAG_TCP1]);				  
				  }
              else if(Dw==2)
				  {
				  	  sIP.EnableFlag[eFLAG_TCP] = ENABLE_FLAG;								              //打开主服务器TCP连接
	              DFE.Write(EE_ENABLE_TCP,1,(uchar*)&sIP.EnableFlag[eFLAG_TCP]);
				  }
				  else if(Dw==3)
				  {
				  	  sIP.EnableFlag[eFLAG_TCP1] = ENABLE_FLAG;											 //打开备份服务器TCP1连接
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
	     case 	   SET_SERVER_UDP_PORT_ID:											     // 服务器UDP端口
		  	  if(s!=0)
			  {
				  t = 80;
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  if(Dw==0)
				  {
				  	  sIP.EnableFlag[eFLAG_UDP] = 0;								              //关闭主服务器UDP连接
	              DFE.Write(EE_ENABLE_UDP,1,(uchar*)&sIP.EnableFlag[eFLAG_UDP]);
	              
				  }
				  else if(Dw==1)
				  {
				  	  sIP.EnableFlag[eFLAG_UDP1] = 0;											 //关闭备份服务器UDP1连接
				  	  DFE.Write(EE_ENABLE_UDP1,1,(uchar*)&sIP.EnableFlag[eFLAG_UDP1]);
				  }
				  else if(Dw==2)
				  {
				  	  sIP.EnableFlag[eFLAG_UDP] = ENABLE_FLAG;								              //打开主服务器UDP连接
	              DFE.Write(EE_ENABLE_UDP,1,(uchar*)&sIP.EnableFlag[eFLAG_UDP]);
				  }
				  else if(Dw==3)
				  {
				  	  sIP.EnableFlag[eFLAG_UDP1] = ENABLE_FLAG;											 //打开备份服务器UDP1连接
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

	     case 		SET_POSITION_UP_MODES_ID:			                          // 回报策略
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
	     case 		SET_POSITION_UP_PORJECT_ID:							 			  // 位置回报方案
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
	     case 		SET_POSITION_UP_TIME_NO_DRIVER_ID:							     // 驾驶员未登录回报时间间隔
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

	     case 		SET_POSITION_UP_TIME_IDEL_ID:			                       //休眠时回报时间间隔
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
	     case 	   SET_POSITION_UP_TIME_ALARM_ID:									  //紧急报警时间回报间隔
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
	     case 		SET_POSITION_UP_TIME_DEFAULT_ID:									  //缺省定时回报间隔
		     if(s!=0)
			  {
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  if(Dw!=0)
				  {
				     SendInterval.DefauletTime = Dw;
					  GSM.CalculateSendInterval(0);
				     DFE.Write(EE_JT_SEND_TIME_DEFAULET, 4,(uchar *)&Dw);

					   Uart.puts("设置缺省定时回报：");
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
	     case 		SET_POSITION_UP_DISTANCE_DEFAULT_ID:		                 //缺省定距回报间隔
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
	     case 		SET_POSITION_UP_DISTANCE_NO_DRIVER_ID:							  // 驾驶员未登录回报距离间隔
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
	     case 		SET_POSITION_UP_DISTANCE_IDEL_ID:				 			     //休眠时回报距离间隔
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
	     case 	   SET_POSITION_UP_DISTANCE_ALARM_ID:							     //紧急报警回报距离间隔
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
	     case 		SET_BREAK_POINT_ANGLE_ID:										     //拐角补传角度
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

	     case 		SET_PHONE_MONITOR_CENTER_ID:	                             //监控平台号码
		  	  if(s!=0)
			  {
				  if(s<=LEN_NUMBER)               // 监控中心号码目前只支持16B长度     
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
	     case 		SET_PHONE_RESET_ID:									              //复位号码
		  	  if(s!=0)
			  {
				  if(s<=LEN_NUMBER)               // 复位电话号码目前只支持16B长度     
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
	     case 		SET_PHONE_RESET_FACTORY_ID:									     //恢复出厂设置号码
		  	  if(s!=0)
			  {
				  if(s<=LEN_NUMBER)               // 恢复出厂设置电话号码目前只支持16B长度     
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
	     case 		SET_PHONE_SMS_MONITOR_CENTER_ID:								     //监控平台SMS号码
		  	  if(s!=0)
			  {
				  if(s<=LEN_NUMBER)               // 电话号码目前只支持16B长度     
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
	     case 		SET_PHONE_TEXT_SMS_ALARM_ID:									     //接收文本SMS报警号码
		  	  if(s!=0)
			  {
				  if(s<=LEN_NUMBER)               // 电话号码目前只支持16B长度     
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
	     case 		SET_PHONE_GET_MODES_ID:											     //电话接通策略
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
	     case 		SET_PHONE_ONLINE_TIME_TIMES_LIMIT_ID:						     //每次最长通话时间
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
	     case 		SET_PHONE_ONLINE_TIME_MONTH_LIMIT_ID:						     //当月最长允许通话时长
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
	     case 		SET_PHONE_MONITORING_ID:											  //监听电话号码
		  	  if(s!=0)
			  {
				  if(s<=LEN_NUMBER)               // 电话号码目前只支持16B长度     
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
	     case 		SET_PHONE_PRIVILEGE_SMS_ID:									     //特权短信号码
		  	  if(s!=0)
			  {
				  if(s<=LEN_NUMBER)               // 电话号码目前只支持16B长度     
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
	     case 		SET_WORD_FOR_NO_ALARM_ID:	                                //报警屏蔽字
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
	     case 		SET_WORD_FOR_TEST_SMS_ALARM_ID:								     //报警发送文本SMS开关
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
	     case 		SET_WORD_FOR_TAKE_PHOTO_ALARM_ID:								  //报警拍照开关
		  	  
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
	     case 		SET_WORD_FOR_TAKE_PHOTO_SAVE_ALARM_ID:						     //报警拍照保存开关
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
	     case 		SET_WORD_FOR_KEY_ALARM_ID:										     //关键报警标志
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
	     case 		SET_SPEED_HITH_LIMIT_ID:											  //最高速度
		  	  if(s!=0)
			  {
			  	  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  if(Dw>500)	    Dw = 250;				  
				  DFE.Write(FM_HIGH_SPEED_LIMIT, 1,(uchar *)&Dw);		
				  // 即时更新数据													 
				  SpeedC.SetGetPara( eSPEED_CMD_GET_OVER_SPEED,LEN_OVER_SPEED_PARA,Tp.Buffer);
				  Tp.Buffer[0] = TRUE;
				  Tp.Buffer[1] = Dw;
				  SpeedC.SetGetPara( eSPEED_CMD_SET_OVER_SPEED,LEN_OVER_SPEED_PARA,Tp.Buffer);
				  Uart.puts("设置最高速度：");
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
	     case 		SET_TIME_OVER_LIMIT_SPEED_ID:									     //超速持续时间
		  	  if(s!=0)
			  {
			  	//	ClearOverSpeedCntFlag = 0xA5;
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  if(Dw>65535)	Dw = 65535;
				  DFE.Write(FM_OVP_ENABLE_TIME, 2,(uchar *)&Dw);	
				  // 即时更新数据													 
				  SpeedC.SetGetPara( eSPEED_CMD_GET_OVER_SPEED,LEN_OVER_SPEED_PARA,Tp.Buffer);
				  Tp.Buffer[0] = TRUE;
				  Tp.B16[2] = Dw;
				  SpeedC.SetGetPara( eSPEED_CMD_SET_OVER_SPEED,LEN_OVER_SPEED_PARA,Tp.Buffer);
				  Uart.puts("设置超速允许时间：");
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
	     case 		SET_TIME_DRIVEING_LIMIT_ID:										  //持续驾驶时间门限
		  	  if(s!=0)
			  {			  		
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  DFE.Write(FM_DRIVEING_TIME, 4,(uchar *)&Dw);	
				  // 即时更新数据													 
				  SpeedC.SetGetPara( eSPEED_CMD_GET_DRIVING_TIME,LEN_DRIVING_TIME_LIMIT,Tp.Buffer);
				  Tp.B32[0] = Dw;
				  Tp.Buffer[18] = TRUE;
				  SpeedC.SetGetPara( eSPEED_CMD_SET_DRIVING_TIME,LEN_DRIVING_TIME_LIMIT,Tp.Buffer);
				  Uart.puts("设置持续驾驶时间上限：");
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
	     case 		SET_TIME_DRIVEING_LIMIT_ONE_DAY_ID:							     //一天累计驾驶时间门限
		  	  if(s!=0)
			  {
			  		Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
			  		DFE.Write(FM_DRIVE_ONE_DAY_LIMINT, 4,(uchar *)&Dw);	
			  		// 即时更新数据													 
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

	     case 		SET_TIME_REST_ID:													     //最小休息时候
		  	  if(s!=0)
			  {			  		
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  DFE.Write(FM_REST_TIME, 4,(uchar *)&Dw);	
				  // 即时更新数据													 
				  SpeedC.SetGetPara( eSPEED_CMD_GET_DRIVING_TIME,LEN_DRIVING_TIME_LIMIT,Tp.Buffer);
				  Tp.B32[1] = Dw;
				  Tp.Buffer[18] = TRUE;
				  SpeedC.SetGetPara( eSPEED_CMD_SET_DRIVING_TIME,LEN_DRIVING_TIME_LIMIT,Tp.Buffer);
				  Uart.puts("设置最小休息时间：");
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
	     case 		SET_TIME_STOP_CAR_ID:												  //最长停车时间
		  	  if(s!=0)
			  {			  		
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  DFE.Write(FM_PARKING_TIME_LIMINT, 4,(uchar *)&Dw);	
				  // 即时更新数据													 
				  SpeedC.SetGetPara( eSPEED_CMD_GET_DRIVING_TIME,LEN_DRIVING_TIME_LIMIT,Tp.Buffer);
				  Tp.B32[3] = Dw;
				  Tp.Buffer[19] = TRUE;
				  SpeedC.SetGetPara( eSPEED_CMD_SET_DRIVING_TIME,LEN_DRIVING_TIME_LIMIT,Tp.Buffer);
				  Uart.puts("设置最长停车时间：");
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
	     case 		SET_VIDIO_QUOLITY_ID:	                                   //图像/照片质量
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
	     case 		SET_VIDIO_BRIGHTNESS_ID:										     //亮度
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
	     case 		SET_VIDIO_CONTRAST_ID:										        //对比度
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
	     case 		SET_VIDIO_SATULATION_ID:										     //饱和度
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
	     case 		SET_VIDIO_CHROMA_ID:											        //色度
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
	     case 		SET_ODOMETER_VALUS_ID:	                                   //里程表读数
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
	     case 		SET_PROVINCIAL_ID:													  //省域
		  	  if(s!=0)
			  {
			    DFE.Write(EE_PROVINCE_REGION_ID, 1, p+1); 
				NOP(); NOP(); NOP();
				DFE.Write(EE_PROVINCE_REGION_ID+1, 1, p); 
				NOP(); NOP(); NOP();	 
			  }
			  else
		      {  
				  DFE.Read(EE_PROVINCE_REGION_ID, 2,(uchar *)&Dw);		// 存储是小端数据，发到服务器是大端数据
				  Dw = SwapINT16(Dw);
				  t = 2;
				  memmove(p, (uchar *)&Dw, t);
			  }
		   	break;
	     case 		SET_CITY_WHERE_ID:													  //市域
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
	     case 		SET_CAR_NUBER_ID:													     //车牌号码
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
	     case 		SET_CAR_COLOUR_ID:													  //车牌颜色
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
		case SET_SERVER_IC_AUTH_MAIN_IP:												// 道路运输证IC 卡认证主服务器IP地址或域名
			if(s!=0)
			 {
				  if(s<=LEN_IPorDNS)               //IP或DNS目前只支持32B长度
				  {
				  	  SetBufferValue(&JT808_IP_DNS.IC_Auth_M_IPorDNS[0], LEN_IPorDNS, 0);
					  memmove((char *)&JT808_IP_DNS.IC_Auth_M_IPorDNS[0], (char *)p, s);
					  if(ValidIP_Port(JT808_IP_DNS.IC_Auth_M_IPorDNS)==OK)
					  {
						  sIP.EnableFlag[eFLAG_IC_AUTH_TCP] = ENABLE_FLAG;								              //打开主服务器TCP,UDP连接
	                 	  //sIP.EnableFlag[eFLAG_UDP] = ENABLE_FLAG;	
	                      DFE.Write(EE_ENABLE_IC_AUTH_TCP,1,(uchar*)&sIP.EnableFlag[eFLAG_IC_AUTH_TCP]);
	                     //DFE.Write(EE_ENABLE_UDP,1,(uchar*)&sIP.EnableFlag[eFLAG_UDP]);
						  DFE.Write(EE_JT_IC_AUTH_M_IP_DNS, LEN_IPorDNS, (uchar *)&JT808_IP_DNS.IC_Auth_M_IPorDNS[0]);
						  t = 80;
						  	Uart.puts("设置IC卡认证IP地址：");
					 		Int2Str( Dw, (char*)Tp.Buffer);
					 		Uart.puts((char *)&JT808_IP_DNS.IC_Auth_M_IPorDNS[0]);

							U2PinSelDevice = U2_FOR_RFID;
							os_dly_wait(100);
							Uart2.Init( 9600, U2_FOR_RFID);

					  }
					  else
					  {
					  	  //sIP.EnableFlag[eFLAG_TCP] = 0;								              //关闭主服务器TCP,UDP连接
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

		case	SET_SERVER_IC_AUTH_TCP:											    				// 道路运输证IC 卡认证主服务器TCP端口
			  if(s!=0)
			  {
				  t = 80;
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  if(Dw==0)
				  {
				  	  sIP.EnableFlag[eFLAG_IC_AUTH_TCP] = 0;								              //关闭IC卡认证主服务器TCP连接
	              	  DFE.Write(EE_ENABLE_IC_AUTH_TCP,1,(uchar*)&sIP.EnableFlag[eFLAG_IC_AUTH_TCP]);
				  }
				  else if(Dw==1)
				  {
				  	  sIP.EnableFlag[eFLAG_IC_AUTH_B_TCP] = 0;											 //关闭IC卡认证备份服务器TCP1连接
	                  DFE.Write(EE_ENABLE_IC_AUTH_B_TCP,1,(uchar*)&sIP.EnableFlag[eFLAG_IC_AUTH_B_TCP]);				  
				  }
                  else if(Dw==2)
				  {
				  	  sIP.EnableFlag[eFLAG_IC_AUTH_TCP] = ENABLE_FLAG;								              //打开IC卡认证主服务器TCP连接
	                  DFE.Write(EE_ENABLE_IC_AUTH_TCP,1,(uchar*)&sIP.EnableFlag[eFLAG_IC_AUTH_TCP]);
				  }
				  else if(Dw==3)
				  {
				  	  sIP.EnableFlag[eFLAG_IC_AUTH_B_TCP] = ENABLE_FLAG;											 //打开IC卡认证主备份服务器TCP1连接
	                  DFE.Write(EE_ENABLE_IC_AUTH_B_TCP,1,(uchar*)&sIP.EnableFlag[eFLAG_IC_AUTH_B_TCP]);				  
				  }
				  else if(Dw<65535)
				  {
				  	 JT808_IP_DNS.IC_Auth_TCP_Port = Dw;
				    DFE.Write(EE_JT_IC_AUTH_TCP_PORT, 4,(uchar *)&Dw);
					 Uart.puts("设置IC卡认证端口：");
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

		case	SET_SERVER_IC_AUTH_UDP:											    				// 道路运输证IC 卡认证主服务器UDP端口
			  if(s!=0)
			  {
				  t = 80;
				  Dw = ByteToINT32(*p, *(p+1),*(p+2),*(p+3));
				  if(Dw==0)
				  {
				  	  sIP.EnableFlag[eFLAG_IC_AUTH_UDP] = 0;								              //关闭IC卡认证主服务器UDP连接
	              	  DFE.Write(EE_ENABLE_IC_AUTH_UDP,1,(uchar*)&sIP.EnableFlag[eFLAG_IC_AUTH_UDP]);
	              
				  }
				  else if(Dw==1)
				  {
				  	  sIP.EnableFlag[eFLAG_IC_AUTH_B_UDP] = 0;											 //关闭IC卡认证备份服务器UDP1连接
				  	  DFE.Write(EE_ENABLE_IC_AUTH_B_UDP,1,(uchar*)&sIP.EnableFlag[eFLAG_IC_AUTH_B_UDP]);
				  }
				  else if(Dw==2)
				  {
				  	  sIP.EnableFlag[eFLAG_IC_AUTH_UDP] = ENABLE_FLAG;								              //打开IC卡认证主主服务器UDP连接
	                  DFE.Write(EE_ENABLE_IC_AUTH_UDP,1,(uchar*)&sIP.EnableFlag[eFLAG_IC_AUTH_UDP]);
				  }
				  else if(Dw==3)
				  {
				  	  sIP.EnableFlag[eFLAG_IC_AUTH_B_UDP] = ENABLE_FLAG;											 //打开IC卡认证主备份服务器UDP1连接
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

		case	SET_SERVER_IC_AUTH_BAK_IP:									// 道路运输证IC 卡认证备份服务器IP地址或域名，端口同主服务器
			if(s!=0)
			{			
				  if(s<=LEN_IPorDNS)               //IP或DNS目前只支持32B长度
				  {
				  	  SetBufferValue(&JT808_IP_DNS.IC_Auth_B_IPorDNS[0], LEN_IPorDNS, 0);
					  memmove((char *)&JT808_IP_DNS.IC_Auth_B_IPorDNS[0], (char *)p, s);
					  if(ValidIP_Port(JT808_IP_DNS.IC_Auth_B_IPorDNS)==OK)
					  {
						  sIP.EnableFlag[eFLAG_IC_AUTH_B_TCP] = ENABLE_FLAG;								   //打开备份服务器TCP,UDP连接
	                 	  //sIP.EnableFlag[eFLAG_UDP1] = ENABLE_FLAG;	
	                	  DFE.Write(EE_ENABLE_IC_AUTH_B_TCP,1,(uchar*)&sIP.EnableFlag[eFLAG_IC_AUTH_B_TCP]);
	                	  //DFE.Write(EE_ENABLE_UDP1,1,(uchar*)&sIP.EnableFlag[eFLAG_UDP1]);
						  DFE.Write(EE_JT_IC_AUTH_B_IP_DNS, LEN_IPorDNS, (uchar *)&JT808_IP_DNS.IC_Auth_B_IPorDNS[0]);
					     t = 80;
					  }
					  else
					  {
					  	  //sIP.EnableFlag[eFLAG_TCP] = 0;								              //关闭主服务器TCP,UDP连接
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

		case	SET_ELE_RAID_RADIUS_ID:			   // 电子围栏半径	（非法位移阈值） ，单位为米
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


		case	SET_OVER_SPEED_PRE_ALARM_ID:	 // 超速报警预警差值
			if(s!=0)
			{
				Dw = ByteToINT16(*p, *(p+1));
			   DFE.Write(EE_JT_OVER_SPEED_PRE_ALARM, 2, (uchar *)&Dw); 	
			    // 即时更新数据													 
				  SpeedC.SetGetPara( eSPEED_CMD_GET_OVER_SPEED,LEN_OVER_SPEED_PARA,Tp.Buffer);
				  Tp.Buffer[3] = Dw/10;		// 不取小数
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

		case	SET_TRIAL_FATIGUE_DRIVE_PRE_ALARM_ID:							 // 疲劳驾驶预警差值
			if(s!=0)
			{
				Dw = ByteToINT16(*p, *(p+1));
			    DFE.Write(EE_JT_FATIGUE_DRIVE_PRE_ALARM, 2, (uchar *)&Dw); 
			    // 即时更新数据													 
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

		case	SET_CRASH_ALARM_PARA_ID:									 // 碰撞报警参数设置			  
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
	

		case	SET_TILT_ALARM_PARA_ID:									 // 侧翻报警参数设置			  
			if(s!=0)
			{
				Dw = ByteToINT16(*p, *(p+1));
			   	DFE.Write(EE_JT_TILT_ALARM_PARA, 2, (uchar *)&Dw); 	
				NOP(); NOP(); NOP(); 
				DFE.Write(EE_ACCEL_TILT_PARA + 3, 1, p); 		 // 角度值	注意 只写一个字节
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
		
		case	 SET_FIXED_TIME_TAKE_PHOTO_CTL_ID:						 // 定时拍照控制	

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

		case	 SET_DISTANCE_TAKE_PHOTO_CTL_ID:						 // 定距拍照控制	

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

		case SET_GNNS_POS_MODE_ID:									// GNSS 定位模式
			if(s!=0)
			{				
			   DFE.Write(EE_JT_GNNS_POS_MODE, 1, p);
				JT808Flag.VS3.Bits.UseBD = (*p >> 1) & 0x01;
				JT808Flag.VS3.Bits.UseGPS = (*p >> 0) & 0x01;	
				////// 应付测试，精度测试使用
				#if( TEST_CODE_JUST_FOR_TEST_LUOYANG_NEW==1 )
				TestAccuracy = TRUE;		

				if( (JT808Flag.VS3.Byte&0x0c)==0x0c)
				{
					Uart.puts("远程设置双模定位模式 ");
				}
				else if(JT808Flag.VS3.Bits.UseGPS==1)
				{
					Uart.puts("远程设置单GPS定位模式 ");
				}
				else if(JT808Flag.VS3.Bits.UseBD==1)
				{
					Uart.puts("远程设置单北斗定位模式 ");
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
		case	SET_GNSS_BAUD_RATE_ID:						  			// GNSS波特率
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
		case	SET_GNSS_DATA_OUTPUT_FREQUENCY_ID:							// GNSS模块详细定位数据输出频率
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
		case	SET_GNSS_DATA_COLLECT_FREQUENCY_ID:							// GNSS模块详细定位数据采集频率
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

		case	SET_GNSS_DATA_UPLOAD_MODE_ID:							    // GNSS模块详细定位数据上传方式
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
			Uart.puts("远程设置采集卫星原始数据的模式 ");
			break;
		case	SET_GNSS_DATA_UPLOAD_SET_ID:							    // GNSS模块详细定位数据上传设置
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
			Uart.puts("远程设置采集卫星原始数据的间隔 ");
			break;	
	
		case	SET_CAN_CHAN_1_COLLECT_TIME_INTERVAL_ID:		         // CAN总线通道1 采集时间间隔(ms)，0表示不采集
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

		case	SET_CAN_CHAN_1_UPLOAD_TIME_INTERVAL_ID:			        	// CAN总线通道1 上传时间间隔(s)，0表示不采集
			if(s!=0)
			{
				  Dw = ByteToINT16(*p, *(p+1));
				  DFE.Write(EE_JT_CAN_CHAN_1_UPLOAD_TIME_INTERVAL, 2,(uchar *)&Dw);	
				  JT_CanSet.Can1UploadInterval = Dw;
				  Uart.puts("远程设备CAN通道1上传间隔：");
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

		case SET_CAN_CHAN_2_COLLECT_TIME_INTERVAL_ID:         			// CAN总线通道2 采集时间间隔(ms)，0表示不采集
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

		case	SET_CAN_CHAN_2_UPLOAD_TIME_INTERVAL_ID:			        	// CAN总线通道2 上传时间间隔(s)，0表示不采集
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

		 	///////////// CAN ID，暂时只处理16个
		   if ((CmdID>=SET_CAN_ID_COLLECT_SET_0_ID)	&& (CmdID<=SET_CAN_ID_COLLECT_SET_15_ID))	  
		   {
		   	s1 =  CmdID - SET_CAN_ID_COLLECT_SET_0_ID;		// 偏移
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
			strcpy( (char*)Tp.Buffer,"\r\n远程读取行驶记录仪数据  ");
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
		strcpy( (char*)Tp.Buffer,"\r\n远程读取行驶记录仪数据  ");
		ByteToHexChar( GetVDR_Cmd,(char*)&Tp.Buffer[26]);		
		Uart.puts((char*)Tp.Buffer);
		
	}

}


// 08H - 15H 记录仪数据采集
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
		// 获取数据总长
		GB_LargeData.SearchFlag = 0;
		TotalItem = GB_LargeData.TotalItem;	 
		L = 1;
		JT_VehRecord.TotalPack = 0;
		while (L)
		{
			L = Uart.SendVDR_Data_08H_To_15H(1024,pData+6);
			if (L)  
			{
				Size += L;	//  数据长度不为0
				JT_VehRecord.TotalPack++;
			}
			else	  break;	
		}

		//  取第一包数据
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
//		//Size += 3 + 6 + 1;	 // 3个字节为命令字和平台流水号，6个字节为GB19056的头部，1个字节为校验码
//		//JT_VehRecord.TotalPack = Size / Len;
//		//if ((Size % Len) || (JT_VehRecord.TotalPack==0))  JT_VehRecord.TotalPack += 1;
//		JT_VehRecord.PackIndex = 1;	 ///////////// 从1开始
//	}
//	else
//	{
//		JT_VehRecord.PackIndex++; 
//	}

	pData -= 3;  // 指回	 TxdTBuffer
	Len += 3;

	Temp =  (JT_VehRecord.PackIndex << 16) | JT_VehRecord.TotalPack;
	//if (JT_VehRecord.TotalPack==1) Temp = 0;
	 Temp = 0;	  // 用锐明的平台，不写总包，都是一包一包的传，所以这里设置为0
	Len = JT808_Packaging(eBB_gCMD_TER_VDR_DATA_UPLOAD,Len,Temp,pData,(uchar*)pBuffer);

	

	Uart.puts("包序 Test Data:");	Uart.putb(1,2,(uchar *)&JT_VehRecord.PackIndex); Uart.puts("\r\n");
	Uart.puts("包数 Test Data:");	Uart.putb(1,2,(uchar *)&JT_VehRecord.TotalPack); Uart.puts("\r\n");


	os_dly_wait(80); // 延时

	return Len;

}









/**************************************** End of File *************************************************/






