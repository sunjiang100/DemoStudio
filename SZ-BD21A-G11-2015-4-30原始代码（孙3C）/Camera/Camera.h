/***************************************  Copyright (c)  **************************************
**										
**									
**                      
**	  
****************************************  Copyright (c)  **************************************
**                                         文件描述
**	文件名:   Camera.h
**	功能描述：串口摄像头驱动头文件
**	编译环境：Keil uVision3 + RTX
**	目标CPU:  LPC2378
**	作者：	   	
**	创建时间：2009.06.19
**  修改时间: 
*************************************************************************************************/
#ifndef			__CAMERA_H__
#define			__CAMERA_H__

#ifdef      	CAMERA_GLOBALS
#define     	CAMERA_EXT
#else
#define     	CAMERA_EXT    extern
#endif

#include			"..\UART\UART2.h"
#include			"..\UART\UART0.h"

#include			"..\FLASH\DFE_25xxx.H"
#include			"..\FLASH\DFF_25xxx.H"
#include			"..\GPS\GPS.h"


#define		__CAMERA_DEBUG__		0							 // 用于调试的宏



#define			JT_CAMERA_CHECK_TIME			 3600                 // 检查摄像头时间	  1小时


CAMERA_EXT	enum
{
	eCAM_TEST = 1,
	eCAM_CHANGE_ID = 0x0d,					// 修改ID
	eCAM_TAKE_PHOTO_RET_SIZE = 0x30,		// 请求拍照，返回图片大小
	eCAM_REQ_DATA_PACK = 0x31,				// 请求传输指定包的图像数据
	eCAM_SET_PACK_SIZE = 0x32,				// 设置分包传输的包大小
	eCAM_REQ_PRE_PIC_SIZE = 0x33,			// 报告上次拍照命令拍摄的图片数据大小
	eCAM_SET_RESOLUTION = 0x34,			//	设置默认的图象分辨率	
	
}ENUM_CAMERA_CMD;


#pragma		pack(1)
typedef		struct
{
	uint32	TotalNumber;			// 图片总张数（从安装开始开当前时间，实际张数64取模）
	uchar		Resolution;				// 分辨率
	uchar		DuiBiDu;					// 
	uchar		Lingth;					//
	uchar		IR;						// 红外
	uint32	DelPicNo;				// 删除
	uint32	SendPicNo;				// 发送的图片序号
	uint16	SendPackNo;				// 发送的数据包序号	
	uint16	TakeNumber;				// 需要拍摄的图片张数
	uint16	SendNumber;				// 需发送的图片张数,0不发送	
	uchar		SaveAndSend;			// 即时拍摄存储的时同发送
	uchar		TakeStart;				// 
	uchar		AddPackHead;			// 增加*GY开头的包头
	uchar		CameraID;
} PIC_STRUCT,*pPIC_STRUCT;

#define		LEN_SEND_PIC_HEAD			36
#define		SAVE_PIC_HEAD_FLAG		0x4745504A
typedef		struct
{
	uint32	HeadFlag;				// 0x4745504A		JPEG	4A 50 45 47 
	uint32	TotalNumber;			// 总图片数（图片ID）
	uint32	StartTimeSec;			// 拍照时间
	uint32	TotalPage;				// 该张图片存储总页数（256B/Page）		
	uint32	MediaId;					// 多媒体ID
	uchar		MediaType;				// 类型，0：图片；1：音频； 2：视频
	uchar		MediaFormat;			// 格式
	uchar		MediaEvent;				// 事件，0：平台指令
	uchar		MediaChannel;			// 通道
	uchar		LocationInfo[LEN_JT808_GPS_PACK];
} SAVE_PIC_HEAD,*pSAVE_PIC_HEAD;



/**************** Camera类 ****************************/
class			CameraCls
{
public:
	CameraCls(void);
	void		eTask(void);
	char 		Test(uchar id);
	void		Init(); 
	void		SetPower(uchar);		
	uint16	SetCommand(uchar ID,uchar Cmd,uint16 Length,uint32 Data,uchar *pData);	

	void		ReadPhotoDataFromDF(uint32 Number,uint32 PicNo,uint32 PackNo);
	uchar		DeletePhotoDataFromDF(uchar    PicNo);
	void		PictureDataToGPRS(uchar PicNo,uchar TotalPack,uchar PackNo,uchar *pPic);
	void		JT_PictureDataToGPRS(uint16 Length,uint16 TotalPack,uint16 PackNo,uchar *pPic);
	void		CheckCamera(void);
	
	void		SetOperateParameter(uchar s,PIC_STRUCT *pPic);
	void		GetOperateParameter(uchar s,PIC_STRUCT *pPic);
	void		RestoreFactoryParameter(void);
	void		SetTakeNumber(uchar Cnt,uchar Head);

	uchar		NCameraID[8];		// 最多8个摄像头
	
private:	
	uint32	TickCnt;
	uchar		CameraPowerOn,ForcePowerOn,TestErrorCnt;
	uint32	PicPackNum;
	

	//uchar		NCameraID[8];		// 最多8个摄像头

	PIC_STRUCT	Pic;
	uint32	TimeToPowerOff;
		
};


CAMERA_EXT		__noinit__		CameraCls		Camera;		

#endif

/******************************* End of File *************************************/













