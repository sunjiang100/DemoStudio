/***************************************  Copyright (c)  **************************************
**										
**									
**                      
**	  
****************************************  Copyright (c)  **************************************
**                                         �ļ�����
** �ļ���:   Print.cpp
** ����������������ӡ������
** ���뻷����Keil uVision4 + RTX
** Ŀ��CPU:  LPC1766
** �ӿ�ģ��: 
** ���ߣ�	 	
** ����ʱ�䣺2011-8-3 7:09:15
** �޸�ʱ��: 
*************************************************************************************************/
#ifndef			__PRINT_H__
#define			__PRINT_H__

#ifdef      	PRINT_GLOBALS
#define     	PRINT_EXT
#else
#define     	PRINT_EXT    extern
#endif

#include	    	"..\Config\RTOS.h"

#define     IFPrint        1               //1 ���ô�ӡ����    0������

#define		DOT_PER_LINE							384

//#define		CHAR_DOT_SIZE							16
#define		DOT_LINE_NUMBER_PER_CHAR			24			// ÿ���ַ�������

#define		BYTE_NUMBER_PER_DOT_LINE			48			// ÿ�е����ֽ���	  48
#define		CHAR_NUMBER_PER_LINE					32			// ÿ���ַ���

#define		BYTE_LATTICE							(BYTE_NUMBER_PER_DOT_LINE*DOT_LINE_NUMBER_PER_CHAR+16)

#define		MOTOR_FORWARD							'F'
#define		MOTOR_BACK								'B'


//#define		DOT_SIZE									0x1616

// ASCII��ĵ��������½ǵģ����¶��峵�ƺ���ĵ㣬���м�λ��
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








/**************** Print�� ****************************/
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
	uchar		StepTick,StepDir;			// ��������
	uint32	StepTickCnt;				// �������ļ�����
	//uchar    *pPrintBuf;					//BYTE_LATTICE ��С���ڴ����ָ��
	//uchar    *pOneCLB;					//CHAR_NUMBER_PER_LINE+2  ��С���ڴ����ָ��
	
	 uchar	 *LatticeBuffer;
	 uchar	 *OneCharLineBuffer;
	//uchar		LatticeBuffer[BYTE_LATTICE];		// ÿ���ֵĵ��󻺳�
	//uchar		OneCharLineBuffer[CHAR_NUMBER_PER_LINE+2];		// 
	uchar		WriteByte(uchar Data);		
	uchar	 	shift_Value(uchar org, uint16 index);		
};


PRINT_EXT		__noinit__		PrintCls		Print;		

#endif

/******************************* End of File *************************************/













