// =============================================================================
// <-- CLI SHELL MODE -->
// =============================================================================
r = csvRead(SCI+"/modules/spreadsheet/" + "tests/nonreg_tests/emptyfields.csv", ";",[],"string");
assert_checkequal(r,["foo","bar","","","";"foo2","bar2","bar3","",""]);

r = read_csv(SCI+"/modules/spreadsheet/" + "tests/nonreg_tests/emptyfields.csv", ";",[]);
assert_checkequal(r,["foo","bar","","","";"foo2","bar2","bar3","",""]);
