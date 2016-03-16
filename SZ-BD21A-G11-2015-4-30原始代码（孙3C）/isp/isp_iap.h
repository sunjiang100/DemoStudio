//-----------------------------------------------------------------------------
// Software that is described herein is for illustrative purposes only  
// which provides customers with programming information regarding the  
// products. This software is supplied "AS IS" without any warranties.  
// NXP Semiconductors assumes no responsibility or liability for the 
// use of the software, conveys no license or title under any patent, 
// copyright, or mask work right to the product. NXP Semiconductors 
// reserves the right to make changes in the software without 
// notification. NXP Semiconductors also make no representation or 
// warranty that such application will be suitable for the specified 
// use without further testing or modification. 
//-----------------------------------------------------------------------------

#ifndef  _ISP_IAP_H
#define  _ISP_IAP_H

#define     CR                  0x0D
#define     LF                  0x0A
#define     NUL                 0x00
#define     SP                  0x20
#define     NO_OF_IAP_PARAMS    5
#define     NO_OF_ISP_PARAMS    5
#define     PARAM_SIZE          15
#define     ISP_CMD_CODE        0
#define     ISP_PARAM_0         0
#define     ISP_PARAM_1         1
#define     ISP_PARAM_2         2
#define     ISP_PARAM_3         3
#define     ISP_PARAM_4         4
#define     IAP_CMD_CODE        0
#define     IAP_PARAM_0         1
#define     IAP_PARAM_1         2
#define     IAP_PARAM_2         3
#define     IAP_PARAM_3         4
#define     IAP_STAT_CODE       0
#define     IAP_RESULT_0        1
#define     IAP_RESULT_1        2
#define     IAP_RESULT_2        3
#define     IAP_RESULT_3        4

#define     CMD_SIZE            70
#define     UU_LINE             45
#define     LINE_CHECKSUM       20
//#define     TRUE                1
//#define     FALSE               (!TRUE)
#define     UNLOCK_CODE         0x5A5A
#define     BY_4                4

typedef enum
{
PREPARE_SECTOR_FOR_WRITE=50,
COPY_RAM_TO_FLASH=51,
ERASE_SECTOR=52,
BLANK_CHECK_SECTOR=53,
READ_PART_ID=54,
READ_BOOT_VER=55,
COMPARE=56,
REINVOKE_ISP=57,
READ_SERIAL_NUMBER=58
}IAP_Command_Code;

typedef enum
{
CMD_SUCCESS=0,
INVALID_COMMAND=1,
SRC_ADDR_ERROR=2, /* Source address not on word boundary */
DST_ADDR_ERROR=3, /* Destination address not on word or 256 byte boundary */
SRC_ADDR_NOT_MAPPED=4,
DST_ADDR_NOT_MAPPED=5,
COUNT_ERROR=6, /* Byte count is not multiple of 4 or is not a permitted value */
INVALID_SECTOR=7,
SECTOR_NOT_BLANK=8,
SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION=9,
COMPARE_ERROR=10,
BUSY=11, /* Flash programming hardware interface is busy */
PARAM_ERROR=12, /* Insufficient number of parameters */
ADDR_ERROR=13, /* Address not on word boundary */
ADDR_NOT_MAPPED=14,
CMD_LOCKED=15, /* Command is locked */
INVALID_CODE=16, /* Unlock code is invalid */
INVALID_BAUD_RATE=17,
INVALID_STOP_BIT=18,
CODE_READ_PROTECTION_ENABLED=19
}Command_ErrorCode;

typedef enum
{
COUNT=100,
FLASH_PROG_COUNT,
RAM_OR_FLASH_ADDRESS,
RAM_ADDRESS,
FLASH_ADDRESS,
NO_PARAM_CHECK
}Parameter_Type;

#define CMD_SUCCESS 0
#define IAP_ADDRESS 0x1FFF1FF1

void execute_user_code(void);
unsigned user_code_present(void);
unsigned check_isp_entry_pin(void);
void isp_cmd_loop(void);
int str_cmp(char *, char *);
int is_LPC175x(void);
void enter_isp_mode(void);

void memset_x(unsigned char *str, int len, unsigned char ch);

#endif /* _ISP_IAP_H */


