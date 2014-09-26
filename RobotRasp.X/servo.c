#include "servo.h"

#include <plib.h>

#include "data.h"
// Le signal du servomoteur est générer à l'aide du module OC1

// Les modules OC peuvent être configurer pour utiliser le
// timer 2 ou le timer 3. Nous utilisons déjà le timer 2 pour
// générer les signaux PWM des moteurs, ils faut donc utiliser le
// timer 3

// Il faut généré une signal variant de 0.6 à 2.4 ms, qui se répete
// toute les 10 ms

// Recherche du prescaler

// 10ms sur 16 bits donne un résolution de:
// 10'000 / 65536 = 0.0152 us
// 0.0152 / 0.0125 = 12.2 => 16

#define T3_PRESCALER 16
#define T3_TICK ((1000000.0 * T3_PRESCALER) / 80000000L) // en us

// Valeur pour 10ms:
#define T3_VALUE (10000.0 / T3_TICK) // 5000

uint32_t pulse_stop = T3_VALUE * 0.06 +  (45 * T3_VALUE *(0.24-0.06) / 90 );
uint32_t pulse_start = 0;

void servo_init(void){

    OpenTimer3(T3_ON | T3_PS_1_16, T3_VALUE);
    OpenOC1(OC_ON | OC_TIMER_MODE16 | OC_TIMER3_SRC | OC_CONTINUE_PULSE,
            pulse_stop, pulse_start);
}

void servo_update(void){
    if(data_get(SERVO_DONE)){
        int angle = T3_VALUE * 0.06 +
                (data_get(SERVO_ANGLE) * T3_VALUE *(0.24-0.06) / 90 );
        SetPulseOC1(0, angle);
        data_set(SERVO_DONE, 0);
    }
    
}   
