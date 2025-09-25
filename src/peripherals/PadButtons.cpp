#include "peripherals/PadButtons.h"

#include "hardware/gpio.h"
#include "pico/time.h"

namespace Dancecon::Peripherals {

PadButtons::Button::Button(const uint8_t pin) : m_gpio_pin(pin), m_gpio_mask(1 << pin) {}

void PadButtons::Button::setState(const bool state, const uint8_t debounce_delay) {
    if (m_active == state) {
        return;
    }

    // Immediately change the input state, but only allow a change every debounce_delay milliseconds.
    const uint32_t now = to_ms_since_boot(get_absolute_time());
    if (m_last_change + debounce_delay <= now) {
        m_active = state;
        m_last_change = now;
    }
}

PadButtons::PadButtons(const Config &config) : m_config(config) {
    m_buttons.emplace(Id::START, config.pins.start);
    m_buttons.emplace(Id::SELECT, config.pins.select);

    for (const auto &button : m_buttons) {
        gpio_init(button.second.getGpioPin());
        gpio_set_dir(button.second.getGpioPin(), (bool)GPIO_IN);
        gpio_pull_up(button.second.getGpioPin());
    }
}

void PadButtons::updateInputState(Utils::InputState &input_state) {
    const uint32_t gpio_state = ~gpio_get_all();

    for (auto &button : m_buttons) {
        button.second.setState((bool)(gpio_state & button.second.getGpioMask()), m_config.debounce_delay_ms);
    }

    auto &pad = input_state.pad;
    pad.start = m_buttons.at(Id::START).getState();
    pad.select = m_buttons.at(Id::SELECT).getState();
}
} // namespace Dancecon::Peripherals
