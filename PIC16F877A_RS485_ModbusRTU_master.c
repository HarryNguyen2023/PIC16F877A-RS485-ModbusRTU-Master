#include "config.h"
#include "RS485_ModbusRTU_master.h"

// Define the external crystal frequency
#define _XTAL_FREQ 16000000
#define BUFFER_LEN 25

// Global variables
uint8_t tx_buffer[BUFFER_LEN];
uint8_t rcv_buffer[BUFFER_LEN];
char error_msg[] = "Transmission error";

// Testing variables
uint8_t coil_write_state[] = {0xCD, 0x21};
uint16_t register_write_buffer[] = {113, 48};
uint8_t state = 0;

// Function to handle interrupt service routine
void __interrupt() ISR(void)
{
    // Receive message from slave via ModbusRTU
    if(RCIF)
    {
        if(modbusRcvResponse(rcv_buffer))
        {
            // Write some application code here
        }
    }
}

void main(void) {    
    // Initiate the ModbusRTU communication
    modbusRTUInit();
    
    // Main duty
    while(1)
    {
        switch(state++)
        {
            case 0:
                if(! modbusReadCoils(tx_buffer, BUFFER_LEN, READ_COILS, 0x05, 1005, 7))
                    RS485sendString(error_msg);
                break;
            case 1:
                if(! modbusReadCoils(tx_buffer, BUFFER_LEN, READ_DISCRETE_INPUTS, 0x05, 1001, 7))
                    RS485sendString(error_msg);
                break;
            case 2:
                if(! modbusReadRegisters(tx_buffer, BUFFER_LEN, READ_HOLDING_REGISTERS, 0x05, 4003, 3))
                    RS485sendString(error_msg);
                break;
            case 3:
                if(! modbusReadRegisters(tx_buffer, BUFFER_LEN, READ_INPUT_REGISTERS, 0x05, 4005, 2))
                    RS485sendString(error_msg);
                break;
            case 4:
                if(! modbusWriteCoil(tx_buffer, BUFFER_LEN, 0x05, 1003, COIL_ON))
                    RS485sendString(error_msg);
                break;
            case 5:
                if(! modbusWriteRegister(tx_buffer, BUFFER_LEN, 0x05, 4001, 15))
                    RS485sendString(error_msg);
                break;
            case 6:
                if(! modbusWriteCoils(tx_buffer, BUFFER_LEN, 0x05, 1001, 13, coil_write_state))
                    RS485sendString(error_msg);
                break;
             case 7:
                state = 0;
                if(! modbusWriteRegisters(tx_buffer, BUFFER_LEN, 0x05, 4002, 2, register_write_buffer))
                    RS485sendString(error_msg);
                break;            
        }
        __delay_ms(1000);
    }
    
    return;
}
