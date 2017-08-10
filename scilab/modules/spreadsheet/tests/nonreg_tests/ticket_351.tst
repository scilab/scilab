// =============================================================================
// Copyright (C) 2010 - 2012 - INRIA - Michael BAUDIN
// =============================================================================
// <-- CLI SHELL MODE -->
// =============================================================================
// <-- Non-regression test for bug 351 -->
//
// <-- URL -->
//  http://forge.scilab.org/index.php/p/csv-readwrite/issues/351/
//
// <-- Short Description -->
// The csvRead function always returns complex entries.
// =============================================================================
path = SCI+"/modules/spreadsheet/tests/unit_tests/";

r = csvRead(fullfile(path,"K_1.csv"), [], [], "double");
assert_checkequal ( isreal(r) , %t );
// =============================================================================

