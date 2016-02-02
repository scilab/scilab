#!/bin/sh
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) INRIA - 2007-2008 - Sylvestre Ledru
# Copyright (C) DIGITEO - 2009-2010 - Sylvestre Ledru
# Copyright (C) 2010 - Calixte DENIZET
# Copyright (C) 2012 - 2016 - Scilab Enterprises
#
# This file is hereby licensed under the terms of the GNU GPL v2.0,
# pursuant to article 5.3.4 of the CeCILL v.2.1.
# This file was originally licensed under the terms of the CeCILL v2.1,
# and continues to be available under such terms.
# For more information, see the COPYING file which you should have received
# along with this program.
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
    exit 1
fi

if test -z "$SCI"; then
        echo "Please define the variable SCI"
        exit 2
fi

TMPDIR=""
LAUNCHPAD_DIRECTORY=$1
if test \( ! -d $LAUNCHPAD_DIRECTORY -a -f $LAUNCHPAD_DIRECTORY \); then
    TMPDIR=$(mktemp -d)
    tar --one-top-level=$TMPDIR -xzf $LAUNCHPAD_DIRECTORY
    LAUNCHPAD_DIRECTORY=$TMPDIR
fi
if test ! -d $LAUNCHPAD_DIRECTORY; then
    echo "Could not find $LAUNCHPAD_DIRECTORY."
    echo "Exiting..."
    exit 3
fi

# Don't know why but launchpad is placing some files in some sub dirs
FILES=$(find $LAUNCHPAD_DIRECTORY/ -mindepth 2 -iname '*.po' -type f)
/bin/cp -fiu $FILES $LAUNCHPAD_DIRECTORY/

for file in $LAUNCHPAD_DIRECTORY/*.po; do
    file=`echo $file|awk -F / '{print $NF}'` # get only the filename
    LOC=`echo $file|cut -d. -f1|awk -F - '{print $NF}'` # Get the locale (fr_FR, en_US ...)

    if test "$LOC" = "fr"; then LOC="fr_FR"; fi # Retrieve real format
    if test "$LOC" = "ru"; then LOC="ru_RU"; fi
    if test "$LOC" = "de"; then LOC="de_DE"; fi
    if test "$LOC" = "ca"; then LOC="ca_ES"; fi
    if test "$LOC" = "es"; then LOC="es_ES"; fi
    if test "$LOC" = "ja"; then LOC="ja_JP"; fi
    if test "$LOC" = "it"; then LOC="it_IT"; fi
    if test "$LOC" = "uk"; then LOC="uk_UA"; fi
    if test "$LOC" = "pl"; then LOC="pl_PL"; fi
    if test "$LOC" = "cs"; then LOC="cs_CZ"; fi

    # check that it is the right format
    echo "$LOC"|grep -E "(.*_.*)" > /dev/null # it is a real localization name xx_YY

    if test $? -eq 0; then
        MODULE=`echo $file|sed -e "s|macros-||"|sed -e "s|\(.*\)-.*|\1|"|sed -e "s|-|_|g"` # Get the module name (for example signal_processing)
        if test "$MODULE" = "pvm" -o "$MODULE" = "shell"; then
            continue
        fi
        echo $file|grep "macros-" > /dev/null
        IS_MACRO=$?
        if test "$IS_MACRO" -eq 0; then
            TARGETFILE=$SCI/modules/$MODULE/locales_macros/$LOC.po
        else
            TARGETFILE=$SCI/modules/$MODULE/locales/$LOC.po
        fi

        
        # Do not copy empty files
        if test -n "$(msgcat $LAUNCHPAD_DIRECTORY/$file)"; then
	    # Before the copy, strip the line with the date. It is only making
	    # diff too big for a little gain.
	    # See bug #7059
	    sed -i -e "/X-Launchpad-Export-Date/d" $LAUNCHPAD_DIRECTORY/$file

	    cp -f $LAUNCHPAD_DIRECTORY/$file $TARGETFILE
        fi
    else
        echo "Ignore locale $LOC"
    fi
done

# Remove english variants
rm -f modules/*/locales/en_*.po
[ -d $TMPDIR ] && rm -fr $TMPDIR

