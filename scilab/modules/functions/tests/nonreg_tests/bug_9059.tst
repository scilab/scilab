// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 9059 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9059
//
// <-- Short Description -->
// tbx_build_macros and genlib do not stop on error

cd TMPDIR;
mkdir("bug_9059");

f = ["function [t] = macrobug(z)"; "t=z/"; "endfunction"];
mputl(f,TMPDIR+"/bug_9059/macrobug.sci");

cd("bug_9059");
exec("macrobug.sci","errcatch");

assert_checkerror("genlib(""bug_9059lib"",""TMPDIR/bug_9059"",%f,%t)",[], 999);
