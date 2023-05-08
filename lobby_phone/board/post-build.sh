#!/bin/sh

set -u
set -e

# Create an empty uboot.env file
dd if=/dev/zero of=uboot.env bs=1 count=8192

# Copy the uboot.env file to the images directory
cp uboot.env ${BINARIES_DIR}/

# Add a console on tty1
if [ -e ${TARGET_DIR}/etc/inittab ]; then
    grep -qE '^tty1::' ${TARGET_DIR}/etc/inittab || \
	sed -i '/GENERIC_SERIAL/a\
tty1::respawn:/sbin/getty -L  tty1 0 vt100 # HDMI console' ${TARGET_DIR}/etc/inittab
fi
