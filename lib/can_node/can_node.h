#pragma once

#include "WSerial.h"
#include "api/Common.h"
#include "mcp2515.h"
#include "pins_arduino_digital.h"
#include "variant_NUCLEO_F446RE.h"
#include <cstdint>
#include "api/Print.h"
#include <Arduino.h>

class can_send {
  public:
    can_send(uint8_t cs_pin);
    bool init_can();
    bool send_can(can_frame& frame);

  private:
    MCP2515 _can;
};

class can_receive {

};