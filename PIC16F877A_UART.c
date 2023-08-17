#include "PIC16F877A_UART.h"

#define _XTAL_FREQ 16000000

// Function to initiate the UART transmission module only
void UARTTransInit()
{
    // Configure Tx and Rx pins
    TRISC6 = 1;
    TRISC7 = 1;
    // Enable serial port
    SPEN = 1;
    // Configure asynchronous mode
    SYNC = 0;
    // 8-bit transmission mode
    TX9 = 0;
    // Enable transmission
    TXEN = 1;
    // Configure the baud rate
    BRGH = 1;
    SPBRG = ((_XTAL_FREQ) / BAUD_RATE) / 16 - 1;
}

// Function to initiate only the UART reception module
void UARTRcvInit()
{
    // Configure Tx and Rx pins
    TRISC6 = 1;
    TRISC7 = 1;
    // Enable serial port
    SPEN = 1;
    // Configure asynchronous mode
    SYNC = 0;
    // 8-bit reception mode
    RX9 = 0;
    // Enable continuous reception
    CREN = 1;
    // Configure the baud rate
    BRGH = 1;
    SPBRG = ((_XTAL_FREQ) / BAUD_RATE) / 16 - 1;
    // Enable receive interrupt
    RCIE = 1;
    PEIE = 1;
    GIE = 1;
}

// Function to initiate the UART both transmit and receive on PIC16F877A
void UARTTransRcvInit()
{
    // Configure Tx and Rx pins
    TRISC6 = 1;
    TRISC7 = 1;
    // Enable serial port
    SPEN = 1;
    // Configure asynchronous mode
    SYNC = 0;
    // 8-bit transmission and reception mode
    TX9 = 0;
    RX9 = 0;
    // Enable both transmission and reception
    TXEN = 1;
    CREN = 1;
    // Configure the baud rate
    BRGH = 1;
    SPBRG = ((_XTAL_FREQ) / BAUD_RATE) / 16 - 1;
    // Enable receive interrupt
    RCIE = 1;
    PEIE = 1;
    GIE = 1;
}

// Function to send a character via UART
void UARTsendChar(char c)
{
    // Check if the transmit register is empty
    while (!TRMT)
        ;
    TXREG = c;
}

// Function to send a string via UART
void UARTsendString(char *str)
{
    for (int i = 0; str[i] != '\0'; ++i)
        UARTsendChar(str[i]);
}

// Function to receive single character via UART interrupt
char UARTrcvChar()
{
    char c;
    c = RCREG;
    return c;
}

// Function to receive string via UART interrupt
int UARTrcvString(char *rcv_buffer, uint16_t length)
{
    if (uart_str_idx == length - 1)
    {
        rcv_buffer[uart_str_idx] = UARTrcvChar();
        uart_str_idx++;
        rcv_buffer[uart_str_idx] = '\0';
        uart_str_idx = 0;
        return 1;
    }
    else
    {
        rcv_buffer[uart_str_idx] = UARTrcvChar();
        uart_str_idx++;
        return 0;
    }
}