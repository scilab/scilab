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
// <-- Non-regression test for bug 16522 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16522
//
// <-- Short Description -->
// bitget(x,pos) and bitset(x,pos) results could be wrong when pos is an
// encoded integer.
// Ex: bitget(2^(n*8-1), uint8(n*8)) with n in [1 2 4 8] returned 0 instead of 1

for cast = list(int8, int16, int32, int64, uint8, uint16, uint32, uint64)
    assert_checkequal(bitget(255,  cast(8)), 1);
    assert_checkequal(bitget(2^15, cast(16)), 1);
    assert_checkequal(bitset(0,  cast(8)), 2^7);
    assert_checkequal(bitset(0, cast(16)), 2^15);
end
