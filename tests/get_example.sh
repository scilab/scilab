#!/bin/sh
RM='rm -f'
SCI=..
FILE=$2
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
	echo "for k=winsid(),xdel(k);end" >> prov
else
	echo "$1" NO EXAMPLE >> $LOGFILE
fi
echo '' >> prov

}

echo -n "Processing man/eng/$1 "
for f in $SCI/man/eng/$1/*.xml
do
	echo -n '.'
	do_example $f
done


echo ""

echo ''
echo `grep PROCESSED $LOGFILE|wc -l` examples extracted from `cat  $LOGFILE|wc -l` manual files.

$RM $FILE

sed -e 's/\\\\/\\/' prov > $FILE

$RM prov
