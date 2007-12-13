// <-- Non-regression test for bug 2639 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=2639
//
// <-- Short Description -->
//    'ls' function is crashing when trying to list in an empty directory

// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Copyright INRIA 2007
// Date : 13 dec. 2007

cd(TMPDIR);
mkdir(TMPDIR+'/test_bug2639');
files_1 = ls(TMPDIR+'/test_bug2639');
cd(TMPDIR+'/test_bug2639');
files_2 = ls();

if files_1 <> [] | files_2 <> [] then pause,end

