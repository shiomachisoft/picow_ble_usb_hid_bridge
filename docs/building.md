# Building

## With Nix

`flake.nix` pins the compiler, CMake, picotool and Pico SDK 2.2.0, so nothing
has to be installed system-wide:

``` bash
nix develop
cmake -S src -B build
cmake --build build
```

## Without Nix

Any environment with `arm-none-eabi-gcc`, CMake and `PICO_SDK_PATH` pointing at
Pico SDK 2.2.0 works the same way. To build from the Raspberry Pi Pico extension
for Visual Studio Code instead, see
[vscode_instructions.md](vscode_instructions.md).

## Flashing

The build writes `build/ble_usb_hid_bridge.uf2`. Hold `BOOTSEL` while plugging
the board in, then copy that file onto the `RPI-RP` drive that appears.

## Options

Options go on the configure step:

``` bash
cmake -S src -B build -DPICO_BOARD=pico_w -DENABLE_USB_LOGGING=ON
```

| Option | Default | Effect |
|----|----|----|
| `PICO_BOARD` | `pico2_w` | Target board, `pico2_w` or `pico_w`. |
| `ENABLE_USB_LOGGING` | `OFF` | Log USB device events and every forwarded HID report. |
| `ENABLE_HEARTBEAT_LOGS` | `OFF` | Log a heartbeat line every 5 seconds to show Core 0 is alive. |
| `UART_BAUD_RATE` | `115200` | Baud rate of the log UART. |

## Formatting

CMake files are formatted with
[gersemi](https://github.com/BlankSpruce/gersemi), configured by `.gersemirc`
and included in the dev shell:

``` bash
gersemi -i src/CMakeLists.txt   # or --check to verify without writing
```

The block the Raspberry Pi Pico VS Code extension marks "DO NOT EDIT" is fenced
off with `# gersemi: off` / `# gersemi: on` and left exactly as generated.
