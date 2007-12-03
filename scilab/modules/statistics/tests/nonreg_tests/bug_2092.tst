// <-- Non-regression test for bug 2092 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=2092
//
// <-- Short Description -->
//    Incorrect handling of second input argument to function mad()

// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Copyright INRIA 2006
// Date : 17 aout 2006

A = mad([1:10;2:11],2);
B = mad([1:10;2:11],'c');

if or(A <> B) then pause,end
