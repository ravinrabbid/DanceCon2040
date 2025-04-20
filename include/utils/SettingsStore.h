#ifndef _UTILS_SETTINGSSTORE_H_
#define _UTILS_SETTINGSSTORE_H_

#include "peripherals/Pad.h"
#include "usb/device_driver.h"

#include "hardware/flash.h"

namespace Dancecon::Utils {

class SettingsStore {
  private:
    const static uint32_t m_flash_size = FLASH_SECTOR_SIZE;
    const static uint32_t m_flash_offset = PICO_FLASH_SIZE_BYTES - m_flash_size;
    const static uint32_t m_store_size = FLASH_PAGE_SIZE;
    const static uint32_t m_store_pages = m_flash_size / m_store_size;
    const static uint8_t m_magic_byte = 0x39;

    struct __attribute((packed, aligned(1))) Storecache {
        uint8_t in_use;
        usb_mode_t usb_mode;
        Peripherals::Pad::Config::Thresholds trigger_thresholds;
        uint16_t debounce_delay;
        uint8_t led_brightness;
        bool led_enable_player_color;

        uint8_t _padding[m_store_size - sizeof(uint8_t) - sizeof(usb_mode_t) -
                         sizeof(Peripherals::Pad::Config::Thresholds) - sizeof(uint8_t) - sizeof(bool) -
                         sizeof(uint16_t)];
    };
    static_assert(sizeof(Storecache) == m_store_size);

    enum class RebootType {
        None,
        Normal,
        Bootsel,
    };

    Storecache m_store_cache;
    bool m_dirty;

    RebootType m_scheduled_reboot;

  private:
    Storecache read();

  public:
    SettingsStore();

    void setUsbMode(const usb_mode_t mode);
    usb_mode_t getUsbMode();

    void setTriggerThresholds(const Peripherals::Pad::Config::Thresholds &thresholds);
    Peripherals::Pad::Config::Thresholds getTriggerThresholds();

    void setLedBrightness(const uint8_t brightness);
    uint8_t getLedBrightness();

    void setLedEnablePlayerColor(const bool do_enable);
    bool getLedEnablePlayerColor();

    void setDebounceDelay(const uint16_t delay);
    uint16_t getDebounceDelay();

    void scheduleReboot(const bool bootsel = false);

    void store();
    void reset();
};
} // namespace Dancecon::Utils

#endif // _UTILS_SETTINGSSTORE_H_