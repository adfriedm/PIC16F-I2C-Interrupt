/*
 * File:   i2c2message.h
 * Author: adfriedman
 *
 * Created on March 6, 2014, 1:41 AM
 */

#ifndef MESSAGES_H
#define	MESSAGES_H

#define QUEUE_ERROR_FULL	-1
#define QUEUE_ERROR_LONG	-2
#define QUEUE_ERROR_EMPTY	-1


typedef struct
{
	unsigned char* messageData;
	unsigned char length;
	signed char messageType;
} Message;

typedef struct
{
	Message* message;
	unsigned char read;
	unsigned char write;
	unsigned char maxMessageSize;
	unsigned char queueLength;
} MessageQueue;


void initQueue(MessageQueue* queue, Message* message, unsigned char* messageData, unsigned char maxMessageSize, unsigned char queueLength);
signed char writeQueue(signed char messageType, MessageQueue* queue, unsigned char* buffer, unsigned char length);
signed char readQueue(MessageQueue* queue, unsigned char* buffer);
signed char isEmptyQueue(MessageQueue* queue);

#endif	/* MESSAGES_H */