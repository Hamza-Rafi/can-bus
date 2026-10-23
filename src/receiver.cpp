#include "can_node.h"


#ifdef ARDUINO_AVR_UNO
can_node receiver(10);
#else
can_node receiver(D10);
#endif 

void setup() {
    Serial.begin(115200);

    if (!receiver.init_can()) {
        while (1);   // halt if the MCP2515 didn't initialise
    }

    Serial.println("Receiver ready");
}

void loop() {
    struct can_frame frame;
    receiver.receive_can(frame);
}
