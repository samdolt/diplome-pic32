#include "board.h"

#include <plib.h>
#include "delay.h"
#include "serial.h"
#include "data.h"
#include "moving.h"

#include "sonar.h"

#include "power.h"

#define SYS_FREQ (80000000L)
#define CORE_TIMER_TICK_RATE (SYS_FREQ/2000.0)

#define TIMER1_TICK_PS_8 ((1.0/ (SYS_FREQ / 8)))
#define TIMER1_1MS_PS_8 (0.001 / TIMER1_TICK_PS_8 )

int cpt_ms = 0;

void bsp_init(void) {

    /*********************************
     * Alimentation
     *********************************/

    /* Solution de contournement :
     * --------------------------
     * Lors de l'extinction du PIC32 par la mise à l'état bas
     * de la pin PWR_EN, le processeur redémarre et réactive la pin.
     *
     * Un délai de 50ms est donc nécessaire pour éviter les rermérrages
     * intempestif lors de l'extinction
     */

    delay_ms(50);

    gpio_set_high(PWR_EN);
    gpio_set_output(PWR_EN);

    gpio_set_low(RASP_OFF_REQUEST);
    gpio_set_output(RASP_OFF_REQUEST);

    /****************************************
     * Configuration de l'ADC
     ****************************************/
    AD1PCFG = 0xFFFF; // Disable analog mode on PORTB


    /* AD1CON:
     *  - Valeur décimale codée sur 16 bits non singé
     *  - Auto-convert
     */
    AD1CON1 = 0x00F0;

    /*  - Désactivation du réglage d'offset
     *  - Utilisation du buffer 0x0 à 0x7
     * - Buffer en mode 16 bits
     * - Utilisation du multiplexeur A
     */
    AD1CON2 = 0;

    /* - Utilisation de l'horloge RC interne à l'ADC
     * - Auto-samplig sur 15x T_AD
     * - T_AD sélectionner sur F_PB/2
     */
    AD1CON3 = 0x8F00;

    /* - Activation de l'ADC
     */
    AD1CON1bits.ADON = 1;

    power_init();

    /*****************************************
     * LED
     *****************************************/
    gpio_set_high(LED1);
    gpio_set_high(LED2);
    gpio_set_high(LED3);
    gpio_set_high(LED4);

    gpio_set_output(LED1);
    gpio_set_output(LED2);
    gpio_set_output(LED3);
    gpio_set_output(LED4);

    data_init();
    serial_init();
    moving_init();
    servo_init();
    follower_init();


    while(gpio_read(RASP_STATUS) != 0)
    {
        gpio_set_low(LED1);
        delay_ms(200);
        gpio_set_high(LED1);
        gpio_set_low(LED2);
        delay_ms(200);
        gpio_set_high(LED2);
        gpio_set_low(LED3);
        delay_ms(200);
        gpio_set_high(LED3);
        gpio_set_low(LED4);
        delay_ms(200);
        gpio_set_high(LED4);
        // Attente du démarrage du raspberry pi
    }
    
    sonar_init();

    /****************************************
     * TIMER
     ****************************************/
    OpenTimer1( T1_ON | T1_SOURCE_INT | T1_PS_1_8, TIMER1_1MS_PS_8 );
    ConfigIntTimer1(T1_INT_ON | T1_INT_PRIOR_4);
    INTEnableSystemMultiVectoredInt ();
}


void __ISR(_TIMER_1_VECTOR, IPL4AUTO) Timer1Handler(void) {
    //static int cpt = 0;
    mT1ClearIntFlag();
    cpt_ms++;
    /*
    if(cpt > 1000)
    {
        cpt = 0;
        gpio_toggle(LED1);
    }
     */
        
}


