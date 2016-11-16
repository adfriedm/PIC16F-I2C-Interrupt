
#include "uartmessage.h"
#include "messages.h"


// MessageQueue data allocation
MessageQueue uartInboxQueue;
unsigned char uartInboxMessageData[ UART_INBOX_QUEUE_LENGTH ][ UART_INBOX_MESSAGE_SIZE ];
Message uartInboxMessage[ UART_INBOX_QUEUE_LENGTH ];

MessageQueue uartOutboxQueue;
unsigned char uartOutboxMessageData[ UART_OUTBOX_QUEUE_LENGTH ][ UART_OUTBOX_MESSAGE_SIZE ];
Message uartOutboxMessage[ UART_OUTBOX_QUEUE_LENGTH ];


void initUARTQueues()
{
    initQueue(&uartInboxQueue, &uartInboxMessage[0], &uartInboxMessageData[0][0], UART_INBOX_MESSAGE_SIZE, UART_INBOX_QUEUE_LENGTH);
    initQueue(&uartOutboxQueue, &uartOutboxMessage[0], &uartOutboxMessageData[0][0], UART_OUTBOX_MESSAGE_SIZE, UART_OUTBOX_QUEUE_LENGTH);
}

signed char writeUARTInboxQueue(unsigned char* buffer, unsigned char length)
{
    return writeQueue(0, &uartInboxQueue, buffer, length);
}

signed char readUARTInboxQueue(unsigned char* buffer)
{
    return readQueue(&uartInboxQueue, (void*)buffer);
}

signed char writeUARTOutboxQueue(unsigned char* buffer, unsigned char length)
{
    return writeQueue(0, &uartOutboxQueue, buffer, length);
}

signed char readUARTOutboxQueue(unsigned char* buffer)
{
    return readQueue(&uartOutboxQueue, (void*)buffer);
}

signed char isEmptyUARTInboxQueue()
{
    return isEmptyQueue(&uartInboxQueue);
}

signed char isEmptyUARTOutboxQueue()
{
    return isEmptyQueue(&uartOutboxQueue);
}