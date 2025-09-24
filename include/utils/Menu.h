#ifndef UTILS_MENU_H_
#define UTILS_MENU_H_

#include "utils/InputState.h"
#include "utils/SettingsStore.h"

#include <functional>
#include <map>
#include <memory>
#include <stack>
#include <string>
#include <vector>

namespace Dancecon::Utils {

enum class MenuPage : uint8_t {
    Main,

    DeviceMode,
    Pad,
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

template <size_t TPanelCount, size_t TPanelLedsCount> class Menu {
  public:
    using calibration_callback_t = std::function<void(void)>;

    const static std::map<MenuPage, const MenuDescriptor> descriptors;

  private:
    std::shared_ptr<SettingsStore<TPanelCount, TPanelLedsCount>> m_store;

    bool m_active{false};
    std::stack<MenuState> m_state_stack{{{.page = MenuPage::Main, .selected_value = 0, .original_value = 0}}};

    calibration_callback_t m_calibration_callback;

    uint16_t getCurrentValue(MenuPage page);
    void gotoPage(MenuPage page);
    void gotoParent(bool do_restore);
    void performAction(MenuDescriptor::Action action, uint16_t value);

  public:
    Menu(std::shared_ptr<SettingsStore<TPanelCount, TPanelLedsCount>> settings_store,
         calibration_callback_t calibration_callback);

    void activate();
    void update(const InputState::Controller &controller_state);
    bool active();
    MenuState getState();
};

} // namespace Dancecon::Utils

#endif // UTILS_MENU_H_