/* 
 * File:   serial.h
 * Author: doltsa
 *
 * Created on 18. septembre 2014, 13:24
 */

#ifndef SERIAL_H
#define	SERIAL_H

/*--------------------------------------------------------*/
/* Mc32Gest_RS232.h
/*--------------------------------------------------------*/
/*	Description :	emission et reception spécialisée
/*			pour TP2B
/*
/*	Auteur 		: 	F. Dominé
/*
/*	Version		:	V1.0
/*	Compilateur	:	XC32 V1.20
//
/*--------------------------------------------------------*/


/*--------------------------------------------------------*/
/* Définition des fonctions prototypes
/*--------------------------------------------------------*/
#include <stdint.h>


// prototypes des fonctions
void serial_init(void);
void serial_handling(void);

#endif	/* SERIAL_H */

