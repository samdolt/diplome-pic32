/* 
 * File:   fifo.h
 * Author: doltsa
 *
 * Created on 18. septembre 2014, 14:29
 */

#ifndef FIFO_H
#define	FIFO_H

/*--------------------------------------------------------*/
/*
/*--------------------------------------------------------*/
/*	Description :	Création entête pour utilisation
/*			de GesFifoTh32
/*
/*	Auteur 		: 	C. Huber
/*
/*	Version		:	V1.0
/*	Compilateur	:	XC32 V1.20
//
/*--------------------------------------------------------*/

#include <stdint.h>

// structure décrivant un FIFO
typedef struct fifo {
   int8_t fifoSize;   // taille du fifo
   int8_t *pDebFifo;   // pointeur sur début du fifo
   int8_t *pFinFifo;   // pointeur sur fin du fifo
   int8_t *pWrite;      // pointeur d'écriture
   int8_t *pRead;      // pointeur de lecture
} S_fifo;

/*--------------------------------------------------------*/
/* Définition des fonctions de gestion du fifo            */
/*--------------------------------------------------------*/

/*---------------*/
/* InitFifo      */
/*===============*/

// Initialisation du descripteur de FIFO
void InitFifo ( S_fifo *pDescrFifo, int8_t FifoSize, int8_t *pDebFifo, int8_t InitVal );

/*---------------*/
/* GetWriteSpace */
/*===============*/

// Retourne la place disponible en écriture

int8_t GetWriteSpace ( S_fifo *pDescrFifo);

/*-------------*/
/* GetReadSize */
/*=============*/

// Retourne le nombre de caractères à lire

int8_t GetReadSize ( S_fifo *pDescrFifo);

/*---------------*/
/* PutCharInFifo */
/*===============*/

// Dépose un caractère dans le FIFO
// Retourne 0 si OK, 1 si FIFO full

uint8_t PutCharInFifo ( S_fifo *pDescrFifo, int8_t charToPut );

/*-----------------*/
/* GetCharFromFifo */
/*=================*/

// Obtient (lecture) un caractère du fifo
// retourne 0 si OK, 1 si empty
// le caractère lu est retourné par réference

uint8_t GetCharFromFifo ( S_fifo *pDescrFifo, int8_t *carLu );



#endif	/* FIFO_H */

