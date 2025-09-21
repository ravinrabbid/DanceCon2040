#ifndef _PERIPHERALS_PAD_H_
#define _PERIPHERALS_PAD_H_

#include "utils/InputState.h"

#include "hardware/spi.h"

#include <ads124s0x/Ads124S0x.h>

#include <array>
#include <map>
#include <memory>
#include <stdint.h>
#include <variant>

namespace Dancecon::Peripherals {

template <size_t TPanelCount> class Pad {
  private:
    template <size_t TConfigPanelCount>
        requires(TConfigPanelCount == 4 || TConfigPanelCount == 5 || TConfigPanelCount == 6 || TConfigPanelCount == 8 ||
                 TConfigPanelCount == 9)
    struct TConfig {
        static constexpr auto PANEL_COUNT = TPanelCount;

        struct ThresholdsFourPanel {
            uint16_t up;
            uint16_t left;
            uint16_t right;
            uint16_t down;

            bool operator==(const ThresholdsFourPanel &) const = default;
        };
        struct ThresholdsFivePanel {
            uint16_t up_left;
            uint16_t up_right;
            uint16_t center;
            uint16_t down_left;
            uint16_t down_right;

            bool operator==(const ThresholdsFivePanel &) const = default;
        };
        struct ThresholdsSixPanel {
            uint16_t up_left;
            uint16_t up;
            uint16_t up_right;
            uint16_t left;
            uint16_t right;
            uint16_t down;

            bool operator==(const ThresholdsSixPanel &) const = default;
        };
        struct ThresholdsEightPanel {
            uint16_t up_left;
            uint16_t up;
            uint16_t up_right;
            uint16_t left;
            uint16_t right;
            uint16_t down_left;
            uint16_t down;
            uint16_t down_right;

            bool operator==(const ThresholdsEightPanel &) const = default;
        };
        struct ThresholdsNinePanel {
            uint16_t up_left;
            uint16_t up;
            uint16_t up_right;
            uint16_t left;
            uint16_t center;
            uint16_t right;
            uint16_t down_left;
            uint16_t down;
            uint16_t down_right;

            bool operator==(const ThresholdsNinePanel &) const = default;
        };

        using Thresholds =                              //
            std::conditional<                           //
                TConfigPanelCount == 4,                 //
                ThresholdsFourPanel,                    //
                typename std::conditional<              //
                    TConfigPanelCount == 5,             //
                    ThresholdsFivePanel,                //
                    typename std::conditional<          //
                        TConfigPanelCount == 6,         //
                        ThresholdsSixPanel,             //
                        typename std::conditional<      //
                            TConfigPanelCount == 8,     //
                            ThresholdsEightPanel,       //
                            typename std::conditional<  //
                                TConfigPanelCount == 9, //
                                ThresholdsNinePanel,    //
                                std::monostate          //
                                >::type>::type>::type>::type>::type;

        struct AdcChannelsFourPanel {
            uint8_t up;
            uint8_t left;
            uint8_t right;
            uint8_t down;
        };
        struct AdcChannelsFivePanel {
            uint8_t up_left;
            uint8_t up_right;
            uint8_t center;
            uint8_t down_left;
            uint8_t down_right;
        };
        struct AdcChannelsSixPanel {
            uint8_t up_left;
            uint8_t up;
            uint8_t up_right;
            uint8_t left;
            uint8_t right;
            uint8_t down;
        };
        struct AdcChannelsEightPanel {
            uint8_t up_left;
            uint8_t up;
            uint8_t up_right;
            uint8_t left;
            uint8_t right;
            uint8_t down_left;
            uint8_t down;
            uint8_t down_right;
        };
        struct AdcChannelsNinePanel {
            uint8_t up_left;
            uint8_t up;
            uint8_t up_right;
            uint8_t left;
            uint8_t center;
            uint8_t right;
            uint8_t down_left;
            uint8_t down;
            uint8_t down_right;
        };

        using AdcChannels =                             //
            std::conditional<                           //
                TConfigPanelCount == 4,                 //
                AdcChannelsFourPanel,                   //
                typename std::conditional<              //
                    TConfigPanelCount == 5,             //
                    AdcChannelsFivePanel,               //
                    typename std::conditional<          //
                        TConfigPanelCount == 6,         //
                        AdcChannelsSixPanel,            //
                        typename std::conditional<      //
                            TConfigPanelCount == 8,     //
                            AdcChannelsEightPanel,      //
                            typename std::conditional<  //
                                TConfigPanelCount == 9, //
                                AdcChannelsNinePanel,   //
                                std::monostate          //
                                >::type>::type>::type>::type>::type;

        struct InternalAdc {
            uint8_t sample_count;
        };

        struct GpioAdc {
            uint8_t base_pin;
        };

        template <size_t TAdcCount> struct ExternalAdc {
            static constexpr auto ADC_COUNT = TAdcCount;

            struct Spi {
                uint8_t mosi_pin;
                uint8_t miso_pin;
                uint8_t sclk_pin;
                spi_inst_t *block;
                uint speed_hz;
            };
            struct Ads124s06 {
                uint8_t scsn_pin;
                uint8_t drdy_pin;
            };

            Spi spi;

            Ads124S0x::SampleRate sample_rate;
            Ads124S0x::Gain gain;
            Ads124S0x::Filter filter;

            std::array<Ads124s06, TAdcCount> adcs;
        };

        using AdcConfig =                                                  //
            std::conditional<                                              //
                TConfigPanelCount == 4,                                    //
                std::variant<ExternalAdc<2>, InternalAdc, GpioAdc>,        //
                typename std::conditional<                                 //
                    TConfigPanelCount == 5 || TConfigPanelCount == 6,      //
                    std::variant<ExternalAdc<2>, ExternalAdc<3>, GpioAdc>, //
                    typename std::conditional<                             //
                        TConfigPanelCount == 8 || TConfigPanelCount == 9,  //
                        std::variant<ExternalAdc<3>, GpioAdc>,             //
                        std::monostate                                     //
                        >::type>::type>::type;

        Thresholds thresholds;
        uint16_t debounce_delay_ms;

        AdcChannels adc_channels;
        AdcConfig adc_config;
    };

  public:
    using Config = TConfig<TPanelCount>;

  private:
    class Panel {
      private:
        uint32_t last_change;
        bool active;

      public:
        Panel();

        bool getState() const { return active; };
        void setState(const bool state, const uint16_t debounce_delay);
    };

    class AdcInterface {
      public:
        virtual std::array<uint16_t, TPanelCount> read() = 0;
    };

    class GpioAdc : public AdcInterface {
      private:
        Config::GpioAdc m_config;

      public:
        GpioAdc(const Config::GpioAdc &config);
        virtual std::array<uint16_t, TPanelCount> read() final;
    };

    class InternalAdc : public AdcInterface {
      private:
        Config::InternalAdc m_config;

      public:
        InternalAdc(const Config::InternalAdc &config);
        virtual std::array<uint16_t, TPanelCount> read() final;
    };

    template <size_t TAdcCount> class ExternalAdc : public AdcInterface {
      private:
        std::array<std::unique_ptr<Ads124S0x>, TAdcCount> m_ads124s06;

      public:
        ExternalAdc(const typename Config::ExternalAdc<TAdcCount> &config);
        virtual std::array<uint16_t, TPanelCount> read() final;
    };

    Config m_config;

    std::unique_ptr<AdcInterface> m_adc;
    std::array<Panel, TPanelCount> m_panels;

    std::array<uint16_t, TPanelCount> m_panel_offsets;

  public:
    Pad(const Config &config);

    void calibrate();

    void updateInputState(Utils::InputState &input_state);

    void setDebounceDelay(const uint16_t delay);
    void setThresholds(const Config::Thresholds &thresholds);
};

} // namespace Dancecon::Peripherals

#endif // _PERIPHERALS_PAD_H_