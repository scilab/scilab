// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 14379 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14379
//
// <-- Short Description -->
// Problem with lists of functions having 2 arguments

function [y1, y2] = F1(x)
    y1 = x + 1;
    y2 = x + 2;
endfunction

L = list(F1);
[y1, y2] = L(1)(1);

assert_checkequal(y1, 2);
assert_checkequal(y2, 3);


res = L(1)(1:10)(5);
assert_checkequal(res, 6);
