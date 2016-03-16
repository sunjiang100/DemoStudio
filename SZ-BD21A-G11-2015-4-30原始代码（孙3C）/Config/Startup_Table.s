;/*----------------------------------------------------------------------------
; *      Name:    STARTUP_TABLE.S
; *      Purpose: Pre-defined STARTUP Table
;/*----------------------------------------------------------------------------
; *      Name:    STARTUP_TABLE.S
; *      Purpose: Pre-defined STARTUP Table
; *---------------------------------------------------------------------------*/
                PRESERVE8                       ; 8-Byte aligned Stack
                AREA    STARTUP_TABLE, CODE, READONLY

                IMPORT	Reset_Handler
				EXPORT  VIC_Vect_Addr0  
;用户ROM当前中断向量地址VIC_Vect_Addr0~VIC_Vect_Addr15
			    IMPORT  os_clock_interrupt
VIC_Vect_Addr0  DCD     os_clock_interrupt
			    IMPORT  Reset_Handler
VIC_Vect_Addr1  DCD     Reset_Handler             
;			    IMPORT  Reset_Handler
VIC_Vect_Addr2  DCD     Reset_Handler
;			    IMPORT  Reset_Handler
VIC_Vect_Addr3  DCD     Reset_Handler
;			    IMPORT  Reset_Handler
VIC_Vect_Addr4  DCD     os_clock_interrupt             
			    IMPORT  IRQ_TIMER1
VIC_Vect_Addr5  DCD     IRQ_TIMER1             
			    IMPORT  IRQ_UART0
VIC_Vect_Addr6  DCD     IRQ_UART0             
			    IMPORT  IRQ_UART1
VIC_Vect_Addr7  DCD     IRQ_UART1             
;			    IMPORT  Reset_Handler
VIC_Vect_Addr8  DCD     Reset_Handler             
;			    IMPORT  IRQ_IIC
VIC_Vect_Addr9  DCD     Reset_Handler;IRQ_IIC
;			    IMPORT  Reset_Handler
VIC_Vect_Addr10 DCD     Reset_Handler             
;			    IMPORT  Reset_Handler
VIC_Vect_Addr11 DCD     Reset_Handler             
;			    IMPORT  Reset_Handler
VIC_Vect_Addr12 DCD     Reset_Handler             
;			    IMPORT  IRQ_RTC
VIC_Vect_Addr13 DCD     Reset_Handler;IRQ_RTC             
;			    IMPORT  Reset_Handler
VIC_Vect_Addr14 DCD     Reset_Handler             
;			    IMPORT  Reset_Handler
VIC_Vect_Addr15 DCD     Reset_Handler             
;			    IMPORT  Reset_Handler
VIC_Vect_Addr16 DCD     Reset_Handler             
;			    IMPORT  Reset_Handler
VIC_Vect_Addr17 DCD     Reset_Handler             
;			    IMPORT  Reset_Handler
VIC_Vect_Addr18 DCD     Reset_Handler             
;			    IMPORT  Reset_Handler
VIC_Vect_Addr19 DCD     Reset_Handler             
;			    IMPORT  Reset_Handler
VIC_Vect_Addr20 DCD     Reset_Handler             
;			    IMPORT  Reset_Handler
VIC_Vect_Addr21 DCD     Reset_Handler             
;			    IMPORT  Reset_Handler
VIC_Vect_Addr22 DCD     Reset_Handler             
			    IMPORT  IRQ_CAN
VIC_Vect_Addr23 DCD     IRQ_CAN
;			    IMPORT  Reset_Handler
VIC_Vect_Addr24 DCD     Reset_Handler             
;			    IMPORT  Reset_Handler
VIC_Vect_Addr25 DCD     Reset_Handler             
;			    IMPORT  Reset_Handler
VIC_Vect_Addr26 DCD     Reset_Handler             
;			    IMPORT  Reset_Handler
VIC_Vect_Addr27 DCD     Reset_Handler             
			    IMPORT  IRQ_UART2
VIC_Vect_Addr28 DCD     IRQ_UART2
			    IMPORT   IRQ_UART3
VIC_Vect_Addr29 DCD     IRQ_UART3
;			    IMPORT  Reset_Handler
VIC_Vect_Addr30 DCD     Reset_Handler             
;			    IMPORT  Reset_Handler
VIC_Vect_Addr31 DCD     Reset_Handler             
                END
