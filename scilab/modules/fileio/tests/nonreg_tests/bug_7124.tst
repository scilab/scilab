// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 7124 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7124
//
// <-- Short Description -->
// mputl returned a wrong error message if file does not exist.
//
ierr = execstr("mputl(""ABC"", ""xzwz\names"")", "errcatch");
if ierr <> 999 then pause, end
if lasterror() <> strsubst(msprintf(_("%s: Cannot open file %s.\n"), "mputl", "xzwz/names"), "/", "\") then pause, end


