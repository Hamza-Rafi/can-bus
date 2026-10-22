#include "can_node.h"

can_node::can_node(uint8_t csPin) : _can(csPin) {}

bool can_node::init_can()
{
    // reset signal puts mcp2515 into config mode
    _can.reset();

    MCP2515::ERROR err =  _can.setBitrate(CAN_500KBPS, MCP_8MHZ);
    if (err != MCP2515::ERROR_OK) {
        Serial.println("MCP2515 init failed!");
        return false;
    }

    // switches the mcp2515 from config mode to normal mode
    _can.setNormalMode(); 
    Serial.println("MCP2515 ready");
    return true;
}

bool can_node::send_can(can_frame& frame)
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

bool can_node::receive_can(can_frame& frame)
{
    if(_can.readMessage(&frame) == MCP2515::ERROR_OK)
    {
        Serial.print("ID: 0x");
        Serial.println(frame.can_id, HEX);

        Serial.print("Data: ");
        for (int i = 0; i < frame.can_dlc; i++) {
            Serial.print(frame.data[i], HEX);
            Serial.print(" ");
        }

        Serial.println(" ");
        Serial.println(" ");

        return true;
    }
    // error handling to be implemented later
    else
    {
      return false;
    }
}