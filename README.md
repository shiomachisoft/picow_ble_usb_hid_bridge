# Pico W / Pico 2 W BLE to USB HID Bridge

Firmware that turns a Raspberry Pi Pico W or Pico 2 W into a wired adapter for a
Bluetooth keyboard or mouse. The board connects to the BLE device as a Central
and presents it to the PC as an ordinary USB HID device, so the keyboard works
on machines without Bluetooth and in UEFI setup screens.

For the opposite direction, USB to BLE, see
[pico_usb_ble_hid_bridge](https://github.com/shiomachisoft/pico_usb_ble_hid_bridge).

<img width="716" height="391" alt="image" src="https://github.com/user-attachments/assets/6d4410d5-2912-4bd5-93dc-8aef206fb2b0" />

## Usage

1.  Plug the board into a USB port. The LED blinks while nothing is connected
    over BLE.
2.  Put the keyboard or mouse into pairing mode; its manual will say how.
3.  The LED goes solid once the device is connected, and the PC sees a USB input
    device.

If your device requires a paring code, you will need to read that from the
logs. The firmware logs what it is doing over the UART pins. See
[docs/troubleshooting.md](docs/troubleshooting.md) for how to read it.

After the first pairing, the Pico persistently stores which device it needs to
reconnect to, at the next power-on. Some peripherals sleep deeply, and do not
reconnect unprompted — press a key or two to wake them up and reconnect.

## How it works

**Two cores.** Core 0 runs the USB device stack and Core 1 runs BTstack, so a
report received over BLE can be sent over USB with little delay between the two.
The USB endpoint is polled every 1 ms.

**Pass-through.** The HID report descriptor is read from the BLE device and
handed to the PC unchanged, so device-specific keys such as media controls keep
working. Input reports are forwarded byte for byte. The USB device re-enumerates
once the BLE link is up, which is what makes the PC read the new descriptor.

**Connection handling.** The bridge alternates between reconnecting to a bonded
device and scanning for new ones. Once the link is encrypted it asks for a
12.5-15 ms connection interval, so a power-saving default on the peripheral does
not turn into input lag.

## Documentation

- [Building](docs/building.md) — toolchains, flashing, build options
- [Building in VS Code](docs/vscode_instructions.md)
- [Troubleshooting](docs/troubleshooting.md)
- [Verified devices](docs/verified_devices.md)

## License

Built from the TinyUSB `dev_hid_composite` and BTstack `hog_host_demo` samples;
see LICENSE.TXT for the terms of both.
