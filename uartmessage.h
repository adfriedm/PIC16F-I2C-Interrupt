#ifndef UARTMESSAGE_H
#define	UARTMESSAGE_H

#define UART_INBOX_MESSAGE_SIZE     31
#define UART_INBOX_QUEUE_LENGTH     4

#define UART_OUTBOX_MESSAGE_SIZE    6
#define UART_OUTBOX_QUEUE_LENGTH    4


void initUARTQueues();

signed char writeUARTInboxQueue(unsigned char* buffer, unsigned char length);
signed char readUARTInboxQueue(unsigned char* buffer);

signed char writeUARTOutboxQueue(unsigned char* buffer, unsigned char length);
signed char readUARTOutboxQueue(unsigned char* buffer);

signed char isEmptyUARTInboxQueue();
signed char isEmptyUARTOutboxQueue();

#endif
