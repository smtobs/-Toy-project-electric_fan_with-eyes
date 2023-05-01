FROM ubuntu:20.04

ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=Asia/Seoul

# Set DL_DIR environment variable
#ENV DL_DIR /usr/local/buildroot/dl

RUN apt-get update && apt-get install -y \
    tzdata \
    build-essential \
    libncurses5-dev \
    unzip \
    python3 \
    python3-pip \
    rsync \
    bc \
    wget \
    git \
    libssl-dev \
    libelf-dev \
    libdw-dev \
    libiberty-dev \
    pkg-config \
    gettext \
    file \
    sudo \
    cpio \
    make

RUN pip3 install setuptools

# Download and extract Buildroota

RUN cd /usr/local && \
    wget https://buildroot.org/downloads/buildroot-2023.02.tar.gz && \
    #tar -xvf buildroot-2021.02.1.tar.gz && \
    tar -xvf buildroot-2023.02.tar.gz && \
    rm buildroot-2023.02.tar.gz && \
    mv buildroot-2023.02 buildroot

#COPY board/config.txt /usr/local/buildroot/board/raspberrypi4/config_4.txt
#COPY board/bcm2711-rpi-4-b.dts /usr/local/buildroot/board/raspberrypi4/bcm2711-rpi-4-b.dts
#COPY board/boot.scr /usr/local/buildroot/board/raspberrypi4/boot.scr
#COPY board/u-boot.config /usr/local/buildroot/board/raspberrypi4/u-boot.config
#COPY board/genimage-raspberrypi4.cfg /usr/local/buildroot/board/raspberrypi4/genimage-raspberrypi4.cfg
#COPY board/post-image.sh /usr/local/buildroot/board/raspberrypi4/post-image.sh

#COPY package/Config.in /usr/local/buildroot/package/Config.in
#COPY package/my_application /usr/local/buildroot/package/my_application
#COPY package/my_drivers /usr/local/buildroot/package/my_drivers
#COPY package/busybox/inittab /usr/local/buildroot/package/busybox/inittab
#COPY package/wpa_supplicant/wpa_supplicant.conf /usr/local/buildroot/package/wpa_supplicant/wpa_supplicant.conf
#COPY package/ifupdown-scripts/ifupdown-scripts.mk /usr/local/buildroot/package/ifupdown-scripts/ifupdown-scripts.mk
#COPY .config /usr/local/buildroot/.config

COPY package/ /usr/local/buildroot/package/
COPY board/ /usr/local/buildroot/board/raspberrypi4/
COPY .config /usr/local/buildroot/.config

# Set the working directory to /usr/local/buildroot
WORKDIR /usr/local/buildroot

