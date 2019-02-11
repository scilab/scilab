// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Sylvestre LEDRU
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 7954 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/7954
//
// <-- Short Description -->
//
// # Bug present => it should freeze at some point

for i = 1:100
    x = string(rand(1));
    TCL_EvalStr("set a "+x);
    x2 = TCL_GetVar("a");
    assert_checkequal(x2, x);
end
