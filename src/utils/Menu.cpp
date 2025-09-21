#include "utils/Menu.h"

#include "GlobalConfiguration.h"
#include "peripherals/Pad.h"

namespace Dancecon::Utils {

using config_t = decltype(Config::Default::pad_config);

template class Menu<Config::Default::pad_config.PANEL_COUNT, Config::Default::led_config.PANEL_COUNT>;

namespace {

template <size_t TPanelLedsCount> const MenuDescriptor thresholds_descriptor = {};
template <>
const MenuDescriptor thresholds_descriptor<4> = {                         //
    MenuDescriptor::Type::Menu,                                           //
    "Thresholds",                                                         //
    {{"Up", MenuDescriptor::Action::GotoPagePadTriggerThresholdUp},       //
     {"Left", MenuDescriptor::Action::GotoPagePadTriggerThresholdLeft},   //
     {"Right", MenuDescriptor::Action::GotoPagePadTriggerThresholdRight}, //
     {"Down", MenuDescriptor::Action::GotoPagePadTriggerThresholdDown}},  //
    0};
template <>
const MenuDescriptor thresholds_descriptor<5> = {                                  //
    MenuDescriptor::Type::Menu,                                                    //
    "Thresholds",                                                                  //
    {{"Up Left", MenuDescriptor::Action::GotoPagePadTriggerThresholdUpLeft},       //
     {"Up Right", MenuDescriptor::Action::GotoPagePadTriggerThresholdUpRight},     //
     {"Center", MenuDescriptor::Action::GotoPagePadTriggerThresholdCenter},        //
     {"Down Left", MenuDescriptor::Action::GotoPagePadTriggerThresholdDownRight},  //
     {"Down Right", MenuDescriptor::Action::GotoPagePadTriggerThresholdDownLeft}}, //
    0};
template <>
const MenuDescriptor thresholds_descriptor<6> = {                              //
    MenuDescriptor::Type::Menu,                                                //
    "Thresholds",                                                              //
    {{"Up Left", MenuDescriptor::Action::GotoPagePadTriggerThresholdUpLeft},   //
     {"Up", MenuDescriptor::Action::GotoPagePadTriggerThresholdUp},            //
     {"Up Right", MenuDescriptor::Action::GotoPagePadTriggerThresholdUpRight}, //
     {"Left", MenuDescriptor::Action::GotoPagePadTriggerThresholdLeft},        //
     {"Right", MenuDescriptor::Action::GotoPagePadTriggerThresholdRight},      //
     {"Down", MenuDescriptor::Action::GotoPagePadTriggerThresholdDown}},       //
    0};

template <>
const MenuDescriptor thresholds_descriptor<8> = {                                  //
    MenuDescriptor::Type::Menu,                                                    //
    "Thresholds",                                                                  //
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
const MenuDescriptor thresholds_descriptor<9> = {                                  //
    MenuDescriptor::Type::Menu,                                                    //
    "Thresholds",                                                                  //
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

template <size_t TPanelCount> const MenuDescriptor idle_colors_descriptor = {};
template <>
const MenuDescriptor idle_colors_descriptor<4> = {                 //
    MenuDescriptor::Type::Menu,                                    //
    "Idle Colors",                                                 //
    {{"Up", MenuDescriptor::Action::GotoPageLedIdleColorUp},       //
     {"Left", MenuDescriptor::Action::GotoPageLedIdleColorLeft},   //
     {"Right", MenuDescriptor::Action::GotoPageLedIdleColorRight}, //
     {"Down", MenuDescriptor::Action::GotoPageLedIdleColorDown}},  //
    0};
template <>
const MenuDescriptor idle_colors_descriptor<5> = {                          //
    MenuDescriptor::Type::Menu,                                             //
    "Idle Colors",                                                          //
    {{"Up Left", MenuDescriptor::Action::GotoPageLedIdleColorUpLeft},       //
     {"Up Right", MenuDescriptor::Action::GotoPageLedIdleColorUpRight},     //
     {"Center", MenuDescriptor::Action::GotoPageLedIdleColorCenter},        //
     {"Down Left", MenuDescriptor::Action::GotoPageLedIdleColorDownRight},  //
     {"Down Right", MenuDescriptor::Action::GotoPageLedIdleColorDownLeft}}, //
    0};
template <>
const MenuDescriptor idle_colors_descriptor<6> = {                      //
    MenuDescriptor::Type::Menu,                                         //
    "Idle Colors",                                                      //
    {{"Up Left", MenuDescriptor::Action::GotoPageLedIdleColorUpLeft},   //
     {"Up", MenuDescriptor::Action::GotoPageLedIdleColorUp},            //
     {"Up Right", MenuDescriptor::Action::GotoPageLedIdleColorUpRight}, //
     {"Left", MenuDescriptor::Action::GotoPageLedIdleColorLeft},        //
     {"Right", MenuDescriptor::Action::GotoPageLedIdleColorRight},      //
     {"Down", MenuDescriptor::Action::GotoPageLedIdleColorDown}},       //
    0};
template <>
const MenuDescriptor idle_colors_descriptor<8> = {                          //
    MenuDescriptor::Type::Menu,                                             //
    "Idle Colors",                                                          //
    {{"Up Left", MenuDescriptor::Action::GotoPageLedIdleColorUpLeft},       //
     {"Up", MenuDescriptor::Action::GotoPageLedIdleColorUp},                //
     {"Up Right", MenuDescriptor::Action::GotoPageLedIdleColorUpRight},     //
     {"Left", MenuDescriptor::Action::GotoPageLedIdleColorLeft},            //
     {"Right", MenuDescriptor::Action::GotoPageLedIdleColorRight},          //
     {"Down Left", MenuDescriptor::Action::GotoPageLedIdleColorDownRight},  //
     {"Down", MenuDescriptor::Action::GotoPageLedIdleColorDown},            //
     {"Down Right", MenuDescriptor::Action::GotoPageLedIdleColorDownLeft}}, //
    0};
template <>
const MenuDescriptor idle_colors_descriptor<9> = {                          //
    MenuDescriptor::Type::Menu,                                             //
    "Idle Colors",                                                          //
    {{"Up Left", MenuDescriptor::Action::GotoPageLedIdleColorUpLeft},       //
     {"Up", MenuDescriptor::Action::GotoPageLedIdleColorUp},                //
     {"Up Right", MenuDescriptor::Action::GotoPageLedIdleColorUpRight},     //
     {"Left", MenuDescriptor::Action::GotoPageLedIdleColorLeft},            //
     {"Center", MenuDescriptor::Action::GotoPageLedIdleColorCenter},        //
     {"Right", MenuDescriptor::Action::GotoPageLedIdleColorRight},          //
     {"Down Left", MenuDescriptor::Action::GotoPageLedIdleColorDownRight},  //
     {"Down", MenuDescriptor::Action::GotoPageLedIdleColorDown},            //
     {"Down Right", MenuDescriptor::Action::GotoPageLedIdleColorDownLeft}}, //
    0};

template <size_t TPanelCount> const MenuDescriptor active_colors_descriptor = {};
template <>
const MenuDescriptor active_colors_descriptor<4> = {                 //
    MenuDescriptor::Type::Menu,                                      //
    "Active Colors",                                                 //
    {{"Up", MenuDescriptor::Action::GotoPageLedActiveColorUp},       //
     {"Left", MenuDescriptor::Action::GotoPageLedActiveColorLeft},   //
     {"Right", MenuDescriptor::Action::GotoPageLedActiveColorRight}, //
     {"Down", MenuDescriptor::Action::GotoPageLedActiveColorDown}},  //
    0};
template <>
const MenuDescriptor active_colors_descriptor<5> = {                          //
    MenuDescriptor::Type::Menu,                                               //
    "Active Colors",                                                          //
    {{"Up Left", MenuDescriptor::Action::GotoPageLedActiveColorUpLeft},       //
     {"Up Right", MenuDescriptor::Action::GotoPageLedActiveColorUpRight},     //
     {"Center", MenuDescriptor::Action::GotoPageLedActiveColorCenter},        //
     {"Down Left", MenuDescriptor::Action::GotoPageLedActiveColorDownRight},  //
     {"Down Right", MenuDescriptor::Action::GotoPageLedActiveColorDownLeft}}, //
    0};
template <>
const MenuDescriptor active_colors_descriptor<6> = {                      //
    MenuDescriptor::Type::Menu,                                           //
    "Active Colors",                                                      //
    {{"Up Left", MenuDescriptor::Action::GotoPageLedActiveColorUpLeft},   //
     {"Up", MenuDescriptor::Action::GotoPageLedActiveColorUp},            //
     {"Up Right", MenuDescriptor::Action::GotoPageLedActiveColorUpRight}, //
     {"Left", MenuDescriptor::Action::GotoPageLedActiveColorLeft},        //
     {"Right", MenuDescriptor::Action::GotoPageLedActiveColorRight},      //
     {"Down", MenuDescriptor::Action::GotoPageLedActiveColorDown}},       //
    0};
template <>
const MenuDescriptor active_colors_descriptor<8> = {                          //
    MenuDescriptor::Type::Menu,                                               //
    "Active Colors",                                                          //
    {{"Up Left", MenuDescriptor::Action::GotoPageLedActiveColorUpLeft},       //
     {"Up", MenuDescriptor::Action::GotoPageLedActiveColorUp},                //
     {"Up Right", MenuDescriptor::Action::GotoPageLedActiveColorUpRight},     //
     {"Left", MenuDescriptor::Action::GotoPageLedActiveColorLeft},            //
     {"Right", MenuDescriptor::Action::GotoPageLedActiveColorRight},          //
     {"Down Left", MenuDescriptor::Action::GotoPageLedActiveColorDownRight},  //
     {"Down", MenuDescriptor::Action::GotoPageLedActiveColorDown},            //
     {"Down Right", MenuDescriptor::Action::GotoPageLedActiveColorDownLeft}}, //
    0};
template <>
const MenuDescriptor active_colors_descriptor<9> = {                          //
    MenuDescriptor::Type::Menu,                                               //
    "Active Colors",                                                          //
    {{"Up Left", MenuDescriptor::Action::GotoPageLedActiveColorUpLeft},       //
     {"Up", MenuDescriptor::Action::GotoPageLedActiveColorUp},                //
     {"Up Right", MenuDescriptor::Action::GotoPageLedActiveColorUpRight},     //
     {"Left", MenuDescriptor::Action::GotoPageLedActiveColorLeft},            //
     {"Center", MenuDescriptor::Action::GotoPageLedActiveColorCenter},        //
     {"Right", MenuDescriptor::Action::GotoPageLedActiveColorRight},          //
     {"Down Left", MenuDescriptor::Action::GotoPageLedActiveColorDownRight},  //
     {"Down", MenuDescriptor::Action::GotoPageLedActiveColorDown},            //
     {"Down Right", MenuDescriptor::Action::GotoPageLedActiveColorDownLeft}}, //
    0};

template <size_t TPanelCount>
uint16_t getThresholdValue(const MenuPage page,
                           const typename Peripherals::Pad<TPanelCount>::Config::Thresholds &thresholds) {
    if constexpr (TPanelCount == 4 || TPanelCount == 6 || TPanelCount == 8 || TPanelCount == 9) {
        switch (page) {
        case MenuPage::PadTriggerThresholdUp:
            return thresholds.up;
        case MenuPage::PadTriggerThresholdLeft:
            return thresholds.left;
        case MenuPage::PadTriggerThresholdRight:
            return thresholds.right;
        case MenuPage::PadTriggerThresholdDown:
            return thresholds.down;
        default:
            break;
        }
    }
    if constexpr (TPanelCount == 5 || TPanelCount == 6 || TPanelCount == 8 || TPanelCount == 9) {
        switch (page) {
        case MenuPage::PadTriggerThresholdUpLeft:
            return thresholds.up_left;
        case MenuPage::PadTriggerThresholdUpRight:
            return thresholds.up_right;
        default:
            break;
        }
    }
    if constexpr (TPanelCount == 5 || TPanelCount == 8 || TPanelCount == 9) {
        switch (page) {
        case MenuPage::PadTriggerThresholdDownLeft:
            return thresholds.down_left;
        case MenuPage::PadTriggerThresholdDownRight:
            return thresholds.down_right;
        default:
            break;
        }
    }
    if constexpr (TPanelCount == 5 || TPanelCount == 9) {
        switch (page) {
        case MenuPage::PadTriggerThresholdCenter:
            return thresholds.center;
        default:
            break;
        }
    }

    return 0;
}

template <size_t TPanelLedsCount>
uint8_t getLedColorValue(const MenuPage page,
                         const typename Peripherals::PanelLeds<TPanelLedsCount>::Config::PanelColors &colors) {
    if constexpr (TPanelLedsCount == 4 || TPanelLedsCount == 6 || TPanelLedsCount == 8 || TPanelLedsCount == 9) {
        switch (page) {
        case MenuPage::LedIdleColorUpRed:
        case MenuPage::LedActiveColorUpRed:
            return colors.up.r;
        case MenuPage::LedIdleColorUpGreen:
        case MenuPage::LedActiveColorUpGreen:
            return colors.up.g;
        case MenuPage::LedIdleColorUpBlue:
        case MenuPage::LedActiveColorUpBlue:
            return colors.up.b;
        case MenuPage::LedIdleColorLeftRed:
        case MenuPage::LedActiveColorLeftRed:
            return colors.left.r;
        case MenuPage::LedIdleColorLeftGreen:
        case MenuPage::LedActiveColorLeftGreen:
            return colors.left.g;
        case MenuPage::LedIdleColorLeftBlue:
        case MenuPage::LedActiveColorLeftBlue:
            return colors.left.b;
        case MenuPage::LedIdleColorRightRed:
        case MenuPage::LedActiveColorRightRed:
            return colors.right.r;
        case MenuPage::LedIdleColorRightGreen:
        case MenuPage::LedActiveColorRightGreen:
            return colors.right.g;
        case MenuPage::LedIdleColorRightBlue:
        case MenuPage::LedActiveColorRightBlue:
            return colors.right.b;
        case MenuPage::LedIdleColorDownRed:
        case MenuPage::LedActiveColorDownRed:
            return colors.down.r;
        case MenuPage::LedIdleColorDownGreen:
        case MenuPage::LedActiveColorDownGreen:
            return colors.down.g;
        case MenuPage::LedIdleColorDownBlue:
        case MenuPage::LedActiveColorDownBlue:
            return colors.down.b;
        default:
            break;
        }
    }
    if constexpr (TPanelLedsCount == 5 || TPanelLedsCount == 6 || TPanelLedsCount == 8 || TPanelLedsCount == 9) {
        switch (page) {
        case MenuPage::LedIdleColorUpLeftRed:
        case MenuPage::LedActiveColorUpLeftRed:
            return colors.up_left.r;
        case MenuPage::LedIdleColorUpLeftGreen:
        case MenuPage::LedActiveColorUpLeftGreen:
            return colors.up_left.g;
        case MenuPage::LedIdleColorUpLeftBlue:
        case MenuPage::LedActiveColorUpLeftBlue:
            return colors.up_left.b;
        case MenuPage::LedIdleColorUpRightRed:
        case MenuPage::LedActiveColorUpRightRed:
            return colors.up_right.r;
        case MenuPage::LedIdleColorUpRightGreen:
        case MenuPage::LedActiveColorUpRightGreen:
            return colors.up_right.g;
        case MenuPage::LedIdleColorUpRightBlue:
        case MenuPage::LedActiveColorUpRightBlue:
            return colors.up_right.b;
        default:
            break;
        }
    }
    if constexpr (TPanelLedsCount == 5 || TPanelLedsCount == 8 || TPanelLedsCount == 9) {
        switch (page) {
        case MenuPage::LedIdleColorDownLeftRed:
        case MenuPage::LedActiveColorDownLeftRed:
            return colors.down_left.r;
        case MenuPage::LedIdleColorDownLeftGreen:
        case MenuPage::LedActiveColorDownLeftGreen:
            return colors.down_left.g;
        case MenuPage::LedIdleColorDownLeftBlue:
        case MenuPage::LedActiveColorDownLeftBlue:
            return colors.down_left.b;
        case MenuPage::LedIdleColorDownRightRed:
        case MenuPage::LedActiveColorDownRightRed:
            return colors.down_right.r;
        case MenuPage::LedIdleColorDownRightGreen:
        case MenuPage::LedActiveColorDownRightGreen:
            return colors.down_right.g;
        case MenuPage::LedIdleColorDownRightBlue:
        case MenuPage::LedActiveColorDownRightBlue:
            return colors.down_right.b;
        default:
            break;
        }
    }
    if constexpr (TPanelLedsCount == 5 || TPanelLedsCount == 9) {
        switch (page) {
        case MenuPage::LedIdleColorCenterRed:
        case MenuPage::LedActiveColorCenterRed:
            return colors.center.r;
        case MenuPage::LedIdleColorCenterGreen:
        case MenuPage::LedActiveColorCenterGreen:
            return colors.center.g;
        case MenuPage::LedIdleColorCenterBlue:
        case MenuPage::LedActiveColorCenterBlue:
            return colors.center.b;
        default:
            break;
        }
    }

    return 0;
}

template <size_t TPanelCount>
void setThresholdValue(const MenuPage page, const uint16_t value,
                       typename Peripherals::Pad<TPanelCount>::Config::Thresholds &thresholds) {
    if constexpr (TPanelCount == 4 || TPanelCount == 6 || TPanelCount == 8 || TPanelCount == 9) {
        switch (page) {
        case MenuPage::PadTriggerThresholdUp:
            thresholds.up = value;
            return;
        case MenuPage::PadTriggerThresholdLeft:
            thresholds.left = value;
            return;
        case MenuPage::PadTriggerThresholdRight:
            thresholds.right = value;
            return;
        case MenuPage::PadTriggerThresholdDown:
            thresholds.down = value;
            return;
        default:
            break;
        }
    }
    if constexpr (TPanelCount == 5 || TPanelCount == 6 || TPanelCount == 8 || TPanelCount == 9) {
        switch (page) {
        case MenuPage::PadTriggerThresholdUpLeft:
            thresholds.up_left = value;
            return;
        case MenuPage::PadTriggerThresholdUpRight:
            thresholds.up_right = value;
            return;
        default:
            break;
        }
    }
    if constexpr (TPanelCount == 5 || TPanelCount == 8 || TPanelCount == 9) {
        switch (page) {
        case MenuPage::PadTriggerThresholdDownLeft:
            thresholds.down_left = value;
            return;
        case MenuPage::PadTriggerThresholdDownRight:
            thresholds.down_right = value;
            return;
        default:
            break;
        }
    }
    if constexpr (TPanelCount == 5 || TPanelCount == 9) {
        switch (page) {
        case MenuPage::PadTriggerThresholdCenter:
            thresholds.center = value;
            return;
        default:
            break;
        }
    }

    return;
}

template <size_t TPanelLedsCount>
void setLedColorValue(const MenuPage page, const uint8_t value,
                      typename Peripherals::PanelLeds<TPanelLedsCount>::Config::PanelColors &colors) {
    if constexpr (TPanelLedsCount == 4 || TPanelLedsCount == 6 || TPanelLedsCount == 8 || TPanelLedsCount == 9) {
        switch (page) {
        case MenuPage::LedIdleColorUpRed:
        case MenuPage::LedActiveColorUpRed:
            colors.up.r = value;
            return;
        case MenuPage::LedIdleColorUpGreen:
        case MenuPage::LedActiveColorUpGreen:
            colors.up.g = value;
            return;
        case MenuPage::LedIdleColorUpBlue:
        case MenuPage::LedActiveColorUpBlue:
            colors.up.b = value;
            return;
        case MenuPage::LedIdleColorLeftRed:
        case MenuPage::LedActiveColorLeftRed:
            colors.left.r = value;
            return;
        case MenuPage::LedIdleColorLeftGreen:
        case MenuPage::LedActiveColorLeftGreen:
            colors.left.g = value;
            return;
        case MenuPage::LedIdleColorLeftBlue:
        case MenuPage::LedActiveColorLeftBlue:
            colors.left.b = value;
            return;
        case MenuPage::LedIdleColorRightRed:
        case MenuPage::LedActiveColorRightRed:
            colors.right.r = value;
            return;
        case MenuPage::LedIdleColorRightGreen:
        case MenuPage::LedActiveColorRightGreen:
            colors.right.g = value;
            return;
        case MenuPage::LedIdleColorRightBlue:
        case MenuPage::LedActiveColorRightBlue:
            colors.right.b = value;
            return;
        case MenuPage::LedIdleColorDownRed:
        case MenuPage::LedActiveColorDownRed:
            colors.down.r = value;
            return;
        case MenuPage::LedIdleColorDownGreen:
        case MenuPage::LedActiveColorDownGreen:
            colors.down.g = value;
            return;
        case MenuPage::LedIdleColorDownBlue:
        case MenuPage::LedActiveColorDownBlue:
            colors.down.b = value;
            return;
        default:
            break;
        }
    }
    if constexpr (TPanelLedsCount == 5 || TPanelLedsCount == 6 || TPanelLedsCount == 8 || TPanelLedsCount == 9) {
        switch (page) {
        case MenuPage::LedIdleColorUpLeftRed:
        case MenuPage::LedActiveColorUpLeftRed:
            colors.up_left.r = value;
            return;
        case MenuPage::LedIdleColorUpLeftGreen:
        case MenuPage::LedActiveColorUpLeftGreen:
            colors.up_left.g = value;
            return;
        case MenuPage::LedIdleColorUpLeftBlue:
        case MenuPage::LedActiveColorUpLeftBlue:
            colors.up_left.b = value;
            return;
        case MenuPage::LedIdleColorUpRightRed:
        case MenuPage::LedActiveColorUpRightRed:
            colors.up_right.r = value;
            return;
        case MenuPage::LedIdleColorUpRightGreen:
        case MenuPage::LedActiveColorUpRightGreen:
            colors.up_right.g = value;
            return;
        case MenuPage::LedIdleColorUpRightBlue:
        case MenuPage::LedActiveColorUpRightBlue:
            colors.up_right.b = value;
            return;
        default:
            break;
        }
    }
    if constexpr (TPanelLedsCount == 5 || TPanelLedsCount == 8 || TPanelLedsCount == 9) {
        switch (page) {
        case MenuPage::LedIdleColorDownLeftRed:
        case MenuPage::LedActiveColorDownLeftRed:
            colors.down_left.r = value;
            return;
        case MenuPage::LedIdleColorDownLeftGreen:
        case MenuPage::LedActiveColorDownLeftGreen:
            colors.down_left.g = value;
            return;
        case MenuPage::LedIdleColorDownLeftBlue:
        case MenuPage::LedActiveColorDownLeftBlue:
            colors.down_left.b = value;
            return;
        case MenuPage::LedIdleColorDownRightRed:
        case MenuPage::LedActiveColorDownRightRed:
            colors.down_right.r = value;
            return;
        case MenuPage::LedIdleColorDownRightGreen:
        case MenuPage::LedActiveColorDownRightGreen:
            colors.down_right.g = value;
            return;
        case MenuPage::LedIdleColorDownRightBlue:
        case MenuPage::LedActiveColorDownRightBlue:
            colors.down_right.b = value;
            return;
        default:
            break;
        }
    }
    if constexpr (TPanelLedsCount == 5 || TPanelLedsCount == 9) {
        switch (page) {
        case MenuPage::LedIdleColorCenterRed:
        case MenuPage::LedActiveColorCenterRed:
            colors.center.r = value;
            return;
        case MenuPage::LedIdleColorCenterGreen:
        case MenuPage::LedActiveColorCenterGreen:
            colors.center.g = value;
            return;
        case MenuPage::LedIdleColorCenterBlue:
        case MenuPage::LedActiveColorCenterBlue:
            colors.center.b = value;
            return;
        default:
            break;
        }
    }

    return;
}

template <size_t TPanelCount>
void setThresholdValue(const MenuDescriptor::Action action, const uint16_t value,
                       typename Peripherals::Pad<TPanelCount>::Config::Thresholds &thresholds) {
    if constexpr (TPanelCount == 4 || TPanelCount == 6 || TPanelCount == 8 || TPanelCount == 9) {
        switch (action) {
        case MenuDescriptor::Action::SetPadTriggerThresholdUp:
            thresholds.up = value;
            return;
        case MenuDescriptor::Action::SetPadTriggerThresholdLeft:
            thresholds.left = value;
            return;
        case MenuDescriptor::Action::SetPadTriggerThresholdRight:
            thresholds.right = value;
            return;
        case MenuDescriptor::Action::SetPadTriggerThresholdDown:
            thresholds.down = value;
            return;
        default:
            break;
        }
    }
    if constexpr (TPanelCount == 5 || TPanelCount == 6 || TPanelCount == 8 || TPanelCount == 9) {
        switch (action) {
        case MenuDescriptor::Action::SetPadTriggerThresholdUpLeft:
            thresholds.up_left = value;
            return;
        case MenuDescriptor::Action::SetPadTriggerThresholdUpRight:
            thresholds.up_right = value;
            return;
        default:
            break;
        }
    }
    if constexpr (TPanelCount == 5 || TPanelCount == 8 || TPanelCount == 9) {
        switch (action) {
        case MenuDescriptor::Action::SetPadTriggerThresholdDownLeft:
            thresholds.down_left = value;
            return;
        case MenuDescriptor::Action::SetPadTriggerThresholdDownRight:
            thresholds.down_right = value;
            return;
        default:
            break;
        }
    }
    if constexpr (TPanelCount == 5 || TPanelCount == 9) {
        switch (action) {
        case MenuDescriptor::Action::SetPadTriggerThresholdCenter:
            thresholds.center = value;
            return;
        default:
            break;
        }
    }

    return;
}

template <size_t TPanelLedsCount>
void setLedColorValue(const MenuDescriptor::Action action, const uint8_t value,
                      typename Peripherals::PanelLeds<TPanelLedsCount>::Config::PanelColors &colors) {
    if constexpr (TPanelLedsCount == 4 || TPanelLedsCount == 6 || TPanelLedsCount == 8 || TPanelLedsCount == 9) {
        switch (action) {
        case MenuDescriptor::Action::SetLedIdleColorUpRed:
        case MenuDescriptor::Action::SetLedActiveColorUpRed:
            colors.up.r = value;
            return;
        case MenuDescriptor::Action::SetLedIdleColorUpGreen:
        case MenuDescriptor::Action::SetLedActiveColorUpGreen:
            colors.up.g = value;
            return;
        case MenuDescriptor::Action::SetLedIdleColorUpBlue:
        case MenuDescriptor::Action::SetLedActiveColorUpBlue:
            colors.up.b = value;
            return;
        case MenuDescriptor::Action::SetLedIdleColorLeftRed:
        case MenuDescriptor::Action::SetLedActiveColorLeftRed:
            colors.left.r = value;
            return;
        case MenuDescriptor::Action::SetLedIdleColorLeftGreen:
        case MenuDescriptor::Action::SetLedActiveColorLeftGreen:
            colors.left.g = value;
            return;
        case MenuDescriptor::Action::SetLedIdleColorLeftBlue:
        case MenuDescriptor::Action::SetLedActiveColorLeftBlue:
            colors.left.b = value;
            return;
        case MenuDescriptor::Action::SetLedIdleColorRightRed:
        case MenuDescriptor::Action::SetLedActiveColorRightRed:
            colors.right.r = value;
            return;
        case MenuDescriptor::Action::SetLedIdleColorRightGreen:
        case MenuDescriptor::Action::SetLedActiveColorRightGreen:
            colors.right.g = value;
            return;
        case MenuDescriptor::Action::SetLedIdleColorRightBlue:
        case MenuDescriptor::Action::SetLedActiveColorRightBlue:
            colors.right.b = value;
            return;
        case MenuDescriptor::Action::SetLedIdleColorDownRed:
        case MenuDescriptor::Action::SetLedActiveColorDownRed:
            colors.down.r = value;
            return;
        case MenuDescriptor::Action::SetLedIdleColorDownGreen:
        case MenuDescriptor::Action::SetLedActiveColorDownGreen:
            colors.down.g = value;
            return;
        case MenuDescriptor::Action::SetLedIdleColorDownBlue:
        case MenuDescriptor::Action::SetLedActiveColorDownBlue:
            colors.down.b = value;
            return;
        default:
            break;
        }
    }
    if constexpr (TPanelLedsCount == 5 || TPanelLedsCount == 6 || TPanelLedsCount == 8 || TPanelLedsCount == 9) {
        switch (action) {
        case MenuDescriptor::Action::SetLedIdleColorUpLeftRed:
        case MenuDescriptor::Action::SetLedActiveColorUpLeftRed:
            colors.up_left.r = value;
            return;
        case MenuDescriptor::Action::SetLedIdleColorUpLeftGreen:
        case MenuDescriptor::Action::SetLedActiveColorUpLeftGreen:
            colors.up_left.g = value;
            return;
        case MenuDescriptor::Action::SetLedIdleColorUpLeftBlue:
        case MenuDescriptor::Action::SetLedActiveColorUpLeftBlue:
            colors.up_left.b = value;
            return;
        case MenuDescriptor::Action::SetLedIdleColorUpRightRed:
        case MenuDescriptor::Action::SetLedActiveColorUpRightRed:
            colors.up_right.r = value;
            return;
        case MenuDescriptor::Action::SetLedIdleColorUpRightGreen:
        case MenuDescriptor::Action::SetLedActiveColorUpRightGreen:
            colors.up_right.g = value;
            return;
        case MenuDescriptor::Action::SetLedIdleColorUpRightBlue:
        case MenuDescriptor::Action::SetLedActiveColorUpRightBlue:
            colors.up_right.b = value;
            return;
        default:
            break;
        }
    }
    if constexpr (TPanelLedsCount == 5 || TPanelLedsCount == 8 || TPanelLedsCount == 9) {
        switch (action) {
        case MenuDescriptor::Action::SetLedIdleColorDownLeftRed:
        case MenuDescriptor::Action::SetLedActiveColorDownLeftRed:
            colors.down_left.r = value;
            return;
        case MenuDescriptor::Action::SetLedIdleColorDownLeftGreen:
        case MenuDescriptor::Action::SetLedActiveColorDownLeftGreen:
            colors.down_left.g = value;
            return;
        case MenuDescriptor::Action::SetLedIdleColorDownLeftBlue:
        case MenuDescriptor::Action::SetLedActiveColorDownLeftBlue:
            colors.down_left.b = value;
            return;
        case MenuDescriptor::Action::SetLedIdleColorDownRightRed:
        case MenuDescriptor::Action::SetLedActiveColorDownRightRed:
            colors.down_right.r = value;
            return;
        case MenuDescriptor::Action::SetLedIdleColorDownRightGreen:
        case MenuDescriptor::Action::SetLedActiveColorDownRightGreen:
            colors.down_right.g = value;
            return;
        case MenuDescriptor::Action::SetLedIdleColorDownRightBlue:
        case MenuDescriptor::Action::SetLedActiveColorDownRightBlue:
            colors.down_right.b = value;
            return;
        default:
            break;
        }
    }
    if constexpr (TPanelLedsCount == 5 || TPanelLedsCount == 9) {
        switch (action) {
        case MenuDescriptor::Action::SetLedIdleColorCenterRed:
        case MenuDescriptor::Action::SetLedActiveColorCenterRed:
            colors.center.r = value;
            return;
        case MenuDescriptor::Action::SetLedIdleColorCenterGreen:
        case MenuDescriptor::Action::SetLedActiveColorCenterGreen:
            colors.center.g = value;
            return;
        case MenuDescriptor::Action::SetLedIdleColorCenterBlue:
        case MenuDescriptor::Action::SetLedActiveColorCenterBlue:
            colors.center.b = value;
            return;
        default:
            break;
        }
    }

    return;
}

InputState::Controller checkPressed(const InputState::Controller &controller_state) {
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

} // namespace

template <size_t TPanelCount, size_t TPanelLedsCount>
const std::map<MenuPage, const MenuDescriptor> Menu<TPanelCount, TPanelLedsCount>::descriptors = {
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
      {{"Spice2x", MenuDescriptor::Action::SetUsbMode},    //
       {"X360 DDR", MenuDescriptor::Action::SetUsbMode},   //
       {"Xbox 360", MenuDescriptor::Action::SetUsbMode},   //
       {"PS3 DDR", MenuDescriptor::Action::SetUsbMode},    //
       {"Dualshock3", MenuDescriptor::Action::SetUsbMode}, //
       {"Dualshock4", MenuDescriptor::Action::SetUsbMode}, //
       {"PS4 Compat", MenuDescriptor::Action::SetUsbMode}, //
       {"Swtch Pro", MenuDescriptor::Action::SetUsbMode},  //
       {"Keybrd P1", MenuDescriptor::Action::SetUsbMode},  //
       {"Keybrd P2", MenuDescriptor::Action::SetUsbMode},  //
       {"Debug", MenuDescriptor::Action::SetUsbMode}},     //
      0}},                                                 //

    {MenuPage::Pad,                                                           //
     {MenuDescriptor::Type::Menu,                                             //
      "Pad Settings",                                                         //
      {{"Calibrate", MenuDescriptor::Action::GotoPagePadCalibrate},           //
       {"Hold Time", MenuDescriptor::Action::GotoPagePadDebounceDelay},       //
       {"Thresholds", MenuDescriptor::Action::GotoPagePadTriggerThresholds}}, //
      0}},                                                                    //

    {MenuPage::PadCalibrate,
     {MenuDescriptor::Type::Menu,                       //
      "Calibration",                                    //
      {{"Start", MenuDescriptor::Action::DoCalibrate}}, //
      0}},                                              //

    {MenuPage::PadDebounceDelay,
     {MenuDescriptor::Type::Value,                         //
      "Step Hold Time (ms)",                               //
      {{"", MenuDescriptor::Action::SetPadDebounceDelay}}, //
      UINT8_MAX}},                                         //

    {MenuPage::PadTriggerThresholds, thresholds_descriptor<TPanelCount>},

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

    {MenuPage::Led,                                                          //
     {MenuDescriptor::Type::Menu,                                            //
      "LED Settings",                                                        //
      {{"Brightness", MenuDescriptor::Action::GotoPageLedBrightness},        //
       {"Anim Speed", MenuDescriptor::Action::GotoPageLedAnimationSpeed},    //
       {"Idle Mode", MenuDescriptor::Action::GotoPageLedIdleMode},           //
       {"Idle Color", MenuDescriptor::Action::GotoPageLedIdleColors},        //
       {"Actv Mode", MenuDescriptor::Action::GotoPageLedActiveMode},         //
       {"Actv Color", MenuDescriptor::Action::GotoPageLedActiveColors},      //
       {"Plyr Color", MenuDescriptor::Action::GotoPageLedEnablePlayerColor}, //
       {"HID Lights", MenuDescriptor::Action::GotoPageLedEnableHidLights}},  //
      0}},                                                                   //

    {MenuPage::LedBrightness,                           //
     {MenuDescriptor::Type::Value,                      //
      "LED Brightness",                                 //
      {{"", MenuDescriptor::Action::SetLedBrightness}}, //
      UINT8_MAX}},                                      //

    {MenuPage::LedAnimationSpeed,                           //
     {MenuDescriptor::Type::Value,                          //
      "LED Animation Speed",                                //
      {{"", MenuDescriptor::Action::SetLedAnimationSpeed}}, //
      UINT8_MAX}},                                          //

    {MenuPage::LedIdleMode,                                  //
     {MenuDescriptor::Type::Selection,                       //
      "LED Idle Mode",                                       //
      {{"Off", MenuDescriptor::Action::SetLedIdleMode},      //
       {"Static", MenuDescriptor::Action::SetLedIdleMode},   //
       {"Pulse", MenuDescriptor::Action::SetLedIdleMode},    //
       {"Rainbow", MenuDescriptor::Action::SetLedIdleMode}}, //
      0}},                                                   //

    {MenuPage::LedIdleColors, idle_colors_descriptor<TPanelLedsCount>},

    {MenuPage::LedIdleColorUpLeft,                                         //
     {MenuDescriptor::Type::Menu,                                          //
      "Idle Up Left",                                                      //
      {{"Red", MenuDescriptor::Action::GotoPageLedIdleColorUpLeftRed},     //
       {"Green", MenuDescriptor::Action::GotoPageLedIdleColorUpLeftGreen}, //
       {"Blue", MenuDescriptor::Action::GotoPageLedIdleColorUpLeftBlue}},  //
      0}},                                                                 //
    {MenuPage::LedIdleColorUpLeftRed,                                      //
     {MenuDescriptor::Type::Value,                                         //
      "Idle Up Left Red",                                                  //
      {{"", MenuDescriptor::Action::SetLedIdleColorUpLeftRed}},            //
      UINT8_MAX}},                                                         //
    {MenuPage::LedIdleColorUpLeftGreen,                                    //
     {MenuDescriptor::Type::Value,                                         //
      "Idle Up Left Green",                                                //
      {{"", MenuDescriptor::Action::SetLedIdleColorUpLeftGreen}},          //
      UINT8_MAX}},                                                         //
    {MenuPage::LedIdleColorUpLeftBlue,                                     //
     {MenuDescriptor::Type::Value,                                         //
      "Idle Up Left Blue",                                                 //
      {{"", MenuDescriptor::Action::SetLedIdleColorUpLeftBlue}},           //
      UINT8_MAX}},                                                         //

    {MenuPage::LedIdleColorUp,                                         //
     {MenuDescriptor::Type::Menu,                                      //
      "Idle Up",                                                       //
      {{"Red", MenuDescriptor::Action::GotoPageLedIdleColorUpRed},     //
       {"Green", MenuDescriptor::Action::GotoPageLedIdleColorUpGreen}, //
       {"Blue", MenuDescriptor::Action::GotoPageLedIdleColorUpBlue}},  //
      0}},                                                             //
    {MenuPage::LedIdleColorUpRed,                                      //
     {MenuDescriptor::Type::Value,                                     //
      "Idle Up Red",                                                   //
      {{"", MenuDescriptor::Action::SetLedIdleColorUpRed}},            //
      UINT8_MAX}},                                                     //
    {MenuPage::LedIdleColorUpGreen,                                    //
     {MenuDescriptor::Type::Value,                                     //
      "Idle Up Green",                                                 //
      {{"", MenuDescriptor::Action::SetLedIdleColorUpGreen}},          //
      UINT8_MAX}},                                                     //
    {MenuPage::LedIdleColorUpBlue,                                     //
     {MenuDescriptor::Type::Value,                                     //
      "Idle Up Blue",                                                  //
      {{"", MenuDescriptor::Action::SetLedIdleColorUpBlue}},           //
      UINT8_MAX}},                                                     //

    {MenuPage::LedIdleColorUpRight,                                         //
     {MenuDescriptor::Type::Menu,                                           //
      "Idle Up Right",                                                      //
      {{"Red", MenuDescriptor::Action::GotoPageLedIdleColorUpRightRed},     //
       {"Green", MenuDescriptor::Action::GotoPageLedIdleColorUpRightGreen}, //
       {"Blue", MenuDescriptor::Action::GotoPageLedIdleColorUpRightBlue}},  //
      0}},                                                                  //
    {MenuPage::LedIdleColorUpRightRed,                                      //
     {MenuDescriptor::Type::Value,                                          //
      "Idle Up Right Red",                                                  //
      {{"", MenuDescriptor::Action::SetLedIdleColorUpRightRed}},            //
      UINT8_MAX}},                                                          //
    {MenuPage::LedIdleColorUpRightGreen,                                    //
     {MenuDescriptor::Type::Value,                                          //
      "Idle Up Right Green",                                                //
      {{"", MenuDescriptor::Action::SetLedIdleColorUpRightGreen}},          //
      UINT8_MAX}},                                                          //
    {MenuPage::LedIdleColorUpRightBlue,                                     //
     {MenuDescriptor::Type::Value,                                          //
      "Idle Up Right Blue",                                                 //
      {{"", MenuDescriptor::Action::SetLedIdleColorUpRightBlue}},           //
      UINT8_MAX}},                                                          //

    {MenuPage::LedIdleColorLeft,                                         //
     {MenuDescriptor::Type::Menu,                                        //
      "Idle Left",                                                       //
      {{"Red", MenuDescriptor::Action::GotoPageLedIdleColorLeftRed},     //
       {"Green", MenuDescriptor::Action::GotoPageLedIdleColorLeftGreen}, //
       {"Blue", MenuDescriptor::Action::GotoPageLedIdleColorLeftBlue}},  //
      0}},                                                               //
    {MenuPage::LedIdleColorLeftRed,                                      //
     {MenuDescriptor::Type::Value,                                       //
      "Idle Left Red",                                                   //
      {{"", MenuDescriptor::Action::SetLedIdleColorLeftRed}},            //
      UINT8_MAX}},                                                       //
    {MenuPage::LedIdleColorLeftGreen,                                    //
     {MenuDescriptor::Type::Value,                                       //
      "Idle Left Green",                                                 //
      {{"", MenuDescriptor::Action::SetLedIdleColorLeftGreen}},          //
      UINT8_MAX}},                                                       //
    {MenuPage::LedIdleColorLeftBlue,                                     //
     {MenuDescriptor::Type::Value,                                       //
      "Idle Left Blue",                                                  //
      {{"", MenuDescriptor::Action::SetLedIdleColorLeftBlue}},           //
      UINT8_MAX}},                                                       //

    {MenuPage::LedIdleColorCenter,                                         //
     {MenuDescriptor::Type::Menu,                                          //
      "Idle Center",                                                       //
      {{"Red", MenuDescriptor::Action::GotoPageLedIdleColorCenterRed},     //
       {"Green", MenuDescriptor::Action::GotoPageLedIdleColorCenterGreen}, //
       {"Blue", MenuDescriptor::Action::GotoPageLedIdleColorCenterBlue}},  //
      0}},                                                                 //
    {MenuPage::LedIdleColorCenterRed,                                      //
     {MenuDescriptor::Type::Value,                                         //
      "Idle Center Red",                                                   //
      {{"", MenuDescriptor::Action::SetLedIdleColorCenterRed}},            //
      UINT8_MAX}},                                                         //
    {MenuPage::LedIdleColorCenterGreen,                                    //
     {MenuDescriptor::Type::Value,                                         //
      "Idle Center Green",                                                 //
      {{"", MenuDescriptor::Action::SetLedIdleColorCenterGreen}},          //
      UINT8_MAX}},                                                         //
    {MenuPage::LedIdleColorCenterBlue,                                     //
     {MenuDescriptor::Type::Value,                                         //
      "Idle Center Blue",                                                  //
      {{"", MenuDescriptor::Action::SetLedIdleColorCenterBlue}},           //
      UINT8_MAX}},                                                         //

    {MenuPage::LedIdleColorRight,                                         //
     {MenuDescriptor::Type::Menu,                                         //
      "Idle Right",                                                       //
      {{"Red", MenuDescriptor::Action::GotoPageLedIdleColorRightRed},     //
       {"Green", MenuDescriptor::Action::GotoPageLedIdleColorRightGreen}, //
       {"Blue", MenuDescriptor::Action::GotoPageLedIdleColorRightBlue}},  //
      0}},                                                                //
    {MenuPage::LedIdleColorRightRed,                                      //
     {MenuDescriptor::Type::Value,                                        //
      "Idle Right Red",                                                   //
      {{"", MenuDescriptor::Action::SetLedIdleColorRightRed}},            //
      UINT8_MAX}},                                                        //
    {MenuPage::LedIdleColorRightGreen,                                    //
     {MenuDescriptor::Type::Value,                                        //
      "Idle Right Green",                                                 //
      {{"", MenuDescriptor::Action::SetLedIdleColorRightGreen}},          //
      UINT8_MAX}},                                                        //
    {MenuPage::LedIdleColorRightBlue,                                     //
     {MenuDescriptor::Type::Value,                                        //
      "Idle Right Blue",                                                  //
      {{"", MenuDescriptor::Action::SetLedIdleColorRightBlue}},           //
      UINT8_MAX}},                                                        //

    {MenuPage::LedIdleColorDownLeft,                                         //
     {MenuDescriptor::Type::Menu,                                            //
      "Idle Down Left",                                                      //
      {{"Red", MenuDescriptor::Action::GotoPageLedIdleColorDownLeftRed},     //
       {"Green", MenuDescriptor::Action::GotoPageLedIdleColorDownLeftGreen}, //
       {"Blue", MenuDescriptor::Action::GotoPageLedIdleColorDownLeftBlue}},  //
      0}},                                                                   //
    {MenuPage::LedIdleColorDownLeftRed,                                      //
     {MenuDescriptor::Type::Value,                                           //
      "Idle Down Left Red",                                                  //
      {{"", MenuDescriptor::Action::SetLedIdleColorDownLeftRed}},            //
      UINT8_MAX}},                                                           //
    {MenuPage::LedIdleColorDownLeftGreen,                                    //
     {MenuDescriptor::Type::Value,                                           //
      "Idle Down Left Green",                                                //
      {{"", MenuDescriptor::Action::SetLedIdleColorDownLeftGreen}},          //
      UINT8_MAX}},                                                           //
    {MenuPage::LedIdleColorDownLeftBlue,                                     //
     {MenuDescriptor::Type::Value,                                           //
      "Idle Down Left Blue",                                                 //
      {{"", MenuDescriptor::Action::SetLedIdleColorDownLeftBlue}},           //
      UINT8_MAX}},                                                           //

    {MenuPage::LedIdleColorDown,                                         //
     {MenuDescriptor::Type::Menu,                                        //
      "Idle Down",                                                       //
      {{"Red", MenuDescriptor::Action::GotoPageLedIdleColorDownRed},     //
       {"Green", MenuDescriptor::Action::GotoPageLedIdleColorDownGreen}, //
       {"Blue", MenuDescriptor::Action::GotoPageLedIdleColorDownBlue}},  //
      0}},                                                               //
    {MenuPage::LedIdleColorDownRed,                                      //
     {MenuDescriptor::Type::Value,                                       //
      "Idle Down Red",                                                   //
      {{"", MenuDescriptor::Action::SetLedIdleColorDownRed}},            //
      UINT8_MAX}},                                                       //
    {MenuPage::LedIdleColorDownGreen,                                    //
     {MenuDescriptor::Type::Value,                                       //
      "Idle Down Green",                                                 //
      {{"", MenuDescriptor::Action::SetLedIdleColorDownGreen}},          //
      UINT8_MAX}},                                                       //
    {MenuPage::LedIdleColorDownBlue,                                     //
     {MenuDescriptor::Type::Value,                                       //
      "Idle Down Blue",                                                  //
      {{"", MenuDescriptor::Action::SetLedIdleColorDownBlue}},           //
      UINT8_MAX}},                                                       //

    {MenuPage::LedIdleColorDownRight,                                         //
     {MenuDescriptor::Type::Menu,                                             //
      "Idle Down Right",                                                      //
      {{"Red", MenuDescriptor::Action::GotoPageLedIdleColorDownRightRed},     //
       {"Green", MenuDescriptor::Action::GotoPageLedIdleColorDownRightGreen}, //
       {"Blue", MenuDescriptor::Action::GotoPageLedIdleColorDownRightBlue}},  //
      0}},                                                                    //
    {MenuPage::LedIdleColorDownRightRed,                                      //
     {MenuDescriptor::Type::Value,                                            //
      "Idle Down Right Red",                                                  //
      {{"", MenuDescriptor::Action::SetLedIdleColorDownRightRed}},            //
      UINT8_MAX}},                                                            //
    {MenuPage::LedIdleColorDownRightGreen,                                    //
     {MenuDescriptor::Type::Value,                                            //
      "Idle Down Right Green",                                                //
      {{"", MenuDescriptor::Action::SetLedIdleColorDownRightGreen}},          //
      UINT8_MAX}},                                                            //
    {MenuPage::LedIdleColorDownRightBlue,                                     //
     {MenuDescriptor::Type::Value,                                            //
      "Idle Down Right Blue",                                                 //
      {{"", MenuDescriptor::Action::SetLedIdleColorDownRightBlue}},           //
      UINT8_MAX}},                                                            //

    {MenuPage::LedActiveMode,                                    //
     {MenuDescriptor::Type::Selection,                           //
      "LED Active Mode",                                         //
      {{"Off", MenuDescriptor::Action::SetLedActiveMode},        //
       {"Idle", MenuDescriptor::Action::SetLedActiveMode},       //
       {"Active", MenuDescriptor::Action::SetLedActiveMode},     //
       {"Actv Fade", MenuDescriptor::Action::SetLedActiveMode},  //
       {"Actv Idle", MenuDescriptor::Action::SetLedActiveMode}}, //
      0}},                                                       //

    {MenuPage::LedActiveColors, active_colors_descriptor<TPanelLedsCount>},

    {MenuPage::LedActiveColorUpLeft,                                         //
     {MenuDescriptor::Type::Menu,                                            //
      "Active Up Left",                                                      //
      {{"Red", MenuDescriptor::Action::GotoPageLedActiveColorUpLeftRed},     //
       {"Green", MenuDescriptor::Action::GotoPageLedActiveColorUpLeftGreen}, //
       {"Blue", MenuDescriptor::Action::GotoPageLedActiveColorUpLeftBlue}},  //
      0}},                                                                   //
    {MenuPage::LedActiveColorUpLeftRed,                                      //
     {MenuDescriptor::Type::Value,                                           //
      "Active Up Left Red",                                                  //
      {{"", MenuDescriptor::Action::SetLedActiveColorUpLeftRed}},            //
      UINT8_MAX}},                                                           //
    {MenuPage::LedActiveColorUpLeftGreen,                                    //
     {MenuDescriptor::Type::Value,                                           //
      "Active Up Left Green",                                                //
      {{"", MenuDescriptor::Action::SetLedActiveColorUpLeftGreen}},          //
      UINT8_MAX}},                                                           //
    {MenuPage::LedActiveColorUpLeftBlue,                                     //
     {MenuDescriptor::Type::Value,                                           //
      "Active Up Left Blue",                                                 //
      {{"", MenuDescriptor::Action::SetLedActiveColorUpLeftBlue}},           //
      UINT8_MAX}},                                                           //

    {MenuPage::LedActiveColorUp,                                         //
     {MenuDescriptor::Type::Menu,                                        //
      "Active Up",                                                       //
      {{"Red", MenuDescriptor::Action::GotoPageLedActiveColorUpRed},     //
       {"Green", MenuDescriptor::Action::GotoPageLedActiveColorUpGreen}, //
       {"Blue", MenuDescriptor::Action::GotoPageLedActiveColorUpBlue}},  //
      0}},                                                               //
    {MenuPage::LedActiveColorUpRed,                                      //
     {MenuDescriptor::Type::Value,                                       //
      "Active Up Red",                                                   //
      {{"", MenuDescriptor::Action::SetLedActiveColorUpRed}},            //
      UINT8_MAX}},                                                       //
    {MenuPage::LedActiveColorUpGreen,                                    //
     {MenuDescriptor::Type::Value,                                       //
      "Active Up Green",                                                 //
      {{"", MenuDescriptor::Action::SetLedActiveColorUpGreen}},          //
      UINT8_MAX}},                                                       //
    {MenuPage::LedActiveColorUpBlue,                                     //
     {MenuDescriptor::Type::Value,                                       //
      "Active Up Blue",                                                  //
      {{"", MenuDescriptor::Action::SetLedActiveColorUpBlue}},           //
      UINT8_MAX}},                                                       //

    {MenuPage::LedActiveColorUpRight,                                         //
     {MenuDescriptor::Type::Menu,                                             //
      "Active Up Right",                                                      //
      {{"Red", MenuDescriptor::Action::GotoPageLedActiveColorUpRightRed},     //
       {"Green", MenuDescriptor::Action::GotoPageLedActiveColorUpRightGreen}, //
       {"Blue", MenuDescriptor::Action::GotoPageLedActiveColorUpRightBlue}},  //
      0}},                                                                    //
    {MenuPage::LedActiveColorUpRightRed,                                      //
     {MenuDescriptor::Type::Value,                                            //
      "Active Up Right Red",                                                  //
      {{"", MenuDescriptor::Action::SetLedActiveColorUpRightRed}},            //
      UINT8_MAX}},                                                            //
    {MenuPage::LedActiveColorUpRightGreen,                                    //
     {MenuDescriptor::Type::Value,                                            //
      "Active Up Right Green",                                                //
      {{"", MenuDescriptor::Action::SetLedActiveColorUpRightGreen}},          //
      UINT8_MAX}},                                                            //
    {MenuPage::LedActiveColorUpRightBlue,                                     //
     {MenuDescriptor::Type::Value,                                            //
      "Active Up Right Blue",                                                 //
      {{"", MenuDescriptor::Action::SetLedActiveColorUpRightBlue}},           //
      UINT8_MAX}},                                                            //

    {MenuPage::LedActiveColorLeft,                                         //
     {MenuDescriptor::Type::Menu,                                          //
      "Active Left",                                                       //
      {{"Red", MenuDescriptor::Action::GotoPageLedActiveColorLeftRed},     //
       {"Green", MenuDescriptor::Action::GotoPageLedActiveColorLeftGreen}, //
       {"Blue", MenuDescriptor::Action::GotoPageLedActiveColorLeftBlue}},  //
      0}},                                                                 //
    {MenuPage::LedActiveColorLeftRed,                                      //
     {MenuDescriptor::Type::Value,                                         //
      "Active Left Red",                                                   //
      {{"", MenuDescriptor::Action::SetLedActiveColorLeftRed}},            //
      UINT8_MAX}},                                                         //
    {MenuPage::LedActiveColorLeftGreen,                                    //
     {MenuDescriptor::Type::Value,                                         //
      "Active Left Green",                                                 //
      {{"", MenuDescriptor::Action::SetLedActiveColorLeftGreen}},          //
      UINT8_MAX}},                                                         //
    {MenuPage::LedActiveColorLeftBlue,                                     //
     {MenuDescriptor::Type::Value,                                         //
      "Active Left Blue",                                                  //
      {{"", MenuDescriptor::Action::SetLedActiveColorLeftBlue}},           //
      UINT8_MAX}},                                                         //

    {MenuPage::LedActiveColorCenter,                                         //
     {MenuDescriptor::Type::Menu,                                            //
      "Active Center",                                                       //
      {{"Red", MenuDescriptor::Action::GotoPageLedActiveColorCenterRed},     //
       {"Green", MenuDescriptor::Action::GotoPageLedActiveColorCenterGreen}, //
       {"Blue", MenuDescriptor::Action::GotoPageLedActiveColorCenterBlue}},  //
      0}},                                                                   //
    {MenuPage::LedActiveColorCenterRed,                                      //
     {MenuDescriptor::Type::Value,                                           //
      "Active Center Red",                                                   //
      {{"", MenuDescriptor::Action::SetLedActiveColorCenterRed}},            //
      UINT8_MAX}},                                                           //
    {MenuPage::LedActiveColorCenterGreen,                                    //
     {MenuDescriptor::Type::Value,                                           //
      "Active Center Green",                                                 //
      {{"", MenuDescriptor::Action::SetLedActiveColorCenterGreen}},          //
      UINT8_MAX}},                                                           //
    {MenuPage::LedActiveColorCenterBlue,                                     //
     {MenuDescriptor::Type::Value,                                           //
      "Active Center Blue",                                                  //
      {{"", MenuDescriptor::Action::SetLedActiveColorCenterBlue}},           //
      UINT8_MAX}},                                                           //

    {MenuPage::LedActiveColorRight,                                         //
     {MenuDescriptor::Type::Menu,                                           //
      "Active Right",                                                       //
      {{"Red", MenuDescriptor::Action::GotoPageLedActiveColorRightRed},     //
       {"Green", MenuDescriptor::Action::GotoPageLedActiveColorRightGreen}, //
       {"Blue", MenuDescriptor::Action::GotoPageLedActiveColorRightBlue}},  //
      0}},                                                                  //
    {MenuPage::LedActiveColorRightRed,                                      //
     {MenuDescriptor::Type::Value,                                          //
      "Active Right Red",                                                   //
      {{"", MenuDescriptor::Action::SetLedActiveColorRightRed}},            //
      UINT8_MAX}},                                                          //
    {MenuPage::LedActiveColorRightGreen,                                    //
     {MenuDescriptor::Type::Value,                                          //
      "Active Right Green",                                                 //
      {{"", MenuDescriptor::Action::SetLedActiveColorRightGreen}},          //
      UINT8_MAX}},                                                          //
    {MenuPage::LedActiveColorRightBlue,                                     //
     {MenuDescriptor::Type::Value,                                          //
      "Active Right Blue",                                                  //
      {{"", MenuDescriptor::Action::SetLedActiveColorRightBlue}},           //
      UINT8_MAX}},                                                          //

    {MenuPage::LedActiveColorDownLeft,                                         //
     {MenuDescriptor::Type::Menu,                                              //
      "Active Down Left",                                                      //
      {{"Red", MenuDescriptor::Action::GotoPageLedActiveColorDownLeftRed},     //
       {"Green", MenuDescriptor::Action::GotoPageLedActiveColorDownLeftGreen}, //
       {"Blue", MenuDescriptor::Action::GotoPageLedActiveColorDownLeftBlue}},  //
      0}},                                                                     //
    {MenuPage::LedActiveColorDownLeftRed,                                      //
     {MenuDescriptor::Type::Value,                                             //
      "Active Down Left Red",                                                  //
      {{"", MenuDescriptor::Action::SetLedActiveColorDownLeftRed}},            //
      UINT8_MAX}},                                                             //
    {MenuPage::LedActiveColorDownLeftGreen,                                    //
     {MenuDescriptor::Type::Value,                                             //
      "Active Down Left Green",                                                //
      {{"", MenuDescriptor::Action::SetLedActiveColorDownLeftGreen}},          //
      UINT8_MAX}},                                                             //
    {MenuPage::LedActiveColorDownLeftBlue,                                     //
     {MenuDescriptor::Type::Value,                                             //
      "Active Down Left Blue",                                                 //
      {{"", MenuDescriptor::Action::SetLedActiveColorDownLeftBlue}},           //
      UINT8_MAX}},                                                             //

    {MenuPage::LedActiveColorDown,                                         //
     {MenuDescriptor::Type::Menu,                                          //
      "Active Down",                                                       //
      {{"Red", MenuDescriptor::Action::GotoPageLedActiveColorDownRed},     //
       {"Green", MenuDescriptor::Action::GotoPageLedActiveColorDownGreen}, //
       {"Blue", MenuDescriptor::Action::GotoPageLedActiveColorDownBlue}},  //
      0}},                                                                 //
    {MenuPage::LedActiveColorDownRed,                                      //
     {MenuDescriptor::Type::Value,                                         //
      "Active Down Red",                                                   //
      {{"", MenuDescriptor::Action::SetLedActiveColorDownRed}},            //
      UINT8_MAX}},                                                         //
    {MenuPage::LedActiveColorDownGreen,                                    //
     {MenuDescriptor::Type::Value,                                         //
      "Active Down Green",                                                 //
      {{"", MenuDescriptor::Action::SetLedActiveColorDownGreen}},          //
      UINT8_MAX}},                                                         //
    {MenuPage::LedActiveColorDownBlue,                                     //
     {MenuDescriptor::Type::Value,                                         //
      "Active Down Blue",                                                  //
      {{"", MenuDescriptor::Action::SetLedActiveColorDownBlue}},           //
      UINT8_MAX}},                                                         //

    {MenuPage::LedActiveColorDownRight,                                         //
     {MenuDescriptor::Type::Menu,                                               //
      "Active Down Right",                                                      //
      {{"Red", MenuDescriptor::Action::GotoPageLedActiveColorDownRightRed},     //
       {"Green", MenuDescriptor::Action::GotoPageLedActiveColorDownRightGreen}, //
       {"Blue", MenuDescriptor::Action::GotoPageLedActiveColorDownRightBlue}},  //
      0}},                                                                      //
    {MenuPage::LedActiveColorDownRightRed,                                      //
     {MenuDescriptor::Type::Value,                                              //
      "Active Down Right Red",                                                  //
      {{"", MenuDescriptor::Action::SetLedActiveColorDownRightRed}},            //
      UINT8_MAX}},                                                              //
    {MenuPage::LedActiveColorDownRightGreen,                                    //
     {MenuDescriptor::Type::Value,                                              //
      "Active Down Right Green",                                                //
      {{"", MenuDescriptor::Action::SetLedActiveColorDownRightGreen}},          //
      UINT8_MAX}},                                                              //
    {MenuPage::LedActiveColorDownRightBlue,                                     //
     {MenuDescriptor::Type::Value,                                              //
      "Active Down Right Blue",                                                 //
      {{"", MenuDescriptor::Action::SetLedActiveColorDownRightBlue}},           //
      UINT8_MAX}},                                                              //

    {MenuPage::LedEnablePlayerColor,                           //
     {MenuDescriptor::Type::Toggle,                            //
      "Player Color (PS4)",                                    //
      {{"", MenuDescriptor::Action::SetLedEnablePlayerColor}}, //
      0}},                                                     //

    {MenuPage::LedEnableHidLights,                           //
     {MenuDescriptor::Type::Toggle,                          //
      "HID Lights",                                          //
      {{"", MenuDescriptor::Action::SetLedEnableHidLights}}, //
      0}},                                                   //

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

template <size_t TPanelCount, size_t TPanelLedsCount>
Menu<TPanelCount, TPanelLedsCount>::Menu(
    const std::shared_ptr<SettingsStore<TPanelCount, TPanelLedsCount>> settings_store,
    const Menu<TPanelCount, TPanelLedsCount>::calibration_callback_t &calibration_callback)
    : m_store(settings_store), m_active(false), m_state_stack({{MenuPage::Main, 0, 0}}),
      m_calibration_callback(calibration_callback){};

template <size_t TPanelCount, size_t TPanelLedsCount> void Menu<TPanelCount, TPanelLedsCount>::activate() {
    m_state_stack = std::stack<MenuState>({{MenuPage::Main, 0, 0}});
    m_active = true;
}

template <size_t TPanelCount, size_t TPanelLedsCount>
uint16_t Menu<TPanelCount, TPanelLedsCount>::getCurrentValue(MenuPage page) {
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
        return getThresholdValue<TPanelCount>(page, m_store->getTriggerThresholds());
    case MenuPage::LedBrightness:
        return m_store->getLedBrightness();
    case MenuPage::LedAnimationSpeed:
        return m_store->getLedAnimationSpeed();
    case MenuPage::LedEnablePlayerColor:
        return static_cast<uint16_t>(m_store->getLedEnablePlayerColor());
    case MenuPage::LedEnableHidLights:
        return static_cast<uint16_t>(m_store->getLedEnableHidLights());
    case MenuPage::LedIdleMode:
        return static_cast<uint16_t>(m_store->getLedIdleMode());
    case MenuPage::LedIdleColors:
    case MenuPage::LedIdleColorUpLeft:
    case MenuPage::LedIdleColorUp:
    case MenuPage::LedIdleColorUpRight:
    case MenuPage::LedIdleColorLeft:
    case MenuPage::LedIdleColorCenter:
    case MenuPage::LedIdleColorRight:
    case MenuPage::LedIdleColorDownLeft:
    case MenuPage::LedIdleColorDown:
    case MenuPage::LedIdleColorDownRight:
        break;
    case MenuPage::LedIdleColorUpLeftRed:
    case MenuPage::LedIdleColorUpLeftGreen:
    case MenuPage::LedIdleColorUpLeftBlue:
    case MenuPage::LedIdleColorUpRed:
    case MenuPage::LedIdleColorUpGreen:
    case MenuPage::LedIdleColorUpBlue:
    case MenuPage::LedIdleColorUpRightRed:
    case MenuPage::LedIdleColorUpRightGreen:
    case MenuPage::LedIdleColorUpRightBlue:
    case MenuPage::LedIdleColorLeftRed:
    case MenuPage::LedIdleColorLeftGreen:
    case MenuPage::LedIdleColorLeftBlue:
    case MenuPage::LedIdleColorCenterRed:
    case MenuPage::LedIdleColorCenterGreen:
    case MenuPage::LedIdleColorCenterBlue:
    case MenuPage::LedIdleColorRightRed:
    case MenuPage::LedIdleColorRightGreen:
    case MenuPage::LedIdleColorRightBlue:
    case MenuPage::LedIdleColorDownLeftRed:
    case MenuPage::LedIdleColorDownLeftGreen:
    case MenuPage::LedIdleColorDownLeftBlue:
    case MenuPage::LedIdleColorDownRed:
    case MenuPage::LedIdleColorDownGreen:
    case MenuPage::LedIdleColorDownBlue:
    case MenuPage::LedIdleColorDownRightRed:
    case MenuPage::LedIdleColorDownRightGreen:
    case MenuPage::LedIdleColorDownRightBlue:
        return getLedColorValue<TPanelLedsCount>(page, m_store->getLedIdleColors());
    case MenuPage::LedActiveMode:
        return static_cast<uint16_t>(m_store->getLedActiveMode());
    case MenuPage::LedActiveColors:
    case MenuPage::LedActiveColorUpLeft:
    case MenuPage::LedActiveColorUp:
    case MenuPage::LedActiveColorUpRight:
    case MenuPage::LedActiveColorLeft:
    case MenuPage::LedActiveColorCenter:
    case MenuPage::LedActiveColorRight:
    case MenuPage::LedActiveColorDownLeft:
    case MenuPage::LedActiveColorDown:
    case MenuPage::LedActiveColorDownRight:
        break;
    case MenuPage::LedActiveColorUpLeftRed:
    case MenuPage::LedActiveColorUpLeftGreen:
    case MenuPage::LedActiveColorUpLeftBlue:
    case MenuPage::LedActiveColorUpRed:
    case MenuPage::LedActiveColorUpGreen:
    case MenuPage::LedActiveColorUpBlue:
    case MenuPage::LedActiveColorUpRightRed:
    case MenuPage::LedActiveColorUpRightGreen:
    case MenuPage::LedActiveColorUpRightBlue:
    case MenuPage::LedActiveColorLeftRed:
    case MenuPage::LedActiveColorLeftGreen:
    case MenuPage::LedActiveColorLeftBlue:
    case MenuPage::LedActiveColorCenterRed:
    case MenuPage::LedActiveColorCenterGreen:
    case MenuPage::LedActiveColorCenterBlue:
    case MenuPage::LedActiveColorRightRed:
    case MenuPage::LedActiveColorRightGreen:
    case MenuPage::LedActiveColorRightBlue:
    case MenuPage::LedActiveColorDownLeftRed:
    case MenuPage::LedActiveColorDownLeftGreen:
    case MenuPage::LedActiveColorDownLeftBlue:
    case MenuPage::LedActiveColorDownRed:
    case MenuPage::LedActiveColorDownGreen:
    case MenuPage::LedActiveColorDownBlue:
    case MenuPage::LedActiveColorDownRightRed:
    case MenuPage::LedActiveColorDownRightGreen:
    case MenuPage::LedActiveColorDownRightBlue:
        return getLedColorValue<TPanelLedsCount>(page, m_store->getLedActiveColors());
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

template <size_t TPanelCount, size_t TPanelLedsCount> void Menu<TPanelCount, TPanelLedsCount>::gotoPage(MenuPage page) {
    const auto current_value = getCurrentValue(page);

    m_state_stack.push({page, current_value, current_value});
}

template <size_t TPanelCount, size_t TPanelLedsCount>
void Menu<TPanelCount, TPanelLedsCount>::gotoParent(bool do_restore) {
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
        case MenuPage::PadTriggerThresholdDownRight: {
            auto thresholds = m_store->getTriggerThresholds();
            setThresholdValue<TPanelCount>(current_state.page, current_state.original_value, thresholds);
            m_store->setTriggerThresholds(thresholds);
        } break;
        case MenuPage::LedBrightness:
            m_store->setLedBrightness(current_state.original_value);
            break;
        case MenuPage::LedAnimationSpeed:
            m_store->setLedAnimationSpeed(current_state.original_value);
            break;
        case MenuPage::LedEnablePlayerColor:
            m_store->setLedEnablePlayerColor(static_cast<bool>(current_state.original_value));
            break;
        case MenuPage::LedEnableHidLights:
            m_store->setLedEnableHidLights(static_cast<bool>(current_state.original_value));
            break;
        case MenuPage::LedIdleMode:
            m_store->setLedIdleMode(
                static_cast<Peripherals::PanelLeds<TPanelLedsCount>::Config::IdleMode>(current_state.original_value));
            break;
        case MenuPage::LedIdleColors:
        case MenuPage::LedIdleColorUpLeft:
        case MenuPage::LedIdleColorUp:
        case MenuPage::LedIdleColorUpRight:
        case MenuPage::LedIdleColorLeft:
        case MenuPage::LedIdleColorCenter:
        case MenuPage::LedIdleColorRight:
        case MenuPage::LedIdleColorDownLeft:
        case MenuPage::LedIdleColorDown:
        case MenuPage::LedIdleColorDownRight:
            break;
        case MenuPage::LedIdleColorUpLeftRed:
        case MenuPage::LedIdleColorUpLeftGreen:
        case MenuPage::LedIdleColorUpLeftBlue:
        case MenuPage::LedIdleColorUpRed:
        case MenuPage::LedIdleColorUpGreen:
        case MenuPage::LedIdleColorUpBlue:
        case MenuPage::LedIdleColorUpRightRed:
        case MenuPage::LedIdleColorUpRightGreen:
        case MenuPage::LedIdleColorUpRightBlue:
        case MenuPage::LedIdleColorLeftRed:
        case MenuPage::LedIdleColorLeftGreen:
        case MenuPage::LedIdleColorLeftBlue:
        case MenuPage::LedIdleColorCenterRed:
        case MenuPage::LedIdleColorCenterGreen:
        case MenuPage::LedIdleColorCenterBlue:
        case MenuPage::LedIdleColorRightRed:
        case MenuPage::LedIdleColorRightGreen:
        case MenuPage::LedIdleColorRightBlue:
        case MenuPage::LedIdleColorDownLeftRed:
        case MenuPage::LedIdleColorDownLeftGreen:
        case MenuPage::LedIdleColorDownLeftBlue:
        case MenuPage::LedIdleColorDownRed:
        case MenuPage::LedIdleColorDownGreen:
        case MenuPage::LedIdleColorDownBlue:
        case MenuPage::LedIdleColorDownRightRed:
        case MenuPage::LedIdleColorDownRightGreen:
        case MenuPage::LedIdleColorDownRightBlue: {
            auto colors = m_store->getLedIdleColors();
            setLedColorValue<TPanelLedsCount>(current_state.page, current_state.original_value, colors);
            m_store->setLedIdleColors(colors);
        } break;
        case MenuPage::LedActiveMode:
            m_store->setLedActiveMode(
                static_cast<Peripherals::PanelLeds<TPanelLedsCount>::Config::ActiveMode>(current_state.original_value));
            break;
        case MenuPage::LedActiveColors:
        case MenuPage::LedActiveColorUpLeft:
        case MenuPage::LedActiveColorUp:
        case MenuPage::LedActiveColorUpRight:
        case MenuPage::LedActiveColorLeft:
        case MenuPage::LedActiveColorCenter:
        case MenuPage::LedActiveColorRight:
        case MenuPage::LedActiveColorDownLeft:
        case MenuPage::LedActiveColorDown:
        case MenuPage::LedActiveColorDownRight:
            break;
        case MenuPage::LedActiveColorUpLeftRed:
        case MenuPage::LedActiveColorUpLeftGreen:
        case MenuPage::LedActiveColorUpLeftBlue:
        case MenuPage::LedActiveColorUpRed:
        case MenuPage::LedActiveColorUpGreen:
        case MenuPage::LedActiveColorUpBlue:
        case MenuPage::LedActiveColorUpRightRed:
        case MenuPage::LedActiveColorUpRightGreen:
        case MenuPage::LedActiveColorUpRightBlue:
        case MenuPage::LedActiveColorLeftRed:
        case MenuPage::LedActiveColorLeftGreen:
        case MenuPage::LedActiveColorLeftBlue:
        case MenuPage::LedActiveColorCenterRed:
        case MenuPage::LedActiveColorCenterGreen:
        case MenuPage::LedActiveColorCenterBlue:
        case MenuPage::LedActiveColorRightRed:
        case MenuPage::LedActiveColorRightGreen:
        case MenuPage::LedActiveColorRightBlue:
        case MenuPage::LedActiveColorDownLeftRed:
        case MenuPage::LedActiveColorDownLeftGreen:
        case MenuPage::LedActiveColorDownLeftBlue:
        case MenuPage::LedActiveColorDownRed:
        case MenuPage::LedActiveColorDownGreen:
        case MenuPage::LedActiveColorDownBlue:
        case MenuPage::LedActiveColorDownRightRed:
        case MenuPage::LedActiveColorDownRightGreen:
        case MenuPage::LedActiveColorDownRightBlue: {
            auto colors = m_store->getLedActiveColors();
            setLedColorValue<TPanelLedsCount>(current_state.page, current_state.original_value, colors);
            m_store->setLedActiveColors(colors);
        } break;
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

template <size_t TPanelCount, size_t TPanelLedsCount>
void Menu<TPanelCount, TPanelLedsCount>::performAction(MenuDescriptor::Action action, uint16_t value) {
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
    case MenuDescriptor::Action::GotoPagePadCalibrate:
        gotoPage(MenuPage::PadCalibrate);
        break;
    case MenuDescriptor::Action::GotoPagePadDebounceDelay:
        gotoPage(MenuPage::PadDebounceDelay);
        break;
    case MenuDescriptor::Action::GotoPagePadTriggerThresholds:
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
    case MenuDescriptor::Action::GotoPageLedAnimationSpeed:
        gotoPage(MenuPage::LedAnimationSpeed);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleMode:
        gotoPage(MenuPage::LedIdleMode);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColors:
        gotoPage(MenuPage::LedIdleColors);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorUpLeft:
        gotoPage(MenuPage::LedIdleColorUpLeft);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorUpLeftRed:
        gotoPage(MenuPage::LedIdleColorUpLeftRed);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorUpLeftGreen:
        gotoPage(MenuPage::LedIdleColorUpLeftGreen);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorUpLeftBlue:
        gotoPage(MenuPage::LedIdleColorUpLeftBlue);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorUp:
        gotoPage(MenuPage::LedIdleColorUp);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorUpRed:
        gotoPage(MenuPage::LedIdleColorUpRed);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorUpGreen:
        gotoPage(MenuPage::LedIdleColorUpGreen);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorUpBlue:
        gotoPage(MenuPage::LedIdleColorUpBlue);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorUpRight:
        gotoPage(MenuPage::LedIdleColorUpRight);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorUpRightRed:
        gotoPage(MenuPage::LedIdleColorUpRightRed);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorUpRightGreen:
        gotoPage(MenuPage::LedIdleColorUpRightGreen);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorUpRightBlue:
        gotoPage(MenuPage::LedIdleColorUpRightBlue);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorLeft:
        gotoPage(MenuPage::LedIdleColorLeft);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorLeftRed:
        gotoPage(MenuPage::LedIdleColorLeftRed);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorLeftGreen:
        gotoPage(MenuPage::LedIdleColorLeftGreen);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorLeftBlue:
        gotoPage(MenuPage::LedIdleColorLeftBlue);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorCenter:
        gotoPage(MenuPage::LedIdleColorCenter);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorCenterRed:
        gotoPage(MenuPage::LedIdleColorCenterRed);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorCenterGreen:
        gotoPage(MenuPage::LedIdleColorCenterGreen);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorCenterBlue:
        gotoPage(MenuPage::LedIdleColorCenterBlue);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorRight:
        gotoPage(MenuPage::LedIdleColorRight);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorRightRed:
        gotoPage(MenuPage::LedIdleColorRightRed);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorRightGreen:
        gotoPage(MenuPage::LedIdleColorRightGreen);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorRightBlue:
        gotoPage(MenuPage::LedIdleColorRightBlue);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorDownLeft:
        gotoPage(MenuPage::LedIdleColorDownLeft);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorDownLeftRed:
        gotoPage(MenuPage::LedIdleColorDownLeftRed);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorDownLeftGreen:
        gotoPage(MenuPage::LedIdleColorDownLeftGreen);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorDownLeftBlue:
        gotoPage(MenuPage::LedIdleColorDownLeftBlue);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorDown:
        gotoPage(MenuPage::LedIdleColorDown);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorDownRed:
        gotoPage(MenuPage::LedIdleColorDownRed);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorDownGreen:
        gotoPage(MenuPage::LedIdleColorDownGreen);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorDownBlue:
        gotoPage(MenuPage::LedIdleColorDownBlue);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorDownRight:
        gotoPage(MenuPage::LedIdleColorDownRight);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorDownRightRed:
        gotoPage(MenuPage::LedIdleColorDownRightRed);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorDownRightGreen:
        gotoPage(MenuPage::LedIdleColorDownRightGreen);
        break;
    case MenuDescriptor::Action::GotoPageLedIdleColorDownRightBlue:
        gotoPage(MenuPage::LedIdleColorDownRightBlue);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveMode:
        gotoPage(MenuPage::LedActiveMode);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColors:
        gotoPage(MenuPage::LedActiveColors);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorUpLeft:
        gotoPage(MenuPage::LedActiveColorUpLeft);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorUpLeftRed:
        gotoPage(MenuPage::LedActiveColorUpLeftRed);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorUpLeftGreen:
        gotoPage(MenuPage::LedActiveColorUpLeftGreen);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorUpLeftBlue:
        gotoPage(MenuPage::LedActiveColorUpLeftBlue);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorUp:
        gotoPage(MenuPage::LedActiveColorUp);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorUpRed:
        gotoPage(MenuPage::LedActiveColorUpRed);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorUpGreen:
        gotoPage(MenuPage::LedActiveColorUpGreen);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorUpBlue:
        gotoPage(MenuPage::LedActiveColorUpBlue);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorUpRight:
        gotoPage(MenuPage::LedActiveColorUpRight);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorUpRightRed:
        gotoPage(MenuPage::LedActiveColorUpRightRed);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorUpRightGreen:
        gotoPage(MenuPage::LedActiveColorUpRightGreen);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorUpRightBlue:
        gotoPage(MenuPage::LedActiveColorUpRightBlue);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorLeft:
        gotoPage(MenuPage::LedActiveColorLeft);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorLeftRed:
        gotoPage(MenuPage::LedActiveColorLeftRed);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorLeftGreen:
        gotoPage(MenuPage::LedActiveColorLeftGreen);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorLeftBlue:
        gotoPage(MenuPage::LedActiveColorLeftBlue);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorCenter:
        gotoPage(MenuPage::LedActiveColorCenter);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorCenterRed:
        gotoPage(MenuPage::LedActiveColorCenterRed);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorCenterGreen:
        gotoPage(MenuPage::LedActiveColorCenterGreen);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorCenterBlue:
        gotoPage(MenuPage::LedActiveColorCenterBlue);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorRight:
        gotoPage(MenuPage::LedActiveColorRight);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorRightRed:
        gotoPage(MenuPage::LedActiveColorRightRed);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorRightGreen:
        gotoPage(MenuPage::LedActiveColorRightGreen);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorRightBlue:
        gotoPage(MenuPage::LedActiveColorRightBlue);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorDownLeft:
        gotoPage(MenuPage::LedActiveColorDownLeft);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorDownLeftRed:
        gotoPage(MenuPage::LedActiveColorDownLeftRed);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorDownLeftGreen:
        gotoPage(MenuPage::LedActiveColorDownLeftGreen);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorDownLeftBlue:
        gotoPage(MenuPage::LedActiveColorDownLeftBlue);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorDown:
        gotoPage(MenuPage::LedActiveColorDown);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorDownRed:
        gotoPage(MenuPage::LedActiveColorDownRed);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorDownGreen:
        gotoPage(MenuPage::LedActiveColorDownGreen);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorDownBlue:
        gotoPage(MenuPage::LedActiveColorDownBlue);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorDownRight:
        gotoPage(MenuPage::LedActiveColorDownRight);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorDownRightRed:
        gotoPage(MenuPage::LedActiveColorDownRightRed);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorDownRightGreen:
        gotoPage(MenuPage::LedActiveColorDownRightGreen);
        break;
    case MenuDescriptor::Action::GotoPageLedActiveColorDownRightBlue:
        gotoPage(MenuPage::LedActiveColorDownRightBlue);
        break;
    case MenuDescriptor::Action::GotoPageLedEnablePlayerColor:
        gotoPage(MenuPage::LedEnablePlayerColor);
        break;
    case MenuDescriptor::Action::GotoPageLedEnableHidLights:
        gotoPage(MenuPage::LedEnableHidLights);
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
    case MenuDescriptor::Action::SetPadTriggerThresholdDownRight: {
        auto thresholds = m_store->getTriggerThresholds();
        setThresholdValue<TPanelCount>(action, value, thresholds);
        m_store->setTriggerThresholds(thresholds);
    } break;
    case MenuDescriptor::Action::SetLedBrightness:
        m_store->setLedBrightness(value);
        break;
    case MenuDescriptor::Action::SetLedAnimationSpeed:
        m_store->setLedAnimationSpeed(value);
        break;
    case MenuDescriptor::Action::SetLedIdleMode:
        m_store->setLedIdleMode(static_cast<Peripherals::PanelLeds<TPanelLedsCount>::Config::IdleMode>(value));
        break;
    case MenuDescriptor::Action::SetLedIdleColorUpLeftRed:
    case MenuDescriptor::Action::SetLedIdleColorUpLeftGreen:
    case MenuDescriptor::Action::SetLedIdleColorUpLeftBlue:
    case MenuDescriptor::Action::SetLedIdleColorUpRed:
    case MenuDescriptor::Action::SetLedIdleColorUpGreen:
    case MenuDescriptor::Action::SetLedIdleColorUpBlue:
    case MenuDescriptor::Action::SetLedIdleColorUpRightRed:
    case MenuDescriptor::Action::SetLedIdleColorUpRightGreen:
    case MenuDescriptor::Action::SetLedIdleColorUpRightBlue:
    case MenuDescriptor::Action::SetLedIdleColorLeftRed:
    case MenuDescriptor::Action::SetLedIdleColorLeftGreen:
    case MenuDescriptor::Action::SetLedIdleColorLeftBlue:
    case MenuDescriptor::Action::SetLedIdleColorCenterRed:
    case MenuDescriptor::Action::SetLedIdleColorCenterGreen:
    case MenuDescriptor::Action::SetLedIdleColorCenterBlue:
    case MenuDescriptor::Action::SetLedIdleColorRightRed:
    case MenuDescriptor::Action::SetLedIdleColorRightGreen:
    case MenuDescriptor::Action::SetLedIdleColorRightBlue:
    case MenuDescriptor::Action::SetLedIdleColorDownLeftRed:
    case MenuDescriptor::Action::SetLedIdleColorDownLeftGreen:
    case MenuDescriptor::Action::SetLedIdleColorDownLeftBlue:
    case MenuDescriptor::Action::SetLedIdleColorDownRed:
    case MenuDescriptor::Action::SetLedIdleColorDownGreen:
    case MenuDescriptor::Action::SetLedIdleColorDownBlue:
    case MenuDescriptor::Action::SetLedIdleColorDownRightRed:
    case MenuDescriptor::Action::SetLedIdleColorDownRightGreen:
    case MenuDescriptor::Action::SetLedIdleColorDownRightBlue: {
        auto colors = m_store->getLedIdleColors();
        setLedColorValue<TPanelLedsCount>(action, value, colors);
        m_store->setLedIdleColors(colors);
    } break;
    case MenuDescriptor::Action::SetLedActiveColorUpLeftRed:
    case MenuDescriptor::Action::SetLedActiveColorUpLeftGreen:
    case MenuDescriptor::Action::SetLedActiveColorUpLeftBlue:
    case MenuDescriptor::Action::SetLedActiveColorUpRed:
    case MenuDescriptor::Action::SetLedActiveColorUpGreen:
    case MenuDescriptor::Action::SetLedActiveColorUpBlue:
    case MenuDescriptor::Action::SetLedActiveColorUpRightRed:
    case MenuDescriptor::Action::SetLedActiveColorUpRightGreen:
    case MenuDescriptor::Action::SetLedActiveColorUpRightBlue:
    case MenuDescriptor::Action::SetLedActiveColorLeftRed:
    case MenuDescriptor::Action::SetLedActiveColorLeftGreen:
    case MenuDescriptor::Action::SetLedActiveColorLeftBlue:
    case MenuDescriptor::Action::SetLedActiveColorCenterRed:
    case MenuDescriptor::Action::SetLedActiveColorCenterGreen:
    case MenuDescriptor::Action::SetLedActiveColorCenterBlue:
    case MenuDescriptor::Action::SetLedActiveColorRightRed:
    case MenuDescriptor::Action::SetLedActiveColorRightGreen:
    case MenuDescriptor::Action::SetLedActiveColorRightBlue:
    case MenuDescriptor::Action::SetLedActiveColorDownLeftRed:
    case MenuDescriptor::Action::SetLedActiveColorDownLeftGreen:
    case MenuDescriptor::Action::SetLedActiveColorDownLeftBlue:
    case MenuDescriptor::Action::SetLedActiveColorDownRed:
    case MenuDescriptor::Action::SetLedActiveColorDownGreen:
    case MenuDescriptor::Action::SetLedActiveColorDownBlue:
    case MenuDescriptor::Action::SetLedActiveColorDownRightRed:
    case MenuDescriptor::Action::SetLedActiveColorDownRightGreen:
    case MenuDescriptor::Action::SetLedActiveColorDownRightBlue: {
        auto colors = m_store->getLedActiveColors();
        setLedColorValue<TPanelLedsCount>(action, value, colors);
        m_store->setLedActiveColors(colors);
    } break;
    case MenuDescriptor::Action::SetLedActiveMode:
        m_store->setLedActiveMode(static_cast<Peripherals::PanelLeds<TPanelLedsCount>::Config::ActiveMode>(value));
        break;
    case MenuDescriptor::Action::SetLedEnablePlayerColor:
        m_store->setLedEnablePlayerColor(static_cast<bool>(value));
        break;
    case MenuDescriptor::Action::SetLedEnableHidLights:
        m_store->setLedEnableHidLights(static_cast<bool>(value));
        break;
    case MenuDescriptor::Action::DoCalibrate:
        m_calibration_callback();
        gotoPage(MenuPage::Pad);
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

template <size_t TPanelCount, size_t TPanelLedsCount>
void Menu<TPanelCount, TPanelLedsCount>::update(const InputState::Controller &controller_state) {
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

template <size_t TPanelCount, size_t TPanelLedsCount> bool Menu<TPanelCount, TPanelLedsCount>::active() {
    return m_active;
}

template <size_t TPanelCount, size_t TPanelLedsCount> MenuState Menu<TPanelCount, TPanelLedsCount>::getState() {
    return m_state_stack.top();
}

} // namespace Dancecon::Utils
