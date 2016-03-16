
#ifndef      __CONSTCODE_H__
#define      __CONSTCODE_H__
/////////////////////////////// 

/*
#ifdef      CONST_GLOBALS
#define     CONST_EXT
#else
#define     CONST_EXT    extern
#endif
*/
////////////////////////
//#define		GUOYEE_PRT					0					  // 国业产品
//#define		SHANGHAI_SIZENG_PRT		1					  // 上海思增产品
//#define		GENERAL_PRT					2					  // 通用厂家产品
//
//#define		SHAGNHAI_SIZENG			1				//  1 ：上海思增		0 ： 广州国业电子（本公司）
//
//
//#define		PRODUCT_TYPE				SHANGHAI_SIZENG_PRT	//GUOYEE_PRT //GENERAL_PRT //SHANGHAI_SIZENG_PRT			 // 各家的产品，在此修改

#include			"Productor.h"								  

///////////////////////////////////////////////////////////////////////////////////////////////////////
#define			CONST_FOR_BOOT				0


#if ( CONST_FOR_BOOT )

// BootLoader代码区，常量，地址和格式固定，请不要随意修改。
#define						CONST_BASE_ADDR			0x00002000
const	char					CodeMD5[16] 				__attribute__((at(CONST_BASE_ADDR+0x0000))) = {0};
const	unsigned long		CodeSum	 					__attribute__((at(CONST_BASE_ADDR+0x0010))) = 0;
const	unsigned long		CodeXorSum	 				__attribute__((at(CONST_BASE_ADDR+0x0014))) = 0;
const	unsigned long		CheckSum1		 			__attribute__((at(CONST_BASE_ADDR+0x0018))) = 0;
const	unsigned long		CheckSum2	 				__attribute__((at(CONST_BASE_ADDR+0x001c))) = 0;
const	char					Model[16] 					__attribute__((at(CONST_BASE_ADDR+0x0020))) = {"G11"};
const	unsigned long		HardVersion					__attribute__((at(CONST_BASE_ADDR+0x0030))) = 0x00000100;
const	unsigned long		SoftVersion					__attribute__((at(CONST_BASE_ADDR+0x0034))) = 0x00000100;
const	unsigned long		CompilerDT 					__attribute__((at(CONST_BASE_ADDR+0x0038))) = 0x00000000;
const char    				CompileDate[12] 			__attribute__((at(CONST_BASE_ADDR+0x003c))) = __DATE__;
const char    				CompileTime[12] 			__attribute__((at(CONST_BASE_ADDR+0x0048))) = __TIME__;
const char    				McuModel[12] 				__attribute__((at(CONST_BASE_ADDR+0x0054))) = {"LPC1766"};
const char    				GpsModel[16] 				__attribute__((at(CONST_BASE_ADDR+0x0060))) = {"NEO-6M"};
const char    				GsmModel[16] 				__attribute__((at(CONST_BASE_ADDR+0x0070))) = {"LEON-G100"};
const	char					GsmIMEI[16] 				__attribute__((at(CONST_BASE_ADDR+0x0080))) = {0};
const	char					RSV_16B[16]					__attribute__((at(CONST_BASE_ADDR+0x0090))) = {0};
const	char					constOEM_ID[32] 			__attribute__((at(CONST_BASE_ADDR+0x00a0))) = {"1205020001\0"};
const	char					EncryIC_Info[32]			__attribute__((at(CONST_BASE_ADDR+0x00c0))) = {0};
const	char					RSV_32B[32] 				__attribute__((at(CONST_BASE_ADDR+0x00e0))) = {0};

#else
// 用户代码区，常量，地址和格式固定，请不要随意修改。
#define						CONST_BASE_ADDR			0x00006000
const	char					CodeMD5[16] 				__attribute__((at(CONST_BASE_ADDR+0x0000))) = {0};
const	unsigned long		CodeSum	 					__attribute__((at(CONST_BASE_ADDR+0x0010))) = 0;
const	unsigned long		CodeXorSum	 				__attribute__((at(CONST_BASE_ADDR+0x0014))) = 0;
const	unsigned long		CheckSum1		 			__attribute__((at(CONST_BASE_ADDR+0x0018))) = 0;
const	unsigned long		CheckSum2	 				__attribute__((at(CONST_BASE_ADDR+0x001c))) = 0;
const	char					Model[16] 					__attribute__((at(CONST_BASE_ADDR+0x0020))) = {"G11"};
const	unsigned long		HardVersion					__attribute__((at(CONST_BASE_ADDR+0x0030))) = 0x00000100;
const	unsigned long		SoftVersion					__attribute__((at(CONST_BASE_ADDR+0x0034))) = 0x00000106;
const	unsigned long		CompilerDT 					__attribute__((at(CONST_BASE_ADDR+0x0038))) = 0x00000000;
const char    				CompileDate[12] 			__attribute__((at(CONST_BASE_ADDR+0x003c))) = __DATE__;
const char    				CompileTime[12] 			__attribute__((at(CONST_BASE_ADDR+0x0048))) = __TIME__;
const char    				McuModel[12] 				__attribute__((at(CONST_BASE_ADDR+0x0054))) = {"LPC1766"};
const char    				GpsModel[16] 				__attribute__((at(CONST_BASE_ADDR+0x0060))) = {"NEO-6M"};
const char    				GsmModel[16] 				__attribute__((at(CONST_BASE_ADDR+0x0070))) = {"M35"};
const	char					DefaultIP[32] 				__attribute__((at(CONST_BASE_ADDR+0x0080))) = {"113.108.150.103:8009\0"};
const	char					GY_DomainName[32]			__attribute__((at(CONST_BASE_ADDR+0x00a0))) = {"www.guoyee.com\0"};
const	char					DefaultCenterNumber[16] __attribute__((at(CONST_BASE_ADDR+0x00c0))) = {"13922280586\0"};
const	char					DefaultUserNumber[16]	__attribute__((at(CONST_BASE_ADDR+0x00d0))) = {"13922732629\0"};
const	char					StandardVersion[16] 		__attribute__((at(CONST_BASE_ADDR+0x00e0))) = {"GB/T19056-2011"};


const	char					GY_Rev16[16] 			__attribute__((at(CONST_BASE_ADDR+0x00f0))) = {0};

const	char					GY_Rev60[60] 			__attribute__((at(CONST_BASE_ADDR+0x0100))) = {0};

const	char					GY_Rev4[4] 				__attribute__((at(CONST_BASE_ADDR+0x013C))) = {0};

//////////////////////////// 当修改常量constModelVersion时，需修改下面几个宏定义	  ( 连同当修改UserConfig.h的对应宏)
//#define		LEN_MODEL_VERSION		55
//#define		LEN_MODEL				7
//#define		MV_POS_MODEL			7
//#define		MV_POS_HV				36
//#define		MV_POS_SV				50
//#define		BUILD_DATE_POS			(LEN_MODEL_VERSION+1)
//#define		BUILD_TIME_POS			(BUILD_DATE_POS+12)


#if(PRODUCT_TYPE==GUOYEE_PRT)
const	char					constModelVersion[] 		__attribute__((at(CONST_BASE_ADDR+0x0140))) = 
   {"Model: GY-G11A,CPU: LPC1765,HardVer:3.0.1,SoftVer:1.3.0\0\0\0\0\0\0\0"};	  // 要与   HardVersion	   SoftVersion 一致,此处与显示屏的“产品信息”相关联
#elif (PRODUCT_TYPE==SHANGHAI_SIZENG_PRT)
const	char					constModelVersion[] 		__attribute__((at(CONST_BASE_ADDR+0x0140))) = 
   {"Model: SZ-BD21A,CPU:LPC1765,HardVer:3.0.1,SoftVer:1.3.0\0\0\0\0\0\0\0"};	  // 要与   HardVersion	   SoftVersion 一致,此处与显示屏的“产品信息”相关联
#elif (PRODUCT_TYPE==GENERAL_PRT)
const	char					constModelVersion[] 		__attribute__((at(CONST_BASE_ADDR+0x0140))) = 
   {"Model: BD-GPS ,CPU: LPC1765,HardVer:3.0.1,SoftVer:1.3.0\0\0\0\0\0\0\0"};	  // 要与   HardVersion	   SoftVersion 一致,此处与显示屏的“产品信息”相关联
#endif


const	char					constHSVersion[] 			__attribute__((at(CONST_BASE_ADDR+0x0180)))	=  {"H_3.0.1 S_1.3.5"};	// 要与   HardVersion	   SoftVersion 一致


#endif															     



#if ( CONST_FOR_BOOT )
typedef	struct
{
	uchar		CodeMD5[16];			// 用户代码明文MD5（0x6000~0x601f不参与计算）
	uint32	CodeSum;					// 用户代码累加和	（0x6000~0x601f不参与计算）
	uint32	CodeXorSum;				// 用户代码异或和	（0x6000~0x601f不参与计算）
	uint32	CheckSum1;				// 0x6000~0x6017的累加和
	uint32	CheckSum2;				// 0x6000~0x6017的累加和
	uchar		Model[16];				// 产品型号
	uint32	HardVersion;			// 硬件版本
	uint32	SoftVersion;			// 软件版本
	uint32	CompilerDT;				// 编译时间，秒
	uchar		CompileDate[12];		// 编译日期字符串
	uchar		CompileTime[12];		// 编译时间字符串
	uchar		McuModel[12];			// MCU型号
	uchar		GpsModel[16];			// GPS模块型号
	uchar		GsmModel[16];			// GSM模块型号	
	uchar		GsmIMEI[16];			// 默认厂家域名
	uchar		RSV_16B[16];			// 保留
	uchar		constOEM_ID[32];		// 产品唯一序列号（国业编码规则） 
	uchar		EncryIC_Info[32];		// 加密IC信息
	uchar		RSV_32B[16];			// 产品执行标准及版本
}BOOT_CODE_CONST,*pBOOT_CODE_CONST;


#else

typedef	struct
{
	uchar		CodeMD5[16];			// BootLoader代码明文MD5（0x2000~0x201f不参与计算）
	uint32	CodeSum;					// BootLoader代码累加和	（0x2000~0x201f不参与计算）
	uint32	CodeXorSum;				// BootLoader代码异或和	（0x2000~0x201f不参与计算）
	uint32	CheckSum1;				// 0x2000~0x2017的累加和
	uint32	CheckSum2;				// 0x2000~0x2017的累加和
	uchar		Model[16];				// 产品型号
	uint32	HardVersion;			// 硬件版本
	uint32	SoftVersion;			// 软件版本
	uint32	CompilerDT;				// 编译时间，秒
	uchar		CompileDate[12];		// 编译日期字符串
	uchar		CompileTime[12];		// 编译时间字符串
	uchar		McuModel[12];			// MCU型号
	uchar		GpsModel[16];			// GPS模块型号
	uchar		GsmModel[16];			// GSM模块型号
	uchar		DefaultIP[32];			// 默认厂家IP
	uchar		GY_Domain[32];			// 默认厂家域名
	uchar		CenterNumber[16];		// 默认中心号码 
	uchar		UserNumber1[16];		// 默认用户号码
	uchar		StandardVersion[16];	// 产品执行标准及版本
}USER_CODE_CONST,*pUSER_CODE_CONST;  




#endif



#endif


/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */


