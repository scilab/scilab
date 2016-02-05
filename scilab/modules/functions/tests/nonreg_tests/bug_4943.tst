// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 4943 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4943
//
// <-- Short Description -->
// execstr does not check input arguments.
// =============================================================================
ierr = execstr("a=zzzzzzz","errcatch","n");
// =============================================================================
ierr = execstr("ierr = execstr(''a=zzzzzzz'',''errcatch'',''nooon'');","errcatch");
if ierr <> 999 then pause,end
// =============================================================================
ierr = execstr("a=zzzzzzz","errcatch");
if ierr <> 999 then pause,end
// =============================================================================
ierr = execstr("ierr = execstr(''a=zzzzzzz'',''errBADcatch'');","errcatch");
if ierr <> 999 then pause,end
// =============================================================================
ierr = execstr("ierr = execstr(''a=zzzzzzz'',''errBADcatch'',''n'');","errcatch");
if ierr <> 999 then pause,end
// =============================================================================
