# can-telemetry

A Particle library that allows for basic telemetry through the CAN network. 

## Get started 

To get started, either clone this repo and add it as a private library in your Particle account, or add the files to your source directory. 

For the first option, run the commands `particle library upload`, and add it to a project by running `particle library add can-telemetry`.

## Usage

Add the library as a dependency to your particle project (`particle library add can-telemetry`), or add the source files directly to your project source. Then make sure you include the header `#include "can-telemetry.h"`

See the [examples](examples) folder for more details.

## Documentation

`CANTelemetry(CANChannel &channel, int baud_rate, int timeout, bool debug)`
- `channel` - the CAN channel object
- `baud_rate` - the desired baud_rate for the CAN channel
- `timeout` - the desired timeout before we stop listening for a response (optional, default 1000 ms)
- `debug` - flag to enable `CAN_TEST_MODE` (optional, default false)

`poll(uint32_t header, uint32_t filter, int len)`
- `header` - the CAN ID to send
- `filter` - the response ID desired (optional, default header)
- `len` - the length of the response message (optional, default 8)

`interpret<type>(uint64_t data, int start, int end)`
- `type` - the desired return type
- `data` - the 8 byte data to interpret
- `start` - the starting byte
- `end` - the ending byte
NOTE: if start > end, `interpret` assumes big-endian.