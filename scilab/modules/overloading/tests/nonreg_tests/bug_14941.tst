//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 14941 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14941
//
// <-- NO CHECK REF -->  // no reason to test the display
//
// <-- Short Description -->
// find() did not accept encoded integers

m = grand(3,5,"uin",1,10);
for i = [1 2 4 8 11 12 14 18]
    mi = iconvert(m,i);
    assert_checkequal(find(mi), 1:15);
end
