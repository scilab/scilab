#!/bin/sh

# English is the reference. Update all other languages to put them at the right
# place.

if test -z "$SCI"; then
        echo "Please define the variable SCI"
        exit 254
fi

REFERENCE_LANG=en_US
LANGS="ja_JP fr_FR pt_BR"

ENGLISH_FILES=$(find modules/ -iname '*.xml'|grep "/help/$REFERENCE_LANG")

for f in $ENGLISH_FILES; do
    FILENAME=$(basename $f)
    DIRNAME=$(dirname $f)
    MODULENAME=$(echo $f|awk -F '/' '{print $1"/"$2}')

    # Check if the file already exists in other langs
    for l in $LANGS; do
        fTranslated=$(echo $f|sed -e "s|$REFERENCE_LANG|$l|")

        if test ! -f $fTranslated; then
            # File not found. However, it might not have been translated
            # Look if it the case or not

            # File found but at the wrong place
            FOUNDFILE=$(find $MODULENAME -name $FILENAME|grep $l)
            if test "$FOUNDFILE" != ""; then
                # File found.
                TARGET=$(echo $DIRNAME|sed -e "s|$REFERENCE_LANG|$l|")
                if test ! -d $TARGET; then
                    echo "Creation of $TARGET"
                    mkdir $TARGET
                fi
                echo "mv $FOUNDFILE $TARGET"
                mv $FOUNDFILE $TARGET
            fi
            

        fi
    done
done
