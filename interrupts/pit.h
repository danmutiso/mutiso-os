#ifndef PIT_H
#define PIT_H

#include <stdint.h>

void init_pit(void);
void pit_handler(void);
uint32_t pit_get_ticks(void);
uint32_t pit_get_seconds(void);
uint32_t pit_get_minutes(void);
uint32_t pit_get_hours(void);
void pit_sleep_ms(uint32_t);

extern void halt_cpu(void);

#endif
