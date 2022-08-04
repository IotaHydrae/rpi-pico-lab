#!/bin/bash

CWD=`pwd`

# set up the environment
IFS='/' read -ra ADDR <<< "$CWD"
if [ "${ADDR[${#ADDR[@]} - 1]}" == "tools" ]; then
        echo "You are in the tools directory. Please run this script from the root directory."
        exit -1
fi

echo "Setting up environment..."
export PICO_SDK_PATH=$CWD/pico-sdk
export PATH=$PATH:$CWD/tools

cd tools
ln -sf daplink-reset.sh daplink-reset
chmod +x daplink-reset

ln -sf daplink-program.sh daplink-program
chmod +x daplink-program
cd -