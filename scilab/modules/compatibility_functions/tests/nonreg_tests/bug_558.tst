// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 558 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=558
//
// <-- Short Description -->
//    mtlb_save reports an unnecessary warning. This is due to
//    line 57 of  SCI/macros/mtlb/mtlb_save.sci:
//
//      clear('rhs','lhs','kk','k','err','mtlb_thefile','or');
//
//    the last 'or' looks superfluous to me.

// <-- INTERACTIVE TEST -->

fileName = TMPDIR+"/bug558.txt";
diary(fileName);
x = 20;
mtlb_save('bug558','x')
diary(0);

w=fileinfo(fileName);
if w(1) == 0  then pause,end

mdelete(fileName);
mdelete('bug558.mat');
