#include "peripherals/Controller.h"
#include "peripherals/Display.h"
#include "peripherals/PadButtons.h"
#include "usb/device/hid/ps4_auth.h"
#include "usb/device_driver.h"
#include "utils/InputReport.h"
#include "utils/InputState.h"
#include "utils/Menu.h"
#include "utils/PS4AuthProvider.h"
#include "utils/SettingsStore.h"

#include "GlobalConfiguration.h"
#include "PS4AuthConfiguration.h"

#include "pico/multicore.h"
#include "pico/util/queue.h"

#include <cstdio>

using namespace Dancecon;

namespace {

queue_t control_queue;
queue_t menu_display_queue;
queue_t pad_input_queue;
queue_t controller_input_queue;
queue_t panel_led_queue;

queue_t auth_challenge_queue;
queue_t auth_signed_challenge_queue;

enum class ControlCommand : uint8_t {
    SetUsbMode,
    SetPlayerLed,
    SetLedBrightness,
    SetLedAnimationSpeed,
    SetLedIdleMode,
    SetLedIdleColors,
    SetLedActiveMode,
    SetLedActiveColors,
    SetLedEnablePlayerColor,
    SetDisplayWeightUnit,
    EnterMenu,
    ExitMenu,
};

struct ControlMessage {
    ControlCommand command;
    union {
        usb_mode_t usb_mode;
        usb_player_led_t player_led;
        uint8_t led_brightness;
        uint8_t led_animation_speed;
        Peripherals::PanelLeds<Config::Default::led_config.PANEL_COUNT>::Config::IdleMode led_idle_mode;
        Peripherals::PanelLeds<Config::Default::led_config.PANEL_COUNT>::Config::PanelColors led_idle_colors;
        Peripherals::PanelLeds<Config::Default::led_config.PANEL_COUNT>::Config::ActiveMode led_active_mode;
        Peripherals::PanelLeds<Config::Default::led_config.PANEL_COUNT>::Config::PanelColors led_active_colors;
        bool led_enable_player_color;
        Peripherals::Display::WeightUnit display_weight_unit;
    } data;
};

void core1_task() {
    multicore_lockout_victim_init();

    // Init i2c port here because Controller and Display share it and
    // therefore can't init it themself.
    gpio_set_function(Config::Default::i2c_config.sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(Config::Default::i2c_config.scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(Config::Default::i2c_config.sda_pin);
    gpio_pull_up(Config::Default::i2c_config.scl_pin);
    i2c_init(Config::Default::i2c_config.block, Config::Default::i2c_config.speed_hz);

    const Peripherals::StatusLed status_led(Config::Default::status_led_config);
    Peripherals::Controller controller(Config::Default::controller_config);
    Peripherals::PanelLeds<Config::Default::led_config.PANEL_COUNT> led(Config::Default::led_config);
    Peripherals::Display display(Config::Default::display_config);

    Utils::PS4AuthProvider ps4authprovider;
    std::array<uint8_t, Utils::PS4AuthProvider::SIGNATURE_LENGTH> auth_challenge{};

    Utils::InputState input_state{};
    Utils::MenuState menu_display_msg{};
    ControlMessage control_msg{};
    usb_panel_led_t panel_led{};

    while (true) {
        controller.updateInputState(input_state);

        queue_try_add(&controller_input_queue, &input_state.controller);
        queue_try_remove(&pad_input_queue, &input_state.pad);

        if (queue_try_remove(&panel_led_queue, &panel_led)) {
            led.update(panel_led);
        }

        if (queue_try_remove(&control_queue, &control_msg)) {
            switch (control_msg.command) {
            case ControlCommand::SetUsbMode:
                display.setUsbMode(control_msg.data.usb_mode);
                break;
            case ControlCommand::SetPlayerLed:
                status_led.setPlayer(control_msg.data.player_led);

                switch (control_msg.data.player_led.type) {
                case USB_PLAYER_LED_ID:
                    display.setPlayerId(control_msg.data.player_led.id);
                    break;
                case USB_PLAYER_LED_COLOR:
                    led.setPlayerColor({.r = control_msg.data.player_led.red,
                                        .g = control_msg.data.player_led.green,
                                        .b = control_msg.data.player_led.blue});
                    break;
                }
                break;
            case ControlCommand::SetLedBrightness:
                led.setBrightness(control_msg.data.led_brightness);
                break;
            case ControlCommand::SetLedAnimationSpeed:
                led.setAnimationSpeed(control_msg.data.led_animation_speed);
                break;
            case ControlCommand::SetLedIdleMode:
                led.setIdleMode(control_msg.data.led_idle_mode);
                break;
            case ControlCommand::SetLedIdleColors:
                led.setIdleColors(control_msg.data.led_idle_colors);
                break;
            case ControlCommand::SetLedActiveMode:
                led.setActiveMode(control_msg.data.led_active_mode);
                break;
            case ControlCommand::SetLedActiveColors:
                led.setActiveColors(control_msg.data.led_active_colors);
                break;
            case ControlCommand::SetLedEnablePlayerColor:
                led.setEnablePlayerColor(control_msg.data.led_enable_player_color);
                break;
            case ControlCommand::SetDisplayWeightUnit:
                display.setWeightUnit(control_msg.data.display_weight_unit);
                break;
            case ControlCommand::EnterMenu:
                display.showMenu();
                break;
            case ControlCommand::ExitMenu:
                display.showIdle();
                break;
            }
        }
        if (queue_try_remove(&menu_display_queue, &menu_display_msg)) {
            display.setMenuState(menu_display_msg);
        }
        if (queue_try_remove(&auth_challenge_queue, auth_challenge.data())) {
            const auto signed_challenge = ps4authprovider.sign(auth_challenge);
            queue_try_remove(&auth_signed_challenge_queue, nullptr); // clear queue first
            queue_try_add(&auth_signed_challenge_queue, &signed_challenge);
        }

        led.setInputState(input_state);
        display.setInputState(input_state);

        led.update();
        display.update();
    }
}

} // namespace

int main() {
    queue_init(&control_queue, sizeof(ControlMessage), 1);
    queue_init(&menu_display_queue, sizeof(Utils::MenuState), 1);
    queue_init(&pad_input_queue, sizeof(Utils::InputState::Pad), 1);
    queue_init(&controller_input_queue, sizeof(Utils::InputState::Controller), 1);
    queue_init(&panel_led_queue, sizeof(usb_panel_led_t), 1);
    queue_init(&auth_challenge_queue, sizeof(std::array<uint8_t, Utils::PS4AuthProvider::SIGNATURE_LENGTH>), 1);
    queue_init(&auth_signed_challenge_queue, sizeof(std::array<uint8_t, Utils::PS4AuthProvider::SIGNATURE_LENGTH>), 1);

    stdio_init_all();

    Peripherals::PadButtons pad_buttons(Config::Default::pad_buttons_config);
    Peripherals::Pad<Config::Default::pad_config.PANEL_COUNT> pad(Config::Default::pad_config);

    Utils::InputState input_state{};
    Utils::InputReport input_report;
    const auto checkHotkey = [&input_state]() {
        static const uint32_t hold_timeout = 2000;
        static uint32_t hold_since = 0;
        static bool hold_active = false;

        if (input_state.controller.buttons.start && input_state.controller.buttons.select) {
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
    };

    auto settings_store = std::make_shared<
        Utils::SettingsStore<Config::Default::pad_config.PANEL_COUNT, Config::Default::led_config.PANEL_COUNT>>();
    const auto mode = settings_store->getUsbMode();
    const auto readSettings = [&]() {
        const auto sendCtrlMessage = [&](const ControlMessage &msg) { queue_add_blocking(&control_queue, &msg); };

        sendCtrlMessage({.command = ControlCommand::SetUsbMode, .data = {.usb_mode = mode}});
        sendCtrlMessage({.command = ControlCommand::SetLedBrightness,
                         .data = {.led_brightness = settings_store->getLedBrightness()}});
        sendCtrlMessage({.command = ControlCommand::SetLedAnimationSpeed,
                         .data = {.led_animation_speed = settings_store->getLedAnimationSpeed()}});
        sendCtrlMessage(
            {.command = ControlCommand::SetLedIdleMode, .data = {.led_idle_mode = settings_store->getLedIdleMode()}});
        sendCtrlMessage({.command = ControlCommand::SetLedIdleColors,
                         .data = {.led_idle_colors = settings_store->getLedIdleColors()}});
        sendCtrlMessage({.command = ControlCommand::SetLedActiveMode,
                         .data = {.led_active_mode = settings_store->getLedActiveMode()}});
        sendCtrlMessage({.command = ControlCommand::SetLedActiveColors,
                         .data = {.led_active_colors = settings_store->getLedActiveColors()}});
        sendCtrlMessage({.command = ControlCommand::SetLedEnablePlayerColor,
                         .data = {.led_enable_player_color = settings_store->getLedEnablePlayerColor()}});
        sendCtrlMessage({.command = ControlCommand::SetDisplayWeightUnit,
                         .data = {.display_weight_unit = settings_store->getDisplayWeightUnit()}});

        pad.setDebounceDelay(settings_store->getDebounceDelay());
        pad.setThresholds(settings_store->getTriggerThresholds());
        pad.setHysteresis(settings_store->getHysteresis());
    };

    Utils::Menu menu(settings_store, [&pad]() { pad.calibrate(); });

    std::array<uint8_t, Utils::PS4AuthProvider::SIGNATURE_LENGTH> auth_challenge_response{};
    if (Config::PS4Auth::config.enabled) {
        ps4_auth_init(Config::PS4Auth::config.key_pem.c_str(), Config::PS4Auth::config.key_pem.size() + 1,
                      Config::PS4Auth::config.serial.data(), Config::PS4Auth::config.signature.data(),
                      [](const uint8_t *challenge) { queue_try_add(&auth_challenge_queue, challenge); });
    }

    multicore_launch_core1(core1_task);

    usbd_driver_init(mode);
    usbd_driver_set_player_led_cb([](usb_player_led_t player_led) {
        const auto ctrl_message =
            ControlMessage{.command = ControlCommand::SetPlayerLed, .data = {.player_led = player_led}};
        queue_try_add(&control_queue, &ctrl_message);
    });
    usbd_driver_set_panel_led_cb([](usb_panel_led_t panel_led) { queue_try_add(&panel_led_queue, &panel_led); });

    readSettings();
    pad.calibrate();

    while (true) {
        pad_buttons.updateInputState(input_state);
        pad.updateInputState(input_state);
        queue_try_remove(&controller_input_queue, &input_state.controller);

        const auto pad_message = input_state.pad;

        if (menu.active()) {
            menu.update(input_state.controller);
            if (menu.active()) {
                const auto display_msg = menu.getState();
                queue_add_blocking(&menu_display_queue, &display_msg);
            } else {
                settings_store->store();

                ControlMessage ctrl_message = {.command = ControlCommand::ExitMenu, .data = {}};
                queue_add_blocking(&control_queue, &ctrl_message);
            }

            readSettings();
            input_state.releaseAll();

        } else if (checkHotkey()) {
            menu.activate();

            ControlMessage ctrl_message{.command = ControlCommand::EnterMenu, .data = {}};
            queue_add_blocking(&control_queue, &ctrl_message);
        }

        usbd_driver_send_report(input_report.getUsbReport(input_state, mode));
        usbd_driver_task();

        queue_try_add(&pad_input_queue, &pad_message);

        if (queue_try_remove(&auth_signed_challenge_queue, auth_challenge_response.data())) {
            ps4_auth_set_signed_challenge(auth_challenge_response.data());
        }
    }

    return 0;
}