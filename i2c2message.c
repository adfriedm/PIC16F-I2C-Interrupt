#include "i2c2message.h"
#include "messages.h"


// MessageQueue data allocation
MessageQueue i2c2InboxQueue;
unsigned char i2c2InboxMessageData[ I2C2_INBOX_QUEUE_LENGTH ][ I2C2_INBOX_MESSAGE_SIZE ];
Message i2c2InboxMessage[ I2C2_INBOX_QUEUE_LENGTH ];

MessageQueue i2c2OutboxQueue;
unsigned char i2c2OutboxMessageData[ I2C2_OUTBOX_QUEUE_LENGTH ][ I2C2_OUTBOX_MESSAGE_SIZE ];
Message i2c2OutboxMessage[ I2C2_OUTBOX_QUEUE_LENGTH ];


void initi2c2Queues()
{
    initQueue(&i2c2InboxQueue, &i2c2InboxMessage[0], &i2c2InboxMessageData[0][0], I2C2_INBOX_MESSAGE_SIZE, I2C2_INBOX_QUEUE_LENGTH);
    initQueue(&i2c2OutboxQueue, &i2c2OutboxMessage[0], &i2c2OutboxMessageData[0][0], I2C2_OUTBOX_MESSAGE_SIZE, I2C2_OUTBOX_QUEUE_LENGTH);
}

signed char writei2c2InboxQueue(unsigned char* buffer, unsigned char length)
{
    return writeQueue(0, &i2c2InboxQueue, buffer, length);
}

signed char readi2c2InboxQueue(unsigned char* buffer)
{
    return readQueue(&i2c2InboxQueue, (void*)buffer);
}

signed char writei2c2OutboxQueue(unsigned char* buffer, unsigned char length)
{
    return writeQueue(0, &i2c2OutboxQueue, buffer, length);
}

signed char readi2c2OutboxQueue(unsigned char* buffer)
{
    return readQueue(&i2c2OutboxQueue, (void*)buffer);
}

signed char isEmptyi2c2InboxQueue()
{
    return isEmptyQueue(&i2c2InboxQueue);
}

signed char isEmptyi2c2OutboxQueue()
{
    return isEmptyQueue(&i2c2OutboxQueue);
}