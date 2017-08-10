
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
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

/*--------------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include "men_Sutils.h"
#include "getfastcode.h"
#include "sci_malloc.h"             /* MALLOC */
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
void ScilabStr2C(int *n, int *Scistring, char **strh, int *ierr)
{
    int j = 0;

    *strh = (char *)MALLOC((*n + 1) * sizeof(char));
    if ((*strh) == NULL)
    {
        *ierr = 1;
        return;
    }

    for (j = 0; j < *n; ++j)
    {
        (*strh)[j] = convertScilabCodeToAsciiCode(Scistring[j]);
    }
    (*strh)[*n] = '\0';
}

/*--------------------------------------------------------------------------*/
void ScilabMStr2CM(int *Scistring, int *nstring, int *ptrstrings, char ***strh, int *ierr)
{
    char **strings = NULL, *p = NULL;
    int li = 0, ni = 0, *SciS = NULL, i = 0;

    strings = (char **)MALLOC(((*nstring) + 1) * sizeof(char *));

    if (strings == NULL)
    {
        *ierr = 1;
        return;
    }

    li = 1;
    SciS = Scistring;
    for (i = 1; i < *nstring + 1; i++)
    {
        ni = ptrstrings[i] - li;
        li = ptrstrings[i];
        /* p is allocated here */
        ScilabStr2C(&ni, SciS, &p, ierr);
        if (*ierr == 1)
        {
            if (p)
            {
                FREE(p);
            }

            freeArrayOfString(strings, i - 1);
            return;
        }
        strings[i - 1] = p;
        SciS += ni;
    }
    strings[*nstring] = NULL;
    *strh = strings;
}

/*--------------------------------------------------------------------------*/
void ScilabMStr2C(int *desc, int *nd, int *ptrdesc, char **strh, int *ierr)
{
    int ln = 0, i = 0;
    int *SciS = NULL;
    char *description = NULL;

#ifdef _MSC_VER
    ln = ptrdesc[*nd] + 2 * (*nd) + 1;
#else
    ln = ptrdesc[*nd] + *nd + 1;
#endif

    description = (char *)MALLOC(ln * sizeof(char));

    if (description == NULL)
    {
        *ierr = 1;
        return;
    }

    SciS = desc;

    for (i = 1; i < *nd + 1; i++)
    {
        int j = 0;
        int ni = 0;
        int li = 1;
        int di = 0;
        char *p = NULL;

        p = &(description[di]);
        ni = ptrdesc[i] - li;

        /* conversion code -> ascii */
        for (j = 0; j < ni; ++j)
        {
            p[j] = convertScilabCodeToAsciiCode(SciS[j]);
        }
        SciS += ni;
#ifdef _MSC_VER
        p[ni] = '\r';
        ni++;
#endif
        p[ni] = '\n';
    }
    description[ln - 2] = '\0';
    *strh = description;
}

/*--------------------------------------------------------------------------*/
