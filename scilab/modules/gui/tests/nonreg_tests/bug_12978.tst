// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 13053 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13053
//
// <-- Short Description -->
// datatipCreate did not return datatip handle.

clf();
x=linspace(-20,20,1000);
plot(x,sinc(x),x,sinc(x.^2));
e=gce();
e=e.children;
d1=datatipCreate(e(1),500);

assert_checkequal(typeof(d1), "handle");
assert_checkequal(d1.type, "Datatip");