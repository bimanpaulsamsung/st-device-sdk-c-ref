#!/bin/bash

CHIP_NAME=${1}

export DEBUG_CONFIG=1
export CONFIG_SAMSUNG_BUILD_ENG=y

tools/${CHIP_NAME}/build_${CHIP_NAME}.sh $@
