#include "gpio.h"

#include <p32xxxx.h>
#include <plib.h>
#include <stdint.h>

/*************************************************
 * Prototype des fonctions internes
 ************************************************/
char gpio_get_port(const char PIN[]);
uint8_t gpio_get_pin(const char PIN[]);


/*************************************************
 * Contrôle de la direction
 ************************************************/

void gpio_set_direction(const char GPIO[], enum gpio_direction direction) {
    if(direction == OUTPUT) {
        gpio_set_output(GPIO);
    }
    else if(ANALOG){
        gpio_set_analog(GPIO);
    }
    else {
        gpio_set_input(GPIO);
    }
}

void gpio_set_analog(const char GPIO[]) {
    // Mise à 0 du bit dans TRISx

    const char M_ADC_NUMBER = gpio_get_pin(GPIO);

    // Configure la broche en entrée digitale
    TRISB |= (1<<M_ADC_NUMBER); // Configure la broche en entrée

    //Active le mode analogique sur la broche
    AD1PCFG &= ~(1 << M_ADC_NUMBER);
}

void gpio_set_output(const char GPIO[]) {
    // Mise à 0 du bit dans TRISx

    const char M_PORT = gpio_get_port(GPIO);
    const char M_PIN_NUMBER = gpio_get_pin(GPIO);

    switch (M_PORT) {
        case 'B':
            TRISB &= ~(1 << M_PIN_NUMBER);
            break;
        case 'C':
            TRISC &= ~(1 << M_PIN_NUMBER);
            break;
        case 'D':
            TRISD &= ~(1 << M_PIN_NUMBER);
            break;
        case 'E':
            TRISE &= ~(1 << M_PIN_NUMBER);
            break;
        case 'F':
            TRISF &= ~(1 << M_PIN_NUMBER);
            break;
        case 'G':
            TRISG &= ~(1 << M_PIN_NUMBER);
            break;
        default:
            break;
    }
}


void gpio_set_input(const char GPIO[]) {
    const char M_PORT = gpio_get_port(GPIO);
    const char M_PIN_NUMBER = gpio_get_pin(GPIO);

    // Mettre à 1 le bit dans TRISx
            switch(M_PORT) {
            case 'B':
                TRISB |= (1<<M_PIN_NUMBER);
                break;
            case 'C':
                TRISC |= (1<<M_PIN_NUMBER);
                break;
            case 'D':
                TRISD |= (1<<M_PIN_NUMBER);
                break;
            case 'E':
                TRISE |= (1<<M_PIN_NUMBER);
                break;
            case 'F':
                TRISF |= (1<<M_PIN_NUMBER);
                break;
            case 'G':
                TRISG |= (1<<M_PIN_NUMBER);
                break;
            default:
                break;
        }

}

/*************************************************
 * Écriture
 ************************************************/

void gpio_write(const char GPIO[], const bool STATE) {
    if(STATE)
    {
        gpio_set_high(GPIO);
    }
    else
    {
        gpio_set_low(GPIO);
    }
}

void gpio_set_low(const char GPIO[]) {
    const char M_PORT = gpio_get_port(GPIO);
    const char M_PIN_NUMBER = gpio_get_pin(GPIO);

    switch (M_PORT) {
        case 'B':
            LATB &= ~(1 << M_PIN_NUMBER);
            break;
        case 'C':
            LATC &= ~(1 << M_PIN_NUMBER);
            break;
        case 'D':
            LATD &= ~(1 << M_PIN_NUMBER);
            break;
        case 'E':
            LATE &= ~(1 << M_PIN_NUMBER);
            break;
        case 'F':
            LATF &= ~(1 << M_PIN_NUMBER);
            break;
        case 'G':
            LATG &= ~(1 << M_PIN_NUMBER);
            break;
        default:
            break;
    }

}

void gpio_set_high(const char GPIO[]) {
    const char M_PORT = gpio_get_port(GPIO);
    const char M_PIN_NUMBER = gpio_get_pin(GPIO);

    switch (M_PORT) {
        case 'B':
            LATB |= (1 << M_PIN_NUMBER);
            break;
        case 'C':
            LATC |= (1 << M_PIN_NUMBER);
            break;
        case 'D':
            LATD |= (1 << M_PIN_NUMBER);
            break;
        case 'E':
            LATE |= (1 << M_PIN_NUMBER);
            break;
        case 'F':
            LATF |= (1 << M_PIN_NUMBER);
            break;
        case 'G':
            LATG |= (1 << M_PIN_NUMBER);
            break;
        default:
            break;
    }

}

void gpio_toggle(const char GPIO[]) {
    const char M_PORT = gpio_get_port(GPIO);
    const char M_PIN_NUMBER = gpio_get_pin(GPIO);

    switch (M_PORT) {
        case 'B':
            LATB ^= (1 << M_PIN_NUMBER);
            break;
        case 'C':
            LATC ^= (1 << M_PIN_NUMBER);
            break;
        case 'D':
            LATD ^= (1 << M_PIN_NUMBER);
            break;
        case 'E':
            LATE ^= (1 << M_PIN_NUMBER);
            break;
        case 'F':
            LATF ^= (1 << M_PIN_NUMBER);
            break;
        case 'G':
            LATG ^= (1 << M_PIN_NUMBER);
            break;
        default:
            break;
    }
}
/*************************************************
 * Lecture
 ************************************************/
bool gpio_read(const char GPIO[]) {
    const char M_PORT = gpio_get_port(GPIO);
    const char M_PIN_NUMBER = gpio_get_pin(GPIO);

    switch (M_PORT) {
        case 'B':
            return ((PORTB & (1 << M_PIN_NUMBER))) >> M_PIN_NUMBER;
        case 'C':
            return ((PORTC & (1 << M_PIN_NUMBER))) >> M_PIN_NUMBER;
        case 'D':
            return ((PORTD & (1 << M_PIN_NUMBER))) >> M_PIN_NUMBER;
        case 'E':
            return ((PORTE & (1 << M_PIN_NUMBER))) >> M_PIN_NUMBER;
        case 'F':
            return ((PORTF & (1 << M_PIN_NUMBER))) >> M_PIN_NUMBER;
        case 'G':
            return ((PORTG & (1 << M_PIN_NUMBER))) >> M_PIN_NUMBER;
        default:
            return 0;
    }

    return 0;

}

float gpio_analog_read(const char GPIO[]) {
    const char M_ADC_NUMBER = gpio_get_pin(GPIO);

        // Désactivation du module ADC
    AD1CON1bits.ADON = 0;

    // Séléction de la broche sur l'entrée ADC A positive
    AD1CHS = M_ADC_NUMBER << 16;

    // Réactivation du module ADC
    AD1CON1bits.ADON = 1;

    // Démarrage de la conversion
    AD1CON1bits.SAMP = 1;

    while(!AD1CON1bits.DONE)
    {
        // Wait for ADC to complete
    }
    return ((float) ( ADC1BUF0 +(8 *(AD1CON2bits.BUFS & 0x01))   )) / (1023.0 / 3.3);
}
/**********************************************
 * Fonction interne
 ***********************************************/

char gpio_get_port(const char PIN[]) {
    return PIN[0];
}

uint8_t gpio_get_pin(const char PIN[]) {
    uint8_t return_data = 0;
    uint8_t i = 0;
    for (i = 0; PIN[i] != '\0'; i++) {
        if (i == 0) {
            // Ne rien faire, information sur le port
        } else if (i == 1) {
            return_data = PIN[i] - '0';
        } else if (i == 2) {
            // Le numéro de pin est supérieur à 9:
            return_data *= 10;
            return_data += PIN[i] - '0';
        } else {
            // Ne rien faire
        }
    }
    return return_data;
}