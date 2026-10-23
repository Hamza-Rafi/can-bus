#pragma once

#include "mcp2515.h"
#include <Arduino.h>

class can_node {
    public:
        can_node(uint8_t cs_pin);
        bool init_can();
        bool send_can(can_frame& frame);
        bool receive_can(can_frame& frame);

    private:
        MCP2515 _can;
};
