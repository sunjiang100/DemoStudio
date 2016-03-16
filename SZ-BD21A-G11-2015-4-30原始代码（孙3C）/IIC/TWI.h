/* $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
$												广州国业电子科技有限公司
$										Shawyoo Electronics (GuangZhou) R&D Center
$                       	http://www.guoyee.com		 Email: guoyegps@126.com
$								 QQ群：9069471/9069433/9524253		All rights reserved.	  
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  Copyright (c)  $$$$$$$$$$$$$$$$$$$$$$$$$$$ */
/*  
                                      文件描述
	文件名: 	 TWI.h
	功能描述：IIC相关常定义
	编译环境：
	目标CPU: 
	作者：
	创建时间：
	建议编辑环境：UltraEdit 11.00a+  制表符宽度为三个字符，程序可读性更好。
$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
*/


#ifndef 		_AVR_TWI_H_
#define		_AVR_TWI_H_ 1

//#include <avr/io.h>

/* TWSR values (not bits) */
/* Master */
#define		TW_MS_START						0x08
#define		TW_MS_REP_START				0x10
/* Master Transmitter */		
#define		TW_MT_SLA_ACK					0x18
#define		TW_MT_SLA_NACK					0x20
#define		TW_MT_DATA_ACK					0x28
#define		TW_MT_DATA_NACK				0x30
#define		TW_MT_ARB_LOST					0x38
/* Master Receiver */
#define		TW_MR_ARB_LOST					0x38
#define		TW_MR_SLA_ACK					0x40
#define		TW_MR_SLA_NACK					0x48
#define		TW_MR_DATA_ACK					0x50
#define		TW_MR_DATA_NACK				0x58
/* Slave Transmitter */
#define		TW_ST_SLA_ACK					0xA8
#define		TW_ST_ARB_LOST_SLA_ACK		0xB0
#define		TW_ST_DATA_ACK					0xB8
#define		TW_ST_DATA_NACK				0xC0
#define		TW_ST_LAST_DATA				0xC8
/* Slave Receiver */											 
#define		TW_SR_SLA_ACK					0x60
#define		TW_SR_ARB_LOST_SLA_ACK		0x68
#define		TW_SR_GCALL_ACK				0x70
#define		TW_SR_ARB_LOST_GCALL_ACK 	0x78
#define		TW_SR_DATA_ACK					0x80
#define		TW_SR_DATA_NACK				0x88
#define		TW_SR_GCALL_DATA_ACK			0x90
#define		TW_SR_GCALL_DATA_NACK		0x98
#define		TW_SR_STOP						0xA0
/* Misc */
#define		TW_NO_INFO						0xF8
#define		TW_BUS_ERROR					0x00


#define		TW_AA_FLAG						2
#define		TW_INT_FLAG						0x28
#define		TW_STOP							4
#define		TW_START							0x20
#define		TW_ENABLE						6



///		LPC2xxx I2C Control Set Register I2CONSET bit define
//#define 		TestAck() 	  		(TWSR&0xf8)

#define 		TW_Ack()	  			(LPC_I2C0->I2CONSET = 1<<2)
#define 		TW_NoAck()    		(LPC_I2C0->I2CONCLR = 1<<2)
#define		TW_Start()			(LPC_I2C0->I2CONSET = 1<<5)
#define 		TW_Stop()		  	(LPC_I2C0->I2CONSET = 1<<4)

/*
 * R/~W bit in SLA+R/W address field.
 */
#define		TW_READ				1
#define		TW_WRITE				0

#endif  /* _AVR_TWI_H_ */

/*
 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$  End of File $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
 */
