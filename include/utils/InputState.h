#ifndef UTILS_INPUTSTATE_H_
#define UTILS_INPUTSTATE_H_

#include "usb/device/hid/keyboard_driver.h"
#include "usb/device/hid/ps3_dance_pad_driver.h"
#include "usb/device/hid/ps3_driver.h"
#include "usb/device/hid/ps4_driver.h"
#include "usb/device/hid/spice2x_driver.h"
#include "usb/device/hid/switch_driver.h"
#include "usb/device/vendor/xinput_driver.h"
#include "usb/device_driver.h"

#include <cstdint>
#include <string>

namespace Dancecon::Utils {

struct InputState {
  public:
    struct Pad {
        struct Panel {
            bool triggered;
            uint16_t raw;
        };

        Panel up_left, up, up_right, left, center, right, down_left, down, down_right;
        bool start, select;
    };

    struct Controller {
        struct DPad {
            bool up, down, left, right;
        };

        struct Buttons {
            bool north, east, south, west;
            bool l, r;
            bool start, select, home, share;
        };

        DPad dpad;
        Buttons buttons;
    };

  private:
    enum class Player : uint8_t {
        One,
        Two,
    };

    Pad m_pad{};
    Controller m_controller{};

    hid_switch_report_t m_switch_report{};
    hid_ps3_dance_pad_report_t m_ps3_dance_pad_report{};
    hid_ps3_report_t m_ps3_report{};
    hid_ps4_report_t m_ps4_report{};
    hid_nkro_keyboard_report_t m_keyboard_report{};
    xinput_report_t m_xinput_report{};
    hid_spice2x_report_t m_spice2x_report{};
    std::string m_debug_report;

    usb_report_t getSwitchReport();
    usb_report_t getPS3DancePadInputReport();
    usb_report_t getPS3InputReport();
    usb_report_t getPS4InputReport();
    usb_report_t getKeyboardReport(Player player);
    usb_report_t getXinputReport();
    usb_report_t getSpice2xReport();
    usb_report_t getDebugReport();

  public:
    InputState() = default;

    [[nodiscard]] Pad &getPad() { return m_pad; };
    [[nodiscard]] const Pad &getPad() const { return m_pad; };
    [[nodiscard]] Controller &getController() { return m_controller; };
    [[nodiscard]] const Controller &getController() const { return m_controller; };

    usb_report_t getReport(usb_mode_t mode);

    void releaseAll();

    [[nodiscard]] bool checkHotkey() const;
};

} // namespace Dancecon::Utils

#endif // UTILS_INPUTSTATE_H_