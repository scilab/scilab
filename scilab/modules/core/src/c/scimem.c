/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include <stdlib.h>
#include "MALLOC.h"
#include "stack-def.h"
#include "scimem.h"
#include "sciprint.h"
#include "localization.h"
#include "BOOL.h"
/*--------------------------------------------------------------------------*/
#ifndef _MSC_VER
extern int scimem64(int *n, int newsize, BOOL isglobal);
extern void freemem64(BOOL isglobal);
#else
static char *the_p = NULL;
static char *the_ps = NULL;
static char *the_gp = NULL;
static char *the_gps = NULL;
#endif
/*--------------------------------------------------------------------------*/
int C2F(scimem) (int *newsize, int *ptr)
{
#ifndef _MSC_VER
    return scimem64(ptr, *newsize, FALSE);
#else
    register char *p1 = NULL;
    double coef = 1.0;

    if (*newsize > 0)
    {
        /* add 1 for alignment problems */
        /* the last +2 is to overcome a problem with adjuststack. */
        /* Which appears on OpenSolaris and on mandriva + EEEPC. */
        /* To be corrected. Thanks Jonathan */
        while (coef > 0)
        {
            int size = (int)(*newsize * coef);
            p1 = (char *)SCISTACKMALLOC((size + 1 + 2) * sizeof(double));
            if (p1 != NULL)
            {
                the_ps = the_p;
                the_p = p1;
                /* add 1 for alignment problems */
                *ptr = ((int)(the_p - (char *)C2F(stack).Stk)) / sizeof(double) + 1;
                *newsize = size;
                //sciprint("SUCCEED to alloc %0.0f MB ( %0.0f%% )\n", (double)(size + 1 + 2) * sizeof(double) / (1024*1024), coef * 100);
                break;
            }
            else
            {
                //sciprint("FAILED to alloc %0.0f MB\n", (double)(size + 1 + 2) * sizeof(double) / (1024*1024));
                coef -= 0.01; //remove 1%
            }
        }

        if (p1 == NULL)
        {
            /* We could not create a new stack, so, we are using the previous one */
            if (the_p == NULL)
            {
                /* The stack never been created */
                sciprint(_("No space to allocate Scilab stack.\n"));
                exit(1);
            }

            *ptr = 0;
        }

    }
    return (0);
#endif
}
/*--------------------------------------------------------------------------*/
int C2F(scigmem) (int *n, int *ptr)
{
#ifndef _MSC_VER
    return scimem64(ptr, *n, TRUE);
#else
    register char *p1 = NULL;

    if (*n > 0)
    {
        /* add 1 for alignment problems */
        /* YC: Same comment as above */
        p1 = (char *)SCISTACKMALLOC((unsigned)sizeof(double) * (*n + 1 + 2));
        if (p1 != NULL)
        {
            the_gps = the_gp;
            the_gp = p1;
            /* add 1 for alignment problems */
            *ptr = ((int)(the_gp - (char *)C2F(stack).Stk)) / sizeof(double) + 1;
        }
        else
        {
            if (the_gp == NULL)
            {
                sciprint(_("No space to allocate Scilab stack.\n"));
                exit(1);
            }
            *ptr = 0;
        }
    }
    return (0);
#endif
}

/*--------------------------------------------------------------------------*/
void C2F(freeglobalstacklastmemory) (void)
{
#ifdef _MSC_VER
    if (the_gps != NULL)
    {
        SCISTACKFREE(the_gps);
    }
    the_gps = NULL;
#endif
}

/*--------------------------------------------------------------------------*/
void C2F(freestacklastmemory) (void)
{
#ifdef _MSC_VER
    if (the_ps != NULL)
    {
        SCISTACKFREE(the_ps);
    }
    the_ps = NULL;
#endif
}

/*--------------------------------------------------------------------------*/
void freeGlobalStackCurrentMemory()
{
#ifndef _MSC_VER
    freemem64(TRUE);
#else
    if (the_gp != NULL)
    {
        SCISTACKFREE(the_gp);
    }
    the_gp = NULL;
#endif
}

void freeStackCurrentMemory()
{
#ifndef _MSC_VER
    freemem64(FALSE);
#else
    if (the_p != NULL)
    {
        SCISTACKFREE(the_p);
    }
    the_p = NULL;
#endif
}
