#ifndef EVENTS_H_
#define EVENTS_H_

// Lengths
#define SIZE_OF_BINARY_BROADCAST_PROTOCOL 37
#define PAYLOAD_OF_BROADCAST_PROTOCOL 30 

#define PROTOCOL_STX '{'
#define PROTOCOL_ETX '}'
#define ACK 'A'
// Protocol header index is:    protocol.length() - 2
//                    {O}\n             4         - 2 =     2

#define ERROR_PROTOCOL_UNFORMATED   "{XUNFORMATED PROTOCOL}"
#define ERROR_PROTOCOL_SMALL        "{XPROTOCOL TOO SMALL}"
#define ERROR_UNKOWN_PROTOCOL       "{XUNKOWN PROTOCOL}"



// Protocols
#define PROTOCOL_PRINT_STATUS                   '0'
#define PROTOCOL_READ_WRITE_WIFI                'O'
#define PROTOCOL_READ_WRITE_BROADCAST_FREQ      'F'
#define PROTOCOL_READ_MEASURES                  '0'
#define PROTOCOL_BROADCAST                      'Q'
#define PROTOCOL_RESET_BOARD                    'R'



#define MAP_BROADCAST_FREQUENCY "frequency"
#define MAP_CURRENT "current"
#define MAP_VOLTAGE "voltage"
#define MAP_FREQUENCY "frequency"
#define MAP_POWER_FACTOR "power_factor"
#define MAP_APPARENT_POWER "apparent_power"
#define MAP_ACTIVE_POWER "active_power"
#define MAP_REACTIVE_POWER "reactive_power"

#endif