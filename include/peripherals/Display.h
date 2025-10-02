#ifndef PERIPHERALS_DISPLAY_H_
#define PERIPHERALS_DISPLAY_H_

#include "usb/device_driver.h"
#include "utils/InputState.h"
#include "utils/Menu.h"

#include "hardware/i2c.h"
#include <ssd1306/ssd1306.h>

#include <cstdint>
#include <list>
#include <memory>

namespace Dancecon::Peripherals {

class Display {
  public:
    struct Config {
        i2c_inst_t *i2c_block;
        uint8_t i2c_address;
    };

  private:
    enum class State : uint8_t {
        Idle,
        Menu,
    };

    class BpmCounter {
      private:
        class Buffer {
          private:
            std::list<uint16_t> m_buffer;
            size_t m_size_limit;

          public:
            Buffer(size_t size_limit) : m_size_limit(size_limit) {};

            void insert(uint16_t value);
            void clear() { m_buffer.clear(); };

            [[nodiscard]] uint16_t getAvarage() const;
        };

        static constexpr size_t WINDOW_SIZE = 20;
        static constexpr uint32_t DOUBLE_HIT_TIMEOUT_MS = 50;

        uint32_t m_timeout_ms;
        Buffer m_buffer{WINDOW_SIZE};

        Utils::InputState::Pad m_prev_pad{};
        uint32_t m_prev_press_time{0};

        uint16_t m_current_bpm{0};

      public:
        BpmCounter(uint32_t timeout_ms) : m_timeout_ms(timeout_ms) {};
        void update(const Utils::InputState::Pad &pad);

        [[nodiscard]] uint16_t getBpm() const { return m_current_bpm; };
    };

    Config m_config;
    State m_state{State::Idle};

    usb_mode_t m_usb_mode{USB_MODE_DEBUG};
    uint8_t m_player_id{0};

    Utils::InputState m_input_state{};
    Utils::MenuState m_menu_state{};

    ssd1306_t m_display{};
    uint32_t m_next_frame_time{0};

    BpmCounter m_bpm_counter{2000};

    void drawIdleScreen();
    void drawMenuScreen();

  public:
    Display(const Config &config);

    void setInputState(const Utils::InputState &state);
    void setUsbMode(usb_mode_t mode);
    void setPlayerId(uint8_t player_id);

    void setMenuState(const Utils::MenuState &menu_state);

    void showIdle();
    void showMenu();

    void update();
};

} // namespace Dancecon::Peripherals

#endif // PERIPHERALS_DISPLAY_H_