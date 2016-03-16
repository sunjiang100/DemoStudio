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
//#include "..\Rtc\Rtc.h"

//#include		"..\UART\UART0.h"

#define  USB_DEBUG					 1

//#define USB_CONNECT              (*(volatile unsigned long *)(0x5000C054)) 

//#define memaddr                  (*(volatile unsigned long *)(0x20080000))

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
#define IN_LINE_SIZE		160
#define PRINT_TMP_SIZE		100

/* Local variables */
static char in_line[IN_LINE_SIZE];		// 用于命令行
static char	printmp[PRINT_TMP_SIZE];     // 用于格式化输出


//static FILE  *FhandleBuf;

/* Local Function Prototypes */
static void dot_format (U64 val, char *sp);
static char *get_entry (char *cp, char **pNext);
static unsigned char init_msd (void);
static void memset_x(char *adr, unsigned int len, char ch);
static char	get_line_usb(char *in_line);
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
  	 memset_x(&in_line[0], IN_LINE_SIZE, 0); 
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

  memset_x(printmp, PRINT_TMP_SIZE, 0);

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
#if 	1

//static  char fnamebuf[100];
void gdata_to_file (char *fname) 
{
  //FILE *gfile;
  unsigned int t, ret;
  unsigned  char  CheckSum, *buffer;


  //init_msd ();


  usbh_engine();
  SER_putString ("\nInsert Mass Storage Device 1...\r\n\0");
  os_dly_wait(3);
  while (!usbh_msc_status()) 		  /* If device is not connected  */
  {    
       #if(USB_DEBUG)             
       SER_putString ("\nInsert Mass Storage Device 2...\r\n\0");
  		 os_dly_wait(3);
		 #endif
		 if (USB_CONNECT & 0x01)	   // 检测U盘是否插入	  
	    {
			  os_dly_wait(1);	 
			  usbh_engine();
		 }
		 else 
		 {	  
		     wfile_flag = 0;
		 	  return;
		 }
		   
      
  }
  #if(USB_DEBUG)
  SER_putString ("\nInsert Mass Storage Device 3...r\n\0");
  os_dly_wait(3);
  #endif
  memset_x(printmp, PRINT_TMP_SIZE, 0);

  if (fname == NULL) 
  {
     #if(USB_DEBUG)
	  SER_putString("\nFilename missing.\r\n\0");
	  #endif
    return;
  }
  //fdelete (fname);
  if(gfile != NULL )
  //if(0)
  {  
     if((fclose (gfile)==0))         //fclose (gfile);     //while((gfile != NULL )&&(fclose (gfile)!=0))  os_dly_wait(10); 
	  {
	    #if(USB_DEBUG)
		 SER_putString("\nClose file succeed!\r\n\0"); 
	    os_dly_wait(2);
		 #endif 
	  }

  }
  gfile = fopen (fname, "w");               /* open a file for writing           */
  
  //gfile = fopen (fname, "w"); 
  SER_putString ("\nInsert Mass Storage Device 4...r\n\0");
  os_dly_wait(2);
  if (gfile == NULL) 
  {
    #if(USB_DEBUG)
	 SER_putString("\nCan not open file!\r\n\0");  /* error when trying to open file    */
	 #endif
	 write_file_error_flag = 1;
	 //finit ("U0:");
	 os_dly_wait(100);
    return;
  } 
//   FhandleBuf = gfile;
	USBUseFlag = 1;
	os_dly_wait(10);
if(0)
{
	ret = 0; 
	//USBUseFlag = 1;
	CheckSum = 0;
	//write_file_error_flag = 0;
  while (USBUseFlag == 1)
  {
  	  	
	  //USBUseFlag = 1;
	     if (LenDataForUSB)
		  {
		  	  
			  buffer = 	&TxdTBuffer[0];
			 // 加校验
			  for(t=0; t<LenDataForUSB; t++)	{ CheckSum ^= *(buffer+t);	}
				  
			  //while((ret==0)&&(write_file_error_flag==0)) 
			 // {	
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
						 write_file_error_flag = 1;
						 wfile_flag = 0;
						 LenDataForUSB = 0;
						 #if(USB_DEBUG)
						 SER_putString ("\n USB write ERROR!!!\r\n\0");
						 os_dly_wait(2);
						 #endif       
				       return;
					}
					//os_dly_wait(1);
			 //} 
	
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
}		
}
#endif

/*----------------------------------------------------------------------------
 *        Read file and dump it to serial window
 *---------------------------------------------------------------------------*/
static void cmd_type (char *par) 
{
  char *fname,*next;
  FILE *f;
  int ch;

  memset_x(printmp,PRINT_TMP_SIZE, 0);

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
  
  memset_x(in_line, 160, 0);
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

  memset_x(printmp, PRINT_TMP_SIZE, 0);

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

  memset_x(printmp, PRINT_TMP_SIZE, 0);

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

  memset_x(printmp, PRINT_TMP_SIZE, 0);

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
  memset_x(printmp,PRINT_TMP_SIZE, 0);

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
 

  
  memset_x(printmp,PRINT_TMP_SIZE, 0);
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
  //retv = getkey();

//  while (SER_getString ((unsigned char *)in_line) == __FALSE) 		 // 获取命令								  
//  {
//  	  os_dly_wait(1);	 
//  }

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


  //while ((retv = finit ("U0:")) != 0) {           /* Wait until the MSD is ready*/
	//funinit (NULL);
	//os_dly_wait(50);
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
      //strcpy (&in_line[0], "KEIL\r\n");
      //cmd_format (&in_line[0]); *
		
    }	
	 	return 1;
   }

	return 0;

}
/*----------------------------------------------------------------------------
 *        memset 	add by xzb   for G11
 *---------------------------------------------------------------------------*/
static void memset_x(char *adr, unsigned int len, char ch)
{
	int i = 0;

	while (i < len)
	{
		adr[i++] = ch;
	}


}


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
static void usb_main(void)
{
  char *sp,*cp,*next;
  U32 i ;

  
  //printf (intro);                             /* display example info        */
  //printf (help);

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
 // if(1)
  {
		return;
		
  }
  
  SER_putString("COMMAND: CAP   TYPE   REN   DEL  DIR  FORMAT   FILL EXIT \r\n\0");
  SER_putString("Enter EXIT to remove USB \r\n\0");

  while (1) 
  {
	 os_dly_wait(1);

	 SER_putString ("\nCmd> ");                       /* display prompt              */
                                              /* get command line input      */
	 memset_x(&in_line[0], IN_LINE_SIZE, 0);		
//	 while (SER_getString ((unsigned char *)in_line) == __FALSE) 		 // 获取命令								  
//	 {
//			  //os_dly_wait(1);
//			  
//		  if (USB_CONNECT & 0x01)	   // 检测U盘是否插入	  
//		  {
//			   os_dly_wait(1);	 
//		
//		  }
//		  else 
//		  {
//		  		SER_putString("USB has removed!\r\n\0");
//			 	return;
//		  }	 
//		 
//   }

		if (get_line_usb(in_line) == 0)				// 暂停并等待用户输入命令
		{
			return;
		}

	 //SER_putString("\n in_line value:\n");
	 //SER_putString(in_line);
	 
	 os_dly_wait(1);
    usbh_engine();
    while (!usbh_msc_status()) 		  /* If device is not connected  */
	 {                 
       SER_putString ("\nInsert Mass Storage Device ...\n");
  
		 if (USB_CONNECT & 0x01)	   // 检测U盘是否插入	  
	    {
			  os_dly_wait(1);	 
			  usbh_engine();
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
      break;
    }

    if (i == CMD_COUNT) 
	 {
      SER_putString ("\nCommand error\n");
    }
  }
}


#define    		 NOP()      __nop()
void   TaskUSB(void) 
{
	
	///unsigned char  *memPoiter;
	//memPoiter = memaddr;
	//char  databuffer[5] = {1,2,3,4};
	NOP(); NOP();NOP(); NOP();
	//SER_putString("usb task ...\n");
	//os_dly_wait(1);

	//finit ("U0:");
	//usbh_init();
	 
	//while (1)
	//if(uiSpeed==0)
	if(1)
	{	
		//os_dly_wait(1);

		//wfile_flag = 0;
		if (USB_CONNECT & 0x01)	   // 检测U盘是否插入
		//while(USB_CONNECT & 0x01)	  
	   {
			 
			 USBCnt++;

			 //LPC_USB->HcCommandStatus &= 	~(3<<5);
			 //usb_main();
			 //SER_putString("ok!\n");
			 if (wfile_flag == 0)	 // 当插入U盘，写文件 
			 {
			    wfile_flag = 1;
			 	 #if(USB_DEBUG)
				 SER_putString("usb task ...\n");  os_dly_wait(2);
				 #endif
			    //usbh_init();
				 //memset((char *)memaddr, 4096, 0);
				 if(gfile==NULL)
				 {   
						if(init_msd ()!=0) { funinit ("U0:");  os_dly_wait(50);  finit ("U0:"); return;}         // 初始化U盘
				 }
				 else
				 {
					  //if( fclose (FhandleBuf)!=0)
					  if(1)
					  {	//funinit ("U0:");
					     	//os_dly_wait(100);
							if(init_msd ()!=0) { funinit ("U0:");  os_dly_wait(50);  finit ("U0:");  return;}         // 初始化U盘
							//os_dly_wait(100);
							//FhandleBuf = fopen(fnamebuf, "a");
							//SER_putString("Open File ok...\n");
							//os_dly_wait(10);
							//fputs ((const char *)databuffer, FhandleBuf); //fwrite(databuffer,1 ,3 , FhandleBuf);
							//SER_putString("Write File ok...\n");
							//os_dly_wait(10);
							//if(fclose(FhandleBuf)==0) 
							//{  
							//   SER_putString("Close File ok ...\n");
							//   os_dly_wait(2);
							//}
					      //funinit ("U0:");
							//SER_putString("Close File fail ...\n");
						   //os_dly_wait(10);
							if(init_msd ()!=0) { finit ("U0:");  os_dly_wait(50);  return;}         // 初始化U盘
					  }
				   
				 }
				 #if(USB_DEBUG)
				 SER_putString("usb task step  2 ...\n");
				 #endif
				 os_dly_wait(10);
				 //SER_putb(4, (unsigned  char *)&(LPC_USB->HcRhPortStatus1));
				// os_dly_wait(2);
				 //SER_putb(4, (unsigned  char *)&(LPC_USB->HcCommandStatus));
				  //os_dly_wait(2);
				 //USBUseFlag = 1;
			 	 //if(USBCnt%1==0)
				  //data_to_file((char *)&USBCnt);
				// else 
				   Get_Fname(in_line);
					//memmove(fnamebuf, in_line, 100);
					//SER_putb(36, (unsigned  char *)in_line);
					//SER_putb(4, (unsigned  char *)FnameBuf);
               //os_dly_wait(3);
				   gdata_to_file(in_line);
					//SER_putString("usb task step  5 ...\n");
					//os_dly_wait(2);

			 }
			 else if(USBUseFlag==0)				      // 否则，进入USB命令模式
			 {
			 	 wfile_flag = 0;
				 usb_main();
			 }
			 //os_dly_wait(100);
			 
		}
		
	}
  
}





void     WrintDataToFile(void)
{

	unsigned  char  *buffer, ret, t, CheckSum;
	
	ret = 0; 
	//USBUseFlag = 1;
	//CheckSum = 0;
	//write_file_error_flag = 0;
  //while (USBUseFlag == 1)
  //{
  	  	
	  //USBUseFlag = 1;
	    // if (LenDataForUSB)
		  //{
		  	  //#if(USB_DEBUG)
			//	SER_putString ("\n USB writing!!!\r\n\0");
				//os_dly_wait(10);
			//	#endif       
			  buffer = 	&TxdTBuffer[0];
			 // 加校验
			  //for(t=0; t<LenDataForUSB; t++)	{ CheckSum ^= *(buffer+t);	}
				  
			  //while((ret==0)&&(write_file_error_flag==0)) 
			 // {	
			      if(USB_CONNECT & 0x01)
				     ret = fwrite(buffer, LenDataForUSB ,1 , gfile);	  //   写文件
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
						 write_file_error_flag = 1;
						 wfile_flag = 0;
						 LenDataForUSB = 0;
						 #if(USB_DEBUG)
						 SER_putString ("\n USB write ERROR!!!\r\n\0");
						 os_dly_wait(2);
						 #endif       
				       return;
					}
					//os_dly_wait(1);
			 //} 
			   
			  if(TD.Display==tDISP_USB_RECODE)
			  {
			       SER_putb(LenDataForUSB, buffer);
					 os_dly_wait(LenDataForUSB/15);
			  }
			  LenDataForUSB = 0;
			//}
			//else
			 // os_dly_wait(1);							 
	  //}				  
	  // 写入检验数据
	  
	  //fwrite((char *)&CheckSum, 1, 1, gfile);
	  if(USBUseFlag==0)
	  {
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
			  SER_putString ("\nAuto write file finished!!!\r\n\0");                      /* close the output file               */
			  os_dly_wait(1);
			  #endif
			  //funinit("U0:");
			  //SER_putString("\nFile closed.\r\n\0");
		  }
	  }
}





/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
