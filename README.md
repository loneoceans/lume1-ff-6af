# lume1-ff-6af

## Overview

The 2022 Lume1-FF-6AF is an advanced, electronic-switch, single cell, 6A fully regulated dual-phase synchronous buck LED flashlight driver with turbo FET capabilities and Ultra Dynamic Range (UDR). It also includes an intelligent 2A USB-C battery charger. 

The driver is designed for 1-series n-parallel LED configurations, and operates at a high efficiency of over 90% for most of the range (>95% at some levels) to maximise flashlight runtime. Output regulation is constant-current and eliminates PWM flicker. UDR capability allows probably the lowest moonlight modes of any flashlight on this planet. Lume1 adopts the open-sourced Anduril 2 flashlight firmware by ToyKeeper, allowing for a variety of effects and user customization. 

This driver is an update to the previous driver from early 2020 used in some Fireflies/FireflyLite flashlights. The new 2022 driver shows ‘Rev C’ printed on the battery-contact side. While the general design and feature-set remains similar, the updated driver brings improvements such as UDR capability and enhanced stability & performance. Some component optimisations have also been made due to the global supply chain issues from the Covid pandemic. The constant-current buck converter remains the same with similar performance. The Attiny1634 has been replaced with the Attiny1616 MCU, which now provides a 3-pin UPDI programming interface for user firmware customisation.  Note that this firmware is not directly compatible with the previous driver due to these changes. 

## Firmware

This repository provides the Anduril 2 firmware source for the Lume1-FF-6AF driver (2022). Compiled .hex binaries are available in the ‘Debug’ subfolders for flashing and updates. I am aware of the current awkward repository management - there is ongoing work to merge this firmware branch with the main flashlight repository on Launchpad. Thank you for your patience.  

- Driver name: Lume1-FF-6AF
- HWID number: 0481
- HW board revision: Rev C

### FW Change-List 

**Date: 27 Dec 2022 (R653)**

- Updated to the recent stable Anduril 2 codebase. 
- Enhanced ramp smoothness across the entire ramp range via dynamic Vref.
- Extended regulated UDR range from ~3 million : 1, to ~13 million : 1. Level 1 is almost imperceptible to the eye and requires a completely dark room and night-adjusted vision to observe (estimated ~0.0002 lumens depending on emitters).
- Added subfreezing temperature readout - negative sign is represented with a long blink (previously Anduril2 would blink out an overflowed number).
- Fixed glitch with sunset-timer on ramp mode which caused the flashlight to increase, not decrease, in brightness at some levels. Some particularities of the Anduril ramp implementation means that sunset-stepdowns are in a stepped fashion for now.
- Fixed missing blink for first menu configuration in some menus.
- Improved strobe modes such as bicycle-flasher. 
- Improved settings for blinks and blips to be more obvious.

**Date: 25 Oct 2022 (R624)**
- Released with the initial batch of 2022 flashlights. 


## Driver Information

### Thermal Behaviour

The Lume1-FF-6AF uses the factory-calibrated internal temperature sensor of the Attiny1616 for thermal regulation. Because the internal sensor is calibrated, there is no need for further thermal calibration. The Lume1-FF-6AF adds additional capability for sub-freezing temperature readouts in temp-check mode, which could be useful for outdoor excursions. For example, 1C reads out as a single blink. 0C reads out as a single short blink. -3C reads out as one long blink (representing the negative sign) followed by 3 blinks.  

By default, the Lume1 has a thermal threshold of 55C (configurable via Anduril), additionally boosted by +7C on Turbo FET mode. The driver steps down to fully regulated 6A output when the Turbo limit is hit.  Similar to the previous Lume1 driver, it is possible to reconfigure the ramp values such that the driver PWMs the FET to allow a smoother transition between Turbo FET and 6A regulated. Note however that this is not recommended due to lower efficiency and inherent lack of current regulation. The FET and the buck should not be on simultaneously.  

Some flashlights with this driver may use a single small emitter, which cannot take the high currents from Turbo FET mode. These flashlights should have the FET mode disabled via FW or HW (hex binaries are provided for both configurations). Please understand that the usage of FET mode is inherently unregulated and while I do not advocate for the use of this mode of operation, it is included due to community demand. Please be careful. 

### UDR and Ramping

Initially developed for my Lume X1 prototype drivers in 2019, UDR is a design topology allowing for ultra-low moonlight / firefly modes with a dynamic range on the order of ~10 million : 1. Practically speaking, the lowest mode is almost imperceptible to the eye (unless viewed in a completely dark room with dark-adapted eyes). Thus the firmware defaults to 1039x and 177x the lowest brightness for Simple and Advanced UI respectively, corresponding to level 13 and level 8 out of a 150-level ramp table, or ~0.2 and ~0.03 lumens depending on emitters used. 

The floor and ceiling levels can be configured via 7H from ON. Please be careful when looking directly at the emitters since it is easy to accidentally turn on the light at full brightness.

The Lume1-FF-6AF uses a 4th Polynomial ramp curve with 150 levels. This was found to be a good balance between accessibility to useful brightness levels, and the ~logarithmic human perception of luminous flux. The curious user is encouraged to play around with different ramp tables and curves to find the modes and levels that suit them best.

### Batteries and Charging

The Lume1-FF-6AF driver is designed to be used in single-cell flashlights, primarily with 21700 lithium-ion cells. Due to its high power capability, high-drain cells such as the Samsung 40T or Molicel P42A / P45B are recommended for best performance. The driver includes a low-resistance PFET for reverse polarity protection. No voltage correction factor (in Anduril) is required. 

The Lume1-FF-6AF includes an intelligent, high-efficiency, 2A constant-current synchronous buck converter lithium-ion battery charger with USB-C input. For maximum charging rate, the USB port used should be able to source at least 5V 2A. The driver supports charge indication LEDs, though the implementation may differ depending on how the flashlight is configured. Typically, charge indication LEDs will be under the side-switch, and indicate power-source-ok (e.g. blue LED on), charging-in-progress (e.g. red LED on), fault (e.g. flashing red), and charge complete (e.g. red LED off).

For battery safety and health, cells < 3V pre-charge at 100mA, before full 2A charging begins, transitioning to constant voltage, and ending with trickle-charging to top-off. The charger has a 10 hr safety timer to prevent extended charging cycles from abnormal battery conditions. If the timer expires, a fault is triggered. The charging process can be manually restarted, which resets the safety timer and/or clears the fault. While not recommended, the flashlight may be operated while charging - keep in mind that it is possible to operate the flashlight at a higher power level than the charging rate, which will prevent the battery from being charged. 

### Beacontower

Beacontower is a new blinky mode previously introduced in the original Lume1 driver, which mimics the gradual turn-on and fade of large red incandescent beacon bulbs found on obstruction towers. Access this mode from the blinky group in this order: Batt Check, Temp Check, Beacon, SOS, Beacontower. The brightness will be the last memorized level, and defaults at 30 flashes-per-minute. Holding down the button configures the period in seconds (the light will blink once per second to aid in counting). 

### Programming

This driver offers a standard 3-pin UPDI pogo-pin interface (with 50-mil spacing) for reflashing the Attiny1616 with a custom firmware. Fuses can be left as factory-default. The fully-featured ATMEL ICE programmer can be used for memories-flashing and debug, with a 3-pin pogo-pin to interface with the pads. A variety of 3rd-party USB UPDI/Serial programmers are also becoming available. Note that VCC power must be supplied externally if using the ATMEL ICE. The firmware provided here is organized for Microchip Studio, for development on Windows. Microchip Studio is available here: https://www.microchip.com/en-us/tools-resources/develop/microchip-studio

## Notes

As far as I am aware, this driver is planned to be used in the 2022 versions of the FireflyLite E12R, NOV-Mu, E07X Pro, and T1/9R flashlights, and possibly others which have not been disclosed to me. The default behaviour and configurations have been customized to the requests of FireflyLite. For example, the driver defaults in stepped-ramping mode, which can be changed to smooth-ramping via Advanced UI. Unfortunately, I did not receive any of these flashlights from the manufacturer for driver-performance validation in form-factor, so feedback from the community is appreciated until I am able to purchase one for myself. 

Thanks goes out to ToyKeeper, gHart, and the flashlight community for their work on Anduril 2, and AVR 1-series support. I appreciate all the feedback, comments, and support from the BLF flashlight community and other forums. Finally, a shout-out to FireflyLite for picking up the Lume1 driver for some of their flashlights and making it available to the public to experience. I hope you will be able to enjoy using it as much as I did creating it. 

For more information: www.loneceans.com/labs/ 

Anduril2 Manual by TK: http://toykeeper.net/torches/fsm/anduril2/anduril-manual.txt
