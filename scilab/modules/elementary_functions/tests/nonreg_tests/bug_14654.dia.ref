// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 14654 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14654
//
// <-- Short Description -->
//    bitor(), bitxor() and bitand() did not accept int8, int16, int32, int64
//    and uint64 positive integers
icodes = [1 2 4 8 18];
for i = icodes
    a = iconvert(57,i);
    assert_checktrue(execstr("bitor(a,a)", "errcatch")==0);
    assert_checktrue(execstr("bitxor(a,a)", "errcatch")==0);
    assert_checktrue(execstr("bitand(a,a)", "errcatch")==0);
end
