#!/bin/bash

echo "update script run"

file_path="/etc/hwrevision"
cmd_set_env="fw_setenv"

#Check file
if [ ! -f "$file_path" ]; then
    echo "Error: $file_path does not exist."
    exit 1
fi

if [ ! -r "$file_path" ]; then
    echo "Error: $file_path is not readable."
    exit 1
fi

#Parsing
read -r line < "$file_path"
board_name=$(echo "$line" | awk '{print $1}')
ver=$(echo "$line" | awk '{print $2}')

if [ -z "$board_name" ]; then
    echo "Error: board name not found in $file_path."
    exit 1
fi


echo "$board_name"
echo "$ver"

#Change partition
if [ "$board_name" = "rpi_boot_b" ]; then
	"$cmd_set_env" rootfs_partition /dev/mmcblk0p4
elif [ "$board_name" = "rpi_boot_a" ]; then
	"$cmd_set_env" rootfs_partition /dev/mmcblk0p3
else
	echo "Invalid board name $board_name"
	exit 1
fi

sudo umount /mnt

echo "update script end"
