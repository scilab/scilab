// =============================================================================
// Copyright (C) 2010 - 2012 - INRIA - Allan CORNET
// =============================================================================
// <-- CLI SHELL MODE -->
// =============================================================================
// <-- Non-regression test for bug 245 -->
//
// <-- URL -->
//  http://forge.scilab.org/index.php/p/csv-readwrite/issues/245/
//
// <-- Short Description -->
// csvStringToDouble failed on some special cases.

path = SCI+"/modules/spreadsheet/tests/unit_tests/";

a = csvStringToDouble("3.14159e-10+inf");
assert_checkequal ( isnan(a) , %t );

assert_checkequal ( execstr("a = csvStringToDouble(""3.14159e-10+inf"", %f);", "errcatch") , 999 );
assert_checkequal ( lasterror() , msprintf(_("%s: can not convert data.\n"), "csvStringToDouble") );

a = csvStringToDouble("6+3*I");
assert_checkequal ( isnan(a) , %t );

assert_checkequal ( execstr("a = csvStringToDouble(""6+3*I"", %f);", "errcatch") , 999 );
assert_checkequal ( lasterror() , msprintf(_("%s: can not convert data.\n"), "csvStringToDouble") );
// =============================================================================
