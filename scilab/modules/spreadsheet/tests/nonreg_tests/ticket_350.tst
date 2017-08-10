// =============================================================================
// Copyright (C) 2010 - 2012 - INRIA - Michael BAUDIN
// =============================================================================
// <-- CLI SHELL MODE -->
// =============================================================================
// <-- Non-regression test for bug 350 -->
//
// <-- URL -->
//  http://forge.scilab.org/index.php/p/csv-readwrite/issues/350/
//
// <-- Short Description -->
// The csvStringToDouble function always returns complex doubles.
// =============================================================================
path = SCI+"/modules/spreadsheet/tests/unit_tests/";

r = csvStringToDouble("12");
assert_checkequal ( isreal(r) , %t );
// =============================================================================

