# Changelog

## Unreleased

### Added

- Pico 2 W (RP2350) support alongside the Pico W. `PICO_BOARD` picks the target
  and defaults to `pico2_w`.
- `flake.nix`, pinning the compiler, CMake, picotool and Pico SDK 2.2.0.
- `ENABLE_USB_LOGGING`, `ENABLE_HEARTBEAT_LOGS` and `UART_BAUD_RATE` build
  options.
- `docs/` with build, VS Code, troubleshooting and verified-device guides.

### Changed

- Sources moved from `src_fw/picow_ble_usb_hid_bridge/` to `src/`.
- The project and executable are now `ble_usb_hid_bridge`, so the build produces
  `ble_usb_hid_bridge.uf2`. `picow_bt_example_common.{c,h}` became
  `bt_example_common.{c,h}`.
- Log messages carry a `[SYS]`, `[BLE]` or `[USB]` prefix, since both cores
  share one console.
- The USB string descriptors name the project instead of the TinyUSB sample.
- `pico_flash` is a declared dependency, since `flash_safe_execute_core_init()`
  is called directly.
- The prebuilt `binary/*.uf2` is no longer tracked.

### Fixed

- Keyboards that place a descriptor after the CCC descriptor no longer stall in
  GATT discovery, via BTstack's `ENABLE_GATT_LEGACY_CCC_DISCOVERY`.
- The HID report descriptor cache grew from 500 bytes to 2 KB, so descriptors
  from keyboards with media keys or multi-device switching are not truncated.
- Core 0 no longer touches the CYW43-attached status LED before Core 1 has
  finished `cyw43_arch_init()`, which could hang Core 0 during boot.
- A 12.5-15 ms connection interval is requested once the link is encrypted,
  instead of accepting the device's power-saving default.
