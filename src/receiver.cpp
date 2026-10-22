#include "can_node.h"

can_node receiver(D10);

void setup() {
    Serial.begin(115200);

    if (!receiver.init_can()) {
        while (1);   // halt if the MCP2515 didn't initialise
    }
}

void loop() {
    struct can_frame frame;
    receiver.receive_can(frame);
}
