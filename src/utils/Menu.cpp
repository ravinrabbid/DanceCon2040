#include "utils/Menu.h"

#include "GlobalConfiguration.h"
#include "peripherals/Pad.h"

namespace Dancecon::Utils {

using config_t = decltype(Config::Default::pad_config);

template class Menu<config_t::Thresholds>;

template <>
const MenuDescriptor Menu<config_t::ThresholdsFourPanel>::thresholds_descriptor = {
    MenuDescriptor::Type::Menu,
    "Thresholds",
    {{"Up", MenuDescriptor::Action::GotoPagePadTriggerThresholdUp},       //
     {"Left", MenuDescriptor::Action::GotoPagePadTriggerThresholdLeft},   //
     {"Right", MenuDescriptor::Action::GotoPagePadTriggerThresholdRight}, //
     {"Down", MenuDescriptor::Action::GotoPagePadTriggerThresholdDown}},  //
    0};

template <>
const MenuDescriptor Menu<config_t::ThresholdsFivePanel>::thresholds_descriptor = {
    MenuDescriptor::Type::Menu,
    "Thresholds",
    {{"Up Left", MenuDescriptor::Action::GotoPagePadTriggerThresholdUpLeft},       //
     {"Up Right", MenuDescriptor::Action::GotoPagePadTriggerThresholdUpRight},     //
     {"Center", MenuDescriptor::Action::GotoPagePadTriggerThresholdCenter},        //
     {"Down Left", MenuDescriptor::Action::GotoPagePadTriggerThresholdDownRight},  //
     {"Down Right", MenuDescriptor::Action::GotoPagePadTriggerThresholdDownLeft}}, //
    0};

template <>
const MenuDescriptor Menu<config_t::ThresholdsSixPanel>::thresholds_descriptor = {
    MenuDescriptor::Type::Menu,
    "Thresholds",
    {{"Up Left", MenuDescriptor::Action::GotoPagePadTriggerThresholdUpLeft},   //
     {"Up", MenuDescriptor::Action::GotoPagePadTriggerThresholdUp},            //
     {"Up Right", MenuDescriptor::Action::GotoPagePadTriggerThresholdUpRight}, //
     {"Left", MenuDescriptor::Action::GotoPagePadTriggerThresholdLeft},        //
     {"Right", MenuDescriptor::Action::GotoPagePadTriggerThresholdRight},      //
     {"Down", MenuDescriptor::Action::GotoPagePadTriggerThresholdDown}},       //
    0};

template <>
const MenuDescriptor Menu<config_t::ThresholdsEightPanel>::thresholds_descriptor = {
    MenuDescriptor::Type::Menu,
    "Thresholds",
    {{"Up Left", MenuDescriptor::Action::GotoPagePadTriggerThresholdUpLeft},       //
     {"Up", MenuDescriptor::Action::GotoPagePadTriggerThresholdUp},                //
     {"Up Right", MenuDescriptor::Action::GotoPagePadTriggerThresholdUpRight},     //
     {"Left", MenuDescriptor::Action::GotoPagePadTriggerThresholdLeft},            //
     {"Right", MenuDescriptor::Action::GotoPagePadTriggerThresholdRight},          //
     {"Down Left", MenuDescriptor::Action::GotoPagePadTriggerThresholdDownRight},  //
     {"Down", MenuDescriptor::Action::GotoPagePadTriggerThresholdDown},            //
     {"Down Right", MenuDescriptor::Action::GotoPagePadTriggerThresholdDownLeft}}, //
    0};

template <>
const MenuDescriptor Menu<config_t::ThresholdsNinePanel>::thresholds_descriptor = {
    MenuDescriptor::Type::Menu,
    "Thresholds",
    {{"Up Left", MenuDescriptor::Action::GotoPagePadTriggerThresholdUpLeft},       //
     {"Up", MenuDescriptor::Action::GotoPagePadTriggerThresholdUp},                //
     {"Up Right", MenuDescriptor::Action::GotoPagePadTriggerThresholdUpRight},     //
     {"Left", MenuDescriptor::Action::GotoPagePadTriggerThresholdLeft},            //
     {"Center", MenuDescriptor::Action::GotoPagePadTriggerThresholdCenter},        //
     {"Right", MenuDescriptor::Action::GotoPagePadTriggerThresholdRight},          //
     {"Down Left", MenuDescriptor::Action::GotoPagePadTriggerThresholdDownRight},  //
     {"Down", MenuDescriptor::Action::GotoPagePadTriggerThresholdDown},            //
     {"Down Right", MenuDescriptor::Action::GotoPagePadTriggerThresholdDownLeft}}, //
    0};

template <typename TThresholds>
const std::map<MenuPage, const MenuDescriptor> Menu<TThresholds>::descriptors = {
    {MenuPage::Main,
     {MenuDescriptor::Type::Menu,                               //
      "Settings",                                               //
      {{"Mode", MenuDescriptor::Action::GotoPageDeviceMode},    //
       {"Pad", MenuDescriptor::Action::GotoPagePad},            //
       {"Led", MenuDescriptor::Action::GotoPageLed},            //
       {"Reset", MenuDescriptor::Action::GotoPageReset},        //
       {"USB Flash", MenuDescriptor::Action::GotoPageBootsel}}, //
      0}},                                                      //

    {MenuPage::DeviceMode,
     {MenuDescriptor::Type::Selection,                     //
      "Mode",                                              //
      {{"Swtch Pro", MenuDescriptor::Action::SetUsbMode},  //
       {"Dualshock3", MenuDescriptor::Action::SetUsbMode}, //
       {"Dualshock4", MenuDescriptor::Action::SetUsbMode}, //
       {"PS4 Compat", MenuDescriptor::Action::SetUsbMode}, //
       {"Keybrd P1", MenuDescriptor::Action::SetUsbMode},  //
       {"Keybrd P2", MenuDescriptor::Action::SetUsbMode},  //
       {"Xbox 360", MenuDescriptor::Action::SetUsbMode},   //
       {"MIDI", MenuDescriptor::Action::SetUsbMode},       //
       {"Debug", MenuDescriptor::Action::SetUsbMode}},     //
      0}},                                                 //

    {MenuPage::Pad,                                                        //
     {MenuDescriptor::Type::Menu,                                          //
      "Pad Settings",                                                      //
      {{"Calibrate", MenuDescriptor::Action::DoCalibrate},                 //
       {"Hold Time", MenuDescriptor::Action::GotoPagePadDebounceDelay},    //
       {"Thresholds", MenuDescriptor::Action::GotoPageTriggerThresholds}}, //
      0}},                                                                 //

    {MenuPage::PadDebounceDelay,
     {MenuDescriptor::Type::Value,                         //
      "Step Hold Time (ms)",                               //
      {{"", MenuDescriptor::Action::SetPadDebounceDelay}}, //
      UINT8_MAX}},                                         //

    {MenuPage::PadTriggerThresholds, thresholds_descriptor},

    {MenuPage::PadTriggerThresholdUpLeft,                           //
     {MenuDescriptor::Type::Value,                                  //
      "Threshold Up Left",                                          //
      {{"", MenuDescriptor::Action::SetPadTriggerThresholdUpLeft}}, //
      UINT16_MAX}},

    {MenuPage::PadTriggerThresholdUp,                           //
     {MenuDescriptor::Type::Value,                              //
      "Threshold Up",                                           //
      {{"", MenuDescriptor::Action::SetPadTriggerThresholdUp}}, //
      UINT16_MAX}},

    {MenuPage::PadTriggerThresholdUpRight,                           //
     {MenuDescriptor::Type::Value,                                   //
      "Threshold Up Right",                                          //
      {{"", MenuDescriptor::Action::SetPadTriggerThresholdUpRight}}, //
      UINT16_MAX}},

    {MenuPage::PadTriggerThresholdLeft,                           //
     {MenuDescriptor::Type::Value,                                //
      "Threshold Left",                                           //
      {{"", MenuDescriptor::Action::SetPadTriggerThresholdLeft}}, //
      UINT16_MAX}},

    {MenuPage::PadTriggerThresholdCenter,                           //
     {MenuDescriptor::Type::Value,                                  //
      "Threshold Center",                                           //
      {{"", MenuDescriptor::Action::SetPadTriggerThresholdCenter}}, //
      UINT16_MAX}},

    {MenuPage::PadTriggerThresholdRight,                           //
     {MenuDescriptor::Type::Value,                                 //
      "Threshold Right",                                           //
      {{"", MenuDescriptor::Action::SetPadTriggerThresholdRight}}, //
      UINT16_MAX}},

    {MenuPage::PadTriggerThresholdDownLeft,                           //
     {MenuDescriptor::Type::Value,                                    //
      "Threshold Down Left",                                          //
      {{"", MenuDescriptor::Action::SetPadTriggerThresholdDownLeft}}, //
      UINT16_MAX}},

    {MenuPage::PadTriggerThresholdDown,                           //
     {MenuDescriptor::Type::Value,                                //
      "Threshold Down",                                           //
      {{"", MenuDescriptor::Action::SetPadTriggerThresholdDown}}, //
      UINT16_MAX}},

    {MenuPage::PadTriggerThresholdDownRight,                           //
     {MenuDescriptor::Type::Value,                                     //
      "Threshold Down Right",                                          //
      {{"", MenuDescriptor::Action::SetPadTriggerThresholdDownRight}}, //
      UINT16_MAX}},

    {MenuPage::Led,                                                           //
     {MenuDescriptor::Type::Menu,                                             //
      "LED Settings",                                                         //
      {{"Brightness", MenuDescriptor::Action::GotoPageLedBrightness},         //
       {"Plyr Color", MenuDescriptor::Action::GotoPageLedEnablePlayerColor}}, //
      0}},                                                                    //

    {MenuPage::LedBrightness,                           //
     {MenuDescriptor::Type::Value,                      //
      "LED Brightness",                                 //
      {{"", MenuDescriptor::Action::SetLedBrightness}}, //
      UINT8_MAX}},                                      //

    {MenuPage::LedEnablePlayerColor,                           //
     {MenuDescriptor::Type::Toggle,                            //
      "Player Color (PS4)",                                    //
      {{"", MenuDescriptor::Action::SetLedEnablePlayerColor}}, //
      0}},                                                     //

    {MenuPage::Reset,                              //
     {MenuDescriptor::Type::Menu,                  //
      "Reset all Settings?",                       //
      {{"No", MenuDescriptor::Action::GotoParent}, //
       {"Yes", MenuDescriptor::Action::DoReset}},  //
      0}},                                         //

    {MenuPage::Bootsel,                                         //
     {MenuDescriptor::Type::Menu,                               //
      "Reboot to Flash Mode",                                   //
      {{"Reboot?", MenuDescriptor::Action::DoRebootToBootsel}}, //
      0}},                                                      //

    {MenuPage::BootselMsg,                         //
     {MenuDescriptor::Type::RebootInfo,            //
      "Ready to Flash...",                         //
      {{"BOOTSEL", MenuDescriptor::Action::None}}, //
      0}},                                         //
};

template <typename TThresholds>
Menu<TThresholds>::Menu(std::shared_ptr<SettingsStore<TThresholds>> settings_store)
    : m_store(settings_store), m_active(false), m_state_stack({{MenuPage::Main, 0, 0}}){};

template <typename TThresholds> void Menu<TThresholds>::activate() {
    m_state_stack = std::stack<MenuState>({{MenuPage::Main, 0, 0}});
    m_active = true;
}

static InputState::Controller checkPressed(const InputState::Controller &controller_state) {
    struct ButtonState {
        enum State {
            Idle,
            RepeatDelay,
            Repeat,
            FastRepeat,
        };
        State state;
        uint32_t pressed_since;
        uint32_t last_repeat;
    };

    static const uint32_t repeat_delay = 1000;
    static const uint32_t repeat_interval = 20;
    static const uint32_t fast_repeat_delay = 5000;
    static const uint32_t fast_repeat_interval = 2;

    static ButtonState state_north = {ButtonState::State::Idle, 0, 0};
    static ButtonState state_east = {ButtonState::State::Idle, 0, 0};
    static ButtonState state_south = {ButtonState::State::Idle, 0, 0};
    static ButtonState state_west = {ButtonState::State::Idle, 0, 0};

    static ButtonState state_up = {ButtonState::State::Idle, 0, 0};
    static ButtonState state_down = {ButtonState::State::Idle, 0, 0};
    static ButtonState state_left = {ButtonState::State::Idle, 0, 0};
    static ButtonState state_right = {ButtonState::State::Idle, 0, 0};

    InputState::Controller result{{false, false, false, false},
                                  {false, false, false, false, false, false, false, false, false, false}};

    auto handle_button = [](ButtonState &button_state, bool input_state) {
        bool result = false;
        if (input_state) {
            uint32_t now = to_ms_since_boot(get_absolute_time());
            switch (button_state.state) {
            case ButtonState::State::Idle:
                result = true;
                button_state.state = ButtonState::State::RepeatDelay;
                button_state.pressed_since = now;
                break;
            case ButtonState::State::RepeatDelay:
                if ((now - button_state.pressed_since) > repeat_delay) {
                    result = true;
                    button_state.state = ButtonState::State::Repeat;
                    button_state.last_repeat = now;
                } else {
                    result = false;
                }
                break;
            case ButtonState::State::Repeat:
                if ((now - button_state.pressed_since) > fast_repeat_delay) {
                    result = true;
                    button_state.state = ButtonState::State::FastRepeat;
                    button_state.last_repeat = now;
                } else if ((now - button_state.last_repeat) > repeat_interval) {
                    result = true;
                    button_state.last_repeat = now;
                } else {
                    result = false;
                }
                break;
            case ButtonState::State::FastRepeat:
                if ((now - button_state.last_repeat) > fast_repeat_interval) {
                    result = true;
                    button_state.last_repeat = now;
                } else {
                    result = false;
                }
                break;
            }
        } else {
            result = false;
            button_state.state = ButtonState::State::Idle;
        }

        return result;
    };

    result.buttons.north = handle_button(state_north, controller_state.buttons.north);
    result.buttons.east = handle_button(state_east, controller_state.buttons.east);
    result.buttons.south = handle_button(state_south, controller_state.buttons.south);
    result.buttons.west = handle_button(state_west, controller_state.buttons.west);

    result.dpad.up = handle_button(state_up, controller_state.dpad.up);
    result.dpad.down = handle_button(state_down, controller_state.dpad.down);
    result.dpad.left = handle_button(state_left, controller_state.dpad.left);
    result.dpad.right = handle_button(state_right, controller_state.dpad.right);

    return result;
}

template <> uint16_t Menu<config_t::ThresholdsFourPanel>::getCurrentThresholdValue(MenuPage page) {
    switch (page) {
    case MenuPage::PadTriggerThresholdUp:
        return m_store->getTriggerThresholds().up;
    case MenuPage::PadTriggerThresholdLeft:
        return m_store->getTriggerThresholds().left;
    case MenuPage::PadTriggerThresholdRight:
        return m_store->getTriggerThresholds().right;
    case MenuPage::PadTriggerThresholdDown:
        return m_store->getTriggerThresholds().down;
    default:
        return 0;
    }
}

template <> uint16_t Menu<config_t::ThresholdsFivePanel>::getCurrentThresholdValue(MenuPage page) {
    switch (page) {
    case MenuPage::PadTriggerThresholdUpLeft:
        return m_store->getTriggerThresholds().up_left;
    case MenuPage::PadTriggerThresholdUpRight:
        return m_store->getTriggerThresholds().up_right;
    case MenuPage::PadTriggerThresholdCenter:
        return m_store->getTriggerThresholds().center;
    case MenuPage::PadTriggerThresholdDownLeft:
        return m_store->getTriggerThresholds().down_left;
    case MenuPage::PadTriggerThresholdDownRight:
        return m_store->getTriggerThresholds().down_right;
    default:
        return 0;
    }
}

template <> uint16_t Menu<config_t::ThresholdsSixPanel>::getCurrentThresholdValue(MenuPage page) {
    switch (page) {
    case MenuPage::PadTriggerThresholdUpLeft:
        return m_store->getTriggerThresholds().up_left;
    case MenuPage::PadTriggerThresholdUp:
        return m_store->getTriggerThresholds().up;
    case MenuPage::PadTriggerThresholdUpRight:
        return m_store->getTriggerThresholds().up_right;
    case MenuPage::PadTriggerThresholdLeft:
        return m_store->getTriggerThresholds().left;
    case MenuPage::PadTriggerThresholdRight:
        return m_store->getTriggerThresholds().right;
    case MenuPage::PadTriggerThresholdDown:
        return m_store->getTriggerThresholds().down;
    default:
        return 0;
    }
}

template <> uint16_t Menu<config_t::ThresholdsEightPanel>::getCurrentThresholdValue(MenuPage page) {
    switch (page) {
    case MenuPage::PadTriggerThresholdUpLeft:
        return m_store->getTriggerThresholds().up_left;
    case MenuPage::PadTriggerThresholdUp:
        return m_store->getTriggerThresholds().up;
    case MenuPage::PadTriggerThresholdUpRight:
        return m_store->getTriggerThresholds().up_right;
    case MenuPage::PadTriggerThresholdLeft:
        return m_store->getTriggerThresholds().left;
    case MenuPage::PadTriggerThresholdRight:
        return m_store->getTriggerThresholds().right;
    case MenuPage::PadTriggerThresholdDownLeft:
        return m_store->getTriggerThresholds().down_left;
    case MenuPage::PadTriggerThresholdDown:
        return m_store->getTriggerThresholds().down;
    case MenuPage::PadTriggerThresholdDownRight:
        return m_store->getTriggerThresholds().down_right;
    default:
        return 0;
    }
}

template <> uint16_t Menu<config_t::ThresholdsNinePanel>::getCurrentThresholdValue(MenuPage page) {
    switch (page) {
    case MenuPage::PadTriggerThresholdUpLeft:
        return m_store->getTriggerThresholds().up_left;
    case MenuPage::PadTriggerThresholdUp:
        return m_store->getTriggerThresholds().up;
    case MenuPage::PadTriggerThresholdUpRight:
        return m_store->getTriggerThresholds().up_right;
    case MenuPage::PadTriggerThresholdLeft:
        return m_store->getTriggerThresholds().left;
    case MenuPage::PadTriggerThresholdRight:
        return m_store->getTriggerThresholds().center;
    case MenuPage::PadTriggerThresholdCenter:
        return m_store->getTriggerThresholds().right;
    case MenuPage::PadTriggerThresholdDownLeft:
        return m_store->getTriggerThresholds().down_left;
    case MenuPage::PadTriggerThresholdDown:
        return m_store->getTriggerThresholds().down;
    case MenuPage::PadTriggerThresholdDownRight:
        return m_store->getTriggerThresholds().down_right;
    default:
        return 0;
    }
}

template <typename TThresholds> uint16_t Menu<TThresholds>::getCurrentValue(MenuPage page) {
    switch (page) {
    case MenuPage::DeviceMode:
        return static_cast<uint16_t>(m_store->getUsbMode());
    case MenuPage::PadDebounceDelay:
        return m_store->getDebounceDelay();
    case MenuPage::PadTriggerThresholdUpLeft:
    case MenuPage::PadTriggerThresholdUp:
    case MenuPage::PadTriggerThresholdUpRight:
    case MenuPage::PadTriggerThresholdLeft:
    case MenuPage::PadTriggerThresholdCenter:
    case MenuPage::PadTriggerThresholdRight:
    case MenuPage::PadTriggerThresholdDownLeft:
    case MenuPage::PadTriggerThresholdDown:
    case MenuPage::PadTriggerThresholdDownRight:
        return getCurrentThresholdValue(page);
    case MenuPage::LedBrightness:
        return m_store->getLedBrightness();
    case MenuPage::LedEnablePlayerColor:
        return static_cast<uint16_t>(m_store->getLedEnablePlayerColor());
    case MenuPage::Main:
    case MenuPage::Pad:
    case MenuPage::PadCalibrate:
    case MenuPage::PadTriggerThresholds:
    case MenuPage::Led:
    case MenuPage::Reset:
    case MenuPage::Bootsel:
    case MenuPage::BootselMsg:
        break;
    }

    return 0;
}

template <typename TThresholds> void Menu<TThresholds>::gotoPage(MenuPage page) {
    const auto current_value = getCurrentValue(page);

    m_state_stack.push({page, current_value, current_value});
}

template <> void Menu<config_t::ThresholdsFourPanel>::restoreThresholdValue() {
    const auto current_state = m_state_stack.top();
    auto thresholds = m_store->getTriggerThresholds();

    switch (current_state.page) {
    case MenuPage::PadTriggerThresholdUp:
        thresholds.up = current_state.original_value;
        break;
    case MenuPage::PadTriggerThresholdLeft:
        thresholds.left = current_state.original_value;
        break;
    case MenuPage::PadTriggerThresholdRight:
        thresholds.right = current_state.original_value;
        break;
    case MenuPage::PadTriggerThresholdDown:
        thresholds.down = current_state.original_value;
        break;
    default:
        break;
    }

    m_store->setTriggerThresholds(thresholds);
}

template <> void Menu<config_t::ThresholdsFivePanel>::restoreThresholdValue() {
    const auto current_state = m_state_stack.top();
    auto thresholds = m_store->getTriggerThresholds();

    switch (current_state.page) {
    case MenuPage::PadTriggerThresholdUpLeft:
        thresholds.up_left = current_state.original_value;
        break;
    case MenuPage::PadTriggerThresholdUpRight:
        thresholds.up_right = current_state.original_value;
        break;
    case MenuPage::PadTriggerThresholdCenter:
        thresholds.center = current_state.original_value;
        break;
    case MenuPage::PadTriggerThresholdDownLeft:
        thresholds.down_left = current_state.original_value;
        break;
    case MenuPage::PadTriggerThresholdDownRight:
        thresholds.down_right = current_state.original_value;
        break;
    default:
        break;
    }

    m_store->setTriggerThresholds(thresholds);
}

template <> void Menu<config_t::ThresholdsSixPanel>::restoreThresholdValue() {
    const auto current_state = m_state_stack.top();
    auto thresholds = m_store->getTriggerThresholds();

    switch (current_state.page) {
    case MenuPage::PadTriggerThresholdUpLeft:
        thresholds.up_left = current_state.original_value;
        break;
    case MenuPage::PadTriggerThresholdUp:
        thresholds.up = current_state.original_value;
        break;
    case MenuPage::PadTriggerThresholdUpRight:
        thresholds.up_right = current_state.original_value;
        break;
    case MenuPage::PadTriggerThresholdLeft:
        thresholds.left = current_state.original_value;
        break;
    case MenuPage::PadTriggerThresholdRight:
        thresholds.right = current_state.original_value;
        break;
    case MenuPage::PadTriggerThresholdDown:
        thresholds.down = current_state.original_value;
        break;
    default:
        break;
    }

    m_store->setTriggerThresholds(thresholds);
}

template <> void Menu<config_t::ThresholdsEightPanel>::restoreThresholdValue() {
    const auto current_state = m_state_stack.top();
    auto thresholds = m_store->getTriggerThresholds();

    switch (current_state.page) {
    case MenuPage::PadTriggerThresholdUpLeft:
        thresholds.up_left = current_state.original_value;
        break;
    case MenuPage::PadTriggerThresholdUp:
        thresholds.up = current_state.original_value;
        break;
    case MenuPage::PadTriggerThresholdUpRight:
        thresholds.up_right = current_state.original_value;
        break;
    case MenuPage::PadTriggerThresholdLeft:
        thresholds.left = current_state.original_value;
        break;
    case MenuPage::PadTriggerThresholdRight:
        thresholds.right = current_state.original_value;
        break;
    case MenuPage::PadTriggerThresholdDownLeft:
        thresholds.down_left = current_state.original_value;
        break;
    case MenuPage::PadTriggerThresholdDown:
        thresholds.down = current_state.original_value;
        break;
    case MenuPage::PadTriggerThresholdDownRight:
        thresholds.down_right = current_state.original_value;
        break;
    default:
        break;
    }

    m_store->setTriggerThresholds(thresholds);
}

template <> void Menu<config_t::ThresholdsNinePanel>::restoreThresholdValue() {
    const auto current_state = m_state_stack.top();
    auto thresholds = m_store->getTriggerThresholds();

    switch (current_state.page) {
    case MenuPage::PadTriggerThresholdUpLeft:
        thresholds.up_left = current_state.original_value;
        break;
    case MenuPage::PadTriggerThresholdUp:
        thresholds.up = current_state.original_value;
        break;
    case MenuPage::PadTriggerThresholdUpRight:
        thresholds.up_right = current_state.original_value;
        break;
    case MenuPage::PadTriggerThresholdLeft:
        thresholds.left = current_state.original_value;
        break;
    case MenuPage::PadTriggerThresholdCenter:
        thresholds.center = current_state.original_value;
        break;
    case MenuPage::PadTriggerThresholdRight:
        thresholds.right = current_state.original_value;
        break;
    case MenuPage::PadTriggerThresholdDownLeft:
        thresholds.down_left = current_state.original_value;
        break;
    case MenuPage::PadTriggerThresholdDown:
        thresholds.down = current_state.original_value;
        break;
    case MenuPage::PadTriggerThresholdDownRight:
        thresholds.down_right = current_state.original_value;
        break;
    default:
        break;
    }

    m_store->setTriggerThresholds(thresholds);
}

template <typename TThresholds> void Menu<TThresholds>::gotoParent(bool do_restore) {
    const auto current_state = m_state_stack.top();

    if (current_state.page == MenuPage::Main) {
        m_active = false;
    }

    if (do_restore) {
        switch (current_state.page) {
        case MenuPage::DeviceMode:
            m_store->setUsbMode(static_cast<usb_mode_t>(current_state.original_value));
            break;
        case MenuPage::PadDebounceDelay:
            m_store->setDebounceDelay(current_state.original_value);
            break;
        case MenuPage::PadTriggerThresholdUpLeft:
        case MenuPage::PadTriggerThresholdUp:
        case MenuPage::PadTriggerThresholdUpRight:
        case MenuPage::PadTriggerThresholdLeft:
        case MenuPage::PadTriggerThresholdCenter:
        case MenuPage::PadTriggerThresholdRight:
        case MenuPage::PadTriggerThresholdDownLeft:
        case MenuPage::PadTriggerThresholdDown:
        case MenuPage::PadTriggerThresholdDownRight:
            restoreThresholdValue();
            break;
        case MenuPage::LedBrightness:
            m_store->setLedBrightness(current_state.original_value);
            break;
        case MenuPage::LedEnablePlayerColor:
            m_store->setLedEnablePlayerColor(static_cast<bool>(current_state.original_value));
            break;
        case MenuPage::Main:
        case MenuPage::Pad:
        case MenuPage::PadCalibrate:
        case MenuPage::PadTriggerThresholds:
        case MenuPage::Led:
        case MenuPage::Reset:
        case MenuPage::Bootsel:
        case MenuPage::BootselMsg:
            break;
        }
    }

    m_state_stack.pop();
}

template <>
void Menu<config_t::ThresholdsFourPanel>::performActionSetThresholdValue(MenuDescriptor::Action action,
                                                                         uint16_t value) {
    auto thresholds = m_store->getTriggerThresholds();

    switch (action) {
    case MenuDescriptor::Action::SetPadTriggerThresholdUp:
        thresholds.up = value;
        break;
    case MenuDescriptor::Action::SetPadTriggerThresholdLeft:
        thresholds.left = value;
        break;
    case MenuDescriptor::Action::SetPadTriggerThresholdRight:
        thresholds.right = value;
        break;
    case MenuDescriptor::Action::SetPadTriggerThresholdDown:
        thresholds.down = value;
        break;
    default:
        break;
    }

    m_store->setTriggerThresholds(thresholds);
}

template <>
void Menu<config_t::ThresholdsFivePanel>::performActionSetThresholdValue(MenuDescriptor::Action action,
                                                                         uint16_t value) {
    auto thresholds = m_store->getTriggerThresholds();

    switch (action) {
    case MenuDescriptor::Action::SetPadTriggerThresholdUpLeft:
        thresholds.up_left = value;
        break;
    case MenuDescriptor::Action::SetPadTriggerThresholdUpRight:
        thresholds.up_right = value;
        break;
    case MenuDescriptor::Action::SetPadTriggerThresholdCenter:
        thresholds.center = value;
        break;
    case MenuDescriptor::Action::SetPadTriggerThresholdDownLeft:
        thresholds.down_left = value;
        break;
    case MenuDescriptor::Action::SetPadTriggerThresholdDownRight:
        thresholds.down_right = value;
        break;
    default:
        break;
    }

    m_store->setTriggerThresholds(thresholds);
}

template <>
void Menu<config_t::ThresholdsSixPanel>::performActionSetThresholdValue(MenuDescriptor::Action action, uint16_t value) {
    auto thresholds = m_store->getTriggerThresholds();

    switch (action) {
    case MenuDescriptor::Action::SetPadTriggerThresholdUpLeft:
        thresholds.up_left = value;
        break;
    case MenuDescriptor::Action::SetPadTriggerThresholdUp:
        thresholds.up = value;
        break;
    case MenuDescriptor::Action::SetPadTriggerThresholdUpRight:
        thresholds.up_right = value;
        break;
    case MenuDescriptor::Action::SetPadTriggerThresholdLeft:
        thresholds.left = value;
        break;
    case MenuDescriptor::Action::SetPadTriggerThresholdRight:
        thresholds.right = value;
        break;
    case MenuDescriptor::Action::SetPadTriggerThresholdDown:
        thresholds.down = value;
        break;
    default:
        break;
    }

    m_store->setTriggerThresholds(thresholds);
}

template <>
void Menu<config_t::ThresholdsEightPanel>::performActionSetThresholdValue(MenuDescriptor::Action action,
                                                                          uint16_t value) {
    auto thresholds = m_store->getTriggerThresholds();

    switch (action) {
    case MenuDescriptor::Action::SetPadTriggerThresholdUpLeft:
        thresholds.up_left = value;
        break;
    case MenuDescriptor::Action::SetPadTriggerThresholdUp:
        thresholds.up = value;
        break;
    case MenuDescriptor::Action::SetPadTriggerThresholdUpRight:
        thresholds.up_right = value;
        break;
    case MenuDescriptor::Action::SetPadTriggerThresholdLeft:
        thresholds.left = value;
        break;
    case MenuDescriptor::Action::SetPadTriggerThresholdRight:
        thresholds.right = value;
        break;
    case MenuDescriptor::Action::SetPadTriggerThresholdDownLeft:
        thresholds.down_left = value;
        break;
    case MenuDescriptor::Action::SetPadTriggerThresholdDown:
        thresholds.down = value;
        break;
    case MenuDescriptor::Action::SetPadTriggerThresholdDownRight:
        thresholds.down_right = value;
        break;
    default:
        break;
    }

    m_store->setTriggerThresholds(thresholds);
}

template <>
void Menu<config_t::ThresholdsNinePanel>::performActionSetThresholdValue(MenuDescriptor::Action action,
                                                                         uint16_t value) {
    auto thresholds = m_store->getTriggerThresholds();

    switch (action) {
    case MenuDescriptor::Action::SetPadTriggerThresholdUpLeft:
        thresholds.up_left = value;
        break;
    case MenuDescriptor::Action::SetPadTriggerThresholdUp:
        thresholds.up = value;
        break;
    case MenuDescriptor::Action::SetPadTriggerThresholdUpRight:
        thresholds.up_right = value;
        break;
    case MenuDescriptor::Action::SetPadTriggerThresholdLeft:
        thresholds.left = value;
        break;
    case MenuDescriptor::Action::SetPadTriggerThresholdCenter:
        thresholds.center = value;
        break;
    case MenuDescriptor::Action::SetPadTriggerThresholdRight:
        thresholds.right = value;
        break;
    case MenuDescriptor::Action::SetPadTriggerThresholdDownLeft:
        thresholds.down_left = value;
        break;
    case MenuDescriptor::Action::SetPadTriggerThresholdDown:
        thresholds.down = value;
        break;
    case MenuDescriptor::Action::SetPadTriggerThresholdDownRight:
        thresholds.down_right = value;
        break;
    default:
        break;
    }

    m_store->setTriggerThresholds(thresholds);
}

template <typename TThresholds> void Menu<TThresholds>::performAction(MenuDescriptor::Action action, uint16_t value) {
    switch (action) {
    case MenuDescriptor::Action::None:
        break;
    case MenuDescriptor::Action::GotoParent:
        gotoParent(false);
        break;
    case MenuDescriptor::Action::GotoPageDeviceMode:
        gotoPage(MenuPage::DeviceMode);
        break;
    case MenuDescriptor::Action::GotoPagePad:
        gotoPage(MenuPage::Pad);
        break;
    case MenuDescriptor::Action::GotoPageLed:
        gotoPage(MenuPage::Led);
        break;
    case MenuDescriptor::Action::GotoPageReset:
        gotoPage(MenuPage::Reset);
        break;
    case MenuDescriptor::Action::GotoPageBootsel:
        gotoPage(MenuPage::Bootsel);
        break;
    case MenuDescriptor::Action::GotoPagePadDebounceDelay:
        gotoPage(MenuPage::PadDebounceDelay);
        break;
    case MenuDescriptor::Action::GotoPageTriggerThresholds:
        gotoPage(MenuPage::PadTriggerThresholds);
        break;
    case MenuDescriptor::Action::GotoPagePadTriggerThresholdUpLeft:
        gotoPage(MenuPage::PadTriggerThresholdUpLeft);
        break;
    case MenuDescriptor::Action::GotoPagePadTriggerThresholdUp:
        gotoPage(MenuPage::PadTriggerThresholdUp);
        break;
    case MenuDescriptor::Action::GotoPagePadTriggerThresholdUpRight:
        gotoPage(MenuPage::PadTriggerThresholdUpRight);
        break;
    case MenuDescriptor::Action::GotoPagePadTriggerThresholdLeft:
        gotoPage(MenuPage::PadTriggerThresholdLeft);
        break;
    case MenuDescriptor::Action::GotoPagePadTriggerThresholdCenter:
        gotoPage(MenuPage::PadTriggerThresholdCenter);
        break;
    case MenuDescriptor::Action::GotoPagePadTriggerThresholdRight:
        gotoPage(MenuPage::PadTriggerThresholdRight);
        break;
    case MenuDescriptor::Action::GotoPagePadTriggerThresholdDownLeft:
        gotoPage(MenuPage::PadTriggerThresholdDownLeft);
        break;
    case MenuDescriptor::Action::GotoPagePadTriggerThresholdDown:
        gotoPage(MenuPage::PadTriggerThresholdDown);
        break;
    case MenuDescriptor::Action::GotoPagePadTriggerThresholdDownRight:
        gotoPage(MenuPage::PadTriggerThresholdDownRight);
        break;
    case MenuDescriptor::Action::GotoPageLedBrightness:
        gotoPage(MenuPage::LedBrightness);
        break;
    case MenuDescriptor::Action::GotoPageLedEnablePlayerColor:
        gotoPage(MenuPage::LedEnablePlayerColor);
        break;
    case MenuDescriptor::Action::SetUsbMode:
        m_store->setUsbMode(static_cast<usb_mode_t>(value));
        break;
    case MenuDescriptor::Action::SetPadDebounceDelay:
        m_store->setDebounceDelay(value);
        break;
    case MenuDescriptor::Action::SetPadTriggerThresholdUpLeft:
    case MenuDescriptor::Action::SetPadTriggerThresholdUp:
    case MenuDescriptor::Action::SetPadTriggerThresholdUpRight:
    case MenuDescriptor::Action::SetPadTriggerThresholdLeft:
    case MenuDescriptor::Action::SetPadTriggerThresholdCenter:
    case MenuDescriptor::Action::SetPadTriggerThresholdRight:
    case MenuDescriptor::Action::SetPadTriggerThresholdDownLeft:
    case MenuDescriptor::Action::SetPadTriggerThresholdDown:
    case MenuDescriptor::Action::SetPadTriggerThresholdDownRight:
        performActionSetThresholdValue(action, value);
        break;
    case MenuDescriptor::Action::SetLedBrightness:
        m_store->setLedBrightness(value);
        break;
    case MenuDescriptor::Action::SetLedEnablePlayerColor:
        m_store->setLedEnablePlayerColor(static_cast<bool>(value));
        break;
    case MenuDescriptor::Action::DoCalibrate:
        // TODO;
        break;
    case MenuDescriptor::Action::DoReset:
        m_store->reset();
        break;
    case MenuDescriptor::Action::DoRebootToBootsel:
        m_store->scheduleReboot(true);
        gotoPage(MenuPage::BootselMsg);
        break;
    }

    return;
}

template <typename TThresholds> void Menu<TThresholds>::update(const InputState::Controller &controller_state) {
    InputState::Controller pressed = checkPressed(controller_state);
    auto &current_state = m_state_stack.top();

    auto descriptor_it = descriptors.find(current_state.page);
    if (descriptor_it == descriptors.end()) {
        assert(false);
        return;
    }

    if (descriptor_it->second.type == MenuDescriptor::Type::RebootInfo) {
        m_active = false;
    } else if (pressed.dpad.left) {
        switch (descriptor_it->second.type) {
        case MenuDescriptor::Type::Toggle:
            current_state.selected_value = !current_state.selected_value;
            performAction(descriptor_it->second.items.at(0).second, current_state.selected_value);
            break;
        case MenuDescriptor::Type::Selection:
            if (current_state.selected_value == 0) {
                current_state.selected_value = descriptor_it->second.items.size() - 1;
            } else {
                current_state.selected_value--;
            }
            performAction(descriptor_it->second.items.at(current_state.selected_value).second,
                          current_state.selected_value);
            break;
        case MenuDescriptor::Type::Menu:
            if (current_state.selected_value == 0) {
                current_state.selected_value = descriptor_it->second.items.size() - 1;
            } else {
                current_state.selected_value--;
            }
            break;
        case MenuDescriptor::Type::Value:
        case MenuDescriptor::Type::RebootInfo:
            break;
        }
    } else if (pressed.dpad.right) {
        switch (descriptor_it->second.type) {
        case MenuDescriptor::Type::Toggle:
            current_state.selected_value = !current_state.selected_value;
            performAction(descriptor_it->second.items.at(0).second, current_state.selected_value);
            break;
        case MenuDescriptor::Type::Selection:
            if (current_state.selected_value == descriptor_it->second.items.size() - 1) {
                current_state.selected_value = 0;
            } else {
                current_state.selected_value++;
            }
            performAction(descriptor_it->second.items.at(current_state.selected_value).second,
                          current_state.selected_value);
            break;
        case MenuDescriptor::Type::Menu:
            if (current_state.selected_value == descriptor_it->second.items.size() - 1) {
                current_state.selected_value = 0;
            } else {
                current_state.selected_value++;
            }
            break;
        case MenuDescriptor::Type::Value:
        case MenuDescriptor::Type::RebootInfo:
            break;
        }
    } else if (pressed.dpad.up) {
        switch (descriptor_it->second.type) {
        case MenuDescriptor::Type::Value:
            if (current_state.selected_value < descriptor_it->second.max_value) {
                current_state.selected_value++;
                performAction(descriptor_it->second.items.at(0).second, current_state.selected_value);
            }
            break;
        case MenuDescriptor::Type::Toggle:
        case MenuDescriptor::Type::Selection:
        case MenuDescriptor::Type::Menu:
        case MenuDescriptor::Type::RebootInfo:
            break;
        }
    } else if (pressed.dpad.down) {
        switch (descriptor_it->second.type) {
        case MenuDescriptor::Type::Value:
            if (current_state.selected_value > 0) {
                current_state.selected_value--;
                performAction(descriptor_it->second.items.at(0).second, current_state.selected_value);
            }
            break;
        case MenuDescriptor::Type::Toggle:
        case MenuDescriptor::Type::Selection:
        case MenuDescriptor::Type::Menu:
        case MenuDescriptor::Type::RebootInfo:
            break;
        }
    } else if (pressed.buttons.south) { // Back/Exit
        switch (descriptor_it->second.type) {
        case MenuDescriptor::Type::Value:
        case MenuDescriptor::Type::Toggle:
        case MenuDescriptor::Type::Selection:
            gotoParent(true);
            break;
        case MenuDescriptor::Type::Menu:
            gotoParent(false);
            break;
        case MenuDescriptor::Type::RebootInfo:
            break;
        }
    } else if (pressed.buttons.east) { // Select
        switch (descriptor_it->second.type) {
        case MenuDescriptor::Type::Value:
        case MenuDescriptor::Type::Toggle:
        case MenuDescriptor::Type::Selection:
            gotoParent(false);
            break;
        case MenuDescriptor::Type::Menu:
            performAction(descriptor_it->second.items.at(current_state.selected_value).second,
                          current_state.selected_value);
            break;
        case MenuDescriptor::Type::RebootInfo:
            break;
        }
    }
}

template <typename TThresholds> bool Menu<TThresholds>::active() { return m_active; }

template <typename TThresholds> MenuState Menu<TThresholds>::getState() { return m_state_stack.top(); }

} // namespace Dancecon::Utils
