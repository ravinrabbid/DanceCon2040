# DanceCon2040 - A RP2040 based Dance Pad

DanceCon2040 is a firmware (and by extension a PCB) for arcade style Dance Pad controllers with up to nine panels.

It is pretty much tailored to this specific use case, if you are looking for something universal, ready-to-flash and on-the-fly configurable I'd recommend to have a look at more generic approaches like [GP2040-CE](https://github.com/OpenStickCommunity/GP2040-CE). If you however want build something more specialized or custom, feel free to use this project as a base, it is designed to be somewhat modular and should be easy remodel. See [DivaCon2040](https://github.com/ravinrabbid/DivaCon2040) or [DonCon2040](https://github.com/ravinrabbid/DonCon2040) for examples on how this could look like.
If you have any questions about the project in general or need hints how to build this thing, feel free to open a [discussion](https://github.com/ravinrabbid/DanceCon2040/discussions) anytime!

## Features

- Various controller emulation modes
  - Spice2x - Basic USB gamepad with support for HID lights
  - Xbox360 DDR - Emulates a Xbox360 DDR Dance Mat (untested!)
  - Xbox360
  - PS3 DDR - Emulates a Playstation 3 DDR Dance Mat
  - Dualshock 3
  - Dualshock 4 (Only for PC/Steam, does not work on an actual PS4!)
  - PS4 Compat - Works on PS4 (see [PS4 Authentication](#ps4-authentication) for details)
  - Switch Pro Controller
  - Keyboard - Two different mappings for player 1 or 2 available
  - Debug mode - Outputs current state via USB serial and allows direct flashing
- Support for either 4,5,6,8 or 9 panels for different games/game modes.
- Additional buttons via external i2c GPIO expander
- Basic configuration via on-screen menu on attached OLED screen
- WS2812 LED for panel illumination
- Steps-per-minute counter counter on display

## Building

<!-- I highly recommend to build the firmware yourself so you can make adjustments in `include/GlobalConfiguration.h` to match your specific controller build.
You can still use the [binary release](https://github.com/ravinrabbid/DanceCon2040/releases) which is pre-configured for the [DanceConIO](/pcb/DanceConIO). -->

### VSCode (Windows, Linux, MacOS)

Install [VSCode](https://code.visualstudio.com/) and get the [Raspberry Pi Pico](https://marketplace.visualstudio.com/items?itemName=raspberry-pi.raspberry-pi-pico) extension. From the extension choose 'Import Project' and select the folder where you've checked out this repository, then use 'Compile Project'.

### CLI

See [pico-sdk readme](https://github.com/raspberrypi/pico-sdk/blob/master/README.md#quick-start-your-own-project) for a list of pre-requisites.

Use the environment variables `PICO_SDK_PATH` to use a local pico-sdk, and `PICO_BOARD` to select another target board.
By default the pico-sdk will be fetched from Github and the target board will be "pico".

```sh
mkdir build && cd build
cmake ..
make
```

## Configuration

Few things which you probably want to change more regularly can be changed using an on-screen menu on the attached OLED display, hold both Start and Select for 2 seconds to enter the menu:

- Controller emulation mode
- Trigger thresholds
- Hold Time
- Hysteresis
- Calibration
- LED brightness, animation style and colors
- Reset settings to defaults
- Enter BOOTSEL mode for firmware flashing

Those settings are persisted to flash memory if you choose 'Save' when exiting the Menu and will survive power cycles.

Defaults and everything else is compiled statically into the firmware. You can find everything in `include/GlobalConfiguration.h`. This covers default controller emulation mode, i2c pins, ADC configuration, default trigger thresholds, hysteresis, debounce delay, panel mapping, LED colors and brightness.

### Debounce Delay / Hold Time / Hysteresis

The debounce delay also implicitly serves as the hold time of the input after a panel if triggered. On some platforms inputs won't be registered properly if this time is too short.
If you notice dropped inputs even if the controller signals an input on the LED/Display, try to increase this value.

Since the values read from load cells are not very stable, rapid triggering can happen right at the threshold value. If you encounter this, try to increase the hysteresis offset.
A panel will trigger if its value exceeds `threshold + hysteresis` and will un-trigger if its value drops below `thresholds - hysteresis` again.

### PS4 Authentication

The PS4 needs a controller to sign a cryptographic challenge every few seconds, otherwise it will stop working after around 8 minutes after plugging in. For rhythm games this is somewhat bearable, since you can re-plug the controller before starting each song to avoid running into the timeout during gameplay. Still, this is annoying.

DanceCon2040 can sign those challenges, but you will need to obtain some data from an original DS4 and compile it into the firmware. You will need a serial, signature and private key file. I can't and won't help you how to obtain those, you'll need to figure this out yourself.

To build the firmware run `scripts/generateAuthConfig.py` in the folder where you placed the required files. Copy the resulting `PS4AuthConfiguration.h` to the `include` directory, replacing the existing header. Then build the firmware as described in [Building](#building).

Signing the challenge will block the second core of the rp2040 for 2-3 seconds, so the display, external controller and LED will appear stuck from time to time. Input handling of the drum is unaffected.

## Hardware

### IO Board

The [DanceConIO](/pcb/DanceConIO) board in the pcb subfolder is built to be used with Wheatstone Bridge type panel sensors, e.g. load cells. It hosts a Waveshare RP2040-Zero and two or three ADS124S06 ADCs for up to nine differential inputs. See its [README](/pcb/DanceConIO/README.md) for details.

If you don't want to use this board, the firmware should be usable on most RP2040 boards with appropriate configuration. You may also use a more simple panel trigger solution, the firmware can also be configured to use the RP2040's internal ADC or simple GPIO style inputs.

### Controller Buttons and Display

Additional controller buttons and the display are attached to the same i2c bus. For the display, use a standard SSD1306 OLED display with 128x64 resolution. The buttons need to be attached to a MCP23017 IO expander.

See [DonConPad](/pcb/DonConPad/) for a exemplary gamepad pcb.

## Acknowledgements

- [daschr](https://github.com/daschr) for the [SSD1306 OLED driver](https://github.com/daschr/pico-ssd1306)
- [FeralAI](https://github.com/FeralAI) for the inspiration and Dualshock3/XInput driver from the [GP2040 Project](https://github.com/FeralAI/GP2040)
- The linux kernel contributors for documenting the game controllers in their drivers
