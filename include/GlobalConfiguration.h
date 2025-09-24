#ifndef GLOBALCONFIGURATION_H_
#define GLOBALCONFIGURATION_H_

#include "peripherals/Controller.h"
#include "peripherals/Display.h"
#include "peripherals/Pad.h"
#include "peripherals/PadButtons.h"
#include "peripherals/PanelLeds.h"
#include "peripherals/StatusLed.h"

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

// Either Pad<4>, Pad<5>, Pad<6>, Pad<8>, Pad<9> according to panel count.
// Adjust .thresholds and .adc_channels accordingly.
const Peripherals::Pad<4>::Config pad_config = {
    .thresholds =
        {
            .up = 50, .left = 50, .right = 50, .down = 50,

            // .up_left = 50,
            // .up_right = 50,
            // .center = 50,
            // .down_left = 50,
            // .down_right = 50,
        },

    .hysteresis = 8,
    .debounce_delay_ms = 10,

    .adc_channels =
        {
            .up = 0, .left = 1, .right = 2, .down = 3,

            // .up_left = 4,
            // .up_right = 5,
            // .center = 6,
            // .down_left = 7,
            // .down_right = 8,
        },

    // Choose either GpioAdc, InternalAdc, ExternalAdc<2> or ExternalAdc<3>:
    //  - GpioAdc needs consecutive pins according to panel count
    //  - InternalAdc supports 4 Panels only
    //  - ExternalAdc<2> supports 4, 5 or 6 panels
    //  - ExternalAdc<3> supports 5, 6, 8 or 9 panels

    // .adc_config =
    //     Peripherals::Pad<pad_config.PANEL_COUNT>::Config::GpioAdc{
    //         .base_pin = 28,
    //     },
    // .adc_config =
    //     Peripherals::Pad<pad_config.PANEL_COUNT>::Config::InternalAdc{
    //         .sample_count = 10,
    //     }
    .adc_config =
        Peripherals::Pad<pad_config.PANEL_COUNT>::Config::ExternalAdc<2>{
            .spi =
                {
                    .mosi_pin = 11,
                    .miso_pin = 12,
                    .sclk_pin = 10,
                    .block = spi1,
                    .speed_hz = 10000000,
                },

            .sample_rate = Ads124S0x::SampleRate::R4000,
            .gain = Ads124S0x::Gain::G128,
            .filter = Ads124S0x::Filter::LowLatency,

            .adcs = {{
                {
                    .scsn_pin = 14,
                    .drdy_pin = 15,
                },
                {
                    .scsn_pin = 5,
                    .drdy_pin = 6,
                },
            }},
        },
    // .adc_config =
    //     Peripherals::Pad<pad_config.PANEL_COUNT>::Config::ExternalAdc<3>{
    //         .spi =
    //             {
    //                 .mosi_pin = 11,
    //                 .miso_pin = 12,
    //                 .sclk_pin = 10,
    //                 .block = spi1,
    //                 .speed_hz = 10000000,
    //             },
    //
    //         .sample_rate = Ads124S0x::SampleRate::R4000,
    //         .gain = Ads124S0x::Gain::G128,
    //         .filter = Ads124S0x::Filter::LowLatency,
    //
    //         .adcs = {{
    //             {
    //                 .scsn_pin = 14,
    //                 .drdy_pin = 15,
    //             },
    //             {
    //                 .scsn_pin = 7,
    //                 .drdy_pin = 8,
    //             },
    //             {
    //                 .scsn_pin = 5,
    //                 .drdy_pin = 6,
    //             },
    //         }},
    //     },
};

// Either PanelLeds<4>, PanelLeds<5>, PanelLeds<6>, PanelLeds<8>, PanelLeds<9> according to panel count.
// Adjust .idle_colors, .active_colors and .panel_order accordingly.
const Peripherals::PanelLeds<4>::Config led_config =
    {
        .idle_colors =
            {
                .up = {.r = 0, .g = 80, .b = 0},
                .left = {.r = 80, .g = 0, .b = 0},
                .right = {.r = 0, .g = 80, .b = 80},
                .down = {.r = 80, .g = 80, .b = 80},

                // .up_left = {.r = 80, .g = 80, .b = 0},
                // .up_right = {.r = 0, .g = 80, .b = 80},
                // .center = {.r = 0, .g = 80, .b = 0},
                // .down_left = {.r = 0, .g = 80, .b = 80},
                // .down_right = {.r = 0, .g = 80, .b = 80},
            },
        .active_colors =
            {
                .up = {.r = 0, .g = 255, .b = 0},
                .left = {.r = 255, .g = 0, .b = 0},
                .right = {.r = 0, .g = 255, .b = 255},
                .down = {.r = 255, .g = 255, .b = 255},

                // .up_left = {.r = 255, .g = 255, .b = 0},
                // .up_right = {.r = 0, .g = 255, .b = 255},
                // .center = {.r = 0, .g = 255, .b = 0},
                // .down_left = {.r = 0, .g = 255, .b = 255},
                // .down_right = {.r = 0, .g = 255, .b = 255},
            },

        .brightness = 64,
        .animation_speed = 128,
        .idle_mode = Peripherals::PanelLeds<led_config.PANEL_COUNT>::Config::IdleMode::Static,
        .active_mode = Peripherals::PanelLeds<led_config.PANEL_COUNT>::Config::ActiveMode::ActiveFade,

        .enable_player_color = true,
        .enable_hid_lights = true,

        .led_pin = 29,
        .is_rgbw = false,
        .leds_per_panel = 3,
        .panel_order =
            {
                .up = 0, .left = 1, .right = 2, .down = 3,

                // .up_left = 4,
                // .up_right = 5,
                // .center = 6,
                // .down_left = 7,
                // .down_right = 8,
            },
};

const Peripherals::PadButtons::Config pad_buttons_config = {
    .pins =
        {
            .start = 0,
            .select = 1,
        },

    .debounce_delay_ms = 10,
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

    // Either ExternalGpio or InternalGpio
    .gpio_config =
        Peripherals::Controller::Config::ExternalGpio{
            .i2c =
                {
                    .block = i2c_config.block,
                    .address = 0x20,
                },
        },
    // .gpio_config = Peripherals::Controller::Config::InternalGpio{},
};

const Peripherals::Display::Config display_config = {
    .i2c_block = i2c_config.block,
    .i2c_address = 0x3C,
};

const Peripherals::StatusLed::Config status_led_config = {
    .led_pin = 16,
    .led_enable_pin = 255,
    .is_rgbw = false,

    .idle_color = {.r = 0, .g = 255, .b = 128},
    .brightness = 128,
};

} // namespace Default
} // namespace Dancecon::Config

#endif // GLOBALCONFIGURATION_H_