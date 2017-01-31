// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- Non-regression test for bug 14976 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14976
//
// <-- Short Description -->
// asciimat(asciiNums) concatenated rows when asciiNums has only one column.
// asciimat(asciimat("àâãäéèêëìîïòôöùûü")) yielded an error.

assert_checkequal(asciimat(97:122), "abcdefghijklmnopqrstuvwxyz");
c = (97:122)';
r = strsplit("a":"z",1:25);
assert_checkequal(asciimat(c), r);
assert_checkequal(asciimat(cat(3,c,c)), [r r]);
assert_checkequal(asciimat(cat(7,c,c)), cat(6, r, r));

// With UTF8 (limited application)
t = "àâãäéèêëìîïòôöùûü";
assert_checkequal(asciimat(asciimat(t)), t);
assert_checkequal(asciimat(asciimat([t t])), t+t);
assert_checkequal(asciimat(asciimat([t;t])), [t;t]);
