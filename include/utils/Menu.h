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

class Menu {
  public:
    enum class Page {
        Main,

        DeviceMode,
        Pad,
        Led,
        Reset,
        Bootsel,

        PadDebounceDelay,
        PadTriggerThresholdUp,
        PadTriggerThresholDown,
        PadTriggerThresholdLeft,
        PadTriggerThresholdRight,

        LedBrightness,
        LedEnablePlayerColor,

        BootselMsg,
    };

    struct State {
        Page page;
        uint16_t selected_value;
        uint16_t original_value;
    };

    struct Descriptor {
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
            GotoPagePadTriggerThresholdUp,
            GotoPagePadTriggerThresholdDown,
            GotoPagePadTriggerThresholdLeft,
            GotoPagePadTriggerThresholdRight,

            GotoPageLedBrightness,
            GotoPageLedEnablePlayerColor,

            SetUsbMode,

            SetPadDebounceDelay,
            SetPadTriggerThresholdUp,
            SetPadTriggerThresholdDown,
            SetPadTriggerThresholdLeft,
            SetPadTriggerThresholdRight,

            SetLedBrightness,
            SetLedEnablePlayerColor,

            DoReset,
            DoRebootToBootsel,
        };

        Type type;
        std::string name;
        std::vector<std::pair<std::string, Action>> items;
        uint16_t max_value;
    };

    const static std::map<Page, const Descriptor> descriptors;

  private:
    std::shared_ptr<SettingsStore> m_store;
    bool m_active;
    std::stack<State> m_state_stack;

    uint16_t getCurrentValue(Page page);
    void gotoPage(Page page);
    void gotoParent(bool do_restore);

    void performAction(Descriptor::Action action, uint8_t value);

  public:
    Menu(std::shared_ptr<SettingsStore> settings_store);

    void activate();
    void update(const InputState::Controller &controller_state);
    bool active();
    State getState();
};
} // namespace Dancecon::Utils

#endif // _UTILS_MENU_H_