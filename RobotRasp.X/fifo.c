
// ETML Ecole Technique
// Fichier GesFifoTh.c

// Exemple gestion d'un fifo de caractère, utilisation de pointeur et
// d'un descripteur de fifo

// Création : Christian HUBER      06/05/2003

#include "fifo.h"


// prototypes des fonctions de gestion du fifo
uint8_t putChar ( S_fifo *pDescrFifo, int8_t charToPut );
uint8_t getChar ( S_fifo *pDescrFifo, int8_t *carLu );


/*---------------*/
/* InitFifo      */
/*===============*/

// Initialisation du descripteur de FIFO
void InitFifo ( S_fifo *pDescrFifo, int8_t FifoSize, int8_t *pDebFifo, int8_t InitVal )
{
   int8_t i;
   int8_t *pFif;
   pDescrFifo->fifoSize =   FifoSize;
   pDescrFifo->pDebFifo =   pDebFifo; // début du fifo
   // fin du fifo
   pDescrFifo->pFinFifo =   pDebFifo + (FifoSize - 1);
   pDescrFifo->pWrite   =   pDebFifo;  // début du fifo
   pDescrFifo->pRead     =   pDebFifo;  // début du fifo
   pFif = pDebFifo;
   for (i=0; i < FifoSize; i++) {
      *pFif  = InitVal;
      pFif++;
   }
} /* InitFifo */


/*---------------*/
/* GetWriteSpace */
/*===============*/

// Retourne la place disponible en écriture

int8_t GetWriteSpace ( S_fifo *pDescrFifo)
{
   int8_t writeSize;

   // Détermine le nb de car.que l'on peut déposer
   writeSize = pDescrFifo->pRead - pDescrFifo->pWrite -1;
   if (writeSize < 0) {
      writeSize = writeSize + pDescrFifo->fifoSize;
    }
   return (writeSize);
} /* GetWriteSpace */


/*-------------*/
/* GetReadSize */
/*=============*/

// Retourne le nombre de caractères à lire

int8_t GetReadSize ( S_fifo *pDescrFifo)
{
   int8_t readSize;

   readSize = pDescrFifo->pWrite - pDescrFifo->pRead;
   if (readSize < 0) {
       readSize = readSize +  pDescrFifo->fifoSize;
   }

   return (readSize);
} /* GetReadSize */

/*---------------*/
/* PutCharInFifo */
/*===============*/

// Dépose un caractère dans le FIFO
// Retourne 0 si OK, 1 si FIFO full

uint8_t PutCharInFifo ( S_fifo *pDescrFifo, int8_t charToPut )
{
   uint8_t writeStatus;

   // test si fifo est FULL
   if (GetWriteSpace(pDescrFifo) == 0) {
      writeStatus = 1; // fifo FULL
   }
   else {
      // écrit le caractère dans le FIFO
      *(pDescrFifo->pWrite) = charToPut;

      // incrément le pointeur d'écriture
      pDescrFifo->pWrite++;
      // gestion du rebouclement
      if (pDescrFifo->pWrite > pDescrFifo->pFinFifo) {
          pDescrFifo->pWrite = pDescrFifo->pDebFifo;
      }

      writeStatus = 0; // OK
   }
   return (writeStatus);
} // PutCharInFifo


/*-----------------*/
/* GetCharFromFifo */
/*=================*/

// Obtient (lecture) un caractère du fifo
// retourne 0 si OK, 1 si empty
// le caractère lu est retourné par réference

uint8_t GetCharFromFifo ( S_fifo *pDescrFifo, int8_t *carLu )
{
   int8_t readSize;
   uint8_t readStatus;

   // détermine le nb de car. que l'on peut lire
   readSize = GetReadSize(pDescrFifo);

   // test si fifo est vide
   if (readSize == 0) {
      readStatus = 1; // fifo EMPTY
      *carLu = 0;     // carLu = NULL
   }
   else {
      // lis le caractère dans le FIFO
      *carLu = *(pDescrFifo->pRead);

      // incrément du pointeur de lecture
      pDescrFifo->pRead++;
      // gestion du rebouclement
      if (pDescrFifo->pRead > pDescrFifo->pFinFifo) {
          pDescrFifo->pRead = pDescrFifo->pDebFifo;
      }
      readStatus = 0; // OK
   }
   return (readStatus);
} // GetCharFromFifo

