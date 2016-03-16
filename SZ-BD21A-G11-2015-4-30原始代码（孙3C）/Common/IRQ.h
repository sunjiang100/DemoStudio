/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               IRQ.h
** Latest modified Date:    2011-01-14
** Latest Version:          1.00
** Descriptions:            �ж����ñ�������Ȩģʽ����
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
** Descriptions:        ����NVIC���ȼ����飨���÷���ʱ����д��������У�
** input parameters:    PriorityGroup - ���ȼ�������
** output parameters:   ��
** Returned value:      ��
**                      __NVIC_PRIO_BITS����������
*********************************************************************************************************/
static __INLINE void OS_NVIC_SetPriorityGrouping(uint32_t PriorityGroup);

/*********************************************************************************************************
** Function name:       OS_NVIC_GetPriorityGrouping
** Descriptions:        ��ȡNVIC���ȼ�����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ����SCB->AIRCR [10:8]�е����ȼ�����
*********************************************************************************************************/
static __INLINE uint32_t OS_NVIC_GetPriorityGrouping(void);

/*********************************************************************************************************
** Function name:       OS_NVIC_EnableIRQ
** Descriptions:        ��IRQͨ���ж�
** input parameters:    IRQn - �ж�ͨ����ͨ���Ų����Ǹ�ֵ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
static __INLINE void OS_NVIC_EnableIRQ(IRQn_Type IRQn);

/*********************************************************************************************************
** Function name:       OS_NVIC_DisableIRQ
** Descriptions:        �ر�IRQͨ���ж�
** input parameters:    IRQn - �ж�ͨ����ͨ���Ų����Ǹ�ֵ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
static __INLINE void OS_NVIC_DisableIRQ(IRQn_Type IRQn);

/*********************************************************************************************************
** Function name:       OS_NVIC_GetPendingIRQ
** Descriptions:        ��ȡPending IRQ
** input parameters:    IRQn - �ж�ͨ����ͨ���Ų����Ǹ�ֵ��
** output parameters:   ��
** Returned value:      1 - �жϴ��ڹ���״̬ 0 - ����
*********************************************************************************************************/
static __INLINE uint32_t OS_NVIC_GetPendingIRQ(IRQn_Type IRQn);

/*********************************************************************************************************
** Function name:       OS_NVIC_SetPendingIRQ
** Descriptions:        ����Pending IRQ
** input parameters:    IRQn - �ж�ͨ����ͨ���Ų����Ǹ�ֵ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
static __INLINE void OS_NVIC_SetPendingIRQ(IRQn_Type IRQn);

/*********************************************************************************************************
** Function name:       OS_NVIC_ClearPendingIRQ
** Descriptions:        ���PendIRQ
** input parameters:    IRQn - �ж�ͨ����ͨ���Ų����Ǹ�ֵ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
static __INLINE void OS_NVIC_ClearPendingIRQ(IRQn_Type IRQn);

/*********************************************************************************************************
** Function name:       OS_NVIC_GetActive
** Descriptions:        ��ȡ��Ч�жϱ�־
** input parameters:    IRQn - �ж�ͨ��
** output parameters:   ��
** Returned value:      1 - �жϱ�־��Ч 0 - ����
*********************************************************************************************************/
static __INLINE uint32_t OS_NVIC_GetActive(IRQn_Type IRQn);

/*********************************************************************************************************
** Function name:       OS_NVIC_SetPriority
** Descriptions:        �����ж����ȼ�
** input parameters:    IRQn     - �ж�ͨ����ͨ���ſ�Ϊ�����жϻ��ں˿�����ϵͳ�쳣��  
**                      priority - �������ȼ�
** output parameters:   ��
** Returned value:      ��
**                      ���øú��������ж����ȼ����뱣֤��Ϊ���������ȼ��쳣
*********************************************************************************************************/
static __INLINE void OS_NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority);

/*********************************************************************************************************
** Function name:       OS_NVIC_GetPriority
** Descriptions:        ���ָ��ͨ�����ж����ȼ�
** input parameters:    IRQn - �ж�ͨ����ͨ���ſ�Ϊ�����жϻ��ں˿�����ϵͳ�쳣��  
** output parameters:   ��
** Returned value:      �����ж����ȼ�
**                      ���øú��������ж����ȼ����뱣֤��Ϊ���������ȼ��쳣
*********************************************************************************************************/
static __INLINE uint32_t OS_NVIC_GetPriority(IRQn_Type IRQn);

/*********************************************************************************************************
** Function name:       OS_NVIC_EncodePriority
** Descriptions:        �����ж����ȼ�
** input parameters:    PriorityGroup - �ж����ȼ���
**                      PreemptPriority - ��ռ���ȼ�
**                      SubPriority - �����ȼ�
**                      __NVIC_PRIO_BITS���������ȼ������������  
** output parameters:   ��
** Returned value:      �����ж����ȼ�
**                      �������ȼ�������NVIC_SetPriority()������ڲ��������ж����ȼ�
*********************************************************************************************************/
static __INLINE uint32_t OS_NVIC_EncodePriority (uint32_t PriorityGroup, 
                                                 uint32_t PreemptPriority,
                                                 uint32_t SubPriority);

/*********************************************************************************************************
** Function name:       OS_NVIC_DecodePriority
** Descriptions:        �����ж����ȼ�
** input parameters:    Priority - �������ȼ�
**                      PrioGroup - �����ж����ȼ���
**                      pPreemptPrio - ������ռ���ȼ�
**                      pSubPrio - ���������ȼ�
**                      __NVIC_PRIO_BITS���������ȼ������������  
** output parameters:   ��
** Returned value:      �����ж����ȼ�
**                      ���������ȼ������ɴ�NVIC_GetPriority()�������ػ��
*********************************************************************************************************/
static __INLINE void OS_NVIC_DecodePriority (uint32_t Priority, 
                                             uint32_t PriorityGroup, 
                                             uint32_t* pPreemptPriority,
                                             uint32_t* pSubPriority);

/*********************************************************************************************************
** Function name:       OS_NVIC_SystemReset
** Descriptions:        �����λϵͳ
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
static __INLINE void OS_NVIC_SystemReset(void);

#ifdef __cplusplus
}
#endif                                                                  /*  __cplusplus                 */

#endif                                                                  /*  __IRQ_H                     */

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
