// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 8245 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8245
//
// <-- Short Description -->
// gsort() did not allow overloading on not managed type

s.a = %pi;
assert_checkequal(execstr("gsort(s)","errcatch"), 999);

function r = %st_gsort(s)
    r = 666;
endfunction

assert_checkequal(execstr("r = gsort(s)","errcatch"), 0);
assert_checkequal(r, 666);
