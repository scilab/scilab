// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15607 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15607
//
// <-- Short Description -->
//
// 1) TCL_GetVar no longer worked for arrays
// 2) Running TCL_CreateSlave() froze Scilab
// 3) In a function, TCL_EvalStr("ScilabEval h=5") created h at the top level.

// 1)
a = ["A","B","C" ; "D","E","F"];
TCL_SetVar("A", a);
assert_checkequal(TCL_GetVar("A"), a);

// 2) Just run the following instructions.
//    If they freezes Scilab, the whole test will fail.
TCL_CreateSlave('TCLSlave');
b = [66,44,11 ; 22,33,55];
TCL_SetVar("B_slave", b, 'TCLSlave');
TCL_GetVar('B_slave','TCLSlave');
TCL_DeleteInterp('TCLSlave');

// 3) 
clear h
function xyz()
   TCL_EvalStr("ScilabEval h=5")
endfunction
xyz();
assert_checkfalse(isdef("h","l"));
