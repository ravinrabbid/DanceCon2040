#include "peripherals/Display.h"

#include "GlobalConfiguration.h"
#include "bitmaps/MenuScreens.h"

#include "pico/time.h"

#include <list>
#include <numeric>
#include <string>

namespace Dancecon::Utils {
template <>
const std::map<MenuPage, const MenuDescriptor> Menu<Dancecon::Config::Default::pad_config.PANEL_COUNT,
                                                    Dancecon::Config::Default::led_config.PANEL_COUNT>::descriptors;
} // namespace Dancecon::Utils

namespace Dancecon::Peripherals {

namespace {

std::string modeToString(const usb_mode_t mode) {
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

} // namespace

void Display::BpmCounter::Buffer::insert(uint16_t value) {
    if (m_buffer.size() >= m_size_limit) {
        m_buffer.pop_back();
    }
    m_buffer.push_front(value);
}

uint16_t Display::BpmCounter::Buffer::getAvarage() const {
    if (!m_buffer.empty()) {
        return std::accumulate(m_buffer.begin(), m_buffer.end(), 0) / m_buffer.size();
    }
    return 0;
}

void Display::BpmCounter::update(const Utils::InputState::Pad &pad) {
    // Somewhat ugly gimmick to calculate the how often the panels
    // are pressed per minute.
    //
    // It records the average time between the last 'WINDOW_SIZE' panel
    // presses for calculation. To avoid spikes caused by simultaneous
    // panel presses, all presses within 'DOUBLE_HIT_TIMEOUT_MS' are counted
    // as a single press.
    //
    // Counter resets after 'm_timeout_ms'.
    const uint32_t now = to_ms_since_boot(get_absolute_time());
    const uint32_t interval = now - m_prev_press_time;

    if (interval > m_timeout_ms) {
        m_buffer.clear();
        m_current_bpm = 0;
        m_prev_press_time = 0;
    }

    if ((interval > DOUBLE_HIT_TIMEOUT_MS) &&
        ((pad.up_left.triggered && !m_prev_pad.up_left.triggered) || (pad.up.triggered && !m_prev_pad.up.triggered) ||
         (pad.up_right.triggered && !m_prev_pad.up_right.triggered) ||
         (pad.left.triggered && !m_prev_pad.left.triggered) || (pad.center.triggered && !m_prev_pad.center.triggered) ||
         (pad.right.triggered && !m_prev_pad.right.triggered) ||
         (pad.down_left.triggered && !m_prev_pad.down_left.triggered) ||
         (pad.down.triggered && !m_prev_pad.down.triggered) ||
         (pad.down_right.triggered && !m_prev_pad.down_right.triggered))) {

        if (m_prev_press_time != 0) {
            m_buffer.insert(interval);
            m_current_bpm = 60000 / m_buffer.getAvarage();
        }

        m_prev_press_time = now;
    }

    m_prev_pad = pad;
}

Display::Display(const Config &config) : m_config(config) {
    m_display.external_vcc = false;
    ssd1306_init(&m_display, 128, 64, m_config.i2c_address, m_config.i2c_block);
    ssd1306_clear(&m_display);
}

void Display::setInputState(const Utils::InputState &state) {
    m_input_state = state;
    m_bpm_counter.update(state.pad);
}
void Display::setUsbMode(const usb_mode_t mode) { m_usb_mode = mode; };
void Display::setPlayerId(const uint8_t player_id) { m_player_id = player_id; };

void Display::setMenuState(const Utils::MenuState &menu_state) { m_menu_state = menu_state; }

void Display::showIdle() { m_state = State::Idle; }
void Display::showMenu() { m_state = State::Menu; }

void Display::drawIdleScreen() {
    // Header
    const auto mode_string = modeToString(m_usb_mode) + " mode";
    ssd1306_draw_string(&m_display, 0, 0, 1, mode_string.c_str());
    ssd1306_draw_line(&m_display, 0, 10, 128, 10);

    // SPM
    const auto bpm_str = std::to_string(m_bpm_counter.getBpm());
    const auto bpm_label_str = std::string("steps/m");
    ssd1306_draw_string(&m_display, 24 - ((bpm_str.length() * 12) / 2), 20, 2, bpm_str.c_str());
    ssd1306_draw_string(&m_display, 24 - ((bpm_label_str.length() * 6) / 2), 38, 1, bpm_label_str.c_str());


    // Pad
    static const uint8_t panel_dim = 10;
    const auto draw_pad = [&](const auto pos_x, const auto pos_y, const auto triggered) {
        if (triggered) {
            ssd1306_draw_square(&m_display, pos_x - (panel_dim / 2), pos_y - (panel_dim / 2), panel_dim, panel_dim);
        } else {
            ssd1306_draw_empty_square(&m_display, pos_x - (panel_dim / 2), pos_y - (panel_dim / 2), panel_dim - 1,
                                      panel_dim - 1);
        }
    };

    draw_pad((127 / 2) - panel_dim, (64 / 2) - panel_dim, m_input_state.pad.up_left.triggered);
    draw_pad((127 / 2), (64 / 2) - panel_dim, m_input_state.pad.up.triggered);
    draw_pad((127 / 2) + panel_dim, (64 / 2) - panel_dim, m_input_state.pad.up_right.triggered);
    draw_pad((127 / 2) - panel_dim, (64 / 2), m_input_state.pad.left.triggered);
    draw_pad((127 / 2), (64 / 2), m_input_state.pad.center.triggered);
    draw_pad((127 / 2) + panel_dim, (64 / 2), m_input_state.pad.right.triggered);
    draw_pad((127 / 2) - panel_dim, (64 / 2) + panel_dim, m_input_state.pad.down_left.triggered);
    draw_pad((127 / 2), (64 / 2) + panel_dim, m_input_state.pad.down.triggered);
    draw_pad((127 / 2) + panel_dim, (64 / 2) + panel_dim, m_input_state.pad.down_right.triggered);

    // Player "LEDs"
    if (m_player_id != 0) {
        for (uint8_t i = 0; i < 4; ++i) {
            if ((m_player_id & (1 << i)) != 0) {
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
    using menu_t = Utils::Menu<Dancecon::Config::Default::pad_config.PANEL_COUNT,
                               Dancecon::Config::Default::led_config.PANEL_COUNT>;

    auto descriptor_it = menu_t::descriptors.find(m_menu_state.page);
    if (descriptor_it == menu_t::descriptors.end()) {
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
        selection = m_menu_state.selected_value == 0 ? "Off" : "On";
        break;
    }
    ssd1306_draw_string(&m_display, (127 - (selection.length() * 12)) / 2, 15, 2, selection.c_str());

    // Breadcrumbs
    switch (descriptor_it->second.type) {
    case Utils::MenuDescriptor::Type::Menu:
    case Utils::MenuDescriptor::Type::Selection: {
        auto selection_count = descriptor_it->second.items.size();
        for (size_t i = 0; i < selection_count; ++i) {
            if (i == m_menu_state.selected_value) {
                ssd1306_draw_square(&m_display, ((127) - ((selection_count - i) * 6)) - 1, 2, 4, 4);
            } else {
                ssd1306_draw_square(&m_display, (127) - ((selection_count - i) * 6), 3, 2, 2);
            }
        }
    } break;
    case Utils::MenuDescriptor::Type::RebootInfo:
    case Utils::MenuDescriptor::Type::Value:
    case Utils::MenuDescriptor::Type::Toggle:
        break;
    }
}

void Display::update() {
    static const uint32_t interval_ms = 17; // Limit to ~60fps

    if (to_ms_since_boot(get_absolute_time()) - m_next_frame_time < interval_ms) {
        return;
    }
    m_next_frame_time += interval_ms;

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