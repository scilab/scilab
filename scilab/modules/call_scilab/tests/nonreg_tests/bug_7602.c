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
// <-- Non-regression test for bug 7602 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7602
//
// <-- Short Description -->
// it was not possible to start/close a Scilab engine at anytime.

// Create a C code to use call_scilab:

#include <stdlib.h>
#include <stdio.h>
#include "call_scilab.h"

int main(void)
{
#define NB_LOOPS 10
    int i = 0;
    for (i = 0; i < NB_LOOPS; i++)
    {
        DisableInteractiveMode();
#ifdef _MSC_VER
        StartScilab(NULL, NULL, 0);
#else
        StartScilab(getenv("SCI"), NULL, 0);
#endif
        TerminateScilab(NULL);

        DisableInteractiveMode();
#ifdef _MSC_VER
        StartScilab(NULL, NULL, 0);
#else
        StartScilab(getenv("SCI"), NULL, 0);
#endif
        SendScilabJob("disp([2,3]+[-44,39]);"); // Will display   - 42.    42.
        TerminateScilab(NULL);
    }
    return 0;
}
