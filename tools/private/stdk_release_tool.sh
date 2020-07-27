#!/bin/bash

usage() {
	err_msg "Usage: $0 -v <version>"
	exit 1
}

err_msg() { echo "$@" ;} >&2
err_msg_v() { err_msg "-v option argument required" ;}
err_msg_v_arg() {
	err_msg "-v option argument should be {MAJOR NUMBER}.{MINOR NUMBER}.{PATCH NUMBER}"
	exit 1
}

# Take care of long options
A=""
while true; do
	[ $# -eq 0 ] && break
	case $1 in
		--help)
			usage;
			;;
		--)
			IFS=$(echo -e "\a")
			A=$A$([ -n "$A" ] && echo -e "\a")$*
			break
			;;
		--*)
			err_msg "Invalid option: $1"
			usage;
			;;
	esac

	A=$A$([ -n "$A" ] && echo -e "\a")$1
	shift
done

IFS=$(echo -e "\a"); set -f; set -- $A; set +f; IFS=$(echo -e " \n\t")

# Take care of short options
export TARGET_MAJOR_VER=""
export TARGET_MINOR_VER=""
export TARGET_PATCH_VER=""
export TARGET_VERSION_PROVIDED=0
while getopts ":v:" opt; do
	case $opt in
		v)
			TARGET_MAJOR_VER=`echo $OPTARG | cut -d"." -f1`
			echo $TARGET_MAJOR_VER | grep -E -q '^[0-9]+$' || err_msg_v_arg
			TARGET_MINOR_VER=`echo $OPTARG | cut -d"." -f2`
			echo $TARGET_MINOR_VER | grep -E -q '^[0-9]+$' || err_msg_v_arg
			TARGET_PATCH_VER=`echo $OPTARG | cut -d"." -f3`
			echo $TARGET_PATCH_VER | grep -E -q '^[0-9]+$' || err_msg_v_arg
			TARGET_VERSION_PROVIDED=1
			;;
		:)
			case $OPTARG in
				v)
					err_msg_v
					exit 1
					;;
			esac
			;;
		\?)
			err_msg "Invalid option -$OPTARG"
			usage
			;;
	esac
done

# Prepare Workspace
mkdir release_workspace
if [ $? != 0 ]; then
	read -p "There is release_workspace dir already. Are you sure to erase them?(y/N)" ERASE_WORKSPACE
	if [ "$ERASE_WORKSPACE" = "Y" ] || [ "$ERASE_WORKSPACE" = "y" ]
	then
		rm -rf release_workspace
		mkdir release_workspace
		if [ $? != 0 ]; then
			exit 1
		fi
	else
		exit 1
	fi
fi
cd release_workspace

export WORK_SPACE=`pwd`
export SDK_REF_PATH_PUBLIC="${WORK_SPACE}/stdk_ref_public"
export SDK_REF_PATH_INTERNAL="${WORK_SPACE}/stdk_ref_internal"
export VERSION_HEADER="src/include/st_dev_version.h"
export REMOTE_INTERNAL_REPO_NAME="internal"
export GIT_MERGE_AUTOEDIT=no

# make bashrc file for resolve merge conflict prompt shell
cat /etc/bash.bashrc > ./tmpbashrc
cat ~/.bashrc >> ./tmpbashrc
echo "PS1=\"\[\e]0;\u@\h: \w\a\]${debian_chroot:+($debian_chroot)}\u@\h:\w\$ (MERGE CONFLICT RESOLVING)\"" >> ./tmpbashrc

set -e
if [ ! -d "$SDK_REF_PATH_PUBLIC" ]; then
	git clone https://github.com/SmartThingsCommunity/st-device-sdk-c-ref.git $SDK_REF_PATH_PUBLIC
    cd $SDK_REF_PATH_PUBLIC
	git remote add $REMOTE_INTERNAL_REPO_NAME git@github.sec.samsung.net:STDK/st-device-sdk-c-ref.git
    git submodule init iot-core
	git submodule sync iot-core
	git submodule update iot-core
    cd iot-core
    git remote add $REMOTE_INTERNAL_REPO_NAME git@github.sec.samsung.net:STDK/st-device-sdk-c.git
fi

if [ ! -d "$SDK_REF_PATH_INTERNAL" ]; then
	git clone git@github.sec.samsung.net:STDK/st-device-sdk-c-ref.git $SDK_REF_PATH_INTERNAL
	cd $SDK_REF_PATH_INTERNAL
    git submodule init iot-core
	git submodule sync iot-core
	git submodule update iot-core
fi

# Internal iot-core merge release develop-(merge)->private-(merge)->master
cd $SDK_REF_PATH_INTERNAL
git checkout -f private
git reset --hard origin/private
git submodule sync iot-core/
cd $SDK_REF_PATH_INTERNAL/iot-core
git fetch --prune
git checkout -f develop
git reset --hard origin/develop
git pull
if [ $TARGET_VERSION_PROVIDED == 0 ] ; then
TARGET_MAJOR_VER=`sed -n '/#define VER_MAJOR/p' $VERSION_HEADER | sed 's/[^0-9]*//g'`
TARGET_MINOR_VER=`sed -n '/#define VER_MINOR/p' $VERSION_HEADER | sed 's/[^0-9]*//g'`
TARGET_PATCH_VER=`sed -n '/#define VER_PATCH/p' $VERSION_HEADER | sed 's/[^0-9]*//g'`
((TARGET_PATCH_VER=TARGET_PATCH_VER+1))
fi
sed -i '/#define VER_MAJOR/c\#define VER_MAJOR\t'${TARGET_MAJOR_VER}'' $VERSION_HEADER
sed -i '/#define VER_MINOR/c\#define VER_MINOR\t'${TARGET_MINOR_VER}'' $VERSION_HEADER
sed -i '/#define VER_PATCH/c\#define VER_PATCH\t'${TARGET_PATCH_VER}'' $VERSION_HEADER
git add $VERSION_HEADER
git commit -s -m "Update core version to $TARGET_MAJOR_VER.$TARGET_MINOR_VER.$TARGET_PATCH_VER"
git checkout -B develop_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER}
set +e
git push origin develop_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER}
if [ $? != 0 ]; then
	set -e
	echo "There is problem pushing core internal develop_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER} branch"
	read -p "Do you want overwrite it in force?(y/N)" PUSH_IN_FORCE
	if [ "$PUSH_IN_FORCE" = "Y" ] || [ "$PUSH_IN_FORCE" = "y" ]
	then
		git push --force origin develop_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER}
	else
		exit 1
	fi
fi
set -e
git checkout -f private
git reset --hard origin/private
git pull
set +e
git merge --log --commit develop_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER}
while [ $? != 0 ];
do
	set -e
	echo
	echo "               MERGE CONFLICT(sdk-core)                  "
	echo "      (Public)                         {Internal)        "
	echo "  master    develop          develop    private    master"
	echo "    |         |                 |          |          |  "
	echo "    |         |                 | ------>  |          |  "
	echo "    |         |                 |          |          |  "
	echo "You can resolve it manually.(hint : find out conflicts with \"git diff\", and \"git add\" after edit. then \"exit\")"
	read -p "Do you want resolve it?(Y/n)" TRY_MERGE_RESOLVE
	if [ "$TRY_MERGE_RESOLVE" = "Y" ] || [ "$TRY_MERGE_RESOLVE" = "y" ] || [ "$TRY_MERGE_RESOLVE" = "" ]
	then
		bash --rcfile ${WORK_SPACE}/tmpbashrc
		set +e
		git merge --continue
	else
		exit 1
	fi
done
set -e
git checkout -B private_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER}
set +e
git push origin private_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER}
if [ $? != 0 ]; then
	set -e
	echo "There is problem pushing core internal private_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER} branch"
	read -p "Do you want overwrite it in force?(y/N)" PUSH_IN_FORCE
	if [ "$PUSH_IN_FORCE" = "Y" ] || [ "$PUSH_IN_FORCE" = "y" ]
	then
		git push --force origin private_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER}
	else
		exit 1
	fi
fi
set -e
git checkout -f master
git reset --hard origin/master
git pull
set +e
git merge --log --commit private_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER}
while [ $? != 0 ];
do
	set -e
	echo
	echo "               MERGE CONFLICT(sdk-core)                  "
	echo "      (Public)                         {Internal)        "
	echo "  master    develop          develop    private    master"
	echo "    |         |                 |          |          |  "
	echo "    |         |                 |          | ------>  |  "
	echo "    |         |                 |          |          |  "
	echo "You can resolve it manually.(hint : find out conflicts with \"git diff\", and \"git add\" after edit. then \"exit\")"
	read -p "Do you want resolve it?(Y/n)" TRY_MERGE_RESOLVE
	if [ "$TRY_MERGE_RESOLVE" = "Y" ] || [ "$TRY_MERGE_RESOLVE" = "y" ] || [ "$TRY_MERGE_RESOLVE" = "" ]
	then
		bash --rcfile ${WORK_SPACE}/tmpbashrc
		set +e
		git merge --continue
	else
		exit 1
	fi
done
set -e
git checkout -B master_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER}
set +e
git push origin master_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER}
if [ $? != 0 ]; then
	set -e
	echo "There is problem pushing core internal master_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER} branch"
	read -p "Do you want overwrite it in force?(y/N)" PUSH_IN_FORCE
	if [ "$PUSH_IN_FORCE" = "Y" ] || [ "$PUSH_IN_FORCE" = "y" ]
	then
		git push --force origin master_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER}
	else
		exit 1
	fi
fi
set -e

# Public iot-core merge release develop(internal)-(merge)->develop(public)
cd $SDK_REF_PATH_PUBLIC/iot-core
git fetch $REMOTE_INTERNAL_REPO_NAME --prune
git checkout -B develop
git reset --hard origin/develop
git pull origin develop
set +e
git merge --log --commit $REMOTE_INTERNAL_REPO_NAME/develop_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER}
while [ $? != 0 ];
do
	set -e
	echo
	echo "               MERGE CONFLICT(sdk-core)                  "
	echo "      (Public)                         {Internal)        "
	echo "  master    develop          develop    private    master"
	echo "    |         |                 |          |          |  "
	echo "    |         |  <------------  |          |          |  "
	echo "    |         |                 |          |          |  "
	echo "You can resolve it manually.(hint : find out conflicts with \"git diff\", and \"git add\" after edit. then \"exit\")"
	read -p "Do you want resolve it?(Y/n)" TRY_MERGE_RESOLVE
	if [ "$TRY_MERGE_RESOLVE" = "Y" ] || [ "$TRY_MERGE_RESOLVE" = "y" ] || [ "$TRY_MERGE_RESOLVE" = "" ]
	then
		bash --rcfile ${WORK_SPACE}/tmpbashrc
		set +e
		git merge --continue
	else
		exit 1
	fi
done
set -e
git checkout -B develop_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER}
git config credential.helper store
set +e
git push origin develop_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER}
if [ $? != 0 ]; then
	set -e
	echo "There is problem pushing core public develop_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER} branch"
	read -p "Do you want overwrite it in force?(y/N)" PUSH_IN_FORCE
	if [ "$PUSH_IN_FORCE" = "Y" ] || [ "$PUSH_IN_FORCE" = "y" ]
	then
		git push --force origin develop_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER}
	else
		exit 1
	fi
fi
set -e

# Internal ref merge release develop-(merge)->private-(merge)->master
cd $SDK_REF_PATH_INTERNAL
git checkout -f develop
git reset --hard origin/develop
git pull
git submodule sync iot-core/
cd $SDK_REF_PATH_INTERNAL/iot-core
git fetch --prune
git reset --hard origin/develop_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER}
cd $SDK_REF_PATH_INTERNAL
git add iot-core
git commit -s -m "Sync-up with iot-core $TARGET_MAJOR_VER.$TARGET_MINOR_VER.$TARGET_PATCH_VER"
git checkout -B develop_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER}
set +e
git push origin develop_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER}
if [ $? != 0 ]; then
	set -e
	echo "There is problem pushing ref internal develop_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER} branch"
	read -p "Do you want overwrite it in force?(y/N)" PUSH_IN_FORCE
	if [ "$PUSH_IN_FORCE" = "Y" ] || [ "$PUSH_IN_FORCE" = "y" ]
	then
		git push --force origin develop_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER}
	else
		exit 1
	fi
fi
set -e
git checkout -f private
git reset --hard origin/private
git pull
git submodule sync iot-core/
cd $SDK_REF_PATH_INTERNAL/iot-core
git fetch --prune
git reset --hard origin/private_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER}
cd $SDK_REF_PATH_INTERNAL
set +e
git merge --log --commit develop_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER}
if [ $? == 0 ]; then
	echo "There sholud be iot-core merge conflict when merging from ref devleop to private"
	exit 1
fi
git add iot-core
git diff --diff-filter=U --quiet
DIFF_RESULT=$?
if [ $DIFF_RESULT != 0 ]; then
	while [ $DIFF_RESULT != 0 ];
	do
		set -e
		echo
		echo "               MERGE CONFLICT(sdk-ref)                   "
		echo "      (Public)                         {Internal)        "
		echo "  master    develop          develop    private    master"
		echo "    |         |                 |          |          |  "
		echo "    |         |                 | -------> |          |  "
		echo "    |         |                 |          |          |  "
		echo "You can resolve it manually.(hint : find out conflicts with \"git diff\", and \"git add\" after edit. then \"exit\")"
		read -p "Do you want resolve it?(Y/n)" TRY_MERGE_RESOLVE
		if [ "$TRY_MERGE_RESOLVE" = "Y" ] || [ "$TRY_MERGE_RESOLVE" = "y" ] || [ "$TRY_MERGE_RESOLVE" = "" ]
		then
			bash --rcfile ${WORK_SPACE}/tmpbashrc
			set +e
			git merge --continue
			DIFF_RESULT=$?
		else
			exit 1
		fi
	done
else
	git commit --no-edit
fi
set -e
echo "${TARGET_MAJOR_VER}.${TARGET_MINOR_VER}.${TARGET_PATCH_VER}" > VERSION
git add VERSION
git commit -s -m "Update VERSION to ${TARGET_MAJOR_VER}.${TARGET_MINOR_VER}.${TARGET_PATCH_VER}"
git checkout -B private_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER}
set +e
git push origin private_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER}
if [ $? != 0 ]; then
	set -e
	echo "There is problem pushing ref internal private_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER} branch"
	read -p "Do you want overwrite it in force?(y/N)" PUSH_IN_FORCE
	if [ "$PUSH_IN_FORCE" = "Y" ] || [ "$PUSH_IN_FORCE" = "y" ]
	then
		git push --force origin private_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER}
	else
		exit 1
	fi
fi
set -e
git checkout -f master
git reset --hard origin/master
git pull
set +e
git merge --log --commit private_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER}
while [ $? != 0 ];
do
	set -e
	echo
	echo "               MERGE CONFLICT(sdk-ref)                   "
	echo "      (Public)                         {Internal)        "
	echo "  master    develop          develop    private    master"
	echo "    |         |                 |          |          |  "
	echo "    |         |                 |          | -------> |  "
	echo "    |         |                 |          |          |  "
	echo "You can resolve it manually.(hint : find out conflicts with \"git diff\", and \"git add\" after edit. then \"exit\")"
	read -p "Do you want resolve it?(Y/n)" TRY_MERGE_RESOLVE
	if [ "$TRY_MERGE_RESOLVE" = "Y" ] || [ "$TRY_MERGE_RESOLVE" = "y" ] || [ "$TRY_MERGE_RESOLVE" = "" ]
	then
		bash --rcfile ${WORK_SPACE}/tmpbashrc
		set +e
		git merge --continue
	else
		exit 1
	fi
done
set -e
git checkout -B master_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER}
set +e
git push origin master_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER}
if [ $? != 0 ]; then
	set -e
	echo "There is problem pushing ref internal master_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER} branch"
	read -p "Do you want overwrite it in force?(y/N)" PUSH_IN_FORCE
	if [ "$PUSH_IN_FORCE" = "Y" ] || [ "$PUSH_IN_FORCE" = "y" ]
	then
		git push --force origin master_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER}
	else
		exit 1
	fi
fi
set -e

# Public ref merge release develop(internal)-(merge)->develop(public)
cd $SDK_REF_PATH_PUBLIC
git fetch $REMOTE_INTERNAL_REPO_NAME --prune
git checkout -B develop
git reset --hard origin/develop
git pull origin develop
set +e
git merge --log --commit $REMOTE_INTERNAL_REPO_NAME/develop_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER}
while [ $? != 0 ];
do
	set -e
	echo
	echo "               MERGE CONFLICT(sdk-ref)                   "
	echo "      (Public)                         {Internal)        "
	echo "  master    develop          develop    private    master"
	echo "    |         |                 |          |          |  "
	echo "    |         | <------------   |          |          |  "
	echo "    |         |                 |          |          |  "
	echo "You can resolve it manually.(hint : find out conflicts with \"git diff\", and \"git add\" after edit. then \"exit\")"
	read -p "Do you want resolve it?(Y/n)" TRY_MERGE_RESOLVE
	if [ "$TRY_MERGE_RESOLVE" = "Y" ] || [ "$TRY_MERGE_RESOLVE" = "y" ] || [ "$TRY_MERGE_RESOLVE" = "" ]
	then
		bash --rcfile ${WORK_SPACE}/tmpbashrc
		set +e
		git merge --continue
	else
		exit 1
	fi
done
set -e
git checkout -B develop_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER}
git config credential.helper store
set +e
git push origin develop_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER}
if [ $? != 0 ]; then
	set -e
	echo "There is problem pushing ref public develop_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER} branch"
	read -p "Do you want overwrite it in force?(y/N)" PUSH_IN_FORCE
	if [ "$PUSH_IN_FORCE" = "Y" ] || [ "$PUSH_IN_FORCE" = "y" ]
	then
		git push --force origin develop_rel_${TARGET_MAJOR_VER}_${TARGET_MINOR_VER}_${TARGET_PATCH_VER}
	else
		exit 1
	fi
fi
set -e

echo "SDK RELEASE SUCCESS!!"
