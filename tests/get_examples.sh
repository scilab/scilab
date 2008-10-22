#!/bin/sh
RM='rm -f'
SCI=..

if test -n "$1"
then
	Lang=$1
else
	Lang=eng
fi

FILE=examples_$Lang.tst
LOGFILE='get_examples.log'

$RM prov $LOGFILE

echo '//' `date` > prov
echo '' >> prov

do_example()
{
echo '//====================================================' >> prov
echo '//' "$1" >> prov
echo '//====================================================' >> prov


res=`grep '<EXAMPLE>' $1 2> /dev/null`
if test -n "$res"
then
	echo "clear;lines(0);" >> prov
	sed -e '1,/<EXAMPLE>/d' $1 |sed -e '/<\/EXAMPLE>/,$d'| \
	    sed -e 's/<\!\[CDATA\[//'|sed -e 's/ \]\]>//' >>prov
	echo "$1" PROCESSED >> $LOGFILE
	echo "xdel(winsid())" >> prov
else
	echo "$1" NO EXAMPLE >> $LOGFILE
fi
echo '' >> prov

}


for j in arma control dcd elementary fileio functions graphics gui linear metanet nonlinear polynomials programming robust scicos signal sound strings tdcs translation tksci utilities statistics
do
	echo -n "Processing man/$Lang/$j "
	for f in $SCI/man/$Lang/$j/*.xml
	do
		echo -n '.'
		do_example $f
	done
	echo ""
done

echo ''
echo `grep PROCESSED $LOGFILE|wc -l` examples extracted from `cat  $LOGFILE|wc -l` manual files.

$RM $FILE

sed -e 's/\\\\/\\/' prov > $FILE

$RM prov
