#ifndef RS485_MODBUSRTU_SLAVE_H
#define RS485_MODBUSRTU_SLAVE_H

#include "PIC16F877A_RS485.h"
#include "CRC.h"
#include <string.h>

// Define the address of the slave (0 < address < 247)
#define MODBUS_SLAVE_ADDRESS 0x05

// Variable to define the length of the message going to be received
uint16_t slave_rcv_buffer_len = 8;
uint8_t change_rcv_len = 0;

int modbusRcvRequest(uint8_t *rcv_buffer);
uint8_t modbusResponseSingleWrite(uint8_t *tx_buffer, uint16_t length, uint8_t *rcv_buffer);
uint8_t modbusResponseMultipleWrite(uint8_t *tx_buffer, uint16_t length, uint8_t *rcv_buffer);
uint8_t modbusResponseCoilsRead(uint8_t *tx_buffer, uint16_t length, uint8_t *rcv_buffer);
uint8_t modbusResponseRegistersRead(uint8_t *tx_buffer, uint16_t length, uint8_t *rcv_buffer);

#endif