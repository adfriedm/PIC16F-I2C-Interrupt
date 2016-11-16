/* 
 * File:   i2c2message.h
 * Author: adfriedman
 *
 * Created on March 6, 2014, 1:41 AM
 */

#ifndef I2C2MESSAGE_H
#define	I2C2MESSAGE_H

#define I2C2_INBOX_MESSAGE_SIZE     4
#define I2C2_INBOX_QUEUE_LENGTH     3

#define I2C2_OUTBOX_MESSAGE_SIZE    31
#define I2C2_OUTBOX_QUEUE_LENGTH    3


void initi2c2Queues();

signed char writei2c2InboxQueue(unsigned char* buffer, unsigned char length);
signed char readi2c2InboxQueue(unsigned char* buffer);

signed char writei2c2OutboxQueue(unsigned char* buffer, unsigned char length);
signed char readi2c2OutboxQueue(unsigned char* buffer);

signed char isEmptyi2c2InboxQueue();
signed char isEmptyi2c2OutboxQueue();

#endif	/* I2C2MESSAGE_H */

