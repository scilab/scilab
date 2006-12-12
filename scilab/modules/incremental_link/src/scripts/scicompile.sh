#!/bin/sh

ORIGINALLIBNAME=libsciexternal

if test ! -s Makefile.orig; then 
	echo "The compiler detection has been performed. Please launch compilerDetection.sh before"
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
SOURCES=$*

# Check if files exist
for file in $SOURCES; do
	CFILE=`echo $file|sed -s 's|\.o|\.c|'`
	FFILE=`echo $file|sed -s 's|\.o|\.f|'`
	if test ! -s $file -a -s $CFILE -a -s $FFILE; then 
		echo "Cannot find $file"
		exit -3
	fi
done

# Replace fake filenames to the one we want 
sed -s "s|libsciexternal_la_SOURCES = foo.c foo2.f foo3.cxx|lib"$LIB"_la_SOURCES = $SOURCES|g" Makefile.orig > Makefile

## Replace  sources by .lo file
SOURCES=`echo $SOURCES|sed -s 's|\.c|\.lo|g'`
SOURCES=`echo $SOURCES|sed -s 's|\.f|\.lo|g'`
SOURCES=`echo $SOURCES|sed -s 's|\.cxx|\.lo|g'`

### Changes objects in the source code
sed -si "s|am_libsciexternal_la_OBJECTS = foo.lo foo2.lo foo3.lo|am_lib"$LIB"_la_OBJECTS = $SOURCES|g" Makefile

### Changes objects in the source code
sed -si "s|libsciexternal|lib"$LIB"|g" Makefile

### Remove the dependencies computed by the system
sed -si "s|^include[[:space:]]\(.*\)Plo||" Makefile
