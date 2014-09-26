#include "moving.h"

#include "data.h"
#include "board.h"
#include <plib.h>

/*****************************************
 * PWM MOTOR
 *****************************************/
// Période
// =======
// Les ponts en H accepte un PWM jusqu'à 11kHz
// On configure donc le TIMER2 pour une periode de 10kHz
// T = 1/10000 = 100us

// Prescaler
// ==========
// Les timers 2 permet les prescalers 1, 2, 4, 8, 16, 32, 64, 256
//

#define  T2_PRESCALER 1

// Tick de l'horloge
// ==================
//
// Un coup d'horloge sur le timer 2 vaudra donc:
// 1/80000000L = 12.5 ns

// Valeur de rebouclement
// =======================

// Le timer2 est un timer 16bits.
// On partant de 0, il arrive à 100us après 8000 Tick
// 100 / 0.0125 = 8000

#define T2_MAX_VAL  8000

void enable_m1_update(void);
void enable_m2_update(void);
void direction_m1_update(void);
void direction_m2_update(void);
void speed_m1_update(void);
void speed_m2_update(void);
void go_update(void);

void moving_init(void) {

    /*****************************************
     * PWM
     *****************************************/

    // Configuration du Timer 2
    OpenTimer2(T2_ON | T2_PS_1_1, T2_MAX_VAL);

    // Configuration du module PWM du motor 1 à 0%
    OpenOC3( OC_ON | OC_TIMER_MODE16 | OC_TIMER2_SRC |
            OC_PWM_FAULT_PIN_DISABLE, 0, 0);

    // Configuration du module PWM du motor 2 à 0%
    OpenOC2(OC_ON | OC_TIMER_MODE16 | OC_TIMER2_SRC |
            OC_PWM_FAULT_PIN_DISABLE, 0, 0);
    /*****************************************
     * MOTOR
     *****************************************/
    gpio_set_low(M1_FWD);
    gpio_set_low(M1_EN1);
    gpio_set_low(M1_EN2);
    gpio_set_low(M1_REV);

    gpio_set_low(M2_FWD);
    gpio_set_low(M2_EN1);
    gpio_set_low(M2_EN2);
    gpio_set_low(M2_REV);

    gpio_set_output(M1_FWD);
    gpio_set_output(M1_EN1);
    gpio_set_output(M1_EN2);
    gpio_set_output(M1_REV);

    gpio_set_output(M2_FWD);
    gpio_set_output(M2_EN1);
    gpio_set_output(M2_EN2);
    gpio_set_output(M2_REV);

}


void direction_handling(void) {
    // Met à jour le sens de rotation des moteurs
    // Les valeurs posibles sont:
    // 0 - désactivé (défaut)
    // 1 - Avant
    // 2 - Arrière

    if(data_get(FUNC_GO)) {
        go_update();
        data_set(FUNC_GO, 0);
    }

    if(data_get(REG_M1_DONE)){
        direction_m1_update();
        enable_m1_update();
        speed_m1_update();
        data_set(REG_M1_DONE, 0);
    }

    if(data_get(REG_M2_DONE)){
        direction_m2_update();
        enable_m2_update();
        speed_m2_update();
        data_set(REG_M2_DONE, 0);
    }
    


}

void go_update(void) {
    data_set(REG_EN_M1, 1);
    data_set(REG_EN_M2, 1);

    switch(data_get(FUNC_GO_DIR)){
        case 1:
            // GO UP
            data_set(REG_DIR_M1, 1);
            data_set(REG_DIR_M2, 1);

            data_set(REG_PWM_M1, 100);
            data_set(REG_PWM_M2, 100);

            data_set(SONAR_ENABLE, 1);
            data_set(SERVO_ANGLE, 45);
            data_set(SERVO_DONE, 1);
            break;
        case 2:
            // GO DOWN
            data_set(REG_DIR_M1, 2);
            data_set(REG_DIR_M2, 2);

            data_set(REG_PWM_M1, 100);
            data_set(REG_PWM_M2, 100);

            data_set(SONAR_ENABLE, 0);
            data_set(SERVO_ANGLE, 45);
            data_set(SERVO_DONE, 1);
            break;
        case 3:
            // GO RIGHT
            data_set(REG_DIR_M1, 2);
            data_set(REG_DIR_M2, 1);

            data_set(REG_PWM_M1, 55);
            data_set(REG_PWM_M2, 55);

            data_set(SONAR_ENABLE, 1);
            data_set(SERVO_ANGLE, 90);
            data_set(SERVO_DONE, 1);
            break;
        case 4:
            // GO LEFT
            data_set(REG_DIR_M1, 1);
            data_set(REG_DIR_M2, 2);

            data_set(REG_PWM_M1, 55);
            data_set(REG_PWM_M2, 55);

            data_set(SONAR_ENABLE, 1);
            data_set(SERVO_ANGLE, 0);
            data_set(SERVO_DONE, 1);
            break;
        case 0:
        default:
            data_set(REG_PWM_M1, 0);
            data_set(REG_PWM_M2, 0);

            data_set(SONAR_ENABLE, 0);
            data_set(SERVO_ANGLE, 45);
            data_set(SERVO_DONE, 1);
            break;
    }

    data_set(REG_M1_DONE, 1);
    data_set(REG_M2_DONE, 1);
}
void enable_m1_update(void){
    // Mise à jour des PIN EN1 et EN2
    // du pont en H

    // Une valeur de 0 configure le mode "Disabled - Sleeping"
    // Une valeur de 1 active le pont en H
    // Une valeur de 2 configure le mode "Disabled - Runing"
    switch(data_get(REG_EN_M1)) {
        case 1:
            gpio_set_high(M1_EN1);
            gpio_set_high(M1_EN2);
            break;
        case 2:
            gpio_set_high(M1_EN1);
            gpio_set_low(M1_EN2);
            break;
        case 0:
        default:
            gpio_set_low(M1_EN1);
            gpio_set_low(M1_EN1);
            break;
    }
}

void enable_m2_update(void){
    // Mise à jour des PIN EN1 et EN2
    // du pont en H

    // Une valeur de 0 configure le mode "Disabled - Sleeping"
    // Une valeur de 1 active le pont en H
    // Une valeur de 2 configure le mode "Disabled - Runing"
    switch(data_get(REG_EN_M2)) {
        case 1:
            gpio_set_high(M2_EN1);
            gpio_set_high(M2_EN2);
            break;
        case 2:
            gpio_set_high(M2_EN1);
            gpio_set_low(M2_EN2);
            break;
        case 0:
        default:
            gpio_set_low(M2_EN1);
            gpio_set_low(M2_EN1);
            break;
    }
}

void direction_m1_update(void) {
    switch(data_get(REG_DIR_M1)) {
        case 1:
            gpio_set_high(M1_FWD);
            gpio_set_low(M1_REV);
            break;
        case 2:
            gpio_set_low(M1_FWD);
            gpio_set_high(M1_REV);
            break;
        case 0:
        default:
            gpio_set_low(M1_REV);
            gpio_set_low(M1_FWD);
            break;
    }
}

void direction_m2_update(void) {
     switch(data_get(REG_DIR_M2)) {
        case 1:
            gpio_set_high(M2_FWD);
            gpio_set_low(M2_REV);
            break;
        case 2:
            gpio_set_low(M2_FWD);
            gpio_set_high(M2_REV);
            break;
        case 0:
        default:
            gpio_set_low(M2_REV);
            gpio_set_low(M2_FWD);
            break;
    }
}

void speed_m1_update(void){
    float speed_m1 = data_get(REG_PWM_M1) / 127.0;

    SetDCOC3PWM( ( T2_MAX_VAL - 1250)  );

    delay_ms(2);

    SetDCOC3PWM( ( T2_MAX_VAL - 1250) * speed_m1 );
}

void speed_m2_update(void){
    float speed_m2 = data_get(REG_PWM_M2) / 127.0;

    SetDCOC2PWM( ( T2_MAX_VAL - 0)  );

    delay_ms(2);

    SetDCOC2PWM( (T2_MAX_VAL - 0) * speed_m2 );
}