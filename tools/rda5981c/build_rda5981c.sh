#!/bin/bash

export CHIP_NAME=${1}
export PROJECT_TITLE=${2}

export STDK_PATH="${PWD}"
export CORE_PATH="${PWD}/iot-core"

IOT_APPS_PATH="${PWD}/apps/${CHIP_NAME}"
PROJECT_PATH="${IOT_APPS_PATH}/${PROJECT_TITLE}"

CA_CERT_FILE="root_ca_prod.pem"

print_usage () {
  echo "    Usage: ./build.sh CHIP_NAME PROJECT_NAME [make_option]"
  echo "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -"
  echo "    ex) ./build.sh ${CHIP_NAME} st_switch"
  echo "    ex) ./build.sh ${CHIP_NAME} st_switch flash"
  echo
}
if [ ! -d bsp/${CHIP_NAME}/apps ]; then
  mkdir bsp/${CHIP_NAME}/apps
  ln -s ${PROJECT_PATH} bsp/${CHIP_NAME}/apps
fi

if [ "${PROJECT_TITLE}" = "" ]; then
  print_usage
  exit
fi

if [ ! -d ${PROJECT_PATH} ]; then
  echo "    Error: Fail to find ${PROJECT_PATH}"
  print_usage
  exit 1
fi

MC="-DCONFIG_STDK_IOT_CORE_SECURITY_BACKEND_SOFTWARE -DCONFIG_STDK_IOT_CORE_USE_LWIP -DCONFIG_STDK_IOT_CORE -DCONFIG_STDK_IOT_CORE_USE_MBEDTLS -DCONFIG_STDK_IOT_CORE_NET_MBEDTLS -DCONFIG_STDK_IOT_CORE_CRYPTO_SUPPORT_ED25519 -DSTDK_IOT_CORE_EASYSETUP_POSIX_TESTING -DCONFIG_STDK_IOT_CORE_LOG_LEVEL_WARN -DCONFIG_STDK_IOT_CORE_LOG_LEVEL_INFO -DCONFIG_STDK_IOT_CORE_LOG_LEVEL_DEBUG -DCONFIG_MBEDTLS_XTEA_C=1 -DCONFIG_MBEDTLS_KEY_EXCHANGE_RSA=1 -DCONFIG_MBEDTLS_AES_C=1 -DCONFIG_MBEDTLS_RSA_BITLEN_MIN=2048 -DCONFIG_MBEDTLS_SSL_PROTO_TLS1=1 -DCONFIG_MBEDTLS_ECDH_C=1 -DCONFIG_MBEDTLS_PEM_WRITE_C=1 -DCONFIG_MBEDTLS_SSL_OUT_CONTENT_LEN=4096 -DCONFIG_STDK_IOT_CORE_USE_MBEDTLS=1 -DCONFIG_SSL_USING_MBEDTLS=1 -DCONFIG_MBEDTLS_ECP_DP_CURVE25519_ENABLED=1 -DCONFIG_MBEDTLS_ECP_C=1 -DCONFIG_MBEDTLS_RC4_DISABLED=1 -DCONFIG_MBEDTLS_X509_CRL_PARSE_C=1 -DCONFIG_LIBSODIUM_USE_MBEDTLS_SHA=1 -DCONFIG_MBEDTLS_RSA_BITLEN_2048=1 -DCONFIG_MBEDTLS_SSL_PROTO_TLS1_1=1 -DCONFIG_MBEDTLS_X509_CSR_PARSE_C=1 -DCONFIG_MBEDTLS_PEM_PARSE_C=1 -DCONFIG_MBEDTLS_SSL_PROTO_TLS1_2=1 -DCONFIG_MBEDTLS_TLS_ENABLED=1 -DCONFIG_MBEDTLS_TLS_CLIENT=1 -DCONFIG_MBEDTLS_ECP_DP_SECP256R1_ENABLED=1 -DCONFIG_MBEDTLS_HAVE_TIME=1 -DCONFIG_MBEDTLS_TLS_CLIENT_ONLY=1 -DCONFIG_MBEDTLS_SSL_IN_CONTENT_LEN=6144 -DCONFIG_STDK_MQTT_USE_SSL"

#Genarate Root CA
cd ${CORE_PATH}
cat src/include/certs/boilerplate.h > src/iot_root_ca.c
xxd -i src/certs/${CA_CERT_FILE} >> src/iot_root_ca.c
sed -i.bak 's/src.*pem/st_root_ca/g' src/iot_root_ca.c
sed -i.bak 's/unsigned/const unsigned/g' src/iot_root_ca.c
rm -f src/iot_root_ca.c.bak
cd ${STDK_PATH}

#Genarate Object Files from json files
cd ${PROJECT_PATH}/main
/opt/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-ld -r -b binary -o onboarding_config.o onboarding_config.json
/opt/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-ld -r -b binary -o device_info.o device_info.json
cd ${STDK_PATH}

cd ./bsp/${CHIP_NAME}
### Build
mbed compile -m UNO_91H -t GCC_ARM  ${MC}
