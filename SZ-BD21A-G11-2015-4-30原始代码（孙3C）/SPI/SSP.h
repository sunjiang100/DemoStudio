/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
	�ļ���: 
	����������
	���뻷����
	Ŀ��CPU: 
	���ߣ�
	����ʱ�䣺
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  

#ifndef        __SSP_H__
#define			__SSP_H__


#ifdef      SSP_GLOBALS
#define     SSP_EXT
#else
#define     SSP_EXT    extern
#endif

#include 		"..\Config\RTOS.h"              /* ARTX kernel functions & defines     */

///      SPI Control Register (S0SPCR - 0xE0020000)
/*
#define			CPHA			3
#define			CPOL			4
#define			MSTR			5
#define			LSBF			6
#define			SPIE			7


///		SPI Status Register (S0SPSR - 0xE0020004)
#define			ABRT			3
#define			MODF			4
#define			ROVR			5
#define			VCOL			6
#define			SPIF			7			// SPI transfer complete flag.
*/

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	SPI��
*/
class			SSPCls
{
public:
	SSPCls(void);
	void		Init(void);	 
	uchar		WriteByte(uchar Data);		
	uchar		ReadByte(void);

	uchar    WriteByte_no_OS(uchar Data);

	uchar		ReadByte_no_OS(void);
	//uchar		SPI1_ReadByte2PIN(void);

private:
	uchar		Busing;

};

SSP_EXT	__noinit__		SSPCls		SSP0;


#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
