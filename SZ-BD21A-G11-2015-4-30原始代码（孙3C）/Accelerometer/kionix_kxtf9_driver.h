#ifndef _KIONIX_KXTF9_DRIVER_H_
#define _KIONIX_KXTF9_DRIVER_H_


/*==================================================================================================
                                                                               
     Header Name: Kionix_kxtf9_driver.h

     General Description: Function prototypes for an accelerometer with an I2C interface. 
 
====================================================================================================

Revision History:
                            Modification     Tracking
Author                          Date          Number     Description of Changes
-------------------------   ------------    ----------   -------------------------------------------
Calvin Ong                   06/29/2009                  Setup, integrate & modify Chris Hudson's KXTF9 Driver

====================================================================================================
                                         INCLUDE FILES
==================================================================================================*/


#include "kionix_kxtf9_common.h"	


/*==================================================================================================
                                            MACROS
==================================================================================================*/


#define SET_REG_BIT(r,b) r |= b
#define UNSET_REG_BIT(r,b) r &= ~b

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/* TILT_INFO structure to setup TILT function */
typedef struct
{
	INT8U enable;		//1=enable
	INT8U ODR;		//ODR rate ( 1 = 1.6Hz, 6 = 6.3Hz, 12 = 12.5Hz or 50 = 50Hz)
	INT8U timer;		// Tilt count
	INT8U angle;		//device Orientation Angle
	INT8U hysteresis; //Hysteresis
}TILTINFO;

/* WUP_INFO structure to setup Wake Up function */
typedef struct
{
	INT8U enable;		//1=enable
	INT8U ODR;		//ODR rate 25Hz, 50Hz, 100Hz, 200Hz
	INT8U timer;		//Wake-up timer. Every count is output at a  delay period of (1/ODR)
	INT8U thresh;		//Wake-up threshold to detect the change in g (inactivity to activity)
}WUFINFO;


typedef struct
{
    INT8U enable; 
	 INT8U ODR;        
	 INT8U tdt_l_thresh;
	 INT8U tdt_h_thresh;
     INT8U tdt_latency_timer;
	 INT8U tdt_window_timer;
	 INT8U tdt_timer;	
	 INT8U tdt_tap_timer;
     INT8U tdt_total_timer;

}TDTINFO;

/*typedef struct
{
	int tilt_mask_enable;
	int tdt_mask_enable;
	int axis_mask_enable;
	int mask_le;
	int mask_ri;
	int mask_do;
	int mask_up;
	int mask_fd;
	int mask_fu;
	int mask_tle;
	int mask_tri;
	int mask_tdo;
	int mask_tup;
	int mask_tfd;
	int mask_tfu;
	int x_mask;
	int y_mask;
	int z_mask;
	
}MASKINFO; */

typedef union
{
	unsigned int B32;
	struct { 
		// Tilt Mask bits
		INT8U tilt_mask_enable:1;	
		INT8U mask_le:1;
		INT8U mask_ri:1;
		INT8U mask_do:1;
		INT8U mask_up:1;
		INT8U mask_fd:1;
		INT8U mask_fu:1;
		// TDT Mask bits
		INT8U tdt_mask_enable:1;
		INT8U mask_tle:1;
		INT8U mask_tri:1;
		INT8U mask_tdo:1;
		INT8U mask_tup:1;
		INT8U mask_tfd:1;
		INT8U mask_tfu:1;
		// Montion Mask bits
		INT8U axis_mask_enable:1;
		INT8U x_mask:1;
		INT8U y_mask:1;
		INT8U z_mask:1;
		INT8U Rsv1:6;
		INT8U Rsv2:8;
	} Bits;
	
}MASKINFO;


/* Interrupt control */
typedef struct
{
	INT8U enable;		//1 = enable
	INT8U polarity;	//1 = active high
	INT8U latch;		//1 = unlatched
	INT8U alt_latch;  //0 = wuf int latch response
}INTINFO;



typedef struct
{
	INT8U resolution;  
    INT8U  g_range;			
	TILTINFO tiltinfo;	 
	WUFINFO  wufinfo;		 
   TDTINFO  tdtinfo;		 
   MASKINFO maskinfo;	 
   INTINFO  intinfo;		 
    	
}ACCEL_INFO;  

/*==================================================================================================
					KIONIX ACCEL REGISTER MAP
==================================================================================================*/
#define KXTF9_I2C_SLV_ADDR 		0x0F
/**************************************************************************************************
					         KXTF9-SPECIFIC REGISTERS MAP
***************************************************************************************************/
/*Read Only Registers */
#define KXTF9_I2C_ST_RESP		        0x0C
#define KXTF9_I2C_WHO_AM_I		        0x0F
#define KXTF9_I2C_TILT_POS_CUR	        0x10
#define KXTF9_I2C_TILT_POS_PRE	        0x11
#define KXTF9_I2C_STATUS_REG		    0x18
#define KXTF9_I2C_INT_REL		        0x1A

/*Read-write Control Registers */
#define KXTF9_I2C_CTRL_REG1		        0x1B
#define KXTF9_I2C_CTRL_REG2		        0x1C
#define KXTF9_I2C_CTRL_REG3		        0x1D
#define KXTF9_I2C_INT_CTRL_REG2	        0x1F
#define KXTF9_I2C_TILT_TIMER		    0x28
#define KXTF9_I2C_WUF_TIMER		        0x29
#define KXTF9_I2C_WUF_THRESH		    0x5A
#define KXTF9_I2C_TILT_ANGLE            0x5C
#define KXTF9_I2C_HYST_SET              0x5F

/* F9 unique registers*/
#define KXTF9_I2C_XOUT_HPF_L			0x00
#define KXTF9_I2C_XOUT_HPF_H			0x01
#define KXTF9_I2C_YOUT_HPF_L			0x02
#define KXTF9_I2C_YOUT_HPF_H			0x03
#define KXTF9_I2C_ZOUT_HPF_L			0x04
#define KXTF9_I2C_ZOUT_HPF_H			0x05
#define KXTF9_I2C_XOUT_L			    0x06
#define KXTF9_I2C_XOUT_H			    0x07
#define KXTF9_I2C_YOUT_L			    0x08
#define KXTF9_I2C_YOUT_H			    0x09
#define KXTF9_I2C_ZOUT_L			    0x0A
#define KXTF9_I2C_ZOUT_H			    0x0B

#define KXTF9_I2C_INT_SRC_REG1			0x15
#define KXTF9_I2C_INT_SRC_REG2			0x16
#define KXTF9_I2C_INT_CTRL_REG1			0x1E
#define KXTF9_I2C_INT_CTRL_REG3			0x20
#define KXTF9_I2C_DATA_CTRL_REG			0x21

#define KXTF9_I2C_TDT_TIMER			    0x2B
#define KXTF9_I2C_TDT_H_THRESH			0x2C
#define KXTF9_I2C_TDT_L_THRESH			0x2D
#define KXTF9_I2C_TDT_TAP_TIMER			0x2E
#define KXTF9_I2C_TDT_TOTAL_TIMER		0x2F
#define KXTF9_I2C_TDT_LATENCY_TIMER		0x30
#define KXTF9_I2C_TDT_WINDOW_TIMER		0x31
#define KXTF9_I2C_TDT_SELF_TEST         0x3A
/*==================================================================================================
					              KTXE9 CONTROL BITS
==================================================================================================*/


//CTRL_REG 1
#define KXTF9_CTRL_REG1_TPE			0x01	/* enables tilt position function */
#define KXTF9_CTRL_REG1_WUFE		0x02	/* enables wake up function */
#define KXTF9_CTRL_REG1_PC1			0x80	/* operating mode 1 = full power mode; 0 = stand by mode */
#define KXTF9_CTRL_REG1_TDTE			0x04	/* enables tap/double tap function on KXTF9 */
#define KXTF9_CTRL_REG1_GSEL0			0x08	/* bit0 used for selecting the g range */
#define KXTF9_CTRL_REG1_GSEL1			0x10  	/* bit1 used for selecting the g range */
#define KXTF9_CTRL_REG1_DRDYE			0x20	/* enables physical interrupt when new accel data is available on KXTF9 */
#define KXTF9_CTRL_REG1_RES			    0x40	/* performance mode on KXTF9 */


//CTRL REG 2
#define KXTF9_CTRL_REG2_FUM			0x01	/* face up state mask */
#define KXTF9_CTRL_REG2_FDM			0x02	/* face down state mask */
#define KXTF9_CTRL_REG2_UPM			0x04	/* up state mask */
#define KXTF9_CTRL_REG2_DOM			0x08	/* down state mask */
#define KXTF9_CTRL_REG2_RIM			0x10	/* right state mask */
#define KXTF9_CTRL_REG2_LEM			0x20	/* left state mask */

//INIT CTRL REG 3
#define KXTF9_CTRL_REG3_FUM			0x01	/* face up state mask */
#define KXTF9_CTRL_REG3_FDM			0x02	/* face down state mask */
#define KXTF9_CTRL_REG3_UPM			0x04	/* up state mask */
#define KXTF9_CTRL_REG3_DOM			0x08	/* down state mask */
#define KXTF9_CTRL_REG3_RIM			0x10	/* right state mask */
#define KXTF9_CTRL_REG3_LEM			0x20	/* left state mask */

// CTRL REG  3
#define KXTF9_CTRL_REG3_OWUFB		    0x01	/* active mode output data rate */
#define KXTF9_CTRL_REG3_OWUFA		    0x02	/* active mode output data rate */
#define KXTF9_CTRL_REG3_STC			    0x10	/* initiates self-test function */
#define KXTF9_CTRL_REG3_SRST		    0x80	/* software reset */
#define KXTF9_CTRL_REG3_OTPB			0x20	/* sets output data rate for tilt position function */
#define KXTF9_CTRL_REG3_OTPA			0x40	/* sets output data rate for tilt position function */
#define KXTF9_CTRL_REG3_OTDTB			0x04	/* sets output data rate for tap double tap function */
#define KXTF9_CTRL_REG3_OTDTA			0x08	/* sets output data rate for tap double tap function */


//Interrupt CTRL REG 1
#define KXTF9_INT_CTRL_REG1_STPOL	    0x02	/* ST (self test) polarity bit */
#define KXTF9_INT_CTRL_REG1_ULMB	    0x04	/* unlatched mode for WUF 1=unlatched, 0=latched */
#define KXTF9_INT_CTRL_REG1_IEL		    0x08	/* sets response of physical interrupt pin */
#define KXTF9_INT_CTRL_REG1_IEA		    0x10	/* sets polarity of the physical interrupt pin*/
#define KXTF9_INT_CTRL_REG1_IEN		    0x20	/* enables/disables the physical interrupt pin; 1=enable; 0=disable */
    

// Interrupt CTRL REG 2
#define KXTF9_INT_CTRL_REG2_XBW		    0x80	/* X-axis motion mask */
#define KXTF9_INT_CTRL_REG2_YBW		    0x40	/* Y-axis motion mask */
#define KXTF9_INT_CTRL_REG2_ZBW		    0x20	/* Z-axis motion mask */

//Interrupt CTRL REG 3
#define KXTF9_INT_CTRL_REG3_TFUM		0x01	/* Z positive tap detection mask */
#define KXTF9_INT_CTRL_REG3_TFDM		0x02	/* Z negative tap detection mask */
#define KXTF9_INT_CTRL_REG3_TUPM		0x04	/* Y positive tap detection mask */
#define KXTF9_INT_CTRL_REG3_TDOM		0x08	/* Y negative tap detection mask */
#define KXTF9_INT_CTRL_REG3_TRIM		0x10	/* X positive tap detection mask */
#define KXTF9_INT_CTRL_REG3_TLEM		0x20	/* X negative tap detection mask */

// Data Control Register
#define KXTF9_DATA_CTRL_REG_OSAC		0x01	/* sets LPF Acceleration output data rates */
#define KXTF9_DATA_CTRL_REG_OSAB		0x02	/* sets LPF Acceleration output data rates */
#define KXTF9_DATA_CTRL_REG_OSAA		0x04	/* sets LPF Acceleration output data rates */
#define KXTF9_DATA_CTRL_REG_HPFROB		0x10	/* sets HPF roll-off frequency */
#define KXTF9_DATA_CTRL_REG_HPFROA		0x20 	/* sets HPF roll-off frequency */


/*==================================================================================================
                                   COMMON  FUNCTION PROTOTYPES
==================================================================================================*/

//Initialization
#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN kxtf9_init(ACCEL_INFO *accel_info, int enable);

#ifdef	__cplusplus
}
#endif
//extern BOOLEAN kxtf9_init(ACCEL_INFO *accel_info, int enable);
/************ Control Functions ***********/
//extern BOOLEAN kxtf9_enable_outputs(void);

#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN kxtf9_sleep(void);

#ifdef	__cplusplus
}
#endif
//extern BOOLEAN kxtf9_sleep(void);

#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN kxtf9_reset(void);

#ifdef	__cplusplus
}
#endif
//extern BOOLEAN kxtf9_reset(void);

#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN kxtf9_enable_tilt(void);

#ifdef	__cplusplus
}
#endif
//extern BOOLEAN kxtf9_enable_tilt(void);

#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN kxtf9_disable_tilt(void);

#ifdef	__cplusplus
}
#endif
//extern BOOLEAN kxtf9_disable_tilt(void);

/************ Timer / Threshold / Angle ***********/
#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN kxtf9_tilt_timer(INT8U tilt_timer);

#ifdef	__cplusplus
}
#endif

//extern BOOLEAN kxtf9_tilt_timer(INT8U tilt_timer);
#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN kxtf9_motion_wuf_timer(INT8U wuf_timer);

#ifdef	__cplusplus
}
#endif
//extern BOOLEAN kxtf9_motion_wuf_timer(INT8U wuf_timer);

#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN kxtf9_motion_wuf_threshold(INT8U wuf_threshold); 

#ifdef	__cplusplus
}
#endif
//extern BOOLEAN kxtf9_motion_wuf_threshold(INT8U wuf_threshold); 
#ifdef 	__cplusplus
	extern 	"C"	{
#endif 

extern BOOLEAN kxtf9_set_tilt_angle(INT8U tilt_angle);

#ifdef 	__cplusplus
}
#endif

//extern BOOLEAN kxtf9_set_tilt_angle(INT8U tilt_angle);
extern BOOLEAN kxtf9_set_hysteresis(INT8U hysteresis);


/************ Read Functions ***********/
extern BOOLEAN kxtf9_read_previous_tilt_position(INT8U *previous_position);
//extern BOOLEAN kxtf9_read_current_tilt_position(INT8U *current_position);

//Interrupt
//extern BOOLEAN kxtf9_read_interrupt_status(void);
//extern BOOLEAN kxtf9_service_interrupt(INT8U  *source_of_interrupt);





/************  Masking Functions ***********/

//motion activity (WUF) state masking
extern BOOLEAN kxtf9_motion_act_mask_x(void); 
extern BOOLEAN kxtf9_motion_act_mask_y(void);
extern BOOLEAN kxtf9_motion_act_mask_z(void); 
extern BOOLEAN kxtf9_motion_act_unmask_x(void);
extern BOOLEAN kxtf9_motion_act_unmask_y(void);
extern BOOLEAN kxtf9_motion_act_unmask_z(void);

//tilt position state masking
extern BOOLEAN kxtf9_tilt_pos_mask_fu(void);
extern BOOLEAN kxtf9_tilt_pos_mask_fd(void);
extern BOOLEAN kxtf9_tilt_pos_mask_up(void);
extern BOOLEAN kxtf9_tilt_pos_mask_do(void);
extern BOOLEAN kxtf9_tilt_pos_mask_ri(void);
extern BOOLEAN kxtf9_tilt_pos_mask_le(void);
extern BOOLEAN kxtf9_tilt_pos_unmask_fu(void);
extern BOOLEAN kxtf9_tilt_pos_unmask_fd(void);
extern BOOLEAN kxtf9_tilt_pos_unmask_up(void);
extern BOOLEAN kxtf9_tilt_pos_unmask_do(void);
extern BOOLEAN kxtf9_tilt_pos_unmask_ri(void);
extern BOOLEAN kxtf9_tilt_pos_unmask_le(void);


/*==================================================================================================
                                   KXTF9-SPECIFIC FUNCTIONS
==================================================================================================*/

/****************   Control Functions   ******************/

//Enable Controls  
extern BOOLEAN kxtf9_enable_all(void);
extern BOOLEAN kxtf9_disable_all(void);

#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN kxtf9_enable_motion_act_function(void);

#ifdef	__cplusplus
}
#endif
//extern BOOLEAN kxtf9_enable_motion_act_function(void);

#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN kxtf9_disable_motion_act_function(void);

#ifdef	__cplusplus
}
#endif

//extern BOOLEAN kxtf9_disable_motion_act_function(void);

//TAP Controls 
#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN kxtf9_enable_tap_detection(void);

#ifdef	__cplusplus
}
#endif
//extern BOOLEAN kxtf9_enable_tap_detection(void);

#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN kxtf9_disable_tap_detection(void);

#ifdef	__cplusplus
}
#endif   

//extern BOOLEAN kxtf9_disable_tap_detection(void);
extern BOOLEAN kxtf9_read_single_tap_status(void);
extern BOOLEAN kxtf9_read_double_tap_status(void);

//Set Sensor G Range and resolution
//extern BOOLEAN kxtf9_set_G_range(int range);
//extern BOOLEAN kxtf9_set_resolution(int resolution);


/************ ODR ***********/
#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN kxtf9_set_motion_wuf_odr(int frequency);

#ifdef	__cplusplus
}
#endif

//extern BOOLEAN kxtf9_set_motion_wuf_odr(int frequency);               //Set Bits OWUFA and OWUFB of CTRL_REG3

#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN kxtf9_set_tilt_odr(int frequency);

#ifdef	__cplusplus
}
#endif

//extern BOOLEAN kxtf9_set_tilt_odr(int frequency);                     //Set Bits OTPA  and  OTPB of CTRL_REG3
#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN kxtf9_set_tap_odr(int frequency);

#ifdef	__cplusplus
}
#endif
//extern BOOLEAN kxtf9_set_tap_odr(int frequency);                      //Set Bits OTDTA and OTDTB of CTRL_REG3
extern BOOLEAN kxtf9_set_roll_off_hpf_freq(int alpha);                //Set Bits HPFROA and HPFROB of DATA_CTRL_REG
extern BOOLEAN kxtf9_set_lpf_odr(int frequency);                      //Set Bits OSAA, OSAB and OSAC of DATA_CTRL_REG

extern BOOLEAN kxtf9_read_current_motion_odr(int* ODR_rate_motion);   //Read Bits OWUFA and  OWUFB of CTRL_REG3
extern BOOLEAN kxtf9_read_current_tilt_odr(double* ODR_rate_tilt);    //Read Bits OTPA  and   OTPB of CTRL_REG3
extern BOOLEAN kxtf9_read_current_tap_odr(int* ODR_rate_tap);         //Read Bits OTDTA and  OTDTB of CTRL_REG3


/************ Timer / Threshold / Angle ***********/
#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN kxtf9_tdt_timer(int tdt_timer);

#ifdef	__cplusplus
}
#endif
//extern BOOLEAN kxtf9_tdt_timer(int tdt_timer);


#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN kxtf9_tdt_h_thresh(int tdt_l_thresh);

#ifdef	__cplusplus
}
#endif
//extern BOOLEAN kxtf9_tdt_h_thresh(int tdt_h_thresh);

#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN kxtf9_tdt_l_thresh(int tdt_l_thresh);

#ifdef	__cplusplus
}
#endif
//extern BOOLEAN kxtf9_tdt_l_thresh(int tdt_l_thresh);

#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN kxtf9_tdt_tap_timer(int tdt_tap_timer);

#ifdef	__cplusplus
}
#endif
//extern BOOLEAN kxtf9_tdt_tap_timer(int tdt_tap_timer);
extern BOOLEAN kxtf9_tdt_total_timer(int tdt_total_timer);

#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN kxtf9_tdt_latency_timer(int tdt_latency_timer);

#ifdef	__cplusplus
}
#endif

//extern BOOLEAN kxtf9_tdt_latency_timer(int tdt_latency_timer);

#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN kxtf9_tdt_window_timer(int tdt_window_timer);

#ifdef	__cplusplus
}
#endif
//extern BOOLEAN kxtf9_tdt_window_timer(int tdt_window_timer);


/************ Read Functions ***********/
extern BOOLEAN kxtf9_read_position_status(void);                        //Read TPS bit of INT_SRC_REG2
extern BOOLEAN kxtf9_read_motion_wuf_status(void);                      //Read WUFS bit of INT_SRC_REG2
extern BOOLEAN kxtf9_read_LPF_cnt(int* x, int* y, int* z);           
extern BOOLEAN kxtf9_read_LPF_g(int* gx, int* gy, int* gz);         
extern BOOLEAN kxtf9_read_HPF_cnt(int* x, int* y, int* z);
extern BOOLEAN kxtf9_read_HPF_g(int* gx, int* gy, int* gz);

#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN kxtf9_read_tap_direction(INT8U* int_src_reg1);

#ifdef	__cplusplus
}
#endif
//extern BOOLEAN kxtf9_read_tap_direction(INT8U* int_src_reg1);           //Read INT_SRC_REG1


/************ interrupt control ***********/
#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN kxtf9_enable_int(void);

#ifdef	__cplusplus
}
#endif
//extern BOOLEAN kxtf9_enable_int(void);

#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN kxtf9_disable_int(void);

#ifdef	__cplusplus
}
#endif
//extern BOOLEAN kxtf9_disable_int(void);

#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN kxtf9_int_activeh(void);

#ifdef	__cplusplus
}
#endif
//extern BOOLEAN kxtf9_int_activeh(void);

#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN kxtf9_int_activel(void);

#ifdef	__cplusplus
}
#endif
//extern BOOLEAN kxtf9_int_activel(void);

#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN kxtf9_int_latch(void);

#ifdef	__cplusplus
}
#endif
//extern BOOLEAN kxtf9_int_latch(void);

#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN kxtf9_int_pulse(void);

#ifdef	__cplusplus
}
#endif
//extern BOOLEAN kxtf9_int_pulse(void);
//Set latched response of Motion Detection
extern BOOLEAN kxtf9_int_alt_disable(void);
extern BOOLEAN kxtf9_int_alt_enable(void);


/************  Masking Functions ***********/
extern BOOLEAN kxtf9_tap_mask_TFU(void);
extern BOOLEAN kxtf9_tap_unmask_TFU(void);
extern BOOLEAN kxtf9_tap_mask_TFD(void);
extern BOOLEAN kxtf9_tap_unmask_TFD(void);
extern BOOLEAN kxtf9_tap_mask_TUP(void);
extern BOOLEAN kxtf9_tap_unmask_TUP(void);
extern BOOLEAN kxtf9_tap_mask_TDO(void);
extern BOOLEAN kxtf9_tap_unmask_TDO(void);
extern BOOLEAN kxtf9_tap_mask_TRI(void);
extern BOOLEAN kxtf9_tap_unmask_TRI(void);
extern BOOLEAN kxtf9_tap_mask_TLE(void);
extern BOOLEAN kxtf9_tap_unmask_TLE(void);


/// add by xzb


#ifdef	__cplusplus
	extern	"C"	{
#endif

extern void accel_kionix_init(void);

#ifdef	__cplusplus
}
#endif




#ifdef	__cplusplus
	extern	"C"	{
#endif

extern void interrupt_service_for_accel(void);

#ifdef	__cplusplus
}
#endif


#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN  kxtf9_read_current_tilt_position(INT8U *current_position);

#ifdef	__cplusplus
}
#endif

#ifdef	__cplusplus
	extern	"C"	{
#endif

//extern INT8U kxtf9_read_reg_value(INT8U reg);
extern BOOLEAN kxtf9_read_reg_value(INT8U base_reg, INT8U read_reg_counts, INT8U *data);

#ifdef	__cplusplus
}
#endif



#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN kxtf9_service_interrupt(INT8U  *source_of_interrupt);

#ifdef	__cplusplus
}
#endif


#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN kxtf9_read_interrupt_status(void);

#ifdef	__cplusplus
}
#endif


#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN kxtf9_enable_outputs(void);

#ifdef	__cplusplus
}
#endif


#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN kxtf9_set_G_range(int range);

#ifdef	__cplusplus
}
#endif


#ifdef	__cplusplus
	extern	"C"	{
#endif

extern BOOLEAN kxtf9_set_resolution(int resolution);

#ifdef	__cplusplus
}
#endif




/// end




#endif //_KIONIX_KXTF9_DRIVER_H_

