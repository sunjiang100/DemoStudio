		   /***************************************  Copyright (c)  **************************************
**										���ݹ�ҵ���ӿƼ����޹�˾
**									
**                      
**	  
****************************************  Copyright (c)  **************************************
**                                         �ļ�����
**	�ļ���:   Timer2.H
**	����������Timer2�жϷ������ͷ�ļ�.
**	���뻷����Keil for ARM uVision3
**	Ŀ��CPU:  
**	���ߣ�	 
**	����ʱ�䣺2009��05��07��
**  �޸�ʱ��: 
**	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�	
*************************************************************************************************/
			  
#ifndef			__TIMER2_H__
#define			__TIMER2_H__

#ifdef      	TIMER2_GLOBALS
#define     	TIMER2_EXT
#else
#define     	TIMER2_EXT    extern
#endif

#include		"..\Config\RTOS.h"

/******************* UART�� ****************************/
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





