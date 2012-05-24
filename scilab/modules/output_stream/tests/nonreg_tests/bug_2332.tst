// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2332 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2332
//

ierr = execstr("mprintf(""a: %2.0f\tb: %3.4f"",10)","errcatch");
if ierr <> 999 then pause,end

ierr = execstr("mprintf(""a: %2.0f"")","errcatch");
if ierr <> 999 then pause,end

