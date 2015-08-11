// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 8830 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8830
//
// <-- Short Description -->
// In call_scilab, TerminateScilab  did not clear the last error.

// export SCI=/your/scilab/install/share/scilab
// export LD_LIBRARY_PATH=/your/scilab/install/lib/scilab/:/your/scilab/install/lib/thirdparty/
// make bug_8830
// ./bug_8830

// "getLastErrorValue 0" must be displayed
