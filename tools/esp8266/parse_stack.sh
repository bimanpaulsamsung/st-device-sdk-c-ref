#!/bin/bash

# toolchain path
ADDR2LINE=~/esp/xtensa-lx106-elf/bin/xtensa-lx106-elf-addr2line
READELF=~/esp/xtensa-lx106-elf/bin/xtensa-lx106-elf-readelf

# elf and log (use these paths if no args
ELF=~/workspace/model/esp8266/esp8266/output/iotcore_simple_wifi_20190513_b608226/debug/simple_wifi.elf
LOG=~/workspace/test/bsp-utils/wook/usr/sbin/log.txt

# select YES to skip some items (ex. 0x40232c04: cnx_update_bss_more at ??:?)
SKIP_QUESTON_MARK=YES

help()
{
    echo ""
    echo "<<parse_stack.sh usage>>"
    echo "     parse_stack.sh help  -> This help!!"
    echo "     parse_stack.sh {elf_path} {log_path} - use elf and log from user input"
    echo "     parse_stack.sh                       - use elf and log from defined in shell script"
    echo ""
}


prepare_decode_line()
{
    INIT=0
    STEP=0

    while read line
    do
        A1=`echo ${line} | awk -F' |)' '{print $1}'`
        A2=`echo ${line} | awk -F' |)' '{print $2}'`
        A7=`echo ${line} | awk -F' |)' '{print $7}'`
        A8=`echo ${line} | awk -F' |)' '{print $8}'`

        if [[ ${A1} == "Task" ]] && [[ ${A2} == "stack" ]]; then
            NUM_STACK=$((${NUM_STACK}+1))
            #echo "stack found - ${NUM_STACK}"
        fi
        if [[ ${A1} == "PC:" ]] && [[ ${A7} == "A1:" ]]; then
            NUM_SP=$((${NUM_SP}+1))
            #echo "SP found - ${NUM_SP}"
            SP=${A8}
            #echo SP=${SP}
        fi
    done < ${LOG}
}

prepare_decode_word()
{
    INIT=0
    STEP=0
    GET_SP=0

    for var in $FILE ;
    do
        if [[ ${GET_SP} -eq 1 ]] ; then
            SP=${var}
            echo SP=${SP}
            GET_SP=0
        fi
        if [[ ${var} == "Task" ]] ; then
            NUM_STACK=$((${NUM_STACK}+1))
            echo "stack found - ${NUM_STACK}"
        fi
        if [[ ${var} == "A1:" ]] ; then
            NUM_SP=$((${NUM_SP}+1))
            echo "SP found - ${NUM_SP}"
            GET_SP=1;
        fi

    done
}

decode_main_with_sp_line()
{
    while read line
    do
        NEED_TO_COUNT=0
        #echo "++${line}--"
        #echo +++${line}___
        ADDR=`echo ${line} | /usr/bin/awk -F' ' '{print $1}'`
        #echo ${ADDR}

        if [[ ${ADDR} == 3ff* ]] ; then
            DIFF=$((${SP}-0x${ADDR}))
            if [[ ${DIFF} -lt 64 ]] && [[ ${DIFF} -gt 0 ]]; then
                #echo yahoo!!! SP=${SP}, ADDR=${ADDR}, DIFF=${DIFF}
                NEED_TO_COUNT=1
            fi
        fi

        for var in ${line} ;
        do
            if [ ${NEED_TO_COUNT} -eq 1 ]; then
                DIFF=$((${DIFF}-4))
                #echo @@@@@1_${var}__diff=${DIFF}

                if [ ${DIFF} -eq -4 ]; then
                    echo ">>>>>(stack pointer)>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
                    #echo @@@@@2_${var}__diff=${DIFF}
                    #DIFF=1000
                    #NEED_TO_COUNT=0
                fi
            fi

           if [[ ${var} == "Task" ]] ; then
            echo
            echo "-----(task stack)-----------------------------------------------------------------------------------------------------------------------------"
            #echo ___${line}___
           fi

           if [[ ${var} == "PC:" ]] ; then
            echo "-----(register)-----------------------------------------------------------------------------------------------------------------------------"
            #echo ___${line}___
           fi

           if [[ ${var} == *40[0-2]* ]] ; then
        #       cat ${var} | /usr/bin/awk -F':| ' '{print $2"," $4}' >> ${RESULT_FILE}
                WOOK=`${ADDR2LINE} -e ${ELF} -a -p -f -C ${var}`

                if [ "${SKIP_QUESTON_MARK}" == NO ]; then
                    echo ${WOOK}
                else
                    if [[ ${WOOK} != *\?* ]]; then
                        DUMMY=1
                        echo "${WOOK}"
                    fi
                fi
            fi
        done

    done < ${LOG}
}

decode_main_with_sp_word()
{
    for var in $FILE ;
    do

        if [[ ${var} == "Task" ]] ; then
            echo
            echo "-----(task stack)-----------------------------------------------------------------------------------------------------------------------------"
            #echo ___${line}___
        fi

        if [[ ${var} == "PC:" ]] ; then
            echo "-----(register)-----------------------------------------------------------------------------------------------------------------------------"
            #echo ___${line}___
        fi

       if [[ ${var} == *40[0-2]* ]] ; then
    #       cat ${var} | /usr/bin/awk -F':| ' '{print $2"," $4}' >> ${RESULT_FILE}
            WOOK=`${ADDR2LINE} -e ${ELF} -a -p -f -C ${var}`

            if [ "${SKIP_QUESTON_MARK}" == NO ]; then
                echo ${WOOK}
            else
                if [[ ${WOOK} != *\?* ]]; then
                    DUMMY=1
                    echo "${WOOK}"
                fi
            fi
        fi
    done
}
decode_main_parse_only()
{
    for var in $FILE ;
    do
        #echo "++${var}--"

        if [[ ${var} == "Task" ]] ; then
            echo
            echo "-----(task stack)-----------------------------------------------------------------------------------------------------------------------------"
            #echo ___${line}___
        fi
        if [[ ${var} == "PC:" ]] ; then
            echo "-----(register)-----------------------------------------------------------------------------------------------------------------------------"
            #echo ${line}
        fi

       if [[ ${var} == *40[0-2]* ]] ; then
    #       cat ${var} | /usr/bin/awk -F':| ' '{print $2"," $4}' >> ${RESULT_FILE}
            WOOK=`${ADDR2LINE} -e ${ELF} -a -p -f -C ${var}`

            if [ "${SKIP_QUESTON_MARK}" == NO ]; then
                echo ${WOOK}
            else
                if [[ ${WOOK} != *\?* ]]; then
                    DUMMY=1
                    echo "${WOOK}"
                fi
            fi
        fi
    done
}



if [ "$1" == help ]; then
    help
    exit 0
fi

if [ $# -eq 2 ]; then
    #echo "2 args"
    ELF=$1
    LOG=$2
elif [ $# -eq 0 ]; then
    echo "No args. Use path in the shell file"
else
    echo "Invalid args"
    help
    exit -1
fi


echo "ELF=${ELF}"
echo "LOG=${LOG}"
echo

FILE=`cat ${LOG}`
prepare_decode_line

if [[ ${NUM_STACK} -eq 1 ]] && [[ ${NUM_SP} -eq 1 ]]; then
    decode_main_with_sp_line
else
    decode_main_parse_only
fi
