/* 
 * File:   main.c
 * Author: doltsa
 *
 * Created on 16. septembre 2014, 16:06
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <p32xxxx.h>
#include <plib.h>

#include "gpio.h"
#include "board.h"
#include "serial.h"
#include "sonar.h"

#include "power.h"

#include "follower.h"

/*
 * 
 */

/*******************************************************************************
 * CONFIGURATION DES FUSIBLES
 ******************************************************************************/


/* Configuration des oscillateurs
 * ------------------------------ */

#pragma config POSCMOD = HS // Oscillateur primaire en mode "HIGH SPEED CRYSTAL"
#pragma config FSOSCEN = OFF // Oscillateur secondaire désactivé
#pragma config IESO = OFF // Internal External Switch Over bit Disable
#pragma config FNOSC = PRIPLL // Oscillateur primaire avec PLL
#pragma config OSCIOFNC = OFF // Désactivation de la sortie sur la pin CLKO
#pragma config FCKSM = CSDCMD // Clock Switching Disabled, Clock Monitoring Disabled


/* Configuration de la PLL avec un quartz externe de 8 MHz
 * ------------------------------------------------------- */
#pragma config FPLLIDIV = DIV_2 // Division de l'oscillateur primaire par 2 ->4MHz
#pragma config FPLLMUL = MUL_20 // Multiplication par 20 -> 80MHz en sortie de la PLL
#pragma config FPLLODIV = DIV_1 // Division par 1 -> 80MHz pour l'horloge système
#define SYS_FREQ (80000000L)    // Constante stockant la valeur de l'horloge système


/* Configuration du chien de garde (WATCHDOG)
 * ------------------------------------------ */
#pragma  config FWDTEN = OFF // Désactivé




/* Configuration du programmeur et du débogueur
 * -------------------------------------------- */
#pragma config ICESEL = ICS_PGx2 // Les pins ICE sont partagée avec PGC2 et PGD2
#pragma config DEBUG = ON // Mode DEBUG enclenché


void off_button_handling() {
    if (gpio_read(PWR_OFF) == 0) {
        gpio_set_input(RASP_OFF_REQUEST);

        while (gpio_read(RASP_STATUS) != 1) {
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

        cpt_ms = 0;
        delay_ms(200);
        
        while(cpt_ms < 5000){ // 5 seconde de plus par sécurité
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
        }


        gpio_set_low(PWR_EN);

        // FIN DU PROGRAMME
    }


}

void led1_toogle_handling() {

    if(cpt_ms > 1000)
    {
        cpt_ms = 0;
        gpio_toggle(LED1);
    }
}




int main(int argc, char** argv) {
        /*
     * CONFIGURATION
     * -------------- */

    SYSTEMConfigPerformance(SYS_FREQ);

    bsp_init();

    mJTAGPortEnable(0); // Désactivation du JTAG, libère RA0, RA1, RA4 et RA5


   


    for(;;){

        // Gestion de l'appuis sur le bouton OFF
        // En cas d'appuis, cette fonction est bloquante. Elle attent
        // l'exctinction du Raspberry Pi. Ensuite, l'alimentation est coupée
        off_button_handling();

        // Gestion de l'état de la batterie - Non fonctionnel
        power_handling();

        // Clignottement chaque seconde de la led1
        led1_toogle_handling();

        // Gestion de la communication série avec le Raspberry Pi
        serial_handling();

        // Gestion du servomoteur
        servo_update();

        // Gestion de la réception du sonar
        sonar_handling();

        // Gestion du suivi de ligne - Fonctionnelle sur trois capteurs (1,3,5)
        // Les capteurs 2 et 4 ne fonctionnent pas, problème de câblage sur le
        // véroboard
        follower_update();

        // Gestion des moteurs et de la direction
        // Etat actuel:
        // - Pas de mesure du courant moteur
        // - Pas de communication SPI
        direction_handling();



    }
}



