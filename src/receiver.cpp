#include "WSerial.h"
#include "api/Print.h"
#include "mcp2515.h"
#include <Arduino.h>

MCP2515 mcp2515(D10);

void setup() {
  Serial.begin(115200);

  mcp2515.reset();

  MCP2515::ERROR err = mcp2515.setBitrate(CAN_500KBPS, MCP_8MHZ);
  if (err != MCP2515::ERROR_OK) {
    Serial.println("MCP2515 init failed");
    while (1)
      ;
  }

  mcp2515.setNormalMode();
  Serial.println("Receiver ready");
}

void loop() {
  struct can_frame frame;

  // print can frame id
  if (mcp2515.readMessage(&frame) == MCP2515::ERROR_OK) {
    Serial.print("ID: 0x");
    Serial.println(frame.can_id, HEX);

    Serial.print("Data: ");
    for (int i = 0; i < frame.can_dlc; i++) {
      Serial.print(frame.data[i], HEX);
      Serial.print(" ");
    }

    Serial.println(" ");
    Serial.println(" ");
  }
}
