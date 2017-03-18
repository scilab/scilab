// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15079 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15079
//
// <-- Short Description -->
// When all children of a graphic handle have not the same number of sub-children,
// any vectorized extraction or insertion in subchildren failed.

n2 = -1000:200:1000;
nS = -1000:200:1000;
[N2,NS] = ndgrid(n2, nS);
N1 = 2.6*N2 + (3.6)*NS;
clf
contour(n2 ,nS ,N1/100,[-60:10:60]);
ca = gca();
assert_checkequal(execstr("ca.children.children", "errcatch"), 0);
assert_checkequal(execstr("ca.children.children(2:2:$)", "errcatch"), 0);
assert_checkequal(ca.children.children(2:2:$).type, repmat("Polyline",12,1));
assert_checkequal(execstr("ca.children.children(2:2:$).foreground = color(''red'')", "errcatch"), 0);
assert_checkequal(ca.children.children(2:2:$).foreground, repmat(color('red'),12,1));
