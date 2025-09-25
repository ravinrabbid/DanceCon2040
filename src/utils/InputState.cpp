#include "utils/InputState.h"

#include "pico/time.h"

namespace Dancecon::Utils {

void InputState::releaseAll() {
    pad = {};
    controller = {};
}

bool InputState::checkHotkey() const {
    static uint32_t hold_since = 0;
    static bool hold_active = false;
    static const uint32_t hold_timeout = 2000;

    if (controller.buttons.start && controller.buttons.select) {
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
