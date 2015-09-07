// =============================================================================
// Copyright (C) 2010 - 2012 - INRIA - Michael BAUDIN
// =============================================================================
// <-- CLI SHELL MODE -->
// =============================================================================
// <-- Non-regression test for bug 298 -->
//
// <-- URL -->
//  http://forge.scilab.org/index.php/p/csv-readwrite/issues/281/
//
// <-- Short Description -->
// The text_scan function does not extract the correct range.
// =============================================================================
path = SCI+"/modules/spreadsheet/tests/unit_tests/";
//
// Read only rows/columns in range
Astr = [
"1,8,15,22,29,36,43,50"
"2,9,16,23,30,37,44,51"
"3,10,17,6,31,38,45,52"
"4,11,18,25,32,39,46,53"
"5,12,19,26,33,40,47,54"
"6,13,20,27,34,41,48,55"
"+0,-0,Inf,-Inf,Nan,1.D+308,1.e-308,1.e-323"
];
A = csvTextScan ( Astr,[],[],"double",[2 3 5 6]' );
expected = [
 16 23 30 37
 17 6  31 38
 18 25 32 39
 19 26 33 40
];
assert_checkequal ( A , expected );
//
// Extract range as string.
A = csvTextScan ( Astr,[],[],"string",[2 3 5 6]' );
expected = [
"16","23","30","37";
"17","6","31","38";
"18","25","32","39";
"19","26","33","40"
];
assert_checkequal ( A , expected );
//
// Extract a non-symetrical range.
A = csvTextScan ( Astr,[],[],"string",[2 3 5 5]' );
expected = [
"16","23","30";
"17","6","31";
"18","25","32";
"19","26","33"
];
assert_checkequal ( A , expected );
//
// Extract a non-symetrical range.
A = csvTextScan ( Astr,[],[],"double",[2 3 5 5]' );
expected = [
16 23 30
17  6 31
18 25 32
19 26 33
];
assert_checkequal ( A , expected );
// =============================================================================
