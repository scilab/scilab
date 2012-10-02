// =============================================================================
// Copyright (C) 2010 - 2012 - INRIA - Michael BAUDIN
// =============================================================================
// <-- JVM NOT MANDATORY -->
// =============================================================================
// <-- Non-regression test for bug 353 -->
//
// <-- URL -->
//  http://forge.scilab.org/index.php/p/csv-readwrite/issues/353/
//
// <-- Short Description -->
// The csvRead function does not manage the range.
// =================================g============================================
path = SCI+"/modules/spreadsheet/tests/unit_tests/";

//
// Read only rows/columns in range
r = csvRead(fullfile(path,"K_1.csv"), [], [], "double" , [] , [], [2 1 3 2] );
expected = [
0.10000000000000001 1.1000000000000001
0.10000000000000001 0.10000000000000001
];
assert_checkequal ( r , expected );
//
//  Not symetric range
r = csvRead(fullfile(path,"K_1.csv"), [], [], "double" , [] , [], [2 1 2 2] );
expected = [
0.10000000000000001 1.1000000000000001
];
assert_checkequal ( r , expected );
//
// Read complex doubles
r = csvRead(fullfile(path,"K_1.csv"), [], [], "string" , [] , [], [2 1 3 2] );
expected = [
"0.10000000000000001" "1.1000000000000001"
"0.10000000000000001" "0.10000000000000001"
];
assert_checkequal ( r , expected );
//
// Not symetric range
r = csvRead(fullfile(path,"K_1.csv"), [], [], "string" , [] , [], [2 1 2 2] );
expected = [
"0.10000000000000001" "1.1000000000000001"
];
assert_checkequal ( r , expected );
//
// Inconsistent range: C2 < C1
instr = "r = csvRead(fullfile(path,""K_1.csv""), [], [], ""string"" , [] , [], [2 3 3 2] );";
refMsg = msprintf(gettext("%s: Wrong value for input argument #%d: Inconsistent range.\n"), "csvRead", 7);
assert_checkerror ( instr , refMsg );
//
// Inconsistent range: R2 < R1
instr = "r = csvRead(fullfile(path,""K_1.csv""), [], [], ""string"" , [] , [], [3 1 2 2] );";
refMsg = msprintf(gettext("%s: Wrong value for input argument #%d: Inconsistent range.\n"), "csvRead", 7);
assert_checkerror ( instr , refMsg );
//
// Non-integer indice
instr="r = csvRead(fullfile(path,""K_1.csv""), [], [], ""string"" , [] , [], [2 1 1.5 2] );";
refMsg = msprintf(gettext("%s: Wrong value for input argument #%d: A matrix of double, with integer values, expected.\n"), "csvRead", 7);
assert_checkerror ( instr , refMsg );
//
// Infinite indice
instr="r = csvRead(fullfile(path,""K_1.csv""), [], [], ""string"" , [] , [], [2 1 %inf 2] );";
refMsg = msprintf(gettext("%s: Wrong value for input argument #%d: A matrix of double, with integer values, expected.\n"), "csvRead", 7);
assert_checkerror ( instr , refMsg );
//
// Row indice larger than actual number of rows: string case
r = csvRead(fullfile(path,"K_1.csv"), [], [], "string" , [] , [], [2 1 999 2] );
expected = [
"0.10000000000000001" "1.1000000000000001"
"0.10000000000000001" "0.10000000000000001"
];
assert_checkequal ( r , expected );
//
// Column indice larger than actual number of columns: string case
r = csvRead(fullfile(path,"K_1.csv"), [], [], "string" , [] , [], [2 1 3 999] );
expected = [
"0.10000000000000001" "1.1000000000000001"
"0.10000000000000001" "0.10000000000000001"
];
assert_checkequal ( r , expected );
//
// Row indice larger than actual number of rows: double case
r = csvRead(fullfile(path,"K_1.csv"), [], [], "double" , [] , [], [3 1 999 2] );
expected = [
0.10000000000000001 0.10000000000000001
];
assert_checkequal ( r , expected );
//
// Column indice larger than actual number of columns: double case
r = csvRead(fullfile(path,"K_1.csv"), [], [], "double" , [] , [], [2 2 3 999] );
expected = [
1.1000000000000001
0.10000000000000001
];
assert_checkequal ( r , expected );

// r = csvRead(fullfile(path,"complexdata.csv"), [], [], "double" , [] , [], [2 1 3 2] );
// Fails due to ticket #360
// TODO : range of doubles complex 
// TODO : Row indice larger than actual number of rows: complex double case
// TODO : Col indice larger than actual number of columns: complex double case
// =============================================================================

