
#include "gpio.h"
#include "board.h"
#include "data.h"

void follower_init(void) {

    
    gpio_set_output(LF1_EN);
    gpio_set_high(LF1_EN);
    gpio_set_analog(LF1_VALUE);

    gpio_set_output(LF2_EN);
    gpio_set_high(LF2_EN);
    gpio_set_analog(LF2_VALUE);

    gpio_set_output(LF3_EN);
    gpio_set_high(LF3_EN);
    gpio_set_analog(LF3_VALUE);

    gpio_set_output(LF4_EN);
    gpio_set_high(LF4_EN);
    gpio_set_analog(LF4_VALUE);

    gpio_set_output(LF5_EN);
    gpio_set_high(LF5_EN);
    gpio_set_analog(LF5_VALUE);

}

void follower_update(void) {

    float value1, value2, value3, value4, value5;

    value1 = gpio_analog_read(LF1_VALUE);
    value2 = gpio_analog_read(LF2_VALUE);
    value3 = gpio_analog_read(LF3_VALUE);
    value4 = gpio_analog_read(LF4_VALUE);
    value5 = gpio_analog_read(LF5_VALUE);

    data_set(LF_VAL1, value1);
    data_set(LF_VAL2, value2);
    data_set(LF_VAL3, value3);
    data_set(LF_VAL4, value4);
    data_set(LF_VAL5, value5);

}
