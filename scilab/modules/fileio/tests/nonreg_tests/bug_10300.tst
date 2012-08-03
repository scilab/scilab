// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- LONG TIME EXECUTION --> 
//
// <-- Non-regression test for bug 10300 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10300
//
// <-- Short Description -->
// fscanfMat did not free some memory
//

a = ones(1000, 1000);
fprintfMat(TMPDIR + '/bug_10300_file.txt',a);
clear a;

for i = 1:1000
  disp(i)
  a = fscanfMat(TMPDIR + '/bug_10300_file.txt');
end
