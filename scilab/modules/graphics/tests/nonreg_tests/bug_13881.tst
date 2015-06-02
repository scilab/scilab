// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 13881 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13881
//
// <-- Short Description -->
// datatipRemoveAll does nothing

x = linspace(0,1,100)';
clf()
plot(x,x.^3,"b",x,x.^2,"r");
e = gce();
e = e.children;
datatipCreate(e(1),50);
datatipCreate(e(2),20);
datatipRemoveAll(e);
assert_checktrue(size(e(1).datatips, "*")==0);
assert_checktrue(size(e(2).datatips, "*")==0);

clf
plot(x,x.^3,"b",x,x.^2,"r");
e = gce();
e = e.children;
datatipCreate(e(1),50);
datatipCreate(e(2),20);
datatipRemoveAll();
assert_checktrue(size(e(1).datatips, "*")==0);
assert_checktrue(size(e(2).datatips, "*")==0);
