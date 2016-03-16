		  
#ifndef      __VARIABLE_TYPE_H__
#define      __VARIABLE_TYPE_H__

#include		<limits.h>


typedef 		unsigned short		 	WCHAR;			  // ���ַ���

#if(UCHAR_MAX==0xff)
typedef 		unsigned char		 	uchar;			  // ucharΪ8λ
#elif(UCHAR_MAX==0xffff)
typedef 		unsigned short		 	uchar;			  // ucharΪ16λ
#elif(UCHAR_MAX==0xffffffff)
typedef 		unsigned long		 	uchar;			  // ucharΪ32λ
#else
typedef 		unsigned char		 	uchar;			  // ucharΪ32λ
#endif


#if(UCHAR_MAX==0xff)
typedef 		unsigned char		 	uint8;			  // �޷���8λ���ͱ���
typedef 		signed   char	 		int8;				  // �з���8λ���ͱ���
typedef 		unsigned char	 		UINT8;			  // �޷���8λ���ͱ���
typedef 		signed   char	 		INT8;            // �з���8λ���ͱ���
//typedef 		signed   char		 	BOOL;			  	  // �޷���8λ���ͱ���
#elif(USHRT_MAX==0xff)
typedef 		unsigned short 		uint8;           // �޷���8λ���ͱ���
typedef 		signed   short 		int8;            // �з���8λ���ͱ���
typedef 		unsigned short 		UINT8;           // �޷���8λ���ͱ���
typedef 		signed   short 		INT8;            // �з���8λ���ͱ���
typedef 		signed   short 		BOOL;            // �з���8λ���ͱ���
#elif(UINT_MAX==0xff)
typedef 		unsigned int 			uint8;           // �޷���8λ���ͱ���       
typedef 		signed   int 			int8;            // �з���8λ���ͱ��� 
typedef 		unsigned int 			UINT8;           // �޷���8λ���ͱ���       
typedef 		signed   int 			INT8;            // �з���8λ���ͱ���
typedef 		signed   int	 		BOOL;            // �з���8λ���ͱ���
#elif(ULONG_MAX==0xff)
typedef 		unsigned long 			uint8;           // �޷���8λ���ͱ���       
typedef 		signed   long 			int8;            // �з���8λ���ͱ��� 
typedef 		unsigned long 			UINT8;           // �޷���8λ���ͱ���       
typedef 		signed   long 			INT8;            // �з���8λ���ͱ���
typedef 		signed   long	 		BOOL;            // �з���8λ���ͱ���
#elif(ULLONG_MAX==0xff)
typedef 		unsigned long long	 uint8;          // �޷���8λ���ͱ���       
typedef 		signed   long long	 int8;           // �з���8λ���ͱ��� 
typedef 		unsigned long long	 UINT8;          // �޷���8λ���ͱ���       
typedef 		signed   long long	 INT8;           // �з���8λ���ͱ���
typedef 		signed   long long  	 BOOL;           // �з���8λ���ͱ���
#else
#error  "��֧��8λ(���ֽ�)����������"
#endif

#if(UCHAR_MAX==0xffff)
typedef 		unsigned char		 	uint16;				// �޷���16λ���ͱ���
typedef 		signed   char	 		int16;				// �з���16λ���ͱ���
typedef 		unsigned char	 		UINT16;				// �޷���16λ���ͱ���
typedef 		signed   char	 		INT16;            // �з���16λ���ͱ���
#elif(USHRT_MAX==0xffff)
typedef 		unsigned short 		uint16;           // �޷���16λ���ͱ���
typedef 		signed   short 		int16;            // �з���16λ���ͱ���
typedef 		unsigned short 		UINT16;           // �޷���16λ���ͱ���
typedef 		signed   short 		INT16;            // �з���16λ���ͱ���
#elif(UINT_MAX==0xffff)
typedef 		unsigned int 			uint16;           // �޷���16λ���ͱ���       
typedef 		signed   int 			int16;            // �з���16λ���ͱ��� 
typedef 		unsigned int 			UINT16;           // �޷���16λ���ͱ���       
typedef 		signed   int 			INT16;            // �з���16λ���ͱ���
#elif(ULONG_MAX==0xffff)
typedef 		unsigned long 			uint16;           // �޷���16λ���ͱ���       
typedef 		signed   long 			int16;            // �з���16λ���ͱ��� 
typedef 		unsigned long 			UINT16;           // �޷���16λ���ͱ���       
typedef 		signed   long 			INT16;            // �з���16λ���ͱ���
#elif(ULLONG_MAX==0xffff)
typedef 		unsigned long long	 uint16;          // �޷���16λ���ͱ���       
typedef 		signed   long long	 int16;           // �з���16λ���ͱ��� 
typedef 		unsigned long long	 UINT16;          // �޷���16λ���ͱ���       
typedef 		signed   long long	 INT16;           // �з���16λ���ͱ���
#else
#error  "��֧��16λ(˫�ֽ�)����������"
#endif

#if(UCHAR_MAX==0xffffffff)
typedef 		unsigned char		 	uint32;				// �޷���32λ���ͱ���
typedef 		signed   char	 		int32;				// �з���32λ���ͱ���
typedef 		unsigned char	 		UINT32;				// �޷���32λ���ͱ���
typedef 		signed   char	 		INT32;            // �з���32λ���ͱ���
#elif(USHRT_MAX==0xffffffff)
typedef 		unsigned short 		uint32;           // �޷���32λ���ͱ���
typedef 		signed   short 		int32;            // �з���32λ���ͱ���
typedef 		unsigned short 		UINT32;           // �޷���32λ���ͱ���
typedef 		signed   short 		INT32;            // �з���32λ���ͱ���
#elif(UINT_MAX==0xffffffff)
typedef 		unsigned int 			uint32;           // �޷���32λ���ͱ���       
typedef 		signed   int 			int32;            // �з���32λ���ͱ��� 
typedef 		unsigned int 			UINT32;           // �޷���32λ���ͱ���       
typedef 		signed   int 			INT32;            // �з���32λ���ͱ���
#elif(ULONG_MAX==0xffffffff)
typedef 		unsigned long 			uint32;           // �޷���32λ���ͱ���       
typedef 		signed   long 			int32;            // �з���32λ���ͱ��� 
typedef 		unsigned long 			UINT32;           // �޷���32λ���ͱ���       
typedef 		signed   long 			INT32;            // �з���32λ���ͱ���
#elif(ULLONG_MAX==0xffffffff)
typedef 		unsigned long long	 uint32;          // �޷���32λ���ͱ���       
typedef 		signed   long long	 int32;           // �з���32λ���ͱ��� 
typedef 		unsigned long long	 UINT32;          // �޷���32λ���ͱ���       
typedef 		signed   long long	 INT32;           // �з���32λ���ͱ���
#else
#error  "��֧��32λ(4�ֽ�)����������"
#endif

#if(UCHAR_MAX==0xffffffffffffffff)
typedef 		unsigned char		 	uint64;				// �޷���64λ���ͱ���
typedef 		signed   char	 		int64;				// �з���64λ���ͱ���
typedef 		unsigned char	 		UINT64;				// �޷���64λ���ͱ���
typedef 		signed   char	 		INT64;            // �з���64λ���ͱ���
#elif(USHRT_MAX==0xffffffffffffffff)
typedef 		unsigned short 		uint64;           // �޷���64λ���ͱ���
typedef 		signed   short 		int64;            // �з���64λ���ͱ���
typedef 		unsigned short 		UINT64;           // �޷���64λ���ͱ���
typedef 		signed   short 		INT64;            // �з���64λ���ͱ���
#elif(UINT_MAX==0xffffffffffffffff)
typedef 		unsigned int 			uint64;           // �޷���64λ���ͱ���       
typedef 		signed   int 			int64;            // �з���64λ���ͱ��� 
typedef 		unsigned int 			UINT64;           // �޷���64λ���ͱ���       
typedef 		signed   int 			INT64;            // �з���64λ���ͱ���
#elif(ULONG_MAX==0xffffffffffffffff)
typedef 		unsigned long 			uint64;           // �޷���64λ���ͱ���       
typedef 		signed   long 			int64;            // �з���64λ���ͱ��� 
typedef 		unsigned long 			UINT64;           // �޷���64λ���ͱ���       
typedef 		signed   long 			INT64;            // �з���64λ���ͱ���
#elif(ULLONG_MAX==0xffffffffffffffff)
typedef 		unsigned long long	 uint64;          // �޷���64λ���ͱ���       
typedef 		signed   long long	 int64;           // �з���64λ���ͱ��� 
typedef 		unsigned long long	 UINT64;          // �޷���64λ���ͱ���       
typedef 		signed   long long	 INT64;           // �з���64λ���ͱ���
#else
#error  "��֧��64λ(8�ֽ�)����������"
#endif

#endif


/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */

