#ifndef UTILS_INPUTSTATE_H_
#define UTILS_INPUTSTATE_H_

#include <cstdint>

namespace Dancecon::Utils {

struct InputState {
    struct Pad {
        struct Panel {
            bool triggered;
            uint16_t raw;
        };

        Panel up_left, up, up_right, left, center, right, down_left, down, down_right;
        bool start, select;
    };

    struct Controller {
        struct DPad {
            bool up, down, left, right;
        };

        struct Buttons {
            bool north, east, south, west;
            bool l, r;
            bool start, select, home, share;
        };

        DPad dpad;
        Buttons buttons;
    };

    Pad pad;
    Controller controller;

    void releaseAll() {
        pad = {};
        controller = {};
    }
};

} // namespace Dancecon::Utils

#endif // UTILS_INPUTSTATE_H_