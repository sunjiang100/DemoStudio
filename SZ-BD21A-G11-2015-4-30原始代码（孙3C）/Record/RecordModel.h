
#ifndef		__RECORD_MODEL_H__
#define		__RECORD_MODEL_H__


#ifdef		RECORD_MODEL_GLOBALS
#define		RECORD_EXT
#else
#define		RECORD_EXT	extern
#endif

#include		"..\Config\RTOS.h"	




#define			LEN_RECORD_SEND_BUFFER		   64

#define			RECORD_ALL_PACK_UPLOAD		  1
#define			RECORD_SOME_PACK_UPLOAD		  2


#pragma			pack(1)
typedef	struct	
{
	char	File[16];			// ��Ƶ�ļ���
	uint16  PackIndex;			// ���ݰ�����  // ��1��ʼ
	
} TYPE_UP_PACK_RECORD_DATA;
#pragma		    pack()


RECORD_EXT  enum	
{
	// ¼��ģ���ϴ���������
   	UP_CMD_RECORD_ON			=	0x0001,
	UP_CMD_RECORD_OFF			=	0x0002,
	UP_CMD_MP3_PLAY				=	0x0003,
	UP_CMD_MP3_STOP				=	0x0004,
	UP_CMD_RECORD_UPLOAD 		=   0x0005,			 // �����ϴ�������Ƶ�ļ�������
	UP_CMD_CHECK_ERROR  		=   0x0006,   		 //  У�����������
	UP_CMD_RECORD_PACK_DATA	    =   0x0007,			 // ������Ƶ�ְ�����
	UP_CMD_GET_TOTAL_PACK		= 	0x0009,   		//  �ܰ���

	// ��λ���·���������	
	DOWN_CMD_RECORD_ON			=	0x0081,
	DOWN_CMD_RECORD_OFF			=	0x0082,
	DOWN_CMD_MP3_PLAY			=	0x0083,
	DOWN_CMD_MP3_STOP			=	0x0084,
	DOWN_CMD_RECORD_UPLOAD 		=   0x0085,  
	DOWN_CMD_CHECK_ERROR 		=   0x0086,   // ¼��ģ���ϴ�������У����� 
	DOWN_CMD_RECORD_PACK_DATA	=   0x0087,	  // ����¼���ְ�����
	DOWN_CMD_SET_PACK_SIZE		=   0x0088,	  // ���÷ְ���С(���1024��,ȱʡ512
	DOWN_CMD_GET_TOTAL_PACK		= 	0x0089,   // ȡ���ܰ���
} EnumCmdRecordToUpperDevice;  // ¼��ģ������λ��ͨ�ŵ�������ö��


/**************** Record�� ****************************/
class		RecordCls
{
public:
// added 2013.7
	RecordCls(void);
	void 		Init(void);
	void        RecordTask(void);
	void		Ctrl_Record(uint16 Len, uchar *pData);
	void 		SendDataToRecord(uint16 Len, uchar *pData);
//	void 		ReceiveProcessData(void);
	void	    StartRecord(uint16 Time);
	void	    StopRecord(void);
	void	    SendCmdToRecord(uint16 Cmd, uint16 DataLen, uchar *pData);
	void  	UploadRecordData(uchar UpType);
	uchar  	UploadPackRecordData(char *pFile, uint16 PackIndex, uchar UpType);	
	uchar   	WaitForRecordData(uint16 *pDataLen, uchar *pData, uint16  *pTotalPack, uint16 *pPackIndex);
	void		CounterFun(void);

private:

	uchar 	Record_On, Record_Off, RecordDataReady;
	uchar 	UploadDataToCenerFlag;
    uchar 	*RecordBuffer;
	uchar  	SendBuffer[LEN_RECORD_SEND_BUFFER];
	uint16  RecordTotalPack,RecordTime;
	uint32	AudioId;
	uchar  Init_Rs232;


};


RECORD_EXT	 __noinit__  RecordCls	Record;




















#endif // end of __RECORD_MODEL_H__



/******************************* End of File *************************************/
