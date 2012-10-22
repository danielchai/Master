#!/bin/bash
keyword="master"

running=$(ps aux | grep $keyword | wc -l)

if [ $running -gt 0 ];
then

  runningPIDs=$(ps aux | grep $keyword | awk '{print $2}')

  # Count the PIDs
  a=($runningPIDs)

  # Need to subtract two because dont want to kill
  # the new process nor the above grep
  PIDCount=$((${#a[@]} - 2))

  if [ $PIDCount -gt 0 ];
  then

    # Need to start this at *1* rather than *0*
    # so we do not knock out master profilego
    for ((i=1; i<${PIDCount}; i++));
    do
      kill -9 ${a[$i]}
    done

  fi

fi

