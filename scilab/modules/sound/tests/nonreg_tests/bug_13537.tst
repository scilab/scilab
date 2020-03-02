// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 13537 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13537
//
// <-- Short Description -->
// wavwrite(..) yielded EXCEPTION_ACCESS_VIOLATION error
//
s = ones([1:12345678])/2;
filename = TMPDIR+"/test.wav";
assert_checkequal(execstr("wavwrite(s, 44100, 16, filename);","errcatch"), 0);
sleep(100)
mdelete(filename);
