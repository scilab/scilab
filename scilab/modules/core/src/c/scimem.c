/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
#ifdef USE_DYNAMIC_STACK
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
#ifdef USE_DYNAMIC_STACK
    return scimem64(ptr, *newsize, FALSE);
#else
    register char *p1 = NULL;

    if (*newsize > 0)
    {
        /* add 1 for alignment problems */
        /* the last +2 is to overcome a problem with adjuststack. */
        /* Which appears on OpenSolaris and on mandriva + EEEPC. */
        /* To be corrected. Thanks Jonathan */
        p1 = (char *)SCISTACKMALLOC(((unsigned long)sizeof(double)) * (*newsize + 1 + 2));

        if (p1 != NULL)
        {
            the_ps = the_p;
            the_p = p1;
            /* add 1 for alignment problems */
            *ptr = ((int)(the_p - (char *)C2F(stack).Stk)) / sizeof(double) + 1;
        }
        else
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
#ifdef USE_DYNAMIC_STACK
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
void C2F(freegmem) (void)
{
#ifdef USE_DYNAMIC_STACK
    freemem64(TRUE);
#else
    if (the_gps != NULL)
        SCISTACKFREE(the_gps);
#endif
}

/*--------------------------------------------------------------------------*/
void C2F(freemem) (void)
{
#ifdef USE_DYNAMIC_STACK
    freemem64(FALSE);
#else
    if (the_ps != NULL)
        SCISTACKFREE(the_ps);
#endif
}

/*--------------------------------------------------------------------------*/
