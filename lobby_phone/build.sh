#!/bin/bash

# Build the Docker image
docker build -t my_buildroot .

# Run the Docker container and copy the generated build system
docker run --rm -v "$(pwd)":/output my_buildroot sh -c "cp -r /usr/local/buildroot /output"

cd buildroot && mkdir output && make -j $(nproc)
