#!/bin/bash

BSP_NAME=${1}
CORE_PATH="iot-core"

cd ${CORE_PATH}
git submodule status &> /dev/null
if [ "$?" = "0" ]; then
	git submodule init
	git submodule update
	git submodule foreach "git reset --hard HEAD"
fi
