#!/bin/sh
RM='rm -f'
SCI=..

LOGFILE='get_examples.log'

do_example()
{
res=`grep '<EXAMPLE>' $1 2> /dev/null`
if test -n "$res"
then
	name=`basename $1|sed -e 's/.xml//' `
	out=examples/$name.tst
	newest=`ls -t -1 $1 $out 2>/dev/null |sed -n -e '1p'`
	if [ "$newest" = "$1" ]; then
	    echo "diary('examples/$name.dia')" > $out
	    echo "clear;lines(0);" >> $out
	    sed -e '1,/<EXAMPLE>/d' $1 |sed -e '/<\/EXAMPLE>/,$d'| \
	      sed -e 's/<\!\[CDATA\[//'|sed -e 's/ \]\]>//' >>$out
	    echo "$1" PROCESSED >> $LOGFILE
	    echo "for k=winsid(),xdel(k);end" >> $out
	    echo 'diary(0)' >> $out
	fi
else
	echo "$1" NO EXAMPLE >> $LOGFILE
fi

}

for j in arma control dcd elementary fileio functions graphics gui linear metanet nonlinear polynomials programming robust scicos signal sound strings tdcs translation tksci utilities
do
	echo -n "Processing man/eng/$j "
	for f in $SCI/man/eng/$j/*.xml
	do
		echo -n .
		do_example $f
	done
	echo ""
done

echo ''
echo `grep PROCESSED $LOGFILE|wc -l` examples extracted from `cat  $LOGFILE|wc -l` manual files.


