/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												广州国业电子科技有限公司
$										Shawyoo Electronics (GuangZhou) R&D Center
$                       	http://www.guoyee.com		 Email: guoyegps@126.com
$								 QQ群：9069471/9069433/9524253		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 	  IC_Card.h
	功能描述： 头文件
	编译环境： Keil for ARM + ARTX
	目标CPU:   
	作者：	  
	创建时间： 
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  

#ifndef        __IC_CARD_H__
#define			__IC_CARD_H__



#ifdef      IC_CARD_GLOBALS
#define     IC_CARD_EXT
#else
#define     IC_CARD_EXT    extern
#endif


#include		"TWI.h"

#include		"..\Config\RTOS.h"

// $$$$$$$$$$$$$$$$$$ <<< Use Configuration Wizard in Context Menu >>> $$$$$$$$$$$$$$$$$
// <h>IIC0相关设置


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>IC_BYTE_SEL: IC卡字节 <1=> 256字节 <2=> 512字节 <3=> 1024字节
//   	<i> Default: 256
*/
#define			IC_BYTE_SEL				1

#if(IC_BYTE_SEL==1)
#define		IC_CARD_BYTE_NUMBER      256
#elif(IC_BYTE_SEL==2)
#define		IC_CARD_BYTE_NUMBER      512
#elif(IC_BYTE_SEL==2)
#define		IC_CARD_BYTE_NUMBER      1024
#else
#error  "IC卡容量错误"
#endif



// </h>
// $$$$$$$$$$$$$$$$$$$ <<< end of configuration section >>> $$$$$$$$$$$$$$$$$$$$$$$$$$$$



IC_CARD_EXT		enum
{
	eIC_CARD_READ_MAIN_MEMORY = 0x30,
	eIC_CARD_WRITE_MAIN_MEMORY = 0x38,
	eIC_CARD_READ_PROTECT_MEMORY = 0x34,
	eIC_CARD_WRITE_PROTECT_MEMORY = 0x3c,
	eIC_CARD_READ_SECURITY_MEMORY = 0x31,
	eIC_CARD_WRITE_SECURITY_MEMORY = 0x39,
	eIC_CARD_CHECK_SECURITY_CODE = 0x33,
} IC_CARD_CMD;




						
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    
*/
typedef	struct	_IC_CARD_
{
	uchar		CuringInfo[32];			// 0	 	芯片厂商固化信息
	uchar		CCC[7];						// 32	 	发卡机构代码（企业3C认证代码）
	uchar		SetupYear;					// 39	  	启用年
	uchar		SetupMonth;					// 40		启用月
	uchar		SetupDay;               // 41		启用日
	uchar		CardSN[6];              // 42		流水号
	uchar		Permissions;            // 48		权限，01驾驶卡，02企业卡，03车厂卡，05调试卡，06执法卡，07其它卡，08厂商卡
	uchar		LicenseNumber[18];      // 49		驾驶证号
	uchar		Name[10];               // 67		驾驶员姓名
	uchar		Rsv[22];                // 77		标准扩展预留
	uchar		Rsv2[130];               // 99	生产商预留
	uchar		Company[10];            // 219	单位名称
	uchar		Department[10];         // 229	部门名称
	uchar		CardVer;                // 239	厂商IC卡格式版本
	uchar		GB_Ver[14];             // 240	执行的国标版本
	uchar		ChangeNum;              // 254	修改单号
	uchar		XorSum;                 // 255	异或校验
} IC_CARD,*pIC_CARD;



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   
*/




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	IIC类
*/




#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
