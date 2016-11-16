/* 
 * File:   i2c2_18f45j10_master.h
 * Author: adfriedman
 *
 * Created on March 1, 2014, 12:24 PM
 */

#ifndef I2C2_18F45J10_MASTER_H
#define	I2C2_18F45J10_MASTER_H

#define MAX_I2C2_MASTER_BUFFER 50

void initI2C2Master();
void i2c2_int_handler();
unsigned char i2c2MasterSendData(unsigned char dataLength, char* data, unsigned char slaveAddress);
unsigned char i2c2MasterSendString(char* data, unsigned char slaveAddress);
unsigned char isI2C2MasterIdle();
void i2c2MasterRead(unsigned char dataLength, unsigned char slaveAddress);


#endif	/* I2C2_18F45J10_MASTER_H */

