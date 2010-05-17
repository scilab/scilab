#!/bin/sh
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) INRIA - 2007-2008 - Sylvestre Ledru
# Copyright (C) DIGITEO - 2009-2010 - Sylvestre Ledru
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
#
# This script takes a list of localization files dumped by launchpad
# here https://translations.launchpad.net/scilab/trunk/+export 
# and dispatch them into Scilab source tree

if test $# -ne 1; then
    echo "This script takes a list of localization files dumped by launchpad"
    echo "here https://translations.launchpad.net/scilab/trunk/+export "
    echo "and dispatch them into Scilab source tree"
    echo ""
    echo "Syntax : $0 <path_to_launchpad_localization_file>"
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

# Don't know why but launchpad is placing some files in some sub dirs
/bin/cp -fi $LAUNCHPAD_DIRECTORY/*/*.po $LAUNCHPAD_DIRECTORY/

for file in $LAUNCHPAD_DIRECTORY/*.po; do 
    file=`echo $file|awk -F / '{print $NF}'` # get only the filename
    LOC=`echo $file|cut -d. -f1|awk -F - '{print $NF}'` # Get the locale (fr_FR, en_US ...)

    if test "$LOC" == "fr"; then LOC="fr_FR"; fi # Retrieve real format
    if test "$LOC" == "ru"; then LOC="ru_RU"; fi
    if test "$LOC" == "de"; then LOC="de_DE"; fi
    if test "$LOC" == "ca"; then LOC="ca_ES"; fi
    if test "$LOC" == "es"; then LOC="es_ES"; fi
    if test "$LOC" == "ja"; then LOC="ja_JP"; fi
    if test "$LOC" == "it"; then LOC="it_IT"; fi
    if test "$LOC" == "uk"; then LOC="uk_UA"; fi
    if test "$LOC" == "pl"; then LOC="pl_PL"; fi

    # check that it is the right format
    echo "$LOC"|grep -E "(.*_.*)" > /dev/null # it is a real localization name xx_YY

    if test $? -eq 0; then
        MODULE=`echo $file|sed -e "s|\(.*\)-.*|\1|"|sed -e "s|-|_|g"` # Get the module name (for example signal_processing)

        DIR=$SCI/modules/$MODULE/locales/$LOC/
        if test ! -d $DIR; then
            mkdir $DIR
        fi

        # Before the copy, strip the line with the date. It is only making
        # diff too big for a little gain.
        # See bug #7059
        sed -i -e "/X-Launchpad-Export-Date/d" $LAUNCHPAD_DIRECTORY/$file

        echo "/bin/cp $LAUNCHPAD_DIRECTORY/$file $DIR/$MODULE.po"
        /bin/cp -f $LAUNCHPAD_DIRECTORY/$file $DIR/$MODULE.po
        if test $? -ne 0; then
            echo "Error detected in the copy"
            exit 1;
        fi
    else
        echo "Ignore locale $LOC"
    fi
done
