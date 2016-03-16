/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               IRQ.h
** Latest modified Date:    2011-01-14
** Latest Version:          1.00
** Descriptions:            中断配置必须在特权模式运行
**
**--------------------------------------------------------------------------------------------------------
** Created by:              Lanwuqiang
** Created date:            2011-01-14
** Version:                 1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
** Version:
** Descriptions:
**
*********************************************************************************************************/
#ifndef __IRQ_H
#define __IRQ_H

//#include 	"..\CM3\core_cm3.h"

#ifdef __cplusplus
extern "C" {
#endif                                                                  /*  __cplusplus                 */

/*
 * System Reset
 */
#define NVIC_VECTRESET              0                                   /*!< Vector Reset Bit           */
#define NVIC_SYSRESETREQ            2                                   /*!< System Reset Request       */
#define NVIC_AIRCR_VECTKEY         (0x5FA << 16)                        /*!< AIRCR Key for write access */
#define NVIC_AIRCR_ENDIANESS        15                                  /*!< Endianess                  */

/*********************************************************************************************************
** Function name:       OS_NVIC_SetPriorityGrouping
** Descriptions:        设置NVIC优先级分组（配置分组时必须写入解锁序列）
** input parameters:    PriorityGroup - 优先级分组域
** output parameters:   无
** Returned value:      无
**                      __NVIC_PRIO_BITS决定分组数
*********************************************************************************************************/
static __INLINE void OS_NVIC_SetPriorityGrouping(uint32_t PriorityGroup);

/*********************************************************************************************************
** Function name:       OS_NVIC_GetPriorityGrouping
** Descriptions:        获取NVIC优先级分组
** input parameters:    无
** output parameters:   无
** Returned value:      返回SCB->AIRCR [10:8]中的优先级分组
*********************************************************************************************************/
static __INLINE uint32_t OS_NVIC_GetPriorityGrouping(void);

/*********************************************************************************************************
** Function name:       OS_NVIC_EnableIRQ
** Descriptions:        打开IRQ通道中断
** input parameters:    IRQn - 中断通道（通道号不能是负值）
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static __INLINE void OS_NVIC_EnableIRQ(IRQn_Type IRQn);

/*********************************************************************************************************
** Function name:       OS_NVIC_DisableIRQ
** Descriptions:        关闭IRQ通道中断
** input parameters:    IRQn - 中断通道（通道号不能是负值）
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static __INLINE void OS_NVIC_DisableIRQ(IRQn_Type IRQn);

/*********************************************************************************************************
** Function name:       OS_NVIC_GetPendingIRQ
** Descriptions:        获取Pending IRQ
** input parameters:    IRQn - 中断通道（通道号不能是负值）
** output parameters:   无
** Returned value:      1 - 中断处于挂起状态 0 - 其他
*********************************************************************************************************/
static __INLINE uint32_t OS_NVIC_GetPendingIRQ(IRQn_Type IRQn);

/*********************************************************************************************************
** Function name:       OS_NVIC_SetPendingIRQ
** Descriptions:        配置Pending IRQ
** input parameters:    IRQn - 中断通道（通道号不能是负值）
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static __INLINE void OS_NVIC_SetPendingIRQ(IRQn_Type IRQn);

/*********************************************************************************************************
** Function name:       OS_NVIC_ClearPendingIRQ
** Descriptions:        清除PendIRQ
** input parameters:    IRQn - 中断通道（通道号不能是负值）
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static __INLINE void OS_NVIC_ClearPendingIRQ(IRQn_Type IRQn);

/*********************************************************************************************************
** Function name:       OS_NVIC_GetActive
** Descriptions:        获取有效中断标志
** input parameters:    IRQn - 中断通道
** output parameters:   无
** Returned value:      1 - 中断标志有效 0 - 其他
*********************************************************************************************************/
static __INLINE uint32_t OS_NVIC_GetActive(IRQn_Type IRQn);

/*********************************************************************************************************
** Function name:       OS_NVIC_SetPriority
** Descriptions:        配置中断优先级
** input parameters:    IRQn     - 中断通道（通道号可为外设中断或内核可配置系统异常）  
**                      priority - 配置优先级
** output parameters:   无
** Returned value:      无
**                      调用该函数配置中断优先级必须保证其为可配置优先级异常
*********************************************************************************************************/
static __INLINE void OS_NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority);

/*********************************************************************************************************
** Function name:       OS_NVIC_GetPriority
** Descriptions:        获得指定通道的中断优先级
** input parameters:    IRQn - 中断通道（通道号可为外设中断或内核可配置系统异常）  
** output parameters:   无
** Returned value:      返回中断优先级
**                      调用该函数配置中断优先级必须保证其为可配置优先级异常
*********************************************************************************************************/
static __INLINE uint32_t OS_NVIC_GetPriority(IRQn_Type IRQn);

/*********************************************************************************************************
** Function name:       OS_NVIC_EncodePriority
** Descriptions:        编码中断优先级
** input parameters:    PriorityGroup - 中断优先级组
**                      PreemptPriority - 抢占优先级
**                      SubPriority - 子优先级
**                      __NVIC_PRIO_BITS：决定优先级分组可配置数  
** output parameters:   无
** Returned value:      返回中断优先级
**                      返回优先级可用于NVIC_SetPriority()函数入口参数配置中断优先级
*********************************************************************************************************/
static __INLINE uint32_t OS_NVIC_EncodePriority (uint32_t PriorityGroup, 
                                                 uint32_t PreemptPriority,
                                                 uint32_t SubPriority);

/*********************************************************************************************************
** Function name:       OS_NVIC_DecodePriority
** Descriptions:        解码中断优先级
** input parameters:    Priority - 解密优先级
**                      PrioGroup - 返回中断优先级组
**                      pPreemptPrio - 返回抢占优先级
**                      pSubPrio - 返回子优先级
**                      __NVIC_PRIO_BITS：决定优先级分组可配置数  
** output parameters:   无
** Returned value:      返回中断优先级
**                      待解密优先级参数可从NVIC_GetPriority()函数返回获得
*********************************************************************************************************/
static __INLINE void OS_NVIC_DecodePriority (uint32_t Priority, 
                                             uint32_t PriorityGroup, 
                                             uint32_t* pPreemptPriority,
                                             uint32_t* pSubPriority);

/*********************************************************************************************************
** Function name:       OS_NVIC_SystemReset
** Descriptions:        软件复位系统
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static __INLINE void OS_NVIC_SystemReset(void);

#ifdef __cplusplus
}
#endif                                                                  /*  __cplusplus                 */

#endif                                                                  /*  __IRQ_H                     */

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
