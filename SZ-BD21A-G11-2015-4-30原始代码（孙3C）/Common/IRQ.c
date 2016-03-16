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

#define		uint32_t		unsigned int 
                                                               /*  __cplusplus                 */

/*********************************************************************************************************
** Function name:       OS_NVIC_SetPriorityGrouping
** Descriptions:        ����NVIC���ȼ����飨���÷���ʱ����д��������У�
** input parameters:    PriorityGroup - ���ȼ�������
** output parameters:   ��
** Returned value:      ��
**                      __NVIC_PRIO_BITS����������
*********************************************************************************************************/
static __INLINE void OS_NVIC_SetPriorityGrouping(uint32_t PriorityGroup)
{
  uint32_t reg_value;
  uint32_t PriorityGroupTmp = (PriorityGroup & 0x07);                   /* ֻ�С�0~7����ʹ��           */
  
  changeToSYSMode();
  reg_value  = SCB->AIRCR;                                              /* ��ԭ�Ĵ�������               */
  reg_value &= ~((0xFFFFU << 16) | (0x0F << 8));                        /* clear bits to change         */
  reg_value  = ((reg_value | NVIC_AIRCR_VECTKEY | (PriorityGroupTmp << 8)));
                                                                        /* ����VECTKEY�����ȼ�������ֵ  */
  SCB->AIRCR = reg_value;
  changeToUSRMode();
}

/*********************************************************************************************************
** Function name:       OS_NVIC_GetPriorityGrouping
** Descriptions:        ��ȡNVIC���ȼ�����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ����SCB->AIRCR [10:8]�е����ȼ�����
*********************************************************************************************************/
static __INLINE uint32_t OS_NVIC_GetPriorityGrouping(void)
{
  uint32_t PrioGrp;
  changeToSYSMode();
  PrioGrp = ((SCB->AIRCR >> 8) & 0x07);                                 /* read priority grouping field */
  changeToUSRMode();
  return  PrioGrp;
}

/*********************************************************************************************************
** Function name:       OS_NVIC_EnableIRQ
** Descriptions:        ��IRQͨ���ж�
** input parameters:    IRQn - �ж�ͨ����ͨ���Ų����Ǹ�ֵ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
static __INLINE void OS_NVIC_EnableIRQ(IRQn_Type IRQn)
{
  changeToSYSMode();   
  NVIC->ISER[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));/* enable interrupt            */
  changeToUSRMode();
}

/*********************************************************************************************************
** Function name:       OS_NVIC_DisableIRQ
** Descriptions:        �ر�IRQͨ���ж�
** input parameters:    IRQn - �ж�ͨ����ͨ���Ų����Ǹ�ֵ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
static __INLINE void OS_NVIC_DisableIRQ(IRQn_Type IRQn)
{
  changeToSYSMode();  
  NVIC->ICER[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));/* disable interrupt           */
  changeToUSRMode();
}

/*********************************************************************************************************
** Function name:       OS_NVIC_GetPendingIRQ
** Descriptions:        ��ȡPending IRQ
** input parameters:    IRQn - �ж�ͨ����ͨ���Ų����Ǹ�ֵ��
** output parameters:   ��
** Returned value:      1 - �жϴ��ڹ���״̬ 0 - ����
*********************************************************************************************************/
static __INLINE uint32_t OS_NVIC_GetPendingIRQ(IRQn_Type IRQn)
{
  uint32_t IRQPending;
  changeToSYSMode();
  IRQPending = ((uint32_t) ((NVIC->ISPR[(uint32_t)(IRQn) >> 5] & (1 << ((uint32_t)(IRQn) & 0x1F)))?1:0));
                                                                        /* Return 1 if pending else 0   */
  changeToUSRMode();
  return IRQPending;
}

/*********************************************************************************************************
** Function name:       OS_NVIC_SetPendingIRQ
** Descriptions:        ����Pending IRQ
** input parameters:    IRQn - �ж�ͨ����ͨ���Ų����Ǹ�ֵ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
static __INLINE void OS_NVIC_SetPendingIRQ(IRQn_Type IRQn)
{
  changeToSYSMode();
  NVIC->ISPR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));/* set interrupt pending       */
  changeToUSRMode();
}

/*********************************************************************************************************
** Function name:       OS_NVIC_ClearPendingIRQ
** Descriptions:        ���PendIRQ
** input parameters:    IRQn - �ж�ͨ����ͨ���Ų����Ǹ�ֵ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
static __INLINE void OS_NVIC_ClearPendingIRQ(IRQn_Type IRQn)
{
  changeToSYSMode();
  NVIC->ICPR[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F)); 
  changeToUSRMode();
}

/*********************************************************************************************************
** Function name:       OS_NVIC_GetActive
** Descriptions:        ��ȡ��Ч�жϱ�־
** input parameters:    IRQn - �ж�ͨ��
** output parameters:   ��
** Returned value:      1 - �жϱ�־��Ч 0 - ����
*********************************************************************************************************/
static __INLINE uint32_t OS_NVIC_GetActive(IRQn_Type IRQn)
{
  uint32_t IRQFlag;
  changeToSYSMode();  
  IRQFlag = ((uint32_t)((NVIC->IABR[(uint32_t)(IRQn) >> 5] & (1 << ((uint32_t)(IRQn) & 0x1F)))?1:0));
  changeToUSRMode();
  return IRQFlag;
}

/*********************************************************************************************************
** Function name:       OS_NVIC_SetPriority
** Descriptions:        �����ж����ȼ�
** input parameters:    IRQn     - �ж�ͨ����ͨ���ſ�Ϊ�����жϻ��ں˿�����ϵͳ�쳣��  
**                      priority - �������ȼ�
** output parameters:   ��
** Returned value:      ��
**                      ���øú��������ж����ȼ����뱣֤��Ϊ���������ȼ��쳣
*********************************************************************************************************/
static __INLINE void OS_NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority)
{
  changeToSYSMode();
  if(IRQn < 0) {
    /* set Priority for Cortex-M3 System Interrupts */
    SCB->SHP[((uint32_t)(IRQn) & 0xF)-4] = ((priority << (8 - __NVIC_PRIO_BITS)) & 0xff);
   } else {
   /* set Priority for device specific Interrupts      */
    NVIC->IP[(uint32_t)(IRQn)] = ((priority << (8 - __NVIC_PRIO_BITS)) & 0xff);
  }  
  changeToUSRMode();
}

/*********************************************************************************************************
** Function name:       OS_NVIC_GetPriority
** Descriptions:        ���ָ��ͨ�����ж����ȼ�
** input parameters:    IRQn - �ж�ͨ����ͨ���ſ�Ϊ�����жϻ��ں˿�����ϵͳ�쳣��  
** output parameters:   ��
** Returned value:      �����ж����ȼ�
**                      ���øú��������ж����ȼ����뱣֤��Ϊ���������ȼ��쳣
*********************************************************************************************************/
static __INLINE uint32_t OS_NVIC_GetPriority(IRQn_Type IRQn)
{
  uint32_t IRQPrio;
  changeToSYSMode();
  if(IRQn < 0) {
    /* get priority for Cortex-M3 system interrupts */
    IRQPrio = ((uint32_t)(SCB->SHP[((uint32_t)(IRQn) & 0xF)-4] >> (8 - __NVIC_PRIO_BITS)));
  } else {
    /* get priority for device specific interrupts  */
    IRQPrio = ((uint32_t)(NVIC->IP[(uint32_t)(IRQn)]           >> (8 - __NVIC_PRIO_BITS)));
  } 
  changeToUSRMode();
  return IRQPrio;
}

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
                                                 uint32_t SubPriority)
{
  uint32_t PriorityGroupTmp = (PriorityGroup & 0x07);                   /* only values 0..7 are used    */
  uint32_t PreemptPriorityBits;
  uint32_t SubPriorityBits;

  PreemptPriorityBits = ((7 - PriorityGroupTmp) > __NVIC_PRIO_BITS) ? __NVIC_PRIO_BITS : 7 - PriorityGroupTmp;
  SubPriorityBits     = ((PriorityGroupTmp + __NVIC_PRIO_BITS) < 7) ? 0 : PriorityGroupTmp - 7 + __NVIC_PRIO_BITS;
 
  return (
           ((PreemptPriority & ((1 << (PreemptPriorityBits)) - 1)) << SubPriorityBits) |
           ((SubPriority     & ((1 << (SubPriorityBits    )) - 1)))
         );
}

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
                                             uint32_t* pSubPriority)
{
  uint32_t PriorityGroupTmp = (PriorityGroup & 0x07);                   /* only values 0..7 are used    */
  uint32_t PreemptPriorityBits;
  uint32_t SubPriorityBits;

  PreemptPriorityBits = ((7 - PriorityGroupTmp) > __NVIC_PRIO_BITS) ? __NVIC_PRIO_BITS : 7 - PriorityGroupTmp;
  SubPriorityBits     = ((PriorityGroupTmp + __NVIC_PRIO_BITS) < 7) ? 0 : PriorityGroupTmp - 7 + __NVIC_PRIO_BITS;
  
  *pPreemptPriority = (Priority >> SubPriorityBits) & ((1 << (PreemptPriorityBits)) - 1);
  *pSubPriority     = (Priority                   ) & ((1 << (SubPriorityBits    )) - 1);
}

/*********************************************************************************************************
** Function name:       OS_NVIC_SystemReset
** Descriptions:        �����λϵͳ
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
static __INLINE void OS_NVIC_SystemReset(void)
{
  SCB->AIRCR  = (NVIC_AIRCR_VECTKEY | (SCB->AIRCR & (0x700)) | (1<<NVIC_SYSRESETREQ)); 
                                                                        /* Keep priority group unchanged*/
  __DSB();                                                              /* Ensure completion of memory  */
  while(1);                                                             /* access wait until reset      */
}
                                                                 /*  __cplusplus                 */

#endif                                                                  /*  __IRQ_H                     */

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
