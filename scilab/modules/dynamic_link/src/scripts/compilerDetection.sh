#!/bin/sh

PROGNAME="$0"
PATHTOCONFIGURE=`dirname "$PROGNAME"`
cd $PATHTOCONFIGURE

# Relaunch configure if files are missing
if test ! -s Makefile.orig -o ! -s libtool; then 
	echo "Detection of C/C++/Fortran Compilers"
	./configure --disable-static --disable-dependency-tracking "$@"
	mv Makefile Makefile.orig
else 
	echo "Detection of compilers already done"
fi
