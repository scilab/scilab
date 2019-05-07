// =============================================================================
// Copyright (C) 2012 - INRIA - Allan CORNET
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// =============================================================================
// <-- Non-regression test for bug 472 -->
//
// <-- URL -->
//  http://forge.scilab.org/index.php/p/csv-readwrite/issues/472/
//
// <-- Short Description -->
// csvRead, csvTextScan cannot ignore blank lines.
// =============================================================================

txt = ["my data 1";"";"my data 2"];
ref_res = ["my data 1";"my data 2"];

assert_checkequal(csvDefault('blank'), 'on');
assert_checkequal(csvDefault('blank','off'), %t);
res = csvTextScan(txt, [] , [], "string");
assert_checkequal(res, txt); // handle empty lines

assert_checkequal(csvDefault('blank','on'), %t);
res = csvTextScan(txt, [] , [], "string");
assert_checkequal(res, ref_res);
// =============================================================================

assert_checkequal(csvDefault('blank','off'), %t);
