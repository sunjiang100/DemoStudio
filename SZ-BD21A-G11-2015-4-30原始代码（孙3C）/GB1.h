/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												肖遥电子（广州）研发中心
$										Shawyoo Electronics (GuangZhou) R&D Center
$                       	http://www.shawyoo.com		 Email: autoele@qq.com
$								 QQ群：9069471/9069433/9524253		All rights reserved.	  
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

#ifndef      __GB_H__
#define      __GB_H__

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
#define		GET_SPECIAL_NF_DATA			0x3c	// 采集指定NAND Flash地址和长度的数据
#define		GET_SPECIAL_FM_DATA			0x3e	// 采集指定铁电地址和长度的数据
#define		GET_ALL_DATA					0x3f	// 采集所有数据



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
#define		SET_SPECIAL_FM_DATA			0xae	// 指定地址和长度，写入数据
#define		SET_INIT_SYS					0xaf	// 初始化终端存储器。慎用！！

////////////   调试命令 ////////////////////////////////////////////////////////////
#define		SET_KEY_CODE					0xb0	// 模拟键盘操作

		


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	  			各命令操作记录在铁电中的存储地址
				OPR -- OPeration Recorder
*/
/////////	上载命令字	//////////////////////////////////////////////////////////
#define		OPR_GET_DRIVER_CODE			0x100	// *驾驶员代码及其对应的机动车驾驶证号
#define		OPR_GET_REAL_TIME				0x108	// *
#define		OPR_GET_MILAGE_360H			0x110	// *最近360h内的累计行驶里程数据
#define		OPR_GET_VEHICLE_CC			0x118	// *Vehicle Characteristic Coefficients
#define		OPR_GET_SPEED_360H			0x120	// *最近360h内的行驶速度数据（每分钟一个）
#define		OPR_GET_VEHICLE_VIN			0x128	// *车辆VIN号，车牌号码，车辆分类
#define		OPR_GET_STOP_DATA				0x130	// *事故疑点数据（停车前数据）
#define		OPR_GET_MILAGE_2DAY			0x138	// 最近2个日历天内的累计行驶里程
#define		OPR_GET_SPEED_2DAY			0x140	// 最近2个日历天内的行驶速度(每分钟一个)
#define		OPR_GET_OVER_TIME_3H			0x148	// 疲劳驾驶数据
///////// 	其中：国标规定至少有 0x01~0x07共7个上载命令，其它可在显示器显示
/////////	上载附加命令（非国标） ///////////////////////////////////////////////
#define		OPR_GET_SENSOR_SPEED			0x150	// 采集传感器每公里脉冲数、速度上限、加减速度上限
#define		OPR_GET_USER_VEHICLE_NUM	0x158	// 用户车辆编号，4字节10进制字符
#define		OPR_GET_INTERVAL				0x160	// 采集记录时间间隔或报警时间设置值
#define		OPR_GET_SOLE_NUMBER			0x168	// 产品唯一序列号
#define		OPR_GET_OVER_SPEED			0x170	// 超速数据
#define		OPR_GET_OP_RECORD				0x178	// 各种命令操作记录
#define		OPR_GET_USER_FLAG				0x180	// 用户标识
#define		OPR_GET_ALL_DATA				0x188	// 采集所有数据
////////////	下载命令字	//////////////////////////////////////////////////////////
#define		OPR_SET_DRIVER_CODE			0x190	// 设置驾驶员代码，驾驶证号码
#define		OPR_SET_VEHICLE_VIN			0x198	// 设置车辆VIN号，车牌号，车辆分类
#define		OPR_SET_REAL_TIME				0x1a0	// 设置实时时间
#define		OPR_SET_VEHICLE_CC			0x1a8	// 设置车辆特征系数
////////////  下载附加命令（非国标） ////////////////////////////////////////////////
#define		OPR_SET_SENSOR_SPEED			0x1b0	// 设置传感器每公里脉冲数、速度上限、加减速度上限
#define		OPR_SET_USER_VEHICLE_NUM	0x1b8	// 设置用户车辆编号，4字节10进制字符
#define		OPR_SET_INTERVAL				0x1c0	// 设置记录时间间隔或报警时间设置值
#define		OPR_SET_SOLE_NUMBER			0x1c8	// 设置产品唯一序列号
#define		OPR_SET_INIT_SYS				0x1d0	// 初始化终端存储系统。慎用！！
#define		OPR_SET_USER_FLAG				0x1d8	// 设置用户标识

#define		OPR_SET_KEY_CODE				0x1e0	// 模拟键盘操作


////////////	保留字	////////////////////////////////////////////////////////////
#define		RESERVER_WORD				0x00


#define		TIMES_STOP_DATA			10		// 记录停车前数据的次数


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	  			参数在铁电存储器中的起始地址(低地址在前)
*/
////////////	参数在铁电存储器中的起始地址  ///////////////////////////////
#define		FM_VEHICLE_VIN				0x03		// 车辆VIN号
#define		FM_VEHICLE_CODE			0x14		// 车牌号码
#define		FM_VEHICLE_CLASS			0x20		// 车辆分类
#define		FM_USER_VEHICLE_NUM		0x2c		// 用户车辆编号
#define		FM_FILE_TYPE1				0x30		// 文件类型存储起始地址1
#define		FM_FILE_TYPE2				0x38		// 文件类型存储地址2
#define		FM_VEHICLE_CC				0x40		// Vehicle Characteristic Coefficients
#define		FM_SENSOR_KV				0x46		// 传感器每公里脉冲数(K值)

#define		FM_DRIVER_CODE				0x50		// 驾驶员代码及其对应的机动车驾驶证号
#define		FM_RUN_TIME_START			0x65		// 无疲劳驾驶行驶的开始时刻
#define		FM_TIME_SPEED				0x70		// 记录时间间隔或报警时间设置值
#define		FM_SPEED_LIMIT				0x74		// 限速值
#define		FM_ACC_LIMIT				0x75		// 限加速度

#define		FM_INSTALL_DATE			0x78		// 初次安装日期(年月日时分秒)
#define		FM_LAST_SECOND				0x7e		// 记录仪运行状态的最后一秒钟(年月日时分秒)
#define		FM_SYSTEM_START_CNT		0x88		// 系统启动总次数及最后一次启动时间
//#define		FM_SS_LAST_ONE				0x8a		// 系统最后一次启动日期时间
#define		FM_USER_FLAG   			0xb0		// 用户标识存储起始地址
#define		FM_SOLE_NUMBER				0xd0		// 产品唯一序列号
#define		FM_OEM_FLAG					0xe0		// 生产商标志

#define		FM_OP_RECORD				0x100		// 各命令操作记录
#define		FM_MILAGE_360H_BCD		0x200		// 最近360h内的累计行驶里程数据(BCD和时间)
#define		FM_MILAGE_360H_HEX		0x208		// 刚好第360h的累计行驶里程数据（3字节Hex）
#define		FM_TOTAL_SPEED_PULS		0x20c		// 从安装起，速度传感器产生的总脉冲数

#define		FM_MILAGE_2DAY				0x210		// 最近2个日历天内的累计行驶里程
#define		FM_PULS_YESTODAY			0x218		// 昨天的速度传感器脉冲数
#define		FM_PULS_TODAY				0x21c		// 当天的速度传感器脉冲数

#define		FM_RUN_STATUS_CNT			0x220		// 行驶时间(分钟)计数器(2字节)
#define		FM_STOP_TIME_CNT			0x222		// 停车时间(分钟)计数器(2字节)
#define		FM_TIRE_DRIVE_FLAG		0x224		// 正在疲劳驾驶标志（疲劳为非0）
#define		FM_OVER_TIME_CNT			0x226		// 超时总次数计数器
#define		FM_OVER_TIME_REC			0x228		// 超时记录数据起始地址

#define		FM_OVER_SPEED_CNT			0x4ae		// 超速次数计数器
#define		FM_OVER_SPEED_REC			0x4b0		// 超速记录


#define		FM_STOP_TIMS_CNT			0x730		// 现在第几次停车，2字节
#define		FM_20S_DP					0x732		// 20秒数据的当前存储地址指针
#define		FM_20S_CNT					0x734		// 现在是20秒中的第几秒
#define		FM_LAST_5S_PULS			0x736		// 最后5秒种每秒的脉冲数
#define		FM_LAST_23S_DATA			0x740		// 最近23秒数据存储首址（不管是否停车）
#define		FM_STOP_DATA				0x82e		// 事故疑点数据（停车前数据）
#define		FM_20S_DATA_START_ADDR	0x82e		// 20S数据开始存储地址
#define		FM_20S_DATA_DRIVER		0x103a	// 20S数据对应的驾驶证号码
												
#define		FM_KV_OP_REC_CNT			0x10fe	// 车辆特征系数下载次数计数器
#define		FM_KV_OP_REC				0x1100	// 车辆特征系数下载记录(100条，1200字节)

#define		FM_ID_CARD_REC_CNT		0x142e	// 更换驾驶员计数器
#define		FM_ID_CARD_REC				0x1430	// 驾驶员值班记录(驾驶证号+刷卡时间)

// 最近5个驾驶员驾驶证号，刷卡时间，行驶时间和停车时间
#define		FM_LAST_5_DRIVERS			0x2280	// 	

#define		FM_LAST_15MIN_DRIVER		0x23a0	// 最后15分钟对应的驾驶员代码
#define		FM_STOP_15MIN_DRIVER		0x23cd	// 停车前15分钟对应的驾驶员代码
#define		FM_LAST_15MIN_DRIVER_CNT 0x23fa	// 最后15分钟计数器

#define		FM_15MIN_AVG_SPEED		0x2400	// 停车前15分钟内每分钟的平均车速
#define		FM_LAST_STOP_RTC			0x240f	// 最后一次停车时刻的日期
#define		FM_STOP_DRIVER_CODE		0x2415	// 停车时刻驾驶员代码(无驾驶证号)
#define		FM_1H_AVG_START			0x2418	// 最近1小时的起始日期和时间年月日时分
#define		FM_1H_AVG_SPEED_MIN		0x241d	// 最近1小时内每分钟的平均速度
#define		FM_360H_CNT					0x2460	// 现在是360小时的第几个小时
#define		FM_360H_MIN_CNT			0x2464	// 360小时内的分钟计数器(4字节)
#define		FM_LAST_15MIN_AVG_SPEED	0x2468	// 最近15分钟每分钟的平均速度

#define		FM_SPEED_360H				0x2488	// 最近360小时内每分钟的平均速度
															// ~ 0x7ff0 23400字节为360小时速度块数据





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	  			部分参数的长度字节数
*/
#define		LEN_DRIVER_CODE			21		// 驾驶员代码及其对应的机动车驾驶证号
#define		LEN_MILAGE_360H			3		// 最近360h内的累计行驶里程数据
#define		LEN_VEHICLE_CC				3		// Vehicle Characteristic Coefficients
#define		LEN_VEHICLE_VCC			41		// 车辆VIN号，车牌号码，车辆分类
#define		LEN_VEHICLE_VIN			17		// 车辆VIN号
#define		LEN_VEHICLE_CODE			12		// 车牌号码
#define		LEN_VEHICLE_CLASS			12		// 车辆分类
#define		LEN_RTC						6		// RTC

#define		LEN_SENSOR_KV				2		// 采集传感器每公里脉冲数
#define		LEN_USER_VEHICLE_NUM		4		// 用户车辆编号，4字节10进制字符
#define		LEN_ABOUT_TIME				4		// 时间相关值
#define		LEN_TIME_SPEED				6		// 时间相关值、速度上限、加减速度上限
#define		LEN_SOLE_NUMBER			16		// 产品唯一序列号
#define		LEN_OVER_SPEED				645	// 超速数据
#define		LEN_OP_RECORD				256	// 各种命令操作记录
#define		LEN_USER_FLAG				32		// 用户标识
#define		LEN_ALL_DATA				0x7000// 采集所有数据

#define		LEN_OVER_TIME_1REC		28		// 疲劳驾驶记录每条信息长度字节
#define		LEN_OVER_SPEED_1REC		32		// 超速记录每条信息长度字节
#define		LEN_ID_CARD_1REC			24		// 驾驶员身份认证存储的长度(证号+插卡时间)
#define		LEN_RUN_STATUS_CNT		6		// 车辆行驶停止和疲劳驾驶标志位的长度

#define		LEN_LAST_5_DRIVERS_1REC	32		// 最近5个驾驶员（人次）驾驶信息每第的字节长度

#define		LEN_KV_OP_1REC				8		// 车辆特征系数下载操作记录一条的长度
#define		LEN_KV_OP_REC				800	// 车辆特征系数下载操作记录的存储长度

#define		LEN_360H_1REC				65		// 每小时速度块数据字节长度

#define		LEN_OVER_TIME				645	// 超时数据块长度

#define		LEN_INSTALL_DATE			6		// 初次安装日期和时间

#define		LEN_20S_DATA				206	// 一次20S数据长度
#define		LEN_STOP_DATA				(TIMES_STOP_DATA*LEN_20S_DATA)	// 事故疑点数据长度
#define		LEN_20S_DATA_DRIVER	   180	// 10次疑点数据对应的10个驾驶证号的总长度
#define		LEN_MILAGE_2DAY			3		// 最近2个日历天内的累计行驶里程
#define		LEN_360H_DATA				(65*360)	// 360小时内速度数据



#define		FM_CAPACITY					32768		// 铁电容量（字节）

////////////	引用外部变量	/////////////////////////////////////////////////////

 
 

#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */