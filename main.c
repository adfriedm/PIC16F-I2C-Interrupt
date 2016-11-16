#include "maindefs.h"
#include <stdio.h>
#include <string.h>
#include <plib/timers.h>
#include <plib/adc.h>
#include <plib/i2c.h>
#include "interrupts.h"

#include "timer0.h"

#include "i2c2_18f45j10_master.h"
#include "i2c2message.h"

#include "uart.h"
#include "uartmessage.h"


#ifdef __USE18F45J10
// CONFIG1L
#pragma config WDTEN = OFF      // Watchdog Timer Enable bit (WDT disabled (control is placed on SWDTEN bit))
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset Enable bit (Reset on stack overflow/underflow disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode enabled)

// CONFIG1H
#pragma config CP0 = OFF        // Code Protection bit (Program memory is not code-protected)

// CONFIG2L
#pragma config FOSC = HSPLL     // Oscillator Selection bits (HS oscillator, PLL enabled and under software control)
#pragma config FOSC2 = ON       // Default/Reset System Clock Select bit (Clock selected by FOSC as system clock is enabled when OSCCON<1:0> = 00)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)
#pragma config IESO = ON        // Two-Speed Start-up (Internal/External Oscillator Switchover) Control bit (Two-Speed Start-up enabled)

// CONFIG2H
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = DEFAULT // CCP2 MUX bit (CCP2 is multiplexed with RC1)
#else
#error "Unsupported microcontroller - include CONFIG in main.c"
#endif

/*
 * READ THIS FIRST
 * This demonstrates the use of the message i2c2.
 * To request bytes call i2c2MasterRead(,). This will place the read bytes into the i2c2 outbox
 * To send bytes then place the bytes into the i2c2 inbox. Call writei2c2InboxQueue(,)
 */



void main(void)
{
    
    // does all configuration except starting the interrupt system
    OSCCON = 0x82;         // see datasheeet
    OSCTUNEbits.PLLEN = 0; // Makes the clock exceed the PIC's rated speed if the PLL is on
   

    // clear all interrupt enables
    PIE1 = 0;
    PIE2 = 0;
    PIE3 = 0;

    // disable interrupts from INTCON
    INTCONbits.INT0IE = 0;
    INTCONbits.TMR0IE = 0;
    INTCONbits.RBIE = 0;

    // disable interrupts from INTCON3
    INTCON3bits.INT1IE = 0;
    INTCON3bits.INT2IE = 0;


    //initTimer0();
    initI2C2Master();
    initi2c2Queues();


    initUART();
    initUARTQueues();


    enable_interrupts();



    TRISBbits.RB0 = 0;
    LATBbits.LATB0 = 0;
    
    unsigned char tempBuff[32];
    signed char length;

    while (1)
    {

         // If UART has returned reading
        if( !isEmptyUARTOutboxQueue() )
        {
            LATBbits.LATB0 ^= 1;
            length = readUARTOutboxQueue(&tempBuff[1]);
            if(length <= 0)
            {
                // Error occured, check error status
            }
            else
            {
                switch(tempBuff[1])
                {

                    case 0x3c: // SENSOR full read
                    case 0x3d: // SENSOR right parallel read
                    case 0x3e: // SENSOR left parallel read
                        tempBuff[0] = 0x3c;
                        writei2c2InboxQueue((void*)tempBuff, 2);
                        break;
                    // MOTOR
                    case 0x4c:
                        writei2c2InboxQueue((void*)&tempBuff[1], 4);
                        break;
                    // Otherwise ignore
                    default:
                        break;
                }

            }
        }


        /*
        // If UART has returned reading
        if( !isEmptyUARTInboxQueue() )
        {
            length = readUARTInboxQueue(tempBuff);
            if(length <= 0)
            {
                // Error occured, check error status
            }
            else
            {
                //sendUART(tempBuff, length);
            }
        }
        */
        // If there data to send down I2C2 and I2C2 is currently free
        if( !isEmptyi2c2InboxQueue() && isI2C2MasterIdle() )
        {
            length = readi2c2InboxQueue(tempBuff);
            if(length <= 0)
            {
                // Error occured, check error status
            }
            else
            {
                i2c2MasterSendData(length-1, &tempBuff[1], tempBuff[0]);
            }
        }

        // If there is read I2C2 data available?
        if( !isEmptyi2c2OutboxQueue() )
        {
            length = readi2c2OutboxQueue(tempBuff);
          
            if(length <= 0)
            {
                // Error occured, check error status
            }
            else
            {
                // Put data in the i2c2 inbox to send out
                writei2c2InboxQueue(tempBuff, length);
            }
        }
        

    }

}