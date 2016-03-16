
#ifndef      	__FT_GT20L16_H__
#define      	__FT_GT20L16_H__
				 	
#ifdef      	FT_GLOBALS
#define     	FT_EXT
#else
#define    	 	FT_EXT    extern
#endif


#include			"..\Config\RTOS.h" 

#define			FT_COMMON_READ		0
#define			FT_FAST_READ		1
										




/******************FT_GT20L16P1Y√¸¡Ó√∂æŸ*******************************/
FT_EXT	enum		
{
	FT_CMD_COMMON_READ_DATA     = 0x03,
	FT_CMD_FAST_READ_DATA		 = 0x0b,
} FT_CMD;



class FTCls
{
public:
	FTCls();
	void Init(void);
	uchar       Read(uint32 Addr,uint16 Length,uchar *Data, uchar ReadType);
	uint32		CalcFontAddress(uint16 FontCode);
	////////////////////////////////////////////////////
};

FT_EXT	__noinit__		FTCls		FT;

#endif

/**************************************End of File*********************************************/ 


