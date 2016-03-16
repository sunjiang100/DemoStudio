/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 		  
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 	 DownloadCmd.h
	功能描述：下载命令处理函数原形声明及列表
	编译环境：
	目标CPU: 
	作者：
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  

#ifndef		__CMD_PROTOTYPE_H__
#define		__CMD_PROTOTYPE_H__


#ifdef      CMD_GLOBALS
#define     CMD_EXT
#else
#define     CMD_EXT    extern
#endif

#include    "..\Config\UserConfig.h"

#include		"..\UART\UART1.h"

#include		"GSM_GPRS.h"





// $$$$$$$$$$$$$$$$$$ <<< Use Configuration Wizard in Context Menu >>> $$$$$$$$$$$$$$$$$
// <h>下载上传命令相关配置


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>TH_DOWNLOAD_CMD_TOTAL_NUM: 下载的实际命令条数为 <1-512>
//   	<i> Default: 20
*/
#define		DOWNLOAD_CMD_TOTAL_NUM					20




//#if(DOWNLOAD_CMD_TOTAL_NUM>DOWN_UP_MAX_CMD_NUMBER)
//#error	"下载上传的最多命令条数限制为512条！"
//#endif			


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>DOWNLOAD_FRAME_HEAD_TOTAL_NUM: 下载的实际帧头数为 <1-10>
//   	<i> Default: 4
*/
#define		DOWNLOAD_FRAME_HEAD_TOTAL_NUM				5



// </h>

// $$$$$$$$$$$$$$$$$$$ <<< end of configuration section >>> $$$$$$$$$$$$$$$$$$$$$$$$$$$$











#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */

