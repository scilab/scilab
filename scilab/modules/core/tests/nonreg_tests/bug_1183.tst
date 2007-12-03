// <-- Non-regression test for bug 1183 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=1183
//
// <-- Short Description -->
//    @?

// Pierre MARECHAL - Scilab Project
// Copyright INRIA
// 1er juin 2005

res = execstr("A = 1@1;","errcatch");
B   = lasterror();

if B <> "Missing operator, comma, or semicolon" then pause,end
