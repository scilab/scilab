/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU
 * Copyright (C) INRIA - 2008 - Allan CORNET
 * Copyright (C) Digiteo - 2011 - Cedric DELAMARRE
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

/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "sci_malloc.h"
#include "tokens.h"
#include "os_string.h"
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
                FREE(pwstWork);
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
