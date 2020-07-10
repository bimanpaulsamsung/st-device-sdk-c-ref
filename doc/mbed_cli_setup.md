# Setup Mbed CLI in Linux

## Getting Mbed CLI

### Install Python and Pip

Download and install [Python versions 2.7.11 or 3.6.5 or later](https://www.python.org/downloads/source/), or use your distribution's package manager to install Python and Pip.

As an example, you can use the following in Ubuntu:

`$ sudo apt-get install python2.7 python-pip`

### Install Mbed CLI

You can install the latest stable version of Mbed CLI through pip by running:

`$ pip install mbed-cli`

You can ensure Mbed CLI installed correctly by running `mbed help`.

## Download and Install gnu arm cross compiler toolchain

Toolchain is downloadable from [arm developer website](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads). Make sure to select the appropriate file for linux based on architecture. STDK has been tested with `GNU Arm Embedded version 9 (9-2019-q4-major)`.

Once downloaded, extract the archive to appropriate location. Keep a note of the path as this would be required in configuring Mbed CLI.


## Mbed CLI configuration in linux with gnu arm embedded toolchain

Add gnu arm embedded toolchain path to the Mbed CLI global configuration:

```sh
$ mbed config -G GCC_ARM_PATH "<path to GCC ARM bin\>"
[mbed] <path to GCC ARM bin\> now set as global GCC_ARM_PATH
```

To see the configurations use the below command:

```sh
$ mbed config --list
[mbed] Global config:
ARM_PATH=<path to ARM bin\>
```