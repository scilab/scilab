// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// ===========================================================================
//
// <-- Non-regression test for bug 12928 -->
//
// <-- CLI SHELL MODE -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/12928
//
// <-- Short Description -->
// int32, int16 and int8 with %nan and %inf

//int8
ref = int8([-128 -128  0  127 -128]);
computed = int8([2^7, -2^7, %nan, %inf, -%inf]);
assert_checkequal(computed, ref);

//uint8
ref = uint8([0, 255, 0, 255, 0]);
computed = uint8([2^8, -1, %nan, %inf, -%inf]);
assert_checkequal(computed, ref);

//int16
ref = int16([-32768 -32768  0  32767 -32768]);
computed = int16([2^15, -2^15, %nan, %inf, -%inf]);
assert_checkequal(computed, ref);

//uint16
ref = uint16([0  65535  0  65535  0]);
computed = uint16([2^16, -1, %nan, %inf, -%inf]);
assert_checkequal(computed, ref);

//int32
ref = int32([-2147483648 -2147483648  0  2147483647 -2147483648]);
computed = int32([2^31, -2^31, %nan, %inf, -%inf]);
assert_checkequal(computed, ref);

//uint32
ref = uint32([0  4294967295  0  4294967295  0]);
computed = uint32([2^32, -1, %nan, %inf, -%inf]);
assert_checkequal(computed, ref);

//int64
ref = int64([-9223372036854775808 -9223372036854775808  0  9223372036854775807 -9223372036854775808]);
computed = int64([2^63, -2^63, %nan, %inf, -%inf]);
//assert_checkequal(computed, ref);

//uint64
ref = uint64([9223372036854775808  18446744073709551615  0  18446744073709551615  0]);
computed = uint64([2^64, -1, %nan, %inf, -%inf]);
//assert_checkequal(computed, ref);

