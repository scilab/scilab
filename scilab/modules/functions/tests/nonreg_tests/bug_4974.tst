// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 4974 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4974
//
// <-- Short Description -->
// exec('') crashs scilab
// =============================================================================
ierr = execstr("exec('''',-1);","errcatch");
if ierr == 0 then pause,end
ierr = execstr("exec('''');","errcatch");
if ierr == 0 then pause,end
// =============================================================================
