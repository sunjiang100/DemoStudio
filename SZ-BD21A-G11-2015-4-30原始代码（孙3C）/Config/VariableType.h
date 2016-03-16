		  
#ifndef      __VARIABLE_TYPE_H__
#define      __VARIABLE_TYPE_H__

#include		<limits.h>


typedef 		unsigned short		 	WCHAR;			  // 宽字符型

#if(UCHAR_MAX==0xff)
typedef 		unsigned char		 	uchar;			  // uchar为8位
#elif(UCHAR_MAX==0xffff)
typedef 		unsigned short		 	uchar;			  // uchar为16位
#elif(UCHAR_MAX==0xffffffff)
typedef 		unsigned long		 	uchar;			  // uchar为32位
#else
typedef 		unsigned char		 	uchar;			  // uchar为32位
#endif


#if(UCHAR_MAX==0xff)
typedef 		unsigned char		 	uint8;			  // 无符号8位整型变量
typedef 		signed   char	 		int8;				  // 有符号8位整型变量
typedef 		unsigned char	 		UINT8;			  // 无符号8位整型变量
typedef 		signed   char	 		INT8;            // 有符号8位整型变量
//typedef 		signed   char		 	BOOL;			  	  // 无符号8位整型变量
#elif(USHRT_MAX==0xff)
typedef 		unsigned short 		uint8;           // 无符号8位整型变量
typedef 		signed   short 		int8;            // 有符号8位整型变量
typedef 		unsigned short 		UINT8;           // 无符号8位整型变量
typedef 		signed   short 		INT8;            // 有符号8位整型变量
typedef 		signed   short 		BOOL;            // 有符号8位整型变量
#elif(UINT_MAX==0xff)
typedef 		unsigned int 			uint8;           // 无符号8位整型变量       
typedef 		signed   int 			int8;            // 有符号8位整型变量 
typedef 		unsigned int 			UINT8;           // 无符号8位整型变量       
typedef 		signed   int 			INT8;            // 有符号8位整型变量
typedef 		signed   int	 		BOOL;            // 有符号8位整型变量
#elif(ULONG_MAX==0xff)
typedef 		unsigned long 			uint8;           // 无符号8位整型变量       
typedef 		signed   long 			int8;            // 有符号8位整型变量 
typedef 		unsigned long 			UINT8;           // 无符号8位整型变量       
typedef 		signed   long 			INT8;            // 有符号8位整型变量
typedef 		signed   long	 		BOOL;            // 有符号8位整型变量
#elif(ULLONG_MAX==0xff)
typedef 		unsigned long long	 uint8;          // 无符号8位整型变量       
typedef 		signed   long long	 int8;           // 有符号8位整型变量 
typedef 		unsigned long long	 UINT8;          // 无符号8位整型变量       
typedef 		signed   long long	 INT8;           // 有符号8位整型变量
typedef 		signed   long long  	 BOOL;           // 有符号8位整型变量
#else
#error  "不支持8位(单字节)变量！！！"
#endif

#if(UCHAR_MAX==0xffff)
typedef 		unsigned char		 	uint16;				// 无符号16位整型变量
typedef 		signed   char	 		int16;				// 有符号16位整型变量
typedef 		unsigned char	 		UINT16;				// 无符号16位整型变量
typedef 		signed   char	 		INT16;            // 有符号16位整型变量
#elif(USHRT_MAX==0xffff)
typedef 		unsigned short 		uint16;           // 无符号16位整型变量
typedef 		signed   short 		int16;            // 有符号16位整型变量
typedef 		unsigned short 		UINT16;           // 无符号16位整型变量
typedef 		signed   short 		INT16;            // 有符号16位整型变量
#elif(UINT_MAX==0xffff)
typedef 		unsigned int 			uint16;           // 无符号16位整型变量       
typedef 		signed   int 			int16;            // 有符号16位整型变量 
typedef 		unsigned int 			UINT16;           // 无符号16位整型变量       
typedef 		signed   int 			INT16;            // 有符号16位整型变量
#elif(ULONG_MAX==0xffff)
typedef 		unsigned long 			uint16;           // 无符号16位整型变量       
typedef 		signed   long 			int16;            // 有符号16位整型变量 
typedef 		unsigned long 			UINT16;           // 无符号16位整型变量       
typedef 		signed   long 			INT16;            // 有符号16位整型变量
#elif(ULLONG_MAX==0xffff)
typedef 		unsigned long long	 uint16;          // 无符号16位整型变量       
typedef 		signed   long long	 int16;           // 有符号16位整型变量 
typedef 		unsigned long long	 UINT16;          // 无符号16位整型变量       
typedef 		signed   long long	 INT16;           // 有符号16位整型变量
#else
#error  "不支持16位(双字节)变量！！！"
#endif

#if(UCHAR_MAX==0xffffffff)
typedef 		unsigned char		 	uint32;				// 无符号32位整型变量
typedef 		signed   char	 		int32;				// 有符号32位整型变量
typedef 		unsigned char	 		UINT32;				// 无符号32位整型变量
typedef 		signed   char	 		INT32;            // 有符号32位整型变量
#elif(USHRT_MAX==0xffffffff)
typedef 		unsigned short 		uint32;           // 无符号32位整型变量
typedef 		signed   short 		int32;            // 有符号32位整型变量
typedef 		unsigned short 		UINT32;           // 无符号32位整型变量
typedef 		signed   short 		INT32;            // 有符号32位整型变量
#elif(UINT_MAX==0xffffffff)
typedef 		unsigned int 			uint32;           // 无符号32位整型变量       
typedef 		signed   int 			int32;            // 有符号32位整型变量 
typedef 		unsigned int 			UINT32;           // 无符号32位整型变量       
typedef 		signed   int 			INT32;            // 有符号32位整型变量
#elif(ULONG_MAX==0xffffffff)
typedef 		unsigned long 			uint32;           // 无符号32位整型变量       
typedef 		signed   long 			int32;            // 有符号32位整型变量 
typedef 		unsigned long 			UINT32;           // 无符号32位整型变量       
typedef 		signed   long 			INT32;            // 有符号32位整型变量
#elif(ULLONG_MAX==0xffffffff)
typedef 		unsigned long long	 uint32;          // 无符号32位整型变量       
typedef 		signed   long long	 int32;           // 有符号32位整型变量 
typedef 		unsigned long long	 UINT32;          // 无符号32位整型变量       
typedef 		signed   long long	 INT32;           // 有符号32位整型变量
#else
#error  "不支持32位(4字节)变量！！！"
#endif

#if(UCHAR_MAX==0xffffffffffffffff)
typedef 		unsigned char		 	uint64;				// 无符号64位整型变量
typedef 		signed   char	 		int64;				// 有符号64位整型变量
typedef 		unsigned char	 		UINT64;				// 无符号64位整型变量
typedef 		signed   char	 		INT64;            // 有符号64位整型变量
#elif(USHRT_MAX==0xffffffffffffffff)
typedef 		unsigned short 		uint64;           // 无符号64位整型变量
typedef 		signed   short 		int64;            // 有符号64位整型变量
typedef 		unsigned short 		UINT64;           // 无符号64位整型变量
typedef 		signed   short 		INT64;            // 有符号64位整型变量
#elif(UINT_MAX==0xffffffffffffffff)
typedef 		unsigned int 			uint64;           // 无符号64位整型变量       
typedef 		signed   int 			int64;            // 有符号64位整型变量 
typedef 		unsigned int 			UINT64;           // 无符号64位整型变量       
typedef 		signed   int 			INT64;            // 有符号64位整型变量
#elif(ULONG_MAX==0xffffffffffffffff)
typedef 		unsigned long 			uint64;           // 无符号64位整型变量       
typedef 		signed   long 			int64;            // 有符号64位整型变量 
typedef 		unsigned long 			UINT64;           // 无符号64位整型变量       
typedef 		signed   long 			INT64;            // 有符号64位整型变量
#elif(ULLONG_MAX==0xffffffffffffffff)
typedef 		unsigned long long	 uint64;          // 无符号64位整型变量       
typedef 		signed   long long	 int64;           // 有符号64位整型变量 
typedef 		unsigned long long	 UINT64;          // 无符号64位整型变量       
typedef 		signed   long long	 INT64;           // 有符号64位整型变量
#else
#error  "不支持64位(8字节)变量！！！"
#endif

#endif


/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */

