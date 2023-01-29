#!/bin/bash

#ffmpeg -re -i /dev/video0 -c copy -f rtsp -rtsp_transport tcp rtsp://localhost:8888/live.sdp
ffmpeg -re -i $1 -c copy -f rtsp -rtsp_transport tcp rtsp://localhost:8888/live.sdp


