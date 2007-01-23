#!/bin/sh

PROGNAME="$0"
PATHTOCONFIGURE=`dirname "$PROGNAME"`
cd $PATHTOCONFIGURE

# Relaunch configure if files are missing
if test ! -s Makefile.orig; then 
	echo "Detection of C/C++/Fortran Compilers"
	./configure --disable-static
	mv Makefile Makefile.orig	
else 
	echo "Detection already done"
fi
