// <-- Non-regression test for bug 2656 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=2656
//
// <-- Short Description -->
//    M2sci doesn't convert "carriage return" inside a loop.

// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Copyright INRIA
// Date : 15 janvier 2008

if stripblanks("    ") <> "" then pause,end
