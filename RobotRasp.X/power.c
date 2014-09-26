#include "gpio.h"
#include "board.h"

void power_init(void){
    gpio_set_analog(PWR_VOLTAGE);
    gpio_set_analog(PWR_CURRENT);
}

void power_handling(void)
{
    float voltage = gpio_analog_read(PWR_VOLTAGE) * 2.5;
    float current = gpio_analog_read(PWR_CURRENT);

    if(voltage < 6.5){
        gpio_set_low(LED4);
    }
    else{
        gpio_set_high(LED4);
    }
}
