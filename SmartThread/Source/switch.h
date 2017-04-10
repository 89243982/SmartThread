#ifndef SMARTTHREAD_SWITCH_H
#define SMARTTHREAD_SWITCH_H
#include <stdint.h>
void bthread_set_systick_period (uint32_t ms);
void bthread_run_first ();
void bthread_switch ();
uint32_t bthread_enter_critical();
void bthread_exit_critical(uint32_t status);
#endif