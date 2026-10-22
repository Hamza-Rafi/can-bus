#include <Arduino.h>
#include "can_node.h"

// D10 is the CS pin
can_node sender(D10);

void setup() {
    Serial.begin(115200);
    delay(5000); // just there to see the init message this can be commented out
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    if (!sender.init_can()) {
        while (1);   // halt if the MCP2515 didn't initialise
    }
}

void loop(){
    struct can_frame frame;
    frame.can_id = 0x123;
    frame.can_dlc = 4; // Data Length Code - How many bytes of data

    frame.data[0] = 0x12;
    frame.data[1] = 0x34;
    frame.data[2] = 0x56;
    frame.data[3] = 0x78;

    sender.send_can(frame);
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(1000);
}