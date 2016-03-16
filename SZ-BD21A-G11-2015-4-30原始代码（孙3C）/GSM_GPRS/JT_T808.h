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

#ifndef   	__JT_T808_H__
#define   	__JT_T808_H__

#include		"Config\UserConfig.h"


#ifdef      JT808_GLOBALS
#define     JT808_EXT
#else
#define     JT808_EXT    extern
#endif

#define		BB_CMD_ADDR_HEAD						0		        // 	标识位起始字节 
#define		BB_CMD_ADDR_MSG_ID					1				  //	消息ID起始字节
#define		BB_CMD_ADDR_MSG_ATTR					3				  // 	消息体属性起始字节
#define		BB_CMD_ADDR_SIM						5				  //	手机号起始字节
#define		BB_CMD_ADDR_INDEX						11				  //	消息流水号起始字节
#define		BB_CMD_ADDR_MSG_PACKETS				13				  //	消息总包数起始字节
#define		BB_CMD_ADDR_MSG_PACKET_INDEX		15				  // 	包序号起始字节
#define		BB_CMD_ADDR_DATA_BLOCK				17				  //	消息体起始字节
#define		LEN_BB_PACK_HEAD						17				  // 	标识位与消息头的总长度

#define		LEN_BB_PACK_ITEM						4             // 消息包封装项长度


//#define		BB_CMD_ADDR_MSG_ID					0				  //	消息ID起始字节
//#define		BB_CMD_ADDR_MSG_ATTR					2				  // 	消息体属性起始字节
//#define		BB_CMD_ADDR_SIM						4				  //	手机号起始字节
//#define		BB_CMD_ADDR_INDEX						10				  //	消息流水号起始字节
//#define		BB_CMD_ADDR_MSG_PACKETS				12				  //	消息总包数起始字节
//#define		BB_CMD_ADDR_MSG_PACKET_INDEX		14				  // 	包序号起始字节
//#define		BB_CMD_ADDR_DATA_BLOCK				16				  //	消息体起始字节
//#define		LEN_BB_PACK_HEAD						17				  // 	标识位与消息头的总长度


#define 		LEN_BB_REG_PACKET					49		//	37			 modified 2013.3
#define			TER_SUPPORT_CAR_TYPE			((1<<1) | (1<<2) | (1 << 7))		 // 适用危险品车辆，适用普通货车，一体机
#define			TER_GNSS_PRO					(1<<1) 								 // 支持北斗
#define			TER_GSM_PRO						(1<<0)	 							 //	支持GPRS


#ifndef 		MSG_ERROR
#define 		MSG_ERROR 							0x02		  // 消息有误
#endif
#ifndef 		NOT_SUPPORT
#define 		NOT_SUPPORT 						0x03		  // 不支持的消息
#endif
#ifndef 		MSG_ALARM_CONFIRM
#define 		MSG_ALARM_CONFIRM 				0x04		  // 报警处理确认
#endif		



// modified 2013.3
#define	 LEN_BASE_POS							28          				 // 位置基本信息长度
#define  LEN_POSITION_REPORT   				85		   					 // 位置基本信息和附加信息总长	 (预留有油量信息的长度)
#define	 LEN_POSITION_REPORT_NOW    		(57+22)         					 // 现行使用的位置汇报消息体总长  (（注：不含油量以及路线行驶时间不足/过长报警附加信息）)                      					
#define  LEN_JT_POS_RETRY           		85        				 //  位置信息重发包数据段长度	要和位置基本信息和附加信息总长一致
//



#define 	 LEN_AUTH_CODE		  	  				32

#define	 BB_DEFAULT_TIME		  				10
#define  BB_SEND_PIC_TIME      				40
#define  BB_POS_TIME		  	  					60			 // 60秒 上报位置
#define	 BB_HEARTBEAT_TIME	  				120        // 心跳时间 秒
#define	 BB_RETRY_TIME			  				2          //  重传次数

#define	 LEN_MEDIA_EVENT_REPORT      		8          					 // 多媒体事件上报长度
#define	 LEN_MEDIA_SEARCH_FORMAT     		35         					 // 多媒体检索格式长度
#define	 LEN_SET_MEDIA_DATA_UPLOAD   		16   		 					 // 存储多媒体数据上传长度

#define	 OVERSPEED_TYPE_POS_TYPE_INDEX	(16 + LEN_BASE_POS)         // 超速报警附加信息类型中的位置类型索引地址


#define		IC_CARD_IS_OUT						0x02	   // IC卡拔出	 (驾驶员上班)
#define		IC_CARD_INSERT						0x01	   // IC卡插入	 (驾驶员下班)

#define		LEN_GSM_IMEI						15            // IMEI号长度
#define		LEN_SIM_CARD_ICCID				20			  // ICCID号长度

#define		PROVINCE_REGION_ID			 	0x00 //0x0044 	// 省域  0x44 :广东省
#define		PREFECTURE_REGION_ID		 		0x00  //0x0064	 // 市域或县域  0x0064 : 广州市

#define		JT_DRIVER_NAME_LEN_INDEX	(INDEX_DEVICE_PROTOCOL_DATA_BLOCK+1)	
	


#endif    // 

JT808_EXT		enum
{
	eBB_ACK_OK = 0,
	eBB_ACK_FAILURE,
	eBB_ACK_MSG_ER,
	eBB_ACK_NOT_SUPPORT,
	eBB_ACK_ALARM_ACK,
}eBB_ACK_RESULT;



JT808_EXT		enum
{
	eBB_gFLAG_REG_INDEX,								                       //  jt_recv_flag 中第bit0  位	  收到注册应答
	eBB_gFLAG_AUTH_INDEX,														  //	jt_recv_flag 中第bit1  位	  收到鉴权应答
	eBB_gFLAG_HEART_INDEX,														  //  jt_recv_flag 中第bit2  位	  收到心跳应答
	eBB_gFLAG_POS_INDEX,															  //  jt_recv_flag 中第bit3  位	  收到位置上报应报
	eBB_gFLAG_OTHER_INDEX,														  //  jt_recv_flag 中第bit4  位	  收到其他命令应答
   eBB_gFlag_DEREG_INDEX, 
} BB_FLAG_ENUM;


// 附加信息
JT808_EXT		enum
{
	eBB_POS_ADD_MIL					        = 0x01,       // 位置信息上报 里程
	eBB_POS_ADD_OIL,								             // 油量
	eBB_POS_ADD_SPEED,                                  // 速度
	eBB_POS_ADD_MANUL_ALARM_EVENT_ID,								 // 需要人工确认报警事件的ID
	eBB_POS_ADD_OVER_SPEED_ALARM 			  = 0x11,       // 超速报警
	eBB_POS_ADD_INOUT_AREA_ROAD_ALARM,                  // 进出区域/路线报警
	eBB_POS_ADD_ROAD_SECTION_DRIVE_ALARM,               // 路段行驶时间不足/过长报警
	//added 2013.3
	eBB_POS_ADD_EXT_ALARM					 = 0x24,
	eBB_POS_ADD_EXTEN_VS_STATUS			  = 0x25,	// 扩展车辆信号状态
	eBB_POS_ADD_IO_STATUS					  = 0x2A,   // IO状态位
	eBB_POS_ADD_ANALOG,									// 模拟量 bit0-15: AD0; bit16-31, AD1
	eBB_POS_ADD_GSM_SIGNAL					  = 0x30,   // 无线通信网络信号强度
	eBB_POS_ADD_GNSS_SATELLITE_NUMS						// GNSS定位卫星数

} BB_POS_REPORT_ADD_ENUM;

JT808_EXT		enum
{
	eBB_AREA_ALARM_NONE,					// 0：无特定位置
	eBB_AREA_ALARM_ROUND,				// 1：圆形区域
	eBB_AREA_ALARM_RECT,					// 2：矩形区域
	eBB_AREA_ALARM_POLYGON,				// 3: 多边形区域
	eBB_AREA_ALARM_ROAD_SECTION,		// 4：路段
	
} BB_AREA_ALARM_ENUM;


JT808_EXT		enum
{
	eBB_IN_AREA = 0,
	eBB_OUT_AREA = 1,
	eBB_NO_IN_OUT = 2,
}ENUM_IN_OUT_TYPE;



JT808_EXT	  enum
{
	eBB_POS_NORMAL,

	eBB_POS_HAS_ALARM        			     = 0xFA,
	eBB_POS_NETWORK_OFF                   = 0xFB,
	eBB_POS_SEND_SAVED                    = 0xFC,
} BB_POS_SAVE_FLAG;



/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	部标协议代码枚举
*/
JT808_EXT		enum
{
	/* 终端(termial)消息ID */
	eBB_gCMD_TER_ACK 								= 0x0001,	  // 终端通用应答
	eBB_gCMD_TER_HEARTBEAT,										  // 终端心跳
	eBB_gCMD_TER_DEREG,	 										  // 终端注销  
	eBB_gCMD_TER_REG								= 0x0100,	  // 终端注册
	eBB_gCMD_TER_AUTH								= 0x0102,		  // 终端鉴权
	eBB_gCMD_TER_QUERY_PARAM_ACK				= 0x0104,	  // 查询终端参数应答
	eBB_gCMD_TER_QUERY_PRO_ACK					= 0x0107,	  // 查询终端属性应答 added 2013.4
	eBB_gCMD_TER_CREATE_USER					= 0x0110,	  // 使用前锁定（快速开户）
	eBB_gCMD_TER_POS_REPORT 					= 0x0200,	  // 位置信息汇报
	eBB_gCMD_TER_POS_QUERY_ACK,								  // 位置信息查询应答
	eBB_gCMD_TER_EVENT_REPORT					= 0x0301,	  // 事件报告
	eBB_gCMD_TER_ASK_ACK,							 			  // 提问应答
	eBB_gCMD_TER_GET_INFO,										  // 信息点播/取消																				  // K
	eBB_gCMD_TER_VEH_CTRL_ACK				   = 0x0500,	  // 车输(vehicle)控制应答
	eBB_gCMD_TER_VDR_DATA_UPLOAD           = 0x0700,     // 行驶记录仪数据上传
	eBB_gCMD_TER_ELE_WAYBILL_UPLOAD,							  // 电子运单上传
	eBB_gCMD_TER_DRIVER_INFO_REPORT,							  // 驾驶员身份信息采集上报
	eBB_gCMD_TER_POS_INFO_MULTI_UPLOAD		= 0x0704,			// 定位信息批量上传
	eBB_gCMD_TER_CAN_DATA_UPLOAD		    	= 0x0705,			// CAN总线数据上传
	eBB_gCMD_TER_MEDIA_EVENT_UPLOAD			= 0x0800,     // 多媒体事件信息上传
	eBB_gCMD_TER_MEDIA_DATA_UPLOAD,      					  // 多媒体数据上传
	eBB_gCMD_TER_MEDIA_SERCH_ACK,                       // 存储多媒体数据检索应答
	eBB_gCMD_TER_TAKE_PHOTO_ACK				= 0x0805,		// 摄像头立即拍摄命令应答
	eBB_gCMD_TER_DATA_TRAN_UPLOAD				= 0x0900,	  // 数据上行透传 
	eBB_gCMD_TER_DATA_COMPRESS_UPLOAD,						  // 数据压缩上报
	eBB_gCMD_TER_RSA_PUB_KEY					= 0x0A00,     // 终端RSA公钥
	

	/* 平台(platform)消息ID	*/
	eBB_gCMD_PLAT_ACK								= 0x8001,     // 平台通用应答
	eBB_gCMD_RESEND_PART_PACK_REQUEST		= 0x8003,		//  补传分包请求	 added  2013.3
	eBB_gCMD_PLAT_TO_TER_REG_ACK				= 0x8100,	  // 终端注册应答		
	eBB_gCMD_PLAT_SET_TER_PARA					= 0x8103,     // 设置终端参数
	eBB_gCMD_PLAT_QUERY_TER_PARA,								  // 查询终端参数
	eBB_gCMD_PLAT_CTRL_TER,										  // 终端控制
	eBB_gCMD_PLAT_QUERY_SOME_TER_PARA,							// 查询指定终端参数		 addded 2013.3
	eBB_gCMD_PLAT_QUERY_TER_PRO,								// 查询终端属性			added 2013.4

	eBB_gCMD_PLAT_QUERY_TER_POS				= 0x8201,	  // 位置信息查询
	eBB_gCMD_PLAT_TRACE_TER_POS,						        // 临时位置跟踪控制
	eBB_gCMD_PLAT_SEND_TEXT_INFO				= 0x8300,     // 文本信息下发
	eBB_gCMD_PLAT_SET_EVENT,									  // 事件设置
	eBB_gCMD_PLAT_SEND_ASK,										  // 提问下发
	eBB_gCMD_PLAT_SET_INFO_MENU,								  // 信息点播菜单设置
	eBB_gCMD_PLAT_INFO_SERVICE,								  // 信息服务
	eBB_gCMD_PLAT_TEL_DIAL						= 0x8400,	  // 电话回拔
	eBB_gCMD_PLAT_SET_TEL_BOOK,								  // 设置电话本
	eBB_gCMD_PLAT_VEH_CTRL						= 0x8500,	  // 车辆控制
	eBB_gCMD_PLAT_SET_ROUND_REGION  			= 0x8600,	  // 设置圆形区域
	eBB_gCMD_PLAT_DEL_ROUND_REGION,							  // 删除圆形区域
	eBB_gCMD_PLAT_SET_RECT_REGION,							  // 设置矩形区域
	eBB_gCMD_PLAT_DEL_RECT_REGION,							  // 删除矩形区域
	eBB_gCMD_PLAT_SET_POLY_REGION,							  // 设置多边形区域
	eBB_gCMD_PLAT_DEL_POLY_REGION,							  // 删除多边形区域
	eBB_gCMD_PLAT_SET_PATH,										  // 设置路线
	eBB_gCMD_PLAT_DEL_PATH,										  // 删除路线
	eBB_gCMD_PLAT_GET_VDR_DATA					= 0x8700,	  // 行驶记录仪数据采集命令
	eBB_gCMD_PLAT_SET_VDR_PARA,						  		  // 行驶记录仪参数下传命令
	eBB_gCMD_PLAT_DRIVER_INFO_REQUEST,						  // 上报驾驶员身份信息请求
	eBB_gCMD_PLAT_MEDIA_DATA_ACK				= 0x8800,     // 多媒体数据上传应答
	eBB_gCMD_PLAT_TAKE_PHOTO,                            // 摄像头立则拍摄命令
	eBB_gCMD_PLAT_MEDIA_DATA_SERCH,							  // 存储多媒体数据检索
	eBB_gCMD_PLAT_SET_MEDIA_DATA_UPLOAD,					  // 存储多媒体数据上传
	eBB_gCMD_PLAT_START_RECORD,								  // 开始录音命令
	eBB_gCMD_PALT_ONE_MEDIA_DATA_SERCH_UPLOAD,
	eBB_gCMD_PLAT_SEND_TRAN_DATA				= 0x8900,	  // 数据下行透传	
	eBB_gCMD_PLAT_RSA_PUB_KEY					= 0x8A00,	  // 平台RSA公钥







	/* 自定义下行(platform)消息ID	*/
	eBB_gCMD_PLAT_LISTEN_QUIET             = 0xA001,     //监听
	eBB_gCMD_PLAT_CUT_POWER,									  //断电
	eBB_gCMD_SET_SIM_NUMBER,                             //设置设备SIM卡号码
	eBB_gCMD_GET_SIM_NUMBER,                             //查询设备SIM卡号码
	eBB_gCMD_SET_ADSW_FLAG                 = 0xA005,     //设置AD通道做开关量检测高/低检测标志
	eBB_gCMD_GET_ADSW_FLAG,                              //查询AD通道做开关量检测高/低检测标志
	eBB_gCMD_GET_VERSION_MSG,                            //查询设备版本号信息
	eBB_gCMD_I2C_DRIVER_INFO_REQUEST       = 0xA008,	  // 查询上报使用I2C读到的驾驶员身份信息请求(普通IC卡)




   /* 自定义上行(platform)消息ID	*/
	eBB_gCMD_TPMS_DATA_UPLOAD				   = 0x2001,		// 轮胎气压数据上报
	eBB_gCMD_SYS_LOG_UPLOAD				   	= 0x2002,		// 系统日志参数上报
	eBB_gCMD_GET_SIM_NUMBER_ACK            =0x2004,        //查询设备SIM卡号码应答
	eBB_gCMD_GET_ADSW_FLAG_ACK             =0x2006,        //查询AD通道做开关量检测高/低检测标志的应答
	eBB_gCMD_GET_VERSION_MSG_ACK			   =0x2007,			 //查询设备版本号信息应答
	eBB_gCMD_I2C_DRIVER_INFO_REPORT		   = 0x2008,	  //  主动上报I2C读到的驾驶员身份信息采集上报	（普通IC卡）

	
		
} BB_CMD_ENUM;


JT808_EXT		enum
{
	/* 终端(termial)消息ID */
	SET_HEART_BEAT_ID 							   = 0x0001,	     // 终端心跳发送间隔
	SET_TCP_ACK_OVER_TIME_ID,										     // TCP应答超时时间
	SET_TCP_RESEND_TIMES_ID,								           //TCP重传次数
	SET_UDP_ACK_OVER_TIME_ID,											  //  UDP应答超时时间
	SET_UDP_RESEND_TIMES_ID,											  // UDP重传次数
	SET_SMS_ACK_OVER_TIME_ID,											  //  UDP应答超时时间
	SET_SMS_RESEND_TIMES_ID,											  // UDP重传次数
	
	SET_M_APN_ID								   	= 0x0010,		     //  主服务器APN
	SET_M_USE_NAME_ID,											        // 主服务器用户名
	SET_M_USE_SECRIERS_ID,							 			        // 主服务器拨号密码
	SET_M_IP_DNS_ID,											           // 主服务器IP地址或域名
	SET_B_APN_ID,											              //  备份服务器APN
	SET_B_USE_NAME_ID,											        // 备份服务器用户名
	SET_B_USE_SECRIERS_ID,							 			        // 备份服务器拨号密码
   	SET_B_IP_DNS_ID,											           // 备份服务器IP地址或域名
	SET_SERVER_TCP_PORT_ID,							 			        // 服务器TCP端口
  	SET_SERVER_UDP_PORT_ID,											     // 服务器UDP端口				

   SET_SERVER_IC_AUTH_MAIN_IP,											// 道路运输证IC 卡认证主服务器IP地址或域名
	SET_SERVER_IC_AUTH_TCP,											    // 道路运输证IC 卡认证主服务器TCP端口
	SET_SERVER_IC_AUTH_UDP,											    // 道路运输证IC 卡认证主服务器UDP端口
	SET_SERVER_IC_AUTH_BAK_IP,											// 道路运输证IC 卡认证备份服务器IP地址或域名，端口同主服务器
   	

	SET_POSITION_UP_MODES_ID				   	= 0x0020,			  // 回报策略
	SET_POSITION_UP_PORJECT_ID,							 			  // 位置回报方案
	SET_POSITION_UP_TIME_NO_DRIVER_ID,										  // 驾驶员未登录回报时间间隔

	SET_POSITION_UP_TIME_IDEL_ID					= 0x0027,			  //休眠时回报时间间隔
    SET_POSITION_UP_TIME_ALARM_ID,											  //紧急报警时间回报间隔
	SET_POSITION_UP_TIME_DEFAULT_ID,									  //缺省定时回报间隔
	
	
	SET_POSITION_UP_DISTANCE_DEFAULT_ID	   	= 0x002C,			  //缺省定距回报间隔
	SET_POSITION_UP_DISTANCE_NO_DRIVER_ID,										  // 驾驶员未登录回报距离间隔
	SET_POSITION_UP_DISTANCE_IDEL_ID,				 			  //休眠时回报距离间隔
    SET_POSITION_UP_DISTANCE_ALARM_ID,							  //紧急报警回报距离间隔
	SET_BREAK_POINT_ANGLE_ID,										  //拐角补传角度

	SET_ELE_RAID_RADIUS_ID						    = 0x0031,		// 电子围栏半径	（非法位移阈值） ，单位为米	  add 2013.3

	SET_PHONE_MONITOR_CENTER_ID					= 0x0040,	  //监控平台号码
	SET_PHONE_RESET_ID,									              //复位号码
	SET_PHONE_RESET_FACTORY_ID,									     //恢复出厂设置号码
	SET_PHONE_SMS_MONITOR_CENTER_ID,								  //监控平台SMS号码
	SET_PHONE_TEXT_SMS_ALARM_ID,									  //接收文本SMS报警号码
	SET_PHONE_GET_MODES_ID,											  //电话接通策略
	SET_PHONE_ONLINE_TIME_TIMES_LIMIT_ID,						  //每次最长通话时间
	SET_PHONE_ONLINE_TIME_MONTH_LIMIT_ID,						  //当月最长允许通话时长
	SET_PHONE_MONITORING_ID,											  //监听电话号码
	SET_PHONE_PRIVILEGE_SMS_ID,									  //特权短信号码


	SET_WORD_FOR_NO_ALARM_ID						= 0x0050,	  //报警屏蔽字
	SET_WORD_FOR_TEST_SMS_ALARM_ID,								  //报警发送文本SMS开关
	SET_WORD_FOR_TAKE_PHOTO_ALARM_ID,								  //报警拍照开关
	SET_WORD_FOR_TAKE_PHOTO_SAVE_ALARM_ID,						  //报警拍照保存开关
	SET_WORD_FOR_KEY_ALARM_ID,										  //关键报警标志
	SET_SPEED_HITH_LIMIT_ID,											  //最高速度
	SET_TIME_OVER_LIMIT_SPEED_ID,									  //超速持续时间
	SET_TIME_DRIVEING_LIMIT_ID,										  //持续驾驶时间门限
	SET_TIME_DRIVEING_LIMIT_ONE_DAY_ID,							  //一天累计驾驶时间门限
	SET_TIME_REST_ID,													  //最小休息时候
	SET_TIME_STOP_CAR_ID,												  //最长停车时间

	// add 2013.3
	SET_OVER_SPEED_PRE_ALARM_ID						= 0x005B,	 // 超速报警预警差值 
	SET_TRIAL_FATIGUE_DRIVE_PRE_ALARM_ID,							 // 疲劳驾驶预警差值
	SET_CRASH_ALARM_PARA_ID,									 // 碰撞报警参数设置：bit7-bit0: 碰撞时间，单位：4ms;bit15-bit8:碰撞加速，单位 0.1g, 设置范围:0 - 79之间,默认为10
	SET_TILT_ALARM_PARA_ID,										 // 侧翻报警参数设置： 侧翻角度，单位1度，默认为30度

    SET_FIXED_TIME_TAKE_PHOTO_CTL_ID				 = 0x0064,	 // 定时拍照控制
	SET_DISTANCE_TAKE_PHOTO_CTL_ID,					             // 定距拍照控制																										 
	// 

	SET_VIDIO_QUOLITY_ID						      = 0x0070,	  //图像/照片质量
	SET_VIDIO_BRIGHTNESS_ID,										     //亮度
	SET_VIDIO_CONTRAST_ID,										     //对比度
	SET_VIDIO_SATULATION_ID,										     //饱和度
	SET_VIDIO_CHROMA_ID,											     //色度


	SET_ODOMETER_VALUS_ID						   = 0x0080,	  //里程表读数
	SET_PROVINCIAL_ID,													  //省域
	SET_CITY_WHERE_ID,													  //市域
	SET_CAR_NUBER_ID,													  //车牌号码
	SET_CAR_COLOUR_ID,													  //车牌颜色

	// add 2013.3
	SET_GNNS_POS_MODE_ID						   = 0x0090,	// GNSS 定位模式
	SET_GNSS_BAUD_RATE_ID,										// GNSS波特率
	SET_GNSS_DATA_OUTPUT_FREQUENCY_ID,							// GNSS模块详细定位数据输出频率
	SET_GNSS_DATA_COLLECT_FREQUENCY_ID,							// GNSS模块详细定位数据采集频率
	SET_GNSS_DATA_UPLOAD_MODE_ID,							    // GNSS模块详细定位数据上传方式
	SET_GNSS_DATA_UPLOAD_SET_ID,							    // GNSS模块详细定位数据上传设置
	
	SET_CAN_CHAN_1_COLLECT_TIME_INTERVAL_ID    	= 0x0100,         // CAN总线通道1 采集时间间隔(ms)，0表示不采集
	SET_CAN_CHAN_1_UPLOAD_TIME_INTERVAL_ID,			        	// CAN总线通道1 上传时间间隔(ms)，0表示不采集
	SET_CAN_CHAN_2_COLLECT_TIME_INTERVAL_ID,         			// CAN总线通道2 采集时间间隔(ms)，0表示不采集
	SET_CAN_CHAN_2_UPLOAD_TIME_INTERVAL_ID,			        	// CAN总线通道2 上传时间间隔(ms)，0表示不采集
	SET_CAN_ID_COLLECT_SET_0_ID				  		= 0x0110,			// CAN总线ID单独采集设置0
	SET_CAN_ID_COLLECT_SET_15_ID			  			= 0x0120,			// CAN总线ID单独采集设置15

	SET_ACC_PARA											= 0xF00C,			//	加速度
	SET_ACC_DEC_PARA										= 0xF00D,			// 减速度
	// 															

		
} JT808_SET_PARA_ID;



#pragma		pack(1)
typedef		struct	_JT808_MSG_HEAD_
{	
	uint16	Cmd;
	union
	{
		uint16	B16;
		struct
		{
			uint16	DataLen:10;
			uchar		Encrypt:3;
			uchar		MultiPack:1;
			uchar		Rsv:2;
		}Bits;
	} Attr;	
	uchar		SimNum[6];
	uint16	Index;
	uint16	TotalPack;
	uint16	PackIndex;	
} JT808_MSG_HEAD,*pJT808_MSG_HEAD;




typedef		struct	_JT808_MSG_FRAME_
{	
	uchar		Head;
	uint16	Cmd;
	union	
	{
		uint16	B16;
		struct
		{
			uint16	DataLen:10;
			uchar		Encrypt:3;
			uchar		MultiPack:1;
			uchar		Rsv:2;
		}Bits;
	} Attr;	
	uchar		SimNum[6];
	uint16	Index;
	union
	{
		uchar		Buffer[1024];
		struct
		{
			uint16	TotalPack;
			uint16	PackIndex;	
		}PackNo;
	}Data;
} JT808_MSG_FRAME,*pJT808_MSG_FRAME;










#pragma		pack(1)
typedef	struct	_JT808_DEVICE_PROTOCOL_HEAD_
{
	uchar 	CheckSum;
	uint16  VersionNo;
	uint16	ManufacturerNo;
	uchar	DeviceTypeNo;
	uchar	Cmd;
	
}  JT808_DEVICE_PROTOCOL_HEAD,*pJT808_DEVICE_PROTOCOL_HEAD;

#define	INDEX_DEVICE_PROTOCOL_DATA_BLOCK	8      // 与外设通讯协议中的数据段下标			


typedef struct _JT808_COMMON_ACK_
{
	uint16 Index;
	uint16 MsgId;
	uint16 Result;
	
} JT808_COM_ACK, *pJT808_COM_ACK;

#pragma		pack(1)
typedef struct _JT808_MULTY_MEDIA_REPORT_
{
	uint32	MediaId;
	uchar		MediaType;
	uchar		MediaFormat;
	uchar		MediaEvent;
	uchar		MediaChannel;
} JT808_MULTY_MEDIA_REPORT, *pJT808_MULTY_MEDIA_REPORT;



typedef	struct _JT808_CAMERA_TAKE_PHOTO_
{
	uchar		Channel;				// 通道ID
	uint16		Cmd;					//	拍摄命令 0: 停止 ; 0xFFFF: 录像; 其他：拍照张数 
	uint16		interal;				// 拍照间隔/录像时间   单位为秒， 0: 最小间隔拍照或一直录像
	uchar		SaveFlag;			// 1： 保存; 0： 实时上传
	uchar		Res;					// 分辨率： 0x01: 320 * 240; 0x02: 640 * 480; 0x03: 800 * 600; 0x04: 1024 * 768 ...
	uchar		Qulity;				// 图像/视频质量  1 - 10， 1: 质量损失最小; 10: 压缩比最大
	uchar		Bright;				// 亮度	  0 ~ 255
	uchar		Contrast;			// 对比度  0 ~ 127
	uchar		Saturation;			// 饱和度  0 ~	127
	uchar		Chroma;				// 色度    0 ~ 255
	
}  JT808_CAMERA_TAKE_PHOTO, *pJT808_CAMERA_TAKE_PHOTO;



JT808_EXT struct _RETRY_SEND_PIC_ 
{
	uchar		SendPicNo; 
	uchar		TotalPack; 
	union {
		uchar		SendPackNo[125];
		uchar		MediaId[125];
	} SendPic;
	uchar		Index;	

}JtSendPic;

#define		LEN_TAKE_PHOTO_ACK		10
JT808_EXT struct __TAKE_PHOTO_ACK__
{
	uchar	MediaNum;
	uchar	MediaID[LEN_TAKE_PHOTO_ACK];
} TakePhotoAck;



JT808_EXT union  	_MULTY_MEDIA_DATA_
{
	 //  存储多媒体数据检索
	 struct					  
	 {
	 	uchar		MediaType;
		uchar	 	Channel;
		uchar		Event;
		uchar	   StartTime[6];	  // BCD码	 YY-MM-DD-hh-mm-ss
		uchar		EndTime[6];      // BCD码	 YY-MM-DD-hh-mm-ss
	 	
	 }SavedMediaSearch;	        

	 // 存储多媒体数据上传
	 struct
	 {
	 	uchar		MediaType;
		uchar	 	Channel;
		uchar		Event;
		uchar	   StartTime[6];	  // BCD码	 YY-MM-DD-hh-mm-ss
		uchar		EndTime[6];      // BCD码	 YY-MM-DD-hh-mm-ss
		uchar		DeleteFlag;
	 	
	 }SavedMediaUpload;
	

} MulMeDa;



 // 多媒体检索项数据格式
#define		LEN_MEDIA_SEARCH_STRUCT		35
typedef struct  
{
	uint32	MediaId;
	uchar		MediaType;
	uchar		Channel;
	uchar		Event;
	uchar		LocationInfo[28];          // 位置汇报消息体
} MEDIA_SEARCH_STRUCT,*pMEDIA_SEARCH_STRUCT;


JT808_EXT struct 						 // 上行透传
{
	uint16	Len;
	uchar		*ptype_and_data;		// ptype_and_data	指向的第一字节为透传消息类型，第二字节起为透传内容		
} JT_tran_upload;
 

/*
JT808_EXT  struct
{
	uint16  pos_count;			// 保存通信异常位置信息条数
	uint16  alarm_count; 	   // 保存关键报警位置信息条数
	uint16  send_pos_count;		// 发送通信异常位置信息条数
	uint16  send_alarm_count;	// 发送关键报警位置信息条数
} JT_saved_pos;					// 保存位置信息
*/


/*JT808_EXT  struct
{
	uchar   Length;
	uchar   Data[LEN_JT_POS_RETRY];
}JTPosAckBuf;	*/

JT808_EXT  JT808_MULTY_MEDIA_REPORT  MultiMediaReport;

JT808_EXT  uchar MediaEventFlag;


// added 2013.3
typedef union {
	uchar		AllData[104];
	struct 
	{
		uchar Stauts;   // 0x01: 从业资格证IC卡插入（驾驶员上班）; 0x02: 从业资格证IC卡拔出（驾驶员下班）.
		uchar TimeBCD[6];     // 时间
		uchar ReadICResult;    //IC卡读取结果
		uchar LenOfDriverName; // 驾驶员姓名长度
		uchar DriverName[20];      // 驾驶员姓名
		uchar DirverID[20];       // 从业资格证编码
		uchar LenOfDepartmentName;     //  发证机构名称长度
		uchar DepartmentName[50]; //   发证机构名称
		uchar PapersValidTimeBCD[4];       // 证件有效期	   YYYYMMDD
		
	} DriverICInfo;
} DRIVERICINFO;	


//

// added 2013.4
#pragma		pack(1)
typedef	struct __QUERY_TER_PRO__
{
	uchar	Type;				// 终端类型		  按JT808补充说明（2013.1.18）规定1字节
	uchar	ProductorID[5];		// 制造商ID
	uchar	Model[20];			// 终端型号
	uchar	TerID[7];			// 终端ID
	uchar	SimCardICCID[10];	// 终端SIM卡   BCD码
	uchar	LenHardVer;			// 终端硬件版本号长度
	uchar	HardVer[8];		// 终端硬件版本号
	uchar	LenFirmwareVer;		// 终端固件版本号长度
	uchar	FirmwareVer[8];    // 终端固件版本号
	uchar	GNSSPro;			// GNSS模块属性
	uchar	GSMPro;				// 通信模块属性					 
	
}QUERY_TER_PRO,*pQUERY_TER_PRO;
//


// added 2013.5
#pragma		pack(1)
typedef struct  __JT_CAN_MSG__ { 		
  uint32	CAN_ID;                
  uchar  CAN_Data[8];           
} JT_CAN_MSG;

#define	LEN_JT_CAN_MSG		12

//  CAN 总线数据上传数据格式
typedef struct  __JT_CAN_DATA_UPLOAD__ { 		
  uint16	CanItemSum;                	   // 	 包含的CAN总线数据项个数，>0 
  uchar  	CanFirstRecTimeBCD[5];          //    第1条CAN总线数据的接收时间，hh-mm-ss-msms
  JT_CAN_MSG CanItem[16];							
} JT_CAN_DATA_UPLOAD,*pJT_CAN_DATA_UPLOAD;



//JT808_EXT JT_CAN_DATA_UPLOAD  JT_CanDataUpload;


 // CAN总线设置
JT808_EXT  struct					  
{
	uint32	Can1SampleInterval;				 // CAN总线通道1采集时间间隔 (ms)
	uint16 	Can1UploadInterval;				 // CAN总线通道1上传时间间隔 (s)
	uint32	Can2SampleInterval;				 // CAN总线通道2采集时间间隔 (ms)
	uint16 	Can2UploadInterval;				 // CAN总线通道2上传时间间隔 (s)
}JT_CanSet;



JT808_EXT enum 
{
	CONNECT_AUTH_CHECK_ONLINE			=	  0xa1,		   // 检查是否上线，未上线等待达到超时返回
    CONNECT_AUTH_CONNECT_NO_WAIT  	=     0xa2,		   //  仅连接，不检查是否上线
	CONNECT_AUTH_WAIT						=	  0xa3,		   // 连接认证中心，并有超时机制

	CHECK_CENTER_ONLINE					= 	  0xa4,   	   // 检查是否上线，未上线等待达到超时返回
	CONNECT_CENTER_WAIT,							   //  仅连接，不检查是否上线
	CONNECT_CENTER_NO_WAIT,							   // 连接中心，并有超时机制
} JT_CONNECT_TYPE_ENUM;





#define		CONNECT_CREATE_USER_CENTER		'A'
#define		CONNECT_MAIN_CENTER				'B' 
#define		CONNECT_AUTH_CENTER				'C'

///////////////  使用前锁定
//#define		NEED_CREATE_USER					0xfa	

#define		NEED_CREATE_USER					OK	
	

// 使用前锁定（快速开户）加密方式
JT808_EXT  struct
{
	uchar	Flag;
	uint32	Key;
}JTCreatUserEncry;

JT808_EXT  uchar CarUserPhoneNumber[12];				// 
JT808_EXT  uchar CreateUserFlag, Creat_User_Reging;		 //   开户标志， 开户正在注册标志
JT808_EXT  uint32 Creat_User_Reging_OverTime;	


JT808_EXT uchar	AuthServerOverTime;			// 鉴权超时次数

JT808_EXT  uchar	JT_RemoteReadRecodMoreDataCmd;    // 远程读取记录仪数据，这个标志主要用于发送大数据


JT808_EXT  struct					 // 08H - 15H 数据检测所用结构体
{
	uint16 TotalPack;					// 总包
	uint16 PackIndex;					// 包序

}JT_VehRecord;	  // 部标的汽车记录仪数据结构



JT808_EXT		uint32  WithoutAckOfServer;	  //服务器无回复次数
JT808_EXT uchar	RegisterWithoutTer;
JT808_EXT uint32 RestartWithoutAckOfServer;
/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */

