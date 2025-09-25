#include "peripherals/StatusLed.h"

#include "hardware/gpio.h"
#include "pio_ws2812/ws2812.h"

namespace Dancecon::Peripherals {

namespace {
void update_led(const StatusLed::Config::Color &color, uint8_t brightness) {
    const float brightness_factor = (float)brightness / static_cast<float>(UINT8_MAX);

    ws2812_put_pixel(pio1,
                     ws2812_rgb_to_gamma_corrected_u32pixel(static_cast<uint8_t>((float)color.g * brightness_factor),
                                                            static_cast<uint8_t>((float)color.r * brightness_factor),
                                                            static_cast<uint8_t>((float)color.b * brightness_factor)));
}
} // namespace

StatusLed::StatusLed(const Config &config) : m_config(config) {
    gpio_init(m_config.led_enable_pin);
    gpio_set_dir(m_config.led_enable_pin, (bool)GPIO_OUT);
    gpio_put(m_config.led_enable_pin, true);

    ws2812_init(pio1, config.led_pin, m_config.is_rgbw);

    update_led(m_config.idle_color, m_config.brightness);
}

void StatusLed::setPlayer(const usb_player_led_t &player) const {
    switch (player.type) {
    case USB_PLAYER_LED_ID:
        switch (player.id) {
        case 0x01: // Player 1
            update_led({.r = 0, .g = 0, .b = 255}, m_config.brightness);
            break;
        case 0x02: // Player 2
            update_led({.r = 255, .g = 0, .b = 0}, m_config.brightness);
            break;
        case 0x04: // Player 3
            update_led({.r = 255, .g = 0, .b = 255}, m_config.brightness);
            break;
        case 0x08: // Player 4
            update_led({.r = 0, .g = 255, .b = 0}, m_config.brightness);
            break;
        case 0x09: // Player 5
            update_led({.r = 255, .g = 255, .b = 0}, m_config.brightness);
            break;
        case 0x0A: // Player 6
            update_led({.r = 0, .g = 255, .b = 255}, m_config.brightness);
            break;
        case 0x0C: // Player 7
            update_led({.r = 255, .g = 255, .b = 255}, m_config.brightness);
            break;
        default:
            update_led(m_config.idle_color, m_config.brightness);
            break;
        }
        break;
    case USB_PLAYER_LED_COLOR:
        update_led({.r = player.red, .g = player.green, .b = player.blue}, m_config.brightness);
        break;
    }
}

} // namespace Dancecon::Peripherals