#include "Ads124S0x.h"

#include "hardware/gpio.h"

#include <span>

#include "pico/time.h"

namespace {
int32_t readingToInt32(std::span<const uint8_t, 3> data) {
    return ((static_cast<int32_t>(data[0]) << 24) | (static_cast<int32_t>(data[1]) << 16) |
            (static_cast<int32_t>(data[2]) << 8)) >>
           8;
}
} // namespace

Ads124S0x::Ads124S0x(spi_inst *spi, uint8_t cs_pin, uint8_t drdy_pin)
    : m_spi(spi), m_cs_pin(cs_pin), m_drdy_pin(drdy_pin) {}

void Ads124S0x::selectChannel(Ads124S0x::Channel channel) {
    auto getMuxConfig = [](auto channel) {
        switch (channel) {
        case Channel::CH0:
            return 0x01;
        case Channel::CH1:
            return 0x23;
        case Channel::CH2:
            return 0x45;
        case Channel::CH3:
            return 0x67;
        case Channel::CH4:
            return 0x89;
        case Channel::CH5:
            return 0xAB;
        }

        return 0x00; // unreachable
    };

    writeRegister(Register::INPMUX, getMuxConfig(channel));
}

void Ads124S0x::reset() { sendCommand(Command::RESET); }

void Ads124S0x::start(Channel channel) {
    selectChannel(channel);
    sendCommand(Command::START);
}

void Ads124S0x::stop() { sendCommand(Command::STOP); }

int32_t Ads124S0x::readBlocking() {
    const std::array<uint8_t, 3> data_out = {0x00, 0x00, 0x00};

    while (gpio_get(m_drdy_pin)) {
    }

    return readingToInt32(spiReadWrite(data_out));
}

int32_t Ads124S0x::read() {
    const std::array<uint8_t, 4> data_out = {static_cast<uint8_t>(Command::RDATA), 0x00, 0x00, 0x00};

    const auto data_in = spiReadWrite(data_out);

    return readingToInt32(std::span(data_in).last<3>());
}

void Ads124S0x::setConversionMode(Ads124S0x::ConversionMode mode) {
    switch (mode) {
    case ConversionMode::Continuous:
        setBit(Register::DATARATE, 5, false);
        break;
    case ConversionMode::Single:
        setBit(Register::DATARATE, 5, true);
        break;
    }
}

void Ads124S0x::setFilter(Ads124S0x::Filter filter) {
    switch (filter) {
    case Filter::Sinc3:
        setBit(Register::DATARATE, 4, false);
        break;
    case Filter::LowLatency:
        setBit(Register::DATARATE, 4, true);
        break;
    }
}

void Ads124S0x::setGain(Gain gain) {
    const auto current = readRegister(Register::PGA);

    if (gain == Gain::Off) {
        // Disable PGA
        writeRegister(Register::PGA, current & 0xE0);
    } else {
        writeRegister(Register::PGA, (current & 0xE0) | (1 << 3) | static_cast<uint8_t>(gain));
    }
}

void Ads124S0x::setSampleRate(SampleRate rate) {
    const auto current = readRegister(Register::DATARATE);

    writeRegister(Register::DATARATE, (current & 0xF0) | static_cast<uint8_t>(rate));
}

void Ads124S0x::setConversionDelay(ConversionDelay delay) {
    const auto current = readRegister(Register::PGA);

    writeRegister(Register::PGA, (current & 0x1F) | (static_cast<uint8_t>(delay) << 5));
}

void Ads124S0x::setReferenceInput(ReferenceInput input) {
    auto ref = readRegister(Register::REF);

    ref = (ref & 0xF3) | (static_cast<uint8_t>(input) << 2);

    switch (input) {
    case ReferenceInput::Internal:
        // Disable buffers
        ref |= 0x30;
        // Enable internal source
        ref = (ref & 0xFC) | 0x01;
        break;
    case ReferenceInput::External0:
    case ReferenceInput::External1:
        // Disable internal source
        ref &= 0xFC;
        break;
    }

    writeRegister(Register::REF, ref);
}

void Ads124S0x::setPositiveReferenceBuffer(bool enable) { setBit(Register::REF, 5, !enable); }

void Ads124S0x::setNegativeReferenceBuffer(bool enable) { setBit(Register::REF, 4, !enable); }

void Ads124S0x::sendCommand(Ads124S0x::Command cmd) {
    const std::array<uint8_t, 1> data_out = {static_cast<uint8_t>(cmd)};

    spiWrite(data_out);
}

uint8_t Ads124S0x::readRegister(Ads124S0x::Register reg) {
    const uint8_t cmd = static_cast<uint8_t>(Command::RREG) | static_cast<uint8_t>(reg);
    const std::array<uint8_t, 3> data_out = {cmd, 0x00, 0x00};

    return spiReadWrite(data_out)[2];
}

void Ads124S0x::writeRegister(Ads124S0x::Register reg, uint8_t value) {
    const uint8_t cmd = static_cast<uint8_t>(Command::WREG) | static_cast<uint8_t>(reg);
    const std::array<uint8_t, 3> data_out = {cmd, 0x00, value};

    spiWrite(data_out);
}

void Ads124S0x::setBit(Ads124S0x::Register reg, uint8_t position, bool value) {
    const auto current = readRegister(reg);

    if (value) {
        writeRegister(reg, current | (1 << position));
    } else {
        writeRegister(reg, current & ~(1 << position));
    }
}

template <size_t S> std::array<uint8_t, S> Ads124S0x::spiReadWrite(const std::array<uint8_t, S> &data_out) {
    std::array<uint8_t, S> data_in = {};

    gpio_put(m_cs_pin, false);
    spi_write_read_blocking(m_spi, data_out.data(), data_in.data(), S);
    gpio_put(m_cs_pin, true);

    return data_in;
}

template <size_t S> void Ads124S0x::spiWrite(const std::array<uint8_t, S> &data_out) {
    gpio_put(m_cs_pin, false);
    spi_write_blocking(m_spi, data_out.data(), S);
    gpio_put(m_cs_pin, true);
}