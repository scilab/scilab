// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 11055 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11055
//
// <-- Short Description -->
// 'string' function did not manage implicit size type
//

A = 1:1:$;
REF_STR = "1:1:$";
assert_checkequal(string(A), REF_STR);

A = -1:1:$;
REF_STR = "-1:1:$";
assert_checkequal(string(A), REF_STR);

A = 1:$;
REF_STR = "1:1:$";
assert_checkequal(string(A), REF_STR);

A = 1:2:$;
REF_STR = "1:2:$";
assert_checkequal(string(A), REF_STR);

A = -1:-2:-$;
REF_STR = "-1:-2:-$";
assert_checkequal(string(A), REF_STR);
