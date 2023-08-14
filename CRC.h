#ifndef CRC_H
#define CRC_H

static const uint16_t wCRCTable[];

uint16_t crc16(uint8_t* buffer, uint16_t length);

#endif