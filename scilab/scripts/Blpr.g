#!/bin/sh 
# Copyright INRIA
if test "$SCI" = ""; then
  SCI="SCILAB_DIRECTORY"
fi
export SCI
ARG1=$1
shift
$SCI/bin/Slpr "$ARG1" $*
