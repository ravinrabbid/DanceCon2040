#include "utils/InputReport.h"

#include <iomanip>
#include <sstream>

namespace Dancecon::Utils {

namespace {

uint8_t getHidHat(const InputState::Controller::DPad &dpad) {
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

usb_report_t InputReport::getSwitchReport(const InputState &state) {
    const auto &controller = state.controller;
    const auto &pad = state.pad;

    m_switch_report.buttons = 0                                                                   //
                              | ((controller.buttons.west || pad.left.triggered) ? (1 << 0) : 0)  // Y
                              | ((controller.buttons.south || pad.down.triggered) ? (1 << 1) : 0) // B
                              | ((controller.buttons.east || pad.right.triggered) ? (1 << 2) : 0) // A
                              | ((controller.buttons.north || pad.up.triggered) ? (1 << 3) : 0)   // X
                              | ((controller.buttons.l || pad.up_left.triggered) ? (1 << 4) : 0)  // L
                              | ((controller.buttons.r || pad.up_right.triggered) ? (1 << 5) : 0) // R
                              | (pad.down_left.triggered ? (1 << 6) : 0)                          // ZL
                              | (pad.down_right.triggered ? (1 << 7) : 0)                         // ZR
                              | ((controller.buttons.select || pad.select) ? (1 << 8) : 0)        // -
                              | ((controller.buttons.start || pad.start) ? (1 << 9) : 0)          // +
                              | (pad.center.triggered ? (1 << 10) : 0)                            // LS
                              | (false ? (1 << 11) : 0)                                           // RS
                              | (controller.buttons.home ? (1 << 12) : 0)                         // Home
                              | (controller.buttons.share ? (1 << 13) : 0);                       // Capture

    m_switch_report.hat = getHidHat(controller.dpad);

    return {reinterpret_cast<uint8_t *>(&m_switch_report), sizeof(hid_switch_report_t)};
}

usb_report_t InputReport::getPS3DancePadInputReport(const InputState &state) {
    const auto &controller = state.controller;
    const auto &pad = state.pad;

    m_ps3_dance_pad_report.buttons1 = 0                                                                        //
                                      | ((controller.buttons.west || pad.down_right.triggered) ? (1 << 0) : 0) // Square
                                      | ((controller.buttons.south || pad.up_right.triggered) ? (1 << 1) : 0)  // Cross
                                      | ((controller.buttons.east || pad.up_left.triggered) ? (1 << 2) : 0)    // Circle
                                      |
                                      ((controller.buttons.north || pad.down_left.triggered) ? (1 << 3) : 0) // Triangle
                                      | (controller.buttons.l ? (1 << 4) : 0)                                // L1
                                      | (controller.buttons.r ? (1 << 5) : 0);                               // R1
    m_ps3_dance_pad_report.buttons2 = 0 | (controller.buttons.select ? (1 << 0) : 0)                         // Start
                                      | (controller.buttons.start ? (1 << 1) : 0)                            // Select
                                      | (controller.buttons.home ? (1 << 4) : 0);                            // Home

    m_ps3_dance_pad_report.hat = getHidHat(controller.dpad);

    m_ps3_dance_pad_report.vendor_right = ((controller.dpad.right || pad.right.triggered) ? 0xFF : 0);
    m_ps3_dance_pad_report.vendor_left = ((controller.dpad.left || pad.left.triggered) ? 0xFF : 0);
    m_ps3_dance_pad_report.vendor_up = ((controller.dpad.up || pad.up.triggered) ? 0xFF : 0);
    m_ps3_dance_pad_report.vendor_down = ((controller.dpad.down || pad.down.triggered) ? 0xFF : 0);
    m_ps3_dance_pad_report.vendor_north = ((controller.buttons.north || pad.down_left.triggered) ? 0xFF : 0);
    m_ps3_dance_pad_report.vendor_east = ((controller.buttons.east || pad.up_left.triggered) ? 0xFF : 0);
    m_ps3_dance_pad_report.vendor_south = ((controller.buttons.south || pad.up_right.triggered) ? 0xFF : 0);
    m_ps3_dance_pad_report.vendor_west = ((controller.buttons.west || pad.down_right.triggered) ? 0xFF : 0);

    return {reinterpret_cast<uint8_t *>(&m_ps3_dance_pad_report), sizeof(hid_ps3_dance_pad_report_t)};
}

usb_report_t InputReport::getPS3InputReport(const InputState &state) {
    const auto &controller = state.controller;
    const auto &pad = state.pad;

    m_ps3_report.buttons1 = 0                                                                         //
                            | ((controller.buttons.select || pad.select) ? (1 << 0) : 0)              // Select
                            | (false ? (1 << 1) : 0)                                                  // L3
                            | (false ? (1 << 2) : 0)                                                  // R3
                            | ((controller.buttons.start || pad.start) ? (1 << 3) : 0)                // Start
                            | ((controller.dpad.up || pad.up.triggered) ? (1 << 4) : 0)               // Up
                            | ((controller.dpad.right || pad.right.triggered) ? (1 << 5) : 0)         // Right
                            | ((controller.dpad.down || pad.down.triggered) ? (1 << 6) : 0)           // Down
                            | ((controller.dpad.left || pad.left.triggered) ? (1 << 7) : 0);          // Left
    m_ps3_report.buttons2 = 0                                                                         //
                            | (false ? (1 << 0) : 0)                                                  // L2
                            | (false ? (1 << 1) : 0)                                                  // R2
                            | (controller.buttons.l ? (1 << 2) : 0)                                   // L1
                            | ((controller.buttons.r || pad.center.triggered) ? (1 << 3) : 0)         // R1
                            | ((controller.buttons.north || pad.down_left.triggered) ? (1 << 4) : 0)  // Triangle
                            | ((controller.buttons.east || pad.up_left.triggered) ? (1 << 5) : 0)     // Circle
                            | ((controller.buttons.south || pad.up_right.triggered) ? (1 << 6) : 0)   // Cross
                            | ((controller.buttons.west || pad.down_right.triggered) ? (1 << 7) : 0); // Square
    m_ps3_report.buttons3 = 0 | (controller.buttons.home ? (1 << 0) : 0);                             // Home

    return {reinterpret_cast<uint8_t *>(&m_ps3_report), sizeof(hid_ps3_report_t)};
}

usb_report_t InputReport::getPS4InputReport(const InputState &state) {
    const auto &controller = state.controller;
    const auto &pad = state.pad;

    m_ps4_report.buttons1 = getHidHat(controller.dpad)                                          //
                            | ((controller.buttons.west || pad.left.triggered) ? (1 << 4) : 0)  // Square
                            | ((controller.buttons.south || pad.down.triggered) ? (1 << 5) : 0) // Cross
                            | ((controller.buttons.east || pad.right.triggered) ? (1 << 6) : 0) // Circle
                            | ((controller.buttons.north || pad.up.triggered) ? (1 << 7) : 0);  // Triangle
    m_ps4_report.buttons2 = 0                                                                   //
                            | ((controller.buttons.l || pad.up_left.triggered) ? (1 << 0) : 0)  // L1
                            | ((controller.buttons.r || pad.up_right.triggered) ? (1 << 1) : 0) // R1
                            | (pad.down_left.triggered ? (1 << 2) : 0)                          // L2
                            | (pad.down_right.triggered ? (1 << 3) : 0)                         // R2
                            | (pad.center.triggered ? (1 << 6) : 0)                             // L3
                            | (false ? (1 << 7) : 0)                                            // R3
                            | (controller.buttons.share ? (1 << 4) : 0)                         // Share
                            | ((controller.buttons.start || pad.start) ? (1 << 5) : 0);         // Option
    m_ps4_report.buttons3 = (m_ps4_report_counter << 2)                                         //
                            | (controller.buttons.home ? (1 << 0) : 0)                          // PS
                            | ((controller.buttons.select || pad.select) ? (1 << 1) : 0);       // T-Pad

    // This method actually gets called more often than the report is sent,
    // so counters are not consecutive ... let's see if this turns out to
    // be a problem.
    if (++m_ps4_report_counter > (UINT8_MAX >> 2)) {
        m_ps4_report_counter = 0;
    }

    return {reinterpret_cast<uint8_t *>(&m_ps4_report), sizeof(hid_ps4_report_t)};
}

usb_report_t InputReport::getKeyboardReport(const InputState &state, const InputReport::Player player) {
    const auto &controller = state.controller;
    const auto &pad = state.pad;

    m_keyboard_report = {};

    auto set_key = [&](const bool input, const uint8_t keycode) {
        if (input) {
            m_keyboard_report.keycodes[keycode / 8] |= 1 << (keycode % 8);
        }
    };

    switch (player) {
    case Player::One: {
        set_key(pad.start, HID_KEY_ENTER);
        set_key(pad.select, HID_KEY_SLASH);

        set_key(pad.up_left.triggered, HID_KEY_Q);
        set_key(pad.up.triggered, HID_KEY_ARROW_UP);
        set_key(pad.up_right.triggered, HID_KEY_E);
        set_key(pad.left.triggered, HID_KEY_ARROW_LEFT);
        set_key(pad.center.triggered, HID_KEY_S);
        set_key(pad.right.triggered, HID_KEY_ARROW_RIGHT);
        set_key(pad.down_left.triggered, HID_KEY_Z);
        set_key(pad.down.triggered, HID_KEY_ARROW_DOWN);
        set_key(pad.down_right.triggered, HID_KEY_C);
    } break;
    case Player::Two: {
        set_key(pad.start, HID_KEY_KEYPAD_ENTER);
        set_key(pad.select, HID_KEY_KEYPAD_0);

        set_key(pad.up_left.triggered, HID_KEY_KEYPAD_7);
        set_key(pad.up.triggered, HID_KEY_KEYPAD_8);
        set_key(pad.up_right.triggered, HID_KEY_KEYPAD_9);
        set_key(pad.left.triggered, HID_KEY_KEYPAD_4);
        set_key(pad.center.triggered, HID_KEY_KEYPAD_5);
        set_key(pad.right.triggered, HID_KEY_KEYPAD_6);
        set_key(pad.down_left.triggered, HID_KEY_KEYPAD_1);
        set_key(pad.down.triggered, HID_KEY_KEYPAD_2);
        set_key(pad.down_right.triggered, HID_KEY_KEYPAD_3);
    } break;
    }

    set_key(controller.dpad.up, HID_KEY_ARROW_UP);
    set_key(controller.dpad.down, HID_KEY_ARROW_DOWN);
    set_key(controller.dpad.left, HID_KEY_ARROW_LEFT);
    set_key(controller.dpad.right, HID_KEY_ARROW_RIGHT);

    set_key(controller.buttons.north, HID_KEY_TAB);
    set_key(controller.buttons.east, HID_KEY_ESCAPE);
    set_key(controller.buttons.south, HID_KEY_ENTER);
    set_key(controller.buttons.west, HID_KEY_BACKSPACE);

    set_key(controller.buttons.l, HID_KEY_F);
    set_key(controller.buttons.r, HID_KEY_J);

    set_key(controller.buttons.start, HID_KEY_ENTER);
    set_key(controller.buttons.select, HID_KEY_SLASH);
    set_key(controller.buttons.home, HID_KEY_HOME);
    set_key(controller.buttons.share, HID_KEY_F1);

    return {reinterpret_cast<uint8_t *>(&m_keyboard_report), sizeof(hid_nkro_keyboard_report_t)};
}

usb_report_t InputReport::getXinputReport(const InputState &state) {
    const auto &controller = state.controller;
    const auto &pad = state.pad;

    m_xinput_report.buttons1 = 0                                                                         //
                               | ((controller.dpad.up || pad.up.triggered) ? (1 << 0) : 0)               // Dpad Up
                               | ((controller.dpad.down || pad.down.triggered) ? (1 << 1) : 0)           // Dpad Down
                               | ((controller.dpad.left || pad.left.triggered) ? (1 << 2) : 0)           // Dpad Left
                               | ((controller.dpad.right || pad.right.triggered) ? (1 << 3) : 0)         // Dpad Right
                               | ((controller.buttons.start || pad.start) ? (1 << 4) : 0)                // Start
                               | ((controller.buttons.select || pad.select) ? (1 << 5) : 0)              // Back
                               | (false ? (1 << 6) : 0)                                                  // L3
                               | (false ? (1 << 7) : 0);                                                 // R3
    m_xinput_report.buttons2 = 0                                                                         //
                               | (controller.buttons.l ? (1 << 0) : 0)                                   // L1
                               | ((controller.buttons.r || pad.center.triggered) ? (1 << 1) : 0)         // R1
                               | (controller.buttons.home ? (1 << 2) : 0)                                // Guide
                               | ((controller.buttons.south || pad.up_right.triggered) ? (1 << 4) : 0)   // A
                               | ((controller.buttons.east || pad.up_left.triggered) ? (1 << 5) : 0)     // B
                               | ((controller.buttons.west || pad.down_right.triggered) ? (1 << 6) : 0)  // X
                               | ((controller.buttons.north || pad.down_left.triggered) ? (1 << 7) : 0); // Y

    return {reinterpret_cast<uint8_t *>(&m_xinput_report), sizeof(xinput_report_t)};
}

usb_report_t InputReport::getSpice2xReport(const InputState &state) {
    const auto &controller = state.controller;
    const auto &pad = state.pad;

    m_spice2x_report.controller_buttons = getHidHat(controller.dpad)                    //
                                          | (controller.buttons.south ? (1 << 4) : 0)   //
                                          | (controller.buttons.east ? (1 << 5) : 0)    //
                                          | (controller.buttons.west ? (1 << 6) : 0)    //
                                          | (controller.buttons.north ? (1 << 7) : 0)   //
                                          | (controller.buttons.r ? (1 << 8) : 0)       //
                                          | (controller.buttons.l ? (1 << 9) : 0)       //
                                          | (controller.buttons.start ? (1 << 10) : 0)  //
                                          | (controller.buttons.select ? (1 << 11) : 0) //
                                          | (controller.buttons.home ? (1 << 12) : 0)   //
                                          | (controller.buttons.share ? (1 << 13) : 0); //
    m_spice2x_report.pad_buttons = 0                                                    //
                                   | (pad.start ? (1 << 0) : 0)                         //
                                   | (pad.select ? (1 << 1) : 0)                        //
                                   | (pad.up_left.triggered ? (1 << 2) : 0)             //
                                   | (pad.up.triggered ? (1 << 3) : 0)                  //
                                   | (pad.up_right.triggered ? (1 << 4) : 0)            //
                                   | (pad.left.triggered ? (1 << 5) : 0)                //
                                   | (pad.center.triggered ? (1 << 6) : 0)              //
                                   | (pad.right.triggered ? (1 << 7) : 0)               //
                                   | (pad.down_left.triggered ? (1 << 8) : 0)           //
                                   | (pad.down.triggered ? (1 << 9) : 0)                //
                                   | (pad.down_right.triggered ? (1 << 10) : 0);        //

    return {reinterpret_cast<uint8_t *>(&m_spice2x_report), sizeof(hid_spice2x_report_t)};
}

usb_report_t InputReport::getDebugReport(const InputState &state) {
    const auto &pad = state.pad;

    std::stringstream out;

    out                                                             //
        << (pad.select ? "[SE]" : "[  ]") << "|"                    //
        << (pad.start ? "[ST]" : "[  ]")                            //
        << "\n"                                                     //
        << (pad.up_left.triggered ? "[\\]" : "[ ]")                 //
        << (pad.up.triggered ? "[^]" : "[ ]")                       //
        << (pad.up_right.triggered ? "[/]" : "[ ]")                 //
        << "   ::   [" << std::setw(5) << pad.up_left.raw << "] "   //
        << "[" << std::setw(5) << pad.up.raw << "] "                //
        << "[" << std::setw(5) << pad.up_right.raw << "] "          //
        << "\n"                                                     //
        << (pad.left.triggered ? "[<]" : "[ ]")                     //
        << (pad.center.triggered ? "[o]" : "[ ]")                   //
        << (pad.right.triggered ? "[>]" : "[ ]")                    //
        << "   ::   [" << std::setw(5) << pad.left.raw << "] "      //
        << "[" << std::setw(5) << pad.center.raw << "] "            //
        << "[" << std::setw(5) << pad.right.raw << "] "             //
        << "\n"                                                     //
        << (pad.down_left.triggered ? "[/]" : "[ ]")                //
        << (pad.down.triggered ? "[v]" : "[ ]")                     //
        << (pad.down_right.triggered ? "[\\]" : "[ ]")              //
        << "   ::   [" << std::setw(5) << pad.down_left.raw << "] " //
        << "[" << std::setw(5) << pad.down.raw << "] "              //
        << "[" << std::setw(5) << pad.down_right.raw << "] "        //
        << "\033[0;0H";

    m_debug_report = out.str();

    return {reinterpret_cast<uint8_t *>(m_debug_report.data()), static_cast<uint16_t>(m_debug_report.size() + 1)};
}

usb_report_t InputReport::getUsbReport(const InputState &state, const usb_mode_t mode) {
    switch (mode) {
    case USB_MODE_SWITCH_HORIPAD:
        return getSwitchReport(state);
    case USB_MODE_PS3_DANCE:
        return getPS3DancePadInputReport(state);
    case USB_MODE_DUALSHOCK3:
        return getPS3InputReport(state);
    case USB_MODE_DUALSHOCK4:
    case USB_MODE_PS4_COMPAT:
        return getPS4InputReport(state);
    case USB_MODE_KEYBOARD_P1:
        return getKeyboardReport(state, Player::One);
    case USB_MODE_KEYBOARD_P2:
        return getKeyboardReport(state, Player::Two);
    case USB_MODE_XBOX360_DANCE:
    case USB_MODE_XBOX360:
        return getXinputReport(state);
    case USB_MODE_SPICE2X:
        return getSpice2xReport(state);
    case USB_MODE_DEBUG:
        return getDebugReport(state);
    }

    return getDebugReport(state);
}

} // namespace Dancecon::Utils
