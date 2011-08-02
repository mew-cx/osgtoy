#! /bin/sh
# $Id$
# quick draft of a "no secrets" wrapper around cmake.

ORIG_DIR="${PWD}"
PROJECT_DIR="`dirname $0`"

# edit your configuration preferences here ##################################

SOURCE_DIR="${PROJECT_DIR}"
BUILD_DIR="${SOURCE_DIR}/_BUILD_"
INSTALL_DIR="${PROJECTS}/3rdParty"

CMAKE_BUILD_TYPE="RelWithDebInfo"
#CMAKE_BUILD_TYPE="Debug"
#CMAKE_BUILD_TYPE="Release"
#CMAKE_BUILD_TYPE="MinSizeRel"

VAR_LIST="-L"          # list simple variables
#VAR_LIST="-LA"         # list all variables
#VAR_LIST="-LAH"        # list all variables with help


CMAKE_FLAGS="${VAR_LIST} \
    -DCMAKE_INSTALL_PREFIX:PATH=${INSTALL_DIR} \
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE} \
    -DCMAKE_VERBOSE_MAKEFILE:BOOL=TRUE"


# probably won't need to edit below here ####################################

if [ $# -gt 0 ]
then
    echo "arguments are not accepted; edit \"$0\" to change preferences."
    echo "(the goal is to capture ALL preferences in the script.)"
    exit 1
fi

if [ -z ${PROJECTS} ]
then
    echo "PROJECTS environment variable is not set."
    exit 1
fi

if [ -d ${BUILD_DIR} ]
then
    echo "build directory \"${BUILD_DIR}\" already configured."
    exit 1
fi

# make it so
mkdir -p ${BUILD_DIR}
cd ${SOURCE_DIR}
cmake -H${SOURCE_DIR} -B${BUILD_DIR} ${CMAKE_FLAGS} | tee $0.txt


# vim: set sw=4 ts=8 et ic ai:
