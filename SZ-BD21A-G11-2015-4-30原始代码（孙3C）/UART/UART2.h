/***************************************  Copyright (c)  **************************************
**									 
**									
**                      
**	  
****************************************  Copyright (c)  **************************************
**                                         �ļ�����
**	�ļ���:   Uart2.H
**	����������UART2�ײ�����ͷ�ļ�.
**	���뻷����Keil for ARM uVision3
**	Ŀ��CPU:  LPC2378
**	���ߣ�	 
**	����ʱ�䣺
**  �޸�ʱ��: 2009��05��18��
**	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�	
*************************************************************************************************/
			  
#ifndef			__UART2_H__
#define			__UART2_H__

#ifdef      	UART2_GLOBALS
#define     	UART2_EXT
#else
#define     	UART2_EXT    extern
#endif

#include			<string.h>
#include    	<stdio.h>
#include			"..\Config\RTOS.h"
#include 		"..\COMMON\CppGlobals.h"


#define			FIFO_LENGTH				14
#define			TRIGGER_WITH_FIFO		1

// $$$$$$$$$$$$$$$$$$ <<< Use Configuration Wizard in Context Menu >>> $$$$$$$$$$$$$$$$$
// <h>��ѡ���Ƿ�ʹ��UART����ʹ����ռ��Դ
// <h>UART0��ʼ���������(CPU��Ƶ��CPU.h�ļ�������)
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>UART0_BPS: ���ڲ��������� <1-250000>
//   	<i> Default: 9600
*/
#define      	UART2_BPS    			57600//19200

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>U0_DATA_BIT: ����λ���� <0=> 5λ <1=> 6λ <2=> 7λ <3=> 8λ
//   	<i> Default: 3
*/
#define     	 U2_DATA_BIT    		3

//	<e>��ֹ��ʹ��У��λ
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>U0_CHECK_BIT_EN: У��λʹ�� <0=> ��ֹУ�� <1=> ����У��
//   	<i> Default: 1
*/
#define     	 U2_CHECK_BIT_EN  	0

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>U0_CHECK_BIT: У��λ���� <0=> ��У�� <1=> żУ�� <2=> ǿ��1 <3=> ǿ��0
//   	<i> Default: 0
*/
#define      	 U2_CHECK_BIT    		0

//	</e>
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>U0_STOP_BIT: ֹͣλ���� <0=> 1λ <1=> 2λ 
//   	<i> Default: 0
*/
#define      	U2_STOP_BIT    			0

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>U0_PROIRITY: �����ж����ȼ� <0=> ������ <1=> ����
//   	<i> Default: 1
*/
#define			U2_PROIRITY    			1

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>UART0_REC_LENGTH: ���ջ����������ֽ����� <1-1024>
//   	<i> Default: 100
*/
#define			UART2_REC_LENGTH		640

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>UART0_TXD_LENGTH: ���ͻ����������ֽ����� <1-1024>
//   	<i> Default: 100
*/
#define			UART2_TXD_LENGTH		512

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>USE_QUERY_SEND: ʹ�ò�ѯ���ͺ��� <0=> ��ʹ�� <1=> ʹ��
//   	<i> Default: 0
*/
#define			USE_QUERY_SEND			0
		   	
// </h>

// </h>

// $$$$$$$$$$$$$$$$$$$ <<< end of configuration section >>> $$$$$$$$$$$$$$$$$$$$$$$$$$$$



/******************* UART�� ****************************/
class			Uart2Cls
{
public:
	Uart2Cls(void);
   bool		Init(uint32,uchar);
	uchar		puts(char *p);
	uchar		putb(uchar,uint16,uchar *p);
	void		SendTask();	
	inline	void	Interrupt(void);
	void		ClearRecFlag(void);
	uint16	WaitGetRecBuffer(uchar *p);
	void		RecEnable(void);
	uint16	GetRecBuffer(uchar *pData);
	uint16	RFID_ReceiveData(uchar *pData);
	void		ReceiveProcessData(void);
	uint16	ReceiveAudioData(uint16 Len,uchar *pRec, uchar *pSend);
	uint16	SendDataToAudioDevice(uint16 Cmd,uint16 Length,uchar *pData);
private:	
	uchar		RecBuffer[UART2_REC_LENGTH];		//uchar		RecBuffer[UART2_REC_LENGTH];
	uchar		TxdBuffer[UART2_TXD_LENGTH];
	uchar		TxdBufUsing;							//д���ͻ�����ʱ�ı���
	uint32	RecOverTimeCnt;						//���ճ�ʱ��־
	uint16	Counter;								//
	uint16   RecLength,TxdLength,CameraDataLen;					//���ջ��������ȣ����ͻ���������
	//uchar		FrameHead[8];
	uchar		Camera;
};

UART2_EXT	__noinit__		Uart2Cls		Uart2;		// 






#endif

/*******************************  End of File *************************************/


