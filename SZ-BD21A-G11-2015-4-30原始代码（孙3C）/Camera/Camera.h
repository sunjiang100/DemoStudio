/***************************************  Copyright (c)  **************************************
**										
**									
**                      
**	  
****************************************  Copyright (c)  **************************************
**                                         �ļ�����
**	�ļ���:   Camera.h
**	������������������ͷ����ͷ�ļ�
**	���뻷����Keil uVision3 + RTX
**	Ŀ��CPU:  LPC2378
**	���ߣ�	   	
**	����ʱ�䣺2009.06.19
**  �޸�ʱ��: 
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


#define		__CAMERA_DEBUG__		0							 // ���ڵ��Եĺ�



#define			JT_CAMERA_CHECK_TIME			 3600                 // �������ͷʱ��	  1Сʱ


CAMERA_EXT	enum
{
	eCAM_TEST = 1,
	eCAM_CHANGE_ID = 0x0d,					// �޸�ID
	eCAM_TAKE_PHOTO_RET_SIZE = 0x30,		// �������գ�����ͼƬ��С
	eCAM_REQ_DATA_PACK = 0x31,				// ������ָ������ͼ������
	eCAM_SET_PACK_SIZE = 0x32,				// ���÷ְ�����İ���С
	eCAM_REQ_PRE_PIC_SIZE = 0x33,			// �����ϴ��������������ͼƬ���ݴ�С
	eCAM_SET_RESOLUTION = 0x34,			//	����Ĭ�ϵ�ͼ��ֱ���	
	
}ENUM_CAMERA_CMD;


#pragma		pack(1)
typedef		struct
{
	uint32	TotalNumber;			// ͼƬ���������Ӱ�װ��ʼ����ǰʱ�䣬ʵ������64ȡģ��
	uchar		Resolution;				// �ֱ���
	uchar		DuiBiDu;					// 
	uchar		Lingth;					//
	uchar		IR;						// ����
	uint32	DelPicNo;				// ɾ��
	uint32	SendPicNo;				// ���͵�ͼƬ���
	uint16	SendPackNo;				// ���͵����ݰ����	
	uint16	TakeNumber;				// ��Ҫ�����ͼƬ����
	uint16	SendNumber;				// �跢�͵�ͼƬ����,0������	
	uchar		SaveAndSend;			// ��ʱ����洢��ʱͬ����
	uchar		TakeStart;				// 
	uchar		AddPackHead;			// ����*GY��ͷ�İ�ͷ
	uchar		CameraID;
} PIC_STRUCT,*pPIC_STRUCT;

#define		LEN_SEND_PIC_HEAD			36
#define		SAVE_PIC_HEAD_FLAG		0x4745504A
typedef		struct
{
	uint32	HeadFlag;				// 0x4745504A		JPEG	4A 50 45 47 
	uint32	TotalNumber;			// ��ͼƬ����ͼƬID��
	uint32	StartTimeSec;			// ����ʱ��
	uint32	TotalPage;				// ����ͼƬ�洢��ҳ����256B/Page��		
	uint32	MediaId;					// ��ý��ID
	uchar		MediaType;				// ���ͣ�0��ͼƬ��1����Ƶ�� 2����Ƶ
	uchar		MediaFormat;			// ��ʽ
	uchar		MediaEvent;				// �¼���0��ƽָ̨��
	uchar		MediaChannel;			// ͨ��
	uchar		LocationInfo[LEN_JT808_GPS_PACK];
} SAVE_PIC_HEAD,*pSAVE_PIC_HEAD;



/**************** Camera�� ****************************/
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

	uchar		NCameraID[8];		// ���8������ͷ
	
private:	
	uint32	TickCnt;
	uchar		CameraPowerOn,ForcePowerOn,TestErrorCnt;
	uint32	PicPackNum;
	

	//uchar		NCameraID[8];		// ���8������ͷ

	PIC_STRUCT	Pic;
	uint32	TimeToPowerOff;
		
};


CAMERA_EXT		__noinit__		CameraCls		Camera;		

#endif

/******************************* End of File *************************************/













