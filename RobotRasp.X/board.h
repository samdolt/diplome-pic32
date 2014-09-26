/* 
 * File:   board.h
 * Author: doltsa
 *
 * Created on 17. septembre 2014, 12:51
 */

#ifndef BOARD_H
#define	BOARD_H

extern int cpt_ms;
/*********************************************
 * Gestion de l'alimentation
 *********************************************/

#define PWR_OFF ("E6")
#define PWR_EN ("E7")

#define RASP_OFF_REQUEST ("D7")
#define RASP_STATUS ("D6")

#define PWR_VOLTAGE ("B10")
#define PWR_CURRENT ("B9")

/*********************************************
 * MOTOR
 *********************************************/

#define M1_FWD ("D5")
#define M1_EN1 ("F0")
#define M1_EN2 ("F1")
#define M1_REV ("D4")

#define M2_FWD ("G2")
#define M2_EN1 ("C13")
#define M2_EN2 ("C14")
#define M2_REV ("G3")

/*********************************************
 * LED
 *********************************************/
#define LED1 ("F3")
#define LED2 ("F6")
#define LED3 ("B5")
#define LED4 ("B15")

/*********************************************
 * Line Follow
 *********************************************/
#define LF1_EN ("E4")
#define LF1_VALUE ("B0")

#define LF2_EN ("E3")
#define LF2_VALUE ("B1")

#define LF3_EN ("E2")
#define LF3_VALUE ("B2")

#define LF4_EN ("E1")
#define LF4_VALUE ("B3")

#define LF5_EN ("E0")
#define LF5_VALUE ("B4")
/*********************************************
 * Initalisation
 *********************************************/
void bsp_init(void);
#endif	/* BOARD_H */

