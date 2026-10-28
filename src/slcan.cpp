// Simple SLCAN (Lawicel) adapter: lets Linux talk to the CAN bus through
// this board, using slcand from can-utils.
//
// Commands from the PC, each ending with '\r':
//   S6            set bitrate (S3=100k S4=125k S5=250k S6=500k S8=1M)
//   O             open the channel (join the bus)
//   C             close the channel
//   t1232AABB     send standard frame: id 123, 2 bytes, data AA BB
//   T000001232AABB  send extended frame: id 00000123
//
// Frames received from the bus are sent to the PC in the same format.
// The board replies '\r' for OK and '\a' (bell) for an error.

#include <Arduino.h>
#include <mcp2515.h>

#ifdef ARDUINO_AVR_UNO
MCP2515 mcp2515(10);
#define SERIAL_BAUD 921600
#else
MCP2515 mcp2515(D10);
#define SERIAL_BAUD 921600
#endif

CAN_SPEED bitrate = CAN_500KBPS;
bool channel_open = false;

char line[32];      // the command being received from the PC
int line_len = 0;

// turn 'len' hex characters (e.g. "1A3") into a number
uint32_t hex_to_int(const char* text, int len) {
    uint32_t value = 0;
    for (int i = 0; i < len; i++) {
        char c = text[i];
        value = value * 16;
        if (c >= '0' && c <= '9') value += c - '0';
        else if (c >= 'A' && c <= 'F') value += c - 'A' + 10;
        else if (c >= 'a' && c <= 'f') value += c - 'a' + 10;
    }
    return value;
}

void reply_ok()    { Serial.write('\r'); }
void reply_error() { Serial.write('\a'); }

// handle a 't' or 'T' command: build a frame and send it on the bus
void send_frame() {
    bool extended = (line[0] == 'T');
    int id_len = extended ? 8 : 3;     // number of hex digits in the id

    if (!channel_open || line_len < id_len + 2) {
        reply_error();
        return;
    }

    struct can_frame frame;
    frame.can_id = hex_to_int(&line[1], id_len);
    frame.can_dlc = hex_to_int(&line[1 + id_len], 1);

    if (frame.can_dlc > 8 || line_len != id_len + 2 + frame.can_dlc * 2) {
        reply_error();
        return;
    }

    for (int i = 0; i < frame.can_dlc; i++) {
        frame.data[i] = hex_to_int(&line[id_len + 2 + i * 2], 2);
    }

    if (extended) {
        frame.can_id |= CAN_EFF_FLAG;
    }

    if (mcp2515.sendMessage(&frame) == MCP2515::ERROR_OK) {
        Serial.write(extended ? 'Z' : 'z');
        reply_ok();
    } else {
        reply_error();
    }
}

// a full command has arrived in 'line'
void handle_command() {
    switch (line[0]) {
        case 'S':   // set bitrate
            if      (line[1] == '3') bitrate = CAN_100KBPS;
            else if (line[1] == '4') bitrate = CAN_125KBPS;
            else if (line[1] == '5') bitrate = CAN_250KBPS;
            else if (line[1] == '6') bitrate = CAN_500KBPS;
            else if (line[1] == '8') bitrate = CAN_1000KBPS;
            else { reply_error(); break; }
            reply_ok();
            break;

        case 'O':   // open channel
            mcp2515.reset();
            mcp2515.setBitrate(bitrate, MCP_8MHZ);
            mcp2515.setNormalMode();
            channel_open = true;
            reply_ok();
            break;

        case 'C':   // close channel
            mcp2515.setConfigMode();
            channel_open = false;
            reply_ok();
            break;

        case 't':   // standard frame
        case 'T':   // extended frame
            send_frame();
            break;

        default:
            reply_error();
            break;
    }
}

// send a frame we received from the bus to the PC
void print_frame(struct can_frame& frame) {
    char text[32];
    int n;

    if (frame.can_id & CAN_EFF_FLAG) {
        n = sprintf(text, "T%08lX%d", (unsigned long)(frame.can_id & CAN_EFF_MASK), frame.can_dlc);
    } else {
        n = sprintf(text, "t%03lX%d", (unsigned long)(frame.can_id & CAN_SFF_MASK), frame.can_dlc);
    }

    for (int i = 0; i < frame.can_dlc; i++) {
        n += sprintf(&text[n], "%02X", frame.data[i]);
    }

    text[n++] = '\r';
    Serial.write(text, n);
}

void setup() {
    Serial.begin(SERIAL_BAUD);
    mcp2515.reset();
    mcp2515.setConfigMode();   // stay off the bus until the PC sends 'O'
}

void loop() {
    // 1. read commands from the PC, one character at a time
    while (Serial.available()) {
        char c = Serial.read();

        if (c == '\r') {                        // end of command
            line[line_len] = '\0';
            if (line_len > 0) handle_command();
            line_len = 0;
        } else if (line_len < (int)sizeof(line) - 1) {
            line[line_len++] = c;
        }
    }

    // 2. pass any frames from the bus to the PC
    if (channel_open) {
        struct can_frame frame;
        if (mcp2515.readMessage(&frame) == MCP2515::ERROR_OK) {
            print_frame(frame);
        }
    }
}
