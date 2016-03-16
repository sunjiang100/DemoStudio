/*==================================================================================================
                                                                               
     Header Name: i2c_interface.c

     General Description: Implementation of I2C communication protocol with accelerometer.
 
====================================================================================================


Revision History:
                            Modification     Tracking
Author                          Date          Number     Description of Changes
-------------------------   ------------    ----------   -------------------------------------------
Calvin Ong                   01/11/2010                   Setup I2C interface function to allow developers 
                                                          to place their I2C implementation. 

====================================================================================================
                                         INCLUDE FILES
==================================================================================================*/
#include "i2c_interface.h"
#include "kionix_kxtf9_common.h"	


/*==================================================================================================
                                     LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
                                     LOCAL ENUMS
==================================================================================================*/

/*==================================================================================================

FUNCTION: i2c_write

DESCRIPTION: THIS IS PSEUDO CODE
   this function is the main interface to write a byte to the slave. 

ARGUMENTS PASSED:
   None
      
return VALUE:
   None

PRE-CONDITIONS:
      
POST-CONDITIONS:
   None

IMPORTANT NOTES:

1) This function is follows the protocol to access the Kionix Accelerometer. 
2) This function is implemented to only write one byte to the Accelerometer.


Term Definition
S    = Start Condition
Sr   = Repeated Start Condition
SAD  = Slave Address
W    = Write Bit
R    = Read Bit
ACK  = Acknowledge
NACK = Not Acknowledge
RA   = Register Address
Data = Transmitted/Received Data
P    = Stop Condition


       -----------------------------------------------------------
 Master|START| SAD + W |      |  RA   |      | DATA |       |  P  |    
       |----------------------------------------------------------
 Slave |     |         | ACK  |       | ACK  |      |  ACK  |     |
       -----------------------------------------------------------
==================================================================================================*/
BOOLEAN kionix_i2c_write(i2c_rw_cmd_type *i2c_rw)
{
	
     //Place your i2c implementaion or access function here
 
     //return KIONIX_I2C_SUCCESS;

	 INT8U ret;

    ret = I2C_write_for_accel(i2c_rw->slave_addr, i2c_rw->slave_reg_addr, i2c_rw->length, i2c_rw->data_ptr);

	 if (ret)       
    	return TRUE;


	return  FALSE;
     
	  
}
/*==================================================================================================

FUNCTION: i2c_read

DESCRIPTION: THIS IS PSEUDO CODE
   this function is the main interface to read multiple bytes from the slave. 

ARGUMENTS PASSED:
   None
      
return VALUE:
   None

PRE-CONDITIONS:
      
POST-CONDITIONS:
   None

IMPORTANT NOTES:

This function is follows the protocol to read multiple bytes from the Kionix Accelerometer. 

Term Definition
S    = Start Condition
Sr   = Repeated Start Condition
SAD  = Slave Address
W    = Write Bit
R    = Read Bit
ACK  = Acknowledge
NACK = Not Acknowledge
RA   = Register Address
Data = Transmitted/Received Data
P    = Stop Condition

Read one byte from Slave
       ----------------------------------------------------------------------------------------
 Master|START| SAD + W |      |  RA   |      | Sr   | SAD + R |       |       |  NACK  |   P   |    
       |---------------------------------------------------------------------------------------
 Slave |     |         | ACK  |       | ACK  |      |         |  ACK  | DATA  |        |       |
       ----------------------------------------------------------------------------------------
       
       
Read Multiple Bytes from slaves  
       ----------------------------------------------------------------------------------------------------------
 Master|START| SAD + W |      |  RA   |      | Sr   | SAD + R |       |       |  ACK   |        | NACK   |   P   |  
       |---------------------------------------------------------------------------------------------------------
 Slave |     |         | ACK  |       | ACK  |      |         |  ACK  | DATA  |        | DATA   |        |       |
       ----------------------------------------------------------------------------------------------------------      
 

==================================================================================================*/
BOOLEAN kionix_i2c_read(i2c_rw_cmd_type *i2c_rw)
{
	  
    //Place your i2c implementaion or access function here
	 INT8U ret;

    ret = I2C_read_for_accel(i2c_rw->slave_addr, i2c_rw->slave_reg_addr, i2c_rw->length, i2c_rw->data_ptr);

	 if (ret == OK)       
    	return TRUE;


	return  FALSE;
     
}
