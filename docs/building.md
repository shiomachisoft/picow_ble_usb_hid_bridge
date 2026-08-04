# Building

## With Nix, as a package

`flake.nix` exposes one package per board, each producing a standalone,
sandboxed build of `ble_usb_hid_bridge.uf2`:

``` bash
nix build .#pico_w    # -> result/ble_usb_hid_bridge.uf2, for the Pico W
nix build .#pico2_w   # -> result/ble_usb_hid_bridge.uf2, for the Pico 2 W
nix build              # same as .#pico2_w
```

## With Nix, in a dev shell

`nix develop` pins the compiler, CMake, picotool and Pico SDK 2.2.0 in a
shell, so nothing has to be installed system-wide, and you drive CMake
yourself — useful for incremental builds or a board configuration that isn't
one of the two presets below:

``` bash
nix develop
cmake -B build -DPICO_BOARD=pico_w
cmake --build build
```

## Without Nix

Any environment with `arm-none-eabi-gcc`, CMake and `PICO_SDK_PATH` pointing at
Pico SDK 2.2.0 works the same way. To build from the Raspberry Pi Pico extension
for Visual Studio Code instead, see
[vscode_instructions.md](vscode_instructions.md).

## CMake presets

`CMakePresets.json` defines a `pico_w` and a `pico2_w` preset, each setting
`PICO_BOARD` and building into its own `build/<preset>/` directory so the two
boards' CMake caches never collide:

``` bash
cmake --preset pico_w
cmake --build --preset pico_w
```

## Flashing

The build writes `<binary dir>/ble_usb_hid_bridge.uf2` — `build/<preset>/` for
a preset build, `result/` for a Nix package. Hold `BOOTSEL` while plugging the
board in, then copy that file onto the `RPI-RP` drive that appears.

## Options

`PICO_BOARD` is picked by the preset (or the Nix package); the rest go on the
configure step, alongside or instead of a preset:

``` bash
cmake --preset pico_w -DENABLE_USB_LOGGING=ON
```

| Option | Default | Effect |
|----|----|----|
| `PICO_BOARD` | `pico2_w` | Target board, `pico2_w` or `pico_w`. Set by the `pico_w`/`pico2_w` presets. |
| `ENABLE_USB_LOGGING` | `OFF` | Log USB device events and every forwarded HID report. |
| `ENABLE_HEARTBEAT_LOGS` | `OFF` | Log a heartbeat line every 5 seconds to show Core 0 is alive. |
| `UART_BAUD_RATE` | `115200` | Baud rate of the log UART. |

## Formatting

CMake files are formatted with
[gersemi](https://github.com/BlankSpruce/gersemi), configured by `.gersemirc`
and included in the dev shell:

``` bash
gersemi -i CMakeLists.txt   # or --check to verify without writing
```

The block the Raspberry Pi Pico VS Code extension marks "DO NOT EDIT" is fenced
off with `# gersemi: off` / `# gersemi: on` and left exactly as generated.
