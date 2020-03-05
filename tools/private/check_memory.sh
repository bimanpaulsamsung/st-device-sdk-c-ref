#!/bin/bash

CHIP_NAME=${1}
PROJECT_TITLE=${2}

TIME=`date +%Y%m%d_%H%M%S`
COMMIT_ID=`git log --pretty=format:'%h' -n 1`
COMMIT_TS=`git log --pretty=format:'%ci' -n 1`
COMMIT_TS_CT=`git log --pretty=format:'%ct' -n 1`

VERSION=`cat VERSION`
RESULT_FILE_DETAILS=result_${VERSION}_${COMMIT_ID}_${TIME}_details.csv
RESULT_FILE_MAP=result_${VERSION}_${COMMIT_ID}_${TIME}_map.csv
RESULT_FILE_APP=result_${VERSION}_${COMMIT_TS_CT}_${COMMIT_ID}_${TIME}_app.csv


STDK_PATH="$PWD"
TOOL_PATH="${STDK_PATH}/tools/private"
PROJECT_PATH="${STDK_PATH}/apps/${CHIP_NAME}/${PROJECT_TITLE}"


print_usage () {
  echo "    Usage: ${TOOL_PATH}/check_memory.sh CHIP_NAME PROJECT_NAME"
  echo "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -"
  echo "    ex) ${TOOL_PATH}/check_memory.sh esp8266 smart_switch"
  echo
}


get_map_info()
{
	MAP_FILE=`find . -name ${PROJECT_NAME}.map`

	if [ "${MAP_FILE}" = "" ]; then
	    print_usage
	    exit -1
	fi

	cd ${STDK_PATH}
	rm -f ${RESULT_FILE_MAP}

	${TOOL_PATH}/map_parser.py ${PROJECT_PATH}/build/${MAP_FILE} ${RESULT_FILE_MAP}

	mv ${RESULT_FILE_MAP} ${PROJECT_PATH}/build/
	cd ${PROJECT_PATH}/build
	echo "Result is saved to ${RESULT_FILE_MAP}"
}

get_details()
{
	rm  tmp1 -f
	rm  tmp2 -f

	# find all arcive files
	find . -name *.a > tmp1

	# get text, data, bss size by using size command
	while read line
	do
	    size ${line} 2> /dev/null >> tmp2
	done < tmp1


	# Below is out of size command. dec and hex are total(text+data+bss)
	#   text    data     bss     dec     hex filename
	#   1404       0       8    1412     584 partition.o (ex ./build/spi_flash/libspi_flash.a)
	#   3611      24       1    3636     e34 spi_flash.o (ex ./build/spi_flash/libspi_flash.a)
	#   1923       0       0    1923     783 spi_flash_raw.o (ex ./build/spi_flash/libspi_flash.a)

	echo "archive,object,text,data,bss,total" > ${RESULT_FILE_DETAILS}

	while read line
	do
	    A1=`echo ${line} | awk -F' |)' '{print $1}'`
	    #echo ${A1}

	    # If A1 is text, ignore it. It is title
	    if [[ "${A1}" != "text" ]]; then
	        A2=`echo ${line} | awk -F' |)' '{print $2}'`
	        A3=`echo ${line} | awk -F' |)' '{print $3}'`
	        A4=`echo ${line} | awk -F' |)' '{print $4}'`
	        #A5=`echo ${line} | awk -F' |)' '{print $5}'`
	        A6=`echo ${line} | awk -F' |)' '{print $6}'`
	        #A7=`echo ${line} | awk -F' |)' '{print $7}'`
	        A8=`echo ${line} | awk -F' |)' '{print $8}'`
	        #A9=`echo ${line} | awk -F' |)' '{print $9}'`
	        #A10=`echo ${line} | awk -F' |)' '{print $10}'`
	        #echo ++$A1--$A2--$A3--$A4--$A5--$A6--$A7--$A8--$A9--$A10__
	        echo "$A8,$A6,$A1,$A2,$A3,$A4" >> ${RESULT_FILE_DETAILS}
	    fi
	done < tmp2

	echo "Result is saved to ${RESULT_FILE_DETAILS}"
}

get_app()
{
	rm  tmp1 -f
	rm  tmp2 -f
	rm  tmp3 -f

	# find app bin file
	PROJECT_NAME=`cat ../Makefile | grep ^PROJECT_NAME | awk '{print $3}'`
	find . -name ${PROJECT_NAME}.elf > tmp1

	# get text, data, bss size by using size command
	while read line
	do
	    size ${line} 2> /dev/null >> tmp2
	    readelf -S ${line} 2> /dev/null >> tmp3
	done < tmp1


	# Below is out of size command. dec and hex are total(text+data+bss)
	#   text    data     bss     dec     hex filename
	# 316027    1620   24152  341799   53727 ./mqtt_demo.elf

	echo "app,version,commit_ts,commit,text,data,bss,total,text(iram),rodata,text(irom)" > ${RESULT_FILE_APP}

	while read line
	do
	    A1=`echo ${line} | awk -F' |)' '{print $1}'`
	    #echo ${A1}

	    # If A1 is text, ignore it. It is title
	    if [[ "${A1}" != "text" ]]; then
	        A2=`echo ${line} | awk -F' |)' '{print $2}'`
	        A3=`echo ${line} | awk -F' |)' '{print $3}'`
	        A4=`echo ${line} | awk -F' |)' '{print $4}'`
	        #A5=`echo ${line} | awk -F' |)' '{print $5}'`
	        A6=`echo ${line} | awk -F' |)' '{print $6}'`
	        #echo ++$A1--$A2--$A3--$A4--$A5--$A6__
	    fi
	done < tmp2

	while read line
	do
		B3=`echo ${line} | awk -F' ' '{print $3}'`

		if [[ "${B3}" = ".text" ]]; then
			B7=`echo ${line} | awk -F' ' '{print $7}'`
			TEXT_SIZE=`echo $((0x${B7}))`
		elif [[ "${B3}" = ".rodata" ]]; then
			B7=`echo ${line} | awk -F' ' '{print $7}'`
			RODATA_SIZE=`echo $((0x${B7}))`
		elif [[ "${B3}" = ".irom0.text" ]]; then
			B7=`echo ${line} | awk -F' ' '{print $7}'`
			IROM_SIZE=`echo $((0x${B7}))`
		fi
	done < tmp3

	echo "$A6,$VERSION,$COMMIT_TS,$COMMIT_ID,$A1,$A2,$A3,$A4,$TEXT_SIZE,$RODATA_SIZE,$IROM_SIZE" >> ${RESULT_FILE_APP}
	echo "Result is saved to ${RESULT_FILE_APP}"
}


if [ $# -eq 0 ]; then
    print_usage
    exit -1
fi

cd ${PROJECT_PATH}/build

get_details
get_app
get_map_info

mv ${RESULT_FILE_DETAILS} ${STDK_PATH}
mv ${RESULT_FILE_APP} ${STDK_PATH}
mv ${RESULT_FILE_MAP} ${STDK_PATH}

# remove temp files
rm tmp1 -f
rm tmp2 -f
rm tmp3 -f
