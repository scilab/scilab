#!/bin/sh
RM='rm -f'
SCI=..
FILE='one_man.tst'
LOGFILE='get_examples.log'

$RM prov $LOGFILE

echo '//' `date` > prov
echo '' >> prov

do_example()
{
echo '//====================================================' >> prov
echo '//' "$1" >> prov
echo '//====================================================' >> prov

res=`grep 'SH EXAMPLE' $1 2> /dev/null`
if test -n "$res"
then
	echo "clear;lines(0);" >> prov

	sed -e '1,/^.SH EXAMPLE/d' $1 > prov1
	sed -e '1d' prov1 > prov2
	sed -e '/^.fi/,$d' prov2 >> prov
	$RM prov1 prov2
	
	echo "$1" PROCESSED >> $LOGFILE
else
	echo "$1" NO EXAMPLE >> $LOGFILE
fi

echo '' >> prov
}

for j in graphics
do
	echo -n "Processing man/$j "
	for f in $SCI/man/$j/*.man
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
