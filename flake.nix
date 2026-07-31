{
  description = "Development environment for the Pico W / Pico 2 W BLE to USB HID bridge firmware";

  inputs = {
    nixpkgs.url = "nixpkgs/26.05";
  };

  outputs = { self, nixpkgs }:
    let
      systems = [ "x86_64-linux" "aarch64-linux" ];
      forAllSystems = nixpkgs.lib.genAttrs systems;
    in
    {
      devShells = forAllSystems (system:
        let
          pkgs = nixpkgs.legacyPackages.${system};

          # The firmware pulls TinyUSB and BTstack out of the SDK tree, so the
          # SDK submodules have to be present.
          pico-sdk-full = pkgs.pico-sdk.override { withSubmodules = true; };
        in
        {
          default = pkgs.mkShell {
            packages = with pkgs; [
              gcc-arm-embedded # provides arm-none-eabi-gcc and arm-none-eabi-gdb
              cmake
              gnumake
              (python3.withPackages (ps: [ ps.pycryptodomex ])) # the SDK runs compile_gatt.py to build the GATT database

              pico-sdk-full
              picotool

              clang-tools # clangd, reads the build directory's compile_commands.json
              gersemi # CMake formatter, configured by .gersemirc
              tio # serial monitor for the log UART
            ];

            env.PICO_SDK_PATH = "${pico-sdk-full}/lib/pico-sdk";
          };
        });
    };
}
