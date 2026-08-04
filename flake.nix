{
  description = "Development environment for the Pico W / Pico 2 W BLE to USB HID bridge firmware";

  inputs = {
    nixpkgs.url = "nixpkgs/26.05";
  };

  outputs =
    { self, nixpkgs }:
    let
      systems = [
        "x86_64-linux"
        "aarch64-linux"
        "aarch64-darwin"
      ];
      forAllSystems = nixpkgs.lib.genAttrs systems;

      perSystem = forAllSystems (
        system:
        let
          pkgs = nixpkgs.legacyPackages.${system};

          # The firmware pulls TinyUSB and BTstack out of the SDK tree, so the
          # SDK submodules have to be present.
          pico-sdk-full = pkgs.pico-sdk.override { withSubmodules = true; };

          toolchain = [
            # provides arm-none-eabi-gcc and arm-none-eabi-gdb
            pkgs.gcc-arm-embedded
            pkgs.cmake
            pkgs.gnumake
            (pkgs.python3.withPackages (ps: [ ps.pycryptodomex ])) # the SDK runs compile_gatt.py to build the GATT database

            pico-sdk-full
            pkgs.picotool
          ];

          # board must name a preset in CMakePresets.json.
          mkFirmware =
            board:
            pkgs.stdenv.mkDerivation {
              name = "ble_usb_hid_bridge-${board}";
              src = ./.;

              nativeBuildInputs = toolchain;
              env.PICO_SDK_PATH = "${pico-sdk-full}/lib/pico-sdk";

              configurePhase = "cmake --preset ${board}";
              buildPhase = "cmake --build --preset ${board}";
              installPhase = ''
                install -D build/${board}/ble_usb_hid_bridge.uf2 $out/ble_usb_hid_bridge.uf2
              '';
            };
        in
        {
          inherit
            pkgs
            pico-sdk-full
            toolchain
            mkFirmware
            ;
        }
      );
    in
    {
      devShells = forAllSystems (
        system:
        let
          inherit (perSystem.${system}) pkgs pico-sdk-full toolchain;
        in
        {
          default = pkgs.mkShell {
            packages = toolchain ++ [
              pkgs.clang-tools # clangd, reads the build directory's compile_commands.json
              pkgs.gersemi # CMake formatter, configured by .gersemirc
              pkgs.tio # serial monitor for the log UART
            ];

            env.PICO_SDK_PATH = "${pico-sdk-full}/lib/pico-sdk";
          };
        }
      );

      # `nix build .#pico_w` / `.#pico2_w`; plain `nix build` gives the pico2_w
      # firmware, matching the board CMakeLists.txt defaults to.
      packages = forAllSystems (
        system:
        let
          inherit (perSystem.${system}) mkFirmware;
        in
        {
          pico_w = mkFirmware "pico_w";
          pico2_w = mkFirmware "pico2_w";
          default = mkFirmware "pico2_w";
        }
      );
    };
}
