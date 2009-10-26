#!/bin/bash
# Copyright INRIA/Scilab 2008
# Author : Sylvestre Ledru <sylvestre.ledru@inria.fr>
# This script takes a list of localization files dumped by launchpad
# here https://translations.launchpad.net/scilab/trunk/+export 
# and dispatch them into Scilab source tree

if test $# -ne 1; then
	echo "This script takes a list of localization files dumped by launchpad"
	echo "here https://translations.launchpad.net/scilab/trunk/+export "
	echo "and dispatch them into Scilab source tree"
	echo ""
	echo "Syntax : $0 <path_to_launchpad_localization_file"
	exit -1
fi

if test -z "$SCI"; then
        echo "Please define the variable SCI" 
        exit -2
fi

LAUNCHPAD_DIRECTORY=$1
if test ! -d $LAUNCHPAD_DIRECTORY; then
	echo "Could not find $LAUNCHPAD_DIRECTORY."
	echo "Exiting..."
	exit -3
fi

# Workaround because launchpad does not allow to delete a directory
rm $LAUNCHPAD_DIRECTORY/string-v2*.po 

# Don't know why but launchpad is placing some files in some sub dirs
cp -i $LAUNCHPAD_DIRECTORY/*/*.po $LAUNCHPAD_DIRECTORY/

for file in $LAUNCHPAD_DIRECTORY/*.po; do 
	file=`echo $file|awk -F / '{print $NF}'` # get only the filename
	LOC=`echo $file|cut -d. -f1|awk -F - '{print $NF}'` # Get the locale (fr_FR, en_US ...)

	if test "$LOC" == "fr"; then LOC="fr_FR"; fi # Retrieve real format
	if test "$LOC" == "ru"; then LOC="ru_RU"; fi
	if test "$LOC" == "de"; then LOC="de_DE"; fi
	if test "$LOC" == "ca"; then LOC="ca_ES"; fi
	if test "$LOC" == "es"; then LOC="es_ES"; fi
	if test "$LOC" == "ja"; then LOC="ja_JP"; fi

	# check that it is the right format
	echo "$LOC"|grep -E "(.*_.*)" > /dev/null # it is a real localization name xx_YY

	if test $? -eq 0; then
		MODULE=`echo $file|sed -e "s|\(.*\)-.*|\1|"|sed -e "s|-|_|g"` # Get the module name (for example signal_processing)

		DIR=$SCI/modules/$MODULE/locales/$LOC/
		if test ! -d $DIR; then
			mkdir $DIR
		fi
		echo "cp $LAUNCHPAD_DIRECTORY/$file $DIR/$MODULE.po"
		cp $LAUNCHPAD_DIRECTORY/$file $DIR/$MODULE.po
		if test $? -ne 0; then
			echo "Error detected in the copy"
			exit 1;
		fi
	else
		echo "Ignore locale $LOC"
	fi
done
