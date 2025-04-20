#ifndef _GLOBALCONFIGURATION_H_
#define _GLOBALCONFIGURATION_H_

#include "peripherals/Controller.h"
#include "peripherals/Display.h"
#include "peripherals/Pad.h"
#include "peripherals/PadLeds.h"

#include "hardware/i2c.h"

namespace Dancecon::Config {

struct I2c {
    uint8_t sda_pin;
    uint8_t scl_pin;
    i2c_inst_t *block;
    uint speed_hz;
};

namespace Default {

const usb_mode_t usb_mode = USB_MODE_DEBUG;

const I2c i2c_config = {
    6,       // SDA Pin
    7,       // SCL Pin
    i2c1,    // Block
    1000000, // Speed
};

const Peripherals::Pad::Config pad_config = {
    // Trigger thresholds
    {
        1000, // UP
        1000, // DOWN
        1000, // LEFT
        1000, // RIGHT
    },
    true,   // Raw readings grow negative
    {1, 2}, // Button Pins
    25,     // Debounce delay in milliseconds
};

const Peripherals::PadLeds::Config led_config = {
    {128, 128, 128}, // Idle Color
    {255, 0, 0},     // UP Color
    {0, 0, 255},     // DOWN Color
    {255, 255, 0},   // LEFT Color
    {0, 255, 255},   // RIGHT Color

    12,    // LED Pin
    false, // Is RGBW

    255,  // Brightness
    true, // Idle Color is DS4 light bar color
};

const Peripherals::Buttons::Config button_config = {
    // I2c config
    {
        i2c_config.block, // Block
        0x20,             // Address
    },

    // Pins
    {{
         8,  // Up
         9,  // Down
         10, // Left
         11, // Right
     },
     {
         0, // North
         3, // East
         1, // South
         2, // West

         12, // L
         4,  // R

         5,  // Start
         13, // Select
         6,  // Home
         14, // Share
     }},

    25, // Debounce delay in milliseconds
};

const Peripherals::Display::Config display_config = {
    i2c_config.block, // Block
    0x3C,             // Address
};

} // namespace Default
} // namespace Dancecon::Config

#endif // _GLOBALCONFIGURATION_H_