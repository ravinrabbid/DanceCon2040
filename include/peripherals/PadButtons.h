#ifndef PERIPHERALS_PADBUTTONS_H_
#define PERIPHERALS_PADBUTTONS_H_

#include "utils/InputState.h"

#include <cstdint>
#include <map>

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
    enum class Id : uint8_t {
        START,
        SELECT,
    };

    class Button {
      private:
        uint8_t m_gpio_pin;
        uint32_t m_gpio_mask;

        uint32_t m_last_change{0};
        bool m_active{false};

      public:
        Button(uint8_t pin);

        [[nodiscard]] uint8_t getGpioPin() const { return m_gpio_pin; };
        [[nodiscard]] uint32_t getGpioMask() const { return m_gpio_mask; };

        [[nodiscard]] bool getState() const { return m_active; };
        void setState(bool state, uint8_t debounce_delay);
    };

    Config m_config;
    std::map<Id, Button> m_buttons;

  public:
    PadButtons(const Config &config);

    void updateInputState(Utils::InputState &input_state);
};

} // namespace Dancecon::Peripherals

#endif // PERIPHERALS_PADBUTTONS_H_