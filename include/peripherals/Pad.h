#ifndef _PERIPHERALS_PAD_H_
#define _PERIPHERALS_PAD_H_

#include "utils/InputState.h"

#include <array>
#include <map>
#include <memory>
#include <stdint.h>
#include <variant>

namespace Dancecon::Peripherals {

class Pad {
  public:
    struct Config {
        struct Thresholds {
            uint16_t up;
            uint16_t down;
            uint16_t left;
            uint16_t right;
        };

        Thresholds trigger_thresholds;
        bool invert_raw;

        struct {
            uint8_t start;
            uint8_t select;
        } button_pins;

        uint16_t debounce_delay_ms;
    };

  private:
    enum class Id {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        START,
        SELECT,
    };

    class Input {
      private:
        uint8_t channel;
        uint32_t last_change;
        bool active;

      public:
        Input(const uint8_t channel);

        uint8_t getChannel() const { return channel; };
        bool getState() const { return active; };
        void setState(const bool state, const uint16_t debounce_delay);
    };

    Config m_config;
    std::map<Id, Input> m_inputs;

  private:
    std::map<Id, uint16_t> readPanels();
    std::map<Id, uint16_t> readButtons();

  public:
    Pad(const Config &config);

    void updateInputState(Utils::InputState &input_state);

    void setDebounceDelay(const uint16_t delay);
    void setThresholds(const Config::Thresholds &thresholds);
};

} // namespace Dancecon::Peripherals

#endif // _PERIPHERALS_PAD_H_