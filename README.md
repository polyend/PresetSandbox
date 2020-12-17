# Polyend Preset Sandbox

## Introduction

Welcome to the alternative open source firmware for the [Polyend Preset](https://polyend.com/preset/) module for Eurorack system.

This project is a sandbox for developers willing to build their own functionality for the Preset hardware, be it sequencing, preset management or whatever else imagination brings.

It contains:

* Runtime framework.
* Drivers for all peripherals.
* [Easy to use C++ API](app/presetApp.h) for all user interface and I/O on the device (input, outputs, knobs, pads, buttons).
* Example applications using the API, including a [generative sequencing machine](app/euclApp.h).

Below you will find instructions and how to compile the examples and build your own functionality.
If you prefer to jumpstart to playing with one of the examples first, you can download a prebuilt binary of the Euclidean generator in the [Releases](https://github.com/polyend/PresetSandbox/releases/) section and then read [Flash the device](#flash-the-device) below.

## Setup

* Download/clone the project from github
* Install tools to build the firmware, two options are available:
    * [GNU Arm toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads) for command-line use.
        * Specifics of the installation depend on your operating system.
    * [MCUXpresso IDE](https://mcuxpresso.nxp.com/en/welcome) if you prefer GUI.
        * You need both the IDE and the SDK for MK20DX256 board. You can install the latter manually or from within the IDE.
* You will also need the [Polyend Tool](https://polyend.com/downloads/) to flash new firmware to your Preset device.
* (Optional) For easier flashing and to be able to debug on target get a debug probe, for instance the [LPC-Link2](https://www.nxp.com/design/microcontrollers-developer-resources/lpc-microcontroller-utilities/lpc-link2:OM13054)

## Build firmware using command line

In your shell just run:

```bash
make
```

the build should finish shortly and end with (section sizes may differ):

```bash
copy from `build/preset.axf' [elf32-littlearm] to `build/preset.hex' [ihex]
   text	   data	    bss	    dec	    hex	filename
  36568	      0	   7428	  43996	   abdc	build/preset.axf
copy from `build/preset.axf' [elf32-littlearm] to `build/preset.bin' [binary]
```

As you can see, the final `hex` file is now available at `build/preset.hex`.

## Build firmware in MCUXpresso IDE

* Create a new workspace and add this project to it - `.cproject` with all settings is included in the repository.
* Right-click on the project root node and choose Build from the context menu.
* Console window should open and after a few seconds end with (section sizes may differ):

```bash
copy from `Preset.axf' [elf32-littlearm] to `Preset_1.1.16_20201215_1645.hex' [ihex]
   text	   data	    bss	    dec	    hex	filename
  54412	      0	   7424	  61836	   f18c	Preset.axf
 

15:27:36 Build Finished. 0 errors, 0 warnings. (took 1s.570ms)
```

* Final `hex` file is now available in either the `Release` or `Debug` folder (depending on active configuration)

(Refer to MCUXpresso manual if you are new to the IDE)

## Flash the device

 Use the `hex` file to flash the firmware to your device using the [Polyend Tool](https://polyend.com/downloads/) - check included `!instructions.txt`.
 
 You can always revert back to the original Preset firmware - just [download](https://polyend.com/downloads/) it and flash it the same way.

## Modify

There are two examples of what can be done with this sandbox, see `helloApp` and `euclApp` in the [app](app) folder.

By default `euclApp` is enabled.

To enable another application just edit `APP_CLASS` in [main.cpp](app/main.cpp).

You can create your own class inheriting from [presetApp](app/presetApp.h) or just modify one of the examples.

All the API you need is available in [presetApp](app/presetApp.h) - study the comments there as well as the [examples](app). If anything is not clear you can ask in the Issues section of the project.

## Have fun!