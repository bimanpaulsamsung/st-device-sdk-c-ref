# SmartThings Device SDK Reference

[![License](https://img.shields.io/badge/licence-Apache%202.0-brightgreen.svg?style=flat)](LICENSE)

The SmartThings Device SDK(STDK for short) Reference is the git repository of examples using the core device library that allow device applications to securely connect to the SmartThings Cloud. To facilitate the development of device application in an original chipset SDK, the core device library and the examples were separated into two git repositories. That is, if you want to use the core device library in your original chipset SDK that installed before, you may simply link it to develop a device application in your existing development environment.

It is distributed in source form and written in C99 for the purpose of portability to most platforms. If you want to know the overall workflow of using this SDK, please refer to the [Getting Started](https://github.com/SmartThingsCommunity/st-device-sdk-c-ref/blob/master/doc/getting_started.md).

## Announcements

- ### WARNING : PLEASE USE master branch only until mid of Aug. 2020. (estimated plan)

All version 1.2.xx at 'develop' branch contains new features which aren't compatible with the current SmartThings mobile application. So use of version 1.1.18 at 'master' branch is highly recommended for your own project. Release of the new compatible SmartThings mobile application has schedule at mid of Aug. We will inform you further information when these new features merge to 'master' branch

## Directory layout

The reference git is delivered via the following directory structure :

- `apps` : sample device applications for each chipset
- `bsp` : An original chipset vendor's SDK is located.  If you use a chipset that has already been ported, this vendor's SDK can be easily downloaded as a submodule in this directory through the predefined script(e.g. `setup.sh`).
- `doc` : documents
- `iot-core` : [IoT core device library](https://github.com/SmartThingsCommunity/st-device-sdk-c). It can also be downloaded as a submodule in this directory through the predefined script(e.g. `setup.sh`).
- `output` : build outputs will be placed
- `patches` : patches to be applied in the original chipset vendor's SDK for resolving some problems
- `tools` : scripts to be applied for each chipset

## Building a sample device application

Basically, this release builds on the environments of chipset vendor's SDKs.

### Prerequisites

- Install the toolchain defined in the chipset SDK you use. If there is no environmental comment, it is basically only described for 64-bit Ubuntu-based.
  - Example for ESP8266 (Ubuntu/Debian quickstart)
    - Setup [ESP8266 Toolchain for Linux](https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/linux-setup.html) according to the available Expressif website.
      In order to use the pre-supplied build script(e.g. `build.sh`), please extract [the toolchain](https://dl.espressif.com/dl/xtensa-lx106-elf-linux64-1.22.0-100-ge567ec7-5.2.0.tar.gz) into `~/esp/xtensa-lx106-elf/` directory like the original Expressif guide. And according to the above Espressif guideline, you will need to add the toochain path to your PATH environment variable in ~/.profile file. But it is not necessary if you use the pre-supplied build script. Because that path is automatically exported in the build script.

      > Info :
      >
      > The ESP8266 example of STDK was developed from the 19cfb19 commit ID based on ESP8266_RTOS_SDK v3.2.

  - Example for ESP32 (Ubuntu/Debian quickstart)

    - Setup [ESP32 Toolchain for Linux](https://docs.espressif.com/projects/esp-idf/en/stable/get-started/linux-setup.html) according to the available Expressif website.
      In order to use the pre-supplied build script(e.g. `build.sh`), please extract [the toolchain](https://dl.espressif.com/dl/xtensa-esp32-elf-linux64-1.22.0-80-g6c4433a-5.2.0.tar.gz) into `~/esp/xtensa-esp32-elf/` directory like the original Expressif guide. And according to the above Espressif guideline, you will need to add the toochain path to your PATH environment variable in ~/.profile file. But it is not necessary if you use the pre-supplied build script. Because that path is automatically exported in the build script.

      > Info :
      >
      > The ESP32 example of STDK was developed from the beb34b5 commit ID based on esp-idf v3.3.

  - Example for RTL8195

    - Install [mbed Microcontroller](https://os.mbed.com/handbook/Windows-serial-configuration) to use the USB serial port on Windows.
    - Upgrade [DAP F/W](https://www.amebaiot.com/en/change-dap-firmware/)
      Recommended DAP F/W is the `DAP_FW_Ameba_V12_1_3-2M.bin`

      > Note :
      >
      > You have to proceed the above steps at Windows PC for downloading a binary. But you have to build the source code on the cygwin or linux for this chipset.

  - Example for EMW3166

    - Setup [MiCoder Toolchain for Linux](http://firmware.mxchip.com/MiCoder_v1.1.Linux.tar.gz) according to the available MXCHIP website.
    - Install [MiCO Cube](http://developer.mxchip.com/developer/md/bWljby1oYW5kYm9vay8yLk1pQ09fdG9vbHMvc2VjX2xpbmtfcGFnZS9NaUNPX0N1YmVfRW5nbGlzaC5tZA)
      In order to use the pre-supplied build script(e.g. `build.sh`), please set `mico config --global MICODER /path_of_the_toolchain`

  - Example for Cypress cy8cproto_062_4343w (Ubuntu/Debian quickstart)

    - Install [mbed cli](doc/mbed_cli_setup.md) on linux.
    - Install [arm toolchain](doc/mbed_cli_setup.md) and configure with `mbed cli`.


### Build

1. Download the STDK Reference source code. Basically, this STDK Reference will download the ported original chipset vendor's SDKs as submodules in the `bsp` directory through the `setup.sh` script. If a chipset vendor's SDK does not exist as git format, you can manually copy it under the `bsp` directory.

   - Download the source code via `git clone`.
   - And then, run the `setup.sh` to automatically download submodules to the `bsp` directory. At this time, the IoT core device library is also downloaded to the `iot-core` directory.
     ```sh
     $ cd ~
     $ git clone https://github.com/SmartThingsCommunity/st-device-sdk-c-ref.git
     $ cd st-device-sdk-c-ref
     $ ./setup.sh
         Usage: ./setup.sh CHIP_NAME
     - - - - - - - - - - - - - - - - - - -
         ex) ./setup.sh esp8266
         ex) ./setup.sh esp32
         ex) ./setup.sh rtl8195
         ex) ./setup.sh rtl8720c
         ex) ./setup.sh rtl8721c
         ex) ./setup.sh emw3166
         ex) ./setup.sh cy8cproto_062_4343w

     $ ./setup.sh esp8266
     ```

2. Check the build configuration of a sample device application. If you want to use specific build options, you can directly modify the build configuration file(e.g. sdkconfig, sdkconfig.h) at the root directory of a sample device application. On the Espressif chipset, you can additionally use the `menuconfig` option to configure them.

   - If you just want to use the default build configuration, you can skip this step.
   - Example for ESP8266
     > Note :
     > The `menuconfig` option is just supported on the Espressif chipset.
     ```sh
     # ./build.sh {app_path} {option}
     $ cd ~/st-device-sdk-c-ref
     $ ./build.sh apps/esp32/switch_example menuconfig
     ```

3. Run `build.sh` in the SDK's root directory. This builds the sample executables and places them in the `output/{chip_name}/`.

   ```sh
   $ ./build.sh apps/esp32/switch_example          # ./build.sh {app_path}

   ```

### flash & monitor

Serial port needs to be matched to the computer environment for serial port flashing and monitoring.

- Example for ESP8266/ESP32
  - Baud rate 115200 for flashing or 74880 for monitoring, Data bit 8, Parity None, Stop bits 1
  - These data are described in the build configuration file(e.g.  sdkconfig)
- Example for RTL8195
  - Baud rate 115200, Data bit 8, Parity None, Stop bits 1
- Example for EMW3166
  - Baud rate 9600 for flashing(user uart) and 115200 for monitoring(USB uart), Data bit 8, Parity None, Stop bits 1
- Example for Cypress cy8cproto_062_4343w
  - Baud rate 9600, Data bit 8, Parity None, Stop bits 1

  You can open a serial terminal to the tty port of a connected Mbed target (usually board) using the mbed sterm command. If no tty port is specified, Mbed CLI detects the connected Mbed targets and their tty ports.

  `$ mbed sterm`

  > Note: If port is not opening, you may need to use `sudo mbed sterm`

Flashing can be done according to the method supported by chipset SDK.

- Example for ESP8266/ESP32

  You can flash the executables into a target device via `build.sh` with additional option. Actually, you don't need to run `./build.sh esp8266 st_switch` before running `./build.sh esp8266 st_switch flash`, this will automatically rebuild anything which needs it.

  - options
    - clean : clean previous build outputs
      ```sh
      # ./build.sh {app_path} {options}
      $ ./build.sh apps/esp32/switch_example clean
      ```
    - flash : download executable binaries to the device
    - monitor : monitor the serial output of device. this option can be used with flash option.
      ```sh
      # ./build.sh {app_path} {options}
      $ ./build.sh apps/esp32/switch_example flash monitor
      ```

- Example for Cypress cy8cproto_062_4343w

  You can flash the executables into a target device via `build.sh` with additional option. Actually, you don't need to run `./build.sh cy8cproto_062_4343w st_switch` before running `./build.sh cy8cproto_062_4343w st_switch flash`, this will automatically rebuild anything which needs it.

  - options
    - flash : download executable binaries to the device
      ```sh
      # ./build.sh {chip_name} {app_name} {options}
      $ ./build.sh cy8cproto_062_4343w st_switch flash
      ```

- Example for RTL8195

  In order to flash the RTL chipset binary, you have to proceed steps below at Windows PC, even though you build the source code on Linux environment.
  - Connect Ameba RTL8195 to Windows PC, you can find removable disk named MBED.
  - Just copy `ram_all.bin` in `output/rtl8195/iotcore_xxx/ram_all.bin` to MBED.
  - After copy successfully, please reset Ameba RTL8195 target and monitor

- Example for EMW3166

  In order to flash the MXCHIP emw3166 chipset binary, you have to proceed steps below at Windows PC, with a terminal support Ymodem transmission.
  - Connect user uart(PB7-tx, PB6-rx) of emw3166 to Windows PC with a USB-TTL convertor, and connect USB uart for power.
  - Create serial session with flashing config (Baud rate 9600) to connect user uart port, then hold 'BOOT' and press 'RESET' to enable flashing
  - Tansfer binary file in `output/emw3166/iotcore_xxx/xxx@MK3166.bin` with Ymodem on terminal
  - After sending successfully, press 'RESET' on MiCOKit-3166 target and monitor with USB uart

## License

This library is licensed under the [Apache License Ver2.0](LICENSE).
