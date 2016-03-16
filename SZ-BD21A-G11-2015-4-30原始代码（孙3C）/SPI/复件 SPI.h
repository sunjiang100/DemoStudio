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

#ifndef        __SPI_H__
#define			__SPI_H__


#ifdef      SPI_GLOBALS
#define     SPI_EXT
#else
#define     SPI_EXT    extern
#endif

#include		"..\Config\RTOS.h"


///      SPI Control Register (S0SPCR - 0xE0020000)
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





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	IIC��
*/
class			SPICls
{
public:
	SPICls(void);
	void			Init(void);
	uchar			WriteByte(uchar Data);
	uchar			ReadByte(void);
	uchar			ReadByte2PIN(void);
	uchar			WriteReadByte(uchar Data);
	uint16		MCP3208_ADC(uchar ch);
private:
	uchar			Busing;
	
};



SPI_EXT		__noinit__		SPICls		SPI;







#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
