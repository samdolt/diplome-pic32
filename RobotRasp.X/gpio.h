/* 
 * File:   gpio.h
 * Author: doltsa
 *
 * Created on 17. septembre 2014, 12:19
 */

#ifndef GPIO_H
#define	GPIO_H

#include <stdbool.h>

/***************************************
 * Direction
 ***************************************/
enum gpio_direction {
    OUTPUT = 0,
    INPUT = 1,
    ANALOG = 2
};

void gpio_set_direction(const char GPIO[], enum gpio_direction direction);
void gpio_set_output(const char GPIO[]);
void gpio_set_input(const char GPIO[]);
void gpio_set_analog(const char GPIO[]);
/*************************************************
 * Écriture
 ************************************************/

void gpio_write(const char GPIO[], const bool STATE);
void gpio_set_low(const char GPIO[]);
void gpio_set_high(const char GPIO[]);
void gpio_toggle(const char GPIO[]);

/*************************************************
 * Lecture
 ************************************************/
bool gpio_read(const char GPIO[]);
float gpio_analog_read(const char GPIO[]);
#endif	/* GPIO_H */

