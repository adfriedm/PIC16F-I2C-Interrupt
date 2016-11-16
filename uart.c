#include "maindefs.h"
#ifndef __XC8
#include <usart.h>
#else
#include <plib/usart.h>
#endif
#include "uart.h"
#include "uartmessage.h"

#define REC_MSG_LENGTH 4

unsigned char bufferUART[REC_MSG_LENGTH];
unsigned char bufferUARTLength = 0;



void initUART()
{
    OpenUSART(USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_EIGHT_BIT &
        USART_CONT_RX & USART_BRGH_HIGH, 0x26);
    IPR1bits.RCIP = 0;
}



void sendUART(unsigned char* buffer, unsigned char length)
{
    int i;
    for(i = 0; i<length; ++i)
    {
        while(BusyUSART());
        WriteUSART(buffer[i]);
    }
}



void uart_int_handler()
{
    if( DataRdyUSART() )
    {
        bufferUART[bufferUARTLength] = ReadUSART();
        if(++bufferUARTLength == REC_MSG_LENGTH)
        {
            writeUARTOutboxQueue( bufferUART, REC_MSG_LENGTH );
            bufferUARTLength = 0;
        }
    }
    if( USART_Status.OVERRUN_ERROR == 1 )
    {
        // we've overrun the USART and must reset
        // send an error message for this
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;

        // Reset USART
        CloseUSART();
        OpenUSART(USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_EIGHT_BIT &
        USART_CONT_RX & USART_BRGH_HIGH, 0x26);
    }
}
