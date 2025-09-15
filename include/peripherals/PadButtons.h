#ifndef _PERIPHERALS_PAD_BUTTONS_H_
#define _PERIPHERALS_PAD_BUTTONS_H_

#include "utils/InputState.h"

#include <map>
#include <stdint.h>

namespace Dancecon::Peripherals {

class PadButtons {
  public:
    struct Config {
        struct {
            uint8_t start;
            uint8_t select;
        } pins;

        uint8_t debounce_delay_ms;
    };

  private:
    enum class Id {
        START,
        SELECT,
    };

    class Button {
      private:
        uint8_t gpio_pin;
        uint32_t gpio_mask;

        uint32_t last_change;
        bool active;

      public:
        Button(uint8_t pin);

        uint8_t getGpioPin() const { return gpio_pin; };
        uint32_t getGpioMask() const { return gpio_mask; };

        bool getState() const { return active; };
        void setState(bool state, uint8_t debounce_delay);
    };

    Config m_config;
    std::map<Id, Button> m_buttons;

  public:
    PadButtons(const Config &config);

    void updateInputState(Utils::InputState &input_state);
};

} // namespace Dancecon::Peripherals

#endif // _PERIPHERALS_PAD_BUTTONS_H_