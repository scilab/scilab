#!/bin/sh

ORIGINALLIBNAME=libsciexternal

if test ! -s Makefile.orig; then 
	echo "The compiler detection has been performed. Please launch compilerDetection.sh before."
	exit -1
fi

# Check number of parameters
if test $# -lt 2; then
	echo "Syntax : $0 libname sources"
	exit -2
fi

# retrieve parameters
LIB=$1
shift
SOURCES_TEMP=$*

# Check if files really exist and removes include files
for file in $SOURCES_TEMP; do
	CFILE=`echo $file|sed -e 's|\.o$|\.c|g'`
	FFILE=`echo $file|sed -e 's|\.o$|\.f|g'`
	if [ ! -s $file -a -s $CFILE -a -s $FFILE ]; then 
		echo "Error: Cannot find $file"
		exit -3
	fi
	# It is an include file, do not build it!
	if [ ! `echo $file|grep -i -E "(\.h$|\.hh$|\.hxx$|\.H$)"` ]; then
		SOURCES="$SOURCES $file"
	fi
done

# Replace fake filenames to the one we want 
sed -e "s|libsciexternal_la_SOURCES = foo.c foo2.f foo3.cxx foo4.cpp foo5.f90|lib"$LIB"_la_SOURCES = $SOURCES|g" Makefile.orig > Makefile

## Replace sources by .lo file (manage also when it is at the end of the line
SOURCES=`echo $SOURCES|sed -e 's|\.c |\.lo |g' -e 's|\.c$|\.lo|g'`
SOURCES=`echo $SOURCES|sed -e 's|\.f |\.lo |g' -e 's|\.f$|\.lo|g'`
SOURCES=`echo $SOURCES|sed -e 's|\.cxx |\.lo |g' -e 's|\.cxx$|\.lo|g'`
SOURCES=`echo $SOURCES|sed -e 's|\.f90 |\.lo |g' -e 's|\.f90$|\.lo|g'`
SOURCES=`echo $SOURCES|sed -e 's|\.cpp |\.lo |g' -e 's|\.cpp$|\.lo|g'`

# Makefile.swap is used because there is no inline option with sed under Solaris 

### Changes objects in the source code
sed -e "s|am_libsciexternal_la_OBJECTS = foo.lo foo2.lo foo3.lo foo4.lo foo5.lo|am_lib"$LIB"_la_OBJECTS = $SOURCES|g" Makefile > Makefile.swap
mv  Makefile.swap Makefile

### Changes objects in the source code
sed -e "s|libsciexternal|lib"$LIB"|g" Makefile > Makefile.swap
mv  Makefile.swap Makefile

### Remove the dependencies computed by the system
# No longer used because of  --disable-dependency-tracking when calling 
# ./configure (comment left here : we never know)
#sed -e "s|^include\(.*\)Plo||" Makefile > Makefile.swap
#mv  Makefile.swap Makefile


