



/*



static uint32_t  iserbuffer0, iserbuffer1;

static __INLINE void OS_ENTER_CRITICAL(void)
{
    iserbuffer0 = NVIC->ISER[0];//&(~((3<<16)|(3<<3)|(1<<29)));					 //��PPL0 RTC RTI TIM2 TIM3�������жϽ���	 ��OS_EXIT_CRITICAL���ʹ��
	 NVIC->ICER[(uint32_t)(0)] =  0xffffffff;//iserbuffer0; // Disable  all  IRQ interrupt 
	 iserbuffer1 = NVIC->ISER[1];
	 NVIC->ICER[(uint32_t)(1)] =  0xffffffff;//iserbuffer1;
}

static __INLINE void OS_EXIT_CRITICAL(void)
{
   
	 NVIC->ISER[(uint32_t)(0)] |=  iserbuffer0; //enable  all  IRQ  interrupt
	 NVIC->ISER[(uint32_t)(1)] |=  iserbuffer1;
}



*/