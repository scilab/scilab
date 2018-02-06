// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - ESI - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 14573 -->
//
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14573
//
// <-- Short Description -->
// varargout misworked when it was used in addition to fixed output arguments.

//1 var + varargout
function [res, varargout] = test(a)
  res = "abc";
  varargout = list(%i);
endfunction

r = test();
assert_checkequal(r, "abc");

r = test(4.321);
assert_checkequal(r, "abc");

[r1, r2] = test(%pi);
assert_checkequal(r, "abc");
assert_checkequal(r2, %i);

err = execstr("[o1, o2, o3] = test(%pi);", "errcatch");
assert_checkfalse(err == 0);


//2 vars + varargout
function [res1, res2, varargout] = test2(a)
  res1 = "abc";
  res2 = "def";
  varargout = list(%i);
endfunction

r = test2();
assert_checkequal(r, "abc");

r = test2(4.321);
assert_checkequal(r, "abc");

[r1, r2] = test2(%pi);
assert_checkequal(r1, "abc");
assert_checkequal(r2, "def");

[r1, r2, r3] = test2(%pi);
assert_checkequal(r1, "abc");
assert_checkequal(r2, "def");
assert_checkequal(r3, %i);

err = execstr("[r1, r2, r3, r4] = test2(%pi);", "errcatch");
assert_checkfalse(err == 0);
