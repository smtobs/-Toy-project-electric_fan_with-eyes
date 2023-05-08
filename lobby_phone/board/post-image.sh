#!/bin/bash

set -e

START_DIR="$(pwd)"
BOARD_DIR="$(dirname $0)"
BOARD_NAME="$(basename ${BOARD_DIR})"
GENIMAGE_CFG="${BOARD_DIR}/genimage-${BOARD_NAME}.cfg"
GENIMAGE_TMP="${BUILD_DIR}/genimage.tmp"

# Pass an empty rootpath. genimage makes a full copy of the given rootpath to
# ${GENIMAGE_TMP}/root so passing TARGET_DIR would be a waste of time and disk
# space. We don't rely on genimage to build the rootfs image, just to insert a
# pre-built one in the disk image.

# Gen U-Boot script
mkimage -A arm -O linux -T script -C none -n "${START_DIR}/board/raspberrypi4/boot.scr" -d "${START_DIR}/board/raspberrypi4/boot.scr" "${BINARIES_DIR}/boot.scr.uimg"

# Copy swu-pdate files
cp ${START_DIR}/board/raspberrypi4/sw-description ${BINARIES_DIR}/
cp ${START_DIR}/board/raspberrypi4/update.sh ${BINARIES_DIR}/
cp ${START_DIR}/board/raspberrypi4/gen_update_img.sh ${BINARIES_DIR}/

cd ${BINARIES_DIR}
source gen_update_img.sh
cd -

trap 'rm -rf "${ROOTPATH_TMP}"' EXIT
ROOTPATH_TMP="$(mktemp -d)"

rm -rf "${GENIMAGE_TMP}"

genimage \
	--rootpath "${ROOTPATH_TMP}"   \
	--tmppath "${GENIMAGE_TMP}"    \
	--inputpath "${BINARIES_DIR}"  \
	--outputpath "${BINARIES_DIR}" \
	--config "${GENIMAGE_CFG}"

exit $?
