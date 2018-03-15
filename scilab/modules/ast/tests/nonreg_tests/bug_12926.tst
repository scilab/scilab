// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 12926 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/12926
//
// <-- Short Description -->
// "~" had a priority lower than < <= => > == and ~=

assert_checkequal(~%f==1, (~%f)==1);
assert_checkequal(~%t==1, (~%t)==1);
assert_checkequal(~%f~=1, (~%f)~=1);
assert_checkequal(~%t~=1, (~%t)~=1);

assert_checktrue(execstr("~3<1","errcatch")~=0);
assert_checktrue(execstr("~3<=1","errcatch")~=0);
assert_checktrue(execstr("~3>1","errcatch")~=0);
assert_checktrue(execstr("~3>=1","errcatch")~=0);
