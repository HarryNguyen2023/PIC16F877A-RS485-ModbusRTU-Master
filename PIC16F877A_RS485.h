#ifndef PIC16F877A_RS485_H
#define PIC16F877A_RS485_H

#include "PIC16F877A_UART.h"

// Define the state control pin of the MAX485/485 driver (HIGH: TX enable, LOW: Rx enable)
#define RS485_TX_EN RD0
#define TRIS_RS485_TX_EN TRISD0

void RS485Init(void);
void RS485sendChar(char c);
void RS485sendString(char* str);
char RS485rcvChar(void);
int RS485rcvString(char* rcv_buffer, uint16_t length);

#endif