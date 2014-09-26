// Mc32Gest_RS232.C
// Canevas manipulation TP2B RS232 SLO2 2013-2014
// fonction d'émission et de réception des message



#include <plib.h>
#include "fifo.h"
#include "serial.h"
#include "data.h"


#define SYS_FREQ (80000000L)    //80 MHz
#define PB_DIV 1
#define PB_FREQ (SYS_FREQ / PB_DIV)



// Structure décrivant le message

#define MESS_SIZE (6)

const int START_BYTE = '!';

#define READ  'R'
#define WRITE 'W'

const int START = 0;
const int FUNC = 1;
const int ADDR = 2;
const int VALUE = 3;

// Declaration des FIFO pour réception et émission
#define FIFO_RX_SIZE ( (4*MESS_SIZE) + 1)  // 4 messages
#define FIFO_TX_SIZE ( (4*MESS_SIZE) + 1)  // 4 messages

int8_t fifoRX[FIFO_RX_SIZE];
// Declaration du descripteur du FIFO de réception
S_fifo descrFifoRX;


int8_t fifoTX[FIFO_TX_SIZE];
// Declaration du descripteur du FIFO d'émission
S_fifo descrFifoTX;

void msg_processing(uint8_t * mess) {
    int data;
    // Fonction:
    // 0x01 pour READ
    // 0x10 pour WRITE
    switch(mess[FUNC]) {
        case READ:
            data = data_get(mess[ADDR]);
            PutCharInFifo ( &descrFifoTX, START_BYTE);
            PutCharInFifo ( &descrFifoTX, mess[FUNC]);
            PutCharInFifo ( &descrFifoTX, mess[ADDR]);
            PutCharInFifo ( &descrFifoTX, data);

            break;
        case WRITE:
            data_set(mess[ADDR], mess[VALUE]);
            break;
        default:
            break;
    }



}



void serial_handling(void){
    static int step = 0;
    static uint8_t msg[MESS_SIZE];

    if(UARTReceivedDataIsAvailable(UART2))
    {
       PutCharInFifo ( &descrFifoRX, UARTGetDataByte(UART2));
    }

    if(GetReadSize(&descrFifoRX) < 1){
        // Il n'y a rien à faire
        return ;
    }

    GetCharFromFifo(&descrFifoRX, &msg[step]);

    switch(step){
        case 0:
            // Recherche du caractère '!'
            if(msg[0] == '!'){
                step++;
            }
            break;
        case 1:
        case 2:
            step++;
            break;
        case 3:
            // Message complet
            msg_processing(msg);
            step = 0;
        default:
            step = 0;
            break;

    }


    if (GetReadSize(&descrFifoTX) > 0)
    {
        // Autorise int émission
        INTEnable(INT_U2TX, INT_ENABLED);
    }

}


 







// Réponse Interruption de l'UART
// Il est nécessaire de tester si RX ou TX
// L'interruption de réception est supposée se produire au 3/4 du fifo HW
// l'interruption de transmission est supposée avoir lieu lorsque le fifo HW est vide

void __ISR(_UART_2_VECTOR, IPL5SOFT) UART2_isr(void)
{
   uint8_t ErrFiFoFull = 0;
   uint8_t freeSize, TXsize;
   int8_t c;
   uint8_t i_cts = 0;
   BOOL  TxPossible;

   UART_LINE_STATUS lineStatus;



    // Is this an RX interrupt ?
    if ( INTGetFlag(INT_U2RX) && INTGetEnable(INT_U2RX) ) {


        // oui Test si erreur comm
        lineStatus = UARTGetLineStatus(UART2);

        if ( (lineStatus & (UART_PARITY_ERROR | UART_FRAMING_ERROR |
                            UART_OVERRUN_ERROR)) == 0) {
             // transfert dans le fifo de tous les caractères recu
             while (UARTReceivedDataIsAvailable(UART2))
             {
                 c = UARTGetDataByte(UART2);
                 PutCharInFifo ( &descrFifoRX, c);
             }
             INTClearFlag(INT_U2RX); // buffer is empty, clear interrupt flag

        } else {
             UART2ClearAllErrors();   // Macro C32
        }

         freeSize = GetWriteSpace ( &descrFifoRX);
         if (freeSize <= 6 )        // a cause d'un int pour 6 char
         {
            // Demande de ne plus émettre
            //RS232_RTS = 1;

            if (freeSize == 0) {
                 ErrFiFoFull = 1;    // pour debugging si probème ctrl flux
            }
        }
    } // end if RX

    // Is this an TX interrupt ?
    if(INTGetFlag(INT_U2TX) && INTGetEnable(INT_U2TX)  ) {


         TXsize = GetReadSize (&descrFifoTX);
         // i_cts = input(RS232_CTS);
         // On vérifie 3 conditions :
         //    Si CTS = 0 (autorisation d'émettre)
         //    Si il y a un caratères à émettre
         //    Si le txreg est bien disponible

         //i_cts = RS232_CTS;

         TxPossible = UARTTransmitterIsReady(UART2);
         //if ( (i_cts == 0) && ( TXsize > 0 ) && TxPossible )  {
         if (  ( TXsize > 0 ) && TxPossible )  {
             do {
                 GetCharFromFifo(&descrFifoTX, &c);

                 UARTSendDataByte(UART2, c);
                 //i_cts = RS232_CTS;
                 TXsize = GetReadSize (&descrFifoTX);
                 TxPossible = UARTTransmitterIsReady(UART2);

             //} while ( (i_cts == 0) && ( TXsize > 0 ) && TxPossible  );
             } while (  ( TXsize > 0 ) && TxPossible  );
            // Clear the TX interrupt Flag (Seulement aprés TX)
            INTClearFlag(INT_U2TX);
        } else {
           // disable TX interrupt
           INTEnable(INT_U2TX, INT_DISABLED);
        }
    }
} // UART_isr





// Initialisation de la communication sériel
// -----------------------------------------

void serial_init(void)
{

   // Initialisation du fifo de réception
   InitFifo ( &descrFifoRX, FIFO_RX_SIZE, fifoRX, 0 );
   // Initialisation du fifo d'émission
   InitFifo ( &descrFifoTX, FIFO_TX_SIZE, fifoTX, 0 );


  // Utilisation des fonctions séparées (XC32)
  // =========================================

    UARTConfigure(UART2, UART_ENABLE_HIGH_SPEED | UART_ENABLE_PINS_TX_RX_ONLY );
    // UARTSetFifoMode(UART2, UART_INTERRUPT_ON_TX_BUFFER_EMPTY | UART_INTERRUPT_ON_RX_HALF_FULL  );
    // Remarque HALF_FULL ne fonctionne pas
    // Pour INT RX au 3/4
    UARTSetFifoMode(UART2, UART_INTERRUPT_ON_TX_BUFFER_EMPTY | UART_INTERRUPT_ON_RX_3_QUARTER_FULL );
    // Pour INT RX dés que min 1 char
    // UARTSetFifoMode(UART2, UART_INTERRUPT_ON_TX_BUFFER_EMPTY | UART_INTERRUPT_ON_RX_NOT_EMPTY );
    UARTSetLineControl(UART2, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
    UINT32 ActualBaudRate = UARTSetDataRate(UART2, PB_FREQ, 9600);
    UARTEnable(UART2, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));


  // Configuration Int UART2 avec les fonctions séparées
  // ===================================================

  // Configure UART RX Interrupt
  INTEnable(INT_SOURCE_UART_RX(UART2), INT_ENABLED);
  INTSetVectorPriority(INT_VECTOR_UART(UART2), INT_PRIORITY_LEVEL_5);
  INTSetVectorSubPriority(INT_VECTOR_UART(UART2), INT_SUB_PRIORITY_LEVEL_0);

} // InitComm









