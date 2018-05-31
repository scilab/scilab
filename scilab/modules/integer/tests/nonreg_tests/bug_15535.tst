// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// ===========================================================================
//
// <-- Non-regression test for bug 15535 -->
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15535
//
// <-- Short Description -->
// sign(int8(0)) returned 1 instead of 0

for it = [1 2 4 8 11 12 14 18]
    assert_checkequal(sign(iconvert(0, it)), iconvert(0,it));
    assert_checkequal(sign(iconvert(4, it)), iconvert(1,it));
    if it<10
        assert_checkequal(sign(iconvert(-4, it)), iconvert(-1,it));
    end
end
