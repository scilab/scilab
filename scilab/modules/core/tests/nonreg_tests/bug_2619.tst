// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 2619 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=2619
//
// <-- Short Description -->
//
// errcatch with the "stop" action is broken.
//
// man page : "errcatch(-1,'stop')" : interrupts the current Scilab session (useful
// when Scilab is called from an external program).
//
// => It works well with scilab-4.1.2 but with the trunk version, scilab is not
// closed and the error not displayed. After that, scilab is broken and doesn't
// display results anymore
//
//
// Steps to reproduce the bug:
//
// errcatch(-1,'stop');
// error(10000);


// Pierre MARECHAL - Scilab Project
// Copyright INRIA
// 29 novembre 2007

errcatch(-1,'stop');
error(10000);
