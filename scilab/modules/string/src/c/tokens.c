/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU
 * Copyright (C) INRIA - 2008 - Allan CORNET
 * Copyright (C) Digiteo - 2011 - Cedric DELAMARRE
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "sci_malloc.h"
#include "tokens.h"
#include "os_string.h"
#include "os_wcstok.h"


/*------------------------------------------------------------------------*/
char** stringTokens(const char* str, const char* delim, int* sizeoutputs)
{
    char **outputs = NULL;
    *sizeoutputs = 0;
    if (str)
    {
        if (delim)
        {
            int i = 0;
            char *pstToken = NULL;
            char *pstWork = os_strdup(str);
            char *pstState = NULL;

            //compute size of outputs array
            for (pstToken = os_strtok(pstWork, delim, &pstState);
                    pstToken != NULL;
                    pstToken = os_strtok(NULL, delim, &pstState), (*sizeoutputs)++)
            {
                ;
            }

            if (*sizeoutputs == 0)
            {
                FREE(pstWork);
                return NULL;
            }

            //alloc output array
            outputs = (char**)MALLOC(sizeof(char*) * *sizeoutputs);

            FREE(pstWork);
            pstWork = os_strdup(str);
            for (pstToken = os_strtok(pstWork, delim, &pstState);
                    pstToken != NULL;
                    pstToken = os_strtok(NULL, delim, &pstState), i++)
            {
                outputs[i] = os_strdup(pstToken);
            }

            FREE(pstWork);
        }
    }
    return outputs;
}
/*--------------------------------------------------------------------------*/
