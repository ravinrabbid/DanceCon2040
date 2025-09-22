#include "utils/SettingsStore.h"

#include "GlobalConfiguration.h"

#include "hardware/watchdog.h"
#include "pico/bootrom.h"
#include "pico/multicore.h"
#include "pico/stdio_usb.h"

namespace Dancecon::Utils {

template class SettingsStore<Config::Default::pad_config.PANEL_COUNT, Config::Default::led_config.PANEL_COUNT>;

namespace {
uint8_t read_byte(uint32_t offset) { return *(reinterpret_cast<uint8_t *>(XIP_BASE + offset)); }

} // namespace

template <size_t TPanelCount, size_t TPanelLedsCount>
SettingsStore<TPanelCount, TPanelLedsCount>::SettingsStore()
    : m_store_cache({m_magic_byte,
                     Config::Default::usb_mode,
                     Config::Default::pad_config.thresholds,
                     Config::Default::pad_config.debounce_delay_ms,
                     Config::Default::pad_config.hysteresis,
                     Config::Default::led_config.idle_colors,
                     Config::Default::led_config.active_colors,
                     Config::Default::led_config.brightness,
                     Config::Default::led_config.animation_speed,
                     Config::Default::led_config.idle_mode,
                     Config::Default::led_config.active_mode,
                     Config::Default::led_config.enable_player_color,
                     Config::Default::led_config.enable_hid_lights,
                     {}}),
      m_dirty(true), m_scheduled_reboot(RebootType::None) {
    uint32_t current_page = m_flash_offset + m_flash_size - m_store_size;
    bool found_valid = false;
    for (uint8_t i = 0; i < m_store_pages; ++i) {
        if (read_byte(current_page) == m_magic_byte) {
            found_valid = true;
            break;
        } else {
            current_page -= m_store_size;
        }
    }

    if (found_valid) {
        m_store_cache = *(reinterpret_cast<Storecache *>(XIP_BASE + current_page));
        m_dirty = false;
    }
}

template <size_t TPanelCount, size_t TPanelLedsCount>
void SettingsStore<TPanelCount, TPanelLedsCount>::setUsbMode(const usb_mode_t mode) {
    if (mode != m_store_cache.usb_mode) {
        m_store_cache.usb_mode = mode;
        m_dirty = true;

        scheduleReboot();
    }
}
template <size_t TPanelCount, size_t TPanelLedsCount>
usb_mode_t SettingsStore<TPanelCount, TPanelLedsCount>::getUsbMode() {
    return m_store_cache.usb_mode;
}

template <size_t TPanelCount, size_t TPanelLedsCount>
void SettingsStore<TPanelCount, TPanelLedsCount>::setTriggerThresholds(const thresholds_t &thresholds) {
    if (m_store_cache.trigger_thresholds != thresholds) {
        m_store_cache.trigger_thresholds = thresholds;
        m_dirty = true;
    }
}
template <size_t TPanelCount, size_t TPanelLedsCount>
SettingsStore<TPanelCount, TPanelLedsCount>::thresholds_t
SettingsStore<TPanelCount, TPanelLedsCount>::getTriggerThresholds() {
    return m_store_cache.trigger_thresholds;
}

template <size_t TPanelCount, size_t TPanelLedsCount>
void SettingsStore<TPanelCount, TPanelLedsCount>::setHysteresis(const uint16_t hysteresis) {
    if (m_store_cache.hysteresis != hysteresis) {
        m_store_cache.hysteresis = hysteresis;
        m_dirty = true;
    }
}
template <size_t TPanelCount, size_t TPanelLedsCount>
uint16_t SettingsStore<TPanelCount, TPanelLedsCount>::getHysteresis() {
    return m_store_cache.hysteresis;
}

template <size_t TPanelCount, size_t TPanelLedsCount>
void SettingsStore<TPanelCount, TPanelLedsCount>::setLedIdleColors(const led_colors_t &colors) {
    if (m_store_cache.led_idle_colors != colors) {
        m_store_cache.led_idle_colors = colors;
        m_dirty = true;
    }
}
template <size_t TPanelCount, size_t TPanelLedsCount>
SettingsStore<TPanelCount, TPanelLedsCount>::led_colors_t
SettingsStore<TPanelCount, TPanelLedsCount>::getLedIdleColors() {
    return m_store_cache.led_idle_colors;
}

template <size_t TPanelCount, size_t TPanelLedsCount>
void SettingsStore<TPanelCount, TPanelLedsCount>::setLedActiveColors(const led_colors_t &colors) {
    if (m_store_cache.led_active_colors != colors) {
        m_store_cache.led_active_colors = colors;
        m_dirty = true;
    }
}
template <size_t TPanelCount, size_t TPanelLedsCount>
SettingsStore<TPanelCount, TPanelLedsCount>::led_colors_t
SettingsStore<TPanelCount, TPanelLedsCount>::getLedActiveColors() {
    return m_store_cache.led_active_colors;
}

template <size_t TPanelCount, size_t TPanelLedsCount>
void SettingsStore<TPanelCount, TPanelLedsCount>::setLedBrightness(const uint8_t brightness) {
    if (m_store_cache.led_brightness != brightness) {
        m_store_cache.led_brightness = brightness;
        m_dirty = true;
    }
}
template <size_t TPanelCount, size_t TPanelLedsCount>
uint8_t SettingsStore<TPanelCount, TPanelLedsCount>::getLedBrightness() {
    return m_store_cache.led_brightness;
}

template <size_t TPanelCount, size_t TPanelLedsCount>
void SettingsStore<TPanelCount, TPanelLedsCount>::setLedAnimationSpeed(const uint8_t speed) {
    if (m_store_cache.led_animation_speed != speed) {
        m_store_cache.led_animation_speed = speed;
        m_dirty = true;
    }
}
template <size_t TPanelCount, size_t TPanelLedsCount>
uint8_t SettingsStore<TPanelCount, TPanelLedsCount>::getLedAnimationSpeed() {
    return m_store_cache.led_animation_speed;
}

template <size_t TPanelCount, size_t TPanelLedsCount>
void SettingsStore<TPanelCount, TPanelLedsCount>::setLedIdleMode(const idle_mode_t mode) {
    if (m_store_cache.led_idle_mode != mode) {
        m_store_cache.led_idle_mode = mode;
        m_dirty = true;
    }
}
template <size_t TPanelCount, size_t TPanelLedsCount>
SettingsStore<TPanelCount, TPanelLedsCount>::idle_mode_t SettingsStore<TPanelCount, TPanelLedsCount>::getLedIdleMode() {
    return m_store_cache.led_idle_mode;
}

template <size_t TPanelCount, size_t TPanelLedsCount>
void SettingsStore<TPanelCount, TPanelLedsCount>::setLedActiveMode(const active_mode_t mode) {
    if (m_store_cache.led_active_mode != mode) {
        m_store_cache.led_active_mode = mode;
        m_dirty = true;
    }
}
template <size_t TPanelCount, size_t TPanelLedsCount>
SettingsStore<TPanelCount, TPanelLedsCount>::active_mode_t
SettingsStore<TPanelCount, TPanelLedsCount>::getLedActiveMode() {
    return m_store_cache.led_active_mode;
}

template <size_t TPanelCount, size_t TPanelLedsCount>
void SettingsStore<TPanelCount, TPanelLedsCount>::setLedEnablePlayerColor(const bool do_enable) {
    if (m_store_cache.led_enable_player_color != do_enable) {
        m_store_cache.led_enable_player_color = do_enable;
        m_dirty = true;
    }
}
template <size_t TPanelCount, size_t TPanelLedsCount>
bool SettingsStore<TPanelCount, TPanelLedsCount>::getLedEnablePlayerColor() {
    return m_store_cache.led_enable_player_color;
}

template <size_t TPanelCount, size_t TPanelLedsCount>
void SettingsStore<TPanelCount, TPanelLedsCount>::setLedEnableHidLights(const bool do_enable) {
    if (m_store_cache.led_enable_hid_lights != do_enable) {
        m_store_cache.led_enable_hid_lights = do_enable;
        m_dirty = true;
    }
}
template <size_t TPanelCount, size_t TPanelLedsCount>
bool SettingsStore<TPanelCount, TPanelLedsCount>::getLedEnableHidLights() {
    return m_store_cache.led_enable_hid_lights;
}

template <size_t TPanelCount, size_t TPanelLedsCount>
void SettingsStore<TPanelCount, TPanelLedsCount>::setDebounceDelay(const uint16_t delay) {
    if (m_store_cache.debounce_delay != delay) {
        m_store_cache.debounce_delay = delay;
        m_dirty = true;
    }
}
template <size_t TPanelCount, size_t TPanelLedsCount>
uint16_t SettingsStore<TPanelCount, TPanelLedsCount>::getDebounceDelay() {
    return m_store_cache.debounce_delay;
}

template <size_t TPanelCount, size_t TPanelLedsCount> void SettingsStore<TPanelCount, TPanelLedsCount>::store() {
    if (m_dirty) {
        multicore_lockout_start_blocking();
        uint32_t interrupts = save_and_disable_interrupts();

        uint32_t current_page = m_flash_offset;
        bool do_erase = true;
        for (uint8_t i = 0; i < m_store_pages; ++i) {
            if (read_byte(current_page) == 0xFF) {
                do_erase = false;
                break;
            } else {
                current_page += m_store_size;
            }
        }

        if (do_erase) {
            flash_range_erase(m_flash_offset, m_flash_size);
            current_page = m_flash_offset;
        }

        flash_range_program(current_page, reinterpret_cast<uint8_t *>(&m_store_cache), sizeof(m_store_cache));

        m_dirty = false;

        restore_interrupts_from_disabled(interrupts);
        multicore_lockout_end_blocking();
    }

    switch (m_scheduled_reboot) {
    case RebootType::Normal:
        watchdog_reboot(0, 0, 1);
        break;
    case RebootType::Bootsel:
        sleep_ms(100);
        reset_usb_boot(0, PICO_STDIO_USB_RESET_BOOTSEL_INTERFACE_DISABLE_MASK);
        break;
    case RebootType::None:
        break;
    }
}

template <size_t TPanelCount, size_t TPanelLedsCount> void SettingsStore<TPanelCount, TPanelLedsCount>::reset() {
    multicore_lockout_start_blocking();
    uint32_t interrupts = save_and_disable_interrupts();

    flash_range_erase(m_flash_offset, m_flash_size);

    restore_interrupts(interrupts);
    multicore_lockout_end_blocking();

    m_dirty = false;

    scheduleReboot();
    store();
}

template <size_t TPanelCount, size_t TPanelLedsCount>
void SettingsStore<TPanelCount, TPanelLedsCount>::scheduleReboot(const bool bootsel) {
    if (m_scheduled_reboot != RebootType::Bootsel) {
        m_scheduled_reboot = (bootsel ? RebootType::Bootsel : RebootType::Normal);
    }
}

} // namespace Dancecon::Utils
