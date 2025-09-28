#ifndef PERIPHERALS_PANELLEDS_H_
#define PERIPHERALS_PANELLEDS_H_

#include "usb/device_driver.h"
#include "utils/InputState.h"

#include <array>
#include <cstdint>
#include <optional>
#include <variant>
#include <vector>

namespace Dancecon::Peripherals {

template <size_t TPanelCount> class PanelLeds {
  private:
    static constexpr size_t SEGMENT_COUNT = 32;

    static constexpr uint32_t PULSE_STEPS = 4096;
    static constexpr uint32_t RAINBOW_STEPS = 4096;
    static constexpr uint32_t FADE_STEPS = 2048;
    static constexpr uint32_t BLEND_STEPS = 128;

    static constexpr uint8_t PULSE_DIM_PCT_MIN = 40;
    static constexpr uint8_t PULSE_DIM_PCT_MAX = 100;

    template <size_t TConfigPanelCount>
        requires(TConfigPanelCount == 4 || TConfigPanelCount == 5 || TConfigPanelCount == 6 || TConfigPanelCount == 8 ||
                 TConfigPanelCount == 9)
    struct TConfig {
        static constexpr auto PANEL_COUNT = TPanelCount;

        enum class IdleMode : uint8_t {
            Off,
            Static,
            Pulse,
            Rainbow,
        };

        enum class ActiveMode : uint8_t {
            Off,
            Idle,
            Active,
            ActiveFade,
            ActiveIdle,
        };

        struct Color {
            uint8_t r;
            uint8_t g;
            uint8_t b;

            bool operator==(const Color &) const = default;
        };

        struct ColorsFourPanel {
            Color up;
            Color left;
            Color right;
            Color down;

            bool operator==(const ColorsFourPanel &) const = default;
        };
        struct ColorsFivePanel {
            Color up_left;
            Color up_right;
            Color center;
            Color down_left;
            Color down_right;

            bool operator==(const ColorsFivePanel &) const = default;
        };
        struct ColorsSixPanel {
            Color up_left;
            Color up;
            Color up_right;
            Color left;
            Color right;
            Color down;

            bool operator==(const ColorsSixPanel &) const = default;
        };
        struct ColorsEightPanel {
            Color up_left;
            Color up;
            Color up_right;
            Color left;
            Color right;
            Color down_left;
            Color down;
            Color down_right;

            bool operator==(const ColorsEightPanel &) const = default;
        };
        struct ColorsNinePanel {
            Color up_left;
            Color up;
            Color up_right;
            Color left;
            Color center;
            Color right;
            Color down_left;
            Color down;
            Color down_right;

            bool operator==(const ColorsNinePanel &) const = default;
        };

        using PanelColors =                              //
            std::conditional_t<                          //
                TConfigPanelCount == 4,                  //
                ColorsFourPanel,                         //
                typename std::conditional_t<             //
                    TConfigPanelCount == 5,              //
                    ColorsFivePanel,                     //
                    typename std::conditional_t<         //
                        TConfigPanelCount == 6,          //
                        ColorsSixPanel,                  //
                        typename std::conditional_t<     //
                            TConfigPanelCount == 8,      //
                            ColorsEightPanel,            //
                            typename std::conditional_t< //
                                TConfigPanelCount == 9,  //
                                ColorsNinePanel,         //
                                std::monostate>>>>>;

        struct OrderFourPanel {
            uint8_t up;
            uint8_t left;
            uint8_t right;
            uint8_t down;
        };
        struct OrderFivePanel {
            uint8_t up_left;
            uint8_t up_right;
            uint8_t center;
            uint8_t down_left;
            uint8_t down_right;
        };
        struct OrderSixPanel {
            uint8_t up_left;
            uint8_t up;
            uint8_t up_right;
            uint8_t left;
            uint8_t right;
            uint8_t down;
        };
        struct OrderEightPanel {
            uint8_t up_left;
            uint8_t up;
            uint8_t up_right;
            uint8_t left;
            uint8_t right;
            uint8_t down_left;
            uint8_t down;
            uint8_t down_right;
        };
        struct OrderNinePanel {
            uint8_t up_left;
            uint8_t up;
            uint8_t up_right;
            uint8_t left;
            uint8_t center;
            uint8_t right;
            uint8_t down_left;
            uint8_t down;
            uint8_t down_right;
        };

        using PanelOrder =                               //
            std::conditional_t<                          //
                TConfigPanelCount == 4,                  //
                OrderFourPanel,                          //
                typename std::conditional_t<             //
                    TConfigPanelCount == 5,              //
                    OrderFivePanel,                      //
                    typename std::conditional_t<         //
                        TConfigPanelCount == 6,          //
                        OrderSixPanel,                   //
                        typename std::conditional_t<     //
                            TConfigPanelCount == 8,      //
                            OrderEightPanel,             //
                            typename std::conditional_t< //
                                TConfigPanelCount == 9,  //
                                OrderNinePanel,          //
                                std::monostate>>>>>;

        PanelColors idle_colors;
        PanelColors active_colors;

        uint8_t brightness;
        uint8_t animation_speed;
        IdleMode idle_mode;
        ActiveMode active_mode;

        bool enable_player_color;
        bool enable_hid_lights;

        uint8_t led_pin;
        bool is_rgbw;
        uint16_t leds_per_panel;
        PanelOrder panel_order;
    };

  public:
    using Config = TConfig<TPanelCount>;

  private:
    class AnimationStepper {
      private:
        uint32_t m_steps_until_advance;
        uint32_t m_current_steps{0};

      public:
        AnimationStepper(uint32_t steps_until_advance) : m_steps_until_advance(steps_until_advance) {};
        uint32_t getFrameCount(uint32_t steps);
    };

    Config m_config;
    Utils::InputState::Pad m_pad_state{};

    struct {
        AnimationStepper stepper{PULSE_STEPS};
        uint8_t dim_percent{PULSE_DIM_PCT_MAX};
        int8_t advance_factor{-1};
    } m_pulse_state;

    struct {
        AnimationStepper stepper{RAINBOW_STEPS};
        size_t position{0};
    } m_rainbow_state;

    struct {
        AnimationStepper stepper{FADE_STEPS};
        std::array<uint8_t, SEGMENT_COUNT> percent{};
    } m_fade_state;

    struct {
        AnimationStepper stepper{BLEND_STEPS};
        uint8_t percent{100};
    } m_blend_state;

    std::vector<uint32_t> m_rendered_frame;
    uint32_t m_previous_frame_time{0};

    std::array<typename Config::Color, TPanelCount> m_idle_buffer;
    std::array<typename Config::Color, TPanelCount> m_active_buffer;
    std::array<typename Config::Color, TPanelCount> m_direct_buffer;

    std::optional<typename Config::Color> m_player_color;

    bool m_direct_mode{false};

    void updateIdle(uint32_t steps);
    void updateActive(uint32_t steps);

    void render(uint32_t steps);
    void show();

  public:
    PanelLeds(const Config &config);

    void setBrightness(uint8_t brightness);
    void setAnimationSpeed(uint8_t speed);
    void setIdleMode(Config::IdleMode mode);
    void setActiveMode(Config::ActiveMode mode);
    void setIdleColors(const Config::PanelColors &color);
    void setActiveColors(const Config::PanelColors &colors);
    void setEnablePlayerColor(bool do_enable);
    void setEnableHidLights(bool do_enable);

    void setInputState(const Utils::InputState &input_state);
    void setPlayerColor(const Config::Color &color);

    void update();
    void update(const usb_panel_led_t &raw);
};

} // namespace Dancecon::Peripherals

#endif // PERIPHERALS_PANELLEDS_H_