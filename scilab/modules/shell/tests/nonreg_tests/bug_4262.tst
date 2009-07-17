// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4262 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4262
//
// <-- Short Description -->
// lines() on master is broken

// <-- INTERACTIVE TEST -->

// launch scilab -nw mode
lines()
// checks that values are something as 80 28 (on windows) and not 28 80.
