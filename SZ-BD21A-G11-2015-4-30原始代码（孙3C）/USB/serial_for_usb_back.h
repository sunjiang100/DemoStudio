

#ifndef __SERIAL_FOR_USB_H
#define __SERIAL_FOR_USB_H

#ifdef	__cplusplus
	extern	"C"	{
#endif

extern void SER_init      (int uart);

#ifdef	__cplusplus
}
#endif

#ifdef	__cplusplus
	extern	"C"	{
#endif

extern int  SER_getString   (unsigned char *str);

#ifdef	__cplusplus
}
#endif

#ifdef	__cplusplus
	extern	"C"	{
#endif

extern int  SER_putChar   (int uart, int c);

#ifdef	__cplusplus
}
#endif

#ifdef	__cplusplus
	extern	"C"	{
#endif

extern void SER_putString (char *s);

#ifdef	__cplusplus
}
#endif


#ifdef	__cplusplus
	extern	"C"	{
#endif

extern void SER_putb (unsigned short Len,unsigned char *p);

#ifdef	__cplusplus
}
#endif


#endif    // end 	__SERIAL_FOR_USB_H
