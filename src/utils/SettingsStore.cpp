#include "utils/SettingsStore.h"

#include "GlobalConfiguration.h"

#include "hardware/watchdog.h"
#include "pico/bootrom.h"
#include "pico/multicore.h"
#include "pico/stdio_usb.h"

namespace Dancecon::Utils {

template class SettingsStore<decltype(Config::Default::pad_config)::Thresholds>;

static uint8_t read_byte(uint32_t offset) { return *(reinterpret_cast<uint8_t *>(XIP_BASE + offset)); }

template <typename TThresholds>
SettingsStore<TThresholds>::SettingsStore()
    : m_store_cache({m_magic_byte,
                     Config::Default::usb_mode,
                     Config::Default::pad_config.thresholds,
                     Config::Default::pad_config.debounce_delay_ms,
                     Config::Default::led_config.brightness,
                     Config::Default::led_config.enable_player_color,
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

template <typename TThresholds> void SettingsStore<TThresholds>::setUsbMode(const usb_mode_t mode) {
    if (mode != m_store_cache.usb_mode) {
        m_store_cache.usb_mode = mode;
        m_dirty = true;

        scheduleReboot();
    }
}

template <typename TThresholds> usb_mode_t SettingsStore<TThresholds>::getUsbMode() { return m_store_cache.usb_mode; }

template <typename TThresholds> void SettingsStore<TThresholds>::setTriggerThresholds(const TThresholds &thresholds) {
    (void)thresholds;
    // if (m_store_cache.trigger_thresholds.up != thresholds.up ||
    //     m_store_cache.trigger_thresholds.down != thresholds.down ||
    //     m_store_cache.trigger_thresholds.left != thresholds.left ||
    //     m_store_cache.trigger_thresholds.right != thresholds.right) {

    //     m_store_cache.trigger_thresholds = thresholds;
    //     m_dirty = true;
    // }
}
template <typename TThresholds> TThresholds SettingsStore<TThresholds>::getTriggerThresholds() {
    return m_store_cache.trigger_thresholds;
}

template <typename TThresholds> void SettingsStore<TThresholds>::setLedBrightness(const uint8_t brightness) {
    if (m_store_cache.led_brightness != brightness) {
        m_store_cache.led_brightness = brightness;
        m_dirty = true;
    }
}
template <typename TThresholds> uint8_t SettingsStore<TThresholds>::getLedBrightness() {
    return m_store_cache.led_brightness;
}

template <typename TThresholds> void SettingsStore<TThresholds>::setLedEnablePlayerColor(const bool do_enable) {
    if (m_store_cache.led_enable_player_color != do_enable) {
        m_store_cache.led_enable_player_color = do_enable;
        m_dirty = true;
    }
}
template <typename TThresholds> bool SettingsStore<TThresholds>::getLedEnablePlayerColor() {
    return m_store_cache.led_enable_player_color;
}

template <typename TThresholds> void SettingsStore<TThresholds>::setDebounceDelay(const uint16_t delay) {
    if (m_store_cache.debounce_delay != delay) {
        m_store_cache.debounce_delay = delay;
        m_dirty = true;
    }
}
template <typename TThresholds> uint16_t SettingsStore<TThresholds>::getDebounceDelay() {
    return m_store_cache.debounce_delay;
}

template <typename TThresholds> void SettingsStore<TThresholds>::store() {
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

template <typename TThresholds> void SettingsStore<TThresholds>::reset() {
    multicore_lockout_start_blocking();
    uint32_t interrupts = save_and_disable_interrupts();

    flash_range_erase(m_flash_offset, m_flash_size);

    restore_interrupts(interrupts);
    multicore_lockout_end_blocking();

    m_dirty = false;

    scheduleReboot();
    store();
}

template <typename TThresholds> void SettingsStore<TThresholds>::scheduleReboot(const bool bootsel) {
    if (m_scheduled_reboot != RebootType::Bootsel) {
        m_scheduled_reboot = (bootsel ? RebootType::Bootsel : RebootType::Normal);
    }
}

} // namespace Dancecon::Utils
