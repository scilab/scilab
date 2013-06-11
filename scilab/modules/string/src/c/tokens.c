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
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "MALLOC.h"
#include "tokens.h"
#include "os_wcsdup.h"
#include "os_wcstok.h"


/*------------------------------------------------------------------------*/
wchar_t** stringTokens(wchar_t* str, wchar_t* delim, int* sizeOutputs)
{
    wchar_t **Outputs = NULL;
    *sizeOutputs = 0;
    if (str)
    {
        if (delim)
        {
            int i = 0;
            wchar_t *pwstToken = NULL;
            wchar_t *pwstWork = os_wcsdup(str);
            wchar_t *pwstState = NULL;

            //compute size of outputs array
            for (pwstToken = os_wcstok(pwstWork, delim, &pwstState);
                    pwstToken != NULL;
                    pwstToken = os_wcstok(NULL, delim, &pwstState), (*sizeOutputs)++)
            {
                ;
            }

            if (*sizeOutputs == 0)
            {
                return NULL;
            }

            //alloc output array
            Outputs = (wchar_t**)MALLOC(sizeof(wchar_t*) * *sizeOutputs);

            FREE(pwstWork);
            pwstWork = os_wcsdup(str);
            for (pwstToken = os_wcstok(pwstWork, delim, &pwstState);
                    pwstToken != NULL;
                    pwstToken = os_wcstok(NULL, delim, &pwstState), i++)
            {
                Outputs[i] = os_wcsdup(pwstToken);
            }

            FREE(pwstWork);
        }
    }
    return Outputs;
}
/*--------------------------------------------------------------------------*/
