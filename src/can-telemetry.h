#pragma once
// Load the definition for common Particle objects
#include "Particle.h"

#define CALL_AND_RESP   1
#define PASSIVE_POLL    2

#define REMOTE_FRAME    true
#define DATA_FRAME      false

class CANTelemetry {
public:
    CANTelemetry(CANChannel &channel, int baud_rate, int node_id,
                int timeout = 1000, bool debug = false);

    uint64_t poll(uint32_t header, uint32_t filter, int mode = CALL_AND_RESP,
                  bool frame = DATA_FRAME, uint8_t payload[] = NULL, int len = 0);
    uint64_t poll(uint32_t header, int mode = CALL_AND_RESP, bool frame = DATA_FRAME,
                  uint8_t payload[] = NULL, int len = 0);

    void change_timeout(int timeout = 1000);

    template <class T>
    T interpret(uint64_t data, int starting, int ending);
private:
    CANChannel * _can;
    int _baud_rate;
    unsigned long _timeout;
    int _node_id;
    bool _debug;
    uint64_t _decode(uint8_t * arr, int len);
    void _set_mask(int mask);
    void _set_mask();
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
T CANTelemetry::interpret(uint64_t data, int s, int e) {
    uint8_t n[8] = {0};
    uint8_t * data_cast = reinterpret_cast<uint8_t *>(&data);
    if (s > e) {
        for (int i = s, a = 0; i >= e; i--, a++) {
            n[a] = data_cast[i];
        }
    } else {
        for (int i = s, a = 0; i <= e; i++, a++) {
            n[a] = data_cast[i];
        }
    }
    return *(T *)n;
}