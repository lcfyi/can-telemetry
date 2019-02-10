// Example usage for can-telemetry library by Leslie Cheng.

#include "can-telemetry.h"

// Initialize a CAN network 
CANChannel can(CAN_D1_D2);

// Initialize the telemetry object, node id 0, debug
CANTelemetry tele(can, 50000, 0, 1000, true);

unsigned long time_ms;

void setup() {
    // No real setup required
    Serial.begin(9600);
}

void loop() {
    // Send an empty CAN packet with the supplied header
    uint64_t val = tele.poll(0x054); 
    // Interpret the bytes as a float
    float ret_float = tele.interpret<float>(val, 0, 3);
    // Interpret the bytes as a float, big-endian
    float ret_float_be = tele.interpret<float>(val, 3, 0);
    // Interpret the bytes as an int
    int ret_int = tele.interpret<int>(val, 0, 3);
    // Wait 5 seconds
    delay(5000);
}
