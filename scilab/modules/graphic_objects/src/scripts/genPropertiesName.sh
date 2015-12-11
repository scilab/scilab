#! /bin/sh
##  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
##  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
##  Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
##
##  This file must be used under the terms of the CeCILL.
##  This source file is licensed as described in the file COPYING, which
##  you should have received as part of this distribution.  The terms
##  are also available at
##  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
##
##

main()
{
    if [ $# -ne 3 ]; then
        echo "Usage: $0 --java|--C <PropertiesFile> <OuputFile>"
        exit 1
    fi

    OutFile=$3
    PropertiesFile=$2
    Mode=$1

    case "$Mode" in
        --java)
            generateJavaFile;;
        --C)
            generateCFile;;
    esac
}

generateJavaFile()
{
    if test $PropertiesFile -nt $OutFile; then
        echo "-- Building GraphicObjectProperties.java --"
        generateHeader
        echo "package org.scilab.modules.graphic_objects.graphicObject;" >> $OutFile
        echo ""  >> $OutFile
        echo "public class GraphicObjectProperties {" >> $OutFile
        echo ""  >> $OutFile

        awk 'BEGIN {num=0} (NF > 0) {printf "    public static final int %s = %d;\n", $1, num; num++}' < $PropertiesFile >> $OutFile

        echo ""  >> $OutFile
        echo "}"  >> $OutFile
    else
        echo "-- GraphicObjectProperties.java already up-to-date --"
    fi
}

generateCFile()
{
    if test $PropertiesFile -nt $OutFile; then
        echo "-- Building GraphicObjectProperties.h --"
        generateHeader
        echo "#ifndef  __GRAPHIC_OBJECT_PROPERTIES_H__" >> $OutFile
        echo "#define __GRAPHIC_OBJECT_PROPERTIES_H__" >> $OutFile
        echo ""  >> $OutFile

        awk 'BEGIN {num=0} (NF > 0) {printf "#define %s %d\n", $1, num; num++}' < $PropertiesFile >> $OutFile

        echo ""  >> $OutFile
        echo "#endif /* !__GRAPHIC_OBJECT_PROPERTIES_H__ */" >> $OutFile
    else
        echo "-- GraphicObjectProperties.h already up-to-date --"
    fi
}

generateHeader()
{
    echo "/*" > $OutFile
    echo " *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab" >> $OutFile
    echo " *  Copyright (C) 2010-2012 - DIGITEO - Bruno JOFRET" >> $OutFile
    echo " *  Copyright (C) 2012-2014 - Scilab-Enterprises - Bruno JOFRET" >> $OutFile
    echo " *" >> $OutFile
    echo " *  This file must be used under the terms of the CeCILL." >> $OutFile
    echo " *  This source file is licensed as described in the file COPYING, which" >> $OutFile
    echo " *  you should have received as part of this distribution.  The terms" >> $OutFile
    echo " *  are also available at" >> $OutFile
    echo " *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt" >> $OutFile
    echo " *" >> $OutFile
    echo " */" >> $OutFile

    echo ""  >> $OutFile

    echo "/*" >> $OutFile
    echo " * -=- This is a generated file, please do not edit by hand             -=-" >> $OutFile
    echo " * -=- Please see properties definitions in                             -=-" >> $OutFile
    echo " * -=- SCI/modules/graphic_objects/src/scripts/propertiesMap.properties -=-" >> $OutFile
    echo " */" >> $OutFile

    echo "" >> $OutFile
}

main $@
