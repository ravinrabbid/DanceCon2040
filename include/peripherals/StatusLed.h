#ifndef _PERIPHERALS_STATUS_LED_H_
#define _PERIPHERALS_STATUS_LED_H_

#include "usb/device_driver.h"

#include <optional>
#include <stdint.h>

namespace Dancecon::Peripherals {

class StatusLed {
  public:
    struct Config {
        struct Color {
            uint8_t r;
            uint8_t g;
            uint8_t b;
        };

        uint8_t led_pin;
        uint8_t led_enable_pin;
        bool is_rgbw;

        Color idle_color;
        uint8_t brightness;
    };

  private:
    Config m_config;

  public:
    StatusLed(const Config &config);

    void setPlayer(const usb_player_led_t &player);
};

} // namespace Dancecon::Peripherals

#endif // _PERIPHERALS_STATUS_LED_H_