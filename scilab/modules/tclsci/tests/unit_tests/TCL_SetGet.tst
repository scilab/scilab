// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- NO CHECK REF -->
//-----------------------------------------------------------------------------------------
// Tests Interface TCL
//-----------------------------------------------------------------------------------------
assert_checktrue(TCL_SetVar("ScalarA",1.1112));
assert_checkequal(TCL_GetVar("ScalarA"), "1.1112000000");

//-----------------------------------------------------------------------------------------
assert_checktrue(TCL_SetVar("StringB","Test"));
assert_checkequal(TCL_GetVar("StringB"), "Test");

//-----------------------------------------------------------------------------------------
b=[6.1,4.1,1.1;2.1,3.1,5.1];
stringb=["6.1000000000","4.1000000000","1.1000000000";"2.1000000000","3.1000000000","5.1000000000"];

assert_checktrue(TCL_SetVar("B",b));
assert_checkequal(TCL_GetVar('B(1,1)'), stringb(1,1));
assert_checkequal(TCL_GetVar('B(1,2)'), stringb(1,2));
assert_checkequal(TCL_GetVar('B(1,3)'), stringb(1,3));
assert_checkequal(TCL_GetVar('B(2,1)'), stringb(2,1));
assert_checkequal(TCL_GetVar('B(2,2)'), stringb(2,2));
assert_checkequal(TCL_GetVar('B(2,3)'), stringb(2,3));
assert_checkequal(TCL_GetVar("B"), stringb);

//-----------------------------------------------------------------------------------------
a=['A','B','C';'D','E','F']

assert_checktrue(TCL_SetVar("A",a));
assert_checkequal(TCL_GetVar('A(1,1)'), a(1,1));
assert_checkequal(TCL_GetVar('A(1,2)'), a(1,2));
assert_checkequal(TCL_GetVar('A(1,3)'), a(1,3));
assert_checkequal(TCL_GetVar('A(2,1)'), a(2,1));
assert_checkequal(TCL_GetVar('A(2,2)'), a(2,2));
assert_checkequal(TCL_GetVar('A(2,3)'), a(2,3));
assert_checkequal(TCL_GetVar("A"), a);
//-----------------------------------------------------------------------------------------

