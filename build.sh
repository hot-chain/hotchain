#!/usr/bin/env bash

##########################################################################
# This is HOTC bootstrapper script for Linux and OS X.
# This file was downloaded from https://github.com/HOTCIO/hotc
# Feel free to change this file to fit your needs.
##########################################################################

VERSION=1.0

# Define directories.
WORK_DIR=$PWD
BUILD_DIR=${WORK_DIR}/build
TEMP_DIR=/tmp
BINARYEN_BIN=/opt/binaryen/bin/
OPENSSL_ROOT_DIR=/usr/local/opt/openssl
OPENSSL_LIBRARIES=/usr/local/opt/openssl/lib

# Target architectures
ARCH=$1
TARGET_ARCHS="ubuntu darwin"

# Check ARCH
if [[ $# > 1 ]]; then
  echo ""
  echo "Error: too many arguments"
  exit 1
fi

if [[ $# < 1 ]]; then
  echo ""
  echo "Usage: bash build.sh TARGET"
  echo ""
  echo "Targets: $TARGET_ARCHS"
  exit 1
fi

if [[ $ARCH =~ [[:space:]] || ! $TARGET_ARCHS =~ (^|[[:space:]])$ARCH([[:space:]]|$) ]]; then
  echo ""
  echo ">>> WRONG ARCHITECTURE \"$ARCH\""
  exit 1
fi

echo ""
echo ">>> ARCHITECTURE \"$ARCH\""

# Debug flags
INSTALL_DEPS=1
COMPILE_HOTC=1
COMPILE_CONTRACTS=1

# Define default arguments.
CMAKE_BUILD_TYPE=Debug

# Install dependencies
if [ ${INSTALL_DEPS} == "1" ]; then

  print ">> Install dependencies"
  . ${WORK_DIR}/scripts/install_dependencies.sh

fi

# Create the build dir
cd ${WORK_DIR}
mkdir -p ${BUILD_DIR}
cd ${WORK_DIR}/${BUILD_DIR}

# Build HOTC
cmake -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DBINARYEN_BIN=${BINARYEN_BIN} -DOPENSSL_ROOT_DIR=${OPENSSL_ROOT_DIR} -DOPENSSL_LIBRARIES=${OPENSSL_LIBRARIES} ..
make -j4