// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//// This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 12163 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12163
//
// <-- Short Description -->
//   unzoom does not work with an input argument

clf()
plot();
rect=[3 -2 7 10];
zoom_rect(rect); 
assert_checktrue(execstr("unzoom(gcf())","errcatch")==0);
delete();
