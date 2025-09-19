#include "peripherals/PanelLeds.h"

#include "GlobalConfiguration.h"

#include "hardware/pio.h"
#include "pico/rand.h"
#include "pio_ws2812/ws2812.h"

#include <algorithm>

namespace Dancecon::Peripherals {

template class PanelLeds<Config::Default::pad_config.PANEL_COUNT>;

namespace {
const static uint32_t pulse_step_count = 4096;
static const uint8_t pulse_dim_percent_min = 40;
static const uint8_t pulse_dim_percent_max = 100;

const static uint32_t rainbow_step_count = 4096;

const static uint32_t fade_step_count = 2048;

const static uint32_t blend_step_count = 128;

const static size_t rainbow_length = 40;

// Use alternating frames to allow for smoother animation
template <size_t TPanelCount>
const static std::array<std::array<typename PanelLeds<TPanelCount>::Config::Color, rainbow_length>, 2> rainbow_colors{{
    {{
        {0x5a, 0x3a, 0xc6}, {0x76, 0x36, 0xaa}, {0x91, 0x34, 0x8e}, {0xad, 0x30, 0x72}, {0xca, 0x2e, 0x56},
        {0xe6, 0x2a, 0x3a}, {0xf2, 0x2f, 0x2b}, {0xe6, 0x42, 0x33}, {0xce, 0x5c, 0x46}, {0xb6, 0x74, 0x59},
        {0x9e, 0x8d, 0x6c}, {0x86, 0xa6, 0x7e}, {0x6e, 0xbf, 0x90}, {0x57, 0xd8, 0xa3}, {0x4c, 0xea, 0xac},
        {0x58, 0xf0, 0xa3}, {0x6d, 0xf0, 0x92}, {0x83, 0xf0, 0x82}, {0x99, 0xf0, 0x72}, {0xae, 0xf0, 0x61},
        {0xc4, 0xf0, 0x50}, {0xdb, 0xf0, 0x40}, {0xec, 0xea, 0x34}, {0xf2, 0xdc, 0x34}, {0xf4, 0xc9, 0x38},
        {0xf6, 0xb6, 0x3c}, {0xf8, 0xa2, 0x40}, {0xfa, 0x90, 0x45}, {0xfc, 0x7d, 0x49}, {0xfe, 0x6a, 0x4d},
        {0xf8, 0x5c, 0x56}, {0xe2, 0x56, 0x68}, {0xc6, 0x52, 0x7d}, {0xaa, 0x50, 0x93}, {0x8e, 0x4c, 0xa9},
        {0x72, 0x4a, 0xbf}, {0x56, 0x46, 0xd5}, {0x3a, 0x44, 0xeb}, {0x2e, 0x40, 0xf3}, {0x3e, 0x3c, 0xe2},
    }},
    {{
        {0x68, 0x38, 0xb7}, {0x83, 0x35, 0x9c}, {0x9f, 0x32, 0x80}, {0xbb, 0x2f, 0x64}, {0xd8, 0x2c, 0x48},
        {0xf3, 0x29, 0x2c}, {0xf2, 0x35, 0x2a}, {0xda, 0x4f, 0x3c}, {0xc3, 0x68, 0x50}, {0xaa, 0x81, 0x62},
        {0x92, 0x99, 0x75}, {0x7a, 0xb2, 0x87}, {0x63, 0xcc, 0x9a}, {0x4b, 0xe5, 0xac}, {0x4d, 0xf0, 0xab},
        {0x62, 0xf0, 0x9b}, {0x78, 0xf0, 0x8a}, {0x8e, 0xf0, 0x7a}, {0xa4, 0xf0, 0x69}, {0xb9, 0xf0, 0x59},
        {0xd0, 0xf0, 0x48}, {0xe6, 0xf0, 0x37}, {0xf1, 0xe5, 0x32}, {0xf3, 0xd2, 0x36}, {0xf5, 0xc0, 0x3a},
        {0xf7, 0xac, 0x3e}, {0xf9, 0x99, 0x43}, {0xfb, 0x86, 0x47}, {0xfd, 0x74, 0x4b}, {0xff, 0x61, 0x4f},
        {0xf0, 0x57, 0x5d}, {0xd4, 0x54, 0x72}, {0xb7, 0x51, 0x88}, {0x9c, 0x4e, 0x9e}, {0x80, 0x4b, 0xb4},
        {0x64, 0x48, 0xca}, {0x48, 0x45, 0xe0}, {0x2c, 0x42, 0xf6}, {0x2f, 0x3e, 0xf0}, {0x4c, 0x3b, 0xd4},
    }},
}};

struct AnimationStepper {
    const uint32_t steps_to_advance;
    uint32_t current_steps;

    uint32_t advance(uint32_t steps) {
        current_steps += steps;
        if (current_steps < steps_to_advance) {
            return 0;
        }

        const auto advance = current_steps / steps_to_advance;

        current_steps %= steps_to_advance;

        return advance;
    }
};

template <size_t TPanelCount>
PanelLeds<TPanelCount>::Config::Color dim_color(const typename PanelLeds<TPanelCount>::Config::Color &color,
                                                const float dim_factor) {
    return {
        .r = (uint8_t)((float)color.r * dim_factor),
        .g = (uint8_t)((float)color.g * dim_factor),
        .b = (uint8_t)((float)color.b * dim_factor),
    };
}

template <size_t TPanelCount>
PanelLeds<TPanelCount>::Config::Color max_color(const typename PanelLeds<TPanelCount>::Config::Color &a,
                                                const typename PanelLeds<TPanelCount>::Config::Color &b) {
    return {
        .r = std::max(a.r, b.r),
        .g = std::max(a.g, b.g),
        .b = std::max(a.b, b.b),
    };
}

bool is_pad_triggered(const Utils::InputState &input_state) {
    return input_state.pad.up_left.triggered || input_state.pad.up.triggered || input_state.pad.up_right.triggered ||
           input_state.pad.left.triggered || input_state.pad.center.triggered || input_state.pad.right.triggered ||
           input_state.pad.down_left.triggered || input_state.pad.down.triggered || input_state.pad.down_left.triggered;
}

template <size_t TPanelCount>
void fill_colors(std::array<typename PanelLeds<TPanelCount>::Config::Color, TPanelCount> &buffer,
                 const typename PanelLeds<TPanelCount>::Config::PanelColors &colors,
                 const typename PanelLeds<TPanelCount>::Config::PanelOrder &order, const float dim_factor = 1.) {

    using T = std::decay_t<decltype(colors)>;

    if constexpr (std::is_same_v<T, typename PanelLeds<TPanelCount>::Config::ColorsFourPanel>) {
        buffer[order.up] = dim_color<TPanelCount>(colors.up, dim_factor);
        buffer[order.left] = dim_color<TPanelCount>(colors.left, dim_factor);
        buffer[order.right] = dim_color<TPanelCount>(colors.right, dim_factor);
        buffer[order.down] = dim_color<TPanelCount>(colors.down, dim_factor);
    } else if constexpr (std::is_same_v<T, typename PanelLeds<TPanelCount>::Config::ColorsFivePanel>) {
        buffer[order.up_left] = dim_color<TPanelCount>(colors.up_left, dim_factor);
        buffer[order.up_right] = dim_color<TPanelCount>(colors.up_right, dim_factor);
        buffer[order.center] = dim_color<TPanelCount>(colors.center, dim_factor);
        buffer[order.down_left] = dim_color<TPanelCount>(colors.down_left, dim_factor);
        buffer[order.down_right] = dim_color<TPanelCount>(colors.down_right, dim_factor);
    } else if constexpr (std::is_same_v<T, typename PanelLeds<TPanelCount>::Config::ColorsSixPanel>) {
        buffer[order.up_left] = dim_color<TPanelCount>(colors.up_left, dim_factor);
        buffer[order.up] = dim_color<TPanelCount>(colors.up, dim_factor);
        buffer[order.up_right] = dim_color<TPanelCount>(colors.up_right, dim_factor);
        buffer[order.left] = dim_color<TPanelCount>(colors.left, dim_factor);
        buffer[order.right] = dim_color<TPanelCount>(colors.right, dim_factor);
        buffer[order.down] = dim_color<TPanelCount>(colors.down, dim_factor);
    } else if constexpr (std::is_same_v<T, typename PanelLeds<TPanelCount>::Config::ColorsEightPanel>) {
        buffer[order.up_left] = dim_color<TPanelCount>(colors.up_left, dim_factor);
        buffer[order.up] = dim_color<TPanelCount>(colors.up, dim_factor);
        buffer[order.up_right] = dim_color<TPanelCount>(colors.up_right, dim_factor);
        buffer[order.left] = dim_color<TPanelCount>(colors.left, dim_factor);
        buffer[order.right] = dim_color<TPanelCount>(colors.right, dim_factor);
        buffer[order.down_left] = dim_color<TPanelCount>(colors.down_left, dim_factor);
        buffer[order.down] = dim_color<TPanelCount>(colors.down, dim_factor);
        buffer[order.down_right] = dim_color<TPanelCount>(colors.down_right, dim_factor);
    } else if constexpr (std::is_same_v<T, typename PanelLeds<TPanelCount>::Config::ColorsNinePanel>) {
        buffer[order.up_left] = dim_color<TPanelCount>(colors.up_left, dim_factor);
        buffer[order.up] = dim_color<TPanelCount>(colors.up, dim_factor);
        buffer[order.up_right] = dim_color<TPanelCount>(colors.up_right, dim_factor);
        buffer[order.left] = dim_color<TPanelCount>(colors.left, dim_factor);
        buffer[order.center] = dim_color<TPanelCount>(colors.center, dim_factor);
        buffer[order.right] = dim_color<TPanelCount>(colors.right, dim_factor);
        buffer[order.down_left] = dim_color<TPanelCount>(colors.down_left, dim_factor);
        buffer[order.down] = dim_color<TPanelCount>(colors.down, dim_factor);
        buffer[order.down_right] = dim_color<TPanelCount>(colors.down_right, dim_factor);
    } else {
        static_assert(false, "Unknown Panel count!");
    }
}

template <size_t TPanelCount>
uint16_t get_active_bitvector(const typename PanelLeds<TPanelCount>::Config::PanelOrder order,
                              const Utils::InputState &input_state) {
    uint16_t result = 0;

    using T = std::decay_t<decltype(order)>;

    if constexpr (std::is_same_v<T, typename PanelLeds<TPanelCount>::Config::OrderFourPanel>) {
        result |= (input_state.pad.up.triggered << order.up);
        result |= (input_state.pad.left.triggered << order.left);
        result |= (input_state.pad.right.triggered << order.right);
        result |= (input_state.pad.down.triggered << order.down);
    } else if constexpr (std::is_same_v<T, typename PanelLeds<TPanelCount>::Config::OrderFivePanel>) {
        result |= (input_state.pad.up_left.triggered << order.up_left);
        result |= (input_state.pad.up_right.triggered << order.up_right);
        result |= (input_state.pad.center.triggered << order.center);
        result |= (input_state.pad.down_left.triggered << order.down_left);
        result |= (input_state.pad.down_right.triggered << order.down_right);
    } else if constexpr (std::is_same_v<T, typename PanelLeds<TPanelCount>::Config::OrderSixPanel>) {
        result |= (input_state.pad.up_left.triggered << order.up_left);
        result |= (input_state.pad.up.triggered << order.up);
        result |= (input_state.pad.up_right.triggered << order.up_right);
        result |= (input_state.pad.left.triggered << order.left);
        result |= (input_state.pad.right.triggered << order.right);
        result |= (input_state.pad.down.triggered << order.down);
    } else if constexpr (std::is_same_v<T, typename PanelLeds<TPanelCount>::Config::OrderEightPanel>) {
        result |= (input_state.pad.up_left.triggered << order.up_left);
        result |= (input_state.pad.up.triggered << order.up);
        result |= (input_state.pad.up_right.triggered << order.up_right);
        result |= (input_state.pad.left.triggered << order.left);
        result |= (input_state.pad.right.triggered << order.right);
        result |= (input_state.pad.down_left.triggered << order.down_left);
        result |= (input_state.pad.down.triggered << order.down);
        result |= (input_state.pad.down_right.triggered << order.down_right);
    } else if constexpr (std::is_same_v<T, typename PanelLeds<TPanelCount>::Config::OrderNinePanel>) {
        result |= (input_state.pad.up_left.triggered << order.up_left);
        result |= (input_state.pad.up.triggered << order.up);
        result |= (input_state.pad.up_right.triggered << order.up_right);
        result |= (input_state.pad.left.triggered << order.left);
        result |= (input_state.pad.center.triggered << order.center);
        result |= (input_state.pad.right.triggered << order.right);
        result |= (input_state.pad.down_left.triggered << order.down_left);
        result |= (input_state.pad.down.triggered << order.down);
        result |= (input_state.pad.down_right.triggered << order.down_right);
    } else {
        static_assert(false, "Unknown Panel count!");
    }

    return result;
}

} // namespace

template <size_t TPanelCount>
PanelLeds<TPanelCount>::PanelLeds(const PanelLeds<TPanelCount>::Config &config)
    : m_config(config), m_input_state({}), m_idle_buffer({}), m_active_buffer({}), m_direct_buffer({}),
      m_player_color(std::nullopt), m_direct_mode(false) {
    m_rendered_frame = std::vector<uint32_t>(TPanelCount * config.leds_per_panel, ws2812_rgb_to_u32pixel(0, 0, 0));

    ws2812_init(pio0, config.led_pin, m_config.is_rgbw);
}

template <size_t TPanelCount> void PanelLeds<TPanelCount>::setBrightness(uint8_t brightness) {
    m_config.brightness = brightness;
}
template <size_t TPanelCount> void PanelLeds<TPanelCount>::setAnimationSpeed(uint8_t speed) {
    m_config.animation_speed = speed;
};
template <size_t TPanelCount> void PanelLeds<TPanelCount>::setIdleMode(Config::IdleMode mode) {
    m_config.idle_mode = mode;
};
template <size_t TPanelCount> void PanelLeds<TPanelCount>::setActiveMode(Config::ActiveMode mode) {
    m_config.active_mode = mode;
};
template <size_t TPanelCount> void PanelLeds<TPanelCount>::setIdleColors(const Config::PanelColors &colors) {
    m_config.idle_colors = colors;
};
template <size_t TPanelCount> void PanelLeds<TPanelCount>::setActiveColors(const Config::PanelColors &colors) {
    m_config.active_colors = colors;
};
template <size_t TPanelCount> void PanelLeds<TPanelCount>::setEnablePlayerColor(bool do_enable) {
    m_config.enable_player_color = do_enable;
};
template <size_t TPanelCount> void PanelLeds<TPanelCount>::setEnableHidLights(bool do_enable) {
    m_config.enable_hid_lights = do_enable;
};

template <size_t TPanelCount> void PanelLeds<TPanelCount>::setInputState(const Utils::InputState &input_state) {
    m_input_state = input_state;
}
template <size_t TPanelCount> void PanelLeds<TPanelCount>::setPlayerColor(const PanelLeds::Config::Color &color) {
    m_player_color = color;
}

template <size_t TPanelCount> void PanelLeds<TPanelCount>::updateIdle(uint32_t steps) {
    // Pulse
    static AnimationStepper pulse_stepper{pulse_step_count, 0};
    static uint8_t pulse_dim_percent = pulse_dim_percent_max;
    static int8_t pulse_advance_factor = -1;

    // Rainbow
    static AnimationStepper rainbow_stepper{rainbow_step_count, 0};
    static size_t rainbow_position = get_rand_32() % rainbow_length;

    if (steps <= 0) {
        return;
    }

    auto fill_idle_color = [&](const float dim_factor = 1.) {
        if (m_config.enable_player_color && m_player_color.has_value()) {
            m_idle_buffer.fill(dim_color<TPanelCount>(m_player_color.value(), dim_factor));
        } else {
            fill_colors(m_idle_buffer, m_config.idle_colors, m_config.panel_order, dim_factor);
        }
    };

    switch (m_config.idle_mode) {
    case Config::IdleMode::Off:
        m_idle_buffer.fill({0x00, 0x00, 0x00});
        break;
    case Config::IdleMode::Static:
        fill_idle_color();
        break;
    case Config::IdleMode::Pulse: {
        const auto advance = pulse_stepper.advance(steps);

        if (pulse_advance_factor < 0 && advance >= (uint8_t)(pulse_dim_percent - pulse_dim_percent_min)) {
            pulse_dim_percent = pulse_dim_percent_min;
            pulse_advance_factor = -pulse_advance_factor;
        } else if (pulse_advance_factor > 0 && advance + pulse_dim_percent >= pulse_dim_percent_max) {
            pulse_dim_percent = pulse_dim_percent_max;
            pulse_advance_factor = -pulse_advance_factor;
        } else {
            pulse_dim_percent = pulse_dim_percent + (pulse_advance_factor * advance);
        }

        fill_idle_color((float)pulse_dim_percent / 100.);
    } break;
    case Config::IdleMode::Rainbow: {
        rainbow_position =
            (rainbow_position + rainbow_stepper.advance(steps)) % (rainbow_length * rainbow_colors<TPanelCount>.size());

        const auto frame = rainbow_position % rainbow_colors<TPanelCount>.size();
        const auto frame_position = rainbow_position / rainbow_colors<TPanelCount>.size();

        for (size_t idx = 0; idx < m_idle_buffer.size(); ++idx) {
            size_t offset = (frame_position + idx) % rainbow_length;
            m_idle_buffer[idx] = rainbow_colors<TPanelCount>[frame][offset];
        }
    } break;
    }
}

template <size_t TPanelCount> void PanelLeds<TPanelCount>::updateActive(uint32_t steps) {
    static AnimationStepper fade_stepper{fade_step_count, 0};
    static std::array<uint8_t, TPanelCount> fade_percent = {};
    static std::array<typename Config::Color, TPanelCount> active_colors = {};

    fill_colors(active_colors, m_config.active_colors, m_config.panel_order);
    const auto active_vector = get_active_bitvector<TPanelCount>(m_config.panel_order, m_input_state);

    switch (m_config.active_mode) {
    case Config::ActiveMode::Off:
        m_active_buffer.fill({0x00, 0x00, 0x00});
        break;
    case Config::ActiveMode::Idle:
        std::copy(m_idle_buffer.cbegin(), m_idle_buffer.cend(), m_active_buffer.begin());
        break;
    case Config::ActiveMode::Active: {
        for (size_t idx = 0; idx < TPanelCount; ++idx) {
            if (active_vector & ((uint16_t)1 << idx)) {
                m_active_buffer[idx] = active_colors[idx];
            } else {
                m_active_buffer[idx] = {0x00, 0x00, 0x00};
            }
        }
    } break;
    case Config::ActiveMode::ActiveFade: {
        const auto advance = fade_stepper.advance(steps);

        for (size_t idx = 0; idx < TPanelCount; ++idx) {
            if (active_vector & ((uint16_t)1 << idx)) {
                m_active_buffer[idx] = active_colors[idx];
                fade_percent[idx] = 100;
            } else {
                m_active_buffer[idx] = dim_color<TPanelCount>(m_active_buffer[idx], (float)fade_percent[idx] / 100.);
                fade_percent[idx] = advance > fade_percent[idx] ? 0 : fade_percent[idx] - advance;
            }
        }
    } break;
    case Config::ActiveMode::ActiveIdle: {
        const auto advance = fade_stepper.advance(steps);

        for (size_t idx = 0; idx < TPanelCount; ++idx) {
            if (active_vector & ((uint16_t)1 << idx)) {
                m_active_buffer[idx] = m_idle_buffer[idx];
                fade_percent[idx] = 100;
            } else {
                m_active_buffer[idx] = dim_color<TPanelCount>(m_active_buffer[idx], (float)fade_percent[idx] / 100.);
                fade_percent[idx] = advance > fade_percent[idx] ? 0 : fade_percent[idx] - advance;
            }
        }
    } break;
    }
}

template <size_t TPanelCount> void PanelLeds<TPanelCount>::render(uint32_t steps) {
    static AnimationStepper blend_stepper{blend_step_count, 0};
    static uint8_t blend_percent = 100;

    const auto blend_advance = blend_stepper.advance(steps);
    if (is_pad_triggered(m_input_state)) {
        blend_percent = blend_advance > blend_percent ? 0 : blend_percent - blend_advance;
    } else {
        blend_percent = blend_advance + blend_percent > 100 ? 100 : blend_percent + blend_advance;
    }

    const float brightness_dim_factor = (float)m_config.brightness / 255.;
    const float blend_dim_factor = (float)blend_percent / 100.;

    size_t idx = 0;
    for (auto &rendered_segment : m_rendered_frame) {
        auto blended_segment = max_color<TPanelCount>(
            dim_color<TPanelCount>(m_idle_buffer[idx / m_config.leds_per_panel], blend_dim_factor),
            m_active_buffer[idx / m_config.leds_per_panel]);

        rendered_segment =
            ws2812_rgb_to_u32pixel(blended_segment.r * brightness_dim_factor, blended_segment.g * brightness_dim_factor,
                                   blended_segment.b * brightness_dim_factor);

        ++idx;
    }
}

template <size_t TPanelCount> void PanelLeds<TPanelCount>::show() {
    ws2812_put_frame(pio0, m_rendered_frame.data(), m_rendered_frame.size());
}

template <size_t TPanelCount> void PanelLeds<TPanelCount>::update() {
    if (m_direct_mode && m_config.enable_hid_lights) {
        return;
    }

    static uint32_t previous_frame_time = to_ms_since_boot(get_absolute_time());

    const uint32_t now = to_ms_since_boot(get_absolute_time());
    const uint32_t elapsed = now - previous_frame_time;
    const uint32_t steps = elapsed * m_config.animation_speed;

    previous_frame_time = now;

    updateIdle(steps);
    updateActive(steps);

    render(steps);
    show();
}

template <size_t TPanelCount> void PanelLeds<TPanelCount>::update(const usb_panel_led_t &raw) {
    if (!m_config.enable_hid_lights) {
        return;
    }

    m_direct_mode = true;

    auto dim_factor = [](const auto value) { return (float)value / 255.; };

    const auto &order = m_config.panel_order;
    const auto &colors = m_config.active_colors;

    using T = std::decay_t<decltype(m_config.panel_order)>;

    if constexpr (std::is_same_v<T, typename PanelLeds<TPanelCount>::Config::OrderFourPanel>) {
        m_direct_buffer[order.up] = dim_color<TPanelCount>(colors.up, dim_factor(raw.up));
        m_direct_buffer[order.left] = dim_color<TPanelCount>(colors.left, dim_factor(raw.left));
        m_direct_buffer[order.right] = dim_color<TPanelCount>(colors.right, dim_factor(raw.right));
        m_direct_buffer[order.down] = dim_color<TPanelCount>(colors.down, dim_factor(raw.down));
    } else if constexpr (std::is_same_v<T, typename PanelLeds<TPanelCount>::Config::OrderFivePanel>) {
        m_direct_buffer[order.up_left] = dim_color<TPanelCount>(colors.up_left, dim_factor(raw.up_left));
        m_direct_buffer[order.up_right] = dim_color<TPanelCount>(colors.up_right, dim_factor(raw.up_right));
        m_direct_buffer[order.center] = dim_color<TPanelCount>(colors.center, dim_factor(raw.center));
        m_direct_buffer[order.down_left] = dim_color<TPanelCount>(colors.down_left, dim_factor(raw.down_left));
        m_direct_buffer[order.down_right] = dim_color<TPanelCount>(colors.down_right, dim_factor(raw.down_right));
    } else if constexpr (std::is_same_v<T, typename PanelLeds<TPanelCount>::Config::OrderSixPanel>) {
        m_direct_buffer[order.up_left] = dim_color<TPanelCount>(colors.up_left, dim_factor(raw.up_left));
        m_direct_buffer[order.up] = dim_color<TPanelCount>(colors.up, dim_factor(raw.up));
        m_direct_buffer[order.up_right] = dim_color<TPanelCount>(colors.up_right, dim_factor(raw.up_right));
        m_direct_buffer[order.left] = dim_color<TPanelCount>(colors.left, dim_factor(raw.left));
        m_direct_buffer[order.right] = dim_color<TPanelCount>(colors.right, dim_factor(raw.right));
        m_direct_buffer[order.down] = dim_color<TPanelCount>(colors.down, dim_factor(raw.down));
    } else if constexpr (std::is_same_v<T, typename PanelLeds<TPanelCount>::Config::OrderEightPanel>) {
        m_direct_buffer[order.up_left] = dim_color<TPanelCount>(colors.up_left, dim_factor(raw.up_left));
        m_direct_buffer[order.up] = dim_color<TPanelCount>(colors.up, dim_factor(raw.up));
        m_direct_buffer[order.up_right] = dim_color<TPanelCount>(colors.up_right, dim_factor(raw.up_right));
        m_direct_buffer[order.left] = dim_color<TPanelCount>(colors.left, dim_factor(raw.left));
        m_direct_buffer[order.right] = dim_color<TPanelCount>(colors.right, dim_factor(raw.right));
        m_direct_buffer[order.down_left] = dim_color<TPanelCount>(colors.down_left, dim_factor(raw.down_left));
        m_direct_buffer[order.down] = dim_color<TPanelCount>(colors.down, dim_factor(raw.down));
        m_direct_buffer[order.down_right] = dim_color<TPanelCount>(colors.down_right, dim_factor(raw.down_right));
    } else if constexpr (std::is_same_v<T, typename PanelLeds<TPanelCount>::Config::OrderNinePanel>) {
        m_direct_buffer[order.up_left] = dim_color<TPanelCount>(colors.up_left, dim_factor(raw.up_left));
        m_direct_buffer[order.up] = dim_color<TPanelCount>(colors.up, dim_factor(raw.up));
        m_direct_buffer[order.up_right] = dim_color<TPanelCount>(colors.up_right, dim_factor(raw.up_right));
        m_direct_buffer[order.left] = dim_color<TPanelCount>(colors.left, dim_factor(raw.left));
        m_direct_buffer[order.center] = dim_color<TPanelCount>(colors.center, dim_factor(raw.center));
        m_direct_buffer[order.right] = dim_color<TPanelCount>(colors.right, dim_factor(raw.right));
        m_direct_buffer[order.down_left] = dim_color<TPanelCount>(colors.down_left, dim_factor(raw.down_left));
        m_direct_buffer[order.down] = dim_color<TPanelCount>(colors.down, dim_factor(raw.down));
        m_direct_buffer[order.down_right] = dim_color<TPanelCount>(colors.down_right, dim_factor(raw.down_right));
    } else {
        static_assert(false, "Unknown Panel count!");
    }

    const float brightness_dim_factor = (float)m_config.brightness / 255.;

    size_t idx = 0;
    for (auto &rendered_segment : m_rendered_frame) {
        const auto color = m_direct_buffer[idx / m_config.leds_per_panel];

        rendered_segment = ws2812_rgb_to_u32pixel(color.r * brightness_dim_factor, color.g * brightness_dim_factor,
                                                  color.b * brightness_dim_factor);

        ++idx;
    }

    show();
}

} // namespace Dancecon::Peripherals