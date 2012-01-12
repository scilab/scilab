// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - pierre.lando@scilab.org
//
//// This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 2043 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2043
//
// <-- Short Description -->
//   The function copy applied to an axes handle does not work

plot3d();
f=gcf();
a=gca();
a1=copy(a);
assert_checkequal(size(f.children, '*'),2);
