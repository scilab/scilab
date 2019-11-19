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
// unit tests for isvector() function
// =============================================================================

assert_checkfalse(isvector(eye()));
assert_checkfalse(isvector([]));
assert_checkfalse(isvector(1));         // questionnable
assert_checktrue(isvector(1:3));
assert_checktrue(isvector((1:3)'));
assert_checktrue(isvector(cat(3,1,2,3)));
assert_checktrue(isvector(cat(5,1,2,3)));

// Sparse
assert_checkfalse(isvector(sparse(eye())));
assert_checkfalse(isvector(sparse([])));
assert_checkfalse(isvector(sparse(1)));  // questionnable
assert_checktrue(isvector(sparse(1:3)));
assert_checktrue(isvector(sparse((1:3)')));

// Cell
assert_checkfalse(isvector({}));
assert_checkfalse(isvector({1}));       // questionnable
assert_checktrue(isvector({1 2 3}));
assert_checktrue(isvector({1 ; 2 ; 3}));
assert_checktrue(isvector(cat(3,{1},{2},{3})));

// Struct
clear s
s.r = %pi;   assert_checkfalse(isvector(s));  // questionnable
s(2).r = %e; assert_checktrue(isvector(s));
clear
s(1,1,2).r = %e; assert_checktrue(isvector(s));

// Rationals : isvector always crashes
//assert_checkfalse(isvector(1/%s));         // questionnable
//assert_checktrue(isvector((1:3)./%s));
//assert_checktrue(isvector((1:3)'./%s));
//assert_checktrue(isvector(cat(3,1,2,3)./%s));
