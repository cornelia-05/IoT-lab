#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <Arduino.h>

// Adresa I2C a placii Slave
constexpr uint8_t SLAVE_ADDRESS = 0x08;

// Comanda trimisa de Master
constexpr uint8_t CMD_READ_DISTANCE = 0x01;

// Structura pachetului
constexpr uint8_t PACKET_HEAD = 0xAA;
constexpr uint8_t PAYLOAD_LENGTH = 3;
constexpr uint8_t PACKET_SIZE = 6;

// Masuratoarea transmisa
struct Measurement {
    uint16_t distanceMm;
    uint8_t valid;
};

// Calcul checksum prin XOR
inline uint8_t calculateChecksum(
    const uint8_t *data,
    uint8_t length
) {
    uint8_t checksum = 0;

    for (uint8_t i = 0; i < length; i++) {
        checksum ^= data[i];
    }

    return checksum;
}

// Construirea pachetului
inline void buildPacket(
    const Measurement &measurement,
    uint8_t *packet
) {
    packet[0] = PACKET_HEAD;
    packet[1] = PAYLOAD_LENGTH;

    // Status masuratoare
    packet[2] = measurement.valid;

    // Octet inferior
    packet[3] =
        measurement.distanceMm & 0xFF;

    // Octet superior
    packet[4] =
        (measurement.distanceMm >> 8) & 0xFF;

    // Checksum
    packet[5] = calculateChecksum(packet, 5);
}

// Validarea pachetului primit
inline bool validatePacket(
    const uint8_t *packet
) {
    if (packet[0] != PACKET_HEAD) {
        return false;
    }

    if (packet[1] != PAYLOAD_LENGTH) {
        return false;
    }

    if (packet[2] > 1) {
        return false;
    }

    return packet[5] ==
        calculateChecksum(packet, 5);
}

// Extragerea distantei din pachet
inline uint16_t decodeDistance(
    const uint8_t *packet
) {
    return (uint16_t)packet[3] |
           ((uint16_t)packet[4] << 8);
}

#endif