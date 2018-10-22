// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15718 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15718
//
// <-- Short Description -->
// bitwise functions bitand, bitor, and bitxor misworked with uint64(%inf)

i = uint64(%inf);
assert_checkequal(bitand(i,i),i);
assert_checkequal(bitxor(i,i), uint64(0));

IT = [1 2 4 8 11 12 14 18];
for it = IT
    two = iconvert(2,it);
    assert_checkequal(bitand(i, two), uint64(2));
    assert_checkequal(bitxor(i, two), i-two);
    // assert_checkequal(bitand(two, i), uint64(2));  // was not bugged
    // assert_checkequal(bitxor(two, i), i-two);      // was not bugged
end

j = uint64(2).^63-1;
assert_checkequal(bitand(j,j),j);
assert_checkequal(bitor(j,j),j);
assert_checkequal(bitxor(j,j), uint64(0));
