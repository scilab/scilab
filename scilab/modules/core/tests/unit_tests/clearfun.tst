//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//================================================
// test clearfun , funptr, newfun
//================================================
if clearfun('myfunctionname') <> %F then pause,end
//================================================
ptr = funptr('pwd');
newfun("MY_PWD",ptr);

assert_checktrue(clearfun("MY_PWD"));
//================================================
errmsg =  msprintf(gettext("%s: Wrong type for input argument #%d: String expected.\n"), "clearfun", 1);
assert_checkerror('a = clearfun(2);', errmsg);
