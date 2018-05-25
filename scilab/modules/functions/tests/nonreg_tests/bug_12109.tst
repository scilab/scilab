// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 12109 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/12109
//
// <-- Short Description -->
// execstr() of a function including a return(var) before assigning the argout
// crashed Scilab

function y = foo(x)
    txt_foo = 'x est à  '+string(x)
    txt_sci = return(txt_foo)
    y = 1 + x^2
endfunction

assert_checkfalse(execstr("execstr(""foo(1)"", ""errcatch"")", "errcatch")~=0);
