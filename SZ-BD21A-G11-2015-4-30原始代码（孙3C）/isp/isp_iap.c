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

#include "LPC17XX.H"
#include "isp_iap.h"

#include <string.h>

//char cmd_buf[CMD_SIZE];
char * param_buf[NO_OF_ISP_PARAMS];
unsigned int	param_table[NO_OF_IAP_PARAMS];
unsigned int	result_table[NO_OF_IAP_PARAMS];




void iap_entry(unsigned param_tab[],unsigned result_tab[])
{
    void (*iap)(unsigned [],unsigned []);

    iap = (void (*)(unsigned [],unsigned []))IAP_ADDRESS;
    iap(param_tab,result_tab);
}



unsigned char 	GetDeviceSerialNumber(unsigned char *pData)
{
	unsigned int	Cnt;
	result_table[IAP_STAT_CODE] = 0xff;
	Cnt = 0;
	while( (result_table[IAP_STAT_CODE] != CMD_SUCCESS)&&(Cnt<20) )
	{
		param_table[IAP_CMD_CODE] = READ_SERIAL_NUMBER;
   	iap_entry(param_table,result_table);

		if( result_table[IAP_STAT_CODE] == CMD_SUCCESS)
		{
			memmove( pData, (unsigned char*)&result_table[1],16);
			return 0;
		}
		Cnt++;
	}
   return Cnt;   
}



