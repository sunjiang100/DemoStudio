/*----------------------------------------------------------------------------
 *      RL-ARM - FlashFS
 *----------------------------------------------------------------------------
 *      Name:    MSD_SD_FILE.C
 *      Purpose: File manipulation example program
 *----------------------------------------------------------------------------
 *      This code is part of the RealView Run-Time Library.
 *      Copyright (c) 2004-2011 KEIL - An ARM Company. All rights reserved.
 *---------------------------------------------------------------------------*/

#include <RTL.h>                      /* RTL kernel functions & defines      */
#include <rl_usb.h>                   /* RL-USB function prototypes          */
#include <stdio.h>                    /* standard I/O .h-file                */
#include <ctype.h>                    /* character functions                 */
#include <string.h>                   /* string and memory functions         */
#include "File_Config.h"
#include "MSD_SD_File.h"
#include "..\Common\UserGlobals.h"
#include <LPC17xx.H>
#include	".\Config\UserConfig.h"
//#include "..\Rtc\Rtc.h"
//#include		"..\UART\UART0.h"
#define     USB_DEBUG				 1
#define    		 NOP()      __nop()

extern BOOL usbh_ohci_hw_init (void);
extern BOOL usbh_ohci_hw_uninit (void);

//extern unsigned char USBUseFlag;
//extern unsigned char LenDataForUSB;
//extern unsigned char TxdTBuffer[260+16];

//extern unsigned char write_file_error_flag;
											
/* Command Functions */
static void cmd_capture (char *par);
static void cmd_type (char *par);
static void cmd_rename (char *par);
//static void cmd_copy (char *par);
static void cmd_delete (char *par);
static void cmd_dir (char *par);
static void cmd_format (char *par);
//static void cmd_help (char *par);
static void cmd_fill (char *par);


#if 	0			 // 不需要帮助信息
/* Local constants */
static const char intro[] =
  "\n\n\n\n\n\n\n\n"
  "+-----------------------------------------------------------------------+\n"
  "|          Mass Storage Device (MSD) File Manipulation example          |\n";
static const char help[] = 
  "+ command ------------------+ function ---------------------------------+\n"
  "| CAP \"fname\" [/A]          | captures serial data to a file            |\n"
  "|                           |  [/A option appends data to a file]       |\n"
  "| FILL \"fname\" [nnnn]       | create a file filled with text            |\n"
  "|                           |  [nnnn - number of lines, default=1000]   |\n"
  "| TYPE \"fname\"              | displays the content of a text file       |\n"
  "| REN \"fname1\" \"fname2\"     | renames a file 'fname1' to 'fname2'       |\n"
  "|                           |  ['fin2' option merges 'fin' and 'fin2']  |\n"
  "| DEL \"fname\"               | deletes a file                            |\n"
  "| DIR [\"mask\"]              | displays a list of files in the directory |\n"
  "| FORMAT [label [/FAT32]]   | formats Mass Storage Device               |\n"
  "|                           | [/FAT32 option selects FAT32 file system] |\n"
  "| HELP  or  ?               | displays this help                        |\n"
  "+---------------------------+-------------------------------------------+\n";

#endif		  

static const SCMD cmd[] = {
  "CAP",    cmd_capture,
  "TYPE",   cmd_type,
  "REN",    cmd_rename,
  "DEL",    cmd_delete,
  "DIR",    cmd_dir,
  "FORMAT", cmd_format,
  "FILL",   cmd_fill,
};

#define CMD_COUNT   (sizeof (cmd) / sizeof (cmd[0]))
#define IN_LINE_SIZE		120
#define PRINT_TMP_SIZE		90


/* Local variables */
static char in_line[IN_LINE_SIZE];		// 用于命令行
static char	printmp[PRINT_TMP_SIZE];     // 用于格式化输出
static char	FileName[32];


/* Local Function Prototypes */
static void dot_format (U64 val, char *sp);
static char *get_entry (char *cp, char **pNext);
static unsigned char init_msd (void);
//static void memset_x(char *adr, unsigned int len, char ch);
static char	get_line_usb(char *in_line);
void     USB_GetGBdata(void);
void		SetBufferValue(uchar *Buffer,uint16 Len,uchar Data);
//void  WrintDataToFile (unsigned short Len, unsigned char  *p);

//void SER_putb (unsigned long Len,unsigned char *p);

/*----------------------------------------------------------------------------
 *        Process input string for long or short name entry		
 *---------------------------------------------------------------------------*/
static char *get_entry (char *cp, char **pNext) 
{
  char *sp, lfn = 0, sep_ch = ' ';

  if (cp == NULL) 		 /* skip NULL pointers          */
  {                           
    *pNext = cp;
    return (cp);
  }

  for ( ; *cp == ' ' || *cp == '\"'; cp++) 	/* skip blanks and starting  " */
  {  
    if (*cp == '\"') { sep_ch = '\"'; lfn = 1; }
    *cp = 0;
  }
 
  for (sp = cp; *sp != CR && *sp != LF && *sp != '\0'; sp++) 	  // 加上检查尾0代码	  *sp !='\0'
  {
    if ( lfn && *sp == '\"') break;
    if (!lfn && *sp == ' ' ) break;
  }

  for ( ; *sp == sep_ch || *sp == CR || *sp == LF; sp++) 
  {
    *sp = 0;
    if ( lfn && *sp == sep_ch) { sp ++; break; }
  }

  *pNext = (*sp) ? sp : NULL;                 /* next entry                  */
  return (cp);
}

/*----------------------------------------------------------------------------
 *        Print size in dotted fomat
 *---------------------------------------------------------------------------*/
static void dot_format (U64 val, char *sp) 
{

  if (val >= (U64)1e12) 
  {
    sp += sprintf (sp,"%d.",(U32)(val/(U64)1e12));
    val %= (U64)1e12;
    sp += sprintf (sp,"%03d.",(U32)(val/(U64)1e9));
    val %= (U64)1e9;
    sp += sprintf (sp,"%03d.",(U32)(val/(U64)1e6));
    val %= (U64)1e6;
    sprintf (sp,"%03d.%03d",(U32)(val/1000),(U32)(val%1000));
    return;
  }
  if (val >= (U64)1e9) 
  {
    sp += sprintf (sp,"%d.",(U32)(val/(U64)1e9));
    val %= (U64)1e9;
    sp += sprintf (sp,"%03d.",(U32)(val/(U64)1e6));
    val %= (U64)1e6;
    sprintf (sp,"%03d.%03d",(U32)(val/1000),(U32)(val%1000));
    return;
  }
  if (val >= (U64)1e6) 
  {
    sp += sprintf (sp,"%d.",(U32)(val/(U64)1e6));
    val %= (U64)1e6;
    sprintf (sp,"%03d.%03d",(U32)(val/1000),(U32)(val%1000));
    return;
  }
  if (val >= 1000) 
  {
    sprintf (sp,"%d.%03d",(U32)(val/1000),(U32)(val%1000));
    return;
  }
  sprintf (sp,"%d",(U32)(val));
}

/*----------------------------------------------------------------------------
 *        Capture serial data to file
 *---------------------------------------------------------------------------*/
static void cmd_capture (char *par) 
{
  char tmpstr[5] = { 0 },*fname,*next;
  BOOL append;
  FILE *f;
  //int count1 =0, count2 = 0,ret = 0;
  fname = get_entry (par, &next);
  if (fname == NULL) 
  {
    SER_putString("\nFilename missing.\n");
	 

    return;
  }
  append = __FALSE;
  if (next) 
  {
    par = get_entry (next, &next);
    if ((strcmp (par, "/A") == 0) ||(strcmp (par, "/a") == 0)) 
	 {
      append = __TRUE;
    }
    else 
	 {
      SER_putString("\nCommand error.\n");
		
      return;
    }
  }
  sprintf (printmp, (append) ? "\nAppend data to file %s" :
                     "\nCapture data to file %s", fname);
  SER_putString(printmp);
  SER_putString("\nPress ESC to stop.\n");


  f = fopen (fname,append ? "a" : "w"); /* open a file for writing           */
  if (f == NULL) 
  {
    SER_putString("\nCan not open file!\n");  /* error when trying to open file    */
	 
    return;
  }
  

  do 
  {
  	 SetBufferValue((uchar *)&in_line[0], IN_LINE_SIZE, 0); 
	
	 while (SER_getString ((unsigned char *)in_line) == __FALSE) 
	 {
		os_dly_wait(1);
    }
	 
	 memcpy(tmpstr,in_line, 3);  		// 取出命令行前三个字符
    /*for (cp = &tmpstr[0]; *cp && *cp != ' '; cp++) 
	 {	  
      *cp = toupper (*cp);                    // 字母转换在大写
    }*/
	 
	 if (!strcmp(tmpstr, "ESC"))	 // 结束输入标志
	 {
	 		
	 		break;
	 }

    fputs ((const char *)in_line, f);
	 os_dly_wait(20);			           // 写入文件需要延时
 

/***	 重传机制 by xzb ***
	 count1 = strlen(in_line);
//	   printf("count1 = %d\n", count1);
//	 while (count1 > 0)
//	 {
//		 ret = fwrite(in_line + count2, 1, count1, f);
//		 count1 -= ret;
//		 count2 += ret;
//	 }

 ***/

  } while (strlen(in_line) < IN_LINE_SIZE);	 // 串口输入数据不能超过IN_LINE_SIZE

  fclose (f);                         /* close the output file               */
  SER_putString("\nFile closed.\n");
  
}

/*----------------------------------------------------------------------------
 *        Create a file and fill it with some text
 *---------------------------------------------------------------------------*/
static void cmd_fill (char *par) 
{
  char *fname, *next;
  FILE *f;
  int i,cnt = 1000;

  SetBufferValue((uchar *)printmp, PRINT_TMP_SIZE, 0);

  fname = get_entry (par, &next);
  if (fname == NULL) 
  {
    SER_putString("\nFilename missing.\n");
	 
    return;
  }
  if (next) 
  {
    par = get_entry (next, &next);
    if (sscanf (par,"%d", &cnt) == 0) {
      SER_putString("\nCommand error.\n");
		

      return;
    }
  }

  f = fopen (fname, "w");               /* open a file for writing           */
  if (f == NULL) 
  {
    SER_putString("\nCan not open file!\n");  /* error when trying to open file    */
	 

    return;
  } 
  for (i = 0; i < cnt; i++)  
  {
    fprintf (f, "This is line # %d in file %s\n", i, fname);
  	
  }
  os_dly_wait(20);

  fclose (f);                         /* close the output file               */
  SER_putString("\nFile closed.\n");
  
}


/*----------------------------------------------------------------------------
 *        Create a file and fill it with data  add by xzb  for G11
 *
 *			函数名称：  gdata_to_file
 *			函数功能:   创建文件，把数据写到文件中
 *			入口参数：	fname  - 文件名
 *					 		buffer - 数据区
 *					 		glen   - 数据长度	
 *	      返回值：		无		
 *---------------------------------------------------------------------------*/
#if (1)
void gdata_to_file(char *fname)
{
	 uchar buf[32];
	//FILE *gfile;

  usbh_engine(0);
  #if(USB_DEBUG)
  SER_putString ("\nengineing...\r\n\0");
  os_dly_wait(2);
  #endif
  while (!usbh_msc_status(0,0)) 		  /* If device is not connected  */
  {    
    #if(USB_DEBUG)             
    SER_putString ("\nengine failed...\r\n\0");
	 os_dly_wait(2);
	 #endif
	 if (USB_CONNECT & 0x01)	   // 检测U盘是否插入	  
    {
		  os_dly_wait(1);	 
		  usbh_engine(0);
	 }
	 else 
	 {	  
	     Wfile_Flag = 0;
	 	  return;
	 }
  }
  #if(USB_DEBUG)
  SER_putString ("\nengine succesful...r\n\0");
  os_dly_wait(3);
  #endif
  SetBufferValue((uchar *)printmp, PRINT_TMP_SIZE, 0);

  if (fname == NULL) 
  {
     #if(USB_DEBUG)
	  SER_putString("\nFilename missing!\r\n\0");
	  #endif
    return;
  }
  if(gfile != NULL )
  {  
	  #if(USB_DEBUG)
	  SER_putString("\nCloseing file....\r\n\0"); 
	  os_dly_wait(2);
	  #endif 
	  if((fclose (gfile)==0))        
	  {
	    #if(USB_DEBUG)
		 SER_putString("\nClose file succesful!\r\n\0"); 
	    os_dly_wait(2);
		 #endif 
	  }
	  else
	  {
	  	 #if(USB_DEBUG)
		 SER_putString("\nClose file failed!\r\n\0"); 
	    os_dly_wait(2);
		 #endif 
	  }
  }

  
  #if(USB_DEBUG)
  SER_putString ("\nOpening new file...r\n\0");
  os_dly_wait(2);
  #endif
  gfile = fopen (fname, "wb+");               /* open a file for writing           */

  //test
/*	buf[0]=0x67; 
	buf[1]=0x2a;  
	buf[2]=0;
//	buf[2]=0x77;
//	buf[3]=0xe5; 
//	buf[4]=0x8f;
//	buf[5]=0x66;  
//	buf[6]=0x72;
//	buf[7]=0x4c;
	buf[8] = 0;

	SER_putString("file Test Data:"); SER_putb(strlen((char *)buf),buf); SER_putString("\r\n");

   gfile = fopen ((char *)buf, "wb+"); 
*/	

  //


  #if(USB_DEBUG)
  SER_putString ("\nOpen new file succesful...r\n\0");
  os_dly_wait(2);
  #endif
  if (gfile == NULL) 
  {
    #if(USB_DEBUG)
	 SER_putString("\nCan not open file!\r\n\0");  /* error when trying to open file    */
	 #endif
	 Write_File_Error = 1;
	 os_dly_wait(100);
    return;
  } 
	USBUseFlag = 1;
	os_dly_wait(1);

}


#else  // test
void gdata_to_file (char *fname) 
{
  //FILE *gfile;
  uchar ret;

  usbh_engine(0);
  #if(USB_DEBUG)
  SER_putString ("\nengineing...\r\n\0");
  os_dly_wait(2);
  #endif
  while (!usbh_msc_status(0,0)) 		  /* If device is not connected  */
  {    
    #if(USB_DEBUG)             
    SER_putString ("\nengine failed...\r\n\0");
	 os_dly_wait(2);
	 #endif
	 if (USB_CONNECT & 0x01)	   // 检测U盘是否插入	  
    {
		  os_dly_wait(1);	 
		  usbh_engine(0);
	 }
	 else 
	 {	  
	     Wfile_Flag = 0;
	 	  return;
	 }
  }
  #if(USB_DEBUG)
  SER_putString ("\nengine succesful...r\n\0");
  os_dly_wait(3);
  #endif
  SetBufferValue((uchar *)printmp, PRINT_TMP_SIZE, 0);

  if (fname == NULL) 
  {
     #if(USB_DEBUG)
	  SER_putString("\nFilename missing!\r\n\0");
	  #endif
    return;
  }
  if(gfile != NULL )
  {  
	  #if(USB_DEBUG)
	  SER_putString("\nCloseing file....\r\n\0"); 
	  os_dly_wait(2);
	  #endif 
	  if((fclose (gfile)==0))        
	  {
	    #if(USB_DEBUG)
		 SER_putString("\nClose file succesful!\r\n\0"); 
	    os_dly_wait(2);
		 #endif 
	  }
	  else
	  {
	  	 #if(USB_DEBUG)
		 SER_putString("\nClose file failed!\r\n\0"); 
	    os_dly_wait(2);
		 #endif 
	  }
  }

  
  #if(USB_DEBUG)
  SER_putString ("\nOpening new file...r\n\0");
  os_dly_wait(2);
  #endif
  gfile = fopen (fname, "w");               /* open a file for writing           */
  if (gfile==NULL)
  {
  	  	 #if(USB_DEBUG)
		 SER_putString("\nOpen new file failed!\r\n\0"); 
	    os_dly_wait(2);
		 #endif 	
		 return;
  }
  #if(USB_DEBUG)
  SER_putString ("\nOpen new file succesful...r\n\0");
  os_dly_wait(2);
  #endif
  if (gfile == NULL) 
  {
    #if(USB_DEBUG)
	 SER_putString("\nCan not open file!\r\n\0");  /* error when trying to open file    */
	 #endif
	 Write_File_Error = 1;
	 os_dly_wait(100);
    return;
  } 
	USBUseFlag = 1;
	os_dly_wait(1);

	// test 
	ret = fwrite("12345",5 ,1 , gfile);
	if (ret==0)
	{
		 //Write_File_Error = 1;
		 //wfile_flag = 0;
		 LenDataForUSB = 0;
		 #if(USB_DEBUG)
		 SER_putString ("\n USB write ERROR!!!\r\n\0");
		 os_dly_wait(2);
		 #endif  
		 //funinit ("U0:");
       //usbh_init(0);     
       return;
	}

  if(fclose (gfile)!=0)
  {
  	 #if(USB_DEBUG)
	 SER_putString ("\n File Close ERROR!!!\r\n\0");
	 //funinit ("U0:");
	 os_dly_wait(2); 
	 #endif
	 //finit ("U0:"); 
	 //wfile_flag = 0; 
  
  }
  else
  {
	  gfile = NULL;
	  #if(USB_DEBUG)
	  SER_putString ("\nAuto write file finished,File closed!!!\r\n\0");                      /* close the output file               */
	  os_dly_wait(1);
	  Wfile_Flag = 3; 
	  #endif
	  //funinit("U0:");
	  //SER_putString("\nFile closed.\r\n\0");
  } 

	//


#if  0
	ret = 0; 
	//USBUseFlag = 1;
	CheckSum = 0;
	//write_file_error_flag = 0;
  while (USBUseFlag == 1)
  {
	     if (LenDataForUSB)
		  {
		  	  
			  buffer = 	&TxdTBuffer[0];
			 // 加校验
			  for(t=0; t<LenDataForUSB; t++)	{ CheckSum ^= *(buffer+t);	}
		     if(USB_CONNECT & 0x01)
			     ret = fwrite(buffer,LenDataForUSB ,1 , gfile);	  //   写文件
			  else 
				  ret = 0;
				//fflush(gfile);
			  if (ferror(gfile)||(ret==0))								//(ferror(gfile)||
			  {
					 //设置出错标志
					 //fclose (gfile);
					 //usbh_uninit ();
					 //funinit ("U0:");
	             //os_dly_wait(50); 
	             //finit ("U0:");  
					 Write_File_Error = 1;
					 wfile_flag = 0;
					 LenDataForUSB = 0;
					 #if(USB_DEBUG)
					 SER_putString ("\n USB write ERROR!!!\r\n\0");
					 os_dly_wait(2);
					 #endif  
					 //funinit ("U0:");
			       //usbh_init(0);     
			       return;
				 }
			  if(TD.Display==tDISP_USB_RECODE)
			  {
			       SER_putb(LenDataForUSB, buffer);
					 os_dly_wait(LenDataForUSB/15);
			  }
			  LenDataForUSB = 0;
			}
			else
			  os_dly_wait(1);							 
	  }				  
	  // 写入检验数据
	  fwrite((char *)&CheckSum, 1, 1, gfile);
	  if(fclose (gfile)!=0)
	  {
	  	 #if(USB_DEBUG)
		 SER_putString ("\n File Close ERROR!!!\r\n\0");
		 //funinit ("U0:");
		 os_dly_wait(2); 
		 #endif
		 //finit ("U0:"); 
		 wfile_flag = 0; 
	  
	  }
	  else
	  {
		  gfile = NULL;
		  #if(USB_DEBUG)
		  SER_putString ("\nAuto write file finished!!!\r\n\0");                      /* close the output file               */
		  os_dly_wait(1);
		  #endif
		  //funinit("U0:");
		  //SER_putString("\nFile closed.\r\n\0");
	  }
#endif		
}
#endif

/*----------------------------------------------------------------------------
 *        Read file and dump it to serial window
 *---------------------------------------------------------------------------*/
static void cmd_type (char *par) 
{
  char *fname,*next;
  FILE *f;
  int ch = 0;

  SetBufferValue((uchar *)printmp,PRINT_TMP_SIZE, 0);

  fname = get_entry (par, &next);
  if (fname == NULL) 
  {
    SER_putString("\nFilename missing.\n");

    return;
  }
  sprintf(printmp, "\nRead data from file %s\n",fname);
  SER_putString(printmp);

  f = fopen (fname,"r");              /* open the file for reading           */
  if (f == NULL) 
  {
    SER_putString("\nFile not found!\n");
    return;
  }
  
  SetBufferValue((uchar *)in_line, IN_LINE_SIZE, 0);
  while ( fread (in_line, 1, 100 ,f) != 0) 	/* read the characters from the file   */
  {   
	 //SER_putChar(0, ch);                   /* and write them on the screen        */
	 SER_putb(100, (unsigned char *)in_line); os_dly_wait(5); 
	 if(ch++>0)
	 {  
	  ///os_dly_wait(5);
	  ch = 0;
	 }

  }
  os_dly_wait(5);

  fclose (f);                         /* close the input file when done      */

  SER_putString("\nFile closed.\n");
  
}

/*----------------------------------------------------------------------------
 *        Rename a File
 *---------------------------------------------------------------------------*/
static void cmd_rename (char *par) 
{
  char *fname,*fnew,*next,dir;

  SetBufferValue((uchar *)printmp, PRINT_TMP_SIZE, 0);

  fname = get_entry (par, &next);
  if (fname == NULL) 
  {
    SER_putString("\nFilename missing.\n");

    return;
  }
  fnew = get_entry (next, &next);
  if (fnew == NULL) 
  {
    SER_putString("\nNew Filename missing.\n");

    return;
  }
  if (strcmp (fname,fnew) == 0) 
  {
    SER_putString("\nNew name is the same.\n");

    return;
  }

  dir = 0;
  if (*(fname + strlen(fname) - 1) == '\\') 
  {
    dir = 1;
  }

  if (frename (fname, fnew) == 0) 
  {
    if (dir) 
	 {
      sprintf (printmp, "\nDirectory %s renamed to %s\n",fname,fnew);
		SER_putString(printmp);
    }
    else 
	 {
      sprintf (printmp, "\nFile %s renamed to %s\n",fname,fnew);
		SER_putString(printmp);
    }
  }
  else 
  {
    if (dir) 
	 {
      SER_putString("\nDirectory rename error.\n");
    }
    else 
	 {
      SER_putString("\nFile rename error.\n");
    }
  }
}

 #if  0			   // 不需要copy函数
/*----------------------------------------------------------------------------
 *        Copy a File
 *---------------------------------------------------------------------------*/

static void cmd_copy (char *par) 
{
  char *fname,*fnew,*fmer,*next;
  FILE *fin,*fout;
  U32 cnt,total;
  char buf[512];
  BOOL merge;

  SetBufferValue((uchar *)printmp, PRINT_TMP_SIZE, 0);

  fname = get_entry (par, &next);
  if (fname == NULL) 
  {
    SER_putString("\nFilename missing.\n");

    return;
  }
  fmer = get_entry (next, &next);
  if (fmer == NULL) 
  {
    SER_putString("\nNew Filename missing.\n");

    return;
  }
  fnew = get_entry (next, &next);
  if (fnew != NULL) 
  {
    merge = __TRUE;
  }
  else 
  {
    merge = __FALSE;
    fnew = fmer;
  }
  if ((strcmp (fname,fnew) == 0)        ||
      (merge && strcmp (fmer,fnew) == 0)) 
  {
    SER_putString("\nNew name is the same.\n");

    return;
  }

  fin = fopen (fname,"r");            /* open the file for reading           */
  if (fin == NULL) 
  {
    sprintf(printmp, "\nFile %s not found!\n",fname);
	 SER_putString(printmp);

    return;
  }

  if (merge == __FALSE) 
  {
    sprintf(printmp, "\nCopy file %s to %s\n",fname,fnew);
	 SER_putString(printmp);

  }
  else 
  {
    sprintf(printmp, "\nCopy file %s, %s to %s\n",fname,fmer,fnew);
	 SER_putString(printmp);

  }
  fout = fopen (fnew,"w");            /* open the file for writing           */
  if (fout == NULL) 
  {
    sprintf(printmp, "\nFailed to open %s for writing!\n",fnew);
	 SER_putString(printmp);

    fclose (fin);
    return;
  }

  total = 0;

  while ((cnt = fread (&buf, 1, 512, fin)) != 0) 
  {

 	 //sprintf(printmp, "cnt = %d, buf =%s\n", cnt, buf);
	 //SER_putString(printmp);
    fwrite (&buf, 1, cnt, fout);
    total += cnt;
  }
  fclose (fin);                       /* close input file when done          */

  if (merge == __TRUE) 
  {
    fin = fopen (fmer,"r");           /* open the file for reading           */
    if (fin == NULL) 
	 {
      sprintf(printmp, "\nFile %s not found!\n",fmer);
		SER_putString(printmp);
    }
    else 
	 {
      while ((cnt = fread (&buf, 1, 512, fin)) != 0) 
		{
        fwrite (&buf, 1, cnt, fout);
        total += cnt;
      }
      fclose (fin);
    }
  }
  fclose (fout);

  dot_format (total, &buf[0]);
  sprintf(printmp, "\n%s bytes copied.\n", &buf[0]);
  SER_putString(printmp);
}
#endif	 //cmd_copy 


/*----------------------------------------------------------------------------
 *        Delete a File
 *---------------------------------------------------------------------------*/
static void cmd_delete (char *par) 
{
  char *fname,*next,dir;

  SetBufferValue((uchar *)printmp, PRINT_TMP_SIZE, 0);

  fname = get_entry (par, &next);
  if (fname == NULL) 
  {
    SER_putString("\nFilename missing.\n");

    return;
  }

  dir = 0;
  if (*(fname + strlen(fname) - 1) == '\\') 
  {
    dir = 1;
  }

  if (fdelete (fname) == 0) 
  {
    if (dir) 
	 {
      sprintf(printmp, "\nDirectory %s deleted.\n",fname);
		SER_putString(printmp);
    }
    else 
	 {
      sprintf(printmp, "\nFile %s deleted.\n",fname);
		SER_putString(printmp);
    }
  }
  else 
  {
    if (dir) 
	 {
      sprintf(printmp, "\nDirectory %s not found or not empty.\n",fname);
		SER_putString(printmp);
    }
    else 
	 {
      sprintf(printmp, "\nFile %s not found.\n",fname);
		SER_putString(printmp);
    }
  }
}

/*----------------------------------------------------------------------------
 *        Print a Mass Storage Device Directory
 *---------------------------------------------------------------------------*/
static void cmd_dir (char *par)
{
  U64 fsize;
  U32 files,dirs,i;
  char temp[32],  *mask,*next,ch;
  FINFO info;

  mask = get_entry (par, &next);
  if (mask == NULL) 
  {
    mask = "*.*";
  }

  SER_putString ("\nFile System Directory...");
  files = 0;
  dirs  = 0;
  fsize = 0;
  info.fileID  = 0;

  os_dly_wait(1);
  SetBufferValue((uchar *)printmp,PRINT_TMP_SIZE, 0);

  while (ffind (mask,&info) == 0) 	    // 查找文件
  {

    if (info.attrib & ATTR_DIRECTORY)
	 {
		os_dly_wait(1);    

      i = 0;
      while (strlen((const char *)info.name+i) > 41) 
		{
        ch = info.name[i+41];
        info.name[i+41] = 0;

        sprintf (printmp,"\n%-41s", &info.name[i]);
		  SER_putString(printmp);

        info.name[i+41] = ch;
        i += 41;
		  os_dly_wait(1);    
      }

      sprintf (printmp,"\n%-41s    <DIR>       ", &info.name[i]);
		SER_putString(printmp);

	   sprintf (printmp,"  %02d.%02d.%04d  %02d:%02d",
               info.time.day, info.time.mon, info.time.year,
               info.time.hr, info.time.min);	
		SER_putString(printmp);
		
	   dirs++;
    }
    else 
	 {
	   os_dly_wait(1); // 加延时后，否则文件列表打印两次

      dot_format (info.size, &temp[0]);
      i = 0;

      while (strlen((const char *)info.name+i) > 41) 
		{
		SER_putString("in cmd_dir line 527\n");
        ch = info.name[i+41];
        info.name[i+41] = 0;
        sprintf (printmp, "\n%-41s", &info.name[i]);
		  SER_putString(printmp);
        info.name[i+41] = ch;
        i += 41;
		  os_dly_wait(1); 
      }

      sprintf (printmp, "\n%-41s %14s ", &info.name[i], temp);
		SER_putString(printmp);
		
      sprintf (printmp, "  %02d.%02d.%04d  %02d:%02d",
               info.time.day, info.time.mon, info.time.year,
               info.time.hr, info.time.min);
		SER_putString(printmp);

      fsize += info.size;
      files++;
    }
  }
  
  if (info.fileID == 0) 
  {
    SER_putString ("\nNo files...");
  }
  else 
  {
    dot_format (fsize, &temp[0]);
    sprintf (printmp, "\n              %9d File(s)    %21s bytes", files, temp);
	 SER_putString(printmp);	
  }
  dot_format (ffree(""), &temp[0]);
  if (dirs) {
    sprintf (printmp, "\n              %9d Dir(s)     %21s bytes free.\n", dirs, temp);
	 SER_putString(printmp);
  }
  else 
  {
    sprintf (printmp, "\n%56s bytes free.\n",temp);
	 SER_putString(printmp);
  }
  os_dly_wait(20);
}

/*----------------------------------------------------------------------------
 *        Format a Mass Storage Device
 *---------------------------------------------------------------------------*/
static void cmd_format (char *par) 
{
  char *label,*next,*opt;
  char arg[20];
  SetBufferValue((uchar *)printmp,PRINT_TMP_SIZE, 0);
  os_dly_wait(1);

  label = get_entry (par, &next);
  if (label == NULL) 
  {
    label = "KEIL";
  }
  strcpy (arg, label);
  opt = get_entry (next, &next);
  if (opt != NULL) 
  {
    if ((strcmp (opt, "/FAT32") == 0) ||(strcmp (opt, "/fat32") == 0)) 
	 {
      strcat (arg, "/FAT32");
    }
  }
  SER_putString("\nFormat Flash Mass Storage Device? [Y/N]\n");
  if (get_line_usb(in_line) == 0)				// 暂停并等待用户输入命令
  {
		return;
  }

  if (in_line[0] == 'y' || in_line[0] == 'Y') 
  {
    /* Format the MSD with Label "KEIL". "*/
    if (fformat (arg) == 0) 
	 {
      SER_putString("Mass Storage Device Formatted.\n");
      sprintf(printmp, "Mass Storage Device Label is %s\n",label);
		SER_putString(printmp);
    }
    else 
	 {
      SER_putString("Formatting failed.\n");
    }
  }
  os_dly_wait(20);
}

#if 	0
/*----------------------------------------------------------------------------
 *        Display Command Syntax help
 *---------------------------------------------------------------------------*/
static void cmd_help (char *par) {
  printf (help);
}

#endif 
/*----------------------------------------------------------------------------
 *        Initialize a Flash Mass Storage Device
 *---------------------------------------------------------------------------*/

static unsigned char init_msd (void) 
{
  U32 retv;

  	if ((retv = finit ("U0:")) != 0) 
	//if(0)
	{
    if (retv == 1) 
	 {
       #if(USB_DEBUG)
		SER_putString ("\nMSD Init Failed");
      SER_putString ("\nInsert Mass Storage Device...\n");
		os_dly_wait(10);
		#endif
    }
    else 
	 {
       #if(USB_DEBUG)
		SER_putString ("\nMass Storage Device is Unformatted");
		#endif
    }	
	 	return 1;
   }

	return 0;
}
/*----------------------------------------------------------------------------
 *        memset 	add by xzb   for G11
 *---------------------------------------------------------------------------*/
//static void memset_x(char *adr, unsigned int len, char ch)
//{
//	int i = 0;
//
//	while (i < len)
//	{
//		adr[i++] = ch;
//	}
//}
/*----------------------------------------------------------------------------
 *        获取输入    add by xzb  (注意：调此调函数的前提，USB已经初始化，否则系统重启)
 *---------------------------------------------------------------------------*/
static char	get_line_usb(char *in_line)
{
	while (SER_getString ((unsigned char *)in_line) == __FALSE) 		 // 获取命令								  
	{
			  //os_dly_wait(1);
			  
		  if (USB_CONNECT & 0x01)	   // 检测U盘是否插入	  
		  {
			   os_dly_wait(1);	 
		  }
		  else 
		  {
		  		 #if(USB_DEBUG)
				SER_putString("USB has removed!\r\n\0");
				#endif
			 	return  0;
		  }	 
    }
	 return 1;
}

/*----------------------------------------------------------------------------
 *        USB TASK 
 *---------------------------------------------------------------------------*/
static void usb_cmd_mode(void)
{
  char *sp,*cp,*next;
  U32 i, t ;

  #if(USB_DEBUG)
  SER_putString ("\nMass Storage Device has Inserted...\r\n\0");
  SER_putString("Please  Enter any key to use USB!\r\n\0");
  
  os_dly_wait(10);
  #endif

//  while (SER_getString ((unsigned char *)in_line) == __FALSE) 		 // 暂停并等待用户输入命令								  
//  { 
//
//	  if (USB_CONNECT & 0x01)	   // 检测U盘是否插入	  
//	  {
//		   os_dly_wait(1);	 
//	
//	  }
//	  else 
//	  {
//	  		SER_putString("USB has removed!\r\n\0");
//		 	return;
//	  }
//  }

  if (get_line_usb(in_line) == 0)             // 	暂停并等待用户输入命令
  {
		return;
  }

  if( init_msd ())		  // 如果初始USB不成功，则返回 
  {
		return;
  }
  
  SER_putString("COMMAND: CAP   TYPE   REN   DEL  DIR  FORMAT   FILL EXIT \r\n\0");
  SER_putString("Enter EXIT to remove USB \r\n\0");
  t = 360000;              //一分钟无命令输入退出USB
  while (t--) 
  {
	 os_dly_wait(1);

	 SER_putString ("\nCmd> ");                       /* display prompt              */
                                              /* get command line input      */
	 SetBufferValue((uchar *)&in_line[0], IN_LINE_SIZE, 0);		

	 if (get_line_usb(in_line) == 0)				// 暂停并等待用户输入命令
	 {
		 return;
	 }

	 //SER_putString("\n in_line value:\n");
	 //SER_putString(in_line);
	 
	 os_dly_wait(1);
    usbh_engine(0);
    while (!usbh_msc_status(0,0)) 		  /* If device is not connected  */
	 {                 
       SER_putString ("\nInsert Mass Storage Device ...\n");
  
		 if (USB_CONNECT & 0x01)	   // 检测U盘是否插入	  
	    {
			  os_dly_wait(1);	 
			  usbh_engine(0);
		 }
		 else 
		 {	  
		     #if(USB_DEBUG)	
		 	  SER_putString("USB has removed!\r\n\0");
			  #endif
		 	  break;
		 }
    }
    sp = get_entry (&in_line[0], &next);
    if (*sp == 0) 
	 {
      //continue;
		return;
    }
    for (cp = sp; *cp && *cp != ' '; cp++) 
	 {
      *cp = toupper (*cp);                    /* command to upper-case       */
    }

	 //SER_putString("input cmd: \n");
	 //SER_putString(sp);

	os_dly_wait(1);

	if (!strcmp (sp,"EXIT"))			 // 移除USB
	{
		
			//usbh_ohci_hw_uninit();      // 关闭USB	执行函数会导致系统重启，				  不用做任何操作，USB只用到了数据线，可以直接插拔。
			SER_putString("\nyou can remove U disk !\r\n\0");
			os_dly_wait(20);

		  while (USB_CONNECT & 0x01)
		  {
		  			os_dly_wait(10);
		  }
			//SER_putString("\nThe U disk has removed!!!\r\n\0");
			//usbh_ohci_hw_init();
			return;
	}
	 
    for (i = 0; i < CMD_COUNT; i++) 
	 {
	 	
      if (strcmp (sp, (const char *)&cmd[i].val)) 
		{
        continue;
      }
		os_dly_wait(10);
      cmd[i].func (next);                     /* execute command function    */
		t = 360000;      
      break;
    }

    if (i == CMD_COUNT) 
	 {
      SER_putString ("\nCommand error\n");
    }
  }
}




static void ModifyGBFileName(char *pFile)
{
	 /*uchar buf[512+32];
	 // test
	 //usbh_msc_read(0, 0, sect, buf, cnt);
	 usbh_msc_read(0, 0, 0, (U8 *)buf, 1);

	SER_putString ("\nRead fat sector Test Data:"); 
	SER_putb(512,(uchar *)buf);
	SER_putString ("\r\n");
	os_dly_wait(20);	  */
	uint32	BootSectorAddr, i;
	uchar *pT;

	union {
		uchar buf[512+8];
		#pragma	pack(1)
		struct
		{
			uchar JumpCode[3];
			uchar	OEMName[8];
			uint16 BytesPerSector;
			uchar SectorsPerCluster;	 // 每簇的扇区数
			uint16 RevSectors;   // 保留扇区
			uchar TotalFATTab;	// FAT表个数，一般为2
			uint16 NoUse1;
			uint16 NoUse2;
			uchar Type;
			uint16 NoUse3;
			uint16 MaxSecotorsPerHead;	// 每磁道的最大扇区数 （硬盘的参数）
			uint16 MaxHeads;		//   最大磁头数  （硬盘的参数）
			uint32 PartitionHideSecotors; // 隐藏分区扇区数
			uint32 PartitionTotalSecotors; // 分区中的扇区总数
			uint32 SectorsPerFATTab; //  每个FAT表占用的扇区数
			uint16 Flags;
			uint16 FAT32DriverVer;	//  FAT32驱动版本（固定值：00 00）
			uint32 BootDIRStartClusterNo;	// 根目录区的开始簇号
			uint16 BakBootSectorNo;	  //备份引导扇区所在位置的扇区号（此扇区号为相对位置）
			uchar Rev[12]; // 保留
			uchar PartitonLogicNo;	// 分区的逻辑驱动号
			uchar NoUse4;
			uchar ExtenFlag;		// 扩展标识
			uchar ParttionSerial[4];
			uchar PartitionName[11];
			uchar FATName[8];
		}FatBoot;
		#pragma	pack()
	} SecotorInfo;
	 // test
	 //usbh_msc_read(0, 0, sect, buf, cnt);
	 usbh_msc_read(0, 0, 0, (U8 *)SecotorInfo.buf, 1);

	//SER_putString ("\nRead fat sector Test Data:"); 
	//SER_putb(512,(uchar *)SecotorInfo.buf);
	//SER_putString ("\r\n");
	os_dly_wait(20);	  




	if ((SecotorInfo.buf[0]==0xeb)&&(SecotorInfo.buf[1]==0x58)&&(SecotorInfo.buf[2]==0x90)) 	// 不须加偏移找到FAT信息
	{
		// 计算根目录扇区地址 (即DATA区)	  保留扇区数＋（每个FAT表占用的扇区数×2）
		BootSectorAddr = SecotorInfo.FatBoot.RevSectors + SecotorInfo.FatBoot.TotalFATTab*SecotorInfo.FatBoot.SectorsPerFATTab;
	}
	else
	{		 // 从MBR到第一个分区扇区的扇区个数（一般为硬盘扇区的最大值）,即可知道 FAT BOOT 根目录所在扇区
		 pT = &SecotorInfo.buf[0x1c6];	  // 446 + 8 
		 BootSectorAddr	= pT[0] | (pT[1]<<8) | (pT[2]<<16) | (pT[3]<<24);	 // FAT BOOT 扇区地址

		//SER_putString ("\nBootSectorAddr Test Data:"); 
		//SER_putb(4,(uchar *)&BootSectorAddr);
		//SER_putString ("\r\n");
		//os_dly_wait(20);
								
		 usbh_msc_read(0, 0, BootSectorAddr, (U8 *)SecotorInfo.buf, 1);

		 //SER_putString ("\nRead fat sector Test Data:"); 
		 //SER_putb(512,(uchar *)SecotorInfo.buf);
		 //SER_putString ("\r\n");
		 os_dly_wait(20);
		 
		 BootSectorAddr += SecotorInfo.FatBoot.RevSectors + SecotorInfo.FatBoot.TotalFATTab*SecotorInfo.FatBoot.SectorsPerFATTab;

	}
		
		/*SER_putString ("\nSecotorInfo.FatBoot.SectorsPerFATTabTest Data:"); 
		SER_putb(4,(uchar *)&SecotorInfo.FatBoot.SectorsPerFATTab);
		SER_putString ("\r\n");
		os_dly_wait(20);

		SER_putString ("\nSecotorInfo.FatBoot.TotalFATTab Test Data:"); 
		SER_putb(1,(uchar *)&SecotorInfo.FatBoot.TotalFATTab);
		SER_putString ("\r\n");
		os_dly_wait(20);
				 
		SER_putString ("\nSecotorInfo.FatBoot.RevSectors Test Data:"); 
		SER_putb(2,(uchar *)&SecotorInfo.FatBoot.RevSectors);
		SER_putString ("\r\n");
		os_dly_wait(20);

		SER_putString ("\nBootSectorAddr Test Data:"); 
		SER_putb(4,(uchar *)&BootSectorAddr);
		SER_putString ("\r\n");
		os_dly_wait(20); */

	while(1)
	{
		usbh_msc_read(0, 0, BootSectorAddr, (U8 *)SecotorInfo.buf, 1);	


		//SER_putString ("\nRead boot dir Test Data:"); 
		//SER_putb(512,(uchar *)SecotorInfo.buf);
		//SER_putString ("\r\n");
		os_dly_wait(2);

	   if (SecotorInfo.buf[0]==0x00)	 // 没有文件名了
		{
			break;
		}

		pT = &SecotorInfo.buf[0];
		for (i=0; i<512;i += 32)
		{
			if ((pT[i]==0x42)&&(pT[i+1]==0x58)&&(pT[i+2]==0x00)) 		// 找到需要修改的文件名区域
			{
				SER_putString ("Update GB19056 File name 1\r\n");

				//pT[i+1] = 0xa4;	 pT[i+2] = 0x7c;		  // 粤 的uncode码
				Get_Vehicle_Code_GB(&pT[i+1]);		// 获取并修改

				//SER_putString ("\n pT[i+1] Test Data:"); 
				//SER_putb(2,(uchar *)&pT[i+1]);
				//SER_putString ("\r\n");
				//os_dly_wait(20);

				usbh_msc_write(0, 0,BootSectorAddr,SecotorInfo.buf, 1);		// 回写
				break;
				
			}
			else if ((pT[i]==0x42)&&(pT[i+1]=='U')&&(pT[i+2]==0x00)) 
			{
			 	memmove((pT+i+1),L"未知车牌",8);

				//SER_putString ("\n UNKW Test Data:"); 
				//SER_putb(8,(uchar *)(pT+i+1));
				//SER_putString ("\r\n");
				//os_dly_wait(20);
				SER_putString ("Update GB19056 File name 2\r\n");

				usbh_msc_write(0, 0,BootSectorAddr,SecotorInfo.buf, 1);		// 回写
				break;
			}

		}

		if (i != 512)
		{
			break;
		}

		BootSectorAddr++;

   }

	


	 //
	
}



__task    void   tTaskUSB(void)   
{
	//uint16   USBCnt = 0; 
	NOP();	NOP();
	usbh_init(0); 
	Wfile_Flag=0; 
	while(1)
	{	
		NOP();	NOP();
		TC.USB = 0;
		IDEL_Dly_Cnt = 0;
		IDEL_For_Acc_OFF = FALSE;   //插入USB会唤醒休眠
		os_dly_wait(25);				//100ms
		if(GetGB_Data_Flag==0)		//无线通道取记录数据优先
		{
			//Print.TickCnt++;
			USBCnt++;
			if((gCnt.SystemSecond>20)&&(USBCnt%10==0))								  
			   TaskUSB();
		
			if(USBUseFlag)
			{
				os_dly_wait(1);
				NOP();	NOP();
				GetGB_Data_Flag = 1;
			    USB_GetGBdata();	
				
				
				//
				ModifyGBFileName(FileName);
				//
				  
				os_dly_wait(100); 
				Wfile_Flag = 0; 

				
				
			} 
		}
		//if(((USB_CONNECT&0x01)==0))
		if(GET_USB_STATUS==0)
		{
		  //#if(SYSTEM_BOOT_MESSAGE)
		  SER_putString("USB task  delect\r\n\0");
		  //#endif						 
		  os_dly_wait(2);
		 if (usbh_uninit(0)==1)
		 {
		 	SER_putString("usbh_uninit OK\r\n\0");
		 }
		 else
		 {
		 	 usbh_uninit(0);
		 }
		 if (funinit ("U0:")==0)
		 {
		 	SER_putString("funinit OK\r\n\0");
		 }
		 else
		 {
		 	funinit ("U0:");
		 }




		  //Write_File_Error=0;
		  Wfile_Flag=0;
		  GetGB_Data_Flag = 0;
		  USBUseFlag=0;
		  USBTaskCreat = 0;
		  TC.USB = 0;
		  os_tsk_delete_self();
		}
	}

}




/*检测U盘是否插入，并初始化U盘和文件系统 */
void   TaskUSB(void) 
{
	NOP(); NOP();NOP(); NOP();
	
	// test 

//	while (1)
//	{
//		os_dly_wait(1);
//		usb_cmd_mode();
//	}
	//
	
	if(uiSpeed<5)
	{	
		 if (Wfile_Flag==0)	 // 当插入U盘，写文件 
		 {
		    Wfile_Flag = 1;
		 	 #if(USB_DEBUG)
			 SER_putString("usb task ...\n");  os_dly_wait(2);
			 #endif
				  
			 //if(init_msd ()!=0) 				  // 初始化U盘
			 if(finit("U0:")!=0)
		    { 
				SER_putString("usb init failed ...\n");
				os_dly_wait(5); 
				funinit ("U0:");  
				usbh_init(0); 
				os_dly_wait(50); 
				Wfile_Flag = 0; 
				return;
			 }         
			 #if(USB_DEBUG)
			 SER_putString("usb task step  2 ...\n");
			 os_dly_wait(2);
			 #endif
			 os_dly_wait(1);
			 Get_Fname((uchar *)in_line);			//取文件名	in_line 中
			 strncpy(FileName,in_line,32);	  // added 2015.3.2
			 gdata_to_file(in_line);	//创建一个文件并打开
				//gdata_to_file("Temp");								 //先建立一个备份文件，等写结束后重新命名
				//SER_putString("usb task step  5 ...\n");
				//os_dly_wait(2);
		 }
		 else if(USBUseFlag==0)				      // 否则，进入USB命令模式
		 {
		 	 if(Write_File_Error==0)
			 {
			   Wfile_Flag = 0;
			   usb_cmd_mode();
			 }
			 //usbh_uninit(0);
			 //funinit ("U0:");
		 }
	}
  
}



/*把一包数据写入U盘*/

void     WrintDataToFile(void)
{
	unsigned  char  *buffer, ret;
	
	ret = 0; 
   buffer = 	&TxdTBuffer[0];

	if(USB_CONNECT & 0x01)
	  ret = fwrite(buffer, LenDataForUSB ,1 , gfile);	  //   写文件
	else 
	  ret = 0;
	//fflush(gfile);
	if (ferror(gfile)||(ret==0))								//(ferror(gfile)||
	{
		 //设置出错标志
		 Write_File_Error = 1;
		 Wfile_Flag = 0;
		 LenDataForUSB = 0;
		 #if(USB_DEBUG)
		 SER_putString ("\n USB write ERROR!!!\r\n\0");
		 os_dly_wait(2);
		 #endif       
	    return;
	}
	if(TD.Display==tDISP_USB_RECODE)
	{
	 SER_putb(LenDataForUSB, buffer);
	 os_dly_wait(LenDataForUSB/15);
	}
	LenDataForUSB = 0;

	if(USBUseFlag==0)
	{
	  if(fclose (gfile)!=0)
	  {
		 Write_File_Error = 1;
		 Wfile_Flag = 0;
		 #if(USB_DEBUG)
		 SER_putString ("\n File Close ERROR!!!\r\n\0");
		 //funinit ("U0:");	  
		 os_dly_wait(2); 
		 #endif
		 //finit ("U0:"); 
		 //wfile_flag = 0; 
	  }
	  else
	  {
		  gfile = NULL;
		  #if(USB_DEBUG)
		  SER_putString ("\nAuto write file finished!!!\r\n\0");                      /* close the output file               */
		  os_dly_wait(1);
		  Wfile_Flag = 0;
		  #endif
/*		  Get_Fname(FineName);
		  //if(frename("Temp", "D111029_095000_AAAAAAAA_BBBBBBBB.DVR")==0)
		  if(frename("Temp", FineName)==0)					//当文件目录下文件太多且文件名类似的时候会出现fopen慢的问题，所以在建立文件时用了临时的文件名，在文件写完后重新命名
		  {
		  	  #if(USB_DEBUG)
			  SER_putString ("\nRename succesful!!!\r\n\0");                      // close the output file              
			  os_dly_wait(1);
			  #endif
		  }														*/
	  }
	}
}





/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
