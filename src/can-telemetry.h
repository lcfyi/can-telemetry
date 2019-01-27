#pragma once
// Load the definition for common Particle objects
#include "Particle.h"

class CANTelemetry {
public:
    CANTelemetry(CANChannel &channel, int baud_rate, int timeout, bool debug);
    CANTelemetry(CANChannel &channel, int baud_rate);

    uint64_t poll(uint32_t header, uint32_t filter, int len);
    uint64_t poll(uint32_t header, int len);
    uint64_t poll(uint32_t header);

    template <class T>
    T interpret(uint64_t data, int start, int end);
private:
    CANChannel * _can;
    int _baud_rate;
    unsigned long _timeout;
    bool _debug;
    uint64_t _decode(uint8_t * arr, int len);
};

/** 
 * Definitions of templated methods need to be in the header to avoid
 * linking issues, thus the definition is here. This method takes a 
 * 64-bit value, and reinterprets it as whatever sort of value desired.
 * @param data  the data (generally one returned from poll)
 * @param start the starting byte (inclusive)
 * @param end   the ending byte (inclusive)
 * NOTE that this supports little and big-endian byte sequences.
 */
template <class T>
T CANTelemetry::interpret(uint64_t data, int start, int end) {
    uint8_t n[8] = {0};
    uint8_t * data_cast = reinterpret_cast<uint8_t *>(&data);
    if (start > end) {
        for (int i = start, a = 0; i >= end; i--, a++) {
            n[a] = data_cast[i];
        }
    } else {
        for (int i = start, a = 0; i <= end; i++, a++) {
            n[a] = data_cast[i];
        }
    }
    return *(T *)n;
}