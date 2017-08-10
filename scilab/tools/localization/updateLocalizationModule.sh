#!/bin/bash
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) INRIA - 2007-2008 - Sylvestre Ledru
# Copyright (C) DIGITEO - 2009-2011 - Sylvestre Ledru
# Copyright (C) DIGITEO - 2011-2011 - Bruno JOFRET
# Copyright (C) Scilab Enterprises - 2015 - Clement DAVID
#
# Copyright (C) 2012 - 2016 - Scilab Enterprises
#
# This file is hereby licensed under the terms of the GNU GPL v2.0,
# pursuant to article 5.3.4 of the CeCILL v.2.1.
# This file was originally licensed under the terms of the CeCILL v2.1,
# and continues to be available under such terms.
# For more information, see the COPYING file which you should have received
# along with this program.

# This script goes into a module and updates the localization file by checking
# the _( and gettext( calls in the code
#
# This script process all source files as "C" like language and perform extra
# conversion of scilab code.
#

# exit on error
set -e
# trace execution (for debug)
#set -x

if test $# -ne 1; then
    echo "This script goes into a module and updates the localization file "
    echo "by checking the _(xxx), _W() and gettext(xxx) calls in the code"
    echo "It creates the locales directory for C, C++, Java and Scilab script"
    echo
    echo "Syntax : $0 <module>"
    echo "If <module> is equal to 'process_all', it will parse all Scilab module"
    echo "per module"
    exit 42
fi

if test -z "$SCI"; then
    echo "Please define the variable SCI"
    exit 42
fi

MODULES=$1
# Process all the modules and build the list
if test "$MODULES" = "process_all"; then
    echo ".. Process all the modules one by one"
    MODULES=$(find $SCI/modules/ -maxdepth 1 -type d ! -name ".*" -printf '%P\n' |sort)
else
    MODULES=$(echo $MODULES|sed -e 's|./||') # avoid to have ./module_name
fi

XGETTEXT=/usr/bin/xgettext
FROM_CODE=UTF-8
EXTENSIONS=( 'c' 'h' 'cpp' 'hxx' 'java' )
EXTENSIONS_SCILAB=( sci sce start quit )
TARGETDIR=locales
HEADER_TEMPLATE=$SCI/modules/localization/data/header.pot
TIMEZONE="+0100"
# Gettext arg
XGETTEXT_OPTIONS="--add-location --strict --keyword=_ --keyword=_W --from-code $FROM_CODE --omit-header --no-wrap --sort-by-file"

function preprocess_xml() {
#
# Process XML files, both help or xslt
#
    COMMON_SED='s/&amp;/\&/g'

    FILES=$(ls $* 2>/dev/null || true)
    [ -z "$FILES" ] && return
    FAKE_C_FILE=$(printf "%s/src/%s_fake_xml.c" $PATHTOPROCESS $MODULE)

    # remove line which does NOT contain label
    sed  -e '/label/!s/.*//'  -e 's/.*label="\([^"]*\)".*/gettext("\1")/' -e '/^$/d' -e $COMMON_SED $FILES > $FAKE_C_FILE
    # extract the content of the label and switch it to a gettext fake instruction
    sed  -e '/tooltiptext/!s/.*//'  -e 's/.*tooltiptext="\([^"]*\)".*/gettext("\1")/' -e '/^$/d' -e $COMMON_SED $FILES >> $FAKE_C_FILE
    # remove empty lines
    sed -e 's/.*_(\([^"]*\)).*/gettext("\1")/' $FILES >> $FAKE_C_FILE
}

function generate_find_command() {
    # Setting the shell's Internal Field Separator to null
    OLD_IFS=$IFS
    IFS=''
    # Create a string containing "colors[*]"
    local array_string="$1[*]"
    # assign loc_array value to ${colors[*]} using indirect variable reference
    local EXT=(${!array_string})
#
# Retrieve all the sources files
    FILESCMD="find $PATHS -type f "
####### GENERATES THE FIND COMMAND
    i=0
    NB_ELEMENT=${#EXT[@]}

    while [ "$i" -lt "$NB_ELEMENT" ]; do
        ext=${EXT[$i]}
        FILESCMD="$FILESCMD -name '*.$ext'"
        if test "$NB_ELEMENT" -ne $(expr $i + 1); then # because we don't want a trailing -o
            FILESCMD="$FILESCMD -o "
        fi
        i=$((i + 1))
    done
    # Resetting IFS to default
    IFS=$OLD_IFS
}

function process_src() {
    PATHS=$(ls -d $* 2>/dev/null || true)
    [ -z "$PATHS" ] && return

    generate_find_command EXTENSIONS
    [ -z "$FILESCMD" ] && return
    FILES=$(eval $FILESCMD|sort |tr "\n" " ")
    [ -z "$FILES" ] && return

    # It is Scilab code... xgettext does not how to process it
    XGETTEXT_OPTIONS="$XGETTEXT_OPTIONS --language=C"

    echo "..... Source files in"
    printf "....... %s\n" $*
    $XGETTEXT $XGETTEXT_OPTIONS -p $PATHTOPROCESS/$TARGETDIR -o ${MODULE}_src.pot $FILES >/dev/null 2>>xgettext_errors.log
}

function process_scilab_code() {
    PATHS=$(ls -d $* 2>/dev/null || true)
    [ -z "$PATHS" ] && return

    generate_find_command EXTENSIONS_SCILAB
    FILES=$(eval $FILESCMD|sort |tr "\n" " ")

    # It is Scilab code... xgettext does not how to process it
    XGETTEXT_OPTIONS="$XGETTEXT_OPTIONS --language=C"

    if test "$MODULE" = "core"; then
        # We want some strings from the ROOTDIR when it is the core module
        FILES="$FILES $(ls $SCI/etc/scilab.*)"
    fi

    echo "..... Scilab scripts in"
    printf "....... %s\n" $*
    $XGETTEXT $XGETTEXT_OPTIONS -p $PATHTOPROCESS/$TARGETDIR -o ${MODULE}_scilab.pot $FILES >/dev/null 2>>xgettext_errors.log

    # Post-process
    if test -f $PATHTOPROCESS/$TARGETDIR/${MODULE}_scilab.pot; then
        # Empty file => no string found
        # We are modifing on the fly Scilab localization files
        #
        # We need C strings format to be used as gettext key
        # "" -> \"
        # '' -> '
        # '" -> \"
        # "' -> '
        sed -i -e "s/\"\"/\\\"/g" -e "s/''/'/g" -e "s/'\"/\\\"/g" $PATHTOPROCESS/$TARGETDIR/${MODULE}_scilab.pot
        # We introduced invalid tag [msgstr "] and [msgid "]
        # restore them [msgstr ""] and [msgid ""]
        sed -i -e "s/msgstr \"$/msgstr \"\"/" -e "s/msgid \"$/msgid \"\"/" $PATHTOPROCESS/$TARGETDIR/${MODULE}_scilab.pot
    fi
}

function merge_pot() {

    now=$(date +'%Y-%m-%d %H:%M')$TIMEZONE
    if test  -z "$CreationDate"; then
        # File not existing before ... Set the current date a POT-Creation-Date
        sed -e "s/MODULE/$MODULE/" -e "s/CREATION-DATE/$now/" -e "s/REVISION-DATE/$now/" $HEADER_TEMPLATE > $LOCALIZATION_FILE_US
    else
        sed -e "s/MODULE/$MODULE/" -e "s/CREATION-DATE/$CreationDate/" -e "s/REVISION-DATE/$now/" $HEADER_TEMPLATE > $LOCALIZATION_FILE_US
    fi

    for f in $PATHTOPROCESS/$TARGETDIR/${MODULE}_{src,scilab}.pot; do
        if test -f $f; then
            msgcat $f >> $LOCALIZATION_FILE_US.tmp
            [ $? -eq 0 ] || exit 1
            rm -f $f
        fi
    done
    [ -f $LOCALIZATION_FILE_US.tmp ] && msguniq -u $LOCALIZATION_FILE_US.tmp >>$LOCALIZATION_FILE_US
    rm -f $LOCALIZATION_FILE_US.tmp

    MSGCOUNT=$(msgcat $LOCALIZATION_FILE_US |grep msgid |wc -l)
    if test $? -ne 0; then
        echo "Badly formated localization files"
        cd -
        exit 32
    fi
    if test $MSGCOUNT -le 1 ; then
        # empty template. Kill it!
        rm -f $LOCALIZATION_FILE_US
    fi
}

function upgrade_po() {
    # for all available languages, upgrade the po files
    [ ! -f $1/${MODULE}.pot ] && return

    for f in $1/*.po; do
        if test -f $f; then
            msguniq --use-first -o $f $f &>/dev/null ;
            msgmerge -U $f $1/${MODULE}.pot &>/dev/null ;
        fi
    done
}


rm -f xgettext_errors.log # cleanup the error log
cd $SCI # use relative path to SCI
for MODULE in $MODULES; do

    PATHTOPROCESS=modules/$MODULE
    if test ! -d $PATHTOPROCESS; then
        echo "... Cannot find module $PATHTOPROCESS"
        cd - >/dev/null
        exit -1
    fi
    echo "... Processing \"$MODULE\""
    LOCALIZATION_FILE_US=$PATHTOPROCESS/$TARGETDIR/${MODULE}.pot

    if test ! -d $PATHTOPROCESS/$TARGETDIR; then mkdir $PATHTOPROCESS/$TARGETDIR; fi
    if test -f $LOCALIZATION_FILE_US; then
        # Localization file already existing. Retrieve POT-Creation-Date
        CreationDate=`grep POT-Creation-Date: $LOCALIZATION_FILE_US |sed -e 's|\"POT-Creation-Date: \(.*\)\\\n\"|\1|'`
    fi

    preprocess_xml $PATHTOPROCESS/etc/*.x*l $PATHTOPROCESS/etc/*.x*l
    process_scilab_code $PATHTOPROCESS/macros $PATHTOPROCESS/etc $PATHTOPROCESS/demos $PATHTOPROCESS/tests
    process_src $PATHTOPROCESS/sci_gateway $PATHTOPROCESS/src
    rm -f $PATHTOPROCESS/src/${MODULE}_fake_xml.c
    merge_pot $PATHTOPROCESS/locales/*.pot
    upgrade_po $PATHTOPROCESS/locales

done # Browse modules

cd - >/dev/null
exit 0 # success

