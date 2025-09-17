#ifndef _UTILS_INPUTSTATE_H_
#define _UTILS_INPUTSTATE_H_

#include "usb/device/hid/keyboard_driver.h"
#include "usb/device/hid/ps3_driver.h"
#include "usb/device/hid/ps4_driver.h"
#include "usb/device/hid/switch_driver.h"
#include "usb/device/vendor/xinput_driver.h"
#include "usb/device_driver.h"

#include <stdint.h>
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

  public:
    Pad pad;
    Controller controller;

  private:
    enum class Player {
        One,
        Two,
    };

  private:
    hid_switch_report_t m_switch_report;
    hid_ps3_report_t m_ps3_report;
    hid_ps4_report_t m_ps4_report;
    hid_nkro_keyboard_report_t m_keyboard_report;
    xinput_report_t m_xinput_report;
    std::string m_debug_report;

    usb_report_t getSwitchReport();
    usb_report_t getPS3InputReport();
    usb_report_t getPS4InputReport();
    usb_report_t getKeyboardReport(Player player);
    usb_report_t getXinputReport();
    usb_report_t getDebugReport();

  public:
    InputState();

    usb_report_t getReport(usb_mode_t mode);

    void releaseAll();

    bool checkHotkey();
};

} // namespace Dancecon::Utils

#endif // _UTILS_INPUTSTATE_H_