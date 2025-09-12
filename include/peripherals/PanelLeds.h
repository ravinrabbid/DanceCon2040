#ifndef _PERIPHERALS_PANELLEDS_H_
#define _PERIPHERALS_PANELLEDS_H_

#include "utils/InputState.h"

#include <optional>
#include <stdint.h>

namespace Dancecon::Peripherals {

class PanelLeds {
  public:
    struct Config {
        struct Color {
            uint8_t r;
            uint8_t g;
            uint8_t b;
        };

        Color idle_color;
        Color up_color;
        Color down_color;
        Color left_color;
        Color right_color;
        // TODO missing directions

        // TODO LEDs per pad
        // TODO LED index

        uint8_t led_pin;
        bool is_rgbw;

        uint8_t brightness;
        bool enable_player_color;
    };

  private:
    Config m_config;

    Utils::InputState m_input_state;
    std::optional<Config::Color> m_player_color;

  public:
    PanelLeds(const Config &config);

    void setBrightness(const uint8_t brightness);
    void setEnablePlayerColor(const bool do_enable);

    void setInputState(const Utils::InputState &input_state);
    void setPlayerColor(const Config::Color &color);

    void update();
};

} // namespace Dancecon::Peripherals

#endif // _PERIPHERALS_PANELLEDS_H_