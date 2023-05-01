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

COPY package/ /usr/local/buildroot/package/
COPY board/ /usr/local/buildroot/board/raspberrypi4/
COPY .config /usr/local/buildroot/.config

# Set the working directory to /usr/local/buildroot
WORKDIR /usr/local/buildroot

