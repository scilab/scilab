// =============================================================================
// Copyright (C) 2010 - 2012 - INRIA - Allan CORNET
// =============================================================================
// <-- CLI SHELL MODE -->
// =============================================================================
r1_string = csvRead(SCI+"/modules/spreadsheet/" + "tests/nonreg_tests/bug_4_longline_1.csv", ",", [], "string");
r2_string = csvRead(SCI+"/modules/spreadsheet/" + "tests/nonreg_tests/bug_4_longline_2.csv", " ", [], "string");
r3_string = csvRead(SCI+"/modules/spreadsheet/" + "tests/nonreg_tests/bug_4_shortline_1.csv", ",", [], "string");
r4_string = csvRead(SCI+"/modules/spreadsheet/" + "tests/nonreg_tests/bug_4_shortline_2.csv", " ", [], "string");
// =============================================================================
r1_double = csvRead(SCI+"/modules/spreadsheet/" + "tests/nonreg_tests/bug_4_longline_1.csv", ",", ".", "double");
r2_double = csvRead(SCI+"/modules/spreadsheet/" + "tests/nonreg_tests/bug_4_longline_2.csv", " ", ".", "double");
r3_double = csvRead(SCI+"/modules/spreadsheet/" + "tests/nonreg_tests/bug_4_shortline_1.csv", ",", ".", "double");
r4_double = csvRead(SCI+"/modules/spreadsheet/" + "tests/nonreg_tests/bug_4_shortline_2.csv", " ", ".", "double");
// =============================================================================
assert_checkequal(size(r1_string, "r"), 1 );
assert_checkequal(size(r1_string, "c"), 8 );
// =============================================================================
assert_checkequal(size(r2_string, "r"), 1 );
assert_checkequal(size(r2_string, "c"), 8 );
// =============================================================================
assert_checkequal(size(r3_string, "r"), 1 );
assert_checkequal(size(r3_string, "c"), 7 );
// =============================================================================
assert_checkequal(size(r4_string, "r"), 1 );
assert_checkequal(size(r4_string, "c"), 7 );
// =============================================================================
assert_checkequal(size(r1_string, "r"), size(r1_double, "r") );
assert_checkequal(size(r1_string, "c"), size(r1_double, "c") );
// =============================================================================
assert_checkequal(size(r2_string, "r"), size(r2_double, "r") );
assert_checkequal(size(r2_string, "c"), size(r2_double, "c") );
// =============================================================================
assert_checkequal(size(r3_string, "r"), size(r3_double, "r") );
assert_checkequal(size(r3_string, "c"), size(r3_double, "c") );
// =============================================================================
assert_checkequal(size(r4_string, "r"), size(r4_double, "r") );
assert_checkequal(size(r4_string, "c"), size(r4_double, "c") );
// =============================================================================
ref_longline_1 = ["-2.790381273096156e-07" , "3.794290569878876e-07" , ..
                  "1.102895012074256e-06"  , "1.632322210513841e-06" , ..
                  "1.777203879921258e-06"  , "1.486206657150863e-06" , ..
                  "8.635378410712134e-07"  , "1.322840799913625e-07"];
assert_checkequal(ref_longline_1, r1_string);
// =============================================================================
ref_longline_2 = ["-2.790381273096156e-07" , "3.794290569878876e-07" , ..
                  "1.102895012074256e-06"  , "1.632322210513841e-06" , ..
                  "1.777203879921258e-06"  , "1.486206657150863e-06" , ..
                  "8.635378410712134e-07"  , "1.322840799913625e-07"];
assert_checkequal(ref_longline_2, r2_string);
// =============================================================================
ref_shortline_1 = ["-2.790381273096156e-07" , "3.794290569878876e-07" , ..
                   "1.102895012074256e-06"  , "1.632322210513841e-06" , ..
                   "1.777203879921258e-06"  , "1.486206657150863e-06" , ..
                   "8.635378410712134e-07"];
assert_checkequal(ref_shortline_1, r3_string);
// =============================================================================
ref_shortline_2 = ["-2.790381273096156e-07" , "3.794290569878876e-07" , ..
                   "1.102895012074256e-06"  , "1.632322210513841e-06" , ..
                   "1.777203879921258e-06"  , "1.486206657150863e-06" , ..
                   "8.635378410712134e-07"];
assert_checkequal(ref_shortline_2, r4_string);
// =============================================================================
