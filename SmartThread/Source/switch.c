#include <ARMCM3.h>
#include <stdint.h>


#define	NVIC_INT_CTRL			0xE000Ed04
#define	NVIC_PENDSVSET			0x10000000
#define	NVIC_SYSPRI2			0xE000ED22
#define	NVIC_PENDSV_PRI			0x000000FF

#define MEM32(addr)         *(volatile uint32_t *)(addr)
#define MEM8(addr)          *(volatile uint8_t *)(addr)

struct bThread;
extern struct bThread* currentThread;
extern struct bThread* nextThread;

__asm void PendSV_Handler (void)
{
	IMPORT currentThread
	IMPORT nextThread
	
	MRS R0, PSP
	CBZ R0, threadstate_nosave
	
	STMDB R0!, {R4-R11}
	
	LDR R1, =currentThread
	LDR R1, [R1]
	STR R0, [R1]
	
threadstate_nosave
	LDR R0, =currentThread
	LDR R1, =nextThread
	LDR R2, [R1]
	STR R2, [R0]
	
	LDR R0, [R2]
	LDMIA R0!, {R4-R11}
	
	MSR PSP, R0
	ORR LR, LR, #0x04
	BX LR
}

void bthread_run_first ()
{
	__set_PSP(0);
	MEM8(NVIC_SYSPRI2) = NVIC_PENDSV_PRI;
	MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;
}
void bthread_switch ()
{
	MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;
}
void bthread_set_systick_period (uint32_t ms)
{
	SysTick->LOAD = ms * SystemCoreClock / 1000 - 1;
	NVIC_SetPriority(SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1);
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
					SysTick_CTRL_TICKINT_Msk |
					SysTick_CTRL_ENABLE_Msk;
}

uint32_t bthread_enter_critical()
{
	uint32_t primask = __get_PRIMASK();
	__disable_irq();
	return primask;
}


void bthread_exit_critical(uint32_t status)
{
	__set_PRIMASK(status);
}




