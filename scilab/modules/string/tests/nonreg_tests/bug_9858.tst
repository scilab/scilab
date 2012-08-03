// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 9858 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9858
//
// <-- Short Description -->
// length() could not be overloaded with mlist
//

a = mlist(["myMlistT" "field1" "field2"],"aexample","bexampleb");
assert_checkequal(length(a), size(["myMlistT" "field1" "field2"],"*"));
assert_checkequal(length(a), 3);

// we overload length for myMlistT
function r = %myMlistT_length(M)
  r = length(M.field1)
endfunction

assert_checkequal(length(a), length(a.field1));
assert_checkequal(length(a.field1), 8);
