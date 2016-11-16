#ifndef UART_H
#define UART_H

void initUART();
void sendUART(unsigned char* buffer, unsigned char size);
void uart_int_handler();

#endif
