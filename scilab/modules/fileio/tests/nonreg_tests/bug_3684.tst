// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 3684 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3684
//
// <-- Short Description -->
// mget() skips binary data.

ref = [ 53  51  13 ];

fid = mopen('SCI/modules/fileio/tests/nonreg_tests/'+'bug_3684.txt','rb');
data = mgeti(512,'uc',fid);
mclose(fid);

if or(ref <> data(159:161) ) then pause,end
