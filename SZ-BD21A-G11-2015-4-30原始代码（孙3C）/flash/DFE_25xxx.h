/***********************************Copyright (c)******************************************
**								 
**								
**                     	
**							  
************************************** Copyright (c) ***************************************
** 
**                                    �ļ�����
**	�ļ���:   FM25LC04.C
**	����������FM25LC04�ĵײ���������
**	���뻷����Keil uVision3
**	Ŀ��CPU:  LPC2109
**	���ߣ�     
**	����ʱ�䣺2009.04.25
**	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
*********************************************************************************************/
#ifndef      	__FM25LC64_H__
#define      	__FM25LC64_H__
				 	
#ifdef      	DFE_GLOBALS
#define     	DFE_EXT
#else
#define    	 	DFE_EXT    extern
#endif

#define			__FM25LC64_DEBUG__		0


//#include			"..\SPI\SPI.H"
//#include			"..\uart\uart0.h"
#include			"..\Config\RTOS.h" 
//#include			"..\Common\UserGlobals.h"
										




/******************FM25LC64����ö��*******************************/
DFE_EXT	enum		
{
	DFE_CMD_WRITE_ENABLE		 	= 0x06,				 
	DFE_CMD_WRITE_DISABLE        = 0x04,
	DFE_CMD_READ_STATUS_REG      = 0x05,
	DFE_CMD_WRITE_STATUS_REG     = 0x01,
	DFE_CMD_READ_DATA        	 = 0x03,
	DFE_CMD_WRITE_DATA 			 = 0X02,
} DFE_CMD;



class DFECls
{
public:
	DFECls();
	void    	Init();
	uchar		ReadStatusRegister(void);
	void		WriteStatusRegister(uchar s);
	uchar       Read(uint16 Addr,uint16 Length,uchar *Data);
	uchar       Write(uint16 Addr,uint16 Length,uchar *Data);
	////////////////////////////////////////////////////
	uint16	    ReadWord(uint16 Addr);
	void	    WriteWord(uint16 Addr,uint16	Data);
};

DFE_EXT	__noinit__		DFECls		DFE;

#endif

/**************************************End of File*********************************************/ 


