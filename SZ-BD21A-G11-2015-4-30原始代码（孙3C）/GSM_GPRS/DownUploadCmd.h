/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 		  
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 	 DownUploadCmd.h
	功能描述：下载和上传命令列表定义
	编译环境：
	目标CPU: 
	作者：
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/					  

#ifndef		__DOWNUPLOADCMD_H__
#define		__DOWNUPLOADCMD_H__


#ifdef      CMD_GLOBALS
#define     CMD_EXT
#else
#define     CMD_EXT    extern
#endif



#define			DOWNLOAD_CMD_NUM			10

#define			UPLOAD_CMD_NUM				30








/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	上传命令表，从0x8afffe00~0x8affffff
	定义时用UCMD_开头
*/
CMD_EXT		enum
{
	UCMD_REQUESET_SERVER_LICENSE = 0x8afffe00,

	UCMD_GERNERAL_GPRS_DATA_PACK = 0x8afffe10,


} UPLOAD_CMD;



#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
























