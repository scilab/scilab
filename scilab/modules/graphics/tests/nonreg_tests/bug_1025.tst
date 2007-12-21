// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 1025 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=1025
//
// <-- Short Description -->
//    winsid ne marche pas.


// Non-regression test file for bug 1025
// Pierre MARECHAL - Scilab Project
// Copyright INRIA
// 1er juin 2005

set figure_style old;
xset('window',1000);
set figure_style old;
xset('window',0);
set figure_style new;

A = winsid();

if or(A <> [0,1000]) then pause,end
