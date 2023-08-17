#include "RS485_ModbusRTU_master.h"

// --------------------------------Static private functions hidden from user------------------------------------------------
static void clearBuffer(uint8_t *buffer, uint16_t len)
{
    for (uint8_t i = 0; i < len; ++i)
        buffer[i] = 0;
}

// ---------------------------------Public function to interface with user---------------------------------------------------

// Function to initiate the ModbusRTU communication
void modbusRTUInit()
{
    RS485Init();
}

// Function to read the state of coils via ModbusRTU
uint8_t modbusReadCoils(uint8_t *tx_buffer, uint16_t len, uint8_t mode, uint8_t slave_address, uint16_t start_coil, uint16_t coil_num)
{
    // Clear the transmit buffer
    clearBuffer(tx_buffer, len);

    // Assign the transmit request for reading coil/discrete inputs value
    tx_buffer[0] = slave_address;

    // Assign the function
    if (mode == READ_COILS)
        tx_buffer[1] = READ_COILS;
    else if (mode == READ_DISCRETE_INPUTS)
        tx_buffer[1] = READ_DISCRETE_INPUTS;
    else
        return 0;

    // Assign the starting coil/discrete input value
    tx_buffer[3] = start_coil & 0xFF;
    tx_buffer[2] = (start_coil >> 8) & 0xFF;

    // Assign the number of coil/discrete input to be read
    tx_buffer[5] = coil_num & 0xFF;
    tx_buffer[4] = (coil_num >> 8) & 0xFF;

    // Assign 2 bits of check sum
    uint16_t checksum = crc16(tx_buffer, 6);
    tx_buffer[6] = checksum & 0xFF;
    tx_buffer[7] = (checksum >> 8) & 0xFF;

    // Transmit the request message over RS485
    RS485sendString((char *)tx_buffer);

    // Set the length for the response message intended to be received
    master_rcv_buffer_len = 6 + (coil_num / 8);
    return 1;
}

// Function to read the value of registers via ModbusRTU
uint8_t modbusReadRegisters(uint8_t *tx_buffer, uint16_t len, uint8_t mode, uint8_t slave_address, uint16_t start_register, uint16_t register_num)
{
    // Clear the transmit buffer
    clearBuffer(tx_buffer, len);

    // Assign the transmit request for reading coil/discrete inputs value
    tx_buffer[0] = slave_address;

    // Assign the function
    if (mode == READ_HOLDING_REGISTERS)
        tx_buffer[1] = READ_HOLDING_REGISTERS;
    else if (mode == READ_INPUT_REGISTERS)
        tx_buffer[1] = READ_INPUT_REGISTERS;
    else
        return 0;

    // Assign the starting coil/discrete input value
    tx_buffer[3] = start_register & 0xFF;
    tx_buffer[2] = (start_register >> 8) & 0xFF;

    // Assign the number of coil/discrete input to be read
    tx_buffer[5] = register_num & 0xFF;
    tx_buffer[4] = (register_num >> 8) & 0xFF;

    // Assign 2 bits of check sum
    uint16_t checksum = crc16(tx_buffer, 6);
    tx_buffer[6] = checksum & 0xFF;
    tx_buffer[7] = (checksum >> 8) & 0xFF;

    // Transmit the request message over RS485
    RS485sendString((char *)tx_buffer);

    // Set the length for the response message intended to be received
    master_rcv_buffer_len = 5 + (register_num * 2);
    return 1;
}

// Function to write the state of a coil via ModbusRTU
uint8_t modbusWriteCoil(uint8_t *tx_buffer, uint16_t len, uint8_t slave_address, uint16_t coil, uint8_t coil_state)
{
    // Clear the transmit buffer
    clearBuffer(tx_buffer, len);

    // Assign the transmit request for reading coil/discrete inputs value
    tx_buffer[0] = slave_address;

    // Assign the mode
    tx_buffer[1] = 0x05;

    // Assign the starting coil/discrete input value
    tx_buffer[3] = coil & 0xFF;
    tx_buffer[2] = (coil >> 8) & 0xFF;

    // Assign the number of coil/discrete input to be read
    tx_buffer[5] = 0x00;
    if (coil_state == COIL_ON)
        tx_buffer[4] = 0xFF;
    else if (coil_state == COIL_OFF)
        tx_buffer[4] = 0x00;
    else
        return 0;

    // Assign 2 bits of check sum
    uint16_t checksum = crc16(tx_buffer, 6);
    tx_buffer[6] = checksum & 0xFF;
    tx_buffer[7] = (checksum >> 8) & 0xFF;

    // Transmit the request message over RS485
    RS485sendString((char *)tx_buffer);

    // Set the length for the response message intended to be received
    master_rcv_buffer_len = 8;
    return 1;
}

// Function to write value in a single register via ModbusRTU
uint8_t modbusWriteRegister(uint8_t *tx_buffer, uint16_t len, uint8_t slave_address, uint16_t register_, uint16_t register_value)
{
    // Clear the transmit buffer
    clearBuffer(tx_buffer, len);

    // Assign the transmit request for reading coil/discrete inputs value
    tx_buffer[0] = slave_address;

    // Assign the mode
    tx_buffer[1] = 0x06;

    // Assign the starting coil/discrete input value
    tx_buffer[3] = register_ & 0xFF;
    tx_buffer[2] = (register_ >> 8) & 0xFF;

    // Assign the number of coil/discrete input to be read
    tx_buffer[5] = register_value & 0xFF;
    tx_buffer[4] = (register_value >> 8) & 0xFF;

    // Assign 2 bits of check sum
    uint16_t checksum = crc16(tx_buffer, 6);
    tx_buffer[6] = checksum & 0xFF;
    tx_buffer[7] = (checksum >> 8) & 0xFF;

    // Transmit the request message over RS485
    RS485sendString((char *)tx_buffer);

    // Set the length for the response message intended to be received
    master_rcv_buffer_len = 8;
    return 1;
}

// Function to write values to multiple coils via ModbusRTU
uint8_t modbusWriteCoils(uint8_t *tx_buffer, uint16_t len, uint8_t slave_address, uint16_t start_coil, uint16_t coil_num, uint8_t *coil_state)
{
    // Clear the transmit buffer
    clearBuffer(tx_buffer, len);

    // Assign the transmit request for reading coil/discrete inputs value
    tx_buffer[0] = slave_address;

    // Assign the mode
    tx_buffer[1] = 0x0F;

    // Assign the starting coil/discrete input value
    tx_buffer[3] = start_coil & 0xFF;
    tx_buffer[2] = (start_coil >> 8) & 0xFF;

    // Assign the number of coil/discrete input to be read
    tx_buffer[5] = coil_num & 0xFF;
    tx_buffer[4] = (coil_num >> 8) & 0xFF;

    uint8_t i;
    for (i = 0; i < (coil_num / 8) + 1; ++i)
        tx_buffer[i + 6] = coil_state[i];

    // Assign 2 bits of check sum
    uint16_t checksum = crc16(tx_buffer, i);
    tx_buffer[i++] = checksum & 0xFF;
    tx_buffer[i] = (checksum >> 8) & 0xFF;

    // Transmit the request message over RS485
    RS485sendString((char *)tx_buffer);

    // Set the length for the response message intended to be received
    master_rcv_buffer_len = 8;
    return 1;
}

// Function to write value to registers via ModbusRTU
uint8_t modbusWriteRegisters(uint8_t *tx_buffer, uint16_t len, uint8_t slave_address, uint16_t start_register, uint16_t register_num, uint16_t *register_value)
{
    // Clear the transmit buffer
    clearBuffer(tx_buffer, len);

    // Assign the transmit request for reading coil/discrete inputs value
    tx_buffer[0] = slave_address;

    // Assign the mode
    tx_buffer[1] = 0x10;

    // Assign the starting coil/discrete input value
    tx_buffer[3] = start_register & 0xFF;
    tx_buffer[2] = (start_register >> 8) & 0xFF;

    // Assign the number of coil/discrete input to be read
    tx_buffer[5] = register_num & 0xFF;
    tx_buffer[4] = (register_num >> 8) & 0xFF;

    // Assign the number of data byte to be transmitted
    tx_buffer[6] = register_num * 2;

    uint8_t i;
    for (i = 0; i < register_num * 2; ++i)
    {
        if (i % 2 == 0)
            tx_buffer[3 + i] = (register_value[i / 2] >> 8) & 0xFF;
        else
            tx_buffer[3 + i] = register_value[i / 2] & 0xFF;
    }

    // Assign 2 bits of check sum
    uint16_t checksum = crc16(tx_buffer, i);
    tx_buffer[i++] = checksum & 0xFF;
    tx_buffer[i] = (checksum >> 8) & 0xFF;

    // Transmit the request message over RS485
    RS485sendString((char *)tx_buffer);

    // Set the length for the response message intended to be received
    master_rcv_buffer_len = 8;
    return 1;
}

// Function to read the response from the slave via ModbusRTU
int modbusRcvResponse(uint8_t *rcv_buffer)
{
    return RS485rcvString((char *)rcv_buffer, master_rcv_buffer_len);
}

uint8_t modbusTest(uint8_t *tx_buffer, uint16_t len, uint8_t mode, uint8_t slave_address, uint16_t start_coil, uint16_t coil_num)
{
    // Clear the transmit buffer
    clearBuffer(tx_buffer, len);
    
    tx_buffer[0] = 0x31;
    tx_buffer[1] = 0x32;
    tx_buffer[2] = 0x33;
    tx_buffer[3] = 0x34;
    tx_buffer[4] = 0x35;
    tx_buffer[5] = 0x36;
//    uint16_t checksum = crc16(tx_buffer, 6);
    tx_buffer[6] = 0x37;
    tx_buffer[7] = 0x38;
    // Transmit the request message over RS485
//    RS485sendString((char *)tx_buffer);
    RS485sendString((char*)tx_buffer);
    return 1;
}