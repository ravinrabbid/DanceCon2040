#ifndef _UTILS_MENU_H_
#define _UTILS_MENU_H_

#include "utils/InputState.h"
#include "utils/SettingsStore.h"

#include <map>
#include <memory>
#include <stack>
#include <string>
#include <vector>

namespace Dancecon::Utils {

enum class MenuPage {
    Main,

    DeviceMode,
    Pad,
    Led,
    Reset,
    Bootsel,

    PadCalibrate,
    PadDebounceDelay,
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
    LedEnablePlayerColor,

    BootselMsg,
};

struct MenuState {
    MenuPage page;
    uint16_t selected_value;
    uint16_t original_value;
};

struct MenuDescriptor {
    enum class Type {
        Menu,
        Selection,
        Value,
        Toggle,
        RebootInfo,
    };

    enum class Action {
        None,
        GotoParent,

        GotoPageDeviceMode,
        GotoPagePad,
        GotoPageLed,
        GotoPageReset,
        GotoPageBootsel,

        GotoPagePadDebounceDelay,
        GotoPageTriggerThresholds,

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
        GotoPageLedEnablePlayerColor,

        SetUsbMode,

        SetPadDebounceDelay,
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
        SetLedEnablePlayerColor,

        DoCalibrate,
        DoReset,
        DoRebootToBootsel,
    };

    Type type;
    std::string name;
    std::vector<std::pair<std::string, Action>> items;
    uint16_t max_value;
};

template <typename TThresholds> class Menu {
  public:
    const static MenuDescriptor thresholds_descriptor;
    const static std::map<MenuPage, const MenuDescriptor> descriptors;

  private:
    std::shared_ptr<SettingsStore<TThresholds>> m_store;
    bool m_active;
    std::stack<MenuState> m_state_stack;

    uint16_t getCurrentValue(MenuPage page);
    uint16_t getCurrentThresholdValue(MenuPage page);

    void gotoPage(MenuPage page);

    void restoreThresholdValue();
    void gotoParent(bool do_restore);

    void performActionSetThresholdValue(MenuDescriptor::Action action, uint16_t value);
    void performAction(MenuDescriptor::Action action, uint16_t value);

  public:
    Menu(std::shared_ptr<SettingsStore<TThresholds>> settings_store);

    void activate();
    void update(const InputState::Controller &controller_state);
    bool active();
    MenuState getState();
};
} // namespace Dancecon::Utils

#endif // _UTILS_MENU_H_