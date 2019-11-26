// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// ---------------------------------------------------------
// Unit tests for mixed {boolean, double} .* multiplications
// ---------------------------------------------------------

b = [%T %T %F ; %F %T %F];
spb = sparse(b);
d = [1 2 3; 4 5 6];
spd = sparse(d);
ref =  [1 2 0 ; 0 5 0];
spref = sparse(ref);

assert_checkequal(b .* d, ref);
assert_checkequal(d .* b, ref);

assert_checkequal(spb .* d, spref);
assert_checkequal(d .* spb, spref);
assert_checkequal(spb .* spd, spref);
assert_checkequal(spd .* spb, spref);
assert_checkequal(b .*spd, spref);
assert_checkequal(spd .* b, spref);

// -------------------------------------------------
// Unit tests for boolean .* boolean multiplications
// -------------------------------------------------

b =  [%T %T %F ; %F %T %F];
b2 = [%T %F %F ; %T %T %T];
spb = sparse(b);
ref =  [1 0 0 ; 0 1 0];
spref = sparse(ref);

assert_checkequal(b .* b2, ref);
assert_checkequal(spb .* b2, spref);
assert_checkequal(b2 .* spb, spref);
assert_checkequal(spb .* sparse(b2), spref);
