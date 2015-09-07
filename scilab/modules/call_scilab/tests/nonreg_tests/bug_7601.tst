// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
// Copyright (C) 2015 - Scilab-Enterprises - Cedric Delamarre
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

// export SCI=/your/scilab/install/share/scilab
// export LD_LIBRARY_PATH=/your/scilab/install/lib/scilab/:/your/scilab/install/lib/thirdparty/
// make bug_7601
// ./bug_7601

// display   - 42.    42.

// default value returned by main will be -1
