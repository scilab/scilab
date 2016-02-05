// =============================================================================
// Copyright (C) 2010 - 2012 - INRIA - Allan CORNET
// =============================================================================
// <-- CLI SHELL MODE -->
// =============================================================================
r_string = csvRead(SCI+"/modules/spreadsheet/" + "tests/nonreg_tests/bug_2.csv", ",", [], "string");
r_double = csvRead(SCI+"/modules/spreadsheet/" + "tests/nonreg_tests/bug_2.csv", ",", ".", "double");
// =============================================================================
assert_checkequal(size(r_string, "c"), size(r_double, "c"));
assert_checkequal(size(r_string, "r"), size(r_double, "r"));
assert_checkequal(size(r_string, "c"), 100);
assert_checkequal(size(r_string, "r"), 100);
assert_checkequal(r_string(100, 100), '1');
assert_checkequal(and(r_double == eye(100,100)), %T);
// =============================================================================
