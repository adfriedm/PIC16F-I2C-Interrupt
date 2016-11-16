#include "maindefs.h"
#include "messages.h"
#include <string.h>

void initQueue(MessageQueue* queue, Message* message, unsigned char* messageData, unsigned char maxMessageSize, unsigned char queueLength)
{
    queue->message = message;
    queue->read = 0;
    queue->write = 0;
    queue->maxMessageSize = maxMessageSize;
    queue->queueLength = queueLength;

    int i;
    for(i=0; i<queueLength; ++i)
    {
        queue->message[i].length = 0;
        queue->message[i].messageData = &messageData[i*maxMessageSize];
    }
}


signed char writeQueue(signed char messageType, MessageQueue* queue, unsigned char* buffer, unsigned char length)
{
    // If something at the write address then circular buffer has caught up to itself
    if( queue->message[ queue->write ].length )
    {
	return QUEUE_ERROR_FULL;
    }
    // If the length is greater than the designated max message size
    else if( length > queue->maxMessageSize )
    {
	return QUEUE_ERROR_LONG;
    }

    // Otherwise copy data into queue
    memcpy(  queue->message[ queue->write ].messageData,  buffer,  length  );
    queue->message[ queue->write ].length = length;
    queue->message[ queue->write ].messageType = messageType;

    // Circular increment the write address
    queue->write = (queue->write+1) % queue->queueLength;

    return 0;
}


signed char readQueue(MessageQueue* queue, unsigned char* buffer)
{
    // If nothing at the read address then empty

    unsigned char length = queue->message[ queue->read ].length;
    if( !length )
    {
        return QUEUE_ERROR_EMPTY;
    }

    // Otherwise copy data into queue
    memcpy(  buffer, queue->message[ queue->read ].messageData,  length  );

    // Unmark the previous data
    queue->message[ queue->read ].length = 0;

    // Circular increment the read address
    queue->read = (queue->read+1) % queue->queueLength;

    return length;
}


signed char isEmptyQueue(MessageQueue* queue)
{
    return (queue->message[ queue->read ].length == 0);
}