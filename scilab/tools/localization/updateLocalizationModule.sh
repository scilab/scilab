#!/bin/bash
# Copyright INRIA/Scilab 2007/2008
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
	echo "If <module> is equal to 'process_all', it will parse all Scilab module"
	echo "per module"
	exit -1
fi

if test -z "$SCI"; then
	echo "Please define the variable SCI" 
	exit -1
fi

MODULES=$1

if test "$MODULES" = "process_all"; then
	echo ".. Process all the modules one by one"
	PROCESS_ALL=1
else
	PROCESS_ALL=0
fi


XGETTEXT=/usr/bin/xgettext
MSGMERGE=/usr/bin/msgmerge
FROM_CODE=ISO-8859-1
EXTENSIONS=( c h cpp hxx java sci start quit )
TARGETDIR=locales/
LANGS=( fr_FR )
HEADER_TEMPLATE=$SCI/modules/localization/locales/en_US/header.pot
GUI_FILES="etc/*.xml"
FAKE_C_FILE=scilab_fake_localization_file.c
TIMEZONE="+0100"

process_XML_files(){
# First expression => remove line which does NOT contain label
# Second expression =>  extract the content of the label and switch it to a gettext fake instruction
# Third expression => remove empty lines
# Please note that it will only extract string from the label tag
	COMMON_SED='s/&amp;/\&/g'
	sed  -e '/label/!s/.*//'  -e 's/.*label="\([^"]*\)".*/gettext("\1")/' -e '/^$/d' -e $COMMON_SED $GUI_FILES > $FAKE_C_FILE
	sed  -e '/tooltiptext/!s/.*//'  -e 's/.*tooltiptext="\([^"]*\)".*/gettext("\1")/' -e '/^$/d' -e $COMMON_SED $GUI_FILES >> $FAKE_C_FILE
}

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
	MODULES=`find . -maxdepth 1 -type d  ! -name ".*" ! -name 'javasci'`
fi


for MODULE in $MODULES; do

	PATHTOPROCESS=$SCI/modules/$MODULE/
	if test ! -d $PATHTOPROCESS; then
		echo "... Cannot find module $PATHTOPROCESS"
		exit
	fi
	echo "... Processing module $MODULE"

	cd $PATHTOPROCESS
# Extract label from xml files
	process_XML_files
	FILES=`eval $FILESCMD|tr "\n" " "`
	if test "$MODULE" = "core"; then
		# We want some strings from the ROOTDIR when it is the core module
		FILES="$FILES `ls $SCI/etc/scilab.*`"
	fi

	# Also extract string straight from the XML because we have some gettext calls in it
	FILES="$FILES `ls etc/*.xml`"
	MODULE_NAME=`echo $MODULE|sed -e 's|./||'` # avoid to have ./module_name

	echo "..... Parsing all sources in $PATHTOPROCESS"
# Parse all the sources and get the string which should be localized
	LOCALIZATION_FILE_US=$TARGETDIR/en_US/$MODULE_NAME.pot

	if test -f $LOCALIZATION_FILE_US; then
		# Localization file already existing. Retrieve POT-Creation-Date
		CreationDate=`grep POT-Creation-Date: $LOCALIZATION_FILE_US|sed -e 's|\"POT-Creation-Date: \(.*\)\\\n\"|\1|'`
	fi

	echo "........ Generate the english localization file by parsing the code"
	$XGETTEXT $XGETTEXT_OPTIONS -p $TARGETDIR/en_US/ -o $MODULE_NAME.pot.tmp $FILES > /dev/null
	if test  -z "$CreationDate"; then
		# File not existing before ... Set the current date a POT-Creation-Date
		sed -e "s/MODULE/$MODULE_NAME/" -e "s/CREATION-DATE/`date +'%Y-%m-%d %H:%M'`$TIMEZONE/" -e "s/REVISION-DATE/`date +'%Y-%m-%d %H:%M'`$TIMEZONE/" $HEADER_TEMPLATE > $LOCALIZATION_FILE_US
	else
		sed -e "s/MODULE/$MODULE_NAME/" -e "s/CREATION-DATE/$CreationDate/" -e "s/REVISION-DATE/`date +'%Y-%m-%d %H:%M'`$TIMEZONE/" $HEADER_TEMPLATE > $LOCALIZATION_FILE_US
	fi
	cat $LOCALIZATION_FILE_US.tmp >> $LOCALIZATION_FILE_US
	rm $LOCALIZATION_FILE_US.tmp
	if test -z "$NOSTRING"; then
# merge/create the other locales
		for l in $LANGS; do
			DIR_LANG=$TARGETDIR/$l/
			LOCALIZATION_FILE_LANG=$DIR_LANG/$MODULE_NAME.po
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
	# Remove fake file used to extract string from XML
	rm $FAKE_C_FILE
	cd $SCI/
done # Browse modules
