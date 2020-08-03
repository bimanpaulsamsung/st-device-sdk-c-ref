# SmartThings Device SDK Switch Example

## Introduction

SmartThings direct-connected device is Wi-Fi enabled device that uses the SmartThings cloud as its primary cloud infrastructure. And this device will use the MQTT protocol for communication.

## Getting started

For information on detailed workflow, please refer to the [Getting Started](https://github.com/SmartThingsCommunity/st-device-sdk-c-ref/blob/master/doc/getting_started.md)

## SmartThings Device SDK config
If you want to use specific SmartThings Device SDK build options, you can directly modify the build configuration file. For esp8266 example, SmartThings Device SDK config is saved in 'sdkconfig' file. If you want to change this, please execute the following :
```sh
# ./build.sh {app_path} {option}
$ cd ~/st-device-sdk-c-ref/
$ ./build.sh app/esp8266/switch_example menuconfig
```

