/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												 
$										 
$                       	 		  
$								  		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      �ļ�����
	�ļ���: 	 DownUploadCmd.h
	�������������غ��ϴ������б���
	���뻷����
	Ŀ��CPU: 
	���ߣ�
	����ʱ�䣺
	����༭������UltraEdit 11.00a+  �Ʊ�����Ϊ�����ַ�������ɶ��Ը��á�
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
	�ϴ��������0x8afffe00~0x8affffff
	����ʱ��UCMD_��ͷ
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
























