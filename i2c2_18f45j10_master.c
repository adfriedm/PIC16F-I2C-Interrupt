#include "maindefs.h"
#include "i2c2_18f45j10_master.h"

#include <plib/i2c.h>
#include <string.h>
#include "messages.h"
#include "i2c2message.h"


typedef enum {IDLE, WS_1, WS_2, WS_3, WS_4, RS_1, RS_2, RS_3, RS_4, RS_5, RS_6} I2C2State;


// Globals (to be encapsulated later)
unsigned char address;
I2C2State i2c2State;

unsigned char buffer[MAX_I2C2_MASTER_BUFFER];


unsigned char curByte;
unsigned char length;

void initI2C2Master()
{
    // Pull-up resistors
    I2C2_SCL = 1;
    I2C2_SDA = 1;


    PIE3bits.SSP2IE = 1; // Enable SSP2 interrupts
    IPR3bits.SSP2IP = 1; // Set SSP2 to High priority


    SSP2ADD = 0x77; // 100kHz
    //////////////////////////////////
    //    SSP2STAT Register setup   //
    //////////////////////////////////
    SSP2STAT = 0;
    SSP2STATbits.SMP = 1; // Slew rate 100 KHz
    //SSP2STATbits.CKE = 0; //SMBus?
    // SSP2STATbits.D_A // Dont touch for master
    // SSP2STATbits.P // Stop bit
    // SSP2STATbits.S // Start bit
    // SSP2STATbits.RW // Is busy?
    // SSP2STATbits.UA // Ignore for master
    // SSP2STATbits.BF // Buffer full bit



    //////////////////////////////////
    //    SSP2CON1 Register setup   //
    //////////////////////////////////
    SSP2CON1 = 0;
    // SSP2CON1bits.WCOL // Write collision detect bit FOR ERROR
    // SSP2CON1bits.SSPOV // Buffer overflow in receive mode
    //SSP2CON1bits.SSPEN = 1; // Enable serial port
    //SSP2CON1bits.CKP2; // Unused in master mode
    SSP2CON1bits.SSPM = 0x08; // Master mode



    //////////////////////////////////
    //    SSP2CON2 Register setup   //
    //////////////////////////////////
    SSP2CON2 = 0;
    // SSP2CON2bits.GCEN; // Unused in master mode
    // SSP2CON2bits.ACKSTAT; // Acknowledge status bit
    // SSP2CON2bits.ACKDT; // Acknowledge Data bit
    // SSP2CON2bits.ACKEN; // Acknowledge Data bit sequence
    // SSP2CON2bits.RCEN // Receive enable bit
    // SSP2CON2bits.PEN //Stop condition enable bit
    // SSP2CON2bits.RSEN // Repeated start condition enable bit
    // SSP2CON2bits.SEN // Start condition enable bit

    // Cannot set RCEN,PEN,RCEN,SEN,SSPxBUF may not be set when busy

    i2c2State = IDLE;

    SSP2CON1bits.SSPEN = 1; // Enable serial port
}


void i2c2_int_handler()
{

    switch(i2c2State)
    {

        //////////////////////////////////////////////////////////
        //              MASTER WRITE STATE MACHINE              //
        //////////////////////////////////////////////////////////
        case WS_1:
            i2c2State = WS_2;
            SSP2BUF = (address<<1) | 0x00;
            break;
        case WS_2:
            if(!SSP2CON2bits.ACKSTAT && !SSP2STATbits.R_W)
            {
                    SSP2BUF = buffer[curByte];

                    if(length == ++curByte)
                    {
                        i2c2State = WS_3;
                    }
                }
            else
            {
                // Something went wrong
                i2c2State = IDLE;
            }
            break;
        case WS_3:
            i2c2State = WS_4;
            SSP2CON2bits.PEN = 1;
            break;
        case WS_4:
            i2c2State = IDLE;
            break;

        /////////////////////////////////////////////////////////
        //              MASTER READ STATE MACHINE              //
        /////////////////////////////////////////////////////////
        case RS_1:
            i2c2State = RS_2;
            SSP2BUF = (address<<1) | 0x01;
            break;
        case RS_2:
            if(!SSP2CON2bits.ACKSTAT)
            {
                SSP2CON2bits.RCEN = 1; // Change to receive mode
                i2c2State = RS_3;
            }
            else
            {
                // Something went wrong
                i2c2State = IDLE;
            }
            break;
        case RS_3:
            // If buffer full
            if(SSP2STATbits.BF)
            {
                buffer[curByte] = SSP2BUF;
                if(++curByte < length)
                {
                    i2c2State = RS_4;
                    SSP2CON2bits.ACKDT = 0;
                    SSP2CON2bits.ACKEN = 1;
                }
                else
                {
                    // Send NACK to end receive
                    i2c2State = RS_5;
                    SSP2CON2bits.ACKDT = 1;
                    SSP2CON2bits.ACKEN = 1;
                }
            }
            // If buffer overflow
            else if(SSP2CON1bits.SSPOV)
            {
                i2c2State = IDLE;
                SSP2CON1bits.SSPOV = 0;
            }
            // Interrupt thrown for other
            else {}
            break;
        case RS_4:
            SSP2CON2bits.RCEN = 1; // Change to receive mode
            i2c2State = RS_3;
        break;
        case RS_5:
            i2c2State = RS_6;
            SSP2CON2bits.PEN2 = 1;
            break;
        case RS_6:
            i2c2State = IDLE;
            writei2c2OutboxQueue(buffer, length);
            break;

        default:
            break;

    }
}




unsigned char i2c2MasterSendData(unsigned char dataLength, char* data, unsigned char slaveAddress)
{
    
    if(i2c2State != IDLE)
    {
        return -1;
    }
    curByte = 0;
    length = dataLength;
    
    address = slaveAddress;
    
    memcpy(buffer, data, length);
    
    i2c2State = WS_1;
    SSP2CON2bits.SEN2 = 1; // Send start bit
    
    return 0;
}




unsigned char i2c2MasterSendString(char* data, unsigned char slaveAddress)
{
    return i2c2MasterSendData( strlen(data)+1, data, slaveAddress);
}




unsigned char isI2C2MasterIdle()
{
    return (i2c2State == IDLE) ;
}




void i2c2MasterRead(unsigned char dataLength, unsigned char slaveAddress)
{
    // CHECK IDLE FIRST!!!

    curByte = 0;
    /*
    if(length > MSGLEN)
    {
        length = MSGLEN;
    }
    */
    length = dataLength;

    unsigned char temp = SSP2BUF; // Clear buffer
    address = slaveAddress;
    i2c2State = RS_1;
    SSP2CON2bits.SEN2 = 1; // Send start bit
}