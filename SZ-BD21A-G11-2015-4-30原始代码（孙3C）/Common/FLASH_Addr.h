/************************************ Copyright (c) **********************************************
**
**
**
**										  		All rights reserved.	  
************************************* Copyright (c) *********************************************** 
**                                      文件描述
**	文件名:   FLASH_Addr.h
**	功能描述：FLASH存储地址分配
**	编译环境：
**	目标CPU: 
**	作者：	 
**	创建时间：2009.09.09
**	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
******************************************************************************************************/					  

#ifndef      	__FLASH_ADDR_H__
#define      	__FLASH_ADDR_H__

#ifdef      	FLASH_GLOBALS
#define     	FLASH_EXT
#else
#define     	FLASH_EXT    extern
#endif

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	  			Flash存储器中的起始地址(低地址在前)
				MX25L3205  4MByte  flash  记录保存在4096页开始的1M空间内 ，最小擦除单位为64K=256页	块地址计算 X = 页数/256
*/

#define		FF_BYTES_PER_PAGE			256UL
#define		FF_PAGE_PER_BLOCK			256UL
#define		FF_PAGE_PER_SECTOR		16UL
#define		FF_FLASH_TOTAL_PAGE		32768UL	  //16*16*128





//////////// 各参数存储在Flash的总页数（每页256B），开始页地址和总页数必须是16的整数倍
#define		FF_IAP1_CODE_TOTAL_PAGE					2048UL		// 	预留512KB，升级镜像1
#define		FF_IAP2_CODE_TOTAL_PAGE					2048UL		// 	预留512KB，升级镜像2	  应付无锡国标检测，用于48小时速度
#define     FF_48H_SPEED_TOTAL_PAGE			      1536UL		//		48小时速度数据大小1530页
#define		FF_360H_LOCATION_TOTAL_PAGE			1280UL     	//		360小时位置记录数据大小
#define		FF_PARKING_DATA_TOTAL_PAGE          256UL			//		事故疑点数据大小256页	 64k=256*256/1024
#define		FF_TIRE_DRIVE_TOTAL_PAGE            64UL       	//		疲劳驾驶数据大小
#define		FF_DRIVER_IN_OUT_TOTAL_PAGE         64UL			//		驾驶员登录记录数据大小
#define		FF_POWER_ON_OFF_TOTAL_PAGE          64UL	      //		断电上电记录数据大小
#define		FF_PARA_MODIFY_TOTAL_PAGE           64UL	      //		参数修改记录数据大小
#define		FF_SPEED_STATUS_TOTAL_PAGE				64UL			//		速度状态
#define		FF_OVER_SPEED_TOTAL_PAGE				64UL			//		超速记录
#define		FF_SYSTEM_LOG_TOTAL_PAGE				64UL			//		系统日志，每月存储1次，每次1页
#define		FF_RSV_N_PAGE_TOTAL_PAGE				256UL			// 	
#define		FF_ROUND_FENCE_TOTAL_PAGE				32UL      	//    0x1ee0 	圆形电子围栏
#define		FF_RECTANGLE_FENCE_TOTAL_PAGE			32UL      	// 	方形电子围栏
#define		FF_POLYGON_FENCE_TOTAL_PAGE			128UL      	// 	多边形电子围栏数据大小
#define		FF_ROAD_PLAN_TOTAL_PAGE             256UL			// 	道路规划数据大小
#define		FF_CAN_DATA_TOTAL_PAGE					512UL			// 	可存储10752条
#define		FF_ALARM_MSG_TOTAL_PAGE					512UL			// 	只存储512条
#define		FF_NORMAL_MSG_TOTAL_PAGE				512UL			// 	只存储512条
#define		FF_GPS_MESSAGE_TOTAL_PAGE				1280UL		// 	可存储11520条
#define		FF_GPS_BLIND_MESSAGE_TOTAL_PAGE		1280UL		// 	可存储11520条
#define		FF_GNSS_DATA_TOTAL_PAGE					1280UL		// 	可存储11520条
#define		FF_PIC_TOTAL_PAGE							16896UL		// 	
#define		FF_COMMON_TOTAL_PAGE						128UL			// 	8个扇区，做数据交换使用
#define		FF_FONT_POINT_TOTAL_PAGE				1024UL		//

#define		FF_SAVE_ALARM_TOTAL_MESSAGE			(256*3)

///////////////////////////////////////////// 各数据存储的开始页地址
#define		FF_IAP1_CODE_START_PAGE				0
#define		FF_IAP1_CODE_END_PAGE				2047
#define		FF_IAP2_CODE_START_PAGE				2048
#define		FF_IAP2_CODE_END_PAGE				4095

//#define		FF_REMOTE_UPDATA_ADDR	0			// 0～4095页，远程升级
#define		FF_REMOTE_UPDATA_ADDR	0			// 0～2048页，远程升级			  后2048给48小时速度，因检测时，不止48小时，会把数据冲掉
#define		FF_VDR_BASE_ADDR						2048 //4096

#define		FF_48H_SPEED_START_PAGE				FF_VDR_BASE_ADDR
#define		FF_48H_SPEED_END_PAGE				(FF_48H_SPEED_START_PAGE+FF_48H_SPEED_TOTAL_PAGE-1)

#define		FF_360H_LOCATION_START_PAGE		(FF_48H_SPEED_END_PAGE+1)
#define		FF_360H_LOCATION_END_PAGE			(FF_360H_LOCATION_START_PAGE+FF_360H_LOCATION_TOTAL_PAGE-1)

#define		FF_PARKING_DATA_START_PAGE			(FF_360H_LOCATION_END_PAGE+1)
#define		FF_PARKING_DATA_END_PAGE			(FF_PARKING_DATA_START_PAGE+FF_PARKING_DATA_TOTAL_PAGE-1)

#define		FF_TIRE_DRIVE_START_PAGE			(FF_PARKING_DATA_END_PAGE+1)
#define		FF_TIRE_DRIVE_END_PAGE				(FF_TIRE_DRIVE_START_PAGE+FF_TIRE_DRIVE_TOTAL_PAGE-1)

#define		FF_DRIVER_IN_OUT_START_PAGE		(FF_TIRE_DRIVE_END_PAGE+1)
#define		FF_DRIVER_IN_OUT_END_PAGE			(FF_DRIVER_IN_OUT_START_PAGE+FF_DRIVER_IN_OUT_TOTAL_PAGE-1)

#define		FF_POWER_ON_OFF_START_PAGE			(FF_DRIVER_IN_OUT_END_PAGE+1)
#define		FF_POWER_ON_OFF_END_PAGE			(FF_POWER_ON_OFF_START_PAGE+FF_POWER_ON_OFF_TOTAL_PAGE-1)

#define		FF_PARA_MODIFY_START_PAGE			(FF_POWER_ON_OFF_END_PAGE+1)
#define		FF_PARA_MODIFY_END_PAGE				(FF_PARA_MODIFY_START_PAGE+FF_PARA_MODIFY_TOTAL_PAGE-1)

#define		FF_SPEED_STATUS_START_PAGE			(FF_PARA_MODIFY_END_PAGE+1)
#define		FF_SPEED_STATUS_END_PAGE			(FF_SPEED_STATUS_START_PAGE+FF_SPEED_STATUS_TOTAL_PAGE-1)

// 以上为行驶记录仪数据存储区

#define		FF_OVER_SPEED_START_PAGE			(FF_SPEED_STATUS_END_PAGE+1)
#define		FF_OVER_SPEED_END_PAGE				(FF_OVER_SPEED_START_PAGE+FF_OVER_SPEED_TOTAL_PAGE-1)
	
#define		FF_SYSTEM_LOG_START_PAGE			(FF_OVER_SPEED_END_PAGE+1)
#define		FF_SYSTEM_LOG_END_PAGE				(FF_SYSTEM_LOG_START_PAGE+FF_SYSTEM_LOG_TOTAL_PAGE-1)

#define		FF_RSV_N_PAGE_START_PAGE			(FF_SYSTEM_LOG_END_PAGE+1)
#define		FF_RSV_N_PAGE_END_PAGE				(FF_RSV_N_PAGE_START_PAGE+FF_RSV_N_PAGE_TOTAL_PAGE-1)

#define		FF_ROUND_FENCE_START_PAGE			(FF_RSV_N_PAGE_END_PAGE+1)
#define		FF_ROUND_FENCE_END_PAGE				(FF_ROUND_FENCE_START_PAGE+FF_ROUND_FENCE_TOTAL_PAGE-1)

#define		FF_RECTANGLE_FENCE_START_PAGE		(FF_ROUND_FENCE_END_PAGE+1)
#define		FF_RECTANGLE_FENCE_END_PAGE		(FF_RECTANGLE_FENCE_START_PAGE+FF_RECTANGLE_FENCE_TOTAL_PAGE-1)

#define		FF_POLYGON_FENCE_START_PAGE		(FF_RECTANGLE_FENCE_END_PAGE+1)
#define		FF_POLYGON_FENCE_END_PAGE			(FF_POLYGON_FENCE_START_PAGE+FF_POLYGON_FENCE_TOTAL_PAGE-1)

#define		FF_ROAD_PLAN_START_PAGE				(FF_POLYGON_FENCE_END_PAGE+1)
#define		FF_ROAD_PLAN_END_PAGE				(FF_ROAD_PLAN_START_PAGE+FF_ROAD_PLAN_TOTAL_PAGE-1)

#define		FF_CAN_DATA_START_PAGE				(FF_ROAD_PLAN_END_PAGE+1)
#define		FF_CAN_DATA_END_PAGE					(FF_CAN_DATA_START_PAGE+FF_CAN_DATA_TOTAL_PAGE-1)

#define		FF_ALARM_MSG_START_PAGE				(FF_CAN_DATA_END_PAGE+1)
#define		FF_ALARM_MSG_END_PAGE				(FF_ALARM_MSG_START_PAGE+FF_ALARM_MSG_TOTAL_PAGE-1)

#define		FF_NORMAL_MSG_START_PAGE			(FF_ALARM_MSG_END_PAGE+1)
#define		FF_NORMAL_MSG_END_PAGE				(FF_NORMAL_MSG_START_PAGE+FF_NORMAL_MSG_TOTAL_PAGE-1)

#define     FF_GPS_MESSAGE_START_PAGE	 		(FF_NORMAL_MSG_END_PAGE+1)		// GPS历史信息记录起始页地址
#define     FF_GPS_MESSAGE_END_PAGE		 		(FF_GPS_MESSAGE_START_PAGE+FF_GPS_MESSAGE_TOTAL_PAGE-1)	// GPS历史信息记录终止页地址()

#define     FF_GPS_BLIND_MESSAGE_START_PAGE	(FF_GPS_MESSAGE_END_PAGE+1)	// GPS信息盲区补偿起始页地址
#define     FF_GPS_BLIND_MESSAGE_END_PAGE		(FF_GPS_BLIND_MESSAGE_START_PAGE+FF_GPS_BLIND_MESSAGE_TOTAL_PAGE-1)

#define		FF_GNSS_DATA_START_PAGE				(FF_GPS_BLIND_MESSAGE_END_PAGE+1)
#define		FF_GNSS_DATA_END_PAGE				(FF_GNSS_DATA_START_PAGE+FF_GNSS_DATA_TOTAL_PAGE-1)

#define		FF_PIC_START_PAGE						(FF_GNSS_DATA_END_PAGE+1)
#define		FF_PIC_END_PAGE						(FF_PIC_START_PAGE+FF_PIC_TOTAL_PAGE-1)
#define		FF_PIC_BLOCK_ADDR_BASE  			(FF_PIC_PAGE_ADDR_BASE/FF_PAGE_PER_BLOCK)    // 图片地址块地址， 用于8M的flash

#define 	 	FF_COMMON_START_PAGE   				(FF_PIC_END_PAGE+1)
#define		FF_COMMON_END_PAGE					(FF_COMMON_START_PAGE+FF_COMMON_TOTAL_PAGE-1)

//////////// 字库存储首页地址，固定使用最高256KB
#define		FF_FONT_POINT_START_PAGE			31744UL





#define			TOTAL_PHOTO_NUM				   64 //16        //64
#define			MAX_PIC_PACKET						128            

#define			PIC_BLOCK_ADDR_BASE         	(FF_PIC_START_PAGE/FF_PAGE_PER_BLOCK+1)      // 图片地址块地址， 用于8M的flash




#if(	(FF_COMMON_END_PAGE+1) > FF_FONT_POINT_START_PAGE ) 
#error "Flash地址超出范围，请调整大小！！！"
#endif





#endif


/************************** End of File *************************************************/

