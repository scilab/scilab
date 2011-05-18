// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 6789 -->
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/6789
//
// <-- Short Description -->
// toprint(["line 1" "line 2"],"Page header"): The page header was not printed.

// executes this line in scilab :
toprint(["line 1" "line 2"],"Page header")

// and checks that header is on printed page.
