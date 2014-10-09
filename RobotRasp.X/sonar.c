/******************************************************************************
 * Communication SONAR -> PIC32
 ******************************************************************************
 * Principe de traitement:
 * -----------------------
 * 1) Lors de chaque appel de serial_handling, on effectue la copie d'une seul
 *    valeur du fifo hardware au fifo software
 * 2) On essaye de lire un caractère dans le fifo software
 * 3) On traite ce caractère
 * 4) Si on a une trame complète, on la décode
 *****************************************************************************/


#include <plib.h>
#include "fifo.h"
#include "serial.h"
#include "data.h"
#include "gpio.h"
#include "board.h"


#define SYS_FREQ (80000000L)    //80 MHz
#define PB_DIV 1
#define PB_FREQ (SYS_FREQ / PB_DIV)




int8_t fifoRX_s[20];
// Declaration du descripteur du FIFO de réception
S_fifo descrfifoRX_s;



void sonar_processing(uint8_t * mess) {
    int data;

    data =  (mess[1] - '0') * 1000;
    data += (mess[2] - '0') * 100;
    data += (mess[3] - '0') *  10;
    data += (mess[4] - '0') *   1;

    if((data >= 300) && (data <=5000)){
        data_set(SONAR_VALUE, data);
    }

    if(data_get(SONAR_ENABLE))
    {
        if(data <= 350){
            data_set(REG_EN_M1,0);
            data_set(REG_EN_M2,0);

            data_set(REG_M1_DONE, 1);
            data_set(REG_M2_DONE, 1);

            gpio_set_low(LED2);
        }
        else {
            gpio_set_high(LED2);
        }
    }
    else {
         gpio_set_high(LED2);
    }


}



void sonar_handling(void){
    static int step = 0;
    static uint8_t msg[6];

    // On vérifie le FIFO Hardware
    if(UARTReceivedDataIsAvailable(UART1))
    {
       PutCharInFifo ( &descrfifoRX_s, UARTGetDataByte(UART1));
    }

    // On vérifie le FIFO Software
    if(GetReadSize(&descrfifoRX_s) < 1){
        // Il n'y a rien à faire
        return ;
    }

    GetCharFromFifo(&descrfifoRX_s, &msg[step]);

    switch(step){
        case 0:
            // Recherche du caractère '!'
            if(msg[0] == 'R'){
                step++;
            }
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            step++;
            break;
        case 5:
            // Message complet
            sonar_processing(msg);
            step = 0;
        default:
            step = 0;
            break;

    }


}










// Réponse Interruption de l'UART
// Il est nécessaire de tester si RX ou TX
// L'interruption de réception est supposée se produire au 3/4 du fifo HW
// l'interruption de transmission est supposée avoir lieu lorsque le fifo HW est vide

void __ISR(_UART_1_VECTOR, IPL6AUTO) UART1_isr(void)
{
   uint8_t ErrFiFoFull = 0;
   uint8_t freeSize, TXsize;
   int8_t c;
   uint8_t i_cts = 0;
   BOOL  TxPossible;

   UART_LINE_STATUS lineStatus;



    // Is this an RX interrupt ?
    if ( INTGetFlag(INT_U1RX) && INTGetEnable(INT_U1RX) ) {


        // oui Test si erreur comm
        lineStatus = UARTGetLineStatus(UART1);

        if ( (lineStatus & (UART_PARITY_ERROR | UART_FRAMING_ERROR |
                            UART_OVERRUN_ERROR)) == 0) {
             // transfert dans le fifo de tous les caractères recu
             while (UARTReceivedDataIsAvailable(UART1))
             {
                 c = UARTGetDataByte(UART1);
                 PutCharInFifo ( &descrfifoRX_s, c);
             }
             INTClearFlag(INT_U1RX); // buffer is empty, clear interrupt flag

        } else {
             UART1ClearAllErrors();   // Macro C32
        }

         freeSize = GetWriteSpace ( &descrfifoRX_s);
         if (freeSize <= 6 )        // a cause d'un int pour 6 char
         {
            // Demande de ne plus émettre
            //RS232_RTS = 1;

            if (freeSize == 0) {
                 ErrFiFoFull = 1;    // pour debugging si probème ctrl flux
            }
        }
    } // end if RX


} // UART_isr





// Initialisation de la communication sériel
// -----------------------------------------

void sonar_init(void)
{

   // Initialisation du fifo de réception
   InitFifo ( &descrfifoRX_s, 19, fifoRX_s, 0 );


  // Utilisation des fonctions séparées (XC32)
  // =========================================

    UARTConfigure(UART1, UART_ENABLE_HIGH_SPEED | UART_ENABLE_PINS_TX_RX_ONLY );
    // Remarque HALF_FULL ne fonctionne pas
    // Pour INT RX au 3/4
    UARTSetFifoMode(UART1,  UART_INTERRUPT_ON_RX_3_QUARTER_FULL );
    // Pour INT RX dés que min 1 char
    //UARTSetFifoMode(UART1, UART_INTERRUPT_ON_TX_BUFFER_EMPTY | UART_INTERRUPT_ON_RX_NOT_EMPTY );
    UARTSetLineControl(UART1, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UINT32 ActualBaudRate = UARTSetDataRate(UART1, PB_FREQ, 9600);
    UARTEnable(UART1, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX));


  // Configuration Int UART1 avec les fonctions séparées
  // ===================================================

  // Configure UART RX Interrupt
  INTEnable(INT_SOURCE_UART_RX(UART1), INT_ENABLED);
  INTSetVectorPriority(INT_VECTOR_UART(UART1), INT_PRIORITY_LEVEL_6);
  INTSetVectorSubPriority(INT_VECTOR_UART(UART1), INT_SUB_PRIORITY_LEVEL_0);

} // InitComm









