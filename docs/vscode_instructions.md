# Building in Visual Studio Code

An alternative to the CMake instructions in [building.md](building.md), using
the Raspberry Pi Pico extension.

## Prerequisites

Install the "Raspberry Pi Pico" extension in Visual Studio Code on Windows.
These versions are known to work:

- VS Code 1.107.1
- Raspberry Pi Pico extension 0.19.0

## Steps

1.  Open a new VS Code window.
2.  Click "Import Project" in the Raspberry Pi Pico extension.
3.  For "Location", select the `src` folder, which holds the firmware sources.
4.  For "Select Pico SDK version", select "v2.2.0".
5.  Click "Import".
6.  Under "Switch Build Type", pick "Release" or "Debug".
7.  Click "Compile Project".

This configures CMake with the defaults, so you get a Pico 2 W build. Use the
command-line build to choose a different board or any of the other options.
