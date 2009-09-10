#!/bin/sh

OK="OK"
ERR="ERR"
DIFF="DIFF"

if [[ "$TERM" != "dumb" ]]
then
	Normal=$'\e[0m'
	Red=$'\e[31m'
	Green=$'\e[32m'
	B=$'\e[01m'
	
	OK="$B$Green$OK$Normal"
	ERR="$B$Red$ERR$Normal"
	DIFF="$B$Red$DIFF$Normal"
fi

cd $(dirname $0)

for f in *.sci ; do
	if [[ ! -f $f.ref ]] ; then
		echo -n "Generating reference... "
		../bin/scilab6 -nwni -f $f >$f.ref 2>$f.err && echo $OK || echo $ERR
	else
		echo -n "Testing $f... "
		if ../bin/scilab6 -nwni -f $f >$f.out 2>$f.err ; then
			if diff $f.out $f.ref > /dev/null ; then
				echo $OK
			else
				echo $DIFF
			fi
		else
			echo $ERR
		fi
	fi
done
