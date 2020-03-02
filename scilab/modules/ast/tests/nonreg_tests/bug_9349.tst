// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 9349 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/9349
//
// <-- Short Description -->
// Empty parentheses after a field in a macro led to a recurence problem

// No compilation error should occur:
clear a
function foo(), y = a.b(), endfunction
clear foo
function y=foo()
    a = mlist(['V' 'b'], mlist(['V']))
    y = a.b()
endfunction

obj = mlist(['A']);
deff('y=foo()', 'y=123');
deff('y=%A_e(b,a)', 'y=foo');
obj.toto()

function test()
    obj = mlist(['A']);
    deff('y=foo()','y=123');
    deff('y=%A_e(b,a)', 'y=foo');
    obj.toto()
endfunction
test()
