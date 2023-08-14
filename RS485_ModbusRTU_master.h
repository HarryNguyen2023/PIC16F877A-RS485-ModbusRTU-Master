#ifndef RS485_MODBUSRTU_MASTER_H
#define RS485_MODBUSRTU_MASTER_H

#include "PIC16F877A_RS485.h"
#include "CRC.h"

// Define of mode for operation
enum modbusRTU_mode
{
    READ_COILS = 1,
    READ_DISCRETE_INPUTS = 2,
    READ_HOLDING_REGISTERS = 3,
    READ_INPUT_REGISTERS = 4
};

enum coil_status
{
    COIL_ON,
    COIL_OFF
};

// Variable to define the length of the message going to be received
uint16_t rcv_buffer_len;

uint8_t modbusReadCoils(uint8_t *tx_buffer, uint16_t len, uint8_t mode, uint8_t slave_address, uint16_t start_coil, uint16_t coil_num);
uint8_t modbusReadRegisters(uint8_t *tx_buffer, uint16_t len, uint8_t mode, uint8_t slave_address, uint16_t start_register, uint16_t register_num);
uint8_t modbusWriteCoil(uint8_t *tx_buffer, uint16_t len, uint8_t slave_address, uint16_t coil, uint8_t coil_state);
uint8_t modbusWriteRegister(uint8_t *tx_buffer, uint16_t len, uint8_t slave_address, uint16_t register_, uint16_t register_value);
uint8_t modbusWriteCoils(uint8_t *tx_buffer, uint16_t len, uint8_t slave_address, uint16_t start_coil, uint16_t coil_num, uint8_t *coil_state);
uint8_t modbusWriteRegisters(uint8_t *tx_buffer, uint16_t len, uint8_t slave_address, uint8_t start_register, uint8_t register_num, uint16_t *register_value);
int modbusRcvResponse(uint8_t *rcv_buffer);

#endif