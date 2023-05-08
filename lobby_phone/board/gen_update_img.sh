#!/bin/bash

# Gen Update Image

set -e
echo "sw-description,rootfs.ext2,update_completed.sh" | tr ',' '\n' | cpio -ov -H crc > update.swu

