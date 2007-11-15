#!/bin/sh
# Copyright INRIA/Scilab 2007
# Author : Sylvestre Ledru <sylvestre.ledru@inria.fr>
# This script goes into a module and updates the localization file by checking
# the _( and gettext( calls in the code

# TODO : 
# * Write Small documentation
# 

if test $# -ne 1; then
	echo "This script goes into a module and updates the localization file "
	echo "by checking the _(xxx) and gettext(xxx) calls in the code"
	echo
	echo "Syntax : $0 <module>"
	echo "If <module> is equal to 'parse_all', it will parse all Scilab module"
	echo "per module"
	exit
fi

if test -z "$SCI"; then
	echo "Please define the variable SCI" 
	exit
fi

MODULES=$1

if test "$MODULES" == "process_all"; then
	echo ".. Process all the modules one by one"
	PROCESS_ALL=1
else
	PROCESS_ALL=0
fi


XGETTEXT=/usr/bin/xgettext
MSGMERGE=/usr/bin/msgmerge
FROM_CODE=ISO-8859-1
EXTENSIONS=( c h cpp hxx java sci start )
TARGETDIR=locales/
LANGS=( fr_FR )
TARGETFILETEMPLATE=messages.pot
TIMEZONE="+0100"

#
# Retrieve all the sources files
FILESCMD='find . -type f '
# Gettext arg
XGETTEXT_OPTIONS="--add-location --strict --keyword=_ --from-code $FROM_CODE --omit-header "
####### GENERATES THE FIND COMMAND
i=0
NB_ELEMENT=${#EXTENSIONS[@]}
while [ "$i" -lt "$NB_ELEMENT" ]; do
	ext=${EXTENSIONS[$i]}
	FILESCMD="$FILESCMD -name '*.$ext'"
	if test "$NB_ELEMENT" -ne `expr $i + 1`; then # because we don't want a trailing -o
		FILESCMD="$FILESCMD -o "
	fi
	i=$((i + 1))
done

# Process all the modules ... then, build the list
if test $PROCESS_ALL -eq 1; then
	cd $SCI/modules/
	MODULES=`find . -maxdepth 1 -type d  ! -name ".*"`
fi


for MODULE in $MODULES; do

	PATHTOPROCESS=$SCI/modules/$MODULE/
	if test ! -d $PATHTOPROCESS; then
		echo "... Cannot find module $PATHTOPROCESS"
		exit
	fi
	echo "... Processing module $MODULE"

	cd $PATHTOPROCESS
	FILES=`eval $FILESCMD|tr "\n" " "`

	echo "..... Parsing all sources in $PATHTOPROCESS"
# Parse all the sources and get the string which should be localized
	LOCALIZATION_FILE_US=$TARGETDIR/en_US/$TARGETFILETEMPLATE
	echo "........ Generate the english localization file by parsing the code"
	$XGETTEXT $XGETTEXT_OPTIONS -p $TARGETDIR/en_US/ -o $TARGETFILETEMPLATE.tmp $FILES > /dev/null
	sed -e "s/MODULE/$MODULE/" -e "s/DATE/`date +'%Y-%m-%d %H:%M'`$TIMEZONE/" $SCI/modules/localization/locales/en_US/header.pot > $LOCALIZATION_FILE_US
	cat $LOCALIZATION_FILE_US.tmp >> $LOCALIZATION_FILE_US
	
	rm $LOCALIZATION_FILE_US.tmp
	if test -z "$NOSTRING"; then
# merge/create the other locales
		for l in $LANGS; do
			DIR_LANG=$TARGETDIR/$l/
			LOCALIZATION_FILE_LANG=$DIR_LANG/messages.po
			if test -f $LOCALIZATION_FILE_LANG; then
				echo "........ Merging new locales for $l"
				$MSGMERGE $LOCALIZATION_FILE_LANG $LOCALIZATION_FILE_US --output-file $LOCALIZATION_FILE_LANG > /dev/null
			else
				echo "........ Localization file for $l in this module not existing"
				echo "........ Creating it ..."
				if test ! -d $DIR_LANG; then
					# Locale dir doesn't exist
					mkdir $DIR_LANG
				fi
				# Copy the current english localization as default
				cp $LOCALIZATION_FILE_US $LOCALIZATION_FILE_LANG
			fi
		done #Browse langs
	fi
	
	cd -
done # Browse modules
