#include "utils/InputState.h"

#include <iomanip>
#include <sstream>

namespace Dancecon::Utils {

namespace {

uint8_t getHidHat(const InputState::Controller::DPad dpad) {
    if (dpad.up && dpad.right) {
        return 0x01;
    }
    if (dpad.down && dpad.right) {
        return 0x03;
    }
    if (dpad.down && dpad.left) {
        return 0x05;
    }
    if (dpad.up && dpad.left) {
        return 0x07;
    }
    if (dpad.up) {
        return 0x00;
    }
    if (dpad.right) {
        return 0x02;
    }
    if (dpad.down) {
        return 0x04;
    }
    if (dpad.left) {
        return 0x06;
    }

    return 0x08;
}

} // namespace

usb_report_t InputState::getReport(const usb_mode_t mode) {
    switch (mode) {
    case USB_MODE_SWITCH_HORIPAD:
        return getSwitchReport();
    case USB_MODE_PS3_DANCE:
        return getPS3DancePadInputReport();
    case USB_MODE_DUALSHOCK3:
        return getPS3InputReport();
    case USB_MODE_DUALSHOCK4:
    case USB_MODE_PS4_COMPAT:
        return getPS4InputReport();
    case USB_MODE_KEYBOARD_P1:
        return getKeyboardReport(Player::One);
    case USB_MODE_KEYBOARD_P2:
        return getKeyboardReport(Player::Two);
    case USB_MODE_XBOX360_DANCE:
    case USB_MODE_XBOX360:
        return getXinputReport();
    case USB_MODE_SPICE2X:
        return getSpice2xReport();
    case USB_MODE_DEBUG:
        return getDebugReport();
    }

    return getDebugReport();
}

usb_report_t InputState::getSwitchReport() {
    m_switch_report.buttons = 0                                                                       //
                              | ((m_controller.buttons.west || m_pad.left.triggered) ? (1 << 0) : 0)  // Y
                              | ((m_controller.buttons.south || m_pad.down.triggered) ? (1 << 1) : 0) // B
                              | ((m_controller.buttons.east || m_pad.right.triggered) ? (1 << 2) : 0) // A
                              | ((m_controller.buttons.north || m_pad.up.triggered) ? (1 << 3) : 0)   // X
                              | ((m_controller.buttons.l || m_pad.up_left.triggered) ? (1 << 4) : 0)  // L
                              | ((m_controller.buttons.r || m_pad.up_right.triggered) ? (1 << 5) : 0) // R
                              | (m_pad.down_left.triggered ? (1 << 6) : 0)                            // ZL
                              | (m_pad.down_right.triggered ? (1 << 7) : 0)                           // ZR
                              | ((m_controller.buttons.select || m_pad.select) ? (1 << 8) : 0)        // -
                              | ((m_controller.buttons.start || m_pad.start) ? (1 << 9) : 0)          // +
                              | (m_pad.center.triggered ? (1 << 10) : 0)                              // LS
                              | (false ? (1 << 11) : 0)                                               // RS
                              | (m_controller.buttons.home ? (1 << 12) : 0)                           // Home
                              | (m_controller.buttons.share ? (1 << 13) : 0);                         // Capture

    m_switch_report.hat = getHidHat(m_controller.dpad);

    // Center all sticks
    m_switch_report.lx = 0x80;
    m_switch_report.ly = 0x80;
    m_switch_report.rx = 0x80;
    m_switch_report.ry = 0x80;

    return {reinterpret_cast<uint8_t *>(&m_switch_report), sizeof(hid_switch_report_t)};
}

usb_report_t InputState::getPS3DancePadInputReport() {
    m_ps3_dance_pad_report.buttons1 =
        0                                                                            //
        | ((m_controller.buttons.west || m_pad.down_right.triggered) ? (1 << 0) : 0) // Square
        | ((m_controller.buttons.south || m_pad.up_right.triggered) ? (1 << 1) : 0)  // Cross
        | ((m_controller.buttons.east || m_pad.up_left.triggered) ? (1 << 2) : 0)    // Circle
        | ((m_controller.buttons.north || m_pad.down_left.triggered) ? (1 << 3) : 0) // Triangle
        | (m_controller.buttons.l ? (1 << 4) : 0)                                    // L1
        | (m_controller.buttons.r ? (1 << 5) : 0);                                   // R1

    m_ps3_dance_pad_report.buttons2 = 0 | (m_controller.buttons.select ? (1 << 0) : 0) // Start
                                      | (m_controller.buttons.start ? (1 << 1) : 0)    // Select
                                      | (m_controller.buttons.home ? (1 << 4) : 0);    // Home

    m_ps3_dance_pad_report.hat = getHidHat(m_controller.dpad);

    m_ps3_dance_pad_report.x = 0x80;
    m_ps3_dance_pad_report.y = 0x80;
    m_ps3_dance_pad_report.z = 0x80;
    m_ps3_dance_pad_report.rz = 0x80;

    m_ps3_dance_pad_report.vendor_right = ((m_controller.dpad.right || m_pad.right.triggered) ? 0xFF : 0);
    m_ps3_dance_pad_report.vendor_left = ((m_controller.dpad.left || m_pad.left.triggered) ? 0xFF : 0);
    m_ps3_dance_pad_report.vendor_up = ((m_controller.dpad.up || m_pad.up.triggered) ? 0xFF : 0);
    m_ps3_dance_pad_report.vendor_down = ((m_controller.dpad.down || m_pad.down.triggered) ? 0xFF : 0);
    m_ps3_dance_pad_report.vendor_north = ((m_controller.buttons.north || m_pad.down_left.triggered) ? 0xFF : 0);
    m_ps3_dance_pad_report.vendor_east = ((m_controller.buttons.east || m_pad.up_left.triggered) ? 0xFF : 0);
    m_ps3_dance_pad_report.vendor_south = ((m_controller.buttons.south || m_pad.up_right.triggered) ? 0xFF : 0);
    m_ps3_dance_pad_report.vendor_west = ((m_controller.buttons.west || m_pad.down_right.triggered) ? 0xFF : 0);

    m_ps3_dance_pad_report.vendor_0x2C = 0x0200;
    m_ps3_dance_pad_report.vendor_0x2D = 0x0200;
    m_ps3_dance_pad_report.vendor_0x2E = 0x0200;
    m_ps3_dance_pad_report.vendor_0x2F = 0x0200;

    return {reinterpret_cast<uint8_t *>(&m_ps3_dance_pad_report), sizeof(hid_ps3_dance_pad_report_t)};
}

usb_report_t InputState::getPS3InputReport() {
    memset(&m_ps3_report, 0, sizeof(m_ps3_report));

    m_ps3_report.report_id = 0x01;

    m_ps3_report.buttons1 = 0                                                                             //
                            | ((m_controller.buttons.select || m_pad.select) ? (1 << 0) : 0)              // Select
                            | (false ? (1 << 1) : 0)                                                      // L3
                            | (false ? (1 << 2) : 0)                                                      // R3
                            | ((m_controller.buttons.start || m_pad.start) ? (1 << 3) : 0)                // Start
                            | ((m_controller.dpad.up || m_pad.up.triggered) ? (1 << 4) : 0)               // Up
                            | ((m_controller.dpad.right || m_pad.right.triggered) ? (1 << 5) : 0)         // Right
                            | ((m_controller.dpad.down || m_pad.down.triggered) ? (1 << 6) : 0)           // Down
                            | ((m_controller.dpad.left || m_pad.left.triggered) ? (1 << 7) : 0);          // Left
    m_ps3_report.buttons2 = 0                                                                             //
                            | (false ? (1 << 0) : 0)                                                      // L2
                            | (false ? (1 << 1) : 0)                                                      // R2
                            | (m_controller.buttons.l ? (1 << 2) : 0)                                     // L1
                            | ((m_controller.buttons.r || m_pad.center.triggered) ? (1 << 3) : 0)         // R1
                            | ((m_controller.buttons.north || m_pad.down_left.triggered) ? (1 << 4) : 0)  // Triangle
                            | ((m_controller.buttons.east || m_pad.up_left.triggered) ? (1 << 5) : 0)     // Circle
                            | ((m_controller.buttons.south || m_pad.up_right.triggered) ? (1 << 6) : 0)   // Cross
                            | ((m_controller.buttons.west || m_pad.down_right.triggered) ? (1 << 7) : 0); // Square
    m_ps3_report.buttons3 = 0 | (m_controller.buttons.home ? (1 << 0) : 0);                               // Home

    // Center all sticks
    m_ps3_report.lx = 0x80;
    m_ps3_report.ly = 0x80;
    m_ps3_report.rx = 0x80;
    m_ps3_report.ry = 0x80;

    m_ps3_report.lt = 0x00;
    m_ps3_report.rt = 0x00;

    m_ps3_report.unknown_0x02_1 = 0x02;
    m_ps3_report.battery = 0xef;
    m_ps3_report.unknown_0x12 = 0x12;

    m_ps3_report.unknown[0] = 0x12;
    m_ps3_report.unknown[1] = 0xf8;
    m_ps3_report.unknown[2] = 0x77;
    m_ps3_report.unknown[3] = 0x00;
    m_ps3_report.unknown[4] = 0x40;

    m_ps3_report.acc_x = 511;
    m_ps3_report.acc_y = 511;
    m_ps3_report.acc_z = 511;

    m_ps3_report.unknown_0x02_2 = 0x02;

    return {reinterpret_cast<uint8_t *>(&m_ps3_report), sizeof(hid_ps3_report_t)};
}

usb_report_t InputState::getPS4InputReport() {
    static uint8_t report_counter = 0;

    memset(&m_ps4_report, 0, sizeof(m_ps4_report));

    m_ps4_report.report_id = 0x01;

    // Center all sticks
    m_ps4_report.lx = 0x80;
    m_ps4_report.ly = 0x80;
    m_ps4_report.rx = 0x80;
    m_ps4_report.ry = 0x80;

    m_ps4_report.buttons1 = getHidHat(m_controller.dpad)                                            //
                            | ((m_controller.buttons.west || m_pad.left.triggered) ? (1 << 4) : 0)  // Square
                            | ((m_controller.buttons.south || m_pad.down.triggered) ? (1 << 5) : 0) // Cross
                            | ((m_controller.buttons.east || m_pad.right.triggered) ? (1 << 6) : 0) // Circle
                            | ((m_controller.buttons.north || m_pad.up.triggered) ? (1 << 7) : 0);  // Triangle
    m_ps4_report.buttons2 = 0                                                                       //
                            | ((m_controller.buttons.l || m_pad.up_left.triggered) ? (1 << 0) : 0)  // L1
                            | ((m_controller.buttons.r || m_pad.up_right.triggered) ? (1 << 1) : 0) // R1
                            | (m_pad.down_left.triggered ? (1 << 2) : 0)                            // L2
                            | (m_pad.down_right.triggered ? (1 << 3) : 0)                           // R2
                            | (m_pad.center.triggered ? (1 << 6) : 0)                               // L3
                            | (false ? (1 << 7) : 0)                                                // R3
                            | (m_controller.buttons.share ? (1 << 4) : 0)                           // Share
                            | ((m_controller.buttons.start || m_pad.start) ? (1 << 5) : 0);         // Option
    m_ps4_report.buttons3 = (report_counter << 2)                                                   //
                            | (m_controller.buttons.home ? (1 << 0) : 0)                            // PS
                            | ((m_controller.buttons.select || m_pad.select) ? (1 << 1) : 0);       // T-Pad

    m_ps4_report.lt = 0x00;
    m_ps4_report.rt = 0x00;

    m_ps4_report.battery = 0 | (1 << 4) | 11; // Cable connected and fully charged
    m_ps4_report.peripheral = 0x01;
    m_ps4_report.touch_report_count = 0;

    // This method actually gets called more often than the report is sent,
    // so counters are not consecutive ... let's see if this turns out to
    // be a problem.
    report_counter++;
    if (report_counter > (UINT8_MAX >> 2)) {
        report_counter = 0;
    }

    return {reinterpret_cast<uint8_t *>(&m_ps4_report), sizeof(hid_ps4_report_t)};
}

usb_report_t InputState::getKeyboardReport(const InputState::Player player) {
    m_keyboard_report = {.keycodes = {0}};

    auto set_key = [&](const bool input, const uint8_t keycode) {
        if (input) {
            m_keyboard_report.keycodes[keycode / 8] |= 1 << (keycode % 8);
        }
    };

    switch (player) {
    case Player::One: {
        set_key(m_pad.start, HID_KEY_ENTER);
        set_key(m_pad.select, HID_KEY_SLASH);

        set_key(m_pad.up_left.triggered, HID_KEY_Q);
        set_key(m_pad.up.triggered, HID_KEY_ARROW_UP);
        set_key(m_pad.up_right.triggered, HID_KEY_E);
        set_key(m_pad.left.triggered, HID_KEY_ARROW_LEFT);
        set_key(m_pad.center.triggered, HID_KEY_S);
        set_key(m_pad.right.triggered, HID_KEY_ARROW_RIGHT);
        set_key(m_pad.down_left.triggered, HID_KEY_Z);
        set_key(m_pad.down.triggered, HID_KEY_ARROW_DOWN);
        set_key(m_pad.down_right.triggered, HID_KEY_C);
    } break;
    case Player::Two: {
        set_key(m_pad.start, HID_KEY_KEYPAD_ENTER);
        set_key(m_pad.select, HID_KEY_KEYPAD_0);

        set_key(m_pad.up_left.triggered, HID_KEY_KEYPAD_7);
        set_key(m_pad.up.triggered, HID_KEY_KEYPAD_8);
        set_key(m_pad.up_right.triggered, HID_KEY_KEYPAD_9);
        set_key(m_pad.left.triggered, HID_KEY_KEYPAD_4);
        set_key(m_pad.center.triggered, HID_KEY_KEYPAD_5);
        set_key(m_pad.right.triggered, HID_KEY_KEYPAD_6);
        set_key(m_pad.down_left.triggered, HID_KEY_KEYPAD_1);
        set_key(m_pad.down.triggered, HID_KEY_KEYPAD_2);
        set_key(m_pad.down_right.triggered, HID_KEY_KEYPAD_3);
    } break;
    }

    set_key(m_controller.dpad.up, HID_KEY_ARROW_UP);
    set_key(m_controller.dpad.down, HID_KEY_ARROW_DOWN);
    set_key(m_controller.dpad.left, HID_KEY_ARROW_LEFT);
    set_key(m_controller.dpad.right, HID_KEY_ARROW_RIGHT);

    set_key(m_controller.buttons.north, HID_KEY_TAB);
    set_key(m_controller.buttons.east, HID_KEY_ESCAPE);
    set_key(m_controller.buttons.south, HID_KEY_ENTER);
    set_key(m_controller.buttons.west, HID_KEY_BACKSPACE);

    set_key(m_controller.buttons.l, HID_KEY_F);
    set_key(m_controller.buttons.r, HID_KEY_J);

    set_key(m_controller.buttons.start, HID_KEY_ENTER);
    set_key(m_controller.buttons.select, HID_KEY_SLASH);
    set_key(m_controller.buttons.home, HID_KEY_HOME);
    set_key(m_controller.buttons.share, HID_KEY_F1);

    return {reinterpret_cast<uint8_t *>(&m_keyboard_report), sizeof(hid_nkro_keyboard_report_t)};
}

usb_report_t InputState::getXinputReport() {
    m_xinput_report.report_size = sizeof(xinput_report_t);

    m_xinput_report.buttons1 = 0                                                                           //
                               | ((m_controller.dpad.up || m_pad.up.triggered) ? (1 << 0) : 0)             // Dpad Up
                               | ((m_controller.dpad.down || m_pad.down.triggered) ? (1 << 1) : 0)         // Dpad Down
                               | ((m_controller.dpad.left || m_pad.left.triggered) ? (1 << 2) : 0)         // Dpad Left
                               | ((m_controller.dpad.right || m_pad.right.triggered) ? (1 << 3) : 0)       // Dpad Right
                               | ((m_controller.buttons.start || m_pad.start) ? (1 << 4) : 0)              // Start
                               | ((m_controller.buttons.select || m_pad.select) ? (1 << 5) : 0)            // Back
                               | (false ? (1 << 6) : 0)                                                    // L3
                               | (false ? (1 << 7) : 0);                                                   // R3
    m_xinput_report.buttons2 = 0                                                                           //
                               | (m_controller.buttons.l ? (1 << 0) : 0)                                   // L1
                               | ((m_controller.buttons.r || m_pad.center.triggered) ? (1 << 1) : 0)       // R1
                               | (m_controller.buttons.home ? (1 << 2) : 0)                                // Guide
                               | ((m_controller.buttons.south || m_pad.up_right.triggered) ? (1 << 4) : 0) // A
                               | ((m_controller.buttons.east || m_pad.up_left.triggered) ? (1 << 5) : 0)   // B
                               | ((m_controller.buttons.west || m_pad.down_right.triggered) ? (1 << 6) : 0)  // X
                               | ((m_controller.buttons.north || m_pad.down_left.triggered) ? (1 << 7) : 0); // Y

    m_xinput_report.lt = 0;
    m_xinput_report.rt = 0;

    m_xinput_report.lx = 0;
    m_xinput_report.ly = 0;
    m_xinput_report.rx = 0;
    m_xinput_report.ry = 0;

    return {reinterpret_cast<uint8_t *>(&m_xinput_report), sizeof(xinput_report_t)};
}

usb_report_t InputState::getSpice2xReport() {
    m_spice2x_report.report_id = 0x01;

    m_spice2x_report.controller_buttons = getHidHat(m_controller.dpad)                    //
                                          | (m_controller.buttons.south ? (1 << 4) : 0)   //
                                          | (m_controller.buttons.east ? (1 << 5) : 0)    //
                                          | (m_controller.buttons.west ? (1 << 6) : 0)    //
                                          | (m_controller.buttons.north ? (1 << 7) : 0)   //
                                          | (m_controller.buttons.r ? (1 << 8) : 0)       //
                                          | (m_controller.buttons.l ? (1 << 9) : 0)       //
                                          | (m_controller.buttons.start ? (1 << 10) : 0)  //
                                          | (m_controller.buttons.select ? (1 << 11) : 0) //
                                          | (m_controller.buttons.home ? (1 << 12) : 0)   //
                                          | (m_controller.buttons.share ? (1 << 13) : 0); //

    m_spice2x_report.pad_buttons = 0                                               //
                                   | (m_pad.start ? (1 << 0) : 0)                  //
                                   | (m_pad.select ? (1 << 1) : 0)                 //
                                   | (m_pad.up_left.triggered ? (1 << 2) : 0)      //
                                   | (m_pad.up.triggered ? (1 << 3) : 0)           //
                                   | (m_pad.up_right.triggered ? (1 << 4) : 0)     //
                                   | (m_pad.left.triggered ? (1 << 5) : 0)         //
                                   | (m_pad.center.triggered ? (1 << 6) : 0)       //
                                   | (m_pad.right.triggered ? (1 << 7) : 0)        //
                                   | (m_pad.down_left.triggered ? (1 << 8) : 0)    //
                                   | (m_pad.down.triggered ? (1 << 9) : 0)         //
                                   | (m_pad.down_right.triggered ? (1 << 10) : 0); //

    return {reinterpret_cast<uint8_t *>(&m_spice2x_report), sizeof(hid_spice2x_report_t)};
}

usb_report_t InputState::getDebugReport() {
    std::stringstream out;

    out                                                               //
        << (m_pad.select ? "[SE]" : "[  ]") << "|"                    //
        << (m_pad.start ? "[ST]" : "[  ]")                            //
        << "\n"                                                       //
        << (m_pad.up_left.triggered ? "[\\]" : "[ ]")                 //
        << (m_pad.up.triggered ? "[^]" : "[ ]")                       //
        << (m_pad.up_right.triggered ? "[/]" : "[ ]")                 //
        << "   ::   [" << std::setw(5) << m_pad.up_left.raw << "] "   //
        << "[" << std::setw(5) << m_pad.up.raw << "] "                //
        << "[" << std::setw(5) << m_pad.up_right.raw << "] "          //
        << "\n"                                                       //
        << (m_pad.left.triggered ? "[<]" : "[ ]")                     //
        << (m_pad.center.triggered ? "[o]" : "[ ]")                   //
        << (m_pad.right.triggered ? "[>]" : "[ ]")                    //
        << "   ::   [" << std::setw(5) << m_pad.left.raw << "] "      //
        << "[" << std::setw(5) << m_pad.center.raw << "] "            //
        << "[" << std::setw(5) << m_pad.right.raw << "] "             //
        << "\n"                                                       //
        << (m_pad.down_left.triggered ? "[/]" : "[ ]")                //
        << (m_pad.down.triggered ? "[v]" : "[ ]")                     //
        << (m_pad.down_right.triggered ? "[\\]" : "[ ]")              //
        << "   ::   [" << std::setw(5) << m_pad.down_left.raw << "] " //
        << "[" << std::setw(5) << m_pad.down.raw << "] "              //
        << "[" << std::setw(5) << m_pad.down_right.raw << "] "        //
        << "\033[0;0H";

    m_debug_report = out.str();

    return {reinterpret_cast<uint8_t *>(m_debug_report.data()), static_cast<uint16_t>(m_debug_report.size() + 1)};
}

void InputState::releaseAll() {
    m_pad = {};
    m_controller = {};
}

bool InputState::checkHotkey() const {
    static uint32_t hold_since = 0;
    static bool hold_active = false;
    static const uint32_t hold_timeout = 2000;

    if (m_controller.buttons.start && m_controller.buttons.select) {
        const uint32_t now = to_ms_since_boot(get_absolute_time());
        if (!hold_active) {
            hold_active = true;
            hold_since = now;
        } else if ((now - hold_since) > hold_timeout) {
            hold_active = false;
            return true;
        }
    } else {
        hold_active = false;
    }
    return false;
}

} // namespace Dancecon::Utils
