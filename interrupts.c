#include "maindefs.h"
#include "interrupts.h"
#include "messages.h"
#include "i2c2_18f45j10_master.h"
#include "timer0.h"
#include "uart.h"

//----------------------------------------------------------------------------
// Note: This code for processing interrupts is configured to allow for high and
//       low priority interrupts.  The high priority interrupt can interrupt the
//       the processing of a low priority interrupt.  However, only one of each type
//       can be processed at the same time.  It is possible to enable nesting of low
//       priority interrupts, but this code is not setup for that and this nesting is not
//       enabled.

void enable_interrupts()
{
    // allow priority-based interrupt
    RCONbits.IPEN = 1;

    // enable global interrupt low and high priority
    INTCONbits.GIEH = 1;
    INTCONbits.GIEL = 1;
}





//----------------------------------------------------------------------------
// High priority interrupt routine
// this parcels out interrupts to individual handlers
interrupt void InterruptHandlerHigh()
{
    // We need to check the interrupt flag of each enabled high-priority interrupt to
    // see which device generated this interrupt.  Then we can call the correct handler.


    // check to see if we have an I2C2 interrupt
    if (PIR3bits.SSP2IF) {
        // clear the interrupt flag
        PIR3bits.SSP2IF = 0;
        i2c2_int_handler();
    }

    // check to see if we have an interrupt on timer 0
    if (INTCONbits.TMR0IF) {
        // clear the interrupt flag
        INTCONbits.TMR0IF = 0;
        timer0_int_handler();

    }

}

//----------------------------------------------------------------------------
// Low priority interrupt routine
// this parcels out interrupts to individual handlers
// This works the same way as the "High" interrupt handler
interrupt low_priority void InterruptHandlerLow()
{
    // check to see if we have an interrupt on UART
    if (PIR1bits.RCIF) {
        PIR1bits.RCIF = 0; //clear interrupt flag
        uart_int_handler();
    }
}