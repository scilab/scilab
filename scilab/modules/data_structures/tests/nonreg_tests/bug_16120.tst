// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16120 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16120
//
// <-- Short Description -->
// tree_show() was broken for containers including some cells

clear c s
c = makecell([1 2],"abc",%t);
assert_checkequal(execstr("tree_show(c)", "errcatch"), 0);
assert_checkequal(execstr("tree_show({})", "errcatch"), 0);
s.c = c;
assert_checkequal(execstr("tree_show(s)", "errcatch"), 0);
