#include "peripherals/Pad.h"

#include "GlobalConfiguration.h"

#include "hardware/adc.h"
#include "hardware/gpio.h"

#include <algorithm>
#include <limits>

namespace Dancecon::Peripherals {

template class Pad<Config::Default::pad_config.PANEL_COUNT>;

template <size_t TPanelCount> Pad<TPanelCount>::GpioAdc::GpioAdc(const Config::GpioAdc &config) : m_config(config) {
    for (size_t pin_idx = 0; pin_idx < TPanelCount; ++pin_idx) {
        gpio_init(m_config.base_pin + pin_idx);
        gpio_set_dir(m_config.base_pin + pin_idx, GPIO_IN);
        gpio_pull_up(m_config.base_pin + pin_idx);
    }
}

template <size_t TPanelCount> std::array<uint16_t, TPanelCount> Pad<TPanelCount>::GpioAdc::read() {
    std::array<uint16_t, TPanelCount> result{};

    const uint32_t gpio_state = ~gpio_get_all();
    for (size_t pin_idx = 0; pin_idx < TPanelCount; ++pin_idx) {
        result.at(pin_idx) = (gpio_state & (1 << (m_config.base_pin + pin_idx))) ? UINT16_MAX : 0;
    }

    return result;
}

template <> Pad<4>::InternalAdc::InternalAdc(const Config::InternalAdc &config) : m_config(config) {
    static const uint adc_base_pin = 26;

    for (uint pin = adc_base_pin; pin < adc_base_pin + 4; ++pin) {
        adc_gpio_init(pin);
    }

    adc_init();
}

template <> std::array<uint16_t, 4> Pad<4>::InternalAdc::read() {
    if (m_config.sample_count == 0) {
        return {};
    }

    // Oversample ADC inputs to get rid of ADC noise
    std::array<uint32_t, 4> values{};
    for (uint8_t sample_number = 0; sample_number < m_config.sample_count; ++sample_number) {
        for (size_t idx = 0; idx < values.size(); ++idx) {
            adc_select_input(idx);
            values.at(idx) += adc_read();
        }
    }

    // Take average of all samples
    std::array<uint16_t, 4> result{};
    std::ranges::transform(values, result.begin(), [&](const auto &sample) { return sample / m_config.sample_count; });

    return result;
}

template <size_t TPanelCount>
template <size_t TAdcCount>
Pad<TPanelCount>::ExternalAdc<TAdcCount>::ExternalAdc(const Config::template ExternalAdc<TAdcCount> &config) {
    // Configure SPI
    gpio_set_function(config.spi.miso_pin, GPIO_FUNC_SPI);
    gpio_set_function(config.spi.mosi_pin, GPIO_FUNC_SPI);
    gpio_set_function(config.spi.sclk_pin, GPIO_FUNC_SPI);
    spi_init(config.spi.block, config.spi.speed_hz);
    spi_set_format(config.spi.block, 8, SPI_CPOL_0, SPI_CPHA_1, SPI_MSB_FIRST);

    std::transform(config.adcs.begin(), config.adcs.end(), m_ads124s06.begin(), [&](const auto &adc_config) {
        gpio_init(adc_config.scsn_pin);
        gpio_set_dir(adc_config.scsn_pin, GPIO_OUT);
        gpio_put(adc_config.scsn_pin, true);

        gpio_init(adc_config.drdy_pin);
        gpio_set_dir(adc_config.drdy_pin, GPIO_IN);

        auto adc = std::make_unique<Ads124S0x>(config.spi.block, adc_config.scsn_pin, adc_config.drdy_pin);

        return std::move(adc);
    });

    for (auto &adc : m_ads124s06) {
        adc->reset();

        adc->setConversionMode(Ads124S0x::ConversionMode::Single);
        adc->setFilter(config.filter);
        adc->setGain(config.gain);
        adc->setSampleRate(config.sample_rate);
        adc->setConversionDelay(Ads124S0x::ConversionDelay::D14);
        adc->setReferenceInput(Ads124S0x::ReferenceInput::External0);
        adc->setPositiveReferenceBuffer(false);
        adc->setNegativeReferenceBuffer(false);
    }
}

template <size_t TPanelCount>
template <size_t TAdcCount>
std::array<uint16_t, TPanelCount> Pad<TPanelCount>::ExternalAdc<TAdcCount>::read() {
    std::array<uint16_t, TPanelCount> result = {};

    auto read_unsigend = [](const auto &adc) {
        // Move signed into full unsigend range and discard lowest byte
        return ((1U + std::numeric_limits<int16_t>::max()) + adc->readBlocking()) >> 8;
    };

    if constexpr (TPanelCount == 4) {
        m_ads124s06[0]->start(Ads124S0x::Channel::CH0);
        m_ads124s06[1]->start(Ads124S0x::Channel::CH0);

        result[0] = read_unsigend(m_ads124s06[0]);
        m_ads124s06[0]->start(Ads124S0x::Channel::CH1);
        result[2] = read_unsigend(m_ads124s06[1]);
        m_ads124s06[1]->start(Ads124S0x::Channel::CH1);

        result[1] = read_unsigend(m_ads124s06[0]);
        result[3] = read_unsigend(m_ads124s06[1]);
    } else if constexpr (TPanelCount == 5) {
        if constexpr (TAdcCount == 2) {
            m_ads124s06[0]->start(Ads124S0x::Channel::CH0);
            m_ads124s06[1]->start(Ads124S0x::Channel::CH0);

            result[0] = read_unsigend(m_ads124s06[0]);
            m_ads124s06[0]->start(Ads124S0x::Channel::CH1);
            result[3] = read_unsigend(m_ads124s06[1]);
            m_ads124s06[1]->start(Ads124S0x::Channel::CH1);

            result[1] = read_unsigend(m_ads124s06[0]);
            m_ads124s06[0]->start(Ads124S0x::Channel::CH2);
            result[4] = read_unsigend(m_ads124s06[1]);

            result[2] = read_unsigend(m_ads124s06[0]);
        } else if constexpr (TAdcCount == 3) {
            m_ads124s06[0]->start(Ads124S0x::Channel::CH0);
            m_ads124s06[1]->start(Ads124S0x::Channel::CH0);
            m_ads124s06[2]->start(Ads124S0x::Channel::CH0);

            result[0] = read_unsigend(m_ads124s06[0]);
            m_ads124s06[0]->start(Ads124S0x::Channel::CH1);
            result[2] = read_unsigend(m_ads124s06[1]);
            result[3] = read_unsigend(m_ads124s06[2]);
            m_ads124s06[2]->start(Ads124S0x::Channel::CH1);

            result[1] = read_unsigend(m_ads124s06[0]);
            result[4] = read_unsigend(m_ads124s06[2]);
        }
    } else if constexpr (TPanelCount == 6) {
        if constexpr (TAdcCount == 2) {
            m_ads124s06[0]->start(Ads124S0x::Channel::CH0);
            m_ads124s06[1]->start(Ads124S0x::Channel::CH0);

            result[0] = read_unsigend(m_ads124s06[0]);
            m_ads124s06[0]->start(Ads124S0x::Channel::CH1);
            result[3] = read_unsigend(m_ads124s06[1]);
            m_ads124s06[1]->start(Ads124S0x::Channel::CH1);

            result[1] = read_unsigend(m_ads124s06[0]);
            m_ads124s06[0]->start(Ads124S0x::Channel::CH2);
            result[4] = read_unsigend(m_ads124s06[1]);
            m_ads124s06[1]->start(Ads124S0x::Channel::CH2);

            result[2] = read_unsigend(m_ads124s06[0]);
            result[5] = read_unsigend(m_ads124s06[1]);
        } else if constexpr (TAdcCount == 3) {
            m_ads124s06[0]->start(Ads124S0x::Channel::CH0);
            m_ads124s06[1]->start(Ads124S0x::Channel::CH0);
            m_ads124s06[2]->start(Ads124S0x::Channel::CH0);

            result[0] = read_unsigend(m_ads124s06[0]);
            m_ads124s06[0]->start(Ads124S0x::Channel::CH1);
            result[2] = read_unsigend(m_ads124s06[1]);
            m_ads124s06[1]->start(Ads124S0x::Channel::CH1);
            result[4] = read_unsigend(m_ads124s06[2]);
            m_ads124s06[2]->start(Ads124S0x::Channel::CH1);

            result[1] = read_unsigend(m_ads124s06[0]);
            result[3] = read_unsigend(m_ads124s06[1]);
            result[5] = read_unsigend(m_ads124s06[2]);
        }
    } else if constexpr (TPanelCount == 8) {
        m_ads124s06[0]->start(Ads124S0x::Channel::CH0);
        m_ads124s06[1]->start(Ads124S0x::Channel::CH0);
        m_ads124s06[2]->start(Ads124S0x::Channel::CH0);

        result[0] = read_unsigend(m_ads124s06[0]);
        m_ads124s06[0]->start(Ads124S0x::Channel::CH1);
        result[3] = read_unsigend(m_ads124s06[1]);
        m_ads124s06[1]->start(Ads124S0x::Channel::CH1);
        result[5] = read_unsigend(m_ads124s06[2]);
        m_ads124s06[2]->start(Ads124S0x::Channel::CH1);

        result[1] = read_unsigend(m_ads124s06[0]);
        m_ads124s06[0]->start(Ads124S0x::Channel::CH2);
        result[4] = read_unsigend(m_ads124s06[1]);
        result[6] = read_unsigend(m_ads124s06[2]);
        m_ads124s06[2]->start(Ads124S0x::Channel::CH2);

        result[2] = read_unsigend(m_ads124s06[0]);
        result[7] = read_unsigend(m_ads124s06[2]);
    } else if constexpr (TPanelCount == 9) {
        m_ads124s06[0]->start(Ads124S0x::Channel::CH0);
        m_ads124s06[1]->start(Ads124S0x::Channel::CH0);
        m_ads124s06[2]->start(Ads124S0x::Channel::CH0);

        result[0] = read_unsigend(m_ads124s06[0]);
        m_ads124s06[0]->start(Ads124S0x::Channel::CH1);
        result[3] = read_unsigend(m_ads124s06[1]);
        m_ads124s06[1]->start(Ads124S0x::Channel::CH1);
        result[6] = read_unsigend(m_ads124s06[2]);
        m_ads124s06[2]->start(Ads124S0x::Channel::CH1);

        result[1] = read_unsigend(m_ads124s06[0]);
        m_ads124s06[0]->start(Ads124S0x::Channel::CH2);
        result[4] = read_unsigend(m_ads124s06[1]);
        m_ads124s06[1]->start(Ads124S0x::Channel::CH2);
        result[7] = read_unsigend(m_ads124s06[2]);
        m_ads124s06[2]->start(Ads124S0x::Channel::CH2);

        result[2] = read_unsigend(m_ads124s06[0]);
        result[5] = read_unsigend(m_ads124s06[1]);
        result[8] = read_unsigend(m_ads124s06[2]);
    }

    return result;
}

template <size_t TPanelCount> void Pad<TPanelCount>::Panel::setState(const bool state, const uint16_t debounce_delay) {
    if (m_active == state) {
        return;
    }

    // Immediately change the input state, but only allow a change every debounce_delay milliseconds.
    const uint32_t now = to_ms_since_boot(get_absolute_time());
    if (m_last_change + debounce_delay <= now) {
        m_active = state;
        m_last_change = now;
    }
}

template <size_t TPanelCount>
Pad<TPanelCount>::Pad(const Config &config) : m_config(config), m_panels({}), m_panel_offsets({}) {
    std::visit(
        [&](auto &&adc_config) {
            using T = std::decay_t<decltype(adc_config)>;

            if constexpr (std::is_same_v<T, typename Config::GpioAdc>) {
                m_adc = std::make_unique<GpioAdc>(adc_config);
            } else if constexpr (std::is_same_v<T, typename Config::InternalAdc>) {
                m_adc = std::make_unique<InternalAdc>(adc_config);
            } else if constexpr (std::is_same_v<T, typename Config::template ExternalAdc<adc_config.ADC_COUNT>>) {
                m_adc = std::make_unique<ExternalAdc<adc_config.ADC_COUNT>>(adc_config);
            } else {
                static_assert(false, "Unknown ADC type!");
            }
        },
        m_config.adc_config);
}

template <size_t TPanelCount> void Pad<TPanelCount>::calibrate() {
    static const size_t sample_count = 50;

    std::array<uint32_t, TPanelCount> samples = {};

    for (size_t i = 0; i < sample_count; ++i) {
        const auto adc_values = m_adc->read();

        std::transform(adc_values.cbegin(), adc_values.cend(), samples.cbegin(), samples.begin(),
                       [](const auto &sample, const auto &sum) { return sum + sample; });
    }

    std::transform(samples.cbegin(), samples.cend(), m_panel_offsets.begin(),
                   [](const auto &sample) { return sample / sample_count; });
}

template <size_t TPanelCount> void Pad<TPanelCount>::updateInputState(Utils::InputState &input_state) {
    const auto adc_values = m_adc->read();

    auto update_input = [&](auto &input, const auto &channel, const auto &threshold) {
        const auto value =
            adc_values[channel] > m_panel_offsets[channel] ? adc_values[channel] - m_panel_offsets[channel] : 0;
        input.raw = value;

        if (m_panels[channel].getState()) {
            const auto lower_threshold = m_config.hysteresis > threshold ? 0 : threshold - m_config.hysteresis;
            m_panels[channel].setState(value > lower_threshold, m_config.debounce_delay_ms);
        } else {
            const auto upper_threshold =
                m_config.hysteresis > (UINT16_MAX - threshold) ? UINT16_MAX : threshold + m_config.hysteresis;
            m_panels[channel].setState(value > upper_threshold, m_config.debounce_delay_ms);
        }

        input.triggered = m_panels[channel].getState();
    };

    auto &pad = input_state.getPad();
    if constexpr (TPanelCount == 4 || TPanelCount == 6 || TPanelCount == 8 || TPanelCount == 9) {
        update_input(pad.up, m_config.adc_channels.up, m_config.thresholds.up);
        update_input(pad.left, m_config.adc_channels.left, m_config.thresholds.left);
        update_input(pad.right, m_config.adc_channels.right, m_config.thresholds.right);
        update_input(pad.down, m_config.adc_channels.down, m_config.thresholds.down);
    }
    if constexpr (TPanelCount == 5 || TPanelCount == 6 || TPanelCount == 8 || TPanelCount == 9) {
        update_input(pad.up_left, m_config.adc_channels.up_left, m_config.thresholds.up_left);
        update_input(pad.up_right, m_config.adc_channels.up_right, m_config.thresholds.up_right);
    }
    if constexpr (TPanelCount == 8 || TPanelCount == 9) {
        update_input(pad.down_left, m_config.adc_channels.down_left, m_config.thresholds.down_left);
        update_input(pad.down_right, m_config.adc_channels.down_right, m_config.thresholds.down_right);
    }
    if constexpr (TPanelCount == 9) {
        update_input(pad.center, m_config.adc_channels.center, m_config.thresholds.center);
    }
}

template <size_t TPanelCount> void Pad<TPanelCount>::setDebounceDelay(const uint16_t delay) {
    m_config.debounce_delay_ms = delay;
}

template <size_t TPanelCount> void Pad<TPanelCount>::setThresholds(const Config::Thresholds &thresholds) {
    m_config.thresholds = thresholds;
}

template <size_t TPanelCount> void Pad<TPanelCount>::setHysteresis(const uint16_t hysteresis) {
    m_config.hysteresis = hysteresis;
}

} // namespace Dancecon::Peripherals