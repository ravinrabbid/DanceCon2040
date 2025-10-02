#ifndef UTILS_SETTINGSSTORE_H_
#define UTILS_SETTINGSSTORE_H_

#include "peripherals/Display.h"
#include "peripherals/Pad.h"
#include "peripherals/PanelLeds.h"
#include "usb/device_driver.h"

#include "hardware/flash.h"

#include <array>

namespace Dancecon::Utils {

template <size_t TPanelCount, size_t TPanelLedsCount> class SettingsStore {
  private:
    using thresholds_t = Peripherals::Pad<TPanelCount>::Config::Thresholds;
    using led_colors_t = Peripherals::PanelLeds<TPanelLedsCount>::Config::PanelColors;
    using idle_mode_t = Peripherals::PanelLeds<TPanelLedsCount>::Config::IdleMode;
    using active_mode_t = Peripherals::PanelLeds<TPanelLedsCount>::Config::ActiveMode;

    const static uint32_t m_flash_size = FLASH_SECTOR_SIZE;
    const static uint32_t m_flash_offset = PICO_FLASH_SIZE_BYTES - m_flash_size;
    const static uint32_t m_store_size = FLASH_PAGE_SIZE;
    const static uint32_t m_store_pages = m_flash_size / m_store_size;
    const static uint8_t m_magic_byte = 0x39;

    struct __attribute((packed, aligned(1))) Storecache {
        uint8_t in_use;
        usb_mode_t usb_mode;
        thresholds_t trigger_thresholds;
        uint16_t hysteresis;
        uint16_t debounce_delay;
        led_colors_t led_idle_colors;
        led_colors_t led_active_colors;
        uint8_t led_brightness;
        uint8_t led_animation_speed;
        idle_mode_t led_idle_mode;
        active_mode_t led_active_mode;
        bool led_enable_player_color;
        bool led_enable_hid_lights;
        Peripherals::Display::WeightUnit display_weight_unit;

        std::array<uint8_t, m_store_size - sizeof(uint8_t) - sizeof(usb_mode_t) - sizeof(thresholds_t) -
                                sizeof(uint16_t) - sizeof(uint16_t) - sizeof(led_colors_t) - sizeof(led_colors_t) -
                                sizeof(uint8_t) - sizeof(uint8_t) - sizeof(idle_mode_t) - sizeof(active_mode_t) -
                                sizeof(bool) - sizeof(bool) - sizeof(Peripherals::Display::WeightUnit)>
            _padding;
    };
    static_assert(sizeof(Storecache) == m_store_size);

    enum class RebootType : uint8_t {
        None,
        Normal,
        Bootsel,
    };

    Storecache m_store_cache;
    bool m_dirty{true};
    RebootType m_scheduled_reboot{RebootType::None};

    Storecache read();

  public:
    SettingsStore();

    void setUsbMode(usb_mode_t mode);
    [[nodiscard]] usb_mode_t getUsbMode() const;

    void setTriggerThresholds(const thresholds_t &thresholds);
    [[nodiscard]] thresholds_t getTriggerThresholds() const;

    void setHysteresis(uint16_t hysteresis);
    [[nodiscard]] uint16_t getHysteresis() const;

    void setLedIdleColors(const led_colors_t &colors);
    [[nodiscard]] led_colors_t getLedIdleColors() const;

    void setLedActiveColors(const led_colors_t &colors);
    [[nodiscard]] led_colors_t getLedActiveColors() const;

    void setLedBrightness(uint8_t brightness);
    [[nodiscard]] uint8_t getLedBrightness() const;

    void setLedAnimationSpeed(uint8_t speed);
    [[nodiscard]] uint8_t getLedAnimationSpeed() const;

    void setLedIdleMode(idle_mode_t mode);
    [[nodiscard]] idle_mode_t getLedIdleMode() const;

    void setLedActiveMode(active_mode_t mode);
    [[nodiscard]] active_mode_t getLedActiveMode() const;

    void setLedEnablePlayerColor(bool do_enable);
    [[nodiscard]] bool getLedEnablePlayerColor() const;

    void setLedEnableHidLights(bool do_enable);
    [[nodiscard]] bool getLedEnableHidLights() const;

    void setDebounceDelay(uint16_t delay);
    [[nodiscard]] uint16_t getDebounceDelay() const;

    void setDisplayWeightUnit(Peripherals::Display::WeightUnit unit);
    [[nodiscard]] Peripherals::Display::WeightUnit getDisplayWeightUnit() const;

    void scheduleReboot(bool bootsel = false);

    void store();
    void reset();
};
} // namespace Dancecon::Utils

#endif // UTILS_SETTINGSSTORE_H_