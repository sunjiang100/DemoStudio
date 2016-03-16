#ifndef _KIONIX_KXTF9_COMMON_H_
#define _KIONIX_KXTF9_COMMON_H_


typedef unsigned char INT8U;
typedef signed char INT8S;
typedef unsigned char BOOLEAN;




#ifdef	__cplusplus
	extern	"C"	{
#endif
													 
extern BOOLEAN I2C_read_for_accel      (INT8U SAddr, INT8U RAddr, INT8U Len, INT8U *data);

#ifdef	__cplusplus
}
#endif


#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN I2C_write_for_accel      (INT8U SAddr, INT8U RAddr, INT8U Len, INT8U *data);

#ifdef	__cplusplus
}
#endif


#ifdef	__cplusplus
	extern	"C"	{
#endif

void accel_default_para(void);

#ifdef	__cplusplus
}
#endif


#ifdef	__cplusplus
	extern	"C"	{
#endif

void Check_Collision(void);

#ifdef	__cplusplus
}
#endif

#ifdef	__cplusplus
	extern	"C"	{
#endif

void Accelerometer_init(INT8U	first_use);

#ifdef	__cplusplus
}
#endif


			 



#define	LEN_ACCEL_PARA		28






#endif //_KIONIX_KXTF9_COMMON_H_

