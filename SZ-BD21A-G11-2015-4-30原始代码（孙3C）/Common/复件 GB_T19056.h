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

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	  			命令字定义
*/

/////////	上载命令字	//////////////////////////////////////////////////////////
#define		GET_DRIVER_CODE				0x01	// *驾驶员代码及其对应的机动车驾驶证号
#define		GET_REAL_TIME					0x02	// *
#define		GET_MILAGE_360H				0x03	// *最近360h内的累计行驶里程数据
#define		GET_VEHICLE_CC					0x04	// *Vehicle Characteristic Coefficients
#define		GET_SPEED_360H					0x05	// *最近360h内每分钟的行驶速度数据（每分钟一个）
#define		GET_VEHICLE_VIN				0x06	// *车辆VIN号，车牌号码，车辆分类
#define		GET_STOP_DATA					0x07	// *事故疑点数据（停车前20秒数据）
#define		GET_MILAGE_2DAY				0x08	// 最近2个日历天内的累计行驶里程
#define		GET_SPEED_2DAY					0x09	// 最近2个日历天内的行驶速度(每分钟一个)
#define		GET_OVER_TIME_3H				0x11	// 疲劳驾驶数据
///////// 	其中：国标规定至少有 0x01~0x07共7个上载命令，其它可在显示器显示

/////////	上载附加命令（非国标） ///////////////////////////////////////////////

#define		GET_ALL_REAL_TIME				0x20	// 读取CPU和PCF8563时实时钟

//////////// 测试存储器命令字 ///////////////////////////////////////////////////////
#define		GET_SET_FM_DATA				0x28	// 读写指定铁电地址和长度的数据
#define		GET_SET_EE_DATA				0x29	// 读写EEPROM数据
#define		GET_SET_NF_DATA				0x2a	// 读写数据到指定NAND Flash页
#define		GET_SET_CPU_DATA				0x2b	// 读写CPU内部Flash数据，IAP	

#define		GET_SENSOR_KV					0x30	// 采集传感器每公里脉冲数、
#define		GET_USER_VEHICLE_NUM			0x31	// 用户车辆编号，4字节10进制字符
#define		GET_TIME_SPEED					0x32	// 采集时间相关值，速度上限、加减速度上限
#define		GET_SOLE_NUMBER				0x33	// 产品唯一序列号
#define		GET_OVER_SPEED					0x34	// 超速数据
#define		GET_OP_RECORD					0x35	// 各种命令操作记录
#define		GET_USER_FLAG					0x36	// 用户标识
#define		GET_STOP_DATA_DRIVER			0x37	// 采集疑点数据对应的驾驶证号
#define		GET_ID_CARD_REC				0x38	// 采集驾驶员刷卡记录(100次)
#define		GET_SYE_SPECIAL_DATA			0x39	// 采集指定数据
#define		GET_INSTALL_DATE				0x3a	// 采集初次安装日期
#define		GET_KV_OP_REC					0x3b	// 采集车辆K值下载记录(100次记录)
#define		GET_GPS_TIME_DIFFERENCE		0x3c	// 采集GPS时间与本地时间的时差
#define		GET_ALL_DATA					0x3f	// 采集记录仪所有数据








//////////// 一些临时标志    ///////////////////////////////////////////////////////
#define		SET_8_FLAGS						0xae	// 设置8个标志位




////////////	下载命令字	//////////////////////////////////////////////////////////
#define		SET_DRIVER_CODE				0x81	// 设置驾驶员代码，驾驶证号码
#define		SET_VEHICLE_VIN				0x82	// 设置车辆VIN号，车牌号，车辆分类
#define		SET_REAL_TIME					0xc2	// 设置实时时间
#define		SET_VEHICLE_CC					0xc3	// 设置车辆特征系数

////////////  下载附加命令（非国标） ////////////////////////////////////////////////
#define		SET_SENSOR_KV					0xa0	// 设置传感器每公里脉冲数、
#define		SET_USER_VEHICLE_NUM			0xa1	// 设置用户车辆编号，4字节10进制字符
#define		SET_TIME_SPEED					0xa2	// 设置时间相关值，速度上限、加减速度上限
#define		SET_SOLE_NUMBER				0xa3	// 设置产品唯一序列号
#define		SET_USER_FLAG					0xa6	// 设置用户标识
#define		SET_GPS_TIME_DIFFERENCE		0xac	// 设置GPS时间与本地时间的时差
//#define		SET_INIT_SYS					0xaf	// 初始化终端存储器。慎用！！





#define		DISP_ONE_SCR					0xdb	// 显示一个屏幕内容

////////////   调试命令 ////////////////////////////////////////////////////////////
#define		SET_KEY_CODE					0xa5	// 模拟键盘操作

		
#define		SET_U0_U1_CLARITY				0xaf	// 设置第2组测试标志单元，U0和U1透明传输功能

#define		SET_SEND_TO_UART				0xce	// 把数据发送到串口1





////////////  上载GSM/GPRS相关信息命令字 ////////////////////////////////////////////////
#define		GET_SMS_CENTER_NUMBER		0x40	// 读取短信息服务中心号码
#define		GET_GPRS_SERVER_IP			0x41	// 读取GPRS服务器IP和端口号
#define		GET_OEM_ID						0x42	// 读取生产商标志和终端ID
#define		GET_CENTER_NUMBER				0x43	// 设置3个监控中心号码
#define		GET_PIN_CODE					0x44	// 
#define		GET_OEM_WEB						0x45	// 读取生产商标识
#define		GET_GPS_PACK_TIME				0x46	// 读取GPS数据打包相关参数

#define		GET_GPS_MILEAGE_ELE_RAIL	0x47	// 
#define		GET_GY_FLAG						0x48	// 读取我的联系方式
#define		GET_OUTPUT_STATUS				0x49	// 输出状态	


////////////  下载GSM/GPRS相关信息命令字 ////////////////////////////////////////////////
#define		SET_SMS_CENTER_NUMBER		0xb0	// 设置短信服务息中心号码
#define		SET_GPRS_SERVER_IP			0xb1	// 设置GPRS服务器IP和端口号
#define		SET_OEM_ID						0xb2	// 生产商和终端ID
#define		SET_CENTER_NUMBER				0xb3	// 设置3个监控中心号
#define		SET_PIN_CODE					0xb4	// 

#define		SET_OEM_WEB						0xb5	// 设置生产商标识
#define		SET_GPS_PACK_TIME				0xb6	// 设置GPS数据打包相关参数
#define		SET_CLR_GPS_MILEAGE			0xb7	//

#define		SET_OUTPUT_STATUS				0xb9	// 输出状态	

#define		SET_IDLE_PARA					0xba	// 设置低功耗参数

#define		SET_MEMORY_FORMAT				0xfa	// 



///////////// 与GSM/GPRS相关的每条信息的长度字节 ////////////////////////////////////////
#define		LEN_SMS_CENTER_NUMBER		16			// 短消息中心号码存储空间所占长度
#define		LEN_GPRS_SERVER_IP			32			// GPRS服务器IP和端口号存储空间所占的长度





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
