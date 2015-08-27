//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Simon MARCHETTO
//
// This file is distributed under the same license as the Scilab package.
// =============================================================================

path = SCI + "/modules/spreadsheet/tests/unit_tests/";

//  Not symetric range
r = csvRead(fullfile(path,"K_1.csv"), [], [], "double", [], [], [2 1 2 2] );
expected = [
0.10000000000000001 1.1000000000000001
];
assert_checkequal(r, expected);

// Read complex doubles
r = csvRead(fullfile(path,"K_1.csv"), [], [], "string", [], [], [2 1 3 2] );
expected = [
"0.10000000000000001" "1.1000000000000001"
"0.10000000000000001" "0.10000000000000001"
];
assert_checkequal(r, expected);

// Not symetric range
r = csvRead(fullfile(path,"K_1.csv"), [], [], "string", [], [], [2 1 2 2]);
expected = [
"0.10000000000000001" "1.1000000000000001"
];
assert_checkequal(r, expected);

// Inconsistent range: C2 < C1
instr = "r = csvRead(fullfile(path,""K_1.csv""), [], [], ""string"", [], [], [2 3 3 2]);";
refMsg = msprintf(gettext("%s: Wrong value for input argument #%d: Inconsistent range.\n"), "csvRead", 7);
assert_checkerror(instr, refMsg);

// Inconsistent range: R2 < R1
instr = "r = csvRead(fullfile(path,""K_1.csv""), [], [], ""string"", [], [], [3 1 2 2]);";
refMsg = msprintf(gettext("%s: Wrong value for input argument #%d: Inconsistent range.\n"), "csvRead", 7);
assert_checkerror(instr, refMsg);

// Non-integer indice
instr = "r = csvRead(fullfile(path,""K_1.csv""), [], [], ""string"", [], [], [2 1 1.5 2]);";
refMsg = msprintf(gettext("%s: Wrong value for input argument #%d: A matrix of double, with integer values, expected.\n"), "csvRead", 7);
assert_checkerror(instr, refMsg);

// Infinite indice
instr = "r = csvRead(fullfile(path,""K_1.csv""), [], [], ""string"", [], [], [2 1 %inf 2]);";
refMsg = msprintf(gettext("%s: Wrong value for input argument #%d: A matrix of double, with integer values, expected.\n"), "csvRead", 7);
assert_checkerror(instr, refMsg);

// Row right indice greater than actual number of rows (string) => truncate
r = csvRead(fullfile(path,"K_1.csv"), [], [], "string", [], [], [2 1 999 2]);
expected = [
"0.10000000000000001" "1.1000000000000001"
"0.10000000000000001" "0.10000000000000001"
];
assert_checkequal(r, expected);

// Column right indice greater than actual number of columns (string) => truncate
r = csvRead(fullfile(path,"K_1.csv"), [], [], "string", [], [], [2 1 3 999]);
expected = [
"0.10000000000000001" "1.1000000000000001"
"0.10000000000000001" "0.10000000000000001"
];
assert_checkequal(r, expected);

// Row or column left indice greater than actual number of rows (string) => error
refMsg = msprintf(gettext("%s: Range row or/and column left indice(s) out of bounds.\n"), "csvRead");
instr = "r = csvRead(fullfile(path, ""K_1.csv""), [], [], ""string"", [], [], [999 1 1000 2]);";
assert_checkerror(instr, refMsg);
instr = "r = csvRead(fullfile(path, ""K_1.csv""), [], [], ""string"", [], [], [1 999 2 1000]);";
assert_checkerror(instr, refMsg);

// Row right indice greater than actual number of rows (double) => truncate
r = csvRead(fullfile(path,"K_1.csv"), [], [], "double", [], [], [3 1 999 2]);
expected = [
0.10000000000000001 0.10000000000000001
];
assert_checkequal(r, expected);

// Column right indice greater than actual number of columns (double) => truncate
r = csvRead(fullfile(path,"K_1.csv"), [], [], "double", [], [], [2 2 3 999]);
expected = [
1.1000000000000001
0.10000000000000001
];
assert_checkequal(r, expected);

// Row or column left indice greater than actual number of rows (double) => error
refMsg = msprintf(gettext("%s: Range row or/and column left indice(s) out of bounds.\n"), "csvRead");
instr = "r = csvRead(fullfile(path, ""K_1.csv""), [], [], ""double"", [], [], [999 1 1000 2]);";
assert_checkerror(instr, refMsg);
instr = "r = csvRead(fullfile(path, ""K_1.csv""), [], [], ""double"", [], [], [1 999 2 1000]);";
assert_checkerror(instr, refMsg);

// r = csvRead(fullfile(path,"complexdata.csv"), [], [], "double", [], [], [2 1 3 2]);
// Fails due to ticket #360
// TODO : range of doubles complex
// TODO : Row indice larger than actual number of rows: complex double case
// TODO : Col indice larger than actual number of columns: complex double case
// =============================================================================

