#!/bin/bash

in_file=$2
function stop()
{
     send_video_pid=`ps -ef | grep -v "grep" | grep "send_video.sh" | awk '{print $2}'`
     if [ "$send_video_pid" != "" ]
     then
          kill -9 $send_video_pid
          echo $send_video_pid
     fi
     echo "clean up"
}

function start()
{
     trap "stop; exit" SIGHUP SIGINT SIGTERM
     nohup ./send_video.sh $in_file &
}

case $1 in
        start) start;;
        stop) stop;;
esac
exit 0
