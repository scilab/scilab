// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// ===========================================================================
//
// <-- Non-regression test for bug 7641 -->
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/7641
//
// <-- Short Description -->
// `uint8(1):uint16(4)` produced an error instead of being parsed as
// `uint8(1):1:uint16(4)`, that passes.

b = [1 2 4 8 11 12 14 18];
for i = b
    for j = b
        assert_checkequal(iconvert(1,i):iconvert(4,j), iconvert(1:4,i));
    end
end
