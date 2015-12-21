// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 14038 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14038
//
// <-- Short Description -->
// Encoded integers are no longer accepted for list extraction

L=list(%pi,12,%s,14);
// Signed, one input argument
assert_checkequal(L(int8(1)), L(1));
assert_checkequal(L(int16(1)), L(1));
assert_checkequal(L(int32(1)), L(1));
assert_checkequal(L(int64(1)), L(1));
// Unsigned, one input argument
assert_checkequal(L(uint8(1)), L(1));
assert_checkequal(L(uint16(1)), L(1));
assert_checkequal(L(uint32(1)), L(1));
assert_checkequal(L(uint64(1)), L(1));

[ref1, ref2] = L([1, 2]);
// Signed, two input arguments
[res1, res2] = L([int8(1), int8(2)]);
assert_checkequal(res1, ref1);
assert_checkequal(res2, ref2);
[res1, res2] = L([int16(1), int16(2)]);
assert_checkequal(res1, ref1);
assert_checkequal(res2, ref2);
[res1, res2] = L([int32(1), int32(2)]);
assert_checkequal(res1, ref1);
assert_checkequal(res2, ref2);
[res1, res2] = L([int64(1), int64(2)]);
assert_checkequal(res1, ref1);
assert_checkequal(res2, ref2);
// Unsigned, two input arguments
[res1, res2] = L([uint8(1), uint8(2)]);
assert_checkequal(res1, ref1);
assert_checkequal(res2, ref2);
[res1, res2] = L([uint16(1), uint16(2)]);
assert_checkequal(res1, ref1);
assert_checkequal(res2, ref2);
[res1, res2] = L([uint32(1), uint32(2)]);
assert_checkequal(res1, ref1);
assert_checkequal(res2, ref2);
[res1, res2] = L([uint64(1), uint64(2)]);
assert_checkequal(res1, ref1);
assert_checkequal(res2, ref2);
