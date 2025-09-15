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
    .sda_pin = 26,
    .scl_pin = 27,
    .block = i2c1,
    .speed_hz = 1000000,
};

const Peripherals::Pad<9>::Config pad_config = {
    .thresholds =
        {
            .up_left = 100,
            .up = 100,
            .up_right = 100,
            .left = 100,
            .center = 100,
            .right = 100,
            .down_left = 100,
            .down = 100,
            .down_right = 100,
        },

    .debounce_delay_ms = 25,

    .adc_channels =
        {
            .up_left = 0,
            .up = 1,
            .up_right = 2,
            .left = 3,
            .center = 4,
            .right = 5,
            .down_left = 6,
            .down = 7,
            .down_right = 8,
        },

    .adc_config =
        Peripherals::Pad<pad_config.PANEL_COUNT>::Config::ExternalAdc<3>{
            .spi =
                {
                    .mosi_pin = 11,
                    .miso_pin = 12,
                    .sclk_pin = 10,
                    .block = spi1,
                    .speed_hz = 10000000,
                },

            .adcs = {{
                {
                    .scsn_pin = 14,
                    .drdy_pin = 15,
                },
                {
                    .scsn_pin = 7,
                    .drdy_pin = 8,
                },
                {
                    .scsn_pin = 5,
                    .drdy_pin = 6,
                },
            }},
        },
};

const Peripherals::PanelLeds<pad_config.PANEL_COUNT>::Config led_config =
    {
        .idle_colors =
            {
                .up_left = {.r = 80, .g = 80, .b = 0},
                .up = {.r = 0, .g = 80, .b = 0},
                .up_right = {.r = 0, .g = 80, .b = 80},
                .left = {.r = 80, .g = 0, .b = 0},
                .center = {.r = 0, .g = 80, .b = 0},
                .right = {.r = 0, .g = 80, .b = 80},
                .down_left = {.r = 0, .g = 80, .b = 80},
                .down = {.r = 80, .g = 80, .b = 80},
                .down_right = {.r = 0, .g = 80, .b = 80},
            },
        .active_colors =
            {
                .up_left = {.r = 255, .g = 255, .b = 0},
                .up = {.r = 0, .g = 255, .b = 0},
                .up_right = {.r = 0, .g = 255, .b = 255},
                .left = {.r = 255, .g = 0, .b = 0},
                .center = {.r = 0, .g = 255, .b = 0},
                .right = {.r = 0, .g = 255, .b = 255},
                .down_left = {.r = 0, .g = 255, .b = 255},
                .down = {.r = 255, .g = 255, .b = 255},
                .down_right = {.r = 0, .g = 255, .b = 255},
            },

        .brightness = 64,
        .animation_speed = 128,
        .idle_mode = Peripherals::PanelLeds<pad_config.PANEL_COUNT>::Config::IdleMode::Static,
        .active_mode = Peripherals::PanelLeds<pad_config.PANEL_COUNT>::Config::ActiveMode::ActiveFade,

        .enable_player_color = true,

        .led_pin = 29,
        .is_rgbw = false,
        .leds_per_panel = 2,
        .panel_order =
            {
                .up_left = 0,
                .up = 1,
                .up_right = 2,
                .left = 3,
                .center = 4,
                .right = 5,
                .down_left = 6,
                .down = 7,
                .down_right = 8,

            },
};

const Peripherals::Controller::Config controller_config = {
    .pins = {.dpad =
                 {
                     .up = 8,
                     .down = 9,
                     .left = 10,
                     .right = 11,
                 },
             .buttons =
                 {
                     .north = 0,
                     .east = 3,
                     .south = 1,
                     .west = 2,

                     .l = 12,
                     .r = 4,

                     .start = 5,
                     .select = 13,
                     .home = 6,
                     .share = 14,
                 }},

    .debounce_delay_ms = 25,

    .gpio_config =
        Peripherals::Controller::Config::ExternalGpio{
            {
                .block = i2c_config.block,
                .address = 0x20,
            },
        },
};

const Peripherals::Display::Config display_config = {
    .i2c_block = i2c_config.block,
    .i2c_address = 0x3C,
};

} // namespace Default
} // namespace Dancecon::Config

#endif // _GLOBALCONFIGURATION_H_