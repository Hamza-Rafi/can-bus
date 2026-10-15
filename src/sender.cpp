#include "WSerial.h"
#include "api/Common.h"
#include "mcp2515.h"
#include "pins_arduino_digital.h"
#include "variant_NUCLEO_F446RE.h"
#include <cstdint>

// D10 is the CS pin
MCP2515 mcp2515(D10);

void setup(){
	Serial.begin(115200);

	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);

	// reset signal puts mcp2515 into config mode
	mcp2515.reset();

	MCP2515::ERROR err =  mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
	if (err != MCP2515::ERROR_OK) {
		Serial.println("MCP2515 init failed!");
		while (1);
	}

	// switches the mcp2515 from config mode to normal mode
	mcp2515.setNormalMode(); 
	// mcp2515.setLoopbackMode(); 
	Serial.println("Sender ready");
}

void loop(){
	struct can_frame frame;
	frame.can_id = 0x123;
	frame.can_dlc = 4; // Data Length Code - How many bytes of data
	
	frame.data[0] = 0x12;
	frame.data[1] = 0x34;
	frame.data[2] = 0x56;
	frame.data[3] = 0x78;

	MCP2515::ERROR err = mcp2515.sendMessage(&frame);

	if (err == MCP2515::ERROR_OK) {
		Serial.println("frame sent");
	} else {
		// get the error flags on error
		uint8_t eflg = mcp2515.getErrorFlags();
		uint8_t tec = mcp2515.errorCountTX();
		uint8_t rec = mcp2515.errorCountRX();

		Serial.print("Send failed. EFLG=0x");
		Serial.print(eflg, HEX);
		Serial.print(" TEC=");
		Serial.print(tec);
		Serial.print(" REC=");
		Serial.print(rec);

		Serial.println();

	}

	digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
	delay(1000);
}
