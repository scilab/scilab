//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//================================================
// test funptr
//================================================
errmsg =  msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"), "funptr", 1);
assert_checkerror('ptr = funptr(1)', errmsg);

ptr = funptr("pwd");
assert_checkequal(ptr, "pwd");

ptr = funptr('FCT_NOT_IN_SCILAB');
assert_checkequal(ptr, 0);

//================================================
