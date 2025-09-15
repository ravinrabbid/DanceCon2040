#include "peripherals/PadButtons.h"

#include "hardware/gpio.h"
#include "pico/time.h"

namespace Dancecon::Peripherals {

PadButtons::Button::Button(uint8_t pin) : gpio_pin(pin), gpio_mask(1 << pin), last_change(0), active(false) {}

void PadButtons::Button::setState(bool state, uint8_t debounce_delay) {
    if (active == state) {
        return;
    }

    // Immediately change the input state, but only allow a change every debounce_delay milliseconds.
    uint32_t now = to_ms_since_boot(get_absolute_time());
    if (last_change + debounce_delay <= now) {
        active = state;
        last_change = now;
    }
}

PadButtons::PadButtons(const Config &config) : m_config(config) {
    m_buttons.emplace(Id::START, config.pins.start);
    m_buttons.emplace(Id::SELECT, config.pins.select);

    for (const auto &button : m_buttons) {
        gpio_init(button.second.getGpioPin());
        gpio_set_dir(button.second.getGpioPin(), GPIO_IN);
        gpio_pull_up(button.second.getGpioPin());
    }
}

void PadButtons::updateInputState(Utils::InputState &input_state) {
    uint32_t gpio_state = ~gpio_get_all();

    for (auto &button : m_buttons) {
        button.second.setState(gpio_state & button.second.getGpioMask(), m_config.debounce_delay_ms);
    }

    input_state.pad.start = m_buttons.at(Id::START).getState();
    input_state.pad.select = m_buttons.at(Id::SELECT).getState();
}
} // namespace Dancecon::Peripherals
