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

// 2009-01-09 mtlb_save is removed => savematfile

my_mat_file = TMPDIR+"/bug_577.mat";

x=rand(5,5,100)+%i;
warning('off');
ierr = execstr("savematfile(my_mat_file,""x"")","errcatch");
warning('on');

if ierr == 0 then pause,end
if lasterror()<>msprintf(gettext("%s: Mlists of type hm can not be written to Matlab binary files.\n"), "GetMlistVariable") then pause,end
