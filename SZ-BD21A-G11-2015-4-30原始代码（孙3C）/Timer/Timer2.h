		   /***************************************  Copyright (c)  **************************************
**										广州国业电子科技有限公司
**									
**                      
**	  
****************************************  Copyright (c)  **************************************
**                                         文件描述
**	文件名:   Timer2.H
**	功能描述：Timer2中断服务程序头文件.
**	编译环境：Keil for ARM uVision3
**	目标CPU:  
**	作者：	 
**	创建时间：2009年05月07日
**  修改时间: 
**	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。	
*************************************************************************************************/
			  
#ifndef			__TIMER2_H__
#define			__TIMER2_H__

#ifdef      	TIMER2_GLOBALS
#define     	TIMER2_EXT
#else
#define     	TIMER2_EXT    extern
#endif

#include		"..\Config\RTOS.h"

/******************* UART类 ****************************/
class			Timer2Cls
{
public:
	Timer2Cls(void);
	void		Init(unsigned int Clock);	
	__inline	void	Interrupt(void);

};

TIMER2_EXT	__noinit__		Timer2Cls		Timer2;		

#endif

/*******************************  End of File *************************************/





