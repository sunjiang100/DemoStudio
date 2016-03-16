
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
//#define		GUOYEE_PRT					0					  // ��ҵ��Ʒ
//#define		SHANGHAI_SIZENG_PRT		1					  // �Ϻ�˼����Ʒ
//#define		GENERAL_PRT					2					  // ͨ�ó��Ҳ�Ʒ
//
//#define		SHAGNHAI_SIZENG			1				//  1 ���Ϻ�˼��		0 �� ���ݹ�ҵ���ӣ�����˾��
//
//
//#define		PRODUCT_TYPE				SHANGHAI_SIZENG_PRT	//GUOYEE_PRT //GENERAL_PRT //SHANGHAI_SIZENG_PRT			 // ���ҵĲ�Ʒ���ڴ��޸�

#include			"Productor.h"								  

///////////////////////////////////////////////////////////////////////////////////////////////////////
#define			CONST_FOR_BOOT				0


#if ( CONST_FOR_BOOT )

// BootLoader����������������ַ�͸�ʽ�̶����벻Ҫ�����޸ġ�
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
// �û�����������������ַ�͸�ʽ�̶����벻Ҫ�����޸ġ�
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

//////////////////////////// ���޸ĳ���constModelVersionʱ�����޸����漸���궨��	  ( ��ͬ���޸�UserConfig.h�Ķ�Ӧ��)
//#define		LEN_MODEL_VERSION		55
//#define		LEN_MODEL				7
//#define		MV_POS_MODEL			7
//#define		MV_POS_HV				36
//#define		MV_POS_SV				50
//#define		BUILD_DATE_POS			(LEN_MODEL_VERSION+1)
//#define		BUILD_TIME_POS			(BUILD_DATE_POS+12)


#if(PRODUCT_TYPE==GUOYEE_PRT)
const	char					constModelVersion[] 		__attribute__((at(CONST_BASE_ADDR+0x0140))) = 
   {"Model: GY-G11A,CPU: LPC1765,HardVer:3.0.1,SoftVer:1.3.0\0\0\0\0\0\0\0"};	  // Ҫ��   HardVersion	   SoftVersion һ��,�˴�����ʾ���ġ���Ʒ��Ϣ�������
#elif (PRODUCT_TYPE==SHANGHAI_SIZENG_PRT)
const	char					constModelVersion[] 		__attribute__((at(CONST_BASE_ADDR+0x0140))) = 
   {"Model: SZ-BD21A,CPU:LPC1765,HardVer:3.0.1,SoftVer:1.3.0\0\0\0\0\0\0\0"};	  // Ҫ��   HardVersion	   SoftVersion һ��,�˴�����ʾ���ġ���Ʒ��Ϣ�������
#elif (PRODUCT_TYPE==GENERAL_PRT)
const	char					constModelVersion[] 		__attribute__((at(CONST_BASE_ADDR+0x0140))) = 
   {"Model: BD-GPS ,CPU: LPC1765,HardVer:3.0.1,SoftVer:1.3.0\0\0\0\0\0\0\0"};	  // Ҫ��   HardVersion	   SoftVersion һ��,�˴�����ʾ���ġ���Ʒ��Ϣ�������
#endif


const	char					constHSVersion[] 			__attribute__((at(CONST_BASE_ADDR+0x0180)))	=  {"H_3.0.1 S_1.3.5"};	// Ҫ��   HardVersion	   SoftVersion һ��


#endif															     



#if ( CONST_FOR_BOOT )
typedef	struct
{
	uchar		CodeMD5[16];			// �û���������MD5��0x6000~0x601f��������㣩
	uint32	CodeSum;					// �û������ۼӺ�	��0x6000~0x601f��������㣩
	uint32	CodeXorSum;				// �û���������	��0x6000~0x601f��������㣩
	uint32	CheckSum1;				// 0x6000~0x6017���ۼӺ�
	uint32	CheckSum2;				// 0x6000~0x6017���ۼӺ�
	uchar		Model[16];				// ��Ʒ�ͺ�
	uint32	HardVersion;			// Ӳ���汾
	uint32	SoftVersion;			// ����汾
	uint32	CompilerDT;				// ����ʱ�䣬��
	uchar		CompileDate[12];		// ���������ַ���
	uchar		CompileTime[12];		// ����ʱ���ַ���
	uchar		McuModel[12];			// MCU�ͺ�
	uchar		GpsModel[16];			// GPSģ���ͺ�
	uchar		GsmModel[16];			// GSMģ���ͺ�	
	uchar		GsmIMEI[16];			// Ĭ�ϳ�������
	uchar		RSV_16B[16];			// ����
	uchar		constOEM_ID[32];		// ��ƷΨһ���кţ���ҵ������� 
	uchar		EncryIC_Info[32];		// ����IC��Ϣ
	uchar		RSV_32B[16];			// ��Ʒִ�б�׼���汾
}BOOT_CODE_CONST,*pBOOT_CODE_CONST;


#else

typedef	struct
{
	uchar		CodeMD5[16];			// BootLoader��������MD5��0x2000~0x201f��������㣩
	uint32	CodeSum;					// BootLoader�����ۼӺ�	��0x2000~0x201f��������㣩
	uint32	CodeXorSum;				// BootLoader��������	��0x2000~0x201f��������㣩
	uint32	CheckSum1;				// 0x2000~0x2017���ۼӺ�
	uint32	CheckSum2;				// 0x2000~0x2017���ۼӺ�
	uchar		Model[16];				// ��Ʒ�ͺ�
	uint32	HardVersion;			// Ӳ���汾
	uint32	SoftVersion;			// ����汾
	uint32	CompilerDT;				// ����ʱ�䣬��
	uchar		CompileDate[12];		// ���������ַ���
	uchar		CompileTime[12];		// ����ʱ���ַ���
	uchar		McuModel[12];			// MCU�ͺ�
	uchar		GpsModel[16];			// GPSģ���ͺ�
	uchar		GsmModel[16];			// GSMģ���ͺ�
	uchar		DefaultIP[32];			// Ĭ�ϳ���IP
	uchar		GY_Domain[32];			// Ĭ�ϳ�������
	uchar		CenterNumber[16];		// Ĭ�����ĺ��� 
	uchar		UserNumber1[16];		// Ĭ���û�����
	uchar		StandardVersion[16];	// ��Ʒִ�б�׼���汾
}USER_CODE_CONST,*pUSER_CODE_CONST;  




#endif



#endif


/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */


