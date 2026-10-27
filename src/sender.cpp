#include <Arduino.h>
#include <can_node.h>

// D10 is the CS pin

#ifdef ARDUINO_AVR_UNO
can_node sender(10);
#else
can_node sender(D10);
#endif 

uint32_t count = 0;
bool led_state = LOW;

void setup() {
    Serial.begin(115200);
    delay(5000); // just there to see the init message this can be commented out
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, led_state);

    if (!sender.init_can()) {
        while (1);   // halt if the MCP2515 didn't initialise
    }
}


void loop(){
    struct can_frame frame;
    frame.can_id = 0x456;
    frame.can_dlc = 4; // Data Length Code - How many bytes of data

    frame.data[0] = (count >> 24) & 0xFF;
    frame.data[1] = (count >> 16) & 0xFF;
    frame.data[2] = (count >> 8) & 0xFF;
    frame.data[3] = count & 0xFF;

    sender.send_can(frame);
    count++;

    // toggle led
    led_state = !led_state;
    digitalWrite(LED_BUILTIN, led_state);

    delay(1000);
}
