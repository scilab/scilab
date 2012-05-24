// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 3679 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3679
//
// <-- Short Description -->
// mfscanf() returns numbers with slight errors using 5.0.x on Windows.

test_path = get_absolute_file_path('bug_3679.tst');

fid = mopen('SCI/modules/fileio/tests/nonreg_tests/'+'bug_3679.txt','rt');

for i=1:6
  [n,y(i)]=mfscanf(fid, '%lg');  
end

mclose(fid);

ref = [22.631	23.021	24.280	23.551	22.974	23.309]';
if or(ref<>y) then pause,end
