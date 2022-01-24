#!/bin/bash
#export TZ="Europe/Berlin"
#ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
#apt-get update
#apt-get install -y tzdata
#apt-get upgrade -y
#apt-get install build-essential mingw-w64 cmake libsdl2-dev libsdl2-mixer-dev libsdl2-net-dev -y
#apt-get install build-essential mingw-w64 cmake -y

cd /container
make LINUX
