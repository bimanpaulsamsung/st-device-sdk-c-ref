# Setup and Build SmartThings Device SDK for Raspi OS

This version of STDK works on Raspi OS and has been tested with release 2020-05-27

## Prerequisites

- A stable Internet connection to download package dependencies and submodules during setup.

- Install `apt-transport-https` and `ca-certificates` packages, to clone the repositories over https.
  ```sh
  $ sudo apt install apt-transport-https ca-certificates
  ```

- Install the following package dependencies.
  ```sh
  $ sudo apt install build-essential psmisc libssl-dev libpthread-stubs0-dev
  $ sudo apt install wpasupplicant isc-dhcp-client libglib2.0-dev dnsmasq ntp
  ```

- Configure dnsmasq service.
  - Add following statement to `/etc/dnsmasq.conf` file.
    ```sh
    bind-interfaces
    ```
  - Restart dnsmasq service.
    ```sh
    sudo systemctl restart dnsmasq
    ```

- Setting up WiFi 5GHz on Device:\
  Developer needs to ensure their device is setup to work with 5G WiFi, if the 5GHz band is to be used.\
  STDK raspi_os port layer assumes 5G WiFi station support by matching "Band 2" in `iw phy` command output.

## Setup

- Clone the source code.
  ```sh
  $ cd ~
  $ git clone https://github.com/SmartThingsCommunity/st-device-sdk-c-ref.git
  $ cd st-device-sdk-c-ref
  ```
- Run the setup script.
  ```sh
  $ ./setup.sh raspi_os
  ```

## Build Configuration

- To use specific SmartThings Device SDK build options, directly modify the `sdkconfig` build configuration file present in the corresponding app directory. Refer to the app's `README.md` file for instructions to modify build options.

## Build

- Run the build script.
  ```sh
  # ./build.sh {app_path} {make_option}
  $ cd ~/st-device-sdk-c-ref
  $ ./build.sh apps/raspi_os/switch_example
  $ ./build.sh apps/raspi_os/switch_example clean
  ```

  > Info :
  >
  > A successful build generates the executable in the respective raspi_os app directory.

## Run

- Use the command displayed at the end of build script execution to run the app executable.

  > Note :
  >
  > Running the app executable requires sudo for security privileges.
