/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 		  
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
	�ļ���: 	 DownloadCmd.h
	���������������������ԭ���������б�
	���뻷����
	Ŀ��CPU: 
	���ߣ�
	����ʱ�䣺
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
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
// <h>�����ϴ������������


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>TH_DOWNLOAD_CMD_TOTAL_NUM: ���ص�ʵ����������Ϊ <1-512>
//   	<i> Default: 20
*/
#define		DOWNLOAD_CMD_TOTAL_NUM					20




//#if(DOWNLOAD_CMD_TOTAL_NUM>DOWN_UP_MAX_CMD_NUMBER)
//#error	"�����ϴ������������������Ϊ512����"
//#endif			


/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//   	<o>DOWNLOAD_FRAME_HEAD_TOTAL_NUM: ���ص�ʵ��֡ͷ��Ϊ <1-10>
//   	<i> Default: 4
*/
#define		DOWNLOAD_FRAME_HEAD_TOTAL_NUM				5



// </h>

// $$$$$$$$$$$$$$$$$$$ <<< end of configuration section >>> $$$$$$$$$$$$$$$$$$$$$$$$$$$$











#endif

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */

