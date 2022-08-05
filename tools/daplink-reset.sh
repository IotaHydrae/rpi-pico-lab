#!/bin/bash

INTERFACE=cmsis-dap.cfg
TARGET=rp2040.cfg
ADAPTER_SPEED=100000

if [ `which lolcat` ]; then
    echo "Found lolcat" | lolcat
    TO_LOLCAT=lolcat
else
    echo "lolcat not found, installing..."
    sudo apt install -y lolcat
    if [ $? -ne 0 ]; then
        echo "Failed to install lolcat, will not use it."
        TO_LOLCAT="tee"
    fi
fi

# If openocd is not found, we don't need go further.
if [ ${#OPENOCD_DIR} -ne 0 ]; then
    echo "Setting openocd to $OPENOCD_DIR/src/openocd" | ${TO_LOLCAT}
    OPENOCD_BIN=$OPENOCD_DIR/src/openocd
else
    # Use system default openocd.
    echo "Will use system default openocd." | ${TO_LOLCAT}
    OPENOCD_BIN=openocd
fi

# Call the openocd script to reset the target.
if [ ${#OPENOCD_DIR} -ne 0 ]; then
sudo ${OPENOCD_BIN} -s ${OPENOCD_DIR}/tcl \
    -f ${OPENOCD_DIR}/tcl/interface/${INTERFACE} \
    -f ${OPENOCD_DIR}/tcl/target/${TARGET} -c \
"
    adapter speed ${ADAPTER_SPEED};
    init;
    reset;
    shutdown;
"
else
sudo ${OPENOCD_BIN} \
    -f interface/${INTERFACE} \
    -f target/${TARGET} -c \
"
    adapter speed ${ADAPTER_SPEED};
    init;
    reset;
    shutdown;
"
fi

echo "Done." | ${TO_LOLCAT}
