// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 7601 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7601
//
// <-- Short Description -->
// call_scilab C functions did not check if engine is started.

// Create a C code to use call_scilab:

int main(void)
{
#ifdef _MSC_VER
        StartScilab(NULL, NULL, NULL);
#else
        StartScilab(getenv("SCI"), NULL, NULL);
#endif

        SendScilabJob("disp([2,3]+[-44,39]);"); // Will display   - 42.    42.  
        TerminateScilab(NULL);
        return SendScilabJob("disp([2,3]+[-44,39]);")
        
}

default value returned by main will be -1