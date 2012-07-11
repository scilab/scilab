// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2639 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2639
//
// <-- Short Description -->
//    'ls' function is crashing when trying to list in an empty directory

cd(TMPDIR);
mkdir(TMPDIR+'/test_bug2639');
files_1 = ls(TMPDIR+'/test_bug2639');
cd(TMPDIR+'/test_bug2639');
files_2 = ls();

if files_1 <> [] | files_2 <> [] then pause,end

