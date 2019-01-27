#include "can-telemetry.h"

/** 
 * Constructor for the telematics object. 
 * @param &channel  CAN channel object
 * @param baud_rate desired baud_rate to listen at 
 * @param timeout   timeout before returning sentinel values
 * @param debug     debug flag 
 */
CANTelemetry::CANTelemetry(CANChannel &channel, int baud_rate, 
                            int timeout, bool debug) {
    _can = &channel;
    _baud_rate = baud_rate;
    _timeout = (unsigned long)timeout;
    _debug = debug;
}

/**
 * Poll the CAN bus, and returns the values as an unsigned 64-bit object.
 * So an unsigned long long, basically.
 * @param header    header to poll the bus with
 * @param filter    response pattern to listen from the bus
 * @param len       length of the return byte
 */
uint64_t CANTelemetry::poll(uint32_t header, uint32_t filter, int len) {
    if (_debug) {
        _can->begin(_baud_rate, CAN_TEST_MODE);
    } else {
        _can->begin(_baud_rate);
    }
    
    // Listen for only specific messages
    _can->addFilter(filter, 0x7FF);

    // Create the empty packet and send it
    CANMessage message;
    message.id = header;
    message.len = 0;
    _can->transmit(message);

    // Wait for a response, or timeout
    if (_can->errorStatus() == CAN_NO_ERROR) {
        CANMessage response;
        unsigned long t_stamp = millis();
        while (true) {
            if (_can->receive(response)) {
                _can->end();
                return _decode(response.data, len);
            }
            if (millis() - t_stamp >= _timeout) {
                _can->end();
                return -1;
            }
        }
    } else {
        _can->end();
        return -1;
    }
}

/**
 * Helper method for the common scenario in which the header is the 
 * same as the filter.
 */
uint64_t CANTelemetry::poll(uint32_t header, int len) {
    return poll(header, header, len);
}

/** 
 * Decode and return the bytes as an unsigned 64-bit value (unsigned long
 * long). This is a private helper method.
 */
uint64_t CANTelemetry::_decode(uint8_t * arr, int len) {
    uint8_t n[len] = {0};
    for (int i = 0; i < len; i++) {
        n[i] = arr[i];
    }
    return *(uint64_t *)n;
}