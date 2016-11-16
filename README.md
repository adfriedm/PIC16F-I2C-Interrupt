# PIC16F Interrupts

### Disclaimer
I am providing this code after having had extreme difficulty finding a complete
i2c interrupt state machine on the internet. This code is a few years old and
my summary here reflects my memory of it, not necessarily the actual behavior.
This i2c implementation did not drop or corrupt messages throughout countless
hours of its application, but that does not necessarily mean it is flawless. It
is unlikely I will be testing it further, so it should be used with caution.


## Brief Description

## File List
- interrupts.[h/c] - setup hardware interrupts
- messages.[h/c] - provides class-like message queue
- i2c2message.[h/c] - provides statically allocated message inbox and outbox
  for i2c2, reducing functions in messages.h
- uartmessage.[h/c] - provides statically allocated message inbox and outbox
  for UART, reducing functions in messages.h
- maindefs.h - compiler commands for XC8 compiler
- timer0.[h,c] - provides timer0 setup and interrupt handler
- i2c2_18f45j10_master.[h/c] - provides i2c2 master interrupt handler, send and read functionality
- uart.[h/c] - provides UART interrupt handler, send and read functionality
- main.c - 


