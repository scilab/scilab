// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
// Copyright (C) 2015 - Scilab-Enterprises - Cedric Delamarre
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 8115 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8115
//
// <-- Short Description -->
// DisableInteractiveMode did not work

// Create a C code to use call_scilab:

#ifdef _MSC_VER
#pragma comment(lib, "call_scilab.lib")
#pragma comment(lib, "libscilab.lib")
#endif
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "call_scilab.h"
#include "configvariable_interface.h"
/*--------------------------------------------------------------------------*/
int main(void)
{
    int iErr = 0;
    if (getScilabMode() != SCILAB_API)
    {
        fprintf(stderr, "BUG 8115 NOT FIXED. (1)\n");
        iErr = 1;
    }

    DisableInteractiveMode();
#ifdef _MSC_VER
    StartScilab(NULL, NULL, 0);
#else
    StartScilab(getenv("SCI"), NULL, 0);
#endif

    /* check that it is NWNI mode */
    if (getScilabMode() != SCILAB_NWNI)
    {
        fprintf(stderr, "BUG 8115 NOT FIXED. (2)\n");
        iErr = 1;
    }

    TerminateScilab(NULL);

    /* check that we returns to default mode */
    if (getScilabMode() != SCILAB_API)
    {
        fprintf(stderr, "BUG 8115 NOT FIXED. (3)\n");
        iErr = 1;
    }

#ifdef _MSC_VER
    StartScilab(NULL, NULL, 0);
#else
    StartScilab(getenv("SCI"), NULL, 0);
#endif

    /* check that it is API mode */
    if (getScilabMode() != SCILAB_API)
    {
        fprintf(stderr, "BUG 8115 NOT FIXED. (4)\n");
        iErr = 1;
    }

    TerminateScilab(NULL);

    /* check that we returns to default mode */
    if (getScilabMode() != SCILAB_API)
    {
        fprintf(stderr, "BUG 8115 NOT FIXED. (5)\n");
        iErr = 1;
    }

    return iErr;
}
/*--------------------------------------------------------------------------*/
