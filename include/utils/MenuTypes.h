#ifndef UTILS_MENUTYPES_H_
#define UTILS_MENUTYPES_H_

#include <string>
#include <vector>

#include <cstdint>

namespace Dancecon::Utils {

enum class MenuPage : uint8_t {
    Main,

    DeviceMode,
    Pad,
    Display,
    Led,
    Reset,
    Bootsel,

    PadCalibrate,
    PadDebounceDelay,
    PadHysteresis,
    PadTriggerThresholds,

    PadTriggerThresholdUpLeft,
    PadTriggerThresholdUp,
    PadTriggerThresholdUpRight,
    PadTriggerThresholdLeft,
    PadTriggerThresholdCenter,
    PadTriggerThresholdRight,
    PadTriggerThresholdDownLeft,
    PadTriggerThresholdDown,
    PadTriggerThresholdDownRight,

    DisplayWeightUnit,

    LedBrightness,
    LedAnimationSpeed,
    LedEnablePlayerColor,
    LedEnableHidLights,
    LedIdleMode,
    LedIdleColors,
    LedActiveMode,
    LedActiveColors,

    LedIdleColorUpLeft,
    LedIdleColorUpLeftRed,
    LedIdleColorUpLeftGreen,
    LedIdleColorUpLeftBlue,

    LedIdleColorUp,
    LedIdleColorUpRed,
    LedIdleColorUpGreen,
    LedIdleColorUpBlue,

    LedIdleColorUpRight,
    LedIdleColorUpRightRed,
    LedIdleColorUpRightGreen,
    LedIdleColorUpRightBlue,

    LedIdleColorLeft,
    LedIdleColorLeftRed,
    LedIdleColorLeftGreen,
    LedIdleColorLeftBlue,

    LedIdleColorCenter,
    LedIdleColorCenterRed,
    LedIdleColorCenterGreen,
    LedIdleColorCenterBlue,

    LedIdleColorRight,
    LedIdleColorRightRed,
    LedIdleColorRightGreen,
    LedIdleColorRightBlue,

    LedIdleColorDownLeft,
    LedIdleColorDownLeftRed,
    LedIdleColorDownLeftGreen,
    LedIdleColorDownLeftBlue,

    LedIdleColorDown,
    LedIdleColorDownRed,
    LedIdleColorDownGreen,
    LedIdleColorDownBlue,

    LedIdleColorDownRight,
    LedIdleColorDownRightRed,
    LedIdleColorDownRightGreen,
    LedIdleColorDownRightBlue,

    LedActiveColorUpLeft,
    LedActiveColorUpLeftRed,
    LedActiveColorUpLeftGreen,
    LedActiveColorUpLeftBlue,

    LedActiveColorUp,
    LedActiveColorUpRed,
    LedActiveColorUpGreen,
    LedActiveColorUpBlue,

    LedActiveColorUpRight,
    LedActiveColorUpRightRed,
    LedActiveColorUpRightGreen,
    LedActiveColorUpRightBlue,

    LedActiveColorLeft,
    LedActiveColorLeftRed,
    LedActiveColorLeftGreen,
    LedActiveColorLeftBlue,

    LedActiveColorCenter,
    LedActiveColorCenterRed,
    LedActiveColorCenterGreen,
    LedActiveColorCenterBlue,

    LedActiveColorRight,
    LedActiveColorRightRed,
    LedActiveColorRightGreen,
    LedActiveColorRightBlue,

    LedActiveColorDownLeft,
    LedActiveColorDownLeftRed,
    LedActiveColorDownLeftGreen,
    LedActiveColorDownLeftBlue,

    LedActiveColorDown,
    LedActiveColorDownRed,
    LedActiveColorDownGreen,
    LedActiveColorDownBlue,

    LedActiveColorDownRight,
    LedActiveColorDownRightRed,
    LedActiveColorDownRightGreen,
    LedActiveColorDownRightBlue,

    BootselMsg,
};

struct MenuState {
    MenuPage page;
    uint16_t selected_value;
    uint16_t original_value;
};

struct MenuDescriptor {
    enum class Type : uint8_t {
        Menu,
        Selection,
        Value,
        Toggle,
        RebootInfo,
    };

    enum class Action : uint8_t {
        None,
        GotoParent,

        GotoPageDeviceMode,
        GotoPagePad,
        GotoPageDisplay,
        GotoPageLed,
        GotoPageReset,
        GotoPageBootsel,

        GotoPagePadCalibrate,
        GotoPagePadDebounceDelay,
        GotoPagePadHysteresis,
        GotoPagePadTriggerThresholds,

        GotoPagePadTriggerThresholdUpLeft,
        GotoPagePadTriggerThresholdUp,
        GotoPagePadTriggerThresholdUpRight,
        GotoPagePadTriggerThresholdLeft,
        GotoPagePadTriggerThresholdCenter,
        GotoPagePadTriggerThresholdRight,
        GotoPagePadTriggerThresholdDownRight,
        GotoPagePadTriggerThresholdDown,
        GotoPagePadTriggerThresholdDownLeft,

        GotoPageDisplayWeightUnit,

        GotoPageLedBrightness,
        GotoPageLedAnimationSpeed,
        GotoPageLedIdleMode,
        GotoPageLedIdleColors,
        GotoPageLedActiveMode,
        GotoPageLedActiveColors,
        GotoPageLedEnablePlayerColor,
        GotoPageLedEnableHidLights,

        GotoPageLedIdleColorUpLeft,
        GotoPageLedIdleColorUpLeftRed,
        GotoPageLedIdleColorUpLeftGreen,
        GotoPageLedIdleColorUpLeftBlue,

        GotoPageLedIdleColorUp,
        GotoPageLedIdleColorUpRed,
        GotoPageLedIdleColorUpGreen,
        GotoPageLedIdleColorUpBlue,

        GotoPageLedIdleColorUpRight,
        GotoPageLedIdleColorUpRightRed,
        GotoPageLedIdleColorUpRightGreen,
        GotoPageLedIdleColorUpRightBlue,

        GotoPageLedIdleColorLeft,
        GotoPageLedIdleColorLeftRed,
        GotoPageLedIdleColorLeftGreen,
        GotoPageLedIdleColorLeftBlue,

        GotoPageLedIdleColorCenter,
        GotoPageLedIdleColorCenterRed,
        GotoPageLedIdleColorCenterGreen,
        GotoPageLedIdleColorCenterBlue,

        GotoPageLedIdleColorRight,
        GotoPageLedIdleColorRightRed,
        GotoPageLedIdleColorRightGreen,
        GotoPageLedIdleColorRightBlue,

        GotoPageLedIdleColorDownLeft,
        GotoPageLedIdleColorDownLeftRed,
        GotoPageLedIdleColorDownLeftGreen,
        GotoPageLedIdleColorDownLeftBlue,

        GotoPageLedIdleColorDown,
        GotoPageLedIdleColorDownRed,
        GotoPageLedIdleColorDownGreen,
        GotoPageLedIdleColorDownBlue,

        GotoPageLedIdleColorDownRight,
        GotoPageLedIdleColorDownRightRed,
        GotoPageLedIdleColorDownRightGreen,
        GotoPageLedIdleColorDownRightBlue,

        GotoPageLedActiveColorUpLeft,
        GotoPageLedActiveColorUpLeftRed,
        GotoPageLedActiveColorUpLeftGreen,
        GotoPageLedActiveColorUpLeftBlue,

        GotoPageLedActiveColorUp,
        GotoPageLedActiveColorUpRed,
        GotoPageLedActiveColorUpGreen,
        GotoPageLedActiveColorUpBlue,

        GotoPageLedActiveColorUpRight,
        GotoPageLedActiveColorUpRightRed,
        GotoPageLedActiveColorUpRightGreen,
        GotoPageLedActiveColorUpRightBlue,

        GotoPageLedActiveColorLeft,
        GotoPageLedActiveColorLeftRed,
        GotoPageLedActiveColorLeftGreen,
        GotoPageLedActiveColorLeftBlue,

        GotoPageLedActiveColorCenter,
        GotoPageLedActiveColorCenterRed,
        GotoPageLedActiveColorCenterGreen,
        GotoPageLedActiveColorCenterBlue,

        GotoPageLedActiveColorRight,
        GotoPageLedActiveColorRightRed,
        GotoPageLedActiveColorRightGreen,
        GotoPageLedActiveColorRightBlue,

        GotoPageLedActiveColorDownLeft,
        GotoPageLedActiveColorDownLeftRed,
        GotoPageLedActiveColorDownLeftGreen,
        GotoPageLedActiveColorDownLeftBlue,

        GotoPageLedActiveColorDown,
        GotoPageLedActiveColorDownRed,
        GotoPageLedActiveColorDownGreen,
        GotoPageLedActiveColorDownBlue,

        GotoPageLedActiveColorDownRight,
        GotoPageLedActiveColorDownRightRed,
        GotoPageLedActiveColorDownRightGreen,
        GotoPageLedActiveColorDownRightBlue,

        SetUsbMode,

        SetPadDebounceDelay,
        SetPadHysteresis,
        SetPadTriggerThresholdUpLeft,
        SetPadTriggerThresholdUp,
        SetPadTriggerThresholdUpRight,
        SetPadTriggerThresholdLeft,
        SetPadTriggerThresholdCenter,
        SetPadTriggerThresholdRight,
        SetPadTriggerThresholdDownLeft,
        SetPadTriggerThresholdDown,
        SetPadTriggerThresholdDownRight,

        SetDisplayWeightUnit,

        SetLedBrightness,
        SetLedAnimationSpeed,
        SetLedIdleMode,
        SetLedActiveMode,
        SetLedEnablePlayerColor,
        SetLedEnableHidLights,

        SetLedIdleColorUpLeftRed,
        SetLedIdleColorUpLeftGreen,
        SetLedIdleColorUpLeftBlue,

        SetLedIdleColorUpRed,
        SetLedIdleColorUpGreen,
        SetLedIdleColorUpBlue,

        SetLedIdleColorUpRightRed,
        SetLedIdleColorUpRightGreen,
        SetLedIdleColorUpRightBlue,

        SetLedIdleColorLeftRed,
        SetLedIdleColorLeftGreen,
        SetLedIdleColorLeftBlue,

        SetLedIdleColorCenterRed,
        SetLedIdleColorCenterGreen,
        SetLedIdleColorCenterBlue,

        SetLedIdleColorRightRed,
        SetLedIdleColorRightGreen,
        SetLedIdleColorRightBlue,

        SetLedIdleColorDownLeftRed,
        SetLedIdleColorDownLeftGreen,
        SetLedIdleColorDownLeftBlue,

        SetLedIdleColorDownRed,
        SetLedIdleColorDownGreen,
        SetLedIdleColorDownBlue,

        SetLedIdleColorDownRightRed,
        SetLedIdleColorDownRightGreen,
        SetLedIdleColorDownRightBlue,

        SetLedActiveColorUpLeftRed,
        SetLedActiveColorUpLeftGreen,
        SetLedActiveColorUpLeftBlue,

        SetLedActiveColorUpRed,
        SetLedActiveColorUpGreen,
        SetLedActiveColorUpBlue,

        SetLedActiveColorUpRightRed,
        SetLedActiveColorUpRightGreen,
        SetLedActiveColorUpRightBlue,

        SetLedActiveColorLeftRed,
        SetLedActiveColorLeftGreen,
        SetLedActiveColorLeftBlue,

        SetLedActiveColorCenterRed,
        SetLedActiveColorCenterGreen,
        SetLedActiveColorCenterBlue,

        SetLedActiveColorRightRed,
        SetLedActiveColorRightGreen,
        SetLedActiveColorRightBlue,

        SetLedActiveColorDownLeftRed,
        SetLedActiveColorDownLeftGreen,
        SetLedActiveColorDownLeftBlue,

        SetLedActiveColorDownRed,
        SetLedActiveColorDownGreen,
        SetLedActiveColorDownBlue,

        SetLedActiveColorDownRightRed,
        SetLedActiveColorDownRightGreen,
        SetLedActiveColorDownRightBlue,

        DoCalibrate,
        DoReset,
        DoRebootToBootsel,
    };

    Type type;
    std::string name;
    std::vector<std::pair<std::string, Action>> items;
    uint16_t max_value;
};

} // namespace Dancecon::Utils

#endif // UTILS_MENUTYPES_H_
