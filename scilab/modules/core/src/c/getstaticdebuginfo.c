/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
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

#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <libxml/xmlversion.h>

#ifdef WITH_TK
#include <tcl.h>
#include <tk.h>
#endif

/*
Commented for now

#ifdef WITH_UMFPACK
#ifdef UMFPACK_SUITESPARSE
#include <suitesparse/umfpack.h>
#else
#include <umfpack.h>
#endif
#endif
*/
#include "sci_malloc.h"
#include "getstaticdebuginfo.h"
#include "version.h"

char **getStaticDebugInfo(int *sizeArray)
{
    char **outputStaticList = NULL;
    int i;

    static debug_message staticDebug[NB_DEBUG_ELEMENT] =
    {
#ifdef SCI_VERSION_STRING
        {"Scilab Version", SCI_VERSION_STRING},
#endif
#ifdef __DATE__
        {"Compilation date", __DATE__},
#endif
#ifdef __TIME__
        {"Compilation time", __TIME__},
#endif
#ifdef __VERSION__
        {"Compileur version", __VERSION__},
#endif
#ifdef LIBXML_DOTTED_VERSION
        {"XML version", LIBXML_DOTTED_VERSION},
#endif
#ifdef LIBXML_FLAGS
        {"XML compilation flags", LIBXML_FLAGS},
#endif
#ifdef LIBXML_LIBS
        {"XML libraries", LIBXML_LIBS},
#endif
#ifdef PCRE_VERSION
        {"PCRE version", PCRE_VERSION},
#endif
#ifdef PCRE_FLAGS
        {"PCRE compilation flags", PCRE_FLAGS},
#endif
#ifdef PCRE_LIBS
        {"PCRE libraries", PCRE_LIBS},
#endif
#ifdef TCL_PATCH_LEVEL
        {"TCL version", TCL_PATCH_LEVEL},
#endif
#ifdef TK_PATCH_LEVEL
        {"TK version", TK_PATCH_LEVEL},
#endif
#ifdef SHARED_LIB_EXT
        {"Shared library extension", SHARED_LIB_EXT},
#endif
#ifdef WITH_GUI
        {"Scilab GUI", "Enable"},
#endif
#ifdef WITH_FFTW
        {"FFTW", "Enable"},
#endif
#ifdef PATH_SEPARATOR
        {"Path separator", PATH_SEPARATOR},
#endif
#ifdef DIR_SEPARATOR
        {"Directory separator", DIR_SEPARATOR},
#endif
#ifdef WITH_UMFPACK
        {"UMFPACK", "Enable"},
#ifdef UMFPACK_VERSION
        {"UMFPACK version", UMFPACK_VERSION},
#endif
#ifdef UMFPACK_VERSION
        {"UMFPACK version", UMFPACK_VERSION},
#endif
#endif
#ifndef _LP64
        {"Compiler Architecture", "X86"},
#endif
#ifdef _LP64
        {"Compiler Architecture", "X64"},
#endif
    };

    for (i = 0; i < NB_DEBUG_ELEMENT; i++)
    {
        debug_message msg = staticDebug[i];

        if (msg.description == NULL)    /* We reach the end of the static list */
        {
            break;
        }

        if (outputStaticList)
        {
            /* Alloc the big list */
            outputStaticList = (char **)REALLOC(outputStaticList, sizeof(char *) * (i + 1));
        }
        else
        {
            outputStaticList = (char **)MALLOC(sizeof(char *) * (i + 1));
        }

        /* Create the element in the array */
        outputStaticList[i] = (char *)MALLOC((strlen(msg.description) + strlen(msg.value) + 3) * sizeof(char)); /* 3 for :, space and \0 */
        sprintf(outputStaticList[i], "%s: %s", msg.description, msg.value);
    }
    *sizeArray = i;
    return outputStaticList;
}
