// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

//==============================================================================
// blank unit tests
//==============================================================================

x = (["xxx" blanks(20) "yyy"]);
assert_checkequal(x, ["xxx" "                    " "yyy"]);

// Test 1
//LEN_MAX = 20000000;
LEN_MAX = 20000000;
r = blanks(LEN_MAX);
l = length(r);
assert_checkequal(l, LEN_MAX);

// Test 2
// check first and last characters

FIRST_CHAR = part(r,1);
LAST_CHAR = part(r,LEN_MAX);
assert_checkequal(FIRST_CHAR, " ");
assert_checkequal(LAST_CHAR, " ");

// Test 3

A = floor(abs(2^8  * rand()));
B = floor(abs(2^16 * rand()));
C = floor(abs(2^16 * rand()));

D = floor(abs(2^7  * rand()));
E = floor(abs(2^15 * rand()));
F = floor(abs(2^15 * rand()));

assert_checkequal(blanks(A), blanks(uint8(A)));
assert_checkequal(blanks(B), blanks(uint16(B)));
assert_checkequal(blanks(C), blanks(uint32(C)));

assert_checkequal(blanks(D), blanks(int8(D)));
assert_checkequal(blanks(E), blanks(int16(E)));
assert_checkequal(blanks(F), blanks(int32(F)));
