// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- Non-regression test for bug 6549 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6549
//
// <-- Short Description -->
// genlib function had a behavior which was not consistent with the load function.
// =============================================================================
// <-- CLI SHELL MODE -->
f = [
"function titi()"
"  disp(''titi'');"
"endfunction"
""
"function tata()"
"  disp(''tata'');"
"endfunction"
""
"function toto()"
"  disp(''toto'');"
"endfunction"
""
];
cd TMPDIR;
mkdir("bug_6549");
cd("bug_6549");
mputl(f,"toto.sci");
if genlib("bug_6549lib",TMPDIR + "/bug_6549") <> %t then pause,end;
// =============================================================================
ierr = execstr("toto","errcatch");
if ierr <> 0 then pause,end
ierr = execstr("titi","errcatch");
if ierr == 0 then pause,end
ierr = execstr("tata","errcatch");
if ierr == 0 then pause,end
clear bug_6549lib titi toto tata ierr;
// =============================================================================
load(TMPDIR + "/bug_6549/lib");
ierr = execstr("toto","errcatch");
if ierr <> 0 then pause,end
ierr = execstr("titi","errcatch");
if ierr == 0 then pause,end
ierr = execstr("tata","errcatch");
if ierr == 0 then pause,end
clear bug_6549lib titi toto tata ierr;
// =============================================================================
bug_6549lib = lib(TMPDIR + "/bug_6549");
ierr = execstr("toto","errcatch");
if ierr <> 0 then pause,end
ierr = execstr("titi","errcatch");
if ierr == 0 then pause,end
ierr = execstr("tata","errcatch");
if ierr == 0 then pause,end
clear bug_6549lib titi toto tata ierr;
// =============================================================================
