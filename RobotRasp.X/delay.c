
#include "delay.h"
#include <plib.h>

long get_system_clock(void){
    return 80000000L;
}

void delay_s(uint32_t delay) {
    int i;
    for (i = 0; i < delay; i++) {
        delay_ms(1000);
    }
}

void delay_ms(uint32_t delay) {
    uint32_t time_to_wait;
    WriteCoreTimer(0);
    time_to_wait = get_system_clock() / 2000.0 * delay;
    while (ReadCoreTimer() < time_to_wait) {
        // Waiting
    };
}

void delay_us(uint32_t delay) {
    uint32_t time_to_wait;
    WriteCoreTimer(0);
    time_to_wait = get_system_clock() / 2000000.0 * delay;
    while (ReadCoreTimer() < time_to_wait) {
        // Waiting
    };
}