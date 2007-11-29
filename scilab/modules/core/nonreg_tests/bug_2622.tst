// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 2622 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=2622
//
// <-- Short Description -->
//
// errcatch(n,'pause') and errcatch(n,'continue') doesn't display the message
// anymore

// Pierre MARECHAL - Scilab Project
// Copyright INRIA
// 29 novembre 2007

errcatch(20,'pause');
error(20,2);
