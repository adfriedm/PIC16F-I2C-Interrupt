#include "timer0.h"

#include <plib/timers.h>
#include "i2c2message.h"


void initTimer0()
{
    OpenTimer0(TIMER_INT_ON   &
               T0_16BIT       &
               T0_SOURCE_INT  &
               T0_PS_1_128    );

    // Timer0 interrupt at low priority
    INTCON2bits.TMR0IP = 0;

    // allow interrupts from timer 0
    INTCONbits.TMR0IE = 1;
}


void timer0_int_handler()
{
    WriteTimer0(0x5000);
}