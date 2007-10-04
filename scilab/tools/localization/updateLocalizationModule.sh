#!/bin/sh
# This script goes into a module and updates the localization file by checking
# the _( and gettext( calls in the code

if test $# -ne 1; then
	echo "Syntax : $0 <module>"
	exit
fi

if test -z "$SCI"; then
	echo "Please define the variable SCI" 
	exit
fi

MODULE=$1

XGETTEXT=/usr/bin/xgettext
MSGMERGE=/usr/bin/msgmerge
FROM_CODE=ISO-8859-1
EXTENSIONS=( c h cpp hxx java sci )
TARGETDIR=locales/en_US
TARGETFILETMP=messages.po.temp
#
# Retrieve all the sources files
FILES='find . -type f '

####### GENERATES THE FIND COMMAND
i=0
NB_ELEMENT=${#EXTENSIONS[@]}
while [ "$i" -lt "$NB_ELEMENT" ]; do
	ext=${EXTENSIONS[$i]}
	FILES="$FILES -name '*.$ext'"
	if test "$NB_ELEMENT" -ne `expr $i + 1`; then # because we don't want a trailing -o
		FILES="$FILES -o "
	fi
	i=$((i + 1))
done
PATHTOPROCESS=$SCI/modules/$MODULE/
cd $PATHTOPROCESS
FILES=`eval $FILES`

echo "Parsing all sources in $PATHTOPROCESS"
# Parse all the sources and get the string which should be localized
$XGETTEXT --add-location --strict --keyword="_" --from-code $FROM_CODE -p $TARGETDIR -o $TARGETFILETMP $FILES

# Defines the CHARSET
sed -i -e 's|^"Content-Type: text/plain; charset=CHARSET\\n"$|"Content-Type: text/plain; charset=ISO-8859-1\\n"|' $TARGETDIR/$TARGETFILETMP

# Merge the new locale file and the template
$MSGMERGE $TARGETDIR/messages.pot $TARGETDIR/$TARGETFILETMP  --output-file $TARGETDIR/messages.pot
cd -
