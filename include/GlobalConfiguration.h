#ifndef _GLOBALCONFIGURATION_H_
#define _GLOBALCONFIGURATION_H_

#include "peripherals/Controller.h"
#include "peripherals/Display.h"
#include "peripherals/Pad.h"
#include "peripherals/PanelLeds.h"

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
    26,      // SDA Pin
    27,      // SCL Pin
    i2c1,    // Block
    1000000, // Speed
};

const Peripherals::Pad<9>::Config pad_config = {
    // Thresholds
    {
        500, // UP_LEFT
        500, // UP
        500, // UP_RIGHT
        500, // LEFT
        500, // CENTER
        500, // RIGHT
        500, // DOWN_LEFT
        500, // DOWN
        500, // DOWN_RIGHT
    },

    25, // Debounce delay in milliseconds

    //  ADC Channels
    {
        0, // UP_LEFT
        1, // UP
        2, // UP_RIGHT
        3, // LEFT
        4, // CENTER
        5, // RIGHT
        6, // DOWN_LEFT
        7, // DOWN
        8, // DOWN_RIGHT
    },

    // ADC Config
    Peripherals::Pad<pad_config.PANEL_COUNT>::Config::ExternalAdc<3>{
        // SPI Config
        {
            11,       // MOSI
            12,       // MISO
            10,       // SCLK
            spi1,     // SPI Block
            10000000, // Speed in Hz
                      // 2000000, // Speed in Hz
        },

        // ADCs
        {{
            {
                14, // SCSN Pin
                15, // DRDY Pin
            },
            {
                7, // SCSN Pin
                8, // DRDY Pin
            },
            {
                5, // SCSN Pin
                6, // DRDY Pin
            },
        }},
    },
};

const Peripherals::PanelLeds::Config led_config = {
    {128, 128, 128}, // Idle Color
    {255, 0, 0},     // UP Color
    {0, 0, 255},     // DOWN Color
    {255, 255, 0},   // LEFT Color
    {0, 255, 255},   // RIGHT Color

    29,    // LED Pin
    false, // Is RGBW

    255,  // Brightness
    true, // Idle Color is DS4 light bar color
};

const Peripherals::Controller::Config controller_config = {

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

    // GPIO Config, either InternalGpio or ExternalGpio
    //
    // Peripherals::Controller::Config::InternalGpio{},

    Peripherals::Controller::Config::ExternalGpio{
        {
            i2c_config.block, // Block
            0x20,             // Address
        },
    },
};

const Peripherals::Display::Config display_config = {
    i2c_config.block, // Block
    0x3C,             // Address
};

} // namespace Default
} // namespace Dancecon::Config

#endif // _GLOBALCONFIGURATION_H_