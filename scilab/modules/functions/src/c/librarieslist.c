/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <string.h>
#include "Scierror.h"
#include "api_scilab.h"
#include "stack-c.h"
#include "librarieslist.h"
#include "stackinfo.h"
#include "MALLOC.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
static int getnumberoflibraries(void);
/*--------------------------------------------------------------------------*/
char **getlibrarieslist(int *sizearray)
{
    char **librarieslist = NULL;
    int nbElements = getnumberoflibraries();

    if (nbElements > 0)
    {
        librarieslist = (char**)MALLOC(sizeof(char*) * (nbElements));
        if (librarieslist)
        {
            int Lused = 0;
            int Ltotal = 0;
            int j = 0;
            int i = 0;

            C2F(getvariablesinfo)(&Ltotal, &Lused);

            for (j = 1; j < Lused + 1; ++j)
            {
                char * NameVariable = getLocalNamefromId(j);
                int * header = NULL;
                SciErr sciErr = getVarAddressFromName(pvApiCtx, NameVariable, &header);
                if (!sciErr.iErr)
                {
                    if (header && (header[0] == sci_lib))
                    {
                        librarieslist[i] = strdup(NameVariable);
                        i++;
                    }
                }
                if (NameVariable)
                {
                    FREE(NameVariable);
                    NameVariable = NULL;
                }
            }
            *sizearray = i;
        }
        else
        {
            *sizearray = 0;
        }
    }
    else
    {
        *sizearray = 0;
    }
    return librarieslist;
}
/*--------------------------------------------------------------------------*/
static int getnumberoflibraries(void)
{
    int nbrlibraries = 0;
    int Lused = 0;
    int Ltotal = 0;
    int j = 0;

    C2F(getvariablesinfo)(&Ltotal, &Lused);

    for (j = 1; j < Lused + 1; ++j)
    {
        char *NameVariable = getLocalNamefromId(j);
        int * header = NULL;
        SciErr sciErr = getVarAddressFromName(pvApiCtx, NameVariable, &header);

        if (!sciErr.iErr)
        {
            if (header && (header[0] == sci_lib))
            {
                nbrlibraries++;
            }
        }
        if (NameVariable)
        {
            FREE(NameVariable);
            NameVariable = NULL;
        }
    }
    return nbrlibraries;
}
/*--------------------------------------------------------------------------*/
