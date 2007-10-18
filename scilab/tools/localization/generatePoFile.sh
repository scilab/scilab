#!/bin/sh
# This script compiles the Scilab localization (.mo) from all the modules localization files (.po)
# @author Sylvestre LEDRU <sylvestre.ledru@inria.fr> 
# @date 2007-10-17

LANGUAGES="en_US fr_FR"
MASK="modules/*/locales/"
FILENAME="messages.po"
LC=LC_MESSAGES

if test -z "$SCI"; then
	echo "Please define the variable SCI" 
	exit
fi

cd $SCI

for LOCALE in $LANGUAGES;do
	echo "Mixing $LOCALE locales"
	rm $MASK/$LOCALE/$FILENAME~
	FILES=`ls $MASK/$LOCALE/$FILENAME*`
	for f in $FILES; do
		echo $f
	done
	PATHTO=$SCI/locale/$LOCALE/$LC/
	if test ! -d $PATHTO; then
		mkdir -p $PATHTO
	fi
	POFILE=$PATHTO/scilab.po
	msgcat -o $POFILE $FILES
	msgfmt --statistics -o $PATHTO/scilab.mo $POFILE
done
