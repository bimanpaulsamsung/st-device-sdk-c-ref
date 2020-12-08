# SmartThings SDK for Direct Connected Devices for C - Switch Example

## Introduction

SmartThings direct-connected device is Wi-Fi enabled device that uses the SmartThings cloud as its primary cloud infrastructure. And this device will use the MQTT protocol for communication.

## Getting started

For information on detailed workflow, please refer to the [Getting Started](https://github.com/SmartThingsCommunity/st-device-sdk-c-ref/blob/master/doc/getting_started.md)

## Components and Capabilities

SmartThings Device is defined using components and capabilities. Capabilities define the features of the device, and capabilities are grouped into components.
Components and Capabilities are contained in device profile. You can create a device profile in Developer Workspace and associate it with an integration.

This example assumes the following component and capabilities are used. :  

`main` component  
- `healthCheck` capability  
- `switch` capability  

(`healthCheck` capability is automatically added by Developer Workspace. It doesn't need handler at device side)

## SmartThings SDK for Direct Connected Devices - Config
If you want to use specific SmartThings Device SDK build options, you can directly modify the build configuration file. For this example, SmartThings Device SDK config is saved in 'sdkconfig.mk' file. If you want to change this, please execute the following :
```sh
$ cd ~/st-device-sdk-c-ref/
$ vim app/xmc4800/switch_example/sdkconfig.mk
```

## Test device schematics
This example uses XMC4800 GPIO like below.  
Please refer below picture for __XMC48-IoTKit__.  
> Note: If your device's schematics doesn't match with belows. 
> Please modify GPIO defines for your device at [device_control.h](main/device_control.h)
> ```c
> #define GPIO_INPUT_BUTTON_PORT     XMC_GPIO_PORT3
> #define GPIO_INPUT_BUTTON          4
> 
> #define GPIO_OUTPUT_MAINLED_PORT   XMC_GPIO_PORT4
> #define GPIO_OUTPUT_MAINLED        0
> ```

### XMC48_IOTKIT  
| XMC48_IOTKIT                                                     |
|-------------------------------------------------------------------|
|![XMC48_IOTKIT](../../../doc/res/Switch_Example_XMC48_IOTKIT.png) |

