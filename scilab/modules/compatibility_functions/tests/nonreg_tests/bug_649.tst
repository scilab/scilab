// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 649 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=649
//
// <-- Short Description -->
//    scilab crashes under windows, under linux, Scilab loops forever,
//    and it uses the whole CPU resource.

x = rand(5,5,100)+%i;
warning('off');
ierr = execstr('mtlb_save SCI/modules/compatibility_functions/nonreg_tests/bug_649.mat x','errcatch');
warning('on');
if ierr <> 10000 then pause,end;
