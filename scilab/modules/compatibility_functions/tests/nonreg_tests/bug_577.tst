// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 577 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=577
//
// <-- Short Description -->
//    It can't save imaginary in MATLAB format (SCILAB 2.7.x and
//    cvs version)
//
//    I can reproduce the bug with integers data type not with imaginary

my_mat_file = TMPDIR+"/bug_577.mat";

x=rand(5,5,100)+%i;
if (execstr('mtlb_save(my_mat_file,''x'')','errcatch')) <> 0 then pause,end
