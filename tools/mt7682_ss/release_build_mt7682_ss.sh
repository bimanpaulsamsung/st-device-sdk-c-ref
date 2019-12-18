#!/bin/bash

CHIP_NAME=${1}

export RELEASE_CONFIG=1

tools/${CHIP_NAME}/build_${CHIP_NAME}.sh $@
