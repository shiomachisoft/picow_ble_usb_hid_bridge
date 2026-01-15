# Pico W BLE to USB HID Bridge

## Overview

This software is firmware for the Raspberry Pi Pico W. It allows you to use your favorite BLE HID devices such as keyboards or mice as wired USB devices, even on PCs that do not have Bluetooth functionality.
It operates as a BLE Central (Host), forwarding input information from connected BLE devices to a PC (USB Host) via USB.
Since it is recognized as a standard USB HID device by the PC, it can also be used in UEFI environments.

## Usage

1.  **Connect Pico W**
    *   Connect the Pico W to the PC's USB port.
    *   In the standby state where BLE connection is not complete, the LED on the Pico W will **blink**.

2.  **Pairing**
    *   Put the BLE HID device (keyboards, mice, and other peripherals) you want to connect into pairing mode.
    *   *Refer to the manual of each BLE device for how to enter pairing mode.*

3.  **Connection Complete**
    *   When the Pico W detects the device and the connection is complete, the LED changes to **steady on**.
    *   It will be recognized as a USB input device on the PC side, and operation becomes possible.

### Notes
*   **Reconnection**: Once paired (bonded), you do not need to put the BLE device into pairing mode next time. It will automatically reconnect just by turning on the power.
*   **Keyboard Behavior**: Some BLE keyboards (with power-saving features, etc.) may not enter reconnection mode unless a key is pressed to wake them from sleep. If it does not connect, try pressing a random key a few times.

## Verified Devices

Operation has been verified with the following devices:

*   **Mouse**: Sanwa Supply MA-SBB314
*   **Keyboard**: ELECOM TK-FBM119

## Features

### Low Latency Efforts
To achieve a comfortable operational feel as an input device, the following optimizations are performed:

*   **Multi-core Distributed Processing**:
    *   **Core 0**: Handles communication processing as a USB device.
    *   **Core 1**: Handles communication processing as a BLE Host (Central).
    *   By operating these in parallel, processing delay from BLE reception to USB transmission is minimized.
*   **High-Speed Polling**:
    *   The USB endpoint polling interval (`bInterval`) is set to `1` (1ms), configured to transfer reports to the PC at the fastest speed.

### Report Pass-through
*   **HID Report Descriptor**:
    *   Upon completion of the BLE connection, a USB reconnection is triggered to pass the "HID Report Descriptor" acquired from the BLE device directly to the PC (USB host). This ensures that device-specific features, such as multimedia keys, are correctly recognized by the PC.
*   **HID Input Report**:
    *   After the BLE connection is established, the "HID Input Report" received from the BLE device is passed through to the PC (USB host) without modification.

### Connection Management
*   **Smart Scan**:
    *   Operates by automatically switching between reconnection to known devices (bonded devices) and scanning for new devices every few seconds.

## Technical Details

### Base Projects
This software is developed and integrated based on sample code from the following open source projects:

*   **TinyUSB**: `dev_hid_composite` sample
*   **BTstack**: `hog_host_demo` sample

## License

For license details of this software, please refer to LICENSE.TXT.
