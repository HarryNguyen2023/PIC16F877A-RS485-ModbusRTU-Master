#include "PIC16F877A_RS485.h"

// --------------------------------Static private functions hidden from user------------------------------------------------

// Function to config the TX_EN pin connect to the transceiver
static void RS485TxConfig()
{
    // Set the pin as output pin
    TRIS_RS485_TX_EN = 0;
    // Initiate it as receive mode 
    RS485_TX_EN = 0;
}

// ---------------------------------Public function to interface with user---------------------------------------------------

// Function to configure the MCU to use the RS485 driver
void RS485Init()
{
    // Configure the enable pin
    RS485TxConfig();
    // Initiate the UART module at both transmit and receive mode using the maximum allowable baud rate 57600
    UARTTransRcvInit();
}

// Function to send a character over RS485 
void RS485sendChar(char c)
{
    // Enable transmission on RS485 line
    RS485_TX_EN = 1;
    // Send the character via UART
    UARTsendChar(c);
    // Disable transmission line
    RS485_TX_EN = 0;
}

// Function to send a string over RS485
void RS485sendString(char* str)
{
    // Enable transmission on RS485 line
    RS485_TX_EN = 1;
    // Send the character via UART
    UARTsendString(str);
    // Disable transmission line
    RS485_TX_EN = 0;
}

// Function to receive a character over RS485
char RS485rcvChar()
{
    // Enable reception
    RS485_TX_EN = 0;
    // Receive the character via UART
    return UARTrcvChar();
}

// Function to receive a string over RS485
int RS485rcvString(char* rcv_buffer, uint16_t length)
{
    // Enable reception
    RS485_TX_EN = 0;
    // Receive the string over UART
    int done = UARTrcvString(rcv_buffer, length);
    return done;
}