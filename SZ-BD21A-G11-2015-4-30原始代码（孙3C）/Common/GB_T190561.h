/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 		  
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 
	功能描述：
	编译环境：
	目标CPU: 
	作者：
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  

#ifndef      __GB_T19056_H__
#define      __GB_T19056_H__


#ifdef      GB19056_GLOBALS
#define     GB19056_EXT
#else
#define     GB19056_EXT    extern
#endif


// $$$$$$$$$$$$$$$$$$ <<< Use Configuration Wizard in Context Menu >>> $$$$$$$$$$$$$$$$$
// <h>国标文件相关配置

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>ENABLE_MODIFY_VEHICLE_INFO: 允许多次修改车辆信息 <0=> 不允许 <1=> 允许
//   	<i> Default: 不允许
*/
#define		ENABLE_MODIFY_VEHICLE_INFO		1	// 允许多次修改车辆信息
				 
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>TWO_DAY_OVT: 只处理最后停车前2天疲劳驾驶 <0=> 处理全部 <1=> 处理2天内
//   	<i> Default: 处理2天内
*/
#define		TWO_DAY_OVT			1


// </h>

// $$$$$$$$$$$$$$$$$$$ <<< end of configuration section >>> $$$$$$$$$$$$$$$$$$$$$$$$$$$$


#define		OVER_TIME_MAX_CNT				20		// 最多记录超时记录的条数
#define		OVER_SPEED_MAX_CNT			20		// 最多记录超速记录的条数
#define		ID_CARD_REC_MAX_CNT			150	// 最多记录驾驶员身份认证次数
#define		KV_OP_REC_MAX_CNT				100	// 最多记录下载车辆特征系数记录



////////////	通讯出错标志	///////////////////////////////////////////////////////
#define		UP_LOAD_ERROR					0xfa	// 上传出错标志字
#define		DOWN_LOAD_ERROR				0xfb	// 下传出错标志字

#define		RESERVER_WORD					0x00	// 

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	  			命令字定义
*/
GB19056_EXT    enum
{
	//上载命令字	//////////////////////////////////////////////////////////
	eGB_rCMD_GET_STD_VERSION				= 0x00,	// 采集记录仪执行标准
	eGB_rCMD_GET_DRIVER_CODE				= 0x01,	// 驾驶员代码及其对应的机动车驾驶证号
	eGB_rCMD_GET_REAL_TIME					= 0x02,	// 采集实时时钟
	eGB_rCMD_GET_TOTAL_MILAGE				= 0x03,	// *最近360h内的累计行驶里程数据
	eGB_rCMD_GET_SPEED_PULSE_COEFFICIENT= 0x04,	// *Vehicle Characteristic Coefficients	
	eGB_rCMD_GET_SPEED_PER_MIN				= 0x05,	// 每分钟平均速度
	eGB_rCMD_GET_VEHICLE_VIN				= 0x06,	// *车辆VIN号，车牌号码，车辆分类
	eGB_rCMD_GET_STOP_DATA					= 0x07,	// *事故疑点数据（停车前20秒数据）
	eGB_rCMD_GET_SIGNAL_CONFIG				= 0x08,	// 
	eGB_rCMD_GET_SPEED_PER_SECOND			= 0x09,	// 每秒钟平均速度
	eGB_rCMD_GET_GET_LOG_IN_OUT_RECORD	= 0X10,	// 
	eGB_rCMD_GET_OVER_TIME_3H				= 0x11,	// 疲劳驾驶数据
	eGB_rCMD_GET_POSITION_INFO				= 0x13,	// 采集指定的位置信息
	eGB_rCMD_GET_POWER_RECORD				= 0x14,	// 采集外部供电记录
	eGB_rCMD_GET_MODIFY_PARA_RECORD		= 0x15,	// 
	//其中：国标规定至少有 0x01~0x07共7个上载命令，其它可在显示器显示
	
	////////////	下载命令字	//////////////////////////////////////////////////////////
	eGB_rCMD_SET_DRIVER_CODE				= 0x81,	// 设置驾驶员代码，驾驶证号码
	eGB_rCMD_SET_VEHICLE_VIN				= 0x82,	// 设置车辆VIN号，车牌号，车辆分类
	eGB_rCMD_SET_SETUP_DATE					= 0x83,	// 安装日期
	eGB_rCMD_SET_SIGNAL_STATUS				= 0x84,	// 
	eGB_rCMD_SET_REAL_TIME					= 0xc2,	// 设置实时时间
	eGB_rCMD_SET_VEHICLE_CC					= 0xc3,	// 设置车辆特征系数
	
	////////////	检定控制命令	//////////////////////////////////////////
	eGB_rCMD_SET_CHECK
	
} enGB_RS232_CMD;
	




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	  			部分参数的长度字节数
*/

#define		LEN_RTC						6		// RTC

#define		LEN_GPS_TIME_DIFFERENCE	4		// 


////////////	引用外部变量	/////////////////////////////////////////////////////
#pragma		pack(1)
typedef		struct	_RW_FM
{
	uchar		RW;				// 读写标志，'R'读，'W'写
	uchar		DS;				// 选择器件，非零为GSM用EEPROM
	uint16	Addr;				// 存储地址
	uint16	Length;			// 读写长度
	uchar		SerialNum[8];	// 写时需要序列号
	uchar		Buffer[256];	// 写数据
} RW_FM,*pRW_FM; 
 

#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
