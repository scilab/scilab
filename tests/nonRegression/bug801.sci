// <-- Non-regression test for bug 801 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=801
//
// <-- Short Description -->
//    error in the routine covar.sci

// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Date : 3 mai 2005

x=[4 5];
y=[3 4 5 6];
m=[ 2 3 4 5; 5 4 3 2];

correct=%F

if (covar(x,y,m) <> -0.1785714) then
  correct=%T
end

affich_result(correct,801);
