#ifndef _ADS124S0X_ADS124S0X_H_
#define _ADS124S0X_ADS124S0X_H_

#include "hardware/spi.h"

#include <array>

class Ads124S0x {
  public:
    enum class Channel {
        CH0,
        CH1,
        CH2,
        // ADS124S08 only
        CH3,
        CH4,
        CH5,
    };

    enum class ConversionMode {
        Continuous,
        Single,
    };

    enum class Filter {
        Sinc3,
        LowLatency,
    };

    enum class Gain {
        G0,
        G1 = 0x00,
        G2 = 0x01,
        G4 = 0x02,
        G8 = 0x03,
        G16 = 0x04,
        G32 = 0x05,
        G64 = 0x06,
        G128 = 0x07,
    };

    enum class SampleRate {
        R2_5 = 0x00,
        R5 = 0x01,
        R10 = 0x02,
        R16_6 = 0x03,
        R20 = 0x04,
        R50 = 0x05,
        R60 = 0x06,
        R100 = 0x07,
        R200 = 0x08,
        R400 = 0x09,
        R800 = 0x0A,
        R1000 = 0x0B,
        R2000 = 0x0C,
        R4000 = 0x0D, // or 0x0E
    };

    enum class ConversionDelay {
        D14 = 0x00,
        D25 = 0x01,
        D64 = 0x02,
        D256 = 0x03,
        D1024 = 0x04,
        D2048 = 0x05,
        D4096 = 0x06,
        D1 = 0x07,
    };

    enum class ReferenceInput {
        External0 = 0x00,
        External1 = 0x01,
        Internal = 0x02,
    };

  private:
    enum class Command {
        // Control commands
        NOP = 0x00,
        WAKEUP = 0x02,
        POWERDOWN = 0x04,
        RESET = 0x06,
        START = 0x08,
        STOP = 0x0A,

        // Calibration commands
        SYOCAL = 0x16,
        SYGCAL = 0x17,
        SFOCAL = 0x19,

        RDATA = 0x12,
        RREG = 0x20,
        WREG = 0x40,
    };

    enum class Register {
        ID = 0x00,
        STATUS = 0x01,
        INPMUX = 0x02,
        PGA = 0x03,
        DATARATE = 0x04,
        REF = 0x05,
        IDACMAG = 0x06,
        IDACMUX = 0x07,
        VBIAS = 0x08,
        SYS = 0x09,
        OFCAL0 = 0x0A,
        OFACL1 = 0x0B,
        OFCAL2 = 0x0C,
        FSCAL0 = 0x0D,
        FSCAL1 = 0x0E,
        FSCAL2 = 0x0F,
        GPIODAT = 0x10,
        GPIOCON = 0x11,
    };

  private:
    spi_inst *m_spi;
    uint8_t m_cs_pin;
    uint8_t m_drdy_pin;

  public:
    Ads124S0x(spi_inst *spi, uint8_t cs_pin, uint8_t drdy_pin);

    void reset();
    void start(Channel channel);
    void stop();

    int32_t readBlocking();
    int32_t read();

    void setConversionMode(ConversionMode mode);
    void setFilter(Filter filter);
    void setGain(Gain gain);
    void setSampleRate(SampleRate rate);
    void setConversionDelay(ConversionDelay delay);
    void setReferenceInput(ReferenceInput input);
    void setPositiveReferenceBuffer(bool enable);
    void setNegativeReferenceBuffer(bool enable);

  private:
    void selectChannel(Channel channel);

    void sendCommand(Command cmd);

    uint8_t readRegister(Register reg);
    void writeRegister(Register reg, uint8_t value);

    void setBit(Register reg, uint8_t pos, bool value);

    template <size_t S> std::array<uint8_t, S> spiReadWrite(const std::array<uint8_t, S> &data_out);
    template <size_t S> void spiWrite(const std::array<uint8_t, S> &data_out);
};

#endif // _ADS124S0X_ADS124S0X_H_