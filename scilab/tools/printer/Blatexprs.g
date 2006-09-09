#!/bin/sh 
# Copyright INRIA
if test "$SCI" = ""; then
  SCI="SCILAB_DIRECTORY"
fi
export SCI
$SCI/bin/Slatexprs $*
