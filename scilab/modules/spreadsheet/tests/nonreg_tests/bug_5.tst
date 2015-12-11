// =============================================================================
// Copyright (C) 2010 - 2012 - INRIA - Allan CORNET
// =============================================================================
// <-- CLI SHELL MODE -->
// =============================================================================
r1_string = csvRead(SCI+"/modules/spreadsheet/" + "tests/nonreg_tests/bug_5.csv", ",", [], "string");
r1_double = csvRead(SCI+"/modules/spreadsheet/" + "tests/nonreg_tests/bug_5.csv", ",", ".", "double");
// =============================================================================
assert_checkequal(size(r1_string, "c"), size(r1_double, "c"));
assert_checkequal(size(r1_string, "r"), size(r1_double, "r"));
// =============================================================================
assert_checkequal(size(r1_string, "c"), 1);
assert_checkequal(size(r1_string, "r"), 372);
// =============================================================================
assert_checkequal(r1_string(1), "0");
assert_checkequal(r1_double(1), 0);
// =============================================================================
assert_checkequal(r1_string(2), "-0");
assert_checkequal(r1_double(2), 0);
// =============================================================================
assert_checkequal(r1_string(3), "1e-308");
assert_checkequal(r1_double(3), 1e-308);
// =============================================================================
assert_checkequal(r1_string(4), "1e308");
assert_checkequal(r1_double(4), 1e308);
// =============================================================================
assert_checkequal(r1_string(372), "0.001236");
assert_checkequal(r1_double(372), 0.001236);
// =============================================================================
