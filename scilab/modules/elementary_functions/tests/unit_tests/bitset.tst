// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

assert_checkequal(bitset(uint8(9), 5), uint8(25));
assert_checkequal(bitset(uint8(25), 5, 0), uint8(9));
assert_checkequal(bitset(25, 5, 0), 9);

assert_checkerror("bitset(2)",[],10000);
assert_checkerror("bitset(""aze"")",[],10000);

assert_checkerror("bitset(-25, 5)",[],10000);
a=[170,82,24,89,92,59,220,141];
assert_checkequal(bitset(a, ones(1:8)), [171,83,25,89,93,59,221,141]);
