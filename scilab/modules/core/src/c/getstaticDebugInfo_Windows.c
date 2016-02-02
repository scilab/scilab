/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

/* !!! PLEASE DO NOT TRANSLATE STRINGS IN THIS FILE SEE BUG 5505 !!! */

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <xmlversion.h>
#include <tcl.h>
#include <tk.h>
#include "sci_malloc.h"
#include "getstaticDebugInfo_Windows.h"
#include "localization.h"
#include "version.h"
#include "getversion.h"
#include "..\..\..\..\libs\pcre\pcre.h"
#include "getBlasType.h"
/*--------------------------------------------------------------------------*/
extern BOOL BuildWithVS8ExpressF2C(void);
static char ** appendStringStaticDebugInfo(char **listInfo, int *sizeListInfo, char *str);
/*--------------------------------------------------------------------------*/
char **getStaticDebugInfo_Windows(int *sizeArray)
{
#define BUFFER_LEN 255
    int nb_info = 0;
    char **outputDynamicList = NULL;
    char *str_info = NULL;

    str_info = (char*)MALLOC(sizeof(char) * BUFFER_LEN);
    if (str_info)
    {
        sprintf(str_info, "Version: %s", SCI_VERSION_STRING);
        outputDynamicList = appendStringStaticDebugInfo(outputDynamicList, &nb_info, str_info);
    }

    str_info = (char*)MALLOC(sizeof(char) * BUFFER_LEN);
    if (str_info)
    {
        sprintf(str_info, "Compilation date: %s", __DATE__);
        outputDynamicList = appendStringStaticDebugInfo(outputDynamicList, &nb_info, str_info);
    }

    str_info = (char*)MALLOC(sizeof(char) * BUFFER_LEN);
    if (str_info)
    {
        sprintf(str_info, "Compilation time: %s", __TIME__);
        outputDynamicList = appendStringStaticDebugInfo(outputDynamicList, &nb_info, str_info);
    }

    str_info = (char*)MALLOC(sizeof(char) * BUFFER_LEN);
    if (str_info)
    {
#ifdef _WIN64
        sprintf(str_info, "Compiler Architecture: %s", "X64");
#else
        sprintf(str_info, "Compiler Architecture: %s", "x86");
#endif
        outputDynamicList = appendStringStaticDebugInfo(outputDynamicList, &nb_info, str_info);
    }

    str_info = (char*)MALLOC(sizeof(char) * BUFFER_LEN);
    if (str_info)
    {

#ifdef __INTEL_COMPILER
        sprintf(str_info, "Compiled with %s (%d)", "Intel compiler", __INTEL_COMPILER);
#elif defined _MSC_VER
        sprintf(str_info, "Compiled with %s (%d)", "Microsoft compiler", _MSC_FULL_VER);
#else
        sprintf(str_info, "Compiled with %s", "unknown compiler");
#endif
        outputDynamicList = appendStringStaticDebugInfo(outputDynamicList, &nb_info, str_info);
    }

    if (BuildWithVS8ExpressF2C())
    {
        str_info = (char*)MALLOC(sizeof(char) * BUFFER_LEN);
        if (str_info)
        {
            sprintf(str_info, "Compiled with %s", "F2C");
            outputDynamicList = appendStringStaticDebugInfo(outputDynamicList, &nb_info, str_info);
        }
    }

    str_info = (char*)MALLOC(sizeof(char) * BUFFER_LEN);
    if (str_info)
    {
        blas_type blasType = getBlasType();
        strcpy(str_info, "BLAS library optimized version: ");
        switch (blasType)
        {
            default:
            case BLAS_UNKNOW:
                strcat(str_info, "Unknown");
                break;

            case BLAS_REF:
                strcat(str_info, "Reference");
                break;

            case BLAS_ATLAS:
                strcat(str_info, "Atlas");
                break;

            case BLAS_MKL:
                strcat(str_info, "MKL");
                break;
        }
        outputDynamicList = appendStringStaticDebugInfo(outputDynamicList, &nb_info, str_info);
    }


    str_info = (char*)MALLOC(sizeof(char) * BUFFER_LEN);
    if (str_info)
    {
        sprintf(str_info, "XML version: %s", LIBXML_DOTTED_VERSION);
        outputDynamicList = appendStringStaticDebugInfo(outputDynamicList, &nb_info, str_info);
    }

    if ( with_tk() )
    {
        str_info = (char*)MALLOC(sizeof(char) * BUFFER_LEN);
        if (str_info)
        {
            sprintf(str_info, "Tcl/Tk: %s", "Enable");
            outputDynamicList = appendStringStaticDebugInfo(outputDynamicList, &nb_info, str_info);
        }

        str_info = (char*)MALLOC(sizeof(char) * BUFFER_LEN);
        if (str_info)
        {
            sprintf(str_info, "TCL version: %s", TCL_PATCH_LEVEL);
            outputDynamicList = appendStringStaticDebugInfo(outputDynamicList, &nb_info, str_info);
        }

        str_info = (char*)MALLOC(sizeof(char) * BUFFER_LEN);
        if (str_info)
        {
            sprintf(str_info, "TK version: %s", TK_PATCH_LEVEL);
            outputDynamicList = appendStringStaticDebugInfo(outputDynamicList, &nb_info, str_info);
        }
    }

    str_info = (char*)MALLOC(sizeof(char) * BUFFER_LEN);
    if (str_info)
    {
        sprintf(str_info, "Path separator: %s", PATH_SEPARATOR);
        outputDynamicList = appendStringStaticDebugInfo(outputDynamicList, &nb_info, str_info);
    }

    str_info = (char*)MALLOC(sizeof(char) * BUFFER_LEN);
    if (str_info)
    {
        sprintf(str_info, "Directory separator: %s", DIR_SEPARATOR);
        outputDynamicList = appendStringStaticDebugInfo(outputDynamicList, &nb_info, str_info);
    }

    str_info = (char*)MALLOC(sizeof(char) * BUFFER_LEN);
    if (str_info)
    {
        sprintf(str_info, "PCRE Version: %d.%d", PCRE_MAJOR, PCRE_MINOR);
        outputDynamicList = appendStringStaticDebugInfo(outputDynamicList, &nb_info, str_info);
    }

    *sizeArray = nb_info;
    return outputDynamicList;
}
/*--------------------------------------------------------------------------*/
static char ** appendStringStaticDebugInfo(char **listInfo, int *sizeListInfo, char *str)
{
    char ** returnListInfo = NULL;
    if (listInfo)
    {
        (*sizeListInfo)++;
        returnListInfo = (char**)REALLOC(listInfo, sizeof(char*) * (*sizeListInfo));
    }
    else
    {
        (*sizeListInfo) = 1;
        returnListInfo = (char**)MALLOC(sizeof(char*) * (*sizeListInfo));
    }

    if (returnListInfo)
    {
        returnListInfo[(*sizeListInfo) - 1] = str;
    }
    return returnListInfo;
}
/*--------------------------------------------------------------------------*/
