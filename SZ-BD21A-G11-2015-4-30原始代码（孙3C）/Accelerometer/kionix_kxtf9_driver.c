/*==================================================================================================
                                                                               
     Header Name: kionix_kxtf9_driver.c

     General Description: Implementation for function calls to control Kionix KTXE9 via I2C
 
====================================================================================================


Revision History:
                           	 
Author               Modification Date            Description of Changes
-----------     	---------------------      ----------------------------------------------------------
Calvin Ong			06/30/2009					Code Clean up and rename some functions.
Calvin Ong          04/20/2009                  Setup, integrate & modify Chris Hudson's KXTF9 Driver interface


====================================================================================================
                                         INCLUDE FILES
==================================================================================================*/
#include "kionix_kxtf9_driver.h"
#include "i2c_interface.h"
#include		"..\Common\UserGlobals.h"
#include <RTL.h>
//#include		"..\Common\Algol.h"
#include "..\USB\serial_for_usb.h"



/*==================================================================================================
                                            MACROS
==================================================================================================*/
#define SET_REG_BIT(r,b) r |= b
#define UNSET_REG_BIT(r,b) r &= ~b

/*==================================================================================================
                                     LOCAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
                                     GLOBAL VARIABLES
==================================================================================================*/


/*==================================================================================================
                                   KIONIX SENSOR FUNCTIONS
==================================================================================================*/

//communication with KXTF9 via i2c
static BOOLEAN ACCEL_read_byte(INT8U reg, INT8U *data, INT8U length);
static BOOLEAN ACCEL_write_byte(INT8U reg, INT8U data);


//static void driver_debug(void);




/*==================================================================================================
FUNCTION: ACCEL_read_byte
DESCRIPTION: This function must be written to read data from the kionix accelerometer in bytes.
ARGUMENTS PASSED: None
return VALUE:     None
PRE-CONDITIONS:   None
POST-CONDITIONS:  None
IMPORTANT NOTES:  None
==================================================================================================*/
BOOLEAN ACCEL_read_byte(INT8U reg, INT8U *data, INT8U length)
{
    i2c_rw_cmd_type i2c_command;

	i2c_command.slave_addr	    = KXTF9_I2C_SLV_ADDR;	/* Please provide value to define accelerometer slave address*/
	i2c_command.length		    = length;				/* Please provide value to define number of I2C bytes*/
	i2c_command.slave_reg_addr	= reg;              	/* Please provide value of I2C register address*/
	i2c_command.data_ptr	    = data;					/* I2C read back data*/

	if (kionix_i2c_read(&i2c_command) == TRUE) 
	{
		return TRUE;
	}

	return FALSE;
}
/*==================================================================================================
FUNCTION:         ACCEL_write_byte
DESCRIPTION:      This function must be written to write a byte of data to the kionix accelerometer.
ARGUMENTS PASSED:
return VALUE:     None
PRE-CONDITIONS:   
POST-CONDITIONS:  None
IMPORTANT NOTES:  None
==================================================================================================*/
BOOLEAN ACCEL_write_byte(INT8U reg, INT8U data)
{
	i2c_rw_cmd_type i2c_command;

	i2c_command.slave_addr	    = KXTF9_I2C_SLV_ADDR;	/* Please provide value to define accelerometer slave address*/
	i2c_command.length		    = 1;				/*Always 1 byte of data to write to the accelerometer*/
	i2c_command.slave_reg_addr	= reg;				/* Please provide value of I2C register address*/
	i2c_command.data_ptr	    = &data;								
	
    if (kionix_i2c_write(&i2c_command) == TRUE) 
	{
		return TRUE;
	}

	return FALSE;
}
/*==================================================================================================
FUNCTION: KXTF9_init
DESCRIPTION:      this function initializes the sensor
ARGUMENTS PASSED: Pointers to sensor information structures
return VALUE:     None
PRE-CONDITIONS:   None  
POST-CONDITIONS:  None
IMPORTANT NOTES:  None
==================================================================================================*/
BOOLEAN kxtf9_init(ACCEL_INFO *accel_info, int enable)
{
	INT8U ctlreg_1 = 0;
	INT8U ctlreg_2 = 0;
	INT8U ctlreg_3 = 0;
	INT8U int_ctlreg_1 = 0;
	INT8U int_ctlreg_2 = 0;
	INT8U int_ctlreg_3 = 0;
	INT8U read_temp_hyst = 0;
	INT8U write_temp_hyst = 0;
	BOOLEAN status = TRUE;
	//INT8U itemp=0;
	int resolution, g_range;
	
	TILTINFO *dTILT; 
	WUFINFO *dWUF;
	TDTINFO *dTDT;
	MASKINFO *dMASK;
	INTINFO *dINT;
	
	ctlreg_1 = 0;	  
	ctlreg_2 = 0;
	ctlreg_3 = 0;
	int_ctlreg_1 = 0;
	int_ctlreg_2 = 0;
	int_ctlreg_3 = 0;
	read_temp_hyst = 0;
	write_temp_hyst = 0;
	status = TRUE;
	//itemp=0;
	
	resolution = accel_info->resolution;
	g_range = accel_info->g_range;
	dTILT = &accel_info->tiltinfo;
	 dWUF = &accel_info->wufinfo;
	 dTDT = &accel_info->tdtinfo;
	dMASK = &accel_info->maskinfo;
	 dINT = &accel_info->intinfo;
	
	  
	   //Disable sensor at the begining of initialization and reset CTRL_REG1 
	   UNSET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_PC1);
	   ACCEL_write_byte(KXTF9_I2C_CTRL_REG1, ctlreg_1);
       
	   
	    //Setting all bits for Control Register 1 (eliminates register read)
		if (enable == 1)
		{ 	
			/* set PC1 to enable the accelerometer outputs.
		       Recommend that enabling sensor is the last step 
		       of initialization. 
		    */ 	
		    SET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_PC1);

		}
		
				    
		
/************************  System Feature **************************/	
				
//resolution
		if(resolution == 12)
		{
			/* set resolution to 12 bits */
			SET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_RES);			
		}
		else if(resolution == 8)  //note: cannot use KXTF9_set_resolution() function.
		{
	   		/* set resolution to 8 bits */
			UNSET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_RES);		
		}
		
		//else default is set to 8 bits

//G range
		
	    if(g_range == 8)
		{		/* set G-range to 8g */
			SET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_GSEL1);
			UNSET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_GSEL0);
	    }
	    else if(g_range == 4)
	    {		/* set G-range to 4g */
			UNSET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_GSEL1);
			SET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_GSEL0);
		}	    
	    else if(g_range == 2)
		{
			UNSET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_GSEL1);
			UNSET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_GSEL0);
		}

/************************    TILT    **************************/	
	
		if (dTILT->enable == 1)
		{
			SET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_TPE);	/* set TPE bit to enable the screen rotation (tilt) function */

//ODR			
			/* ODR and Tilt Timer should always be set if we're enabling the tilt engine; Tilt Angle and Hysteresis are optional */
			/* set OTPA and OTPB according to dTILT->ODR */
			if (dTILT->ODR == 50) //50Hz
			{
				SET_REG_BIT(ctlreg_3, KXTF9_CTRL_REG3_OTPA);
				SET_REG_BIT(ctlreg_3, KXTF9_CTRL_REG3_OTPB);
			}
			else if (dTILT->ODR == 12) //12.5Hz
			{
				SET_REG_BIT(ctlreg_3, KXTF9_CTRL_REG3_OTPA);
			}
			else if (dTILT->ODR == 6) //6.3Hz
			{	
				SET_REG_BIT(ctlreg_3, KXTF9_CTRL_REG3_OTPB);
		    }	
		   else if(dTILT->ODR == 1) {}//1.5Hz	
 
//Timer
		    if(dTILT->timer!=0){kxtf9_tilt_timer((INT8U)dTILT->timer);}
//Angle		
		    if (dTILT->angle!= 0){		
		     	if (ACCEL_write_byte(KXTF9_I2C_TILT_ANGLE, (INT8U)dTILT->angle) != TRUE){status = FALSE;}
		    }

//hysteresis		
		     if (dTILT->hysteresis != 0)
		     {	
			    if (ACCEL_read_byte(KXTF9_I2C_HYST_SET, &read_temp_hyst, 1) == TRUE)
			    {
				   //write_temp_hyst = read_temp_hyst & (INT8U)dTILT->hysteresis;
					write_temp_hyst = (read_temp_hyst & 0xE0) | (INT8U)dTILT->hysteresis;

				   if (ACCEL_write_byte(KXTF9_I2C_HYST_SET, write_temp_hyst) != TRUE){status = FALSE;}
			    }
			    else
			    {
				    status = FALSE;
			    }
		      }//hysteresis
	   }  //dTILT->enable

/************************    WUF    **************************/	
	if (dWUF->enable == 1)
	{
		SET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_WUFE);	/* set WUFE to enable the wake up function */

//Timer	
		kxtf9_motion_wuf_timer((INT8U)dWUF->timer);
		
//Threshold		
		/* changing WUF Threshold is optional; factory default is 0.5g */
		if (dWUF->thresh != 0)
		{			
			if (ACCEL_write_byte(KXTF9_I2C_WUF_THRESH, (INT8U)dWUF->thresh) != TRUE){status = FALSE;}
		}

//ODR						
		if (dWUF->ODR == 200) //200Hz
		{	/* set OWUFA and OWUFB according to dWUF->ODR */
			SET_REG_BIT(ctlreg_3, KXTF9_CTRL_REG3_OWUFA);
			SET_REG_BIT(ctlreg_3, KXTF9_CTRL_REG3_OWUFB);
		}
		else if (dWUF->ODR == 100) //100Hz
		{
			SET_REG_BIT(ctlreg_3, KXTF9_CTRL_REG3_OWUFA);
		}
		else if (dWUF->ODR == 50) //50Hz
		{
			SET_REG_BIT(ctlreg_3, KXTF9_CTRL_REG3_OWUFB);
		}
		else if(dWUF->ODR == 25){}
			
	}

		
/************************    TDT    **************************/		
	
    if(dTDT->enable == 1)
    {	     
	   SET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_TDTE);	/* set WUFE to enable the wake up function */ 

//timer
	   if(dTDT->tdt_timer > 0){ kxtf9_tdt_timer(dTDT->tdt_timer);}
	   if(dTDT->tdt_tap_timer > 0) {kxtf9_tdt_tap_timer(dTDT->tdt_tap_timer);}
       if(dTDT->tdt_total_timer > 0){kxtf9_tdt_total_timer(dTDT->tdt_total_timer);}
       if(dTDT->tdt_latency_timer > 0){kxtf9_tdt_latency_timer(dTDT->tdt_latency_timer);}
       if(dTDT->tdt_window_timer > 0){ kxtf9_tdt_window_timer(dTDT->tdt_window_timer);}

//Threshold

	   if(dTDT->tdt_h_thresh !=0){kxtf9_tdt_h_thresh(dTDT->tdt_h_thresh);}
	   if(dTDT->tdt_l_thresh !=0){kxtf9_tdt_l_thresh(dTDT->tdt_l_thresh);}
       
//ODR
	   if(dTDT->ODR == 40) //400Hz
	   { 	
          SET_REG_BIT(ctlreg_3, KXTF9_CTRL_REG3_OTDTA);	
	       SET_REG_BIT(ctlreg_3, KXTF9_CTRL_REG3_OTDTB); 
	   }
	   else if(dTDT->ODR == 20) //200Hz
	   {
   	   SET_REG_BIT(ctlreg_3, KXTF9_CTRL_REG3_OTDTA);	   	   
	   }
	   else if(dTDT->ODR == 10) //100Hz
	   {
  	   SET_REG_BIT(ctlreg_3, KXTF9_CTRL_REG3_OTDTB);  	   	   
	   }
	   else if(dTDT->ODR == 5){}// 50Hz
   	  
   }
   	
/************************    Interrupt    **************************/	
	
	//Setting all bits for Control Register 2 if orientation direction masks are enabled (eliminates register write)
	if (dMASK->Bits.tilt_mask_enable == 1){
		if (dMASK->Bits.mask_le == 1){SET_REG_BIT(ctlreg_2, KXTF9_CTRL_REG2_LEM);}
		if (dMASK->Bits.mask_ri == 1){SET_REG_BIT(ctlreg_2, KXTF9_CTRL_REG2_RIM);}
		if (dMASK->Bits.mask_do == 1){SET_REG_BIT(ctlreg_2, KXTF9_CTRL_REG2_DOM);}
		if (dMASK->Bits.mask_up == 1){SET_REG_BIT(ctlreg_2, KXTF9_CTRL_REG2_UPM);}
		if (dMASK->Bits.mask_fd == 1){SET_REG_BIT(ctlreg_2, KXTF9_CTRL_REG2_FDM);}
		if (dMASK->Bits.mask_fu == 1){SET_REG_BIT(ctlreg_2, KXTF9_CTRL_REG2_FUM);}

		if (ACCEL_write_byte(KXTF9_I2C_CTRL_REG2, ctlreg_2) != TRUE){status = FALSE;}
	}

	//Setting all bits for Control Register 2 if orientation direction masks are enabled (eliminates register write)
	if (dMASK->Bits.tdt_mask_enable == 1){
		if (dMASK->Bits.mask_le == 1){SET_REG_BIT(int_ctlreg_3, KXTF9_CTRL_REG3_LEM);}
		if (dMASK->Bits.mask_ri == 1){SET_REG_BIT(int_ctlreg_3, KXTF9_CTRL_REG3_RIM);}
		if (dMASK->Bits.mask_do == 1){SET_REG_BIT(int_ctlreg_3, KXTF9_CTRL_REG3_DOM);}
		if (dMASK->Bits.mask_up == 1){SET_REG_BIT(int_ctlreg_3, KXTF9_CTRL_REG3_UPM);}
		if (dMASK->Bits.mask_fd == 1){SET_REG_BIT(int_ctlreg_3, KXTF9_CTRL_REG3_FDM);}
		if (dMASK->Bits.mask_fu == 1){SET_REG_BIT(int_ctlreg_3, KXTF9_CTRL_REG3_FUM);}

		if (ACCEL_write_byte(KXTF9_I2C_CTRL_REG3, int_ctlreg_3) != TRUE){status = FALSE;}
	}
	
	//Setting all bits for Interrupt Control Register 2 if Axis Masking is enabled (eliminates register write)
	if(dMASK->Bits.axis_mask_enable == 1){
	    if (dMASK->Bits.x_mask == 1){SET_REG_BIT(int_ctlreg_2, KXTF9_INT_CTRL_REG2_XBW);}
		if (dMASK->Bits.y_mask == 1){SET_REG_BIT(int_ctlreg_2, KXTF9_INT_CTRL_REG2_YBW);}
		if (dMASK->Bits.z_mask == 1){SET_REG_BIT(int_ctlreg_2, KXTF9_INT_CTRL_REG2_ZBW);}
		
		if (ACCEL_write_byte(KXTF9_I2C_INT_CTRL_REG2, int_ctlreg_2) != TRUE){status = FALSE;}
    }
    
    
	//Setting all bits for Interrupt Control Register 1 if Interrupt is enabled (eliminates register read)
	    if (dINT->enable == 1){SET_REG_BIT(int_ctlreg_1, KXTF9_INT_CTRL_REG1_IEN);}
		if (dINT->polarity == 1){SET_REG_BIT(int_ctlreg_1, KXTF9_INT_CTRL_REG1_IEA);}
		if (dINT->latch == 1){SET_REG_BIT(int_ctlreg_1, KXTF9_INT_CTRL_REG1_IEL);}
		if (dINT->alt_latch == 1){SET_REG_BIT(int_ctlreg_1, KXTF9_INT_CTRL_REG1_ULMB);}
		
		if (ACCEL_write_byte(KXTF9_I2C_INT_CTRL_REG1, int_ctlreg_1) != TRUE){status = FALSE;}

		if (ACCEL_write_byte(KXTF9_I2C_CTRL_REG3, ctlreg_3) != TRUE){status = FALSE;}	     /* Write ODRs settings*/	
      if (ACCEL_write_byte(KXTF9_I2C_CTRL_REG1, ctlreg_1) != TRUE){status = FALSE;}		 /* Write Control Register 1 last when enabling the part */
            

	return status;
}
/*==================================================================================================
FUNCTION: 		    KXTF9_enable_outputs
DESCRIPTION:    	This function enables accelerometer outputs. 
ARGUMENTS PASSED:  	None
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	Accelerometer outputs enabled
==================================================================================================*/
BOOLEAN kxtf9_enable_outputs(void)
{
	INT8U ctlreg_1 = 0;
	BOOLEAN status = TRUE;
	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG1, &ctlreg_1, 1) == TRUE)
	{
		SET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_PC1); /* sets PC1 bit to be in power up state */
		ACCEL_write_byte(KXTF9_I2C_CTRL_REG1, ctlreg_1);
	}
	else
	{	
		status = FALSE;
	}
	
	return status;
}
/*==================================================================================================
FUNCTION:         KXTF9_sleep
DESCRIPTION:      This function places the Sensor into a standby state while retaining current register values.
ARGUMENTS PASSED: None
return VALUE:     TRUE = pass; FALSE = fail
PRE-CONDITIONS:   None
POST-CONDITIONS:  None
IMPORTANT NOTES:  None
==================================================================================================*/
BOOLEAN kxtf9_sleep(void)
{
	INT8U ctlreg_1 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG1, &ctlreg_1, 1) == TRUE )
	{
		UNSET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_PC1); /* unset the PC1 bit to disable the accelerometer */
		ACCEL_write_byte(KXTF9_I2C_CTRL_REG1, ctlreg_1);
	}
	else
	{
		status = FALSE;
	}

	return status;
}
/*==================================================================================================
FUNCTION:         KXTF9_reset
DESCRIPTION:      this function issues a software reset to Kionix accelerometer.
ARGUMENTS PASSED: None
return VALUE:     TRUE = pass; FALSE = fail
PRE-CONDITIONS:   None
POST-CONDITIONS:  None
IMPORTANT NOTES:  When called, will have a delay of approximately 50mS because sensor is loading default values
==================================================================================================*/
BOOLEAN kxtf9_reset(void)
{
	INT8U ctrl_reg3 = 0;
	BOOLEAN status = TRUE;
	static uchar Cnt = 0;

	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG3, &ctrl_reg3, 1) == TRUE)
	{
		SET_REG_BIT(ctrl_reg3, KXTF9_CTRL_REG3_SRST);
		ACCEL_write_byte(KXTF9_I2C_CTRL_REG3, ctrl_reg3);
	}
	else
	{
		status = FALSE;
	}
    
    //wait until CTRL_REG3_SRST becomes low
    do{
	 		  if (Cnt++ > 3)		// 读3次
			  {
			  		Cnt = 0; 
					status = FALSE;
					break;
				}
           //read back CTRL_REG3_SRST
           ACCEL_read_byte(KXTF9_I2C_CTRL_REG3,&ctrl_reg3, 1);
			  os_dly_wait(1);

			  
          
    }while(ctrl_reg3 & KXTF9_CTRL_REG3_SRST); //mask Bit7 to get SRST bit 
    
	return status;
}
/*==================================================================================================
FUNCTION:         KXTF9_enable_tilt
DESCRIPTION:      This function enables the tilt/rotation function
ARGUMENTS PASSED: None
return VALUE:     TRUE = pass; FALSE = fail
PRE-CONDITIONS:   None
POST-CONDITIONS:  None
IMPORTANT NOTES:  None
==================================================================================================*/
BOOLEAN kxtf9_enable_tilt(void)
{
	INT8U ctlreg_1 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG1, &ctlreg_1, 1) == TRUE)
	{
		SET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_TPE); /* sets TPE bit to enable screen rotation */
		ACCEL_write_byte(KXTF9_I2C_CTRL_REG1, ctlreg_1);
	}
	else
	{
		status = FALSE;
	}

	return status;
}
/*==================================================================================================
FUNCTION:         KXTF9_disable_tilt
DESCRIPTION:      This function disables the screen rotation function
ARGUMENTS PASSED: None
return VALUE:     TRUE = pass; FALSE = fail
PRE-CONDITIONS:   None
POST-CONDITIONS:  None
IMPORTANT NOTES:  None
==================================================================================================*/
BOOLEAN kxtf9_disable_tilt(void)
{
	INT8U ctlreg_1 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG1, &ctlreg_1, 1) == TRUE)
	{
		UNSET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_TPE); /* unset TPE bit to disable screen rotation */
		ACCEL_write_byte(KXTF9_I2C_CTRL_REG1, ctlreg_1);
	}
	else
	{
		status = FALSE;
	}

	return status;
}

/*==================================================================================================
FUNCTION:         KXTF9_tilt_timer
DESCRIPTION:      This function sets the number of screen rotation debounce samples
ARGUMENTS PASSED: Tilt timer vlue from 0-255
return VALUE:     None
PRE-CONDITIONS:   None
POST-CONDITIONS:  None
IMPORTANT NOTES:  None
==================================================================================================*/
BOOLEAN kxtf9_tilt_timer(INT8U tilt_timer)
{
	BOOLEAN status;

	if (ACCEL_write_byte(KXTF9_I2C_TILT_TIMER, tilt_timer) == TRUE)
	{
		status = TRUE;
	}
	else
	{
		status = FALSE;
	}

	return status;
}
/*==================================================================================================
FUNCTION:         KXTF9_motion_wuf_timer
DESCRIPTION:      This function sets the number of wake up debounce samples
ARGUMENTS PASSED: Wuf timer vlue from 0-255
return VALUE:     None
PRE-CONDITIONS:   None
POST-CONDITIONS:  None
IMPORTANT NOTES:  None
==================================================================================================*/
BOOLEAN kxtf9_motion_wuf_timer(INT8U wuf_timer)
{
	BOOLEAN status;

	if (ACCEL_write_byte(KXTF9_I2C_WUF_TIMER, wuf_timer) == TRUE)
	{
		status = TRUE;
	}
	else
	{
		status = FALSE;
	}

	return status;
}
/*==================================================================================================
FUNCTION:         KXTF9_motion_wuf_threshold
DESCRIPTION:      This function writes a wake up threshold value to the register
ARGUMENTS PASSED: wuf threshold value in counts from 0-255
return VALUE:     TRUE = pass; FALSE = fail
PRE-CONDITIONS:   None
POST-CONDITIONS:  None
IMPORTANT NOTES:  None
==================================================================================================*/
BOOLEAN kxtf9_motion_wuf_threshold(INT8U wuf_threshold)
{
	BOOLEAN status;

	if (ACCEL_write_byte(KXTF9_I2C_WUF_THRESH, wuf_threshold) == TRUE)
	{
		status = TRUE;
	}
	else
	{
		status = FALSE;
	}
	
	return status;
}

/*==================================================================================================
FUNCTION:          KXTF9_set_tilt_angle
DESCRIPTION:       this function sets the tilt angle that is used to detect the transition from 
                   Face-up/Face-down states to Screen Rotation states.
ARGUMENTS PASSED:  tilt angle count 0-64   (0 Deg to 90 deg)
return VALUE:      TRUE = pass; FALSE = fail
PRE-CONDITIONS:    None
POST-CONDITIONS:   None
IMPORTANT NOTES:   None
==================================================================================================*/
BOOLEAN kxtf9_set_tilt_angle(INT8U tilt_angle)
{
    BOOLEAN status;

	if(ACCEL_write_byte(KXTF9_I2C_TILT_ANGLE , tilt_angle) == TRUE)
	{
		status = TRUE;
	}
	else
	{
		status = FALSE;
	}
	
	return status;        
}
/*==================================================================================================
FUNCTION:          KXTF9_set_hysteresis
DESCRIPTION:       This function sets hysteresis that is placed in between the Screen Rotation States.
ARGUMENTS PASSED:  tilt angle count 0-15   (3 Deg to 30 deg). Please refer to Sepecification notes.
return VALUE:      TRUE = pass; FALSE = fail
PRE-CONDITIONS:    None
POST-CONDITIONS:   None
IMPORTANT NOTES:   None
==================================================================================================*/
BOOLEAN kxtf9_set_hysteresis(INT8U hysteresis)
{
    BOOLEAN status;
    INT8U read_temp_hyst;

    ACCEL_read_byte(KXTF9_I2C_HYST_SET, &read_temp_hyst, 1);
    //read_temp_hyst= read_temp_hyst & 0xC0;    //Mask and preserve Bit 7  use for kxte9

	 //hysteresis = hysteresis | read_temp_hyst;  //Copy Bit 7 and Bit 7 and Bit 8 to the new hysteresis value.

	 // modify by xzb
	 read_temp_hyst= read_temp_hyst & 0xE0;    //preserve Bit 7 and Bit 6 and Bit5	 use for kxtf9

    hysteresis = hysteresis | read_temp_hyst;  //Copy Bit 7 and Bit 6 and Bit 5 to the new hysteresis value.
	  // end

	if (ACCEL_write_byte(KXTF9_I2C_HYST_SET , hysteresis) == TRUE)
	{
		status = TRUE;
	}
	else
	{
		status = FALSE;
	}
	return status;               
}
/*==================================================================================================
FUNCTION:         KXTF9_read_previous_tilt_position
DESCRIPTION:      This function reads the previous tilt position register 
ARGUMENTS PASSED: None
return VALUE:     TRUE = pass; FALSE = fail
PRE-CONDITIONS:   None
POST-CONDITIONS:  None
IMPORTANT NOTES:  None
==================================================================================================*/
BOOLEAN kxtf9_read_previous_tilt_position(INT8U *previous_position)
{
	BOOLEAN status;

	status = ACCEL_read_byte(KXTF9_I2C_TILT_POS_PRE, previous_position, 1);

	return status;
}
/*==================================================================================================
FUNCTION:         KXTF9_read_current_tilt_position
DESCRIPTION:      This function reads the current tilt position register 
ARGUMENTS PASSED: None
return VALUE:     TRUE = pass; FALSE = fail
PRE-CONDITIONS:   None
POST-CONDITIONS:  None
IMPORTANT NOTES:  None
==================================================================================================*/
BOOLEAN kxtf9_read_current_tilt_position(INT8U *current_position)
{
	BOOLEAN status;
	
	status = ACCEL_read_byte(KXTF9_I2C_TILT_POS_CUR, current_position, 1);

	return status;
}
/*==================================================================================================
FUNCTION:            KXTF9_read_interrupt_status
DESCRIPTION:         this function reads the physical pin interrupt status
ARGUMENTS PASSED:    None
return VALUE:        TRUE if interrupt active, FALSE if interrupt inactive
PRE-CONDITIONS:      None
POST-CONDITIONS:     None
IMPORTANT NOTES:     None
==================================================================================================*/	
BOOLEAN kxtf9_read_interrupt_status(void)
{
	BOOLEAN interrupt_status; 
	INT8U status_reg;
    
    if (ACCEL_read_byte(KXTF9_I2C_STATUS_REG, &status_reg, 1) == TRUE)
	{
		if ((status_reg & 0x10) == 0x00)
		{
			interrupt_status = FALSE;
		}
		else
		{
			interrupt_status = TRUE;
		}
	}
	else
	{
		interrupt_status = FALSE;
	}
 
	
	return interrupt_status;
}
/*==================================================================================================
FUNCTION:         KXTF9_service_interrupt
DESCRIPTION:      This function clears the IRQ status
ARGUMENTS PASSED: Pointers to hold INT_SRC_REG2
return VALUE:     TRUE if an interrupt had been pending.
PRE-CONDITIONS:   None
POST-CONDITIONS:  None
IMPORTANT NOTES:  This function reads INT_SRC_REG2 and clears INT_SRC_REG2
==================================================================================================*/
BOOLEAN kxtf9_service_interrupt(INT8U  *source_of_interrupt)
{
	BOOLEAN return_status = FALSE;
	INT8U dummy = 0;

	// read the interrupt source register
	if (ACCEL_read_byte(KXTF9_I2C_INT_SRC_REG2, (INT8U *)source_of_interrupt, 1) == TRUE)
	{
		// clear the interrupt source information along with interrupt pin
		if (ACCEL_read_byte(KXTF9_I2C_INT_REL, &dummy, 1) == TRUE)
		{
				return_status = TRUE;
		}
		else
		{
			return_status = FALSE;
		}
	}
	else
	{
		return_status = FALSE;
	}	

	return return_status;
}
/*==================================================================================================
FUNCTION:         KXTF9_motion_act_mask_z
DESCRIPTION:      This function masks z-axis from activity engine
ARGUMENTS PASSED: None
return VALUE:     TRUE = pass; FALSE = fail
PRE-CONDITIONS:   None
POST-CONDITIONS:  None
IMPORTANT NOTES:  None
==================================================================================================*/
BOOLEAN kxtf9_motion_act_mask_z(void)
{
	INT8U int_ctrl_reg2 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_INT_CTRL_REG2, &int_ctrl_reg2, 1) == TRUE)
	{
		SET_REG_BIT(int_ctrl_reg2, KXTF9_INT_CTRL_REG2_ZBW);
		ACCEL_write_byte(KXTF9_I2C_INT_CTRL_REG2, int_ctrl_reg2);
	}
	else
	{
		status = FALSE;
	}

	return status;
}
/*==================================================================================================
FUNCTION:          KXTF9_motion_act_unmask_z
DESCRIPTION:       This function unmasks z-axis from activity engine
ARGUMENTS PASSED:  None
return VALUE:      TRUE = pass; FALSE = fail
PRE-CONDITIONS:    None
POST-CONDITIONS:   None
IMPORTANT NOTES:   None
==================================================================================================*/
BOOLEAN kxtf9_motion_act_unmask_z(void)
{
	INT8U int_ctrl_reg2 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_INT_CTRL_REG2, &int_ctrl_reg2, 1) == TRUE)
	{
		UNSET_REG_BIT(int_ctrl_reg2, KXTF9_INT_CTRL_REG2_ZBW);
		ACCEL_write_byte(KXTF9_I2C_INT_CTRL_REG2, int_ctrl_reg2);
	}
	else
	{
		status = FALSE;
	}

	return status;
}
/*==================================================================================================
FUNCTION:         KXTF9_motion_act_mask_y
DESCRIPTION:      This function masks y-axis from activity engine
ARGUMENTS PASSED: None
return VALUE:     TRUE = pass; FALSE = fail
PRE-CONDITIONS:   None
POST-CONDITIONS:  None
IMPORTANT NOTES:  None
==================================================================================================*/
BOOLEAN kxtf9_motion_act_mask_y(void)
{
	INT8U int_ctrl_reg2 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_INT_CTRL_REG2, &int_ctrl_reg2, 1) == TRUE)
	{
		SET_REG_BIT(int_ctrl_reg2, KXTF9_INT_CTRL_REG2_YBW);
		ACCEL_write_byte(KXTF9_I2C_INT_CTRL_REG2, int_ctrl_reg2);
	}
	else
	{
		status = FALSE;
	}

	return status;
}
/*==================================================================================================
FUNCTION:          KXTF9_motion_act_unmask_y
DESCRIPTION:       This function unmasks y-axis from activity engine
ARGUMENTS PASSED:  None
return VALUE:      TRUE = pass; FALSE = fail
PRE-CONDITIONS:    None
POST-CONDITIONS:   None
IMPORTANT NOTES:   None
==================================================================================================*/
BOOLEAN kxtf9_motion_act_unmask_y(void)
{
	INT8U int_ctrl_reg2 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_INT_CTRL_REG2, &int_ctrl_reg2, 1) == TRUE)
	{
		UNSET_REG_BIT(int_ctrl_reg2, KXTF9_INT_CTRL_REG2_YBW);
		ACCEL_write_byte(KXTF9_I2C_INT_CTRL_REG2, int_ctrl_reg2);
	}
	else
	{
		status = FALSE;
	}

	return status;
}
/*==================================================================================================
FUNCTION:          KXTF9_motion_act_mask_x
DESCRIPTION:       This function unmasks x-axis from activity engine
ARGUMENTS PASSED:  None
return VALUE:      TRUE = pass; FALSE = fail
PRE-CONDITIONS:    None
POST-CONDITIONS:   None
IMPORTANT NOTES:   None
==================================================================================================*/
BOOLEAN kxtf9_motion_act_mask_x(void)
{
	INT8U int_ctrl_reg2 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_INT_CTRL_REG2, &int_ctrl_reg2, 1) == TRUE)
	{
		SET_REG_BIT(int_ctrl_reg2, KXTF9_INT_CTRL_REG2_XBW);
		ACCEL_write_byte(KXTF9_I2C_INT_CTRL_REG2, int_ctrl_reg2);
	}
	else
	{
		status = FALSE;
	}

	return status;
}
/*==================================================================================================
FUNCTION:           KXTF9_motion_act_unmask_x
DESCRIPTION:        This function unmasks x-axis from activity engine
ARGUMENTS PASSED:   None
return VALUE:       TRUE = pass; FALSE = fail
PRE-CONDITIONS:     None
POST-CONDITIONS:    None
IMPORTANT NOTES:    None
==================================================================================================*/
BOOLEAN kxtf9_motion_act_unmask_x(void)
{
	INT8U int_ctrl_reg2 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_INT_CTRL_REG2, &int_ctrl_reg2, 1) == TRUE)
	{
		UNSET_REG_BIT(int_ctrl_reg2, KXTF9_INT_CTRL_REG2_XBW);
		ACCEL_write_byte(KXTF9_I2C_INT_CTRL_REG2, int_ctrl_reg2);
	}
	else
	{
		status = FALSE;
	}

	return status;
}
/*==================================================================================================
FUNCTION:            KXTF9_tilt_pos_mask_fu
DESCRIPTION:         This function masks face up in the screen rotation function
ARGUMENTS PASSED:    None
return VALUE:        TRUE = pass; FALSE = fail
PRE-CONDITIONS:      None
POST-CONDITIONS:     None
IMPORTANT NOTES:     None
==================================================================================================*/
BOOLEAN kxtf9_tilt_pos_mask_fu(void)
{
	INT8U ctrl_reg2 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG2, &ctrl_reg2, 1) == TRUE)
	{
		SET_REG_BIT(ctrl_reg2, KXTF9_CTRL_REG2_FUM);
		ACCEL_write_byte(KXTF9_I2C_CTRL_REG2, ctrl_reg2);
	}
	else
	{
		status = FALSE;
	}

	return status;
}
/*==================================================================================================
FUNCTION:            KXTF9_tilt_pos_mask_fd
DESCRIPTION:         This function masks face down in the screen rotation function
ARGUMENTS PASSED:    None
return VALUE:        TRUE = pass; FALSE = fail
PRE-CONDITIONS:      None
POST-CONDITIONS:     None
IMPORTANT NOTES:     None
==================================================================================================*/
BOOLEAN kxtf9_tilt_pos_mask_fd(void)
{
	INT8U ctrl_reg2 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG2, &ctrl_reg2, 1) == TRUE)
	{
		SET_REG_BIT(ctrl_reg2, KXTF9_CTRL_REG2_FDM);
		ACCEL_write_byte(KXTF9_I2C_CTRL_REG2, ctrl_reg2);
	}
	else
	{
		status = FALSE;
	}

	return status;
}
/*==================================================================================================
FUNCTION:            KXTF9_tilt_pos_mask_up
DESCRIPTION:         This function masks up state in the screen rotation function
ARGUMENTS PASSED:    None
return VALUE:        TRUE = pass; FALSE = fail
PRE-CONDITIONS:      None
POST-CONDITIONS:     None
IMPORTANT NOTES:     None
==================================================================================================*/
BOOLEAN kxtf9_tilt_pos_mask_up(void)
{
	INT8U ctrl_reg2 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG2, &ctrl_reg2, 1) == TRUE)
	{
		SET_REG_BIT(ctrl_reg2, KXTF9_CTRL_REG2_UPM);
		ACCEL_write_byte(KXTF9_I2C_CTRL_REG2, ctrl_reg2);
	}
	else
	{
		status = FALSE;
	}

	return status;
}
/*==================================================================================================
FUNCTION:            KXTF9_tilt_pos_mask_do
DESCRIPTION:         This function masks down state in the screen rotation function
ARGUMENTS PASSED:    None
return VALUE:        TRUE = pass; FALSE = fail
PRE-CONDITIONS:      None
POST-CONDITIONS:     None
IMPORTANT NOTES:     None
==================================================================================================*/
BOOLEAN kxtf9_tilt_pos_mask_do(void)
{
	INT8U ctrl_reg2 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG2, &ctrl_reg2, 1) == TRUE)
	{
		SET_REG_BIT(ctrl_reg2, KXTF9_CTRL_REG2_DOM);
		ACCEL_write_byte(KXTF9_I2C_CTRL_REG2, ctrl_reg2);
	}
	else
	{
		status = FALSE;
	}

	return status;
}
/*==================================================================================================
FUNCTION:            KXTF9_tilt_pos_mask_ri
DESCRIPTION:         This function masks right state in the screen rotation function
ARGUMENTS PASSED:    None
return VALUE         TRUE = pass; FALSE = fail
PRE-CONDITIONS:      None
POST-CONDITIONS:     None
IMPORTANT NOTES:     None
==================================================================================================*/
BOOLEAN kxtf9_tilt_pos_mask_ri(void)
{
	INT8U ctrl_reg2 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG2, &ctrl_reg2, 1) == TRUE)
	{
		SET_REG_BIT(ctrl_reg2, KXTF9_CTRL_REG2_RIM);
		ACCEL_write_byte(KXTF9_I2C_CTRL_REG2, ctrl_reg2);
	}
	else
	{
		status = FALSE;
	}

	return status;
}
/*==================================================================================================
FUNCTION:            KXTF9_tilt_pos_mask_le
DESCRIPTION:         This function masks left state in the screen rotation function
ARGUMENTS PASSED:    None
return VALUE:        TRUE = pass; FALSE = fail
PRE-CONDITIONS:      None
POST-CONDITIONS:     None
IMPORTANT NOTES:     None
==================================================================================================*/
BOOLEAN kxtf9_tilt_pos_mask_le(void)
{
	INT8U ctrl_reg2 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG2, &ctrl_reg2, 1) == TRUE)
	{
		SET_REG_BIT(ctrl_reg2, KXTF9_CTRL_REG2_LEM);
		ACCEL_write_byte(KXTF9_I2C_CTRL_REG2, ctrl_reg2);
	}
	else
	{
		status = FALSE;
	}

	return status;
}
/*==================================================================================================
FUNCTION:           KXTF9_tilt_pos_unmask_fu
DESCRIPTION:        This function unmasks face up in the screen rotation function
ARGUMENTS PASSED:   None
return VALUE:       TRUE = pass; FALSE = fail
PRE-CONDITIONS:     None
POST-CONDITIONS:    None
IMPORTANT NOTES:    None
==================================================================================================*/
BOOLEAN kxtf9_tilt_pos_unmask_fu(void)
{
	INT8U ctrl_reg2 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG2, &ctrl_reg2, 1) == TRUE)
	{
		UNSET_REG_BIT(ctrl_reg2, KXTF9_CTRL_REG2_FUM);
		ACCEL_write_byte(KXTF9_I2C_CTRL_REG2, ctrl_reg2);
	}
	else
	{
		status = FALSE;
	}

	return status;
}
/*==================================================================================================
FUNCTION:            KXTF9_tilt_pos_unmask_fd
DESCRIPTION:         This function unmasks face down in the screen rotation function
ARGUMENTS PASSED:    None
return VALUE:        TRUE = pass; FALSE = fail
PRE-CONDITIONS:      None
POST-CONDITIONS:     None
IMPORTANT NOTES:     None
==================================================================================================*/
BOOLEAN kxtf9_tilt_pos_unmask_fd(void)
{
	INT8U ctrl_reg2 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG2, &ctrl_reg2, 1) == TRUE)
	{
		UNSET_REG_BIT(ctrl_reg2, KXTF9_CTRL_REG2_FDM);
		ACCEL_write_byte(KXTF9_I2C_CTRL_REG2, ctrl_reg2);
	}
	else
	{
		status = FALSE;
	}

	return status;
}
/*==================================================================================================
FUNCTION:          KXTF9_tilt_pos_unmask_up
DESCRIPTION:       This function unmasks up state in the screen rotation function
ARGUMENTS PASSED:  None
return VALUE:      TRUE = pass; FALSE = fail
PRE-CONDITIONS:    None
POST-CONDITIONS:   None
IMPORTANT NOTES:   None
==================================================================================================*/
BOOLEAN kxtf9_tilt_pos_unmask_up(void)
{
	INT8U ctrl_reg2 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG2, &ctrl_reg2, 1) == TRUE)
	{
		UNSET_REG_BIT(ctrl_reg2, KXTF9_CTRL_REG2_UPM);
		ACCEL_write_byte(KXTF9_I2C_CTRL_REG2, ctrl_reg2);
	}
	else
	{
		status = FALSE;
	}

	return status;
}
/*==================================================================================================
FUNCTION:            KXTF9_tilt_pos_unmask_do
DESCRIPTION:         This function unmasks down state in the screen rotation function
ARGUMENTS PASSED:    None
return VALUE:        TRUE = pass; FALSE = fail
PRE-CONDITIONS:      None
POST-CONDITIONS:     None
IMPORTANT NOTES:     None
==================================================================================================*/
BOOLEAN kxtf9_tilt_pos_unmask_do(void)
{
	INT8U ctrl_reg2 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG2, &ctrl_reg2, 1) == TRUE)
	{
		UNSET_REG_BIT(ctrl_reg2, KXTF9_CTRL_REG2_DOM);
		ACCEL_write_byte(KXTF9_I2C_CTRL_REG2, ctrl_reg2);
	}
	else
	{
		status = FALSE;
	}

	return status;
}
/*==================================================================================================
FUNCTION:          KXTF9_tilt_pos_unmask_ri
DESCRIPTION:       This function unmasks right state in the screen rotation function
ARGUMENTS PASSED:  None
return VALUE:      TRUE = pass; FALSE = fail
PRE-CONDITIONS:    None
POST-CONDITIONS:   None
IMPORTANT NOTES:   None
==================================================================================================*/
BOOLEAN kxtf9_tilt_pos_unmask_ri(void)
{
	INT8U ctrl_reg2 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG2, &ctrl_reg2, 1) == TRUE)
	{
		UNSET_REG_BIT(ctrl_reg2, KXTF9_CTRL_REG2_RIM);
		ACCEL_write_byte(KXTF9_I2C_CTRL_REG2, ctrl_reg2);
	}
	else
	{
		status = FALSE;
	}

	return status;
}
/*==================================================================================================
FUNCTION:          KXTF9_tilt_pos_unmask_le
DESCRIPTION:       This function unmasks left state in the screen rotation function
ARGUMENTS PASSED:  None
return VALUE:      TRUE = pass; FALSE = fail
PRE-CONDITIONS:    None
POST-CONDITIONS:   None
IMPORTANT NOTES:   None
==================================================================================================*/
BOOLEAN kxtf9_tilt_pos_unmask_le(void)
{
	INT8U ctrl_reg2 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG2, &ctrl_reg2, 1) == TRUE)
	{
		UNSET_REG_BIT(ctrl_reg2, KXTF9_CTRL_REG2_LEM);
		ACCEL_write_byte(KXTF9_I2C_CTRL_REG2, ctrl_reg2);
	}
	else
	{
		status = FALSE;
	}

	return status;
}
/**************************************************************************************************
*                                                                                                 *
*                                      KXTF9-SPECIFIC FUNCTIONS                                   *
*                                                                                                 * 
***************************************************************************************************/
/*==================================================================================================
FUNCTION: 		    KXTF9_enable_all
DESCRIPTION:    	This function enables all engines. 
ARGUMENTS PASSED:   None
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	All engines enabled
==================================================================================================*/
BOOLEAN kxtf9_enable_all(void)
{
	INT8U ctlreg_1 = 0;
	BOOLEAN status = TRUE;
	
	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG1, &ctlreg_1, 1) == TRUE)
	{
		SET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_PC1); /* set PC1 to enable the accelerometer outputs */
		SET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_TPE); /* set TPE bit to enable the tilt function */
		SET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_WUFE); /* set WUFE to enable the wake up function */
		SET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_TDTE); /* set TDTE to enable tap function on KXTF9 */
		ACCEL_write_byte(KXTF9_I2C_CTRL_REG1, ctlreg_1);
	}
	else
	{
			status = FALSE;
	}
	
	return status;
}
/*==================================================================================================
FUNCTION: 		    KXTF9_disable_all
DESCRIPTION:    	This function disables all engines. 
ARGUMENTS PASSED:   None
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	All engines disabled
==================================================================================================*/
BOOLEAN kxtf9_disable_all(void)
{
	INT8U ctlreg_1 = 0;
	BOOLEAN status = TRUE;
	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG1, &ctlreg_1, 1) == TRUE)
	{
		UNSET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_PC1); /* unset the PC1 bit to disable the accelerometer outputs */
		UNSET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_TPE); /* unset the TPE bit to disable the tilt function */
		UNSET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_WUFE); /* unset WUFE to disable wake up function */
		UNSET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_TDTE); /* unset the TDTE bit to disable the tap function */
		ACCEL_write_byte(KXTF9_I2C_CTRL_REG1, ctlreg_1);
	}
	else
	{
			status = FALSE;
	}		
	return status;
}
/*==================================================================================================
FUNCTION: 		KXTF9_enable_motion_act_function
DESCRIPTION:    	This function enables the wake up function. 
ARGUMENTS PASSED:   None
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	Wake up function is enabled
==================================================================================================*/
BOOLEAN kxtf9_enable_motion_act_function(void)
{
	INT8U ctlreg_1 = 0;
	BOOLEAN status = TRUE;
	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG1, &ctlreg_1, 1) == TRUE)
	{
		SET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_WUFE); /* set WUFE bit to enable the wake up function */
		ACCEL_write_byte(KXTF9_I2C_CTRL_REG1, ctlreg_1);
	}
	else
	{
		status = FALSE;
	}
	
	return status;
}
/*==================================================================================================
FUNCTION: 		    KXTF9_disable_motion_act_function
DESCRIPTION:    	This function disables the wake up function. 
ARGUMENTS PASSED:   	None
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	Wake up function is disabled
==================================================================================================*/
BOOLEAN kxtf9_disable_motion_act_function(void)
{
	INT8U ctlreg_1 = 0;
	BOOLEAN status = TRUE;
	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG1, &ctlreg_1, 1) == TRUE)
	{
		UNSET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_WUFE); /* unset the WUFE bit to disable the wake up function */
		ACCEL_write_byte(KXTF9_I2C_CTRL_REG1, ctlreg_1);
	}
	else
	{
		status = FALSE;
	}
	
	return status;
}
/*==================================================================================================
FUNCTION: 		    KXTF9_enable_tap_detection
DESCRIPTION:    	This function enables the tap detection function. 
ARGUMENTS PASSED:   None
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	Tap detection is enabled
==================================================================================================*/
BOOLEAN kxtf9_enable_tap_detection(void)
{
	INT8U ctlreg_1 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG1, &ctlreg_1, 1) == TRUE){
		SET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_TDTE); /* set TDTE bit to enable tap function */
		ACCEL_write_byte(KXTF9_I2C_CTRL_REG1, ctlreg_1);
	}
	else	status = FALSE;
	return status;
}
/*==================================================================================================
FUNCTION: 		KXTF9_disable_tap_detection
DESCRIPTION:    	This function disables the tap detection function. 
ARGUMENTS PASSED:   	None
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	Tap detection is disabled
==================================================================================================*/
BOOLEAN kxtf9_disable_tap_detection(void)
{
	INT8U ctlreg_1 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG1, &ctlreg_1, 1) == TRUE){
		UNSET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_TDTE); /* unset the TDTE bit to disable tap function */
		ACCEL_write_byte(KXTF9_I2C_CTRL_REG1, ctlreg_1);
	}
	else	status = FALSE;
	return status;
}
/*==================================================================================================
FUNCTION: 		    KXTF9_read_single_tap_status
DESCRIPTION:    	This function reads INT_SRC_REG2 to determine whether a single tap event
			        occurred. 
ARGUMENTS PASSED:   None
RETURN VALUE:   	TRUE = single tap occurred; FALSE = single tap did not occur
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	None
==================================================================================================*/
BOOLEAN kxtf9_read_single_tap_status(void)
{
	BOOLEAN single_tap; 
	INT8U src_reg2;

	if (ACCEL_read_byte(KXTF9_I2C_INT_SRC_REG2, &src_reg2, 1) == TRUE){
		if((src_reg2 & 0x0C) == 0x04){
			single_tap = TRUE;
		}
		else	single_tap = FALSE;
	}
	else	single_tap = FALSE;
	return single_tap;
}
/*==================================================================================================
FUNCTION: 		KXTF9_read_double_tap_status
DESCRIPTION:    	This function reads INT_SRC_REG2 to determine whether a double tap event
			occurred.
ARGUMENTS PASSED:   	None
RETURN VALUE:   	0 = double tap occurred; 1 = double tap did not occur
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	None
==================================================================================================*/
BOOLEAN kxtf9_read_double_tap_status(void)
{
	BOOLEAN double_tap; 
	INT8U src_reg2;

	if (ACCEL_read_byte(KXTF9_I2C_INT_SRC_REG2, &src_reg2, 1) == TRUE){
		if((src_reg2 & 0x0C) == 0x08){
			double_tap = TRUE;
		}
		else	double_tap = FALSE;
	}
	else	double_tap = FALSE;
	return double_tap;
}
/*==================================================================================================
FUNCTION: 		    KXTF9_set_G_range
DESCRIPTION:     	This function sets the accelerometer G range. 
ARGUMENTS PASSED:   range variable; 2, 4, or 8
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	G range is set according to range
==================================================================================================*/
BOOLEAN kxtf9_set_G_range(int range)
{
	INT8U ctlreg_1 = 0;

	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG1, &ctlreg_1, 1) != TRUE){
		return FALSE;
	}
	switch (range){
	case 2:		/* set G-range to 2g */
		UNSET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_GSEL1);
		UNSET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_GSEL0);
		break;
	case 4:		/* set G-range to 4g */
		UNSET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_GSEL1);
		SET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_GSEL0);
		break;
	case 8:		/* set G-range to 8g */
		SET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_GSEL1);
		UNSET_REG_BIT(ctlreg_1, KXTF9_CTRL_REG1_GSEL0);
		break;
	default:
		return FALSE;
	}
	ACCEL_write_byte(KXTF9_I2C_CTRL_REG1, ctlreg_1);
	return TRUE;
}
/*==================================================================================================
FUNCTION: 		    KXTF9_set_resolution
DESCRIPTION:    	This function sets the resolution of the accelerometer outputs. 
ARGUMENTS PASSED:   resolution (8-bit or 12-bit)
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	Accelerometer resolution is set according to resolution
==================================================================================================*/
BOOLEAN kxtf9_set_resolution(int resolution)
{
	INT8U ctrl_reg1 = 0;	

	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG1, &ctrl_reg1, 1) != TRUE){
		return FALSE;
	}
	switch (resolution){
	case 8:		/* set resolution to 8 bits */
		UNSET_REG_BIT(ctrl_reg1, KXTF9_CTRL_REG1_RES);		
		break;
	case 12:	/* set resolution to 12 bits */
		SET_REG_BIT(ctrl_reg1, KXTF9_CTRL_REG1_RES);
		break;
	default:
		return FALSE;
	}
	ACCEL_write_byte(KXTF9_I2C_CTRL_REG1, ctrl_reg1);
	return TRUE;
}
/*==================================================================================================
FUNCTION: 		    KXTF9_int_alt_disable
DESCRIPTION:    	This function disables the alternate unlatched response for the physical 
			        interrupt pin. 
ARGUMENTS PASSED:   None
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	Alternate unlatched response for physical interrupt disabled
==================================================================================================*/
BOOLEAN kxtf9_int_alt_disable(void)
{
	INT8U int_ctrl_reg1 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_INT_CTRL_REG1, &int_ctrl_reg1, 1) == TRUE){
		UNSET_REG_BIT(int_ctrl_reg1, KXTF9_INT_CTRL_REG1_ULMB);
		ACCEL_write_byte(KXTF9_I2C_INT_CTRL_REG1, int_ctrl_reg1);
	}
	else	status = FALSE;
	return status;
}
/*==================================================================================================
FUNCTION: 		    KXTF9_int_alt_enable
DESCRIPTION:    	This function enables the alternate unlatched response for the physical 
			        interrupt pin. 
ARGUMENTS PASSED:  	None
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	Alternate unlatched response for physical interrupt enabled
==================================================================================================*/
BOOLEAN kxtf9_int_alt_enable(void)
{
	INT8U int_ctrl_reg1 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_INT_CTRL_REG1, &int_ctrl_reg1, 1) == TRUE){
		SET_REG_BIT(int_ctrl_reg1, KXTF9_INT_CTRL_REG1_ULMB);
		ACCEL_write_byte(KXTF9_I2C_INT_CTRL_REG1, int_ctrl_reg1);
	}
	else	status = FALSE;
	return status;
}
/*==================================================================================================
FUNCTION: 		    KXTF9_set_motion_wuf_odr_frequency
DESCRIPTION:    	This function sets the wuf ODR frequency.
ARGUMENTS PASSED:   frequency variable; 25, 50, 100, or 200 for KXTF9
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	ODR is set according to frequency
==================================================================================================*/
BOOLEAN kxtf9_set_motion_wuf_odr(int frequency)
{
	//INT8U ctlreg_1 = 0;
	INT8U ctlreg_3 = 0;	
	
	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG3, &ctlreg_3, 1) != TRUE)
	{
		return FALSE;
	}
	
	switch (frequency)
	{
		case 25:	/* set ODR to 25Hz */
			UNSET_REG_BIT(ctlreg_3, KXTF9_CTRL_REG3_OWUFA);
			UNSET_REG_BIT(ctlreg_3, KXTF9_CTRL_REG3_OWUFB);
			break;
		case 50:	/* set ODR t0 50 Hz */
			UNSET_REG_BIT(ctlreg_3, KXTF9_CTRL_REG3_OWUFA);
			SET_REG_BIT(ctlreg_3,   KXTF9_CTRL_REG3_OWUFB);
			break;
		case 100:	/* set ODR to 100 Hz */
			SET_REG_BIT(ctlreg_3,   KXTF9_CTRL_REG3_OWUFA);
			UNSET_REG_BIT(ctlreg_3, KXTF9_CTRL_REG3_OWUFB);
			break;
		case 200:	/* set ODR to 200 Hz */
			SET_REG_BIT(ctlreg_3, KXTF9_CTRL_REG3_OWUFA);
			SET_REG_BIT(ctlreg_3, KXTF9_CTRL_REG3_OWUFB);
			break;
		default:
			return FALSE;
	}
	ACCEL_write_byte(KXTF9_I2C_CTRL_REG3, ctlreg_3);
	return TRUE;
}
/*==================================================================================================
FUNCTION: 		    KXTF9_set_tilt_odr
DESCRIPTION:    	This function sets the ODR frequency for the tilt position function. 
ARGUMENTS PASSED:   frequency variable; 1, 6, 12, or 50
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	ODR is set for tilt function according to frequency
==================================================================================================*/
BOOLEAN kxtf9_set_tilt_odr(int frequency)
{
	INT8U ctlreg_3 = 0;

	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG3, &ctlreg_3, 1) != TRUE)
	{
		return FALSE;
	}
	
	switch (frequency)
	{
	case 1:		/* set all ODR's to 1.6Hz */
		UNSET_REG_BIT(ctlreg_3, KXTF9_CTRL_REG3_OTPA);
		UNSET_REG_BIT(ctlreg_3, KXTF9_CTRL_REG3_OTPB);
		break;
	case 6:		/* set all ODR's to 6.3Hz */
		UNSET_REG_BIT(ctlreg_3, KXTF9_CTRL_REG3_OTPA);
		SET_REG_BIT(ctlreg_3,   KXTF9_CTRL_REG3_OTPB);
		break;
	case 12:	/* set all ODR's to 12.5Hz */
		SET_REG_BIT(ctlreg_3,   KXTF9_CTRL_REG3_OTPA);
		UNSET_REG_BIT(ctlreg_3, KXTF9_CTRL_REG3_OTPB);
		break;
	case 50:	/* set all ODR's to 50Hz */
		SET_REG_BIT(ctlreg_3, KXTF9_CTRL_REG3_OTPA);
		SET_REG_BIT(ctlreg_3, KXTF9_CTRL_REG3_OTPB);
		break;
	default:
		return FALSE;
	}
	ACCEL_write_byte(KXTF9_I2C_CTRL_REG3, ctlreg_3);
	
	return TRUE;
}
/*==================================================================================================
FUNCTION: 		    KXTF9_set_tap_odr
DESCRIPTION:    	This function sets the ODR frequency for the tap function. 
ARGUMENTS PASSED:   frequency variable; 50, 100, 200, or 400
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	ODR is set for tap function according to frequency
==================================================================================================*/
extern BOOLEAN kxtf9_set_tap_odr(int frequency)
{
	INT8U ctlreg_3 = 0;

	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG3, &ctlreg_3, 1) != TRUE)
	{
		return FALSE;
	}
	frequency *= 10;
	switch (frequency)
	{
	case 50:		/* set all Tap ODR's to 50Hz */
		UNSET_REG_BIT(ctlreg_3, KXTF9_CTRL_REG3_OTDTA);
		UNSET_REG_BIT(ctlreg_3, KXTF9_CTRL_REG3_OTDTB);
		break;
	case 100:		/* set all ODR's to 100Hz */
		UNSET_REG_BIT(ctlreg_3, KXTF9_CTRL_REG3_OTDTA);
		SET_REG_BIT(ctlreg_3,   KXTF9_CTRL_REG3_OTDTB);
		break;
	case 200:	/* set all ODR's to 200Hz */
		SET_REG_BIT(ctlreg_3,   KXTF9_CTRL_REG3_OTDTA);
		UNSET_REG_BIT(ctlreg_3, KXTF9_CTRL_REG3_OTDTB);
		break;
	case 400:	/* set all ODR's to 400Hz */
		SET_REG_BIT(ctlreg_3, KXTF9_CTRL_REG3_OTDTA);
		SET_REG_BIT(ctlreg_3, KXTF9_CTRL_REG3_OTDTB);
		break;
	default:
		return FALSE;
	}
	ACCEL_write_byte(KXTF9_I2C_CTRL_REG3, ctlreg_3);
	
	return TRUE;
}
/*==================================================================================================
FUNCTION: 		    KXTF9_set_odr_tap
DESCRIPTION:    	This function sets the high pass filter roll off frequency for the accelerometer outputs. 
ARGUMENTS PASSED:   alpha, where roll_off_frequency = ODR/alpha; alpha = (50, 100, 200, 400)
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	High pass filter roll off is set according to alpha
==================================================================================================*/
BOOLEAN kxtf9_set_roll_off_hpf_freq(int alpha)
{
	INT8U data_ctrl_reg = 0;

	if (ACCEL_read_byte(KXTF9_I2C_DATA_CTRL_REG, &data_ctrl_reg, 1) != TRUE){
		return FALSE;
	}
	alpha *= 10;
	switch (alpha){
	case 50:	/* set tap ODR to 50Hz */
		UNSET_REG_BIT(data_ctrl_reg, KXTF9_DATA_CTRL_REG_HPFROA);
		UNSET_REG_BIT(data_ctrl_reg, KXTF9_DATA_CTRL_REG_HPFROB);
		break;
	case 100:	/* set tap ODR to 100 Hz */
		UNSET_REG_BIT(data_ctrl_reg, KXTF9_DATA_CTRL_REG_HPFROA);
		SET_REG_BIT(data_ctrl_reg,   KXTF9_DATA_CTRL_REG_HPFROB);
		break;
	case 200:	/* set tap ODR to 200 Hz */
		SET_REG_BIT(data_ctrl_reg,   KXTF9_DATA_CTRL_REG_HPFROA);
		UNSET_REG_BIT(data_ctrl_reg, KXTF9_DATA_CTRL_REG_HPFROB);
		break;
	case 400:	/* set tap ODR to 400 Hz */
		SET_REG_BIT(data_ctrl_reg, KXTF9_DATA_CTRL_REG_HPFROA);
		SET_REG_BIT(data_ctrl_reg, KXTF9_DATA_CTRL_REG_HPFROB);
		break;
	default:
		return FALSE;
	}
	ACCEL_write_byte(KXTF9_I2C_DATA_CTRL_REG, data_ctrl_reg);
	return TRUE;
}
/*==================================================================================================
FUNCTION: 		KXTF9_set_lpf_odr
DESCRIPTION:    	This function sets the low pass filter roll off for the accelerometer outputs. 
ARGUMENTS PASSED:   	roll off frequency (6, 12, 25, 50, 100, 200, 400)
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	Low pass filter roll off is set according to frequency
==================================================================================================*/
BOOLEAN kxtf9_set_lpf_odr(int frequency)
{
	INT8U data_ctrl_reg = 0;

	if (ACCEL_read_byte(KXTF9_I2C_DATA_CTRL_REG, &data_ctrl_reg, 1) != TRUE){
		return FALSE;
	}
	frequency *= 10;
	switch (frequency){
	case 6:		/* set LPF rolloff to 6.25Hz */
		UNSET_REG_BIT(data_ctrl_reg, KXTF9_DATA_CTRL_REG_OSAA);
		UNSET_REG_BIT(data_ctrl_reg, KXTF9_DATA_CTRL_REG_OSAB);
		UNSET_REG_BIT(data_ctrl_reg, KXTF9_DATA_CTRL_REG_OSAC);
		break;
	case 12:	/* set LPF rolloff to 12.5Hz */
		UNSET_REG_BIT(data_ctrl_reg, KXTF9_DATA_CTRL_REG_OSAA);
		UNSET_REG_BIT(data_ctrl_reg, KXTF9_DATA_CTRL_REG_OSAB);
		SET_REG_BIT(data_ctrl_reg,   KXTF9_DATA_CTRL_REG_OSAC);
		break;
	case 25:	/* set LPF rolloff to 25Hz */
		UNSET_REG_BIT(data_ctrl_reg, KXTF9_DATA_CTRL_REG_OSAA);
		SET_REG_BIT(data_ctrl_reg,   KXTF9_DATA_CTRL_REG_OSAB);
		UNSET_REG_BIT(data_ctrl_reg, KXTF9_DATA_CTRL_REG_OSAC);
		break;
	case 50:	/* set LPF rolloff to 50Hz */
		UNSET_REG_BIT(data_ctrl_reg, KXTF9_DATA_CTRL_REG_OSAA);
		SET_REG_BIT(data_ctrl_reg,   KXTF9_DATA_CTRL_REG_OSAB);
		SET_REG_BIT(data_ctrl_reg,   KXTF9_DATA_CTRL_REG_OSAC);
		break;
	case 100:	/* set LPF rolloff to 100Hz */
		SET_REG_BIT(data_ctrl_reg,   KXTF9_DATA_CTRL_REG_OSAA);
		UNSET_REG_BIT(data_ctrl_reg, KXTF9_DATA_CTRL_REG_OSAB);
		UNSET_REG_BIT(data_ctrl_reg, KXTF9_DATA_CTRL_REG_OSAC);
		break;
	case 200:	/* set LPF rolloff to 200 Hz */
		SET_REG_BIT(data_ctrl_reg,   KXTF9_DATA_CTRL_REG_OSAA);
		UNSET_REG_BIT(data_ctrl_reg, KXTF9_DATA_CTRL_REG_OSAB);
		SET_REG_BIT(data_ctrl_reg,   KXTF9_DATA_CTRL_REG_OSAC);
		break;
	case 400:	/* set LPF rolloff to 400 Hz */
		SET_REG_BIT(data_ctrl_reg,   KXTF9_DATA_CTRL_REG_OSAA);
		SET_REG_BIT(data_ctrl_reg,   KXTF9_DATA_CTRL_REG_OSAB);
		UNSET_REG_BIT(data_ctrl_reg, KXTF9_DATA_CTRL_REG_OSAC);
		break;
	default:
		return FALSE;
	}
	ACCEL_write_byte(KXTF9_I2C_DATA_CTRL_REG, data_ctrl_reg);
	return TRUE;
}
/*==================================================================================================
FUNCTION: 		    KXTF9_read_current_odr_motion
DESCRIPTION:    	This function reads the current ODR of the general motion function. 
ARGUMENTS PASSED:  	ODR_rate_motion pointer
RETURN VALUE:   	TRUE = ODR set correctly; FALSE = ODR invalid
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	ODR_rate_motion is assigned
==================================================================================================*/
BOOLEAN kxtf9_read_current_motion_odr(int* ODR_rate_motion)
{
	BOOLEAN status=TRUE;
	INT8U  ctrl_reg;

	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG3, &ctrl_reg, 1) == TRUE)	
	{
		ctrl_reg &= 0x03;
		ctrl_reg >>= 2;
		
		switch (ctrl_reg)
		{
		case 0:
			*ODR_rate_motion = 25;
			status = TRUE;
			break;
		case 1:
			*ODR_rate_motion = 50;
			status = TRUE;
			break;
		case 2:
			*ODR_rate_motion = 100;
			status = TRUE;
			break;
		case 3:
			*ODR_rate_motion = 200;
			status = TRUE;
			break;
		default:
			status = FALSE;
			break;
		}
	}
	else
	{
		status = FALSE;
	}
	
	return status;
}
/*==================================================================================================
FUNCTION: 		    KXTF9_read_current_odr_tilt
DESCRIPTION:    	This function reads the current ODR of the tilt function. 
ARGUMENTS PASSED:   ODR_rate_tilt pointer
RETURN VALUE:   	TRUE = ODR set correctly; FALSE = ODR invalid
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	ODR_rate_tilt is assigned
==================================================================================================*/
BOOLEAN kxtf9_read_current_tilt_odr(double* ODR_rate_tilt)
{
	BOOLEAN status=TRUE;
	INT8U ctrl_reg;

	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG3, &ctrl_reg, 1) == TRUE){
		ctrl_reg &= 0x60;
		ctrl_reg >>= 5;
		switch (ctrl_reg){
		case 0:
			*ODR_rate_tilt = 1.6;
			status = TRUE;
			break;
		case 1:
			*ODR_rate_tilt = 6.3;
			status = TRUE;
			break;
		case 2:
			*ODR_rate_tilt = 12.5;
			status = TRUE;
			break;
		case 3:
			*ODR_rate_tilt = 50;
			status = TRUE;
			break;
		default:
			status = FALSE;
			break;
		}
	}
	else	status = FALSE;
	return status;
}
/*==================================================================================================
FUNCTION: 		KXTF9_read_current_odr_tap
DESCRIPTION:    	This function reads the current ODR of the tap double tap function. 
ARGUMENTS PASSED:   	ODR_rate_tap pointer
RETURN VALUE:   	TRUE = ODR set correctly; FALSE = ODR invalid
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	ODR_rate_tap is assigned
==================================================================================================*/
BOOLEAN kxtf9_read_current_tap_odr(int* ODR_rate_tap)
{
	BOOLEAN status=TRUE;
	INT8U ctrl_reg;

	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG3, &ctrl_reg, 1) == TRUE){
		ctrl_reg &= 0x0C;
		ctrl_reg >>= 2;
		switch (ctrl_reg){
		case 0:
			*ODR_rate_tap = 50;
			status = TRUE;
			break;
		case 1:
			*ODR_rate_tap = 100;
			status = TRUE;
			break;
		case 2:
			*ODR_rate_tap = 200;
			status = TRUE;
			break;
		case 3:
			*ODR_rate_tap = 400;
			status = TRUE;
			break;
		default:
			status = FALSE;
			break;
		}
	}
	else	status = FALSE;
	return status;
}
/*==================================================================================================
FUNCTION: 		    KXTF9_tdt_timer
DESCRIPTION:    	This function defines the minimum separation between the first and second
			        taps in a double tap event. 
ARGUMENTS PASSED:   tdt_timer; 0-255
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	Double tap minimum separation set according to tdt_timer
IMPORTANT NOTES:   	Default: 0.3s (0x78h)
==================================================================================================*/
BOOLEAN kxtf9_tdt_timer(int tdt_timer)
{
	BOOLEAN status;

	if (ACCEL_write_byte(KXTF9_I2C_TDT_TIMER, (INT8U)tdt_timer) == TRUE){
		status = TRUE;
	}
	else	status = FALSE;
	return status;
}
/*==================================================================================================
FUNCTION: 		KXTF9_tdt_h_thresh
DESCRIPTION:    	This function defines the upper limit for the jerk threshold. 
ARGUMENTS PASSED:   	tdt_h_thresh; 0-255
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	Upper tap threshold set according to tdt_h_thresh
IMPORTANT NOTES:   	Default: 14j (0xB6h)
==================================================================================================*/
BOOLEAN kxtf9_tdt_h_thresh(int tdt_h_thresh)
{
	BOOLEAN status;

	if (ACCEL_write_byte(KXTF9_I2C_TDT_H_THRESH, (INT8U)tdt_h_thresh) == TRUE){
		status = TRUE;
	}
	else	status = FALSE;
	return status;
}
/*==================================================================================================
FUNCTION: 		KXTF9_tdt_l_thresh
DESCRIPTION:    	This function defines the lower limit for the jerk threshold. 
ARGUMENTS PASSED:   	tdt_l_thresh; 0-255
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	Lower tap threshold set according to tdt_l_thresh
IMPORTANT NOTES:   	Default: 1j (0x1Ah)
==================================================================================================*/
BOOLEAN kxtf9_tdt_l_thresh(int tdt_l_thresh)
{
	BOOLEAN status;

	if (ACCEL_write_byte(KXTF9_I2C_TDT_L_THRESH, (INT8U)tdt_l_thresh) == TRUE){
		status = TRUE;
	}
	else	status = FALSE;
	return status;
}
/*==================================================================================================
FUNCTION: 		KXTF9_tdt_tap_timer
DESCRIPTION:    	This function defines the minimum and maximum pulse width for the tap event. 
ARGUMENTS PASSED:   	tdt_tap_timer; 0-255
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	Tap timer set according to tdt_tap_timer
IMPORTANT NOTES:   	Default: 0.005s lower limit, 0.05s upper limit (0xA2h)
==================================================================================================*/
BOOLEAN kxtf9_tdt_tap_timer(int tdt_tap_timer)
{
	BOOLEAN status;

	if (ACCEL_write_byte(KXTF9_I2C_TDT_TAP_TIMER, (INT8U)tdt_tap_timer) == TRUE){
		status = TRUE;
	}
	else	status = FALSE;
	return status;
}
/*==================================================================================================
FUNCTION: 		KXTF9_tdt_total_timer
DESCRIPTION:    	This function defines the amount of time that two taps in a double tap event 
			can be avove the PI threshold. 
ARGUMENTS PASSED:   	tdt_total_timer; 0-255
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	Total timer set according to tdt_total_timer
IMPORTANT NOTES:   	Default: 0.09s (0x24h)
==================================================================================================*/
BOOLEAN kxtf9_tdt_total_timer(int tdt_total_timer)
{
	BOOLEAN status;

	if (ACCEL_write_byte(KXTF9_I2C_TDT_TOTAL_TIMER, (INT8U)tdt_total_timer) == TRUE){
		status = TRUE;
	}
	else	status = FALSE;
	return status;
}
/*==================================================================================================
FUNCTION: 		KXTF9_tdt_latency_timer
DESCRIPTION:    	This function defines the total amount of time that the tap algorithm will 
			count samples that are above the PI threshold. 
ARGUMENTS PASSED:   	tdt_latency_timer; 0-255
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	Latency timer set according to tdt_latency_timer
IMPORTANT NOTES:   	Default: 0.1s (0x28h)
==================================================================================================*/
BOOLEAN kxtf9_tdt_latency_timer(int tdt_latency_timer)
{
	BOOLEAN status;

	if (ACCEL_write_byte(KXTF9_I2C_TDT_LATENCY_TIMER, (INT8U)tdt_latency_timer) == TRUE){
		status = TRUE;
	}
	else	status = FALSE;
	return status;
}
/*==================================================================================================
FUNCTION: 		KXTF9_tdt_window_timer
DESCRIPTION:    	This function defines the time window for the entire tap event, 
			single or double, to occur. 
ARGUMENTS PASSED:   	tdt_window_timer; 0-255
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	Window timer set according to tdt_window_timer
IMPORTANT NOTES:   	Default: 0.4s (0xA0h)
==================================================================================================*/
BOOLEAN kxtf9_tdt_window_timer(int tdt_window_timer)
{
	BOOLEAN status;

	if (ACCEL_write_byte(KXTF9_I2C_TDT_WINDOW_TIMER, (INT8U)tdt_window_timer) == TRUE){
		status = TRUE;
	}
	else	status = FALSE;
	return status;
}
/*==================================================================================================
FUNCTION: 		    KXTF9_read_position_status
DESCRIPTION:    	This function reads INT_SRC_REG to determine if there was a change in tilt. 
ARGUMENTS PASSED:   None
RETURN VALUE:   	TRUE = tilt occurred; FALSE = no tilt occurred
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	None
==================================================================================================*/
BOOLEAN kxtf9_read_position_status(void)
{
	BOOLEAN position_status=TRUE; 
	INT8U  src_reg2;

	if (ACCEL_read_byte(KXTF9_I2C_INT_SRC_REG2, &src_reg2, 1) == TRUE)
	{
		if((src_reg2 & 0x01) == 1)
		{
			position_status = TRUE;
		}
		else
		{
			  position_status = FALSE;
		}
	}
	else
	{
	    position_status = FALSE;
	}

    return position_status;
}
/*==================================================================================================
FUNCTION: 		    KXTF9_read_motion_wuf_status
DESCRIPTION:    	This function reads INT_SRC_REG1 to determine if wake up occurred. 
ARGUMENTS PASSED:   None
RETURN VALUE:   	TRUE = wake up occurred; FALSE = no wake up occurred
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	None
==================================================================================================*/
BOOLEAN kxtf9_read_motion_wuf_status(void)
{
	BOOLEAN wuf_status=TRUE; 
	INT8U  src_reg2;
	
	if (ACCEL_read_byte(KXTF9_I2C_INT_SRC_REG2, &src_reg2, 1) == TRUE)
	{
		if((src_reg2 & 0x02) == 0x02)
		{
			wuf_status = TRUE;
		}
		else
		{
			wuf_status = FALSE;
		}	
	}
	else
	{
			wuf_status = FALSE;
	}
	
	return wuf_status;
}
/*==================================================================================================
FUNCTION: 		    KXTF9_read_LPF_cnt
DESCRIPTION:    	This function reads the number of counts on the X, Y, and Z axes.
ARGUMENTS PASSED:   x, y, and z pointers
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	x, y, and z are assigned
==================================================================================================*/
BOOLEAN kxtf9_read_LPF_cnt(int* x, int* y, int* z)
{
	BOOLEAN status=TRUE; 
	int x_sign, y_sign, z_sign;
	INT8U Res;
	//INT8S ret[3] = {0, 0, 0};
	INT8U xyz[6] = {0, 0, 0, 0, 0, 0};
	
	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG1, &Res, 1) == TRUE){
		Res = Res & 0x40;
		switch(Res){
			case 0x00:	//low-resolution state
				if ((status = ACCEL_read_byte(KXTF9_I2C_XOUT_L, xyz, 6)) == TRUE){
					*x = ((int)xyz[1]);
					x_sign = *x >> 7;	//1 = negative; 0 = positive
					if (x_sign == 1){
						*x = ((~(*x) + 0x01) & 0x0FF);
						*x = -(*x);
					}
					*y = ((int)xyz[3]);
					y_sign = *y >> 7;	//1 = negative; 0 = positive
					if (y_sign == 1){
						*y = ((~(*y) + 0x01) & 0x0FF);	//2's complement
						*y = -(*y);
					}
					*z = ((int)xyz[5]);
					z_sign = *z >> 7;	//1 = negative; 0 = positive
					if (z_sign == 1){
						*z = ((~(*z) + 0x01) & 0x0FF);	//2's complement
						*z = -(*z);						
					}
				}
				break;
			case 0x40:	//high-resolution state
				if ((status = ACCEL_read_byte(KXTF9_I2C_XOUT_L, xyz, 6)) == TRUE){
					*x = ((int)xyz[0]) >> 4;
					*x = *x + (((int)xyz[1]) << 4);
					x_sign = *x >> 11; 	//1 = negative; 0 = positive
					if (x_sign == 1){
						*x = ((~(*x) + 0x01) & 0x0FFF);	//2's complement
						*x = -(*x);
					}
					*y = ((int)xyz[2]) >> 4;
					*y = *y + (((int)xyz[3]) << 4);
					y_sign = *y >> 11; 	//1 = negative; 0 = positive
					if (y_sign == 1){
						*y = ((~(*y) + 0x01) & 0x0FFF);	//2's complement
						*y = -(*y);
					}
					*z = ((int)xyz[4]) >> 4;
					*z = *z + (((int)xyz[5]) << 4);
					z_sign = *z >> 11; 	//1 = negative; 0 = positive
					if (z_sign == 1){
						*z = ((~(*z) + 0x01) & 0x0FFF);	//2's complement
						*z = -(*z);
					}
				}
				break;
		}		
	}
	else status = FALSE;
	return status;
}
/*==================================================================================================
FUNCTION: 		    KXTF9_read_LPF_g
DESCRIPTION:    	This function reads the G(gravity force) values on the X, Y, and Z axes.
			        The units used are milli-g's, or 1/1000*G. 
ARGUMENTS PASSED:   gx, gy, and gz pointers
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	gx, gy, and gz are assigned
==================================================================================================*/
BOOLEAN kxtf9_read_LPF_g(int* gx, int* gy, int* gz)
{
	BOOLEAN status=TRUE;
	int sensitivity;
	int x = 0;
	int y = 0;
	int z = 0;
	int x_sign, y_sign, z_sign;
	INT8U xyz[6] = {0, 0, 0, 0, 0, 0};
	INT8U Res = 0;
	INT8U G_range = 0;
	int range = 0;

	//determine if in the low resolution or high resolution state
	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG1, &Res, 1) == TRUE){
		if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG1, &G_range, 1) == TRUE){
			G_range = G_range & 0x18;
			G_range = G_range >> 3;
			switch(G_range){
				case 0:
					range = 2;
					break;
				case 1:
					range = 4;
					break;
				case 2:
					range = 8;
					break;
				default:
					break;
			}
			Res = Res & 0x40;
			switch(Res){
				case 0x00:	//low-resolution state
					if ((status = ACCEL_read_byte(KXTF9_I2C_XOUT_L, xyz, 6)) == TRUE){
						x = ((int)xyz[1]);
						x_sign = x >> 7;	//1 = negative; 0 = positive
						if (x_sign == 1){
							x = ((~(x) + 0x01) & 0x0FF);
							x = -(x);
						}
						y = ((int)xyz[3]);
						y_sign = y >> 7;	//1 = negative; 0 = positive
						if (y_sign == 1){
							y = ((~(y) + 0x01) & 0x0FF);	//2's complement
							y = -(y);
						}
						z = ((int)xyz[5]);
						z_sign = z >> 7;	//1 = negative; 0 = positive
						if (z_sign == 1){
							z = ((~(z) + 0x01) & 0x0FF);	//2's complement
							z = -(z);						
						}
						sensitivity = (256)/(2*range);
						/* calculate milli-G's */
						*gx = 1000 * (x) / sensitivity; 
						*gy = 1000 * (y) / sensitivity; 
						*gz = 1000 * (z) / sensitivity;
					}
					break;
				case 0x40:	//high-resolution state
					if ((status = ACCEL_read_byte(KXTF9_I2C_XOUT_L, xyz, 6)) == TRUE){
						x = ((int)xyz[0]) >> 4;
						x = x + (((int)xyz[1]) << 4);
						x_sign = x >> 11; 	//1 = negative; 0 = positive
						if (x_sign == 1){
							x = ((~(x) + 0x01) & 0x0FFF);	//2's complement
							x = -(x);
						}
						y = ((int)xyz[2]) >> 4;
						y = y + (((int)xyz[3]) << 4);
						y_sign = y >> 11;	//1 = negative; 0 = positive
						if (y_sign == 1){
							y = ((~(y) + 0x01) & 0x0FFF);	//2's complement
							y = -(y);
						}
						z = ((int)xyz[4]) >> 4;
						z = z + (((int)xyz[5]) << 4);
						z_sign = z >> 11;	//1 = negative; 0 = positive
						if (z_sign == 1){
							z = ((~(z) + 0x01) & 0x0FFF);	//2's complement
							z = -(z);
						}
						sensitivity = (4096)/(2*range);
						/* calculate milli-G's */
						*gx = 1000 * (x) / sensitivity; 
						*gy = 1000 * (y) / sensitivity; 
						*gz = 1000 * (z) / sensitivity;
					}
					break;
				default:
					break;
			}
		}		
		else	status = FALSE;
	}
	else	status = FALSE;
	return status;
}
/*==================================================================================================
FUNCTION: 		KXTF9_read_HPF_cnt
DESCRIPTION:    	This function reads the high pass filtered number of counts on the X, Y, 
			and Z axes. 
ARGUMENTS PASSED:   	x, y, and z pointers
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	x, y, and z are assigned
==================================================================================================*/
BOOLEAN kxtf9_read_HPF_cnt(int* x, int* y, int* z)
{
	BOOLEAN status=TRUE;
	int x_sign, y_sign, z_sign;
	INT8U Res = 0;	
	INT8U xyz[6] = {0, 0, 0, 0, 0, 0};

	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG1, &Res, 1) == TRUE){
			Res = Res & 0x40;
			switch(Res){
				case 0x00:	//low-resolution state
					if ((status = ACCEL_read_byte(KXTF9_I2C_XOUT_HPF_L, xyz, 6)) == 0){
						*x = ((int)xyz[1]);
						x_sign = *x >> 7;	//1 = negative; 0 = positive
						if (x_sign == 1){
							*x = ((~(*x) + 0x01) & 0x0FF);
							*x = -(*x);
						}
						*y = ((int)xyz[3]);
						y_sign = *y >> 7;	//1 = negative; 0 = positive
						if (y_sign == 1){
							*y = ((~(*y) + 0x01) & 0x0FF);	//2's complement
							*y = -(*y);
						}
						*z = ((int)xyz[5]);
						z_sign = *z >> 7;	//1 = negative; 0 = positive
						if (z_sign == 1){
							*z = ((~(*z) + 0x01) & 0x0FF);	//2's complement
							*z = -(*z);						
						}
					}
					break;
				case 0x40:	//high-resolution state
					if ((status = ACCEL_read_byte(KXTF9_I2C_XOUT_HPF_L, xyz, 6)) == 0){
						*x = ((int)xyz[0]) >> 4;
						*x = *x + (((int)xyz[1]) << 4);
						x_sign = *x >> 11; 	//1 = negative; 0 = positive
						if (x_sign == 1){
							*x = ((~(*x) + 0x01) & 0x0FFF);	//2's complement
							*x = -(*x);
						}
						*y = ((int)xyz[2]) >> 4;
						*y = *y + (((int)xyz[3]) << 4);
						y_sign = *y >> 11; 	//1 = negative; 0 = positive
						if (y_sign == 1){
							*y = ((~(*y) + 0x01) & 0x0FFF);	//2's complement
							*y = -(*y);
						}
						*z = ((int)xyz[4]) >> 4;
						*z = *z + (((int)xyz[5]) << 4);
						z_sign = *z >> 11; 	//1 = negative; 0 = positive
						if (z_sign == 1){
							*z = ((~(*z) + 0x01) & 0x0FFF);	//2's complement
							*z = -(*z);
						}
					}
					break;
			}		
	}
	else	status = FALSE;
	return status;
}
/*==================================================================================================
FUNCTION: 		KXTF9_read_HPF_g
DESCRIPTION:    	This function reads the G(gravity force) values on the X, Y, and Z axes.
			The units used are milli-g's, or 1/1000*G. 
ARGUMENTS PASSED:   	gx, gy, and gz pointers
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	gx, gy, and gz are assigned
==================================================================================================*/
BOOLEAN kxtf9_read_HPF_g(int* gx, int* gy, int* gz)
{
	BOOLEAN  status = 0;
	int x,y,z;
	int x_sign, y_sign, z_sign;
	int sensitivity;
	INT8U xyz[6] = {0, 0, 0, 0, 0, 0};
	INT8U Res = 0;
	INT8U G_range = 0;
	int range = 0;

	//determine if in the low resolution or high resolution state
	if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG1, &Res, 1) == TRUE){
		if (ACCEL_read_byte(KXTF9_I2C_CTRL_REG1, &G_range, 1) == TRUE){
			G_range = G_range & 0x18;
			G_range = G_range >> 3;
			switch(G_range){
				case 0:
					range = 2;
					break;
				case 1:
					range = 4;
					break;
				case 2:
					range = 8;
					break;
				default:
					break;
			}
			Res = Res & 0x40; 
			switch(Res){
				case 0x00:	//low-resolution state
					if ((status = ACCEL_read_byte(KXTF9_I2C_XOUT_HPF_L, xyz, 6)) == TRUE){
						x = ((int)xyz[1]);
						x_sign = x >> 7;	//1 = negative; 0 = positive
						if (x_sign == 1){
							x = ((~(x) + 0x01) & 0x0FF);
							x = -(x);
						}
						y = ((int)xyz[3]);
						y_sign = y >> 7;	//1 = negative; 0 = positive
						if (y_sign == 1){
							y = ((~(y) + 0x01) & 0x0FF);	//2's complement
							y = -(y);
						}
						z = ((int)xyz[5]);
						z_sign = z >> 7;	//1 = negative; 0 = positive
						if (z_sign == 1){
							z = ((~(z) + 0x01) & 0x0FF);	//2's complement
							z = -(z);						
						}						
						sensitivity = (256)/(2*range);
						/* calculate milli-G's */
						*gx = 1000 * (x) / sensitivity; 
						*gy = 1000 * (y) / sensitivity; 
						*gz = 1000 * (z) / sensitivity;
					}
					break;
				case 0x40:	//high-resolution state
					if ((status = ACCEL_read_byte(KXTF9_I2C_XOUT_HPF_L, xyz, 6)) == TRUE){
						x = ((int)xyz[0]) >> 4;
						x = x + (((int)xyz[1]) << 4);
						x_sign = x >> 11; 	//1 = negative; 0 = positive
						if (x_sign == 1){
							x = ((~(x) + 0x01) & 0x0FFF);	//2's complement
							x = -(x);
						}
						y = ((int)xyz[2]) >> 4;
						y = y + (((int)xyz[3]) << 4);
						y_sign = y >> 11; 	//1 = negative; 0 = positive
						if (y_sign == 1){
							y = ((~(y) + 0x01) & 0x0FFF);	//2's complement
							y = -(y);
						}
						z = ((int)xyz[4]) >> 4;
						z = z + (((int)xyz[5]) << 4);
						z_sign = z >> 11; 	//1 = negative; 0 = positive
						if (z_sign == 1){
							z = ((~(z) + 0x01) & 0x0FFF);	//2's complement
							z = -(z);
						}
						sensitivity = (4096)/(2*range);
						/* calculate milli-G's */
						*gx = 1000 * (x) / sensitivity; 
						*gy = 1000 * (y) / sensitivity; 
						*gz = 1000 * (z) / sensitivity;
					}
					break;
				default:
					break;
			}
		}
		else	status = FALSE;
	}
	else status = FALSE;
	return status;
}
/*==================================================================================================
FUNCTION: 		    KXTF9_read_tap_direction
DESCRIPTION:    	This function reads INT_SRC_REG1 to determine which axis and in which direction
			        a tap or double tap event occurred. 
ARGUMENTS PASSED:   int_src_reg1 pointer
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	int_src_reg1 is assigned
==================================================================================================*/
BOOLEAN kxtf9_read_tap_direction(INT8U* int_src_reg1)
{
	BOOLEAN status;

	status = ACCEL_read_byte(KXTF9_I2C_INT_SRC_REG1, int_src_reg1, 1);
	return status;
}
/*==================================================================================================
FUNCTION: 		    KXTF9_enable_int
DESCRIPTION:    	This function enables the physical interrupt. 
ARGUMENTS PASSED:   None
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	initialization has been called
POST-CONDITIONS:   	Physical interrupt enabled
==================================================================================================*/
BOOLEAN kxtf9_enable_int(void)
{
	INT8U int_ctrl_reg1 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_INT_CTRL_REG1, &int_ctrl_reg1, 1) == TRUE){
		SET_REG_BIT(int_ctrl_reg1, KXTF9_INT_CTRL_REG1_IEN);
		ACCEL_write_byte(KXTF9_I2C_INT_CTRL_REG1, int_ctrl_reg1);
	}
	 	
	else	status = FALSE; 	
    return status;
}
/*==================================================================================================
FUNCTION: 		    KXTF9_disable_int
DESCRIPTION:    	This function disables the physical interrupt. 
ARGUMENTS PASSED:  	None
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	initialization has been called
POST-CONDITIONS:   	Physical interrupt disabled
==================================================================================================*/
BOOLEAN kxtf9_disable_int(void)
{
	INT8U int_ctrl_reg1 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_INT_CTRL_REG1, &int_ctrl_reg1, 1) == TRUE){
		UNSET_REG_BIT(int_ctrl_reg1, KXTF9_INT_CTRL_REG1_IEN);
		ACCEL_write_byte(KXTF9_I2C_INT_CTRL_REG1, int_ctrl_reg1);
	}
	else	status = FALSE;
	return status;
}
/*==================================================================================================
FUNCTION: 		    KXTF9_int_activeh
DESCRIPTION:    	This function sets the polarity of physical interrupt pin to active high. 
ARGUMENTS PASSED:   None
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	initialization has been called
POST-CONDITIONS:   	Physical interrupt set to active high
==================================================================================================*/
BOOLEAN kxtf9_int_activeh(void)
{
	INT8U int_ctrl_reg1 = 0;
	BOOLEAN status = TRUE;
	
	if (ACCEL_read_byte(KXTF9_I2C_INT_CTRL_REG1, &int_ctrl_reg1, 1) == TRUE){
		SET_REG_BIT(int_ctrl_reg1, KXTF9_INT_CTRL_REG1_IEA);
		ACCEL_write_byte(KXTF9_I2C_INT_CTRL_REG1, int_ctrl_reg1);
	}
	else	status = FALSE;
	return status;
}
/*==================================================================================================
FUNCTION: 		    KXTF9_int_activel
DESCRIPTION:    	This function sets the polarity of physical interrupt pin to active low. 
ARGUMENTS PASSED:   None
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	initialization has been called
POST-CONDITIONS:   	Physical interrupt set to active low
==================================================================================================*/
BOOLEAN kxtf9_int_activel(void)
{
	INT8U int_ctrl_reg1 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_INT_CTRL_REG1, &int_ctrl_reg1, 1) == TRUE){
		UNSET_REG_BIT(int_ctrl_reg1, KXTF9_INT_CTRL_REG1_IEA);
		ACCEL_write_byte(KXTF9_I2C_INT_CTRL_REG1, int_ctrl_reg1);
	}
	else	status = FALSE;
	return status;
}
/*==================================================================================================
FUNCTION: 		    KXTF9_int_latch
DESCRIPTION:    	This function sets the physical interrupt to a latch state. 
ARGUMENTS PASSED:  	None
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	initialization has been called
POST-CONDITIONS:   	Physical interrupt set to latched response
==================================================================================================*/
BOOLEAN kxtf9_int_latch(void)
{
	INT8U int_ctrl_reg1 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_INT_CTRL_REG1, &int_ctrl_reg1, 1) == TRUE){
		UNSET_REG_BIT(int_ctrl_reg1, KXTF9_INT_CTRL_REG1_IEL);
		ACCEL_write_byte(KXTF9_I2C_INT_CTRL_REG1, int_ctrl_reg1);
	}
	else	status = FALSE;
	return status;
}
/*==================================================================================================
FUNCTION: 		    KXTF9_int_pulse
DESCRIPTION:    	This function sets the physical interrupt to a pulse state. 
ARGUMENTS PASSED:   None
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	initialization has been called
POST-CONDITIONS:   	Physical interrupt set to pulse response
==================================================================================================*/
BOOLEAN kxtf9_int_pulse(void)
{
	INT8U int_ctrl_reg1 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_INT_CTRL_REG1, &int_ctrl_reg1, 1) == TRUE){
		SET_REG_BIT(int_ctrl_reg1, KXTF9_INT_CTRL_REG1_IEL);
		ACCEL_write_byte(KXTF9_I2C_INT_CTRL_REG1, int_ctrl_reg1);
	}
	else	status = FALSE;
	return status;
}
/*==================================================================================================
FUNCTION: 		KXTF9_tap_mask_TFU
DESCRIPTION:    	This function masks Z+ (face up) tap. 
ARGUMENTS PASSED:   	None
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	Z+ tap masked
==================================================================================================*/
BOOLEAN kxtf9_tap_mask_TFU(void)
{
	INT8U ctrl_reg3 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_INT_CTRL_REG3, &ctrl_reg3, 1) == TRUE){
		UNSET_REG_BIT(ctrl_reg3, KXTF9_INT_CTRL_REG3_TFUM);
		ACCEL_write_byte(KXTF9_I2C_INT_CTRL_REG3, ctrl_reg3);
	}
	else	status = FALSE;
	return status;
}
/*==================================================================================================
FUNCTION: 		KXTF9_tap_unmask_TFU
DESCRIPTION:    	This function unmasks Z+ (face up) tap. 
ARGUMENTS PASSED:   	None
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	Z+ tap unmasked
==================================================================================================*/
BOOLEAN kxtf9_tap_unmask_TFU(void)
{
	INT8U ctrl_reg3 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_INT_CTRL_REG3, &ctrl_reg3, 1) == TRUE){
		SET_REG_BIT(ctrl_reg3, KXTF9_INT_CTRL_REG3_TFUM);
		ACCEL_write_byte(KXTF9_I2C_INT_CTRL_REG3, ctrl_reg3);
	}
	else	status = FALSE;
	return status;
}
/*==================================================================================================
FUNCTION: 		    KXTF9_tap_mask_TFD
DESCRIPTION:    	This function masks Z- (face down) tap. 
ARGUMENTS PASSED:   None
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	Z- tap masked
==================================================================================================*/
BOOLEAN kxtf9_tap_mask_TFD(void)
{
	INT8U ctrl_reg3 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_INT_CTRL_REG3, &ctrl_reg3, 1) == TRUE){
		UNSET_REG_BIT(ctrl_reg3, KXTF9_INT_CTRL_REG3_TFDM);
		ACCEL_write_byte(KXTF9_I2C_INT_CTRL_REG3, ctrl_reg3);
	}
	else	status = FALSE;
	return status;
}
/*==================================================================================================
FUNCTION: 		KXTF9_tap_unmask_TFD
DESCRIPTION:    	This function unmasks Z- (face down) tap. 
ARGUMENTS PASSED:   	None
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	Z- tap unmasked
==================================================================================================*/
BOOLEAN kxtf9_tap_unmask_TFD(void)
{
	INT8U ctrl_reg3 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_INT_CTRL_REG3, &ctrl_reg3, 1) == TRUE){
		SET_REG_BIT(ctrl_reg3, KXTF9_INT_CTRL_REG3_TFDM);
		ACCEL_write_byte(KXTF9_I2C_INT_CTRL_REG3, ctrl_reg3);
	}
	else	status = FALSE;
	return status;
}
/*==================================================================================================
FUNCTION: 		KXTF9_tap_mask_TUP
DESCRIPTION:    	This function masks Y+ (up) tap. 
ARGUMENTS PASSED:   	None
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	Y+ tap masked
==================================================================================================*/
BOOLEAN kxtf9_tap_mask_TUP(void)
{
	INT8U ctrl_reg3 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_INT_CTRL_REG3, &ctrl_reg3, 1) == TRUE){
		UNSET_REG_BIT(ctrl_reg3, KXTF9_INT_CTRL_REG3_TUPM);
		ACCEL_write_byte(KXTF9_I2C_INT_CTRL_REG3, ctrl_reg3);
	}
	else	status = FALSE;
	return status;
}
/*==================================================================================================
FUNCTION: 		KXTF9_tap_unmask_TUP
DESCRIPTION:    	This function unmasks Y+ (up) tap. 
ARGUMENTS PASSED:   	None
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	Y+ tap unmasked
==================================================================================================*/
BOOLEAN kxtf9_tap_unmask_TUP(void)
{
	INT8U ctrl_reg3 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_INT_CTRL_REG3, &ctrl_reg3, 1) == TRUE){
		SET_REG_BIT(ctrl_reg3, KXTF9_INT_CTRL_REG3_TUPM);
		ACCEL_write_byte(KXTF9_I2C_INT_CTRL_REG3, ctrl_reg3);
	}
	else	status = FALSE;
	return status;
}
/*==================================================================================================
FUNCTION: 		KXTF9_tap_mask_TDO
DESCRIPTION:    	This function masks Y- (down) tap. 
ARGUMENTS PASSED:   	None
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	Y- tap masked
==================================================================================================*/
BOOLEAN kxtf9_tap_mask_TDO(void)
{
	INT8U ctrl_reg3 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_INT_CTRL_REG3, &ctrl_reg3, 1) == TRUE){
		UNSET_REG_BIT(ctrl_reg3, KXTF9_INT_CTRL_REG3_TDOM);
		ACCEL_write_byte(KXTF9_I2C_INT_CTRL_REG3, ctrl_reg3);
	}
	else	status = FALSE;
	return status;
}
/*==================================================================================================
FUNCTION: 		KXTF9_tap_unmask_TDO
DESCRIPTION:    	This function unmasks Y- (down) tap. 
ARGUMENTS PASSED:   	None
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	Y- tap unmasked
==================================================================================================*/
BOOLEAN kxtf9_tap_unmask_TDO(void)
{
	INT8U ctrl_reg3 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_INT_CTRL_REG3, &ctrl_reg3, 1) == TRUE){
		SET_REG_BIT(ctrl_reg3, KXTF9_INT_CTRL_REG3_TDOM);
		ACCEL_write_byte(KXTF9_I2C_INT_CTRL_REG3, ctrl_reg3);
	}
	else	status = FALSE;
	return status;
}
/*==================================================================================================
FUNCTION: 		KXTF9_tap_mask_TRI
DESCRIPTION:    	This function masks X+ (right) tap. 
ARGUMENTS PASSED:   	None
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	X+ tap masked
==================================================================================================*/
BOOLEAN kxtf9_tap_mask_TRI(void)
{
	INT8U ctrl_reg3 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_INT_CTRL_REG3, &ctrl_reg3, 1) == TRUE){
		UNSET_REG_BIT(ctrl_reg3, KXTF9_INT_CTRL_REG3_TRIM);
		ACCEL_write_byte(KXTF9_I2C_INT_CTRL_REG3, ctrl_reg3);
	}
	else	status = FALSE;
	return status;
}
/*==================================================================================================
FUNCTION: 		KXTF9_tap_unmask_TRI
DESCRIPTION:    	This function unmasks X+ (right) tap. 
ARGUMENTS PASSED:   	None
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	X+ tap unmasked
==================================================================================================*/
BOOLEAN kxtf9_tap_unmask_TRI(void)
{
	INT8U ctrl_reg3 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_INT_CTRL_REG3, &ctrl_reg3, 1) == TRUE){
		SET_REG_BIT(ctrl_reg3, KXTF9_INT_CTRL_REG3_TRIM);
		ACCEL_write_byte(KXTF9_I2C_INT_CTRL_REG3, ctrl_reg3);
	}
	else	status = FALSE;
	return status;
}
/*==================================================================================================
FUNCTION: 		KXTF9_tap_mask_TLE
DESCRIPTION:    	This function masks X- (left) tap. 
ARGUMENTS PASSED:   	None
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	X- tap masked
==================================================================================================*/
BOOLEAN kxtf9_tap_mask_TLE(void)
{
	INT8U ctrl_reg3 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_INT_CTRL_REG3, &ctrl_reg3, 1) == TRUE){
		UNSET_REG_BIT(ctrl_reg3, KXTF9_INT_CTRL_REG3_TLEM);
		ACCEL_write_byte(KXTF9_I2C_INT_CTRL_REG3, ctrl_reg3);
	}
	else	status = FALSE;
	return status;
}
/*==================================================================================================
FUNCTION: 		KXTF9_tap_unmask_TLE
DESCRIPTION:    	This function unmasks X- (left) tap. 
ARGUMENTS PASSED:   	None
RETURN VALUE:   	TRUE = pass; FALSE = fail
PRE-CONDITIONS:   	Initialzation has been called
POST-CONDITIONS:   	X- tap unmasked
==================================================================================================*/
BOOLEAN kxtf9_tap_unmask_TLE(void)
{
	INT8U ctrl_reg3 = 0;
	BOOLEAN status = TRUE;

	if (ACCEL_read_byte(KXTF9_I2C_INT_CTRL_REG3, &ctrl_reg3, 1) == TRUE){
		SET_REG_BIT(ctrl_reg3, KXTF9_INT_CTRL_REG3_TLEM);
		ACCEL_write_byte(KXTF9_I2C_INT_CTRL_REG3, ctrl_reg3);
	}
	else	status = FALSE;
	return status;
}



////////////// ////////////////////////   add by xzb		//////////////////////////////////////////////////

/*
INT8U kxtf9_read_reg_value(INT8U reg)
{
	INT8U value = 0xff;

	//ACCEL_read_byte(KXTF9_I2C_WHO_AM_I, &id, 1);

	ACCEL_read_byte(reg, &value, 1);
	
	return value;	
}
*/

BOOLEAN kxtf9_read_reg_value(INT8U base_reg, INT8U read_reg_counts, INT8U *data)
{

	//ACCEL_read_byte(KXTF9_I2C_WHO_AM_I, &id, 1);

	if(ACCEL_read_byte(base_reg, data, read_reg_counts) == TRUE)
	
		return TRUE;
	
	
	return FALSE;	
}





/*************************  accel_kionx_init ************************/
/*
void accel_kionix_init(void)
{
	ACCEL_INFO accel_info;

	//kxtf9_sleep();

	
	kxtf9_reset();
	os_dly_wait(50);
	
  	// System Feature	 Settings
	accel_info.resolution = 12;
	accel_info.g_range = 2;

	// Tillt Settings
	accel_info.tiltinfo.enable = 1;		 // 0 不设置，按出厂设置
	accel_info.tiltinfo.ODR = 12; // 50;		 	// 50Hz
	accel_info.tiltinfo.timer = 0x01;//10;		// 0.2s
	accel_info.tiltinfo.angle = 0; 		//0x0e;   // 26度   sin26 * 32 = 0x0e 
	accel_info.tiltinfo.hysteresis = 0; //0x14;		 // +/-15度
	
	// Motion Detect Settings
	accel_info.wufinfo.enable = 1;
	accel_info.wufinfo.ODR = 200;  		//25;   // 25Hz
	accel_info.wufinfo.timer = 0;
	accel_info.wufinfo.thresh = 0; 		//0x08;   // 0.5g

	// Tap Double Tap(TDT) Settings
	accel_info.tdtinfo.enable = 1;
   accel_info.tdtinfo.ODR = 40;          // 400Hz	 accel_info.tdtinfo.ODR约小了10倍
   accel_info.tdtinfo.tdt_timer = 0;		//0x78;   // 0.3s
   accel_info.tdtinfo.tdt_h_thresh = 0;	//0xb6;
   accel_info.tdtinfo.tdt_l_thresh = 0;	//0x1a;
   accel_info.tdtinfo.tdt_tap_timer = 0;	//0xa2; // 0.005s
   accel_info.tdtinfo.tdt_total_timer = 0; 	//0x24;  // 0.09s
   accel_info.tdtinfo.tdt_latency_timer = 0; //0x28;	  // 0.1s
   accel_info.tdtinfo.tdt_window_timer = 0;	//0xa0;	  // 0.4s

  // Mask info
//	accel_info.maskinfo.tilt_mask_enable = 1;
//	accel_info.maskinfo.tdt_mask_enable = 1;
//	accel_info.maskinfo.axis_mask_enable = 1;
//	accel_info.maskinfo.mask_le = 1;
//	accel_info.maskinfo.mask_ri = 1;
//	accel_info.maskinfo.mask_do = 1;
//	accel_info.maskinfo.mask_up = 1;
//	accel_info.maskinfo.mask_fd = 1;
//	accel_info.maskinfo.mask_fu = 1;
//	accel_info.maskinfo.mask_tle = 1;
//	accel_info.maskinfo.mask_tri = 1;
//	accel_info.maskinfo.mask_tdo = 1;
//	accel_info.maskinfo.mask_tup = 1;
//	accel_info.maskinfo.mask_tfd = 1;
//	accel_info.maskinfo.mask_tfu = 1;
//	accel_info.maskinfo.x_mask = 1;
//	accel_info.maskinfo.y_mask = 1;
//	accel_info.maskinfo.z_mask = 1; 
	accel_info.maskinfo.B32 = 0x0003ffff;		  // 1： 启用  0：禁用/屏蔽


	// Interrupt control
	accel_info.intinfo.enable  = 1;		//1 = enable
	accel_info.intinfo.polarity = 1;		//1 = active high
	accel_info.intinfo.latch = 0; 		//1;		//1 = unlatched
	accel_info.intinfo.alt_latch = 0; 	 //0 = wuf int latch response
	

  kxtf9_init(&accel_info, 0);

  kxtf9_enable_outputs(); //set sensor to operation mode. Note, this should be the last step to perform

	
}	*/











////////////// ////////////////////////  end		//////////////////////////////////////////////////

