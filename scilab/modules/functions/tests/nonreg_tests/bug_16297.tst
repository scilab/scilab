// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - ESI Group - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- Non-regression test for bug 16297 -->
//
// <-- Bugzilla URL -->
// https://bugzilla.scilab.org/show_bug.cgi?id=16297
//
// <-- Short Description -->
// After function test(), e={}, endfunction; macr2tree(test)  crashes Scilab

function test()
    e = {};
    e = {1,2};
    e = {1,2;3,4};
endfunction
t=macr2tree(test);

result = ["function test()"; ...
  "e = {};"; ...
  "e = {1,2};"; ...
  "e = {1,2;3,4};"; ...
  "endfunction"; ...
  "" ];

assert_checkequal(tree2code(t), result);