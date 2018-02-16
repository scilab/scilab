// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 15182 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15182
//
// <-- Short Description -->
// a .*. b or kron(a,b) returned floats instead of encoded integers when a or b
//  is an encoded integer and the other a decimal

a = [1 2 ; 3 4];
b = [-1 2.3];

it = [1 2 4 8];
for i = it
    assert_checkequal(inttype(iconvert(a,i).*.b), i);
    assert_checkequal(inttype(a.*.iconvert(b,i)), i);
end
