// <-- Non-regression test for bug 1802 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=1802
//
// <-- Short Description -->
//    The sci2exp function is very very slow for large matrices


// Non-regression test file for bug 1802
// Copyright INRIA
// Scilab Project - Serge Steer
// Copyright INRIA 2005
// Date : 30 janvier 2006

a=rand(100,500);
timer();sci2exp(a);t=timer();
affich_result(t<60,1802);
