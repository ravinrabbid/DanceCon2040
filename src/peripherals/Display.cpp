#include "peripherals/Display.h"

#include "GlobalConfiguration.h"
#include "bitmaps/MenuScreens.h"

#include "hardware/gpio.h"
#include "pico/time.h"

#include <list>
#include <numeric>
#include <string>

namespace Dancecon::Peripherals {

Display::Display(const Config &config)
    : m_config(config), m_state(State::Idle), m_pad_state({}), m_usb_mode(USB_MODE_DEBUG), m_player_id(0) {
    m_display.external_vcc = false;
    ssd1306_init(&m_display, 128, 64, m_config.i2c_address, m_config.i2c_block);
    ssd1306_clear(&m_display);
}

void Display::setInputState(const Utils::InputState &state) { m_pad_state = state.pad; }
void Display::setUsbMode(usb_mode_t mode) { m_usb_mode = mode; };
void Display::setPlayerId(uint8_t player_id) { m_player_id = player_id; };

void Display::setMenuState(const Utils::MenuState &menu_state) { m_menu_state = menu_state; }

void Display::showIdle() { m_state = State::Idle; }
void Display::showMenu() { m_state = State::Menu; }

static std::string modeToString(usb_mode_t mode) {
    switch (mode) {
    case USB_MODE_SWITCH_HORIPAD:
        return "Switch Horipad";
    case USB_MODE_DUALSHOCK3:
        return "Dualshock 3";
    case USB_MODE_PS3_DANCE:
        return "PS3 DDR";
    case USB_MODE_DUALSHOCK4:
        return "Dualshock 4";
    case USB_MODE_PS4_COMPAT:
        return "PS4 Compat";
    case USB_MODE_KEYBOARD_P1:
        return "Keyboard P1";
    case USB_MODE_KEYBOARD_P2:
        return "Keyboard P2";
    case USB_MODE_XBOX360_DANCE:
        return "X360 DDR";
    case USB_MODE_XBOX360:
        return "Xbox 360";
    case USB_MODE_SPICE2X:
        return "Spice2x";
    case USB_MODE_DEBUG:
        return "Debug";
    }
    return "?";
}

static uint16_t calculateBpm(const Utils::InputState::Pad &pad) {
    // Somewhat ugly gimmick to calculate the how often the panels
    // are pressed per minute.
    //
    // It records the average time between the last 'window_size' panel
    // presses for calculation. To avoid spikes caused by simultaneous
    // panel presses, all presses within 'double_hit_window' are counted
    // as a single press.
    //
    // Counter resets after 'reset_after'.

    static const size_t window_size = 20;
    static const uint32_t double_hit_window = 50;
    static const uint32_t reset_after = 2000;

    static Utils::InputState::Pad prev_pad = {};
    static uint32_t prev_press = 0;
    static uint16_t current_bpm = 0;

    struct StatBuffer {
      private:
        std::list<uint16_t> m_buf;
        size_t m_max_size;

      public:
        StatBuffer(size_t max_size) : m_max_size(max_size) {}

        void clear() { m_buf.clear(); };

        void insert(uint16_t val) {
            if (m_buf.size() >= m_max_size) {
                m_buf.pop_back();
            }
            m_buf.push_front(val);
        };

        uint16_t avg() {
            if (m_buf.size() > 0) {
                return std::accumulate(m_buf.begin(), m_buf.end(), 0) / m_buf.size();
            }
            return 0;
        }
    };
    static StatBuffer stat_buffer(window_size);

    uint32_t now = to_ms_since_boot(get_absolute_time());
    uint32_t interval = now - prev_press;

    if (interval > reset_after) {
        stat_buffer.clear();
        current_bpm = 0;
        prev_press = 0;
    }

    if ((interval > double_hit_window) &&
        ((pad.up_left.triggered && !prev_pad.up_left.triggered) || (pad.up.triggered && !prev_pad.up.triggered) ||
         (pad.up_right.triggered && !prev_pad.up_right.triggered) || (pad.left.triggered && !prev_pad.left.triggered) ||
         (pad.center.triggered && !prev_pad.center.triggered) || (pad.right.triggered && !prev_pad.right.triggered) ||
         (pad.down_left.triggered && !prev_pad.down_left.triggered) ||
         (pad.down.triggered && !prev_pad.down.triggered) ||
         (pad.down_right.triggered && !prev_pad.down_right.triggered))) {

        if (prev_press != 0) {
            stat_buffer.insert(interval);
            current_bpm = 60000 / stat_buffer.avg();
        }

        prev_press = now;
    }

    prev_pad = pad;

    return current_bpm;
}

void Display::drawIdleScreen() {
    // Header
    std::string mode_string = modeToString(m_usb_mode) + " mode";
    ssd1306_draw_string(&m_display, 0, 0, 1, mode_string.c_str());
    ssd1306_draw_line(&m_display, 0, 10, 128, 10);

    // BPM
    auto bpm_str = std::to_string(calculateBpm(m_pad_state)) + " bpm";
    ssd1306_draw_string(&m_display, (127 - (bpm_str.length() * 12)) / 2, 20, 2, bpm_str.c_str());

    // Player "LEDs"
    if (m_player_id != 0) {
        for (uint8_t i = 0; i < 4; ++i) {
            if (m_player_id & (1 << i)) {
                ssd1306_draw_square(&m_display, ((127) - ((4 - i) * 6)) - 1, 2, 4, 4);
            } else {
                ssd1306_draw_square(&m_display, (127) - ((4 - i) * 6), 3, 2, 2);
            }
        }
    }

    // Menu hint
    ssd1306_draw_line(&m_display, 0, 54, 128, 54);
    ssd1306_draw_string(&m_display, 0, 56, 1, "Hold STA+SEL for Menu");
}

void Display::drawMenuScreen() {
    auto descriptor_it =
        Utils::Menu<Dancecon::Config::Default::pad_config.PANEL_COUNT>::descriptors.find(m_menu_state.page);
    if (descriptor_it == Utils::Menu<Dancecon::Config::Default::pad_config.PANEL_COUNT>::descriptors.end()) {
        return;
    }

    // Background
    switch (descriptor_it->second.type) {
    case Utils::MenuDescriptor::Type::Menu:
        if (m_menu_state.page == Utils::MenuPage::Main) {
            ssd1306_bmp_show_image(&m_display, menu_screen_top.data(), menu_screen_top.size());
        } else {
            ssd1306_bmp_show_image(&m_display, menu_screen_sub.data(), menu_screen_sub.size());
        }
        break;
    case Utils::MenuDescriptor::Type::Value:
        ssd1306_bmp_show_image(&m_display, menu_screen_value.data(), menu_screen_value.size());
        break;
    case Utils::MenuDescriptor::Type::Selection:
    case Utils::MenuDescriptor::Type::Toggle:
        ssd1306_bmp_show_image(&m_display, menu_screen_sub.data(), menu_screen_sub.size());
        break;
    case Utils::MenuDescriptor::Type::RebootInfo:
        break;
    }

    // Heading
    ssd1306_draw_string(&m_display, 0, 0, 1, descriptor_it->second.name.c_str());

    // Current Selection
    std::string selection;
    switch (descriptor_it->second.type) {
    case Utils::MenuDescriptor::Type::Menu:
    case Utils::MenuDescriptor::Type::Selection:
    case Utils::MenuDescriptor::Type::RebootInfo:
        selection = descriptor_it->second.items.at(m_menu_state.selected_value).first;
        break;
    case Utils::MenuDescriptor::Type::Value:
        selection = std::to_string(m_menu_state.selected_value);
        break;
    case Utils::MenuDescriptor::Type::Toggle:
        selection = m_menu_state.selected_value ? "On" : "Off";
        break;
    }
    ssd1306_draw_string(&m_display, (127 - (selection.length() * 12)) / 2, 15, 2, selection.c_str());

    // Breadcrumbs
    switch (descriptor_it->second.type) {
    case Utils::MenuDescriptor::Type::Menu:
    case Utils::MenuDescriptor::Type::Selection: {
        auto selection_count = descriptor_it->second.items.size();
        for (uint8_t i = 0; i < selection_count; ++i) {
            if (i == m_menu_state.selected_value) {
                ssd1306_draw_square(&m_display, ((127) - ((selection_count - i) * 6)) - 1, 2, 4, 4);
            } else {
                ssd1306_draw_square(&m_display, (127) - ((selection_count - i) * 6), 3, 2, 2);
            }
        }
    } break;
    case Utils::MenuDescriptor::Type::RebootInfo:
        break;
    case Utils::MenuDescriptor::Type::Value:
    case Utils::MenuDescriptor::Type::Toggle:
        break;
    }
}

void Display::update() {
    static const uint32_t interval_ms = 17; // Limit to ~60fps
    static uint32_t start_ms = 0;

    if (to_ms_since_boot(get_absolute_time()) - start_ms < interval_ms) {
        return;
    }
    start_ms += interval_ms;

    ssd1306_clear(&m_display);

    switch (m_state) {
    case State::Idle:
        drawIdleScreen();
        break;
    case State::Menu:
        drawMenuScreen();
        break;
    }

    ssd1306_show(&m_display);
};

} // namespace Dancecon::Peripherals