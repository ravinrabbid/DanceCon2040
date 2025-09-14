#include "peripherals/Pad.h"

#include "GlobalConfiguration.h"

#include "hardware/gpio.h"

#include <algorithm>
#include <limits>

namespace Dancecon::Peripherals {

template class Pad<Config::Default::pad_config.PANEL_COUNT>;

template <size_t TPanelCount>
template <size_t TAdcCount>
Pad<TPanelCount>::ExternalAdc<TAdcCount>::ExternalAdc(const typename Config::ExternalAdc<TAdcCount> &config) {
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
        adc->setFilter(Ads124S0x::Filter::LowLatency);
        adc->setGain(Ads124S0x::Gain::G128);
        adc->setSampleRate(Ads124S0x::SampleRate::R4000);
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

template <size_t TPanelCount> Pad<TPanelCount>::Panel::Panel() : last_change(0), active(false) {}

template <size_t TPanelCount> void Pad<TPanelCount>::Panel::setState(const bool state, const uint16_t debounce_delay) {
    if (active == state) {
        return;
    }

    // Immediately change the input state, but only allow a change every debounce_delay milliseconds.
    uint32_t now = to_ms_since_boot(get_absolute_time());
    if (last_change + debounce_delay <= now) {
        active = state;
        last_change = now;
    }
}

template <size_t TPanelCount>
Pad<TPanelCount>::Pad(const Config &config) : m_config(config), m_panels({}), m_panel_offsets({}) {
    std::visit(
        [&](auto &&adc_config) {
            using T = std::decay_t<decltype(adc_config)>;

            if constexpr (std::is_same_v<T, typename Config::ExternalAdc<adc_config.ADC_COUNT>>) {
                m_adc = std::make_unique<ExternalAdc<adc_config.ADC_COUNT>>(adc_config);
            } else {
                static_assert(false, "Unknown ADC type!");
            }
        },
        m_config.adc_config);
}

template <size_t TPanelCount> void Pad<TPanelCount>::calibrate() {
    static const size_t sample_count = 100;

    std::array<uint32_t, TPanelCount> samples = {};

    for (size_t i = 0; i < sample_count; ++i) {
        const auto adc_values = m_adc->read();

        std::transform(adc_values.cbegin(), adc_values.cend(), samples.cbegin(), samples.begin(),
                       [](const auto &sample, const auto &sum) { return sum + sample; });
    }

    std::transform(samples.cbegin(), samples.cend(), m_panel_offsets.begin(),
                   [](const auto &sample) { return sample / 100; });
}

template <size_t TPanelCount> void Pad<TPanelCount>::updateInputState(Utils::InputState &input_state) {
    const auto adc_values = m_adc->read();

    auto update_input = [&](auto &input, const auto &channel, const auto &threshold) {
        const auto value =
            adc_values[channel] > m_panel_offsets[channel] ? adc_values[channel] - m_panel_offsets[channel] : 0;
        input.raw = value;
        m_panels[channel].setState(value > threshold, m_config.debounce_delay_ms);
        input.triggered = m_panels[channel].getState();
    };

    if constexpr (TPanelCount == 4 || TPanelCount == 6 || TPanelCount == 8 || TPanelCount == 9) {
        update_input(input_state.pad.up, m_config.adc_channels.up, m_config.thresholds.up);
        update_input(input_state.pad.left, m_config.adc_channels.left, m_config.thresholds.left);
        update_input(input_state.pad.right, m_config.adc_channels.right, m_config.thresholds.right);
        update_input(input_state.pad.down, m_config.adc_channels.down, m_config.thresholds.down);
    }
    if constexpr (TPanelCount == 5 || TPanelCount == 6 || TPanelCount == 8 || TPanelCount == 9) {
        update_input(input_state.pad.up_left, m_config.adc_channels.up_left, m_config.thresholds.up_left);
        update_input(input_state.pad.up_right, m_config.adc_channels.up_right, m_config.thresholds.up_right);
    }
    if constexpr (TPanelCount == 8 || TPanelCount == 9) {
        update_input(input_state.pad.down_left, m_config.adc_channels.down_left, m_config.thresholds.down_left);
        update_input(input_state.pad.down_right, m_config.adc_channels.down_right, m_config.thresholds.down_right);
    }
    if constexpr (TPanelCount == 9) {
        update_input(input_state.pad.center, m_config.adc_channels.center, m_config.thresholds.center);
    }
}

template <size_t TPanelCount> void Pad<TPanelCount>::setDebounceDelay(const uint16_t delay) {
    m_config.debounce_delay_ms = delay;
}

template <size_t TPanelCount> void Pad<TPanelCount>::setThresholds(const Config::Thresholds &thresholds) {
    m_config.thresholds = thresholds;
}

} // namespace Dancecon::Peripherals