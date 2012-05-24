// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 4846 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4846
//
// mopen("","") crashs on windows

ierr = execstr("fd = mopen("""","""");","errcatch");
if ierr <> 999 then pause,end

ierr = execstr("fd = mopen("""",""e"");","errcatch");
if ierr <> 999 then pause,end

ierr = execstr("fd = mopen("""",""z"");","errcatch");
if ierr <> 999 then pause,end
