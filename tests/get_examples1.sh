#!/bin/sh
RM='rm -f'
SCI=..
FILE='examples.tst'
LOGFILE='get_examples.log'

$RM prov $LOGFILE  examples/examples.tst

echo '//' `date` > prov 
echo '' >> prov
echo '' >> examples.tst
do_example()
{
echo '//====================================================' >> prov
echo '//' "$1" >> prov
echo '//====================================================' >> prov

res=`grep 'SH EXAMPLE' $1 2> /dev/null`
if test -n "$res"
then
	name=`basename $1`
	out=examples/$name.tst

	newest=`ls -t -1 $1 $out 2>/dev/null |sed -n -e '1p'`
	if [ "$newest" = "$1" ]; then
	    echo "diary('examples/$name.dia')" > $out
	    echo "clear;lines(0);" >> $out
	    sed -e '1,/^.SH EXAMPLE/d' $1 > prov1
	    sed -e '1d' prov1 > prov2
	    sed -e '/^.fi/,$d' prov2 >> $out
	    $RM prov1 prov2
	
	    echo "$1" PROCESSED >> $LOGFILE
	    echo "for k=winsid(),xdel(k);end" >> $out
	    echo 'diary(0)' >> $out
	fi
else
	echo "$1" NO EXAMPLE >> $LOGFILE
fi

}

for j in arma comm control dcd elementary fileio functions graphics gui linear metanet nonlinear polynomials programming robust scicos signal sound strings tdcs translation tksci utilities
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
