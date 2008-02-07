// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 2623 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=2623
//
// <-- Short Description -->
//
// If an error "n" is catch with an errcatch(-1,'pause'), the only way to quit
// the pause mode is abort. "resume" and "return" don't seem to 
// work

// <-- INTERACTIVE TEST -->

// Pierre MARECHAL - Scilab Project
// Copyright INRIA
// 07/02/2008


errcatch(-1,'pause');
error(1);
resume();

// enter
// enter

