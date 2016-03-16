/***************************************  Copyright (c)  **************************************
**										
**									
**                      
**	  
****************************************  Copyright (c)  **************************************
**                                         文件描述
** 文件名:   Print.cpp
** 功能描述：热敏打印机驱动
** 编译环境：Keil uVision4 + RTX
** 目标CPU:  LPC1766
** 接口模块: 
** 作者：	 	
** 创建时间：2011-8-3 7:09:15
** 修改时间: 
*************************************************************************************************/
#ifndef			__PRINT_H__
#define			__PRINT_H__

#ifdef      	PRINT_GLOBALS
#define     	PRINT_EXT
#else
#define     	PRINT_EXT    extern
#endif

#include	    	"..\Config\RTOS.h"

#define     IFPrint        1               //1 启用打印代码    0不启用

#define		DOT_PER_LINE							384

//#define		CHAR_DOT_SIZE							16
#define		DOT_LINE_NUMBER_PER_CHAR			24			// 每个字符点行数

#define		BYTE_NUMBER_PER_DOT_LINE			48			// 每行点阵字节数	  48
#define		CHAR_NUMBER_PER_LINE					32			// 每行字符数

#define		BYTE_LATTICE							(BYTE_NUMBER_PER_DOT_LINE*DOT_LINE_NUMBER_PER_CHAR+16)

#define		MOTOR_FORWARD							'F'
#define		MOTOR_BACK								'B'


//#define		DOT_SIZE									0x1616

// ASCII表的点是在左下角的，重新定义车牌号里的点，在中间位置
#define		ASCII_CENTER_DOT						0x1F

PRINT_EXT	enum
{
	epPRINT_FREE = 0,
	epPRINT_START,

	epGET_NEW_LINE_CHAR,

	epWALKING_PAPER_N_DOT_LINE,
	
	epWALKING_PAPER_4DOT_LINE,
	epHEATING,
	epWALKING_PAPER_1DOT_LINE,
	
	
	
} ENUM_PRING_STATUS;








/**************** Print类 ****************************/
class			PrintCls
{
public:
	PrintCls(void);
	void		eTask(void);
	
	void		Init(uchar Set); 
	void		SetPrintStatus(uint32 s);
	void		PrintStatus(void);
	void		WalkingPaperOneDotLine(uchar Dir,uint32 Line);

	uchar		GetOneLineCharLattice(void);
	void		SendOneLineDotData(void);
	void		TestLattice(uchar Index);
	
	void		WalkingPaper(void);
	
	uint32	TickCnt,StatusCnt;
	
private:	
	uint32		DotLineAddr,IntCnt;
	uchar		DotLineCnt,CharLineCnt,HeatCnt;
	uchar		GetNewCharLine;
	uchar		Frinting,Busing;
	
	//uchar		EnableHot,EnableWalkingPaper,DirWalkingPaper;
	uchar		StepTick,StepDir;			// 步进节拍
	uint32	StepTickCnt;				// 步进节拍计算器
	//uchar    *pPrintBuf;					//BYTE_LATTICE 大小的内存分配指针
	//uchar    *pOneCLB;					//CHAR_NUMBER_PER_LINE+2  大小的内存分配指针
	
	 uchar	 *LatticeBuffer;
	 uchar	 *OneCharLineBuffer;
	//uchar		LatticeBuffer[BYTE_LATTICE];		// 每行字的点阵缓冲
	//uchar		OneCharLineBuffer[CHAR_NUMBER_PER_LINE+2];		// 
	uchar		WriteByte(uchar Data);		
	uchar	 	shift_Value(uchar org, uint16 index);		
};


PRINT_EXT		__noinit__		PrintCls		Print;		

#endif

/******************************* End of File *************************************/













