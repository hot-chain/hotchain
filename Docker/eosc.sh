#!/bin/bash

# Usage:
# Go into cmd loop: sudo sh hotcc.sh
# Run single cmd:  sudo sh hotcc.sh <hotcc paramers>

PREFIX="docker exec docker_hotc_1 hotcc"
if [ -z $1 ] ; then
  while :
  do
  read  -p "hotcc " cmd
    $PREFIX $cmd
  done
else
  $PREFIX $@
fi
