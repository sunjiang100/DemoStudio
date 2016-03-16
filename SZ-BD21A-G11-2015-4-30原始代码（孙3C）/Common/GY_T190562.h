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

#ifndef      __GY_T19056_H__
#define      __GY_T19056_H__


#ifdef      GY19056_GLOBALS
#define     GY19056_EXT
#else
#define     GY19056_EXT    extern
#endif


// $$$$$$$$$$$$$$$$$$ <<< Use Configuration Wizard in Context Menu >>> $$$$$$$$$$$$$$$$$
// <h>国标文件相关配置




// </h>

// $$$$$$$$$$$$$$$$$$$ <<< end of configuration section >>> $$$$$$$$$$$$$$$$$$$$$$$$$$$$



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	  			命令字定义
*/


GY19056_EXT    enum
{
	/////////	上载命令字	//////////////////////////////////////////////////////////
	
	eGY_rCMD_SELECT_SIM_CARD				= 0X01,	// 
	
	eGY_rCMD_GET_REAL_TIME					= 0x02,	// 采集实时时钟
	
	eGY_rCMD_GET_ALL_REAL_TIME				= 0x20,	// 读取CPU和PCF8563时实时钟	
	// 测试存储器命令字 ///////////////////////////////////////////////////////
	eGY_rCMD_GET_SET_EE_DATA				= 0x28,	// 读写指定铁电地址和长度的数据
	eGY_rCMD_GET_SET_DF_DATA				= 0x29,	// 读写EEPROM数据
	eGY_rCMD_GET_SET_NF_DATA				= 0x2a,	// 读写数据到指定NAND Flash页
	eGY_rCMD_GET_SET_CPU_DATA				= 0x2b,	// 读写CPU内部Flash数据，IAP	
	eGY_rCMD_GET_SENSOR_KV					= 0x30,	// 采集传感器每公里脉冲数、
	eGY_rCMD_GET_USER_VEHICLE_NUM			= 0x31,	// 用户车辆编号，4字节10进制字符
	eGY_rCMD_GET_TIME_SPEED					= 0x32,	// 采集时间相关值，速度上限、加减速度上限
	eGY_rCMD_GET_SOLE_NUMBER				= 0x33,	// 产品唯一序列号
	eGY_rCMD_GET_OVER_SPEED					= 0x34,	// 超速数据
	eGY_rCMD_GET_OP_RECORD					= 0x35,	// 各种命令操作记录
	eGY_rCMD_GET_USER_FLAG					= 0x36,	// 用户标识
	eGY_rCMD_GET_STOP_DATA_DRIVER			= 0x37,	// 采集疑点数据对应的驾驶证号
	eGY_rCMD_GET_ID_CARD_REC				= 0x38,	// 采集驾驶员刷卡记录(100次)
	eGY_rCMD_GET_SYE_SPECIAL_DATA			= 0x39,	// 采集指定数据
	eGY_rCMD_GET_INSTALL_DATE				= 0x3a,	// 采集初次安装日期
	eGY_rCMD_GET_KV_OP_REC					= 0x3b,	// 采集车辆K值下载记录(100次记录)
	eGY_rCMD_GET_GPS_TIME_DIFFERENCE		= 0x3c,	// 采集GPS时间与本地时间的时差
	eGY_rCMD_GET_ALL_DATA					= 0x3f,	// 采集记录仪所有数据

	//////////// 一些临时标志    ///////////////////////////////////////////////////////
	eGY_rCMD_SET_8_FLAGS						= 0xae,	// 设置8个标志位
	////////////  下载附加命令（非国标） ////////////////////////////////////////////////
	eGY_rCMD_SET_SENSOR_KV					= 0xa0,	// 设置传感器每公里脉冲数、
	eGY_rCMD_SET_USER_VEHICLE_NUM			= 0xa1,	// 设置用户车辆编号，4字节10进制字符
	eGY_rCMD_SET_TIME_SPEED					= 0xa2,	// 设置时间相关值，速度上限、加减速度上限
	eGY_rCMD_SET_SOLE_NUMBER				= 0xa3,	// 设置产品唯一序列号
	eGY_rCMD_SET_USER_FLAG					= 0xa6,	// 设置用户标识
	eGY_rCMD_SET_GPS_TIME_DIFFERENCE		= 0xac,	// 设置GPS时间与本地时间的时差
	eGY_rCMD_SET_INIT_SYS					= 0xaf,	// 初始化终端存储器。慎用！！
	
	
	////////////   调试命令 ////////////////////////////////////////////////////////////	
	eGY_rCMD_SET_U0_U1_CLARITY				= 0xaf,	// 设置第2组测试标志单元，U0和U1透明传输功能
	eGY_rCMD_SET_SEND_TO_UART				= 0xce,	// 把数据发送到串口1

	////////////  上载GSM/GPRS相关信息命令字 ////////////////////////////////////////////////
	eGY_rCMD_GET_SMS_CENTER_NUMBER		= 0x40,	// 读取短信息服务中心号码
	eGY_rCMD_GET_GPRS_SERVER_IP			= 0x41,	// 读取GPRS服务器IP和端口号
	eGY_rCMD_GET_OEM_ID						= 0x42,	// 读取生产商标志和终端ID
	eGY_rCMD_GET_CENTER_NUMBER				= 0x43,	// 设置3个监控中心号码
	eGY_rCMD_GET_PIN_CODE					= 0x44,	// 
	eGY_rCMD_GET_OEM_WEB						= 0x45,	// 读取生产商标识
	eGY_rCMD_GET_GPS_PACK_TIME				= 0x46,	// 读取GPS数据打包相关参数
	eGY_rCMD_GET_GPS_MILEAGE_ELE_RAIL	= 0x47,	// 
	eGY_rCMD_GET_SIM_NUMBER					= 0x48,	// 读取SIM卡号
	eGY_rCMD_GET_OUTPUT_STATUS				= 0x49,	// 输出状态
	eGY_rCMD_GET_GY_FLAG						= 0x4b,	// 	

	////////////  下载GSM/GPRS相关信息命令字 ////////////////////////////////////////////////
	eGY_rCMD_SET_SMS_CENTER_NUMBER		= 0xb0,	// 设置短信服务息中心号码
	eGY_rCMD_SET_GPRS_SERVER_IP			= 0xb1,	// 设置GPRS服务器IP和端口号
	eGY_rCMD_SET_OEM_ID						= 0xb2,	// 生产商和终端ID
	eGY_rCMD_SET_CENTER_NUMBER				= 0xb3,	// 设置3个监控中心号
	eGY_rCMD_SET_PIN_CODE					= 0xb4,	// 
	eGY_rCMD_SET_OEM_WEB						= 0xb5,	// 设置生产商标识
	eGY_rCMD_SET_GPS_PACK_TIME				= 0xb6,	// 设置GPS数据打包相关参数
	eGY_rCMD_SET_CLR_GPS_MILEAGE			= 0xb7,	//
	eGY_rCMD_SET_SIM_NUMBER					= 0xb8,	// 设置SIM卡号
	eGY_rCMD_SET_OUTPUT_STATUS				= 0xb9,	// 输出状态	
	eGY_rCMD_SET_IDLE_PARA					= 0xba,	// 设置低功耗参数
	eGY_rCMD_SET_MEMORY_FORMAT				= 0xfa,	// 
	
	eGY_rCMD_SET_REAL_TIME					= 0xc2,	// 设置实时时间
	
	////////////	错误代码	//////////////////////////////////////////////////////////
	eGY_rCMD_UP_LOAD_ERROR					= 0xfa,	// 上传出错标志字
	eGY_rCMD_DOWN_LOAD_ERROR				= 0xfb,	// 下传出错标志字
	
	eGY_rCMD_GET_SYSTEM_LOG					= 0xfe,	// 读系统日志数据
	
	eGY_rCMD_SYSTEM_RESTART					= 0xff,	// 重启动
	
} enGY_RS232_CMD;

///////////// 与GSM/GPRS相关的每条信息的长度字节 ////////////////////////////////////////

#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */




