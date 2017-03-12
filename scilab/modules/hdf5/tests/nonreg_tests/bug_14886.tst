// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - ESI - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 14886 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14886
//
// <-- Short Description -->
// save/load matplot failed ( mistake between rwo/x and cols/y )

Matplot()
f = gcf();
save(TMPDIR+"/Matplot.sod","f")
d0 = gca().children.data;
xdel()
load(TMPDIR+"/Matplot.sod")
d1 = gca().children.data;

assert_checkequal(size(d0), size(d1));
assert_checkequal(d0, d1);
