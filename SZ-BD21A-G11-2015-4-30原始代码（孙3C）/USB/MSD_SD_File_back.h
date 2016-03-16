/*----------------------------------------------------------------------------
 *      RL-ARM - FlashFS
 *----------------------------------------------------------------------------
 *      Name:    MSD_SD_FILE.H
 *      Purpose: File manipulation example definitions
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

enum {BACKSPACE = 0x08, 
      LF        = 0x0A, 
      CR        = 0x0D, 
      CNTLQ     = 0x11, 
      CNTLS     = 0x13, 
      ESC       = 0x1B, 
      DEL       = 0x7F
};

/* Command definitions structure. */
typedef struct scmd {
  char val[8];
  void (*func)(char *par);
} SCMD;

/* External functions */
//#include "serial.h"
#include 		"serial_for_usb.h"


#ifdef 	__cplusplus
	extern 	"C"	{
#endif

extern BOOL getline (char *, U32);

#ifdef 	__cplusplus
}
#endif

#ifdef 	__cplusplus
	extern 	"C"	{
#endif

extern void init_serial (void);	

#ifdef 	__cplusplus
}
#endif

#ifdef 	__cplusplus
	extern 	"C"	{
#endif

//extern int  getkey (void);

#ifdef 	__cplusplus
}
#endif

#ifdef 	__cplusplus
	extern 	"C"	{
#endif

extern __task	void	TaskUSB(void);

#ifdef 	__cplusplus
}
#endif

/*
#ifdef 	__cplusplus
	extern 	"C"	{
#endif

extern void gdata_to_file (char *fname, unsigned char *buffer, unsigned int glen);
 
#ifdef 	__cplusplus
}
#endif
*/
/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
