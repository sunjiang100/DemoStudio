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


#define		OVER_TIME_MAX_CNT				8		// 最多记录超时记录的条数
#define		OVER_SPEED_MAX_CNT			20		// 最多记录超速记录的条数
#define		ID_CARD_REC_MAX_CNT			150	// 最多记录驾驶员身份认证次数
#define		KV_OP_REC_MAX_CNT				100	// 最多记录下载车辆特征系数记录


////////////	通讯出错标志	///////////////////////////////////////////////////////
#define		UP_LOAD_ERROR					0xfa	// 上传出错标志字
#define		DOWN_LOAD_ERROR				0xfb	// 下传出错标志字

#define		RESERVER_WORD					0x00	// 

#define		LEN_U0_GB_PACK_HEAD			6

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	  			命令字定义
*/
GB19056_EXT    enum
{
	//上载命令字	//////////////////////////////////////////////////////////
	eGB_rCMD_GET_STD_VERSION				= 0x00,	// 采集记录仪执行标准
	eGB_rCMD_GET_DRIVER_CODE				= 0x01,	// 驾驶员代码及其对应的机动车驾驶证号
	eGB_rCMD_GET_REAL_TIME					= 0x02,	// 采集实时时钟
	eGB_rCMD_GET_TOTAL_MILAGE				= 0x03,	// 采集累计行驶里程
	eGB_rCMD_GET_SPEED_KV					= 0x04,	// 采集记录仪脉冲系数				  
	eGB_rCMD_GET_VEHICLE_VIN				= 0x05,	// 采集车辆信息		*车辆VIN号，车牌号码，车辆分类	   
	eGB_rCMD_GET_SIGNAL_CONFIG				= 0x06,	// 采集状态信号配置信息	 
	eGB_rCMD_GET_SERIAL_NUMBER		      = 0x07, 	// 采集记录仪唯一序列号（国标使用，实际不使用）
	eGB_rCMD_GET_SPEED_PER_SECOND			= 0x08,	// 采集指定的行驶速度记录		每秒钟平均速度							
	eGB_rCMD_GET_POSITION_INFO				= 0x09,	// 采集指定的位置信息记录
	eGB_rCMD_GET_PARKING_DATA				= 0x10,	// 采集指定的事故疑点记录（停车前20秒数据）
	eGB_rCMD_GET_OVER_TIME_DRIVE			= 0x11,	// 采集指定的超时驾驶记录	疲劳驾驶数据	
	eGB_rCMD_GET_GET_LOG_IN_OUT_RECORD	= 0x12,	// 采集指定的驾驶人身份记录		符合条件的登录退出记录						 
	eGB_rCMD_GET_POWER_RECORD				= 0x13,	// 采集外部供电记录
	eGB_rCMD_GET_MODIFY_PARA_RECORD		= 0x14,	// 采集指定的参数修改记录
	eGB_rCMD_GET_SPEED_STATUS_LOG			= 0x15,	// 采集指定的速度状态日志


	eGB_rCMD_GET_SPEED_PER_MIN				= 0x55,		// 每分钟平均速度	  为了编译通过，先不注释它

	//其他命令字	//////////////////////////////////////////////////////////
	eGB_rCMD_GET_SetUp_PARA       	   = 0x20,	// 记录仪安装参数	     与eGB_rCMD_GET_SPEED_PULSE_COEFFICIENT   重复 是否标准有问题？
	eGB_rCMD_GET_Posicion_PARA     	   = 0x21,	// 定位告警相关参数参数
	eGB_rCMD_GET_ElePen_Data     	      = 0x22,	// 电子围栏数据
	eGB_rCMD_GET_ElePen_List     	      = 0x23,	// 电子围栏目录
   eGB_rCMD_GET_RoadPlan_Data     	   = 0x2A,	// 道路检测数据
	eGB_rCMD_GET_RoadPlan_List    	   = 0x2B,	// 道路检测目录
	

	//其中：国标规定至少有 0x01~0x15共7个上载命令，其它可在显示器显示
	
	////////////	下载命令字	//////////////////////////////////////////////////////////
	eGB_rCMD_SET_DRIVER_CODE				= 0x81,	// 设置驾驶员代码，驾驶证号码			  GB2012不用

	eGB_rCMD_SET_VEHICLE_VIN				= 0x82,	// 设置车辆VIN号，车牌号，车辆分类	   
	eGB_rCMD_SET_SETUP_DATE					= 0x83,	// 安装日期
	eGB_rCMD_SET_SIGNAL_STATUS				= 0x84,	// 设置状态量配置信息						
	eGB_rCMD_SET_REAL_TIME					= 0xc2,	// 设置实时时间								
	eGB_rCMD_SET_VEHICLE_PULSE_KV			= 0xc3,	// 设置车辆特征系数						
	eGB_rCMD_SET_INIT_MIL					= 0xc4,  // 设置初始里程
	
	eGB_rCMD_SET_SERIAL_NUMBER				= 0xc7,	// 设置唯一序列号（国标使用，实际不使用）
	
	////////////	检定控制命令	//////////////////////////////////////////
	eGB_rCMD_SET_CHECK						= 0xE0,	//检定控制
	eGB_rCMD_SET_CHECK_CONTINUSE        = 0xE2,	//检定模式保持
	
} enGB_RS232_CMD;



//GB19056_EXT    enum
//{
//	//USB数据块序号	//////////////////////////////////////////////////////////
//	eGB_GET_STD_VERSION				= 0,	// 采集记录仪执行标准
//	eGB_GET_DRIVER_CODE				= 1,	// 驾驶员代码及其对应的机动车驾驶证号
//	eGB_GET_REAL_TIME					= 2,	// 采集实时时钟
//	eGB_GET_TOTAL_MILAGE				= 3,	// *最近360h内的累计行驶里程数据
//	eGB_GET_SPEED_PULSE_COEFFICIENT= 4,	// *Vehicle Characteristic Coefficients	
//	eGB_GET_VEHICLE_VIN				= 5,	// *车辆VIN号，车牌号码，车辆分类
//	eGB_GET_SIGNAL_CONFIG			= 6,	// 采集状态信号配置信息
//	eGB_GET_SERIAL_NUMBER		   = 7,  // 产品唯一序列号
//	eGB_GET_SPEED_PER_SECOND		= 8,	// 每秒钟平均速度
//	eGB_GET_SPEED_PER_MIN			= 9,	// 每分钟平均速度
//	eGB_GET_POSITION_INFO			= 10,	// 采集指定的位置信息
//	eGB_GET_STOP_DATA					= 11,	// *事故疑点数据（停车前20秒数据）
//	eGB_GET_OVER_TIME_3H				= 12,	// 疲劳驾驶数据
//	eGB_GET_GET_LOG_IN_OUT_RECORD	= 13,	// 符合条件的登录退出记录
//	eGB_GET_POWER_RECORD				= 14,	// 采集外部供电记录
//	eGB_GET_MODIFY_PARA_RECORD	   = 15,	// 采集指定的参数修改记录
//	eGB_GET_SetUp_PARA       	   = 16,	// 记录仪安装参数
//	eGB_GET_Posicion_PARA     	   = 17,	// 定位告警相关参数参数
//	eGB_GET_ElePen_Data     	   = 18,	// 电子围栏数据
//	eGB_GET_ElePen_List     	   = 19,	// 电子围栏目录
//   eGB_GET_RoadPlan_Data     	   = 20,	// 道路检测数据
//	eGB_GET_RoadPlan_List    	   = 21,	// 道路检测目录
//	
//} enGB_USB_SierierNb;

GB19056_EXT    enum
{
	//USB数据块序号	//////////////////////////////////////////////////////////
	eGB_GET_STD_VERSION				= 0,	// 采集记录仪执行标准
	eGB_GET_DRIVER_CODE				= 1,	// 驾驶员代码及其对应的机动车驾驶证号
	eGB_GET_REAL_TIME					= 2,	// 采集实时时钟
	eGB_GET_TOTAL_MILAGE				= 3,	// *最近360h内的累计行驶里程数据
	eGB_GET_SPEED_PULSE_COEFFICIENT= 4,	// *Vehicle Characteristic Coefficients	
	eGB_GET_VEHICLE_VIN				= 5,	// *车辆VIN号，车牌号码，车辆分类
	eGB_GET_SIGNAL_CONFIG			= 6,	// 采集状态信号配置信息
	eGB_GET_SERIAL_NUMBER		   = 7,  // 产品唯一序列号
	eGB_GET_SPEED_PER_SECOND		= 8,	// 每秒钟平均速度	  行驶速度记录
	eGB_GET_POSITION_INFO			= 9,	// 采集指定的位置信息
	eGB_GET_STOP_DATA					= 10,	// *事故疑点数据（停车前20秒数据）
	eGB_GET_OVER_TIME_3H				= 11,	// 疲劳驾驶数据	  超时驾驶记录
	eGB_GET_GET_LOG_IN_OUT_RECORD	= 12,	// 符合条件的登录退出记录	  驾驶人身份记录
	eGB_GET_POWER_RECORD				= 13,	// 采集外部供电记录
	eGB_GET_MODIFY_PARA_RECORD	   = 14,	// 采集指定的参数修改记录
	eGB_GET_SPEED_STATUS_LOG		= 15,  // 速度状态日志
	
} enGB_USB_SierierNb;




////////////  下载附加命令（非国标） ////////////////////////////////////////////////

#define		SET_USER_VEHICLE_NUM			0xa1	// 设置用户车辆编号，4字节10进制字符
#define		SET_TIME_SPEED					0xa2	// 设置时间相关值，速度上限、加减速度上限
#define		SET_SERIAL_NUMBER				0xa3	// 设置产品唯一序列号
#define		SET_USER_FLAG					0xa6	// 设置用户标识
#define		SET_GPS_TIME_DIFFERENCE		0xac	// 设置GPS时间与本地时间的时差
//#define		SET_INIT_SYS					0xaf	// 初始化终端存储器。慎用！！

////////////  检定类型 ////////////////////////////////////////////////
#define     rCMD_CHECK_SPEED_MILAGE    0xE1  //速度里程检定模式下返回速度里程命令字
#define     CHECK_SPEED_MILAGE_MODE    0x01    	  //速度里程测量模式
#define     CHECK_VEHICLE_MODE         0x02    	  //速度脉冲测量模式
#define     CHECK_RTC_TIME_MODE        0x03    	  //实时时钟测量模式


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	  			参数在铁电存储器中的起始地址(低地址在前)
*/
////////////	参数在铁电存储器中的起始地址  ///////////////////////////////


#define		FM_INIT_MILAGE_BCD			0x20			// 初始里程		4B
#define		FM_LAST_SECOND					0x24		// 记录仪运行状态的最后一秒钟(年月日时分秒)	 6B



#define     FM_ICCardInOrOut_FLAG   	0x2B      //IC卡是否在插卡槽标志	1B
#define     FM_SpeeMode_FLAG        	0x2C      //速度类型标志1 GPS速度，0 脉冲速度  默认脉冲速度		1B
#define		FM_USE_GPS_MIL					0x2D		// 使用GPS累计里程
#define		FM_ENABLE_SYS_SLEEP			0x2E		// 允许系统休眠标识
#define		FM_ENABLE_VDR_UPLOAD			0x2F		// 允许系统休眠标识
////////// 轻度恢复出厂状态， 会清除 0x30 - 0x42 的内容


/////////

#define		FM_RESERVE_3					0x46			// 保留3	 6B		
																												
#define		FM_DRIVER_NUMBER				0x4C		  // 驾驶员代码  4B
#define		FM_DRIVER_CODE					0x50		  // 驾驶员机动车驾驶证号	  18B
//#define		FM_DRIVER_NAME					0x62		  // 驾驶员姓名				 20B

#define		FM_RUN_TIME_START		   	0x76		  // 当前驾驶员开始驾驶时间		6B

#define		FM_RESERVE_4					0x7C			// 保留4	4B	
                                    	
                                    	
#define		FM_RESERVE_5					0xAA		  // 保留5    	12B
                                    	
#define		FM_VEHICLE_PULSE_KV			0xB0		  // 传感器每公里脉冲数(K值)   4B	
                                    	
                                    	
#define		FM_RESERVE_6					0xBE		  // 保留6  2B

#define		FM_PLUS_POWROFF				0xC0		  // 		4B	

#define		FM_RESERVE_7					0xC4		  // 保留7		60B  

#define		FM_SERIAL_NUMBER				0x100		  //记录仪唯一编号		36B

#define		FM_RESERVE_8					0x124			// 保留8	12B	

#define		FM_GPS_Standard				0x130		  //执行的标准版本号和修改单号	   2B


#define		FM_RESERVE_8_1					0x132			// 保留8_1	14B

//////////	轻度恢复出厂状态会 清除 下面 0x140 - 0x1ff的内容
#define     FM_FACTORY_SET_BASE     	0x140      //恢复出厂设置起始地址 
#define		FM_LAST_5S_PULS				0x140		  // 最后5秒种每秒的脉冲数		20B

#define		FM_15MIN_SPEED		      	0x160		  // 15分钟每分钟平均速度 15B

#define		FM_STOP_TIME      			0x16F		  // 停车时刻 6B
#define		FM_STOP_LAST15_SPEED			0x175		  // 停车前15分钟每分钟平均速度 15B

#define     FM_TWO_DAY_MILAGE_HEX		0x185  	  //2个日历天总里程HEX码		 4byte
#define     FM_TWO_DAY_MILAGE_BCD		0x189  	  //2个日历天总里程BCD码		 4byte

#define     FM_TWO_DAY_1_MILAGE_HEX		0x18D  	  //2个日历天少当前小时的里程	          4byte
#define     FM_48_HOUR_CNT					0x191 	  //两个日历天小时计数器

#define     FM_360H_1_MILAGE_HEX	   	0x192  	  //360天少当前小时的里程	          4byte

#define		FM_INSTALL_DATE				0x1E5		  // 初次安装日期(年月日时分秒)				6byte
#define		FM_TOTAL_SPEED_PULS			0x1E0		  //从安装起，速度传感器产生的总脉冲数		5byte
#define     FM_MILAGE_BCD					0x1EB  	  //从安装记录仪起到现在, 车辆行驶的总里程BCD码		 4byte
#define     FM_MILAGE_HEX					0x1EF  	  //从安装记录仪起到现在, 车辆行驶的总里程HEX码		 4byte

#define     FM_360_HOUR_CNT        		0x1F3  	  //360小时计时器		 4byte
#define     FM_360H_MILAGE_HEX			0x1F7  	  //360小时总里程HEX码		 4byte
#define     FM_360H_MILAGE_BCD			0x1FB  	  //360小时总里程BCD码		 4byte
//////////////////


#define		FM_DRIVER_OUT_IN           0x200 			// 250B // 记录10条驾驶人登录退出记录

#define		FM_DRIVER_OUT_IN_CNT				0x2FA				//  驾驶人登录退出记录条数		4B

#define		FM_RESERVE_9					0x2FE		  // 保留9， 4B


// 2013-06-21 9:23 在检测中心修改，因为有后备电池，停车数据不存储在铁电，这里修改为Flash页存储计数器
#define		FM_FLASH_PAGE_CNT				0x300			// 各参数存储的Flash页计数器，预留256B
#define		FM_FLASH_PAGE_CNT_INIT_FLAG	0x3F0		// 

#define		FM_OVER_SPEED_PARA			0x4E4			// 
#define		FM_ENABLE_OVER_SPEED			0x4E4			//	1B
#define		FM_HIGH_SPEED_LIMIT			0x4E5			// 1B
#define		FM_LOW_SPEED_LIMIT			0x4E6			// 1B
#define		FM_PRE_ALARM_OVP				0x4E7			// 1B
#define		FM_OVP_ENABLE_TIME			0x4E8			// 2B
#define		FM_OVP_SECOND_CNT				0x4EA			// 2B

#define		FM_DRIVING_PARA				0x4EC			// 
#define		FM_DRIVEING_TIME				0x4EC		  	//疲劳驾驶阀值   4B
#define		FM_REST_TIME   	      	0x4F0		  	//最小休息时间	 	4B
#define		FM_DRIVE_ONE_DAY_LIMINT		0x4F4		  	//一天最长驾驶时间 4B
#define		FM_PARKING_TIME_LIMINT		0x4F8		  	//停车超时时间	 4B	
#define		FM_OVT_PRE_ALARM_TIME		0x4FC			// 预警时间,4B

#define		FM_FATIGUE_DRIVING_CNT		0x500			// 4B
#define		FM_SAVE_FATIGUE_DRIVING		0x504			// 250B(5*50B)



#define		FM_RESERVE_14					0x600		// 保留14，512B

#define	   	FM_EXT_POWER_STATUS		  0x800       // 上电掉电记录	  252B	 循环存36条记录，满后放到flash
#define		   FM_EXT_POWER_STATUS_CNT    0x8FC       //    上电记   4B


#define	   FM_PARA_MODIFY_RECORD		  0x900       // 参数修改记录	  252B	 循环存36条记录，满后放到flash
#define		 FM_PARA_MODIFY_RECORD_CNT    0x9FC       //  参数修改记录计数   4B



#define		FM_SIGNAL_CONFIG				0xA00			// 状态量配置信息 80B


#define		FM_SPEED_STAUTS_JUDGE	  0xA50		// 速度状态判定	8B


#define		FM_RESERVE_17					0xA58		// 保留17， 424B	


#define		FM_CURRENT_DRIVER_INFO_TIME		0xC00		// 当前驾驶员信息及驾驶时间 56B

//////////// 保存最新两2组驾驶员信息  共112字节
#define		FM_RECENT_DRIVER_INFO_TIME		0xC38		// 每组56字节

#define		FM_DRIVER_SERIAL_NUMBER				0xCA8		// 4B 自编号驾驶员代码

#define		FM_RESERVE_17_1			0xCAC		// 保留， 54B


#define		FM_360H_LOCATION_TIME 	0xD00			// 6B  360小时位置的单个小时的开始时间

#define		FM_360H_LOCATION_INFO	0xD06				// 660B   360小时位置的单个小时的数据	 存满后，存到flash

#define		 FM_RESERVE_18				0xF9A				//  保留， 358B



#define		FM_END_ADDR_GB_T19056					0x1100	// 存储GB19056相关的铁电结束地址，该地址不能使用，已经分配要JT808


#define		DISP_ONE_SCR					0xdb	// 显示一个屏幕内容

////////////   调试命令 ////////////////////////////////////////////////////////////
#define		SET_KEY_CODE					0xa5	// 模拟键盘操作

#define		SET_U0_U1_CLARITY				0xaf	// 设置第2组测试标志单元，U0和U1透明传输功能

#define		SET_SEND_TO_UART				0xce	// 把数据发送到串口1





///////////// 与GSM/GPRS相关的每条信息的长度字节 ////////////////////////////////////////
#define		LEN_SMS_CENTER_NUMBER		16			// 短消息中心号码存储空间所占长度
#define		LEN_GPRS_SERVER_IP			32			// GPRS服务器IP和端口号存储空间所占的长度





/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	  			部分参数的长度字节数
*/

#define		LEN_GPS_Standard			2    //执行的标准版本号和修改单号
#define		LEN_DRIVER_CODE			18		// 机动车驾驶证号
#define		LEN_MILAGE		    		20	// 累计行驶里程数据
#define		LEN_VEHICLE_KV				8		// Vehicle Characteristic Coefficients
#define		LEN_VEHICLE_VCC			41		// 车辆VIN号，车牌号码，车辆分类
#define		LEN_VEHICLE_VIN			17		// 车辆VIN号
#define		LEN_VEHICLE_CODE			12		// 车牌号码
#define		LEN_VEHICLE_CLASS			12		// 车辆分类
#define		LEN_RTC						6		// RTC


#define		LEN_USER_VEHICLE_NUM		4		// 用户车辆编号，4字节10进制字符
#define		LEN_ABOUT_TIME				4		// 时间相关值
#define		LEN_TIME_SPEED				6		// 时间相关值、速度上限、加减速度上限
#define		LEN_SERIAL_NUMBER			35		// 产品唯一序列号
#define		LEN_OVER_SPEED				645	// 超速数据
#define		LEN_OP_RECORD				256	// 各种命令操作记录
#define		LEN_USER_FLAG				32		// 用户标识

#define		LEN_DRIVER_



#define		LEN_ALL_DATA				0x7000// 采集所有数据

#define		LEN_OVER_TIME_1REC		30		// 疲劳驾驶记录每条信息长度字节		   原来是28				新GB是30
#define		LEN_OVER_SPEED_1REC		32		// 超速记录每条信息长度字节
#define		LEN_ID_CARD_1REC			24		// 驾驶员身份认证存储的长度(证号+插卡时间)
#define		LEN_RUN_STATUS_CNT	   15		// 车辆行驶停止和疲劳驾驶标志位的长度

#define		LEN_LAST_5_DRIVERS_1REC	36		// 最近5个驾驶员（人次）驾驶信息每第的字节长度

#define		LEN_KV_OP_1REC				8		// 车辆特征系数下载操作记录一条的长度
#define		LEN_KV_OP_REC				800	// 车辆特征系数下载操作记录的存储长度

#define		LEN_48Hour_SPEED_DATA	(126*60*48UL)			// 48小时行驶速度长度	126 * 60 * 48
#define		LEN_360H_POS_DATA			(666*360UL)	// 360小时位置信息
#define		LEN_OVER_TIME_DATA		(50*100)	// 超时数据块长度
#define		LEN_DRIVER_OUT_IN_DATA	(25*200)	// 驾驶员登出登入数据块长度
#define		LEN_GET_POWER_RECORD_DATA (7*100)	// 外部供电数据块长度
#define		LEN_GET_MODIFY_PARA_RECORD_DATA	(7*100)		//  参数修改
#define	   LEN_GET_SPEED_STATUS_LOG_DATA	   (133*10)	 // 速度状态日志

#define		LEN_INSTALL_DATE			6		// 初次安装日期和时间

#define		LEN_20S_DATA				234	// 一次20S数据长度
#define		LEN_STOP_DATA				(100*LEN_20S_DATA)	// 事故疑点数据长度 100条
//#define		LEN_20S_DATA_DRIVER	   180	// 10次疑点数据对应的10个驾驶证号的总长度
//#define		LEN_MILAGE_2DAY			3		// 最近2个日历天内的累计行驶里程
//#define		LEN_360H_DATA				(65*360)	// 360小时内速度数据

#define		LEN_CHECK_SPEED_MILAGE	45		// 用户标识


//#define     LEN_GPS_Standard			2    //记录仪执行的标准版本号

#define     LEN_SETUP_TIME          6     //安装时间

#define		LEN_INIT_MIL			4	  // 初始里程

#define		LEN_GPS_TIME_DIFFERENCE	4		// 


#define     LEN_SIGNAL_STATUS_1PEC   87   //状态信号配置信息长度		  

//#define     LEN_SIGNAL_STATUS_NUMBER 4*8  //信号状态个数，6个字节48个


#define    MAX_SIGNAL_STATUS_BYTE_NUMBER 	12  	//信号状态字节个数


//#define     LEN_TIRE_DRIVE_IN_FM     240		  //疲劳驾驶记录在FM的数据长度
//#define     LEN_LAST_22S_IN_FM       244		  //事故疑点记录在FM的数据长度
//#define     LEN_48HSPEED_IN_FM       246 		  //48小时速度记录在FM的数据长度
//#define     LEN_360HSPEED_IN_FM      245		  //360小时速度记录在FM的数据长度
//#define     LEN_360HLocation_IN_FM   245		  //360小时位置记录在FM的数据长度
//#define     LEN_Drv_InOut_IN_FM		 225		  //登录退出记录在FM的数据长度
//#define     LEN_Powr_OnOff_IN_FM		 238		  //上电掉电记录在FM的数据长度
//#define     LEN_Para_Cha_IN_FM		 245		  //参数修改记录在FM的数据长度
//
//#define     RD_TIRE_DRIVE_IN_FM_TIMES        8		//疲劳驾驶记录在FM的记录次数
//#define     RD_48HSPEED_IN_FM_TIMES          240 	//48小时速度记录在FM的记录次数
//#define     RD_360HSPEED_IN_FM_TIMES         240	//360小时速度记录在FM的记录次数
//#define     RD_360HLocation_IN_FM_TIMES      30	   //360小时位置记录在FM的记录次数
//#define     RD_Drv_InOut_IN_FM_TIMES		   9		//登录退出记录在FM的记录次数
//#define     RD_Powr_OnOff_IN_FM_TIMES		   34		//上电掉电记录在FM的记录次数
//#define     RD_Para_Cha_IN_FM_TIMES		      35		//参数修改记录在FM的记录次数



#define   	USBGBDATANUMBER						16    //USB数据块个数	最多22块

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


GB19056_EXT struct {
	uchar Cmd;
	uchar StartTime[6];
	uchar EndTime[6];
	uint16 TotalItem;
	uchar GetVDRFlag;
	//uint16 TotalPack;
	//uint16 PackIndex; 
	uchar SearchFlag;   // 搜索标志： 0：从头开始搜索flash  1：从上次flash地址搜索
	uchar	OverTimeSearch; // 清除	SearchFlag 的超时

}GB_LargeData;

// 外部电源供电状态
#define		LEN_EXT_POWER_STATUS		7
typedef struct {
	uchar Time[6];
	uchar Status; 			 // 1:通电，2：断电
}EXT_POWER_STATUS,*pEXT_POWER_STATUS;


typedef	struct
{
	uint32	FMAddr;			  // 数据在铁电的地址
	uint32	FMCnt;			  // 数据条目总计数
	uint32	FMSaveItemCnt;	  // 数据条目能保存在铁电的条数
	uint32	SaveItemLen;		// 数据条目长度
	uint32	FlashStartPageAddr;			// 数据条目在flash的开始页地址
	uint32	FlashPageSaveCnt;	 // 数据条目在flash的计数
	uint32	FlashTotalPages;	 //  数据条目在flash总共的页数 
	uchar    TimeBCDPos;				// 每个数据条目时间在哪个位置	 		
	uchar		*ptBuf;				   // 临时缓存	  
	uchar 	*pSendBuffer;			 // 发送缓存
		
} VDR_DATA_FUN_1_ST;

// 参数修改
#define		LEN_PARA_MODIFY_RECORD		7
typedef struct {
	uchar Time[6];		  // 时间
	uchar EventType; 			 // 	  事件类型为设置参数命令字
}PARA_MODIFY_RECORD,*pPARA_MODIFY_RECORD;
 
 

#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
