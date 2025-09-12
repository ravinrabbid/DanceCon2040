#include "peripherals/PanelLeds.h"

namespace Dancecon::Peripherals {

PanelLeds::PanelLeds(const Config &config) { (void)config; }

void PanelLeds::setBrightness(const uint8_t brightness) { (void)brightness; }
void PanelLeds::setEnablePlayerColor(const bool do_enable) { (void)do_enable; }

void PanelLeds::setInputState(const Utils::InputState &input_state) { (void)input_state; }
void PanelLeds::setPlayerColor(const Config::Color &color) { (void)color; }

void PanelLeds::update() {}

} // namespace Dancecon::Peripherals