// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// Unit tests for mixed types concatenations
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// Booleans and encoded integers
for i = [1 2 4 8 11 12 14 18]  // loop on inttype codes
    r = [iconvert(-3,i) , %t];
    r2 = [%t , iconvert(-3,i)];
    c2 = [%t ; iconvert(-3,i)];
    c = [iconvert(-3,i) ; %t];
    assert_checkequal(inttype(r), i);
    assert_checkequal(inttype(r2), i);
    assert_checkequal(inttype(c), i);
    assert_checkequal(inttype(c2), i);
    assert_checkequal(size(r) , [1 2]);
    assert_checkequal(size(r2), [1 2]);
    assert_checkequal(size(c) , [2 1]);
    assert_checkequal(size(c2), [2 1]);
end
