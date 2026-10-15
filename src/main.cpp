#include <Arduino.h>
#include <mcp2515.h>

int delayTime = 500;
int count;

void setup(){
	pinMode(LED_BUILTIN, OUTPUT);
	Serial.begin(115200);
	Serial.println("Hello from STM32!");

	count = 0;
	Serial.printf("Count: %d\n", count);
}

void loop(){
	digitalWrite(LED_BUILTIN, HIGH);
	delay(delayTime);
	digitalWrite(LED_BUILTIN, LOW);
	delay(delayTime);

	count++;
	Serial.printf("Count: %d\n", count);
}
