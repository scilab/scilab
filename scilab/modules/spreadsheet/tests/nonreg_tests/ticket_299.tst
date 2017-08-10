// =============================================================================
// Copyright (C) 2010 - 2012 - INRIA - Allan CORNET
// =============================================================================
// <-- CLI SHELL MODE -->
// =============================================================================
// <-- Non-regression test for bug 299 -->
//
// <-- URL -->
//  http://forge.scilab.org/index.php/p/csv-readwrite/issues/299/
//
// <-- Short Description -->
// extends format to digit in csvDefault and csvWrite
// =============================================================================
assert_checkequal(csvDefault("precision"), "%.17lg");
assert_checkequal(csvDefault("precision","%.17lg") , %T);
 
assert_checkequal(csvDefault("precision",4), %T);
assert_checkequal(csvDefault("precision"), "%.4lg");
 
M = rand(2,3);
filename = fullfile(TMPDIR, "data.csv");
csvWrite(M, filename,[],[],"%.8e");
csvWrite(M, filename,[],[],8);
 
assert_checkequal(execstr("csvWrite(M, filename,[],[],19)", "errcatch"), 999);
// =============================================================================
