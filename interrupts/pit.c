#include "../drivers/port.h"
#include "pit.h"
#include "pic.h"

#define PIT_FREQUENCY 1193182
#define PIT_TARGET_FREQUENCY 100

static volatile uint32_t timer_ticks = 0;
static uint32_t last_displayed_tick = 0;

void init_pit(void){

    uint16_t divisor = PIT_FREQUENCY / PIT_TARGET_FREQUENCY;

    outb(0x43, 0x36);
    outb(0x40, divisor & 0xff);
    outb(0x40, divisor >> 8);
}

void pit_handler(void){
    timer_ticks++;
    pic_send_eoi(0);
}

uint32_t pit_get_ticks(void){
    return timer_ticks;
}

uint32_t pit_get_seconds(void){
    return (timer_ticks / PIT_TARGET_FREQUENCY) % 60;
}

uint32_t pit_get_minutes(void){
    return (timer_ticks / PIT_TARGET_FREQUENCY) / 60;
}

uint32_t pit_get_hours(void){
    return (timer_ticks / PIT_TARGET_FREQUENCY) / 3600;
}

void pit_sleep_ms(uint32_t ms){
    uint32_t delay_ticks = (ms + 9) / 10;
    uint32_t start = pit_get_ticks();
    while((pit_get_ticks() - start) < delay_ticks){
        halt_cpu();
    }
}
