#include "RS485_ModbusRTU_slave.h"

// --------------------------------Static private functions hidden from user------------------------------------------------
static void clearBuffer(uint8_t *buffer, uint16_t len)
{
    for (uint8_t i = 0; i < len; ++i)
        buffer[i] = 0;
}

// ---------------------------------Public function to interface with user---------------------------------------------------

// Function to receive request message from the master
int modbusRcvRequest(uint8_t *rcv_buffer)
{
    // Read the data bit by bit
    int modbus_rcv_request_done = RS485rcvString(rcv_buffer, slave_rcv_buffer_len);

    // Case receive finish
    if (modbus_rcv_request_done)
    {
        slave_rcv_buffer_len = 8;
        change_rcv_len = 0;
        if (rcv_buffer[0] != MODBUS_SLAVE_ADDRESS)
            return 0;
        else
            return 1;
    }

    // Get the function code to define the number of
    if (modbus_rcv_request_done == 0 && uart_str_idx == 2)
    {
        if (rcv_buffer[1] == 0x0F || rcv_buffer[1] == 0x10)
            change_rcv_len = 1;
    }

    // Update the new receive len
    if (change_rcv_len)
    {
        if (modbus_rcv_request_done == 0 && uart_str_idx == 7)
            slave_rcv_buffer_len += 1 + rcv_buffer[6];
    }
    return modbus_rcv_request_done;
}

// Function to response the request for coil/discrete input read via ModbusRTU
uint8_t modbusResponseCoilsRead(uint8_t *tx_buffer, uint16_t length, uint8_t *rcv_buffer)
{
    // Clear the transmit buffer
    clearBuffer(tx_buffer, length);

    // Assgin the address and function
    tx_buffer[0] = MODBUS_SLAVE_ADDRESS;
    tx_buffer[1] = rcv_buffer[1];

    // Get the number of coil to be read
    uint16_t coil_num = (rcv_buffer[4] << 8) + rcv_buffer[5];
    tx_buffer[2] = (coil_num / 8) + 1;

    // Assign the value of coils
    uint8_t i;
    for (i = 0; i < coil_num; ++i)
        tx_buffer[i + 3] = 0x0D;

    // Assign the CRC value
    uint16_t checksum = crc16(tx_buffer, i);
    tx_buffer[i++] = checksum & 0xFF;
    tx_buffer[i] = (checksum >> 8) & 0xFF;

    // Send the string via RS485
    RS485sendString((char *)tx_buffer);

    return 1;
}

// Function to response the request for coil/discrete input read via ModbusRTU
uint8_t modbusResponseRegistersRead(uint8_t *tx_buffer, uint16_t length, uint8_t *rcv_buffer, uint16_t *register_value)
{
    // Clear the transmit buffer
    clearBuffer(tx_buffer, length);

    // Assgin the address and function
    tx_buffer[0] = MODBUS_SLAVE_ADDRESS;
    tx_buffer[1] = rcv_buffer[1];

    // Get the number of coil to be read
    uint16_t register_num = (rcv_buffer[4] << 8) + rcv_buffer[5];
    tx_buffer[2] = register_num * 2;

    // Assign the value of registers
    uint8_t i;
    for (i = 0; i < register_num * 2; ++i)
    {
        if (i % 2 == 0)
            tx_buffer[3 + i] = (register_value[i / 2] >> 8) & 0xFF;
        else
            tx_buffer[3 + i] = register_value[i / 2] & 0xFF;
    }

    // Assign the CRC value
    uint16_t checksum = crc16(tx_buffer, i);
    tx_buffer[i++] = checksum & 0xFF;
    tx_buffer[i] = (checksum >> 8) & 0xFF;

    // Send the string via RS485
    RS485sendString((char *)tx_buffer);

    return 1;
}

// Function to response the request for writing single coil/register via ModbusRTU
uint8_t modbusResponseSingleWrite(uint8_t *tx_buffer, uint16_t length, uint8_t *rcv_buffer)
{
    // Clear the transmit buffer
    clearBuffer(tx_buffer, length);

    memcpy(tx_buffer, rcv_buffer, strlen(rcv_buffer) + 1);

    // Send the string via RS485
    RS485sendString((char *)tx_buffer);
    return 1;
}

// Function to response the request for writing multiple coil/register via ModbusRTU
uint8_t modbusResponseMultipleWrite(uint8_t *tx_buffer, uint16_t length, uint8_t *rcv_buffer)
{
    // Clear the transmit buffer
    clearBuffer(tx_buffer, length);

    // Copy the 6th first elements from the receive message
    uint8_t i;
    for (i = 0; i < 6; ++i)
        tx_buffer[i] = rcv_buffer[0];

    // Assign the CRC value
    uint16_t checksum = crc16(tx_buffer, i);
    tx_buffer[i++] = checksum & 0xFF;
    tx_buffer[i] = (checksum >> 8) & 0xFF;

    // Send the string via RS485
    RS485sendString((char *)tx_buffer);
    return 1;
}