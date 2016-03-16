/************************************ Copyright (c) **********************************************
**
**
**
**										  		All rights reserved.	  
************************************* Copyright (c) *********************************************** 
**                                      �ļ�����
**	�ļ���:   FLASH_Addr.h
**	����������FLASH�洢��ַ����
**	���뻷����
**	Ŀ��CPU: 
**	���ߣ�	 
**	����ʱ�䣺2009.09.09
**	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
******************************************************************************************************/					  

#ifndef      	__FLASH_ADDR_H__
#define      	__FLASH_ADDR_H__

#ifdef      	FLASH_GLOBALS
#define     	FLASH_EXT
#else
#define     	FLASH_EXT    extern
#endif

/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	  			Flash�洢���е���ʼ��ַ(�͵�ַ��ǰ)
				MX25L3205  4MByte  flash  ��¼������4096ҳ��ʼ��1M�ռ��� ����С������λΪ64K=256ҳ	���ַ���� X = ҳ��/256
*/

#define		FF_BYTES_PER_PAGE			256UL
#define		FF_PAGE_PER_BLOCK			256UL
#define		FF_PAGE_PER_SECTOR		16UL
#define		FF_FLASH_TOTAL_PAGE		32768UL	  //16*16*128





//////////// �������洢��Flash����ҳ����ÿҳ256B������ʼҳ��ַ����ҳ��������16��������
#define		FF_IAP1_CODE_TOTAL_PAGE					2048UL		// 	Ԥ��512KB����������1
#define		FF_IAP2_CODE_TOTAL_PAGE					2048UL		// 	Ԥ��512KB����������2	  Ӧ�����������⣬����48Сʱ�ٶ�
#define     FF_48H_SPEED_TOTAL_PAGE			      1536UL		//		48Сʱ�ٶ����ݴ�С1530ҳ
#define		FF_360H_LOCATION_TOTAL_PAGE			1280UL     	//		360Сʱλ�ü�¼���ݴ�С
#define		FF_PARKING_DATA_TOTAL_PAGE          256UL			//		�¹��ɵ����ݴ�С256ҳ	 64k=256*256/1024
#define		FF_TIRE_DRIVE_TOTAL_PAGE            64UL       	//		ƣ�ͼ�ʻ���ݴ�С
#define		FF_DRIVER_IN_OUT_TOTAL_PAGE         64UL			//		��ʻԱ��¼��¼���ݴ�С
#define		FF_POWER_ON_OFF_TOTAL_PAGE          64UL	      //		�ϵ��ϵ��¼���ݴ�С
#define		FF_PARA_MODIFY_TOTAL_PAGE           64UL	      //		�����޸ļ�¼���ݴ�С
#define		FF_SPEED_STATUS_TOTAL_PAGE				64UL			//		�ٶ�״̬
#define		FF_OVER_SPEED_TOTAL_PAGE				64UL			//		���ټ�¼
#define		FF_SYSTEM_LOG_TOTAL_PAGE				64UL			//		ϵͳ��־��ÿ�´洢1�Σ�ÿ��1ҳ
#define		FF_RSV_N_PAGE_TOTAL_PAGE				256UL			// 	
#define		FF_ROUND_FENCE_TOTAL_PAGE				32UL      	//    0x1ee0 	Բ�ε���Χ��
#define		FF_RECTANGLE_FENCE_TOTAL_PAGE			32UL      	// 	���ε���Χ��
#define		FF_POLYGON_FENCE_TOTAL_PAGE			128UL      	// 	����ε���Χ�����ݴ�С
#define		FF_ROAD_PLAN_TOTAL_PAGE             256UL			// 	��·�滮���ݴ�С
#define		FF_CAN_DATA_TOTAL_PAGE					512UL			// 	�ɴ洢10752��
#define		FF_ALARM_MSG_TOTAL_PAGE					512UL			// 	ֻ�洢512��
#define		FF_NORMAL_MSG_TOTAL_PAGE				512UL			// 	ֻ�洢512��
#define		FF_GPS_MESSAGE_TOTAL_PAGE				1280UL		// 	�ɴ洢11520��
#define		FF_GPS_BLIND_MESSAGE_TOTAL_PAGE		1280UL		// 	�ɴ洢11520��
#define		FF_GNSS_DATA_TOTAL_PAGE					1280UL		// 	�ɴ洢11520��
#define		FF_PIC_TOTAL_PAGE							16896UL		// 	
#define		FF_COMMON_TOTAL_PAGE						128UL			// 	8�������������ݽ���ʹ��
#define		FF_FONT_POINT_TOTAL_PAGE				1024UL		//

#define		FF_SAVE_ALARM_TOTAL_MESSAGE			(256*3)

///////////////////////////////////////////// �����ݴ洢�Ŀ�ʼҳ��ַ
#define		FF_IAP1_CODE_START_PAGE				0
#define		FF_IAP1_CODE_END_PAGE				2047
#define		FF_IAP2_CODE_START_PAGE				2048
#define		FF_IAP2_CODE_END_PAGE				4095

//#define		FF_REMOTE_UPDATA_ADDR	0			// 0��4095ҳ��Զ������
#define		FF_REMOTE_UPDATA_ADDR	0			// 0��2048ҳ��Զ������			  ��2048��48Сʱ�ٶȣ�����ʱ����ֹ48Сʱ��������ݳ��
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

// ����Ϊ��ʻ��¼�����ݴ洢��

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

#define     FF_GPS_MESSAGE_START_PAGE	 		(FF_NORMAL_MSG_END_PAGE+1)		// GPS��ʷ��Ϣ��¼��ʼҳ��ַ
#define     FF_GPS_MESSAGE_END_PAGE		 		(FF_GPS_MESSAGE_START_PAGE+FF_GPS_MESSAGE_TOTAL_PAGE-1)	// GPS��ʷ��Ϣ��¼��ֹҳ��ַ()

#define     FF_GPS_BLIND_MESSAGE_START_PAGE	(FF_GPS_MESSAGE_END_PAGE+1)	// GPS��Ϣä��������ʼҳ��ַ
#define     FF_GPS_BLIND_MESSAGE_END_PAGE		(FF_GPS_BLIND_MESSAGE_START_PAGE+FF_GPS_BLIND_MESSAGE_TOTAL_PAGE-1)

#define		FF_GNSS_DATA_START_PAGE				(FF_GPS_BLIND_MESSAGE_END_PAGE+1)
#define		FF_GNSS_DATA_END_PAGE				(FF_GNSS_DATA_START_PAGE+FF_GNSS_DATA_TOTAL_PAGE-1)

#define		FF_PIC_START_PAGE						(FF_GNSS_DATA_END_PAGE+1)
#define		FF_PIC_END_PAGE						(FF_PIC_START_PAGE+FF_PIC_TOTAL_PAGE-1)
#define		FF_PIC_BLOCK_ADDR_BASE  			(FF_PIC_PAGE_ADDR_BASE/FF_PAGE_PER_BLOCK)    // ͼƬ��ַ���ַ�� ����8M��flash

#define 	 	FF_COMMON_START_PAGE   				(FF_PIC_END_PAGE+1)
#define		FF_COMMON_END_PAGE					(FF_COMMON_START_PAGE+FF_COMMON_TOTAL_PAGE-1)

//////////// �ֿ�洢��ҳ��ַ���̶�ʹ�����256KB
#define		FF_FONT_POINT_START_PAGE			31744UL





#define			TOTAL_PHOTO_NUM				   64 //16        //64
#define			MAX_PIC_PACKET						128            

#define			PIC_BLOCK_ADDR_BASE         	(FF_PIC_START_PAGE/FF_PAGE_PER_BLOCK+1)      // ͼƬ��ַ���ַ�� ����8M��flash




#if(	(FF_COMMON_END_PAGE+1) > FF_FONT_POINT_START_PAGE ) 
#error "Flash��ַ������Χ���������С������"
#endif





#endif


/************************** End of File *************************************************/

