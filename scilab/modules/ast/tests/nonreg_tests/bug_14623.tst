// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - ESI - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 14623 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14623
//
// <-- Short Description -->
// wrong arng(1) in MList extration overload

//wrong argn(1) in MList overload
function varargout = %mymlist_e(varargin)
    varargout(1) = argn(1);
    varargout(2) = argn(2);
    for i = 3:argn(1)
        varargout(i) = i;
    end
endfunction

m = mlist("mymlist");


[res1, res2] = m(1);
assert_checkequal(res1, 2);
assert_checkequal(res2, 2);

[res1, res2, res3] = m(1);
assert_checkequal(res1, 3);
assert_checkequal(res2, 2);
assert_checkequal(res3, 3);

[res1, res2, res3, res4] = m(1);
assert_checkequal(res1, 4);
assert_checkequal(res2, 2);
assert_checkequal(res3, 3);
assert_checkequal(res4, 4);

[res1, res2, res3, res4, res5] = m(1);
assert_checkequal(res1, 5);
assert_checkequal(res2, 2);
assert_checkequal(res3, 3);
assert_checkequal(res4, 4);
assert_checkequal(res5, 5);
