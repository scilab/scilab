#!/bin/bash
# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) INRIA - 2007-2008 - Sylvestre Ledru
# Copyright (C) DIGITEO - 2009-2011 - Sylvestre Ledru
# Copyright (C) DIGITEO - 2011-2011 - Bruno JOFRET
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at
# http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
#
# This script goes into a module and updates the localization file by checking
# the _( and gettext( calls in the code

# TODO :
# * Write Small documentation
#

if test $# -ne 1; then
    echo "This script goes into a module and updates the localization file "
    echo "by checking the _(xxx) and gettext(xxx) calls in the code"
    echo "It creates the locales directory for C, C++ and Java and"
    echo "locales_macros for Scilab code"
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

if test "$MODULES" = "process_all"; then
    echo ".. Process all the modules one by one"
    PROCESS_ALL=1
else
    PROCESS_ALL=0
fi


XGETTEXT=/usr/bin/xgettext
FROM_CODE=ISO-8859-1
EXTENSIONS=( 'c' 'h' 'cpp' 'hxx' 'java' )
EXTENSIONS_MACROS=( sci sce start quit )
TARGETDIR=locales/
TARGETDIR_MACROS=locales_macros/
HEADER_TEMPLATE=$SCI/modules/localization/data/header.pot
GUI_FILES="etc/*.x*l"
PREFERENCE_FILES="src/xslt/*.xsl"
FAKE_C_FILE=scilab_fake_localization_file.c
TIMEZONE="+0100"
# Gettext arg
XGETTEXT_OPTIONS="--add-location --strict --keyword=_ --from-code $FROM_CODE --omit-header --no-wrap --sort-by-file"

process_XML_files() {
# First expression => remove line which does NOT contain label
# Second expression =>  extract the content of the label and switch it to a gettext fake instruction
# Third expression => remove empty lines
# Please note that it will only extract string from the label tag
    if test -n "$(ls $GUI_FILES 2>/dev/null)"; then
        COMMON_SED='s/&amp;/\&/g'
	sed  -e '/label/!s/.*//'  -e 's/.*label="\([^"]*\)".*/gettext("\1")/' -e '/^$/d' -e $COMMON_SED $GUI_FILES > $FAKE_C_FILE
	sed  -e '/tooltiptext/!s/.*//'  -e 's/.*tooltiptext="\([^"]*\)".*/gettext("\1")/' -e '/^$/d' -e $COMMON_SED $GUI_FILES >> $FAKE_C_FILE
	sed -e 's/.*_(\([^"]*\)).*/gettext("\1")/' $GUI_FILES >> $FAKE_C_FILE
    fi
    if test -n "$(ls $PREFERENCE_FILES 2>/dev/null)"; then
	sed -e 's/.*_(\([^"]*\)).*/gettext("\1")/' $PREFERENCE_FILES  >> $FAKE_C_FILE
    fi
}


function generate_find_command {
    # Setting the shell's Internal Field Separator to null
    OLD_IFS=$IFS
    IFS=''
    # Create a string containing "colors[*]"
    local array_string="$1[*]"
    # assign loc_array value to ${colors[*]} using indirect variable reference
    local EXT=(${!array_string})
#
# Retrieve all the sources files
    FILESCMD='find . -type f '
####### GENERATES THE FIND COMMAND
    i=0
    NB_ELEMENT=${#EXT[@]}

    while [ "$i" -lt "$NB_ELEMENT" ]; do
        ext=${EXT[$i]}
        FILESCMD="$FILESCMD -name '*.$ext'"
        if test "$NB_ELEMENT" -ne `expr $i + 1`; then # because we don't want a trailing -o
            FILESCMD="$FILESCMD -o "
        fi
        i=$((i + 1))
    done
    # Resetting IFS to default
    IFS=$OLD_IFS
}


# Process all the modules ... then, build the list
if test $PROCESS_ALL -eq 1; then
    cd $SCI/modules/
    MODULES=`find . -maxdepth 1 -type d  ! -name ".*" ! -name 'javasci'`
fi


function process_module {

    IS_MACROS=0
    if test "$1" == "macros"; then
        IS_MACROS=1
    fi

# Extract label from xml files
    process_XML_files
    if test $IS_MACROS -eq 1; then
        generate_find_command EXTENSIONS_MACROS
        local TARGETDIR=$TARGETDIR_MACROS
    else
        generate_find_command EXTENSIONS
    fi
    if test ! -d $TARGETDIR; then mkdir $TARGETDIR; fi

    FILES=`eval $FILESCMD|tr "\n" " "`

    if test "$MODULE" = "core" -o "$MODULE" = "./core"; then
        # We want some strings from the ROOTDIR when it is the core module
        FILES="$FILES `ls $SCI/etc/scilab.*`"
    fi

    # Also extract string straight from the XML because we have some gettext calls in it
    if test -n "$(ls $GUI_FILES 2>/dev/null)" -a $IS_MACROS -ne 1; then
        FILES="$FILES `ls $GUI_FILES`"
    fi

    FILES=$(echo $FILES|sort)

    MODULE_NAME=`echo $MODULE|sed -e 's|./||'` # avoid to have ./module_name

    if test $IS_MACROS -eq 1; then
        echo "..... Parsing all Scilab macros in $PATHTOPROCESS"
    else
        echo "..... Parsing all sources in $PATHTOPROCESS"
    fi
# Parse all the sources and get the string which should be localized


    if test $IS_MACROS -eq 1; then
        MODULE_NAME=$MODULE_NAME-macros
    fi
    LOCALIZATION_FILE_US=$TARGETDIR/$MODULE_NAME.pot

    if test -f $LOCALIZATION_FILE_US; then
        # Localization file already existing. Retrieve POT-Creation-Date
        CreationDate=`grep POT-Creation-Date: $LOCALIZATION_FILE_US|sed -e 's|\"POT-Creation-Date: \(.*\)\\\n\"|\1|'`
    fi

    echo "........ Generate the English localization file by parsing the code"
    if test $IS_MACROS -eq 1; then
        # It is Scilab code... xgettext does not how to process it
        XGETTEXT_OPTIONS="$XGETTEXT_OPTIONS --language=C"
    fi

    $XGETTEXT $XGETTEXT_OPTIONS -p $TARGETDIR/ -o $MODULE_NAME.pot.tmp $FILES > /dev/null
    if test ! -f $MODULE_NAME.pot.tmp -a $IS_MACROS -eq 1; then
        # Empty file => no string found
        # We are modifing on the fly Scilab localization files
        #
        # We need C strings format to be used as gettext key
        # "" -> \"
        # '' -> '
        # '" -> \"
        # "' -> ' -e "s/\"'/'/g" 
        sed -i -e "s/\"\"/\\\"/g" -e "s/''/'/g" -e "s/'\"/\\\"/g" $TARGETDIR/$MODULE_NAME.pot.tmp
        # We introduced invalid tag [msgstr "] and [msgid "]
        # restore them [msgstr ""] and [msgid ""]
        sed -i -e "s/msgstr \"$/msgstr \"\"/" -e "s/msgid \"$/msgid \"\"/" $TARGETDIR/$MODULE_NAME.pot.tmp
    fi

    if test  -z "$CreationDate"; then
        # File not existing before ... Set the current date a POT-Creation-Date
        sed -e "s/MODULE/$MODULE_NAME/" -e "s/CREATION-DATE/`date +'%Y-%m-%d %H:%M'`$TIMEZONE/" -e "s/REVISION-DATE/`date +'%Y-%m-%d %H:%M'`$TIMEZONE/" $HEADER_TEMPLATE > $LOCALIZATION_FILE_US
    else
        sed -e "s/MODULE/$MODULE_NAME/" -e "s/CREATION-DATE/$CreationDate/" -e "s/REVISION-DATE/`date +'%Y-%m-%d %H:%M'`$TIMEZONE/" $HEADER_TEMPLATE > $LOCALIZATION_FILE_US
    fi

    msguniq -u $LOCALIZATION_FILE_US.tmp >> $LOCALIZATION_FILE_US 2> /dev/null

    rm $LOCALIZATION_FILE_US.tmp 2> /dev/null

    MSGOUTPUT=$(msgcat $LOCALIZATION_FILE_US)
    if test $? -ne 0; then
        echo "Badly formated localization files"
        exit 32
    fi
    if test -z "$(msgcat $LOCALIZATION_FILE_US)"; then
        # empty template. Kill it!
        rm $LOCALIZATION_FILE_US
    fi

    if test $IS_MACROS -eq 1; then
        LOCALIZATION_FILE_NATIVE=$(echo $LOCALIZATION_FILE_US|sed -e "s|-macros||g" -e "s|_macros||g")
        if test ! -f $LOCALIZATION_FILE_NATIVE; then
            # no native code. Copy the macro one
            cp $LOCALIZATION_FILE_US $LOCALIZATION_FILE_NATIVE
        else
        # merge locale macros => native code
            msgcat --use-first  -o $LOCALIZATION_FILE_NATIVE.tmp  $LOCALIZATION_FILE_NATIVE $LOCALIZATION_FILE_US
            mv $LOCALIZATION_FILE_NATIVE.tmp $LOCALIZATION_FILE_NATIVE
        fi
        rm -rf $TARGETDIR_MACROS
    fi

    # Remove fake file used to extract string from XML
    rm $FAKE_C_FILE 2> /dev/null


}

for MODULE in $MODULES; do

    PATHTOPROCESS=$SCI/modules/$MODULE/
    if test ! -d $PATHTOPROCESS; then
        echo "... Cannot find module $PATHTOPROCESS"
        exit
    fi
    echo "... Processing module $MODULE"

    cd $PATHTOPROCESS
    process_module "src"
    process_module "macros"

    cd $SCI/
done # Browse modules
