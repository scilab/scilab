// ===================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// ===================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 6434 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/6434
//
// <-- Short Description -->
// Calls like libname.Macro() worked only with one input argument.

// 0 argin
assert_checktrue(type(corelib.getshell())==10);
assert_checkequal(corelib.getshell(), getshell());

// 2 argins
assert_checkequal(execstr("elementary_functionslib.atand(1,0)", "errcatch"), 0);
assert_checkequal(elementary_functionslib.atand(1,0), 90);

// 3 argins
assert_checkequal(execstr("elementary_functionslib.linspace(0,1,3)", "errcatch"), 0);
assert_checkequal(elementary_functionslib.linspace(0,1,3), linspace(0,1,3));
