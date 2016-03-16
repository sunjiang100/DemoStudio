/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 	 EEPROM_Addr.h
	功能描述：EEPROM存储地址分配
	编译环境：
	目标CPU: 
	作者：
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  

#ifndef      __EEPROM_ADDR_H__
#define      __EEPROM_ADDR_H__


#ifdef      EEPROM_GLOBALS
#define     EEPROM_EXT
#else
#define     EEPROM_EXT    extern
#endif

#include    "..\Config\UserConfig.h"


// EEEPROM\x00 W \xa0 \xd0\x00 \x20\x00 

// $$$$$$$$$$$$$$$$$$ <<< Use Configuration Wizard in Context Menu >>> $$$$$$$$$$$$$$$$$
// <h>EEPROM相关常量或变量定义

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>EEPROM_TYPE: 选择EEPROM型号 <0=> AT24C02 <1=> AT24C04  <2=> AT24C512 <3=> FM24CL04
//   	<i> Default: FM24LC04
*/
#define			EEPROM_TYPE			3

#define			AT24C02				0
#define			AT24C04				1
#define			AT24C512				2
#define			FM24LC04				3


// </h>

// $$$$$$$$$$$$$$$$$$$ <<< end of configuration section >>> $$$$$$$$$$$$$$$$$$$$$$$$$$$$


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   EEPROM型号及各个参数
	注：请使用页长度为32的存储器，可大于32,但不能小于32。系统日志是按32字节为组存储的。
*/

#define			EE_TOTAL_BYTE							8192                          //4096									//	8k Fram
#define			EE_PAGE_LEN								32										// 每页长度
#define			EE_TOTAL_PAGE							(EE_TOTAL_BYTE/EE_PAGE_LEN)  	// 实际页数		


/////  	系统第一次运行标志区域使用铁电头32字节  实际用16字节，后16字节是备用,方便进行恢复出厂状态///////
/////    修改存储器位置时，可强制恢复出厂默认值（可在升级时自动恢复出厂值）
#define			EE_SYS_FIRST_RUN_FLAG_ADDR			0
#if(EE_SYS_FIRST_RUN_FLAG_ADDR==0)
#define			EE_SYS_FIRST_RUN						0x0000		// 系统第一次运行标志	,
#else
#define			EE_SYS_FIRST_RUN						0x1FF0		// 
#endif


/////////// change by wrc  ///////////////
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	GPS数据打包相关配置
	
*/
#define        EE_OBL_BASE_ADDR                 0x1100 
////#define			EE_GPS_SEND_CONFIG					(EE_OBL_BASE_ADDR+0x10)		// 16B，包含下面
//#define			EE_SEND_TIMES						(EE_OBL_BASE_ADDR+0x10)		// 4B，
//#define			EE_SEND_INTERVAL					(EE_OBL_BASE_ADDR+0x14)		// 2B，
//#define			EE_STOP_SEND_INTERVAL				(EE_OBL_BASE_ADDR+0x16)		// 2B，
//#define			EE_SEND_DISTANCE					(EE_OBL_BASE_ADDR+0x18)		// 2B，
//#define			EE_T_SEND_INTERVAL					(EE_OBL_BASE_ADDR+0x1a)		// 2B，
//#define			EE_T_SEND_TIME						(EE_OBL_BASE_ADDR+0x1c)		// 2B，
//#define			EE_SEND_TYPE						(EE_OBL_BASE_ADDR+0x1e)		// 1B，
//#define			EE_DELAY_ALARM						(EE_OBL_BASE_ADDR+0x1f)	// 1B

// 16B   left 


#define			EE_ELE_RAIL_TOTAL_NUM				(EE_OBL_BASE_ADDR+0x20)		// 2B
#define			EE_NOW_ELE_RAIL_NUM					(EE_OBL_BASE_ADDR+0x22)		// 2B
#define			EE_ALARM_ELE_RAIL_NUM				(EE_OBL_BASE_ADDR+0x24)		// 1B

// 1 Byte left

#define			EE_SELECT_SIM_CARD					(EE_OBL_BASE_ADDR+0x26)		// 1B
#define			EE_ENABLE_SIMU_SPEED		   		(EE_OBL_BASE_ADDR+0x27)		// 1B

#define			EE_SET_PARA_PASSWORD					(EE_OBL_BASE_ADDR+0x28)		// 4B, 设置参数密码

// 	4 Bytes left


#define			EE_UP_DATA_FRAME_CNT1				(EE_OBL_BASE_ADDR+0x30)		// 4B,升级帧数
#define			EE_UP_DATA_FRAME_CNT2				(EE_OBL_BASE_ADDR+0x34)		// 4B
#define			EE_UP_DATA_DOWN_OVER_FLAG    		(EE_OBL_BASE_ADDR+0x38)		// 1B	

// 1 Byte 	left	  

#define			EE_PACK_HEAD						(EE_OBL_BASE_ADDR+0x40)		// 16B

#define			EE_PARA_BLOCK_STATUS				(EE_OBL_BASE_ADDR+0x50)		// 16B=2B*8，参数块配置标志
#define			EE_GPS_SAVE_PAGE					(EE_OBL_BASE_ADDR+0x60)		// 16B，4组

#define			LEN_LOCK_CONTROL_OUT					16
#define			EE_LOCK_FLAG						(EE_OBL_BASE_ADDR+0x70)		// 2B，锁车标志
#define			EE_FORCE_LOCK_FLAG					(EE_OBL_BASE_ADDR+0x72)		// 2B, 
#define			EE_CONTROL_OUT1						(EE_OBL_BASE_ADDR+0x74)		// 2B, 输出控制１
#define			EE_CONTROL_OUT2						(EE_OBL_BASE_ADDR+0x76)		// 2B, 输出控制2
#define			EE_CONTROL_OUT3						(EE_OBL_BASE_ADDR+0x78)		// 2B, 输出控制3
#define			EE_CONTROL_OUT4						(EE_OBL_BASE_ADDR+0x7a)		// 2B, 输出控制3
#define			EE_CONTROL_OUT5						(EE_OBL_BASE_ADDR+0x7c)		// 2B, 输出控制4
#define			EE_CONTROL_OUT_RSV					(EE_OBL_BASE_ADDR+0x7e)		// 2B, 


#define			EE_ALL_DRIVER_INFO					(EE_OBL_BASE_ADDR+0x80)		// 全部驾驶员相关信息
#define			EE_DRIVER_ID						(EE_OBL_BASE_ADDR+0x80)		// 16B,4个驾驶员号	
#define			EE_DRIVER_INFO						(EE_OBL_BASE_ADDR+0x90)		// 16B,看结构DRIVER_INFO
#define			EE_DRIVER_INFO_1					(EE_OBL_BASE_ADDR+0xa0)		// 16B,看结构DRIVER_INFO
#define			EE_DRIVER_INFO_2					(EE_OBL_BASE_ADDR+0xb0)		// 16B,看结构DRIVER_INFO
#define			EE_DRIVER_INFO_3					(EE_OBL_BASE_ADDR+0xc0)		// 16B,看结构DRIVER_INFO
#define			EE_TIRED_DRIVE						(EE_OBL_BASE_ADDR+0xd0)		// 8B，看结构TIRE_DRIVE

#define			EE_BAUDRATE_U0_232					(EE_OBL_BASE_ADDR+0xe0)		// 4B
#define			EE_BAUDRATE_U2_232					(EE_OBL_BASE_ADDR+0xe4)		// 4B
#define			EE_BAUDRATE_U2_485					(EE_OBL_BASE_ADDR+0xe8)		// 4B
#define			EE_BAUDRATE_CAN_BUS					(EE_OBL_BASE_ADDR+0xec)		// 4B
//#define			EE_BAUDRATE_U1_GSM				不可修改
//#define			EE_BAUDRATE_U0_GPS				不可修改
#define			EE_BAUDRATE_U1_232					(EE_OBL_BASE_ADDR+0xf0)	    // 4B	 保留给U1用	现在不用
#define			EE_BAUDRATE_U3_232					(EE_OBL_BASE_ADDR+0xf4)		// 4B   	 // GNSS 定位使用


#define			EE_GPRS_DISP_PROTOCOL				(EE_OBL_BASE_ADDR+0xf8)		//  8B


/////////////// ///////////////////////////////

#define			EE_HANDLE_WATCH_CAR_PASSWORD		(EE_OBL_BASE_ADDR+0x100)	   // 8B,手柄看车密码
#define			EE_HANDLE_FACTORY_PASSWORD			(EE_OBL_BASE_ADDR+0x108)		// 8B,手柄的出厂设置密码

#define			EE_ENABLE_HANDLE_WATCH_CAR			(EE_OBL_BASE_ADDR+0x110)		// 1B,是否使用手柄看车
#define			EE_HANDLE_WATCH_CAR_MODE			(EE_OBL_BASE_ADDR+0x111)		// 1B,手柄看车方式（自动/手动）
#define			EE_DIAL_ANSWER_ENABLE_FLAG			(EE_OBL_BASE_ADDR+0x112)		// 1B,允许通话标志位(第0位:打出;第1位:接听.)
#define			EE_SET_SCAN_FLAG					(EE_OBL_BASE_ADDR+0x113)		// 1B

// 12 Bytes left

#define			EE_PARK_OVER_TIME						(EE_OBL_BASE_ADDR+0x120)		//  2B,停车超时报警时间
#define			EE_USE_PARK_OVER_TIME				(EE_OBL_BASE_ADDR+0x121)		//  1B,使用停车超时检测

#define			EE_SYSTEM_ON_IDLE_MODE_FLAG			(EE_OBL_BASE_ADDR+0x129)		//  1B
																					  
#define			EE_ENABLE_FLAG						(EE_OBL_BASE_ADDR+0x12A)		//  4B，详看Enable全局变量文件中联合体

// 2 Bytes left

/////////////// 省电相关标志的存储地址，连续空间
#define			LEN_EE_IDLE								8			//
#define			EE_IDLE_STR							(EE_OBL_BASE_ADDR+0x130)		// 
#define			EE_ENABLE_INTER_IDLE_MODE 			(EE_OBL_BASE_ADDR+0x130)		// 1B,允许进入省电模式
#define			EE_IDLE_FLAG						(EE_OBL_BASE_ADDR+0x131)		// 1B
#define			EE_INTER_ILDE_TIME					(EE_OBL_BASE_ADDR+0x132)		// 2B,多少时间后进入省电模式
#define			EE_WAKE_UP_TIME						(EE_OBL_BASE_ADDR+0x134)	   // 2B,多少时间后唤醒
#define			EE_WAKE_UP_LONG_TIME				(EE_OBL_BASE_ADDR+0x136)		// 2B,唤醒后持续时间
#define			EE_WAKE_UP_TYPE						(EE_OBL_BASE_ADDR+0x138)		// 1B,唤醒原因


																				  
#define			EE_HIGH_LOW_LEVEL					(EE_OBL_BASE_ADDR+0x160)		// 4B,高低电平识别
#define			EE_POWER_LOW_ALARM_VALUE			(EE_OBL_BASE_ADDR+0x164)		// 4B
#define			EE_VEHICLE_STATUS					(EE_OBL_BASE_ADDR+0x168)		// 4B,车辆状态 
#define			EE_VEHICLE_STATUS1					(EE_OBL_BASE_ADDR+0x16c)		// 4B,车辆状态 

#define			EE_PULSE_FACTOR						(EE_OBL_BASE_ADDR+0x170)		// 4B,脉冲系数

#define			EE_TEST_MILEAGE					(EE_OBL_BASE_ADDR+0x178)
#define			EE_TEST_GPS_MILEAGE				(EE_OBL_BASE_ADDR+0x178)			// 测试里程
#define			EE_TEST_PULS_MILEAGE				(EE_OBL_BASE_ADDR+0x17C)			// 测试里程
// 6 Bytes left

#define			EE_PIN_CODE							(EE_OBL_BASE_ADDR+0x188)		// 8B

#define			EE_SERVER_LOGIN_USER				(EE_OBL_BASE_ADDR+0x190)		// 10B
#define			EE_SERVER_LOGIN_PASSWORD			(EE_OBL_BASE_ADDR+0x19a)		// 6B

//#define			EE_HEARTBEAT_TIME					(EE_OBL_BASE_ADDR+0x1a0)		// 心跳包发送间隔 4B

#define			EE_SEND_INTERVAL_BASE					(EE_OBL_BASE_ADDR+0x1A0)		// 
#define			EE_JT_HEART_BEAT_TIME					(EE_OBL_BASE_ADDR+0x1A0)		// 4B 心跳包发送间隔
#define			EE_JT_SEND_TIME_NO_DRIVER	      	(EE_OBL_BASE_ADDR+0x1A4)		// 4B 无驾驶员时汇报时间间隔
#define			EE_JT_SEND_TIME_IDEL     	      	(EE_OBL_BASE_ADDR+0x1A8)		// 4B 休眠时汇报时间间隔
#define			EE_JT_SEND_TIME_ALARM		 	   	(EE_OBL_BASE_ADDR+0x1AC)		// 4B 紧急报警时时间汇报间隔
#define			EE_JT_SEND_TIME_DEFAULET         	(EE_OBL_BASE_ADDR+0x1B0)		// 4B 缺省时间汇报间隔
#define			EE_JT_SEND_DISTANCE_DEFAULET     	(EE_OBL_BASE_ADDR+0x1B4)		// 4B 缺省距离汇报间隔
#define			EE_JT_SEND_DISTANCE_NO_DRIVER	   	(EE_OBL_BASE_ADDR+0x1B8)		// 4B 无驾驶员时汇报距离间隔
#define			EE_JT_SEND_DISTANCE_IDEL         	(EE_OBL_BASE_ADDR+0x1BC)		// 4B 休眠时汇报距离间隔
#define			EE_JT_SEND_DISTANCE_ALARM	      	(EE_OBL_BASE_ADDR+0x1C0)		// 4B 紧急报警时距离汇报间隔
#define			EE_JT_SEND_INTERVAL_MODE			   (EE_OBL_BASE_ADDR+0x1C4)		// 1B 位置汇报策略
#define			EE_JT_SEND_INTERVAL_METHOD		   	(EE_OBL_BASE_ADDR+0x1C5)		// 1B 位置汇报方案
#define			EE_JT_SEND_ANGLE	               	(EE_OBL_BASE_ADDR+0x1C6)		// 1B 拐点补传角度
#define			EE_JT_SEND_RSV_1BYTE						(EE_OBL_BASE_ADDR+0x1C7)
#define			EE_JT_SEND_T_SECOND						(EE_OBL_BASE_ADDR+0x1C8)
#define			EE_JT_SEND_T_CONTINUED					(EE_OBL_BASE_ADDR+0x1CC)

#define			EE_JT_SHIELD_WORD	               	(EE_OBL_BASE_ADDR+0x1D3)		// 4B 报警屏蔽字
#define			EE_JT_TEXT_SMS_WORD	            	(EE_OBL_BASE_ADDR+0x1D7)		// 4B 发送文本短信SMS开关
#define			EE_JT_TAKE_PHOTO_WORD	         	(EE_OBL_BASE_ADDR+0x1DB)		// 4B 报警拍照开关
#define			EE_JT_SAVE_PHOTO_WORD	         	(EE_OBL_BASE_ADDR+0x1DF)		// 4B 保存报警拍照照片开关
#define			EE_JT_KEY_WORD	                 	(EE_OBL_BASE_ADDR+0x1E3)		// 4B 关键报警标识

#define			EE_JT_VIDIO_QUALITY	            	(EE_OBL_BASE_ADDR+0x1E7)		// 1B 视频/照片质量
#define			EE_JT_VIDIO_LIGHT	              	(EE_OBL_BASE_ADDR+0x1E8)		// 1B 视频/照片亮度
#define			EE_JT_VIDIO_CONTRAST	            (EE_OBL_BASE_ADDR+0x1E9)		// 1B 视频/照片对比度
#define			EE_JT_VIDIO_SATULATION	         	(EE_OBL_BASE_ADDR+0x1EA)		// 1B 视频/照片饱和度
#define			EE_JT_VIDIO_CALOUR	            	(EE_OBL_BASE_ADDR+0x1EB)		// 1B 视频/照片色度

// 14 Bytes left

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	地址0x200~0x2ff为IP和域名存储区
*/
#define			EE_IP_PORT							(EE_OBL_BASE_ADDR+0x200)		// 4组IP地址，128B
#define			EE_IP_PORT_TCP						(EE_OBL_BASE_ADDR+0x200)		// 32B,IP地址	,服务器1 TCP通道
#define			EE_IP_PORT_UDP						(EE_OBL_BASE_ADDR+0x220)		// 32B,IP地址1	,服务器1 UDP通道
#define			EE_IP_PORT_TCP1						(EE_OBL_BASE_ADDR+0x240)		// 32B,IP地址2	,服务器2 TCP通道	
#define			EE_IP_PORT_UDP1						(EE_OBL_BASE_ADDR+0x260)		// 32B,IP地址2 ,服务器2 UDP通道
#define			EE_DOMAIN_ADDR						(EE_OBL_BASE_ADDR+0x280)		// 32B,监控中心域名地址
#define			EE_APN								(EE_OBL_BASE_ADDR+0x2a0)		// 16B,ANP
#define			EE_CENTER_NUMBER					(EE_OBL_BASE_ADDR+0x2b0)		// 16B,监控中心服务电话号码0
#define			EE_CENTER_NUMBER1					(EE_OBL_BASE_ADDR+0x2c0)		// 16B,监控中心服务电话号码1
#define			EE_USER_NUMBER						(EE_OBL_BASE_ADDR+0x2d0)		// 16B,用户关联电话号码0
#define			EE_USER_NUMBER1						(EE_OBL_BASE_ADDR+0x2e0)		// 16B,用户关联电话号码1
#define			EE_DOMAIN_PORT						(EE_OBL_BASE_ADDR+0x2f0)		// 5B, 域名解析的IP端口号，
                                                  
#define			EE_ENABLE_IP						(EE_OBL_BASE_ADDR+0x2f8)		// 16B,下面16字节
#define			EE_ENABLE_TCP						(EE_OBL_BASE_ADDR+0x2f8)		// 1B, 
#define			EE_ENABLE_UDP						(EE_OBL_BASE_ADDR+0x2f9)		// 1B, 
#define			EE_ENABLE_TCP1						(EE_OBL_BASE_ADDR+0x2fa)		// 1B, 
#define			EE_ENABLE_UDP1						(EE_OBL_BASE_ADDR+0x2fb)		// 1B, 
#define			EE_ENABLE_TCP2						(EE_OBL_BASE_ADDR+0x2fc)		// 1B, 
#define			EE_ENABLE_UDP2						(EE_OBL_BASE_ADDR+0x2fd)		// 1B, 域名使能标志
#define			EE_ENABLE_APN						(EE_OBL_BASE_ADDR+0x2fe)		// 1B, APN使能标志
//#define			EE_NEED_REG_SERVER					    (EE_OBL_BASE_ADDR+0x3FF)		// 1B, 需要注册
#define			EE_NEED_REG_SERVER					(EE_OBL_BASE_ADDR+0x2FF)		// 1B, 需要注册

#define			EE_ENABLE_IC_AUTH_TCP				(EE_OBL_BASE_ADDR+0x300)		// 1B, 
#define			EE_ENABLE_IC_AUTH_UDP				(EE_OBL_BASE_ADDR+0x301)		// 1B, 
#define			EE_ENABLE_IC_AUTH_B_TCP				(EE_OBL_BASE_ADDR+0x302)		// 1B, 
#define			EE_ENABLE_IC_AUTH_B_UDP				(EE_OBL_BASE_ADDR+0x303)		// 1B, 

//#define			EE_JT_ENABLE_M_DNS					(EE_OBL_BASE_ADDR+0x300)		// 1B,  主服务器
//#define			EE_JT_ENABLE_M_DOMAIN				(EE_OBL_BASE_ADDR+0x301)		// 1B, 域名使能标
//#define			EE_JT_ENABLE_B_DNS					(EE_OBL_BASE_ADDR+0x302)		// 1B,  备份服务器
//#define			EE_JT_ENABLE_B_DOMAIN				(EE_OBL_BASE_ADDR+0x303)		// 1B, 域名使能标
//#define			EE_JT_ENABLE_C_DNS					(EE_OBL_BASE_ADDR+0x304)		// 1B, 临时或升级服务器
//#define			EE_JT_ENABLE_C_DOMAIN				(EE_OBL_BASE_ADDR+0x305)		// 1B, 域名使能标

#define			   EE_CREATE_USER_FLAG				 (EE_OBL_BASE_ADDR+0x304)			// 1B, 使用前锁定（快速开户）

#define			   EE_OVERTIME_DRIVE_UPDATE_FLAG	 (EE_OBL_BASE_ADDR+0x306) //	 1B // 超时驾驶更新标志 0,更新超时计数，1：不更新计数
// 10 Bytes left



#define			EE_SIM_NUMBER						(EE_OBL_BASE_ADDR+0x310)		// 16B,本机SIM卡手机号码
#define			EE_HELP_NUMBER						(EE_OBL_BASE_ADDR+0x320)		// 16B,求助号码
#define			EE_SMS_CENTER_NUMBER				(EE_OBL_BASE_ADDR+0x330)		// 16B,移动公司短消息中心号码

// 加速度传感器参数
#define			EE_ACCEL_PARA_BASE					(EE_OBL_BASE_ADDR+0x340)		// 加速度传感参数基地址
#define			EE_ACCEL_RESOLUTION					(EE_ACCEL_PARA_BASE+0x00)		// 分辨率	 1B
#define			EE_ACCEL_G_RANG						(EE_ACCEL_PARA_BASE+0x01)		// 重力加速度范围	 1B
#define			EE_ACCEL_TILT_PARA					(EE_ACCEL_PARA_BASE+0x02)		// 倾斜参数	 5B
#define			EE_ACCEL_WUF_PARA					(EE_ACCEL_PARA_BASE+0x07)		// 运动（移动）参数	 4B
#define			EE_ACCEL_TAP_PARA					(EE_ACCEL_PARA_BASE+0x0b)		// 碰撞（单叩或双叩）参数	 9B
#define			EE_ACCEL_MASK_PARA					(EE_ACCEL_PARA_BASE+0x14)		// 屏蔽参数（倾斜和碰撞六个方向，以及移动的三个方向的屏蔽）  4B
#define			EE_ACCEL_INT_PARA					(EE_ACCEL_PARA_BASE+0x18)		// 中断设置参数	 4B

// 24 Bytes left

// www.guangzhouguoyedianzi.com.cn
	
#define			EE_HEARTBEAT_PACK					(EE_OBL_BASE_ADDR+0x380)		// 64B,心跳包内容
//#define			EE_REGISTER_PACK					(EE_OBL_BASE_ADDR+0x3c0)		// 64B,注册包内容

#define			LEN_EE_ACCEL_ADD_DEC_PARA		30
#define			EE_ACCEL_ADD_DEC_PARA			(EE_OBL_BASE_ADDR+0x3c0)	  	// 18*2 = 36B



#define			EE_CAN_SPECIFY_ID					(EE_OBL_BASE_ADDR+0x400)		// 16*4B，指定的16个CAN数据帧ID
#define			EE_CAN_ID_NUMBER					(EE_OBL_BASE_ADDR+0x440)		// 4B，ID个数
#define			EE_CAN_GET_INTERVAL					(EE_OBL_BASE_ADDR+0x444)		// 4B，

 

// add 2013.3
#define			EE_JT_ELE_RAID_RADIUS				(EE_OBL_BASE_ADDR+0x446)	 // 电子围栏半径	（非法位移阈值） 2B
#define			EE_JT_OVER_SPEED_PRE_ALARM			(EE_OBL_BASE_ADDR+0x448)	 // 超速报警预警差值 				 2B
#define			EE_JT_FATIGUE_DRIVE_PRE_ALARM       (EE_OBL_BASE_ADDR+0x44A)	 	 // 疲劳驾驶预警差值				 2B
#define			EE_JT_CRASH_ALARM_PARA				(EE_OBL_BASE_ADDR+0x44C)	 // 碰撞报警参数设置				 2B
#define			EE_JT_TILT_ALARM_PARA				(EE_OBL_BASE_ADDR+0x44E)	 // 侧翻报警参数设置				 2B

#define			EE_JT_FIXED_TIME_TAKE_PHOTO_CTL		(EE_OBL_BASE_ADDR+0x450)	 // 定时拍照控制					 4B
#define			EE_JT_DISTANCE_TIME_TAKE_PHOTO_CTL	(EE_OBL_BASE_ADDR+0x454)	 // 定距拍照控制					 4B

#define			EE_JT_GNNS_POS_MODE					(EE_OBL_BASE_ADDR+0x458)	 // GNSS 定位模式					 1B
#define			EE_JT_GNSS_BAUD_RATE 				(EE_OBL_BASE_ADDR+0x459)	 // GNSS波特率						 1B
#define			EE_JT_GNSS_DATA_OUTPUT_FREQUENCY 	(EE_OBL_BASE_ADDR+0x45A)	 // GNSS模块详细定位数据输出频率	 1B
#define			EE_JT_GNSS_DATA_COLLECT_FREQUENCY 	(EE_OBL_BASE_ADDR+0x45B)	 // GNSS模块详细定位数据采集频率	 4B
#define			EE_JT_GNSS_DATA_UPLOAD_MODE 		(EE_OBL_BASE_ADDR+0x45F)	 // GNSS模块详细定位数据上传方式	 1B
#define			EE_JT_GNSS_DATA_UPLOAD_SET 			(EE_OBL_BASE_ADDR+0x460)     // GNSS模块详细定位数据上传设置	 4B

#define		EE_JT_CAN_CHAN_1_COLLECT_TIME_INTERVAL  (EE_OBL_BASE_ADDR+0x464) 	 // CAN总线通道1 采集时间间隔(ms)，0表示不采集 4B
#define		EE_JT_CAN_CHAN_1_UPLOAD_TIME_INTERVAL 	(EE_OBL_BASE_ADDR+0x468)     // CAN总线通道1 上传时间间隔(s)，0表示不采集 2B
#define		EE_JT_CAN_CHAN_2_COLLECT_TIME_INTERVAL 	(EE_OBL_BASE_ADDR+0x46A)	 // CAN总线通道2 采集时间间隔(ms)，0表示不采集 4B
#define		EE_JT_CAN_CHAN_2_UPLOAD_TIME_INTERVAL 	(EE_OBL_BASE_ADDR+0x46E)     // CAN总线通道2 上传时间间隔(s)，0表示不采集 2B

		

// 17 Bytes left

#define			EE_PICTURE_TOTAL_NUM				(EE_OBL_BASE_ADDR+0x480)		// 4B+4B	总张数4B,分辨率1B,对比度1B,光度1B,1B

#define		EE_MANUL_ALARM_EVENT_ID				(EE_OBL_BASE_ADDR+0x4F0)		//  人工报警事件ID, 2B

// add by xzb			部标 终端注册所需信息
#define 		EE_PROVINCE_REGION_ID				(EE_OBL_BASE_ADDR+0x500)      // 省域ID	    2B
#define 		EE_PREFECTURE_REGION_ID			   (EE_OBL_BASE_ADDR+0x502)      // 市县域ID	    2B
#define 		EE_VENDOR_ID							(EE_OBL_BASE_ADDR+0x504)      // 制造商ID		 5B
#define 		EE_TERMINAL_TYPE						(EE_OBL_BASE_ADDR+0x509)		// 终端型号		 20B
#define 		EE_TERMINAL_ID							(EE_OBL_BASE_ADDR+0x51D)		// 终端ID		 7B
#define		FM_CAR_COLOUR			   			(EE_OBL_BASE_ADDR+0x524)		  // 车辆颜色	   1B
#define		FM_VEHICLE_CODE						(EE_OBL_BASE_ADDR+0x525)		  // 车牌号码	   12B 
#define		FM_VEHICLE_VIN							(EE_OBL_BASE_ADDR+0x531)		  // 车辆VIN号	  17B
#define		FM_VEHICLE_CLASS						(EE_OBL_BASE_ADDR+0x542)		  // 车辆分类	   12B


// 2 Bytes left

#define			EE_AUTH_CODE						(EE_OBL_BASE_ADDR+0x550)      // 鉴权码       32B




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	JT808  参数设置
*/
#define			EE_JT_MAIN_SERVER				    (EE_OBL_BASE_ADDR+0x570)	   // 96B 主服务器连接参数
#define			EE_JT_M_APN						   	(EE_OBL_BASE_ADDR+0x570)		// 32B 主服务器连接APN
#define			EE_JT_M_USER						(EE_OBL_BASE_ADDR+0x590)		// 16B 主服务器连接用户名
#define			EE_JT_M_SERIERS						(EE_OBL_BASE_ADDR+0x5A0)		// 8B 主服务器连接密码
#define			EE_JT_M_IP_DNS					   	(EE_OBL_BASE_ADDR+0x5A8)		// 32B IP服务器地址或DNS，

#define			EE_JT_BACK_SERVER				    (EE_OBL_BASE_ADDR+0x5C8)		// 96B 备份服务器连接参数
#define			EE_JT_B_APN						   	(EE_OBL_BASE_ADDR+0x5C8)		// 32B 备份服务器连接APN
#define			EE_JT_B_USER						(EE_OBL_BASE_ADDR+0x5E8)		// 16B 备份服务器连接用户名
#define			EE_JT_B_SERIERS						(EE_OBL_BASE_ADDR+0x5F8)		// 8B 备份服务器连接密码
#define			EE_JT_B_IP_DNS					   	(EE_OBL_BASE_ADDR+0x600)		// 32B 备份服务器IP地址或DNS

#define			EE_JT_TCP_PORT					   	(EE_OBL_BASE_ADDR+0x620)		// 4B 服务器TCP端口
#define			EE_JT_UDP_PORT					   	(EE_OBL_BASE_ADDR+0x624)		// 4B 服务器UDP端口


#define			EE_JT_TMP_TCP_IP					(EE_OBL_BASE_ADDR+0x628)	   // 20B	存放临时IP	 // added 2013.4
#define			EE_JT_TMP_TCP_PORT					(EE_OBL_BASE_ADDR+0x63C)	   // 4B	存放临时端口

#define			EE_JT_CENTER_NUMBER			   		(EE_OBL_BASE_ADDR+0x640)		// 16B 平台电话号码
#define			EE_JT_RESET_NUMBER					(EE_OBL_BASE_ADDR+0x650)		// 16B 复位号码
#define			EE_JT_RESET_FACTORY_NUMBER	   		(EE_OBL_BASE_ADDR+0x660)		// 16B 恢复出厂设置号码
#define			EE_JT_CENTER_SMS_NUMBER		   		(EE_OBL_BASE_ADDR+0x670)		// 16B 平台短信号码
#define			EE_JT_TEXT_SMS_NUMBER				(EE_OBL_BASE_ADDR+0x680)		// 16B 接收SMS文本报警号码
#define			EE_JT_GET_CALL_MODE				   	(EE_OBL_BASE_ADDR+0x690)		// 1B 电话接听1策略
#define			EE_JT_CALL_TIME_LIMIT				(EE_OBL_BASE_ADDR+0x691)		// 4B 每次通话时长限制
#define			EE_JT_CALL_TIME_LIMIT_MONTH			(EE_OBL_BASE_ADDR+0x695)		// 4B 每月通话时长限制
#define			EE_JT_JT_NUMBER						(EE_OBL_BASE_ADDR+0x699)		// 16B 监听电话号码
#define			EE_JT_PRIVILILEGE_SMS_NUMBER   		(EE_OBL_BASE_ADDR+0x6A9)		// 16B 监管平台特权短信号码

// 7 Bytes  left

#define			EE_JT_RESEND_MACHINING				(EE_OBL_BASE_ADDR+0x6C0)			// 4B 重传机制
#define			EE_JT_TCP_ACK_TIME				    (EE_OBL_BASE_ADDR+0x6C0)			// 4B TCP应答超时时间
#define			EE_JT_TCP_RESEND_TIMES			    (EE_OBL_BASE_ADDR+0x6C4)			// 4B TCP重传次数
#define			EE_JT_UDP_ACK_TIME     	         	(EE_OBL_BASE_ADDR+0x6C8)		// 4B UDP应答超时时间
#define			EE_JT_UDP_RESEND_TIMES		   		(EE_OBL_BASE_ADDR+0x6CC)		// 4B UDP重传次数
#define			EE_JT_SMS_ACK_TIME     	         	(EE_OBL_BASE_ADDR+0x6D0)		// 4B SMS应答超时时间
#define			EE_JT_SMS_RESEND_TIMES		   		(EE_OBL_BASE_ADDR+0x6D4)		// 4B SMS重传次数

// 14 Bytes left 
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	JT808  终端连接控制
*/
#define			EE_JT_NET_CTR				        (EE_OBL_BASE_ADDR+0x6F0)	   // 1B 连接控制
#define			EE_JT_NET_CTR_APN					(EE_OBL_BASE_ADDR+0x6F1)		// 32B 升级或指定服务器连接APN
#define			EE_JT_NET_CTR_USER					(EE_OBL_BASE_ADDR+0x711)		// 16B 升级或指定服务器连接用户名
#define			EE_JT_NET_CTR_SERIERS				(EE_OBL_BASE_ADDR+0x721)		// 8B 升级或指定服务器连接密码
#define			EE_JT_NET_CTR_IP_DNS				(EE_OBL_BASE_ADDR+0x729)		// 32B 升级或指定服务器IP地址或DNS， 
#define			EE_JT_NET_CTR_TCP_PORT			  	(EE_OBL_BASE_ADDR+0x749)		// 2B 升级或指定服务器TCP端口
#define			EE_JT_NET_CTR_UDP_PORT			 	(EE_OBL_BASE_ADDR+0x74B)		// 2B 升级或指定服务器UDP端口
#define			EE_JT_NET_CTR_CPY_ID				(EE_OBL_BASE_ADDR+0x74D)	   // 5B 制造商ID
#define			EE_JT_NET_CTR_JQM					(EE_OBL_BASE_ADDR+0x752)		// 16B 指定服务器的鉴权码
#define			EE_JT_NET_CTR_HVS					(EE_OBL_BASE_ADDR+0x762)		// 16B 硬件版本号
#define			EE_JT_NET_CTR_HWVS				    (EE_OBL_BASE_ADDR+0x772)		// 16B 固件版本号
#define			EE_JT_NET_CTR_URL_ADDR			  	(EE_OBL_BASE_ADDR+0x782)		// 32B URL地址 
#define			EE_JT_NET_CTR_NET_TIME			  	(EE_OBL_BASE_ADDR+0x7A2)		// 2B 连接时限			  单位分钟
 
/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	JT808  位置跟踪控制
*/
#define			EE_JT_PF_TIME				         (EE_OBL_BASE_ADDR+0x7A4)	   // 2B 跟踪时间间隔
#define			EE_JT_PF_TIME_LIMIT		             (EE_OBL_BASE_ADDR+0x7A6)	   // 4B 跟踪时间

//#define	      EE_JT_ADSW_SET							(EE_OBL_BASE_ADDR+0x7AA)		// 2B AD输入通道 选择高/低开关量检测

#define			EE_JT_IC_AUTH_M_IP_DNS				(EE_OBL_BASE_ADDR+0x7AA)		// 32B IC卡认证主IP服务器地址或DNS
#define			EE_JT_IC_AUTH_B_IP_DNS				(EE_OBL_BASE_ADDR+0x7CA)		// 32B IC卡认证备份IP服务器地址或DNS
#define			EE_JT_IC_AUTH_TCP_PORT				(EE_OBL_BASE_ADDR+0x7EA)		// 4B 服务器TCP端口
#define			EE_JT_IC_AUTH_UDP_PORT				(EE_OBL_BASE_ADDR+0x7EE)		// 4B 服务器UDP端口

// 14 Bytes left




#define 			LEN_PROVINCE_REGION_ID				2    // 省域ID长度
#define 			LEN_PREFECTURE_REGION_ID			2    // 市县域ID长度
#define 			LEN_EE_VENDOR_ID					5	  // 制造商ID长度		 5B




/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	每分钟存储的数据,相当于系统日志,以方便系统维护,故障分析. 占用256字节 
*/
#define			EE_SYSTEM_LOG_BASE					(EE_OBL_BASE_ADDR+0x800)		// 32+32+32+32+32	包含以下，看结构SystemLog (系统日志)

#define			EE_SYSTEM_LOG_GPRS_BASE				 EE_SYSTEM_LOG_BASE
#define			EE_SYS_GPRS_DIAL_CNT					(EE_SYSTEM_LOG_GPRS_BASE+0*4)			// GPRS
#define			EE_SYS_GPRS_DIAL_OK					(EE_SYSTEM_LOG_GPRS_BASE+1*4)
#define			EE_SYS_GPRS_ONLINE_TIME				(EE_SYSTEM_LOG_GPRS_BASE+2*4)
#define			EE_SYS_GPRS_REC_TOTAL_PACK			(EE_SYSTEM_LOG_GPRS_BASE+3*4)
#define			EE_SYS_GPRS_REC_TOTAL_BYTES		(EE_SYSTEM_LOG_GPRS_BASE+4*4)
#define			EE_SYS_GPRS_PUSH_TOTAL_BYTES		(EE_SYSTEM_LOG_GPRS_BASE+5*4)
#define			EE_SYS_GPRS_SEND_TOTAL_BYTES		(EE_SYSTEM_LOG_GPRS_BASE+6*4)
#define			EE_SYS_GPRS_SEND_TOTAL_PACK		(EE_SYSTEM_LOG_GPRS_BASE+7*4)

#define			EE_SYSTEM_LOG_GSM_BASE				(EE_SYSTEM_LOG_GPRS_BASE+32)			// GSM
#define			EE_SYS_GSM_COOL_BOOT					(EE_SYSTEM_LOG_GSM_BASE+0*4)
#define			EE_SYS_GSM_SIM_ERROR					(EE_SYSTEM_LOG_GSM_BASE+1*4)
#define			EE_SYS_GSM_UNREG						(EE_SYSTEM_LOG_GSM_BASE+2*4)
#define			EE_SYS_GSM_EBB_SIGNAL				(EE_SYSTEM_LOG_GSM_BASE+3*4)
#define			EE_SYS_GSM_REC_SMS					(EE_SYSTEM_LOG_GSM_BASE+4*4)
#define			EE_SYS_GSM_SEND_SMS					(EE_SYSTEM_LOG_GSM_BASE+5*4)
#define			EE_SYS_GSM_REG_SERVER				(EE_SYSTEM_LOG_GSM_BASE+6*4)
#define			EE_SYS_GSM_REG_SERVER_OK			(EE_SYSTEM_LOG_GSM_BASE+7*4)

#define			EE_SYSTEM_LOG_GG_BASE				(EE_SYSTEM_LOG_GSM_BASE+32)			// GG
#define			EE_SYS_GG_CALLER_TIMES				(EE_SYSTEM_LOG_GG_BASE+0*4)
#define			EE_SYS_GG_CALLER_TIME				(EE_SYSTEM_LOG_GG_BASE+1*4)
#define			EE_SYS_GG_CALLED_TIMES				(EE_SYSTEM_LOG_GG_BASE+2*4)
#define			EE_SYS_GG_CALLED_TIME				(EE_SYSTEM_LOG_GG_BASE+3*4)
#define			EE_SYS_GG_SAVE_POS_INDEX			(EE_SYSTEM_LOG_GG_BASE+4*4)
#define			EE_SYS_GG_SEND_POS_INDEX			(EE_SYSTEM_LOG_GG_BASE+5*4)
#define			EE_SYS_GG_SAVE_ALARM_INDEX			(EE_SYSTEM_LOG_GG_BASE+6*4)
#define			EE_SYS_GG_SEND_ALARM_INDEX			(EE_SYSTEM_LOG_GG_BASE+7*4)

#define			EE_SYSTEM_LOG_GPS_BASE				(EE_SYSTEM_LOG_GG_BASE+32)				// GPS
#define			EE_SYS_GPS_SPEED_CALCULUS			(EE_SYSTEM_LOG_GPS_BASE+0*4)			// 8B, 速度积分值(0.01km)
#define			EE_SYS_GPS_TOTAL_MILEAGE			(EE_SYSTEM_LOG_GPS_BASE+2*4)			// 4B, 总里程(公里)
#define			EE_SYS_GPS_PACK_TOTAL_NUMBER		(EE_SYSTEM_LOG_GPS_BASE+3*4)			// 4B，GPS打包总数
#define			EE_SYS_GPS_PACK_VALID_NUMBER		(EE_SYSTEM_LOG_GPS_BASE+4*4)			// 4B, 打包的位定条数
#define			EE_SYS_GPS_RESTART_COUNT       	(EE_SYSTEM_LOG_GPS_BASE+5*4)			// 4B, GPS模块重启次数
#define			EE_SYS_GPS_VALID_TIME				(EE_SYSTEM_LOG_GPS_BASE+6*4)			// 4B，有效定位时间
#define			EE_SYS_GPS_DATA_ERROR_COUNT		(EE_SYSTEM_LOG_GPS_BASE+7*4)			// 4B, GPS数据错误次数


#define			EE_SYSTEM_LOG_VDR_BASE				(EE_SYSTEM_LOG_GPS_BASE+32)			// VDR
#define			EE_SYS_VDR_HIGH_SPEED_PULS_CNT	(EE_SYSTEM_LOG_VDR_BASE+0*4)
#define			EE_SYS_VDR_SPEED_PULS_CNT			(EE_SYSTEM_LOG_VDR_BASE+1*4)
#define			EE_SYS_VDR_MOTOR_PULS_CNT			(EE_SYSTEM_LOG_VDR_BASE+2*4)
#define			EE_SYS_VDR_MOTOR_RUN_TIME			(EE_SYSTEM_LOG_VDR_BASE+3*4)
#define			EE_SYS_VDR_SYS_WORK_TIME			(EE_SYSTEM_LOG_VDR_BASE+4*4)		
#define			EE_SYS_VDR_SYS_LAST_RUN_TIME		(EE_SYSTEM_LOG_VDR_BASE+5*4)			
#define			EE_SYS_VDR_POWER1_2					(EE_SYSTEM_LOG_VDR_BASE+6*4)			// 主电源1，备用电源2
#define			EE_SYS_VDR_POWER3_4					(EE_SYSTEM_LOG_VDR_BASE+7*4)		   // 备用电源3， 备用电源4


#define			EE_SYSTEM_LOG_RESET_BASE			(EE_SYSTEM_LOG_VDR_BASE+32)			// ResetCnt
#define			EE_SYS_RESET_SYSTEM_CNT	         (EE_SYSTEM_LOG_RESET_BASE+0*4)	   // 系统重启总次数
#define			EE_SYS_RESET_UPDATE_CNT			   (EE_SYSTEM_LOG_RESET_BASE+1*4)		// 程序升级重启次数
#define			EE_SYS_RESET_GPRS_CNT				(EE_SYSTEM_LOG_RESET_BASE+2*4)		// GPRS故障重启次数
#define			EE_SYS_RESET_TASK_OVERTIME_CNT	(EE_SYSTEM_LOG_RESET_BASE+3*4)		// 任务超时重启次数
#define			EE_SYS_RESET_POWER_ON_CNT			(EE_SYSTEM_LOG_RESET_BASE+4*4)		// 上电重启次数
#define			EE_SYS_RESET_WATCHDOG_CNT			(EE_SYSTEM_LOG_RESET_BASE+5*4)	   // 看门狗重启次数
#define			EE_SYS_RESET_WAKEUP_CNT				(EE_SYSTEM_LOG_RESET_BASE+6*4)		// 休眠后唤醒重启次数
#define			EE_SYS_RESET_PIN_RESET_CNT			(EE_SYSTEM_LOG_RESET_BASE+7*4)		// 复位脚控制的重启


#define			EE_SYSTEM_LOG_GG1_BASE				(EE_SYSTEM_LOG_RESET_BASE+32)			// GG1
#define			EE_SYS_GG1_LON_VAL	         	(EE_SYSTEM_LOG_GG1_BASE+0*4)			// 4B，经度，放大了10e7倍，正为东经，负为西经
#define			EE_SYS_GG1_LAT_VAL	            (EE_SYSTEM_LOG_GG1_BASE+1*4)		   // 4B，纬度，放大了10e7倍，正为北纬，负为南纬
#define			EE_SYS_GG1_RTC_SECOND	         (EE_SYSTEM_LOG_GG1_BASE+2*4)			// 4B，时间，统一为秒，2000年1月1日00:00:00为第0秒
#define			EE_SYS_GG1_SPEED_INFO	         (EE_SYSTEM_LOG_GG1_BASE+3*4)	      // 1字节GPS速度，1字节高度，1字节GPS水平精度因子，1字节卫星个数
#define			EE_SYS_GG1_GSM_BASK_STATION	   (EE_SYSTEM_LOG_GG1_BASE+4*4)		   // 2字节基站信息1，2字节基站信息2
#define			EE_SYS_GG1_GSM_INFO	         	(EE_SYSTEM_LOG_GG1_BASE+5*4)		   // 1字节基站信息3, 1字节GSM信息强度，1字节环境温度，1字节保留
#define			EE_SYS_GG1_VEHICLE_STATUS1	     	(EE_SYSTEM_LOG_GG1_BASE+6*4)			// 车辆状态标志位，前32位
#define			EE_SYS_GG1_VEHICLE_STATUS2	      (EE_SYSTEM_LOG_GG1_BASE+7*4)			// 车辆状态标志位，前32位


#define			EE_SYSTEM_LOG_RSV_BASE				(EE_SYSTEM_LOG_GG1_BASE+32)			// Rsv8   32B	 保留

	

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
   电子围栏属性数据（电子围栏数据存储在Flash）。圆形、矩形、多边形、路线，共4类，每类最多个数为128
   每类属性存储在铁电的长度为32B
   前16B：总个数、当前所在围栏号、
   后16B：128Bits，每个Bits代表围栏的有效性，0：无效，无需检测该围栏，1：有效，需检测
   全局变量结构定义：RoundFenceAttr,RectFenceAttr,PolygonFenceAttr,RoadAttr
*/
// #define			LEN_EE_FENCE_ATTR						64 	// 在UserGobals.h里定义
#define			EE_ELE_FENCE_ATTR_BASE_ADDR		(EE_OBL_BASE_ADDR+0x900)
#define			EE_ROUND_FENCE_ATTR					(EE_OBL_BASE_ADDR+0x900)	// 64B
#define			EE_RECTANGLE_FENCE_ATTR				(EE_OBL_BASE_ADDR+0x940)	// 64B，矩形围栏属性，同上
#define			EE_POLYGON_FENCE_ATTR				(EE_OBL_BASE_ADDR+0x980)	// 64B，多边形围栏，
#define			EE_ROAD_ATTR							(EE_OBL_BASE_ADDR+0x9C0)	// 64B，路线属性，




#define        EE_IN_RECT_AREA_ALARM_MSG			(EE_OBL_BASE_ADDR+0xA00)		// 应付测试使用，矩形围栏报警信息
#define        EE_OUT_RECT_AREA_ALARM_MSG			(EE_OBL_BASE_ADDR+0xA20)		//
#define        EE_ROAD_LINE_ALARM_MSG				(EE_OBL_BASE_ADDR+0xA40)		// 应付测试使用，偏离路线报警信息


#define			EE_ONE_ROAD_LINE_DATA				(EE_OBL_BASE_ADDR+0xC00)



#define			EE_CALL_TIME_MONTH				  (EE_OBL_BASE_ADDR+0xCF0)		//  月度通话时长，  4B
#define			EE_TIME_MONTH				        (EE_OBL_BASE_ADDR+0xCF4)		//  一个周期月，   4B

#define			EE_GSM_IMEI							(EE_OBL_BASE_ADDR+0xCF8)		// GSM模块 IMEI	号15位Asccii码	  15B
#define			EE_SIM_CARD_ICCID					(EE_OBL_BASE_ADDR+0xD07)		// SIM卡串号 20位Asccii码		   20B

#define			EE_JT_DRIVER_NAME_LEN				(EE_OBL_BASE_ADDR+0xD1B)		// 驾驶员姓名长度  1B
#define			EE_JT_DRIVER_NAME					(EE_OBL_BASE_ADDR+0xD1C)		// 驾驶员姓名  20B
#define			EE_JT_DRIVER_CARD_ID				(EE_OBL_BASE_ADDR+0xD30)		//	从业资格证号   20B
#define			EE_JT_CERTIFICATE_NAME_LEN			(EE_OBL_BASE_ADDR+0xD44)	    // 发证机构名称长度 1B
#define			EE_JT_CERTIFICATE_NAME				(EE_OBL_BASE_ADDR+0xD45)	    // 发证机构名称长度 60B
#define			EE_JT_DRIVER_CARD_VALID_DATE		(EE_OBL_BASE_ADDR+0xD81)		// 证件有效期 4B

#define			LEN_JT_DRIVER_IC_CARD_INFO					106



///  CAN总线使用128B，共16个ID
#define		EE_JT_CAN_ID_COLLECT_SET_BASE 				(EE_OBL_BASE_ADDR+0xD85)	 
#define		EE_JT_CAN_ID_COLLECT_SET_0					(EE_JT_CAN_ID_COLLECT_SET_BASE+0*8)		// CAN总线ID单独采集设置1	8B
#define		EE_JT_CAN_ID_COLLECT_SET_1					(EE_JT_CAN_ID_COLLECT_SET_BASE+1*8)		// CAN总线ID单独采集设置2	8B
#define		EE_JT_CAN_ID_COLLECT_SET_2					(EE_JT_CAN_ID_COLLECT_SET_BASE+2*8)		// CAN总线ID单独采集设置3	8B
#define		EE_JT_CAN_ID_COLLECT_SET_3					(EE_JT_CAN_ID_COLLECT_SET_BASE+3*8)		// CAN总线ID单独采集设置4	8B
#define		EE_JT_CAN_ID_COLLECT_SET_4					(EE_JT_CAN_ID_COLLECT_SET_BASE+4*8)		// CAN总线ID单独采集设置5	8B
#define		EE_JT_CAN_ID_COLLECT_SET_5					(EE_JT_CAN_ID_COLLECT_SET_BASE+5*8)		// CAN总线ID单独采集设置6	8B
#define		EE_JT_CAN_ID_COLLECT_SET_6					(EE_JT_CAN_ID_COLLECT_SET_BASE+6*8)		// CAN总线ID单独采集设置7	8B
#define		EE_JT_CAN_ID_COLLECT_SET_7					(EE_JT_CAN_ID_COLLECT_SET_BASE+7*8)		// CAN总线ID单独采集设置8	8B
#define		EE_JT_CAN_ID_COLLECT_SET_8					(EE_JT_CAN_ID_COLLECT_SET_BASE+8*8)		// CAN总线ID单独采集设置9	8B
#define		EE_JT_CAN_ID_COLLECT_SET_9					(EE_JT_CAN_ID_COLLECT_SET_BASE+9*8)		// CAN总线ID单独采集设置10	8B
#define		EE_JT_CAN_ID_COLLECT_SET_10					(EE_JT_CAN_ID_COLLECT_SET_BASE+10*8)		// CAN总线ID单独采集设置11	8B
#define		EE_JT_CAN_ID_COLLECT_SET_11					(EE_JT_CAN_ID_COLLECT_SET_BASE+11*8)		// CAN总线ID单独采集设置12	8B
#define		EE_JT_CAN_ID_COLLECT_SET_12					(EE_JT_CAN_ID_COLLECT_SET_BASE+12*8)		// CAN总线ID单独采集设置13	8B
#define		EE_JT_CAN_ID_COLLECT_SET_13					(EE_JT_CAN_ID_COLLECT_SET_BASE+13*8)		// CAN总线ID单独采集设置14	8B
#define		EE_JT_CAN_ID_COLLECT_SET_14					(EE_JT_CAN_ID_COLLECT_SET_BASE+14*8)		// CAN总线ID单独采集设置15	8B
#define		EE_JT_CAN_ID_COLLECT_SET_15					(EE_JT_CAN_ID_COLLECT_SET_BASE+15*8)		// CAN总线ID单独采集设置16	8B

// 从调度屏获取
#define		EE_JT_PRODUCTOR_CODE						(EE_OBL_BASE_ADDR+0xE05)	// 厂商编码	 4B
#define		EE_JT_CAR_OWNER_PHONE_NUM					(EE_OBL_BASE_ADDR+0xE09)    // 车主号码	 12B
#define		EE_JT_CAR_OWNER_NAME						(EE_OBL_BASE_ADDR+0xE15) 	// 车主姓名  20B
#define		EE_TJP_VESION								(EE_OBL_BASE_ADDR+0xE29)    // 调度屏版本 2B


	


/////////////  电子围栏数据() //////////////////////////
#define			LEN_ELE_RAIL_PARA						32
#define			ELE_TOTAL_NUM							4
#define			EE_ELE_SET_PARAMETER					(EE_OBL_BASE_ADDR+0xEF0)		//
  	
#define			EE_ELE_BASE_ADDR						(EE_OBL_BASE_ADDR+0xF00)	//	 64B参数
#define			EE_ELE_RAIL1							(EE_ELE_BASE_ADDR+0*LEN_ELE_RAIL_PARA)		//  16B,围栏1
#define			EE_ELE_RAIL2							(EE_ELE_BASE_ADDR+1*LEN_ELE_RAIL_PARA)		//  16B,围栏2
#define			EE_ELE_RAIL3							(EE_ELE_BASE_ADDR+2*LEN_ELE_RAIL_PARA)		//  16B
#define			EE_ELE_RAIL4							(EE_ELE_BASE_ADDR+3*LEN_ELE_RAIL_PARA)		//  16B


#define			EE_RSV_1FF0								0x1fe0			// 保留，FIRST_RUN使用

																													
#pragma		pack(1)
typedef		struct	_RW_EEP
{
	uchar		Type;				// 存储器类型 S:SPI,I:I2C
	uchar		PageLen;			// 保留
	uchar		RW;				// 读写标志，'R'读，'W'写
	uchar		DS;				// 选择器件
	uint16	Addr;				// 存储地址
	uint16	Length;			// 读写长度	
	uchar		Buffer[256];	// 写数据
} *pRW_EEP;


#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */

