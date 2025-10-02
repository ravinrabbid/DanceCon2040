#ifndef UTILS_MENU_H_
#define UTILS_MENU_H_

#include "utils/InputState.h"
#include "utils/MenuTypes.h"
#include "utils/SettingsStore.h"

#include <functional>
#include <map>
#include <memory>
#include <stack>
#include <string>
#include <vector>

namespace Dancecon::Utils {

template <size_t TPanelCount, size_t TPanelLedsCount> class Menu {
  public:
    using calibration_callback_t = std::function<void(void)>;

    const static std::map<MenuPage, const MenuDescriptor> descriptors;

  private:
    class Buttons {
      public:
        enum class Id : uint8_t { Up, Down, Left, Right, Confirm, Back };

      private:
        struct State {
            enum class Repeat : uint8_t {
                Idle,
                RepeatDelay,
                Repeat,
                FastRepeat,
            };
            Repeat repeat;
            uint32_t pressed_since;
            uint32_t last_repeat;
            bool pressed;
        };

        std::map<Id, State> m_states;

      public:
        Buttons();

        void update(const InputState::Controller &state);
        [[nodiscard]] bool getPressed(Id id) const;
    };

    std::shared_ptr<SettingsStore<TPanelCount, TPanelLedsCount>> m_store;
    Buttons m_buttons;
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
    [[nodiscard]] bool active() const;
    [[nodiscard]] MenuState getState() const;
};

} // namespace Dancecon::Utils

#endif // UTILS_MENU_H_