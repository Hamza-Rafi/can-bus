#include "can_node.h"

can_send::can_send(uint8_t csPin) : _can(csPin) {}

bool can_send::init_can()
{
  // reset signal puts mcp2515 into config mode
  _can.reset();

  MCP2515::ERROR err =  _can.setBitrate(CAN_500KBPS, MCP_8MHZ);
  if (err != MCP2515::ERROR_OK) {
    Serial.println("MCP2515 init failed!");
    return false;
  }

  // switches the mcp2515 from config mode to normal mode
  mcp2515.setNormalMode(); 
  Serial.println("Sender ready");
  return true;
}

bool can_send::send_can(const can_frame& frame)
{
  MCP2515::ERROR err = _can.sendMessage(&frame);
  
  if (err == MCP2515::ERROR_OK) {
	Serial.println("frame sent");
    return true;
  }

  else {
    uint8_t eflg = _can.getErrorFlags();
	uint8_t tec = _can.errorCountTX();
	uint8_t rec = _can.errorCountRX();

	Serial.print("Send failed. EFLG=0x");
    Serial.print(eflg, HEX);
	Serial.print(" TEC=");
	Serial.print(tec);
	Serial.print(" REC=");
	Serial.print(rec);

	Serial.println();
    return false;
  }
}