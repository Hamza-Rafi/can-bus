# can-bus

Code for a CAN network using STM32 and MCP2515 boards

## Requirements

[PlatformIO](https://platformio.org/)

## Setup

Clone the repo:

`git clone <url>`

`cd <repo>`

Build:

`pio run`

Flash to device:

`pio run -e <sender|receiver> -t upload`

Generate `compile_commands.json` (for clangd lsp):

`pio run -e <env_name> -t compiledb`
